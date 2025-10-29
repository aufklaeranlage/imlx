#include "graphics.h"

#include "mlx.h"

#include <stdlib.h>
#include <stdbool.h>

// Initializes a new image inside the t_scr struct 's'. Fails on a failed malloc
// (check errno), an image already existing (check s->img not being NULL) or 's'
// not having an associated window (check s->win being NULL).
// Returns true on success and flase on error.
bool	scr_img_new(t_scr *s) {
	if (s->img != NULL || s->win == NULL)
		return (false);
	s->img = malloc(sizeof(t_img));
	s->img->ptr = mlx_new_image(s->cid, s->win->w, s->win->h);
	if (s->img->ptr == NULL)
		return (free(s->img), s->img = NULL, false);
	s->img->addr = mlx_get_data_addr(s->img->ptr, &s->img->bpp,
		&s->img->linesize, &s->img->endian);
	return (true);
}

// Serves the image in 's' to the window in 's'.
// Fails if the image or the window haven't been initialized yet (scr_win_new()
// and scr_img_new() need to be called first).
// Returns true on success and flase on error.
bool	scr_serve(t_scr *s) {
	if (s->img == NULL || s->win == NULL)
		return (false);
	mlx_put_image_to_window(s->cid, s->win->ptr, s->img->ptr, 0, 0);
	return (true);
}

// Destroys the image in 's'
// Fails if no image is associated with 's'.
// Returns true on success and false on failure.
bool	scr_img_dest(t_scr *s) {
	if (s->img == NULL)
		return (false);
	mlx_destroy_image(s->cid, s->img->ptr);
	free(s->img);
	s->img = NULL;
	return (true);
}
