#include "imlx.h"

#include "mlx.h"

#include <stddef.h>
#include <stdlib.h>

/*! \fn	bool session_init(t_session *s)
 *	\brief Initializes the t_session struct pointed to by \a s.
 *
 *	\param s A pointer to the t_session struct to be initialized.
 *	
 *	Creates a minilibX session by calling mlx_init() and storing the returned
 *	connection identifier in the \a cid variable of the t_session struct
 *	pointed to by \a s. After success it initializes the other variables inside
 *	the struct to 0 or NULL respectively.
 *	If the call to mlx_init() fails the struct is left unchanged except that the
 *	\a cid variable is set to NULL and false is returned.
 *
 *	\return True on success, false on failure to initialize \a s.
 *
 *	\warning The pointer \a s is not checked for being valid and is expected
 *	\warning to be allocated either on the stack or the heap.
 */
bool	session_init(t_session *s) {
	s->cid = mlx_init();
	if (s->cid == NULL)
		return (false);
	s->win = NULL;
	s->numwin = 0;
	s->img = NULL;
	s->numimg = 0;
	return (true);
}

/*! \nf bool session_end(t_session *s)
 * 	\brief Ends a session and destroys all windows and images associated with
 * 	\brief the session.
 *
 * 	\params s A pointer to the t_session struct that holds the information about
 * 	the session to be ended.
 *
 *	Loops through all the windows and images inside the session, destroys them,
 *	calls mlx_destroy_display() on the connection identifier, frees it and sets
 *	it to zero.
 *
 * 	\return At the moment always returns true.
 * 	\warning The pointer \a s is not checked for being valid.
 * 	\warning Any images and windows associated with this session will be
 * 	\warning destroyed and freed by this function, don't use them after this is
 * 	\warning called.
 */
bool	session_end(t_session *s) {
	while (s->numwin)
		win_dest(s->win[0]);
	if (s->win)
		free(s->win);
	while (s->numimg)
		img_dest(s->img[0]);
	if (s->img)
		free(s->img);
	mlx_destroy_display(s->cid);
	free(s->cid);
	s->cid = NULL;
	return (true);
}

/*!	\fn int session_clean(t_session *s)
 * 	\brief Calls win_dest() on windows marked for destruction.
 *
 * 	\param s The session the windows are associated with.
 *
 *	Internally all windows are linked to set their \a destroy variable to true
 *	if the close window button is clicked. This variable is checked by the
 *	session in session clean and the windows marked so will be destroyed.
 *
 *	This function should be called from your main loop function (the one linked
 *	with mlx_loop()).
 *
 * 	\return At the moment always returns true.
 */
int	session_clean(t_session *s) {
	uint32_t	i;

	i = 0;
	while (i < s->numwin)
	{
		if (s->win[i]->destroy == true)
			win_dest(s->win[i]);
		else
			++i;
	}
	return (0);
}
