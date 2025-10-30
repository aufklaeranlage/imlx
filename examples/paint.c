#include "imlx.h"

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(void) {
	/* We're gonna skip the instructions from the previous lesson in this one
	 * and instead focus on adding some color.
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
	/* This time we wanna modify our image before we put it to the window.
	 * The image stores RGB values in the range from 0 to 255 one character per
	 * value in a uint32_t. You can construct this integer by calling rgb2uint32
	 * or simply using an integer for example in a hex format.
	 * In this example 0x00RRGGBB the Rs are where the red color value is
	 * stored, the Gs are where the green color value is stored and the Bs are
	 * for the blue color value. All of them can hold vlaues from 00 to FF.
	 * Let's create a beautiful random image by calling rand a bunch of times
	 * for our positions and colors.
	*/
	for (int j = 0; j < 1000; j++) {
		int x, y;
		unsigned char	r, g, b;

		/* We need to limit our positions the the size of our image. The image
		 * is wokrign with regular array indexing, since we created it with a
		 * size of 360 x 250 the x value can be 0 to 359 and the y value can be
		 * 0 to 249.
		*/
		x = rand() % 360;
		y = rand() % 250;
		/* As previously described the valid values for colors are between 0 and
		 * 255 so we need to limit the rand output here too.
		*/
		r = rand() % 255;
		g = rand() % 255;
		b = rand() % 255;
		/* Since we don't want to spend time with checking the valid placement
		 * of a pixel everytime we call put_pixel the function assumes that you
		 * pass it values in bound for the image. If you lost track of the size
		 * of an image you can always access it's width and height with the
		 * variables w and h repectively inside the struct.
		*/
		put_pixel(i, x, y, rgb2uint32(r, g, b));
	}
	put_img(i, w, 0, 0);
	sleep(2);
	session_end(&s);
	return (0);
}
