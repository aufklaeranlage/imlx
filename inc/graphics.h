#ifndef GRAPHICS_H
# define GRAPHICS_H

# include "mlx.h"
# include "tuple.h"

# include <stdbool.h>

# ifndef SCR_W
#  define SCR_W 500
# endif

# ifndef SCR_H
#  define SCR_H 500
# endif

typedef struct s_win	t_win;

typedef struct s_img	t_img;

typedef struct s_scr {
	void	*cid;			// connection id, returned by mlx_init
	t_win	*win;
	t_img	*img;
}	t_scr;

struct s_win {
	void	*ptr;			// window pointer returned by mlx_new_window
	int		w;
	int		h;
};

struct s_img {
	void	*ptr;			// window pointer returned by mlx_new_window
	void	*addr;
	int		linesize;		// size a line takes up in memory in bytes
	int		bpp;			// bytes per pixel
	int		endian;			// 0 = little endian, 1 = big endian
};

// Screen / session handling

bool	scr_init(t_scr *s);

bool	scr_dest(t_scr *s);

// Window handling

bool	scr_win_new(t_scr *s, int w, int h, char *title);

bool	scr_win_clear(t_scr *s);

bool	scr_win_dest(t_scr *s);

// Hooks

int		stop_loop(int keycode, void *ptr);

// Image handling

bool	scr_img_new(t_scr *s);

bool	scr_serve(t_scr *s);

bool	scr_img_dest(t_scr *s);

// Drawing

void	scr_put_tup(t_scr *s, int x, int y, const t_tup *a);

#endif
