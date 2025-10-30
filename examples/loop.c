#include "imlx.h"

#include <stdlib.h>
#include <errno.h>

static void	paint_the_sky(t_img *img);

static int	loop(vodi *);

int main(void) {
	/* That was a nice image right? Almost looke like a starry sky, but we only
	 * got to see it for such a short time. So why not give ourself some inter-
	 * activity and keep the window open for as long as we want?
	*/
	t_session	s;
	t_win		*w;
	t_img		*i;

	if (!session_init(&s))
		return (1);
	errno = 0;
	w = add_win(&s, 360, 250, "This is the window title");
	if (w == NULL)
		return (errno);
	errno = 0;
	i = add_img(&s, 360, 250, 0);
	if (w == NULL)
		return (errno);
	paint_the_sky(i);
	put_img(i, w, 0, 0);
	/* Everythign is set up, the image is painter, it is put onto the window
	 * we are ready to look at it. This time we are not using sleep though.
	 * Instead we need to call a function named mlx_loop. This function is
	 * not implmented by libimlx but by the MinilibX. It does exactly what
	 * the name promises and keeps the session in a loop.
	 * Inside this loop there are hooks that we can set for windows. In fact
	 * libimlx already set's one of these for each window. Take a look at the
	 * code for add_win to see what happens there.
	 * Before we start the loop we want to implement a loop hook though. This
	 * is being called with each loop iteration. To this hook you can pass ever-
	 * ything you would like. It just needs a function to be called and a
	 * pointer to the data it should use. In our case we want to pass it a
	 * pointer to our session struct.
	*/
	mlx_loop_hook(loop, &s);
	/*
	 * mlx_loop takes a single argument and the is the X Session that is stored
	 * inside our session struct. Simply pass the cid variable inside it to
	 * mlx_loop and the window should stay open until we press the 'close'
	 * button for the window (That's the hook libimlx adds for each window it
	 * creates btw).
	 * To check how we close them go to the function loop further down.
	*/
	mlx_loop(s.cid);
	session_end(&s);
	return (0);
}

static void	paint_the_sky(t_img *img) {
	int x, y;
	unsigned char	r, g, b;

	for (int i = 0; i < 1000; i++) {
		x = rand() % 360;
		y = rand() % 250;
		r = rand() % 255;
		g = rand() % 255;
		b = rand() % 255;
		put_pixel(img, x, y, rgb2uint32(r, g, b));
	}
}

static int loop(void *ptr) {
	/* Technically we could accept the pointer to a session struct directly
	 * but i want to drive home that you could put anyhting under the pointer
	 * passed to (and through) a hook function. For example you could have a
	 * separate data struct that has a pointer to the session struct.
	*/
	t_session *s;

	s = (t_session *)ptr;
	/* The MinilibX has a section of code in it's implementation of the mlx_loop
	 * setup that checks the ON_DESTROY mask of a window (that's triggered by
	 * the button to close a window) before checking the same struct again for
	 * all the other XEvent masks. If we would destroy the window once the
	 * button is clicked and the event is triggered, it would result in the
	 * program trying to access already freed memory. To prevent this the
	 * windows instead set their 'destroy' variable to true once the ON_DESTROY
	 * event for them is triggered. sesion_clean is responsible for iterating 
	 * over all windows inside the session struct and check if they are queued
	 * to be destroyed. If so it destroys them, frees their pointers etc.
	 *
	 * Once all windows inside a session are destroyed the MinilibX realizes
	 * this and stops the loop itself. More on this later.
	*/
	session_clean(s);
}
