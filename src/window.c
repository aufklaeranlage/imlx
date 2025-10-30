#include "imlx.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static int	win_queue_dest(t_win *ptr);

/*!	\fn bool add_win(t_session *s, int w, int h, const char *title)
 *	\brief Adds a new window to the session and updates internals.
 * 
 * 	\param s A pointer to the session the new window is supposed to be created
 * 	for.
 * 	\param w The width of the window.
 * 	\param h The height of the window.
 * 	\param title A string that describes the title.
 *
 * 	Checks if the session already has a window with the titel \a title with
 * 	get_win(). If a window with that title already exists returns NULL and does
 * 	nothing.
 *	Allocates space for a new t_win struct and an array of t_win structs as long
 *	as the number of windows associated with the session plus one to have room
 *	for the new window. It also duplicates the title string 
 *	If any of these allocations fail it will return NULL after freeing the
 *	already allocated memory.
 *	After the allocation it calles mlx_new_window() and saves the pointer in the
 *	new t_win struct. If the call to mlx_new_window() fails it returns NULL
 *	after freeing the already allocated memory().
 *	After the succesfull call to mlx_new_window() it finishes initialization on
 *	the window struct, adds the already existing windows to the new array as
 *	well as the new window, frees the old array and saves the new array in it's
 *	place.
 *	
 *	This function also creates a hook for the window to queue itself for
 *	destructiion once it's close button is pressed.
 *
 * 	\returns A pointer to the new window on success, and NULL on failure.
 *
 * 	\warning Title needs to be a properly terminated string since it is handed
 * 	\warning to get_win() which uses strcmp() and is copied for the t_win struct
 * 	\warnign with strdup().
 */
t_win	*add_win(t_session *s, uint32_t w, uint32_t h, const char *title) {
	t_win		*tmp;
	t_win		**tmparr;
	uint32_t	i;

	if (get_win(s, title))
		return (NULL);
	tmp = malloc(sizeof(*tmp));
	tmparr = malloc((s->numwin + 1) * sizeof(*tmparr));
	if (tmp == NULL || tmparr == NULL)
		return (free(tmp), free(tmparr), NULL);
	tmp->title = strdup(title);
	if (tmp->title == NULL)
		return (free(tmp), free(tmparr), NULL);
	tmp->ptr = mlx_new_window(s->cid, w, h, tmp->title);
	if (tmp->ptr == NULL)
		return (free(tmp->title), free(tmp), free(tmparr), NULL);
	tmp->w = w;
	tmp->h = h;
	tmp->s = s;
	tmp->destroy = false;
	i = 0;
	while (i < s->numwin)
	{
		tmparr[i] = s->win[i];
		++i;
	}
	tmparr[i] = tmp;
	++s->numwin;
	free(s->win);
	s->win = tmparr;
	mlx_hook(tmp->ptr, ON_DESTROY, 0, win_queue_dest, tmp);
	return (tmp);
}

/*!	\fn t_win *get_win(const t_session *s, const char *title)
 * 	\brief Checks the session pointed to by \a s for a window with the title
 * 	\brief \a title.
 *
 * 	\param s A pointer to the t_session struct to be searched.
 * 	\param title The title of the window to look for.
 *
 *	Iterates over the windows inside the session and returns the one with a
 *	matching title. If none is found it returns a NULL pointer.
 *
 *	\return A pointer to the t_win struct with a matching name if one could be
 *	found or NULL if no matching t_win struct could be found.
 *
 * 	\warning Does comparisons with strcmp(). If a not properly terminated
 * 	string is given as the title this is likely to segfault.
 */ 
t_win	*get_win(const t_session *s, const char *title) {
	uint32_t	i;

	i = 0;
	while (i < s->numwin)
	{
		if (!strcmp(title, s->win[i]->title))
			return (s->win[i]);
		++i;
	}
	return (NULL);
}

/*!	\fn bool win_clear(t_win *w)
 * 	\brief Clears a window (set's it to black)
 *
 * 	\param w A pointer ot the t_win struct that should be cleared.
 *
 *	Wrapped call to mlx_clear_window().
 *
 * 	\return At the moment always returns true
 */
bool	win_clear(t_win *w) {
	mlx_clear_window(w->s->cid, w->ptr);
	return (true);
}

/*!	\fn bool win_dest(t_win *w)
 * 	\brief Destroys a window and updates internals.
 *
 * 	\param w A pointer ot the t_win struct that should be destroyed.
 *
 *	Moves the windows stored in the window array of the associated session
 *	together, skipping the one pointed to by w. If the pointer \a w is not
 *	present in the associated session returns false.
 *	If the window is present it decreases the \a numwin variable for the
 *	associated session and calls mlx_destroy_window() on the window, frees the
 *	title string and the t_win struct itself.
 *
 * 	\return True on success, false on failure.
 */
bool	win_dest(t_win *w) {
	uint32_t	i;

	i = 0;
	while (i < w->s->numwin && w->s->win[i] != w)
		++i;
	if (i == w->s->numwin)
		return (false);
	while (i + 1 < w->s->numwin)
	{
		w->s->win[i] = w->s->win[i + 1];
		++i;
	}
	w->s->win[i] = NULL;
	--w->s->numwin;
	mlx_destroy_window(w->s->cid, w->ptr);
	free(w->title);
	free(w);
	return (true);
}

static int	win_queue_dest(t_win *ptr) {
	ptr->destroy = true;
	return (0);
}
