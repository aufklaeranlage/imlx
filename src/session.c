#include "graphics.h"

#include "mlx.h"

#include <stddef.h>

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
 *	Loops through all the windows and images inside the session, destroys them
 *	and sets the connection identifier to 0.
 *
 * 	\return At the moment always returns true.
 * 	\warning The pointer \a s is not checked for being valid.
 * 	\warning Any images and windows associated with this session will be
 * 	\warning destroyed and freed by this function, don't use them after this is
 * 	\warning called.
 */
bool	session_end(t_session *s) {
	while (s->numwin)
		win_destroy(s->win[0]);
	while (s->numimg)
		win_destroy(s->img[0]);
	s->cid = NULL;
	return (true);
}
