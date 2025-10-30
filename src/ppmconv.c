#include "imlx.h"

#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

static void	__imlx_write_uitoa(int fd, char buf[10], uint32_t n);

static void	put_colr(int fd, uint32_t colr);

static void	__imlx_write_colr(int fd, char buf[3], unsigned char c);

/*! \fn bool img_2ppm(t_img *img, const char *filename)
 * 	\brief Saves the image \a img in the file \a filename.
 *
 * 	\param img The image to be saved.
 * 	\param filename The name of the file the image should be saved to.
 *
 *	Tries to create the file \a filename with the flags OWRONLY | O_TRUNC | 
 *	O_CREAT if the call to open() fails the function returns.
 *	Otherwise it will create a ppm file based on the information saved in
 *	\a img. While adhering to certain standards (not more than 70 characters per
 *	line, maximum or 255 color value). Uses the rgb values stored in the XImage
 *	associated with \a img.
 *
 *	\return True on success, fals on failure.
 *
 * 	\warning This uses internal fucntions that assume the size of the image to
 * 	\warning be non negative, and at max a int32_t if by any chance you modfied
 * 	\warning the values inside the image concerning the width to be negative
 * 	\warning the output will be wrong for the size description of the ppm file
 * 	\warning and if it is bigger than a int32_t it migth segfault.
 * 	\warning It has some internal protection, use at own risk and watch out
 * 	\warning during image creation.
 */
bool	img_2ppm(t_img *img, const char *filename) {
	int				fd;
	char			tmp[10];	// 2147483647 has 10 digits
	uint32_t		x;
	uint32_t		y;

	fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0744);
	if (fd == -1)
		return (false);
	write(fd, "P3\n", 3);
	__imlx_write_uitoa(fd, tmp, img->w);
	write(fd, " ", 1);
	__imlx_write_uitoa(fd, tmp, img->h);
	write(fd, "\n", 1);
	__imlx_write_uitoa(fd, tmp, 255);
	write(fd, "\n", 1);
	y = 0;
	while (y < img->h) {
		x = 0;
		while (x < img->w) {
			put_colr(fd, *(uint32_t *)(img->addr + (y * img->ls + x * (img->bpp / 8))));
			++x;
			if (x % 5 == 0)
				write(fd, "\n", 1);
			else
				write(fd, " ", 1);
		}
		write(fd, "\n", 1);
		++y;
	}
	close(fd);
	return (true);
}

static void	__imlx_write_uitoa(int fd, char buf[10], uint32_t n) {
	uint32_t	i;

	i = 0;
	while (i < 10)
	{
		buf[i++] = n % 10 + '0';
		n /= 10;
		if (n == 0)
			break ;
	}
	while (i > 0) {
		--i;
		write(fd, &buf[i], 1);
	}
	
}

static void	put_colr(int fd, uint32_t colr) {
	char	cbuf[3];
	char	space;
	
	space = ' ';
	__imlx_write_colr(fd, cbuf, (unsigned char)(colr >> 16));
	write(fd, &space, 1);
	__imlx_write_colr(fd, cbuf, (unsigned char)(colr >> 8));
	write(fd, &space, 1);
	__imlx_write_colr(fd, cbuf, (unsigned char)(colr));
}

static void	__imlx_write_colr(int fd, char buf[3], unsigned char c) {
	uint32_t	i;

	memset(buf, ' ', 3);
	i = 0;
	while (i < 3)
	{
		buf[i++] = c % 10 + '0';
		c /= 10;
		if (c == 0)
			break ;
	}
	i = 3;
	while (i > 0) {
		--i;
		write(fd, &buf[i], 1);
	}
}
