#include "imlx.h"

int	main(void) {
	t_session	s;
	t_win		*w;
	t_win		*w2;
	t_img		*i;
	
	session_init(&s);
	w = add_win(&s, SCR_W, SCR_H, "test");
	w2 = add_win(&s, SCR_W, SCR_H, "tast");
	i = add_img(&s, SCR_W, SCR_H, 0);
	
	for (int j = 0; j < SCR_W; j++) {
		put_pixel(i, j, j, 0x00FF0000);
	}
	put_img(i, w, 0, 0);
	put_img(i, w2, 0, 250);
	mlx_loop(s.cid);
}
