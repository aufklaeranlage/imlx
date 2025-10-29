#include "graphics.h"

#include "mlx.h"

static void	img_put_pxl(t_img *i, int x, int y, int color) {
	char *dst;

	dst = i->addr + (y * i->linesize + x * (i->bpp / 8));
	*(unsigned int *)dst = color;
}

void	scr_put_tup(t_scr *s, int x, int y, const t_tup *a) {
	if (x < 0 || y < 0 || x >= s->win->w || y >= s->win->h)
		return ;
	img_put_pxl(s->img, x, y, tup2col(a));
}
