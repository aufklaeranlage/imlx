#ifndef IMLX_H
# define IMLX_H

# include "mlx.h"

# include <stdbool.h>
# include <stdint.h>

# ifndef SCR_W
#  define SCR_W 500
# endif

# ifndef SCR_H
#  define SCR_H 500
# endif

typedef struct s_win	t_win;

typedef struct s_img	t_img;

typedef struct s_session {
	void	*cid;			/* connection id to the X server */
	t_win	**win;			/* pointers to the associated window structs */
	int		numwin;			/* amount of windows associated with this connection */
	t_img	**img;			/* Pointers to the associated image structs */
	int		numimg;			/* amount of images associated with this connection */
}	t_session;

struct s_win {
	char		*title;
	void		*ptr;		/* window pointer returned by mlx_new_window */
	int			w;			/* width of a window (in pixels) */
	int			h;			/* hieght of a window (in pixels) */
	t_session	*s;			/* ease of use pointer to the associated session */
};

struct s_img {
	int			id;
	int			w;
	int			h;
	void		*ptr;		/* window pointer returned by mlx_new_window */
	void		*addr;		/* address of the color array for the image */
	int			ls;			/* size a line takes up in memory in bytes */
	int			bpp;		/* bytes per pixel */
	int			en;			/* 0 = little endian, 1 = big endian */
	t_session	*s;			/* ease of use pointer to the associated session */
};

/* Screen / session handling */

bool	session_init(t_session *s);

bool	session_end(t_session *s);

/* Window handling */

t_win	*add_win(t_session *s, int w, int h, const char *title);

t_win	*get_win(const t_session *s, const char *title);

bool	win_clear(t_win *w);

bool	win_dest(t_win *w);

/* Image handling */

t_img	*add_img(t_session *s, int w, int h, int id);

t_img	*get_img(const t_session *s, int id);

bool	put_img(t_img *i, t_win *w, int x, int y);

bool	img_clear(t_img *i);

bool	img_dest(t_img *s);

/* Drawing */

void	put_pixel(t_img *i, int x, int y, uint32_t color);

#endif
