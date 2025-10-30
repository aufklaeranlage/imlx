#include "imlx.h"

#include <unistd.h>
#include <errno.h>

int main(void) {
	/* These three variables are the basis of any program using the imlx.
	 * The session struct keeps track of the other variables for easy cleanup.
	 * We still need the window and image pointers to refer to them directly.
	 * We could later grab them by their name but it is easier to save them
	 * directly as pointers here.
	*/
	t_session	s;
	t_win		*w;
	t_img		*i;

	/* We pass a pointer to our t_session struct to the session_init function
	 * so it can initialize all it's internals to 0 after calling mlx_init.
	 * If mlx_init fails this function returns 'false'. In this case we didn't
	 * manage to create a connection to the X Session.
	*/
	if (!session_init(&s))
		return (1);
	/* Next we want to add a window and an image to the session, in both cases
	 * we save the return value from their respective fucntions in the variables
	 * If these fucntions fail the pointers to the structs will be NULL since
	 * initialization failed. There can be multuple issues why that's the case.
	 * If you need to know set errno to 0 before calling them and check it after
	 * you recieved a NULL pointer in the variable to get more information.
	 * Since windows and images are always associated with a session in the imlx
	 * we need to provide a pointer to the session, so it get's updated, saves
	 * the pointer for later cleanup and gives the image and window a pointer to
	 * itself for easier access.
	*/
	errno = 0;
	w = add_win(&s, 360, 250, "This is the window title");
	if (w == NULL)
		return (errno);
	errno = 0;
	i = add_img(&s, 360, 250, 0);
	if (w == NULL)
		return (errno);
	/* Once the initialization has worked we can put the image to the window to
	 * display something else than just a black window.
	 * In this case we created an image that is exactly as big as our window so
	 * we just put it into the window with an offset of 0 on the x (horizontal)
	 * Axis and 0 on the y (vertical) Axis.
	*/
	put_img(i, w, 0, 0);
	/* In this case the window is still bacl though. That is the case because we
	 * didn't modify the image yet. It still is a blank canvas. So for now wait
	 * for a couple of seconds and end the session.
	*/
	sleep(2);
	/* session_end does a lot of cleanup work in the background, it frees it's
	 * pointers and all of the images and windows associated with it, always
	 * call it at the end of a session to properly clean up and avoid memory
	 * leaks.
	*/
	session_end(&s);
	return (0);
}
