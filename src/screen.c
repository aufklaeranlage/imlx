#include "graphics.h"

#include "mlx.h"

#include <stddef.h>

// Initializes the t_scr struct pointed to by 's'
bool	scr_init(t_scr *s) {
	s->win = NULL;
	s->img = NULL;
	s->cid = mlx_init();
	if (s->cid == NULL)
		return (false);
	return (true);
}

// Destroys a t_scr struct and it's associated window (if present)
bool	scr_dest(t_scr *s) {
	if (s->img != NULL)
	 	scr_img_dest(s);
	if (s->win != NULL)
		scr_win_dest(s);
	return (true);
}
