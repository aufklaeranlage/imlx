#include "imlx.h"

#include "mlx.h"
#include <stdint.h>

/*!	\fn void put_pixel(t_img *i, int x, int y, uint32_t color)
 * 	\brief Changes the data inside the image to be color at the coordinates 
 * 	\brief \a x , \a y .
 *
 * 	\param i A pointer to the image that should be changed.
 *	\param x The x part (horizontal) the color should be put to.
 *	\param y The y part (vertical) the color should be put to.
 *	\param color The color that should be put at the coordinates.
 *
 *	Gets the address of the images data and changes the color at the x and y
 *	coordinates to the specified color. It uses internally stored variables to
 *	find the proper position.
 *
 *	\warning Doesn't check that x and y are in the bounds for the image. You
 *	can check that as needed by comparing x and y to 0 and the repective limit
 *	of the image (\a x < i->w and \a y < i->h).
 */ 
void	put_pixel(t_img *i, uint32_t x, uint32_t y, uint32_t color) {
	char *dst;

	dst = i->addr + (y * i->ls + x * (i->bpp / 8));
	*(uint32_t *)dst = color;
}

/*!	\fn uint32_t rgb2uint32(char r, char g, char b)
 * 	\brief Converts the RGB values into a unified uint32_t that is supposed to
 * 	\brief be used with the put_pixel function.
 *
 * 	Bitshifts red by 16 to the left, g by 8 and b by 0 and combines them into a
 * 	single uin32_t.
 *
 * 	\return Returns the combined colors.
 *
 * 	\warning don't use negative character, left shifing them could result in
 * 	undefined behavior.
*/
uint32_t	rgb2uint32(char r, char g, char b) {
	uint32_t	colr;

	colr = r << 16 | g << 8 | b;
	return (colr);
}
