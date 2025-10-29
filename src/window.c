#include "graphics.h"

#include <stdlib.h>
#include <stdbool.h>

int	stop_loop(int keycode, void *ptr);

// Initializes a new window in the screen pointed to by s with the width 'w' and
// the height 'h'.
// Returns false if the allocation failed or the screen already has an
// associated window. In case of a false return check the 'win' value inside the
// screen struct to see which failure to set a new window it was.
// The allocation might also fail because of malloc so check errno for the
// appropriate errors.
// Does not perform a safety check on 's' use with already initialized struct.
bool	scr_win_new(t_scr *s, int w, int h, char *title) {
	if (s->win != NULL)
		return (false);
	s->win = malloc(sizeof(t_win));
	if (s->win == NULL)
		return (false);
	s->win->ptr = mlx_new_window(s->cid, w, h, title);
	if (s->win->ptr == NULL)
		return (free(s->win), s->win = NULL, false);
	s->win->w = w;
	s->win->h = h;
	mlx_hook(s->win, 2, 1L << 0, &stop_loop, (void *)s); // 02 = KeyPress, 1L << 0 KeyPressMask
	return (true);
}

// Clears the window in scr (sets it to black)
bool	scr_win_clear(t_scr *s) {
	mlx_clear_window(s->cid, s->win->ptr);
	return (true);
}

// Destroys the window in 's'
// Fails if no window is associated with 's'.
// Returns true on success and false on failure.
bool	scr_win_dest(t_scr *s) {
	if (s->win == NULL)
		return (false);
	mlx_destroy_window(s->cid, s->win->ptr);
	free(s->win);
	s->win = NULL;
	return (true);
}

int	stop_loop(int keycode, void *ptr) {
	if (keycode == 9)
		return (mlx_loop_end(((t_scr *)ptr)->cid), 0);
	return (1);
}
