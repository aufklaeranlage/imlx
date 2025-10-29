#include "imlx.h"

#include <stdlib.h>

int	render(t_session *s) {
	return (session_clean(s));
}

int	main(void) {
	t_session	*s = malloc(sizeof(*s));
	t_win		*w[10];
	const char	*names[10] = {
		"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"
	};
	t_img		*i;
	
	session_init(s);
	i = add_img(s, SCR_W, SCR_H, 0);
	for (int j = 0; j < 10; j++) {
		w[j] = add_win(s, SCR_W, SCR_H, names[j]);
	}
	for (int j = 0; j < SCR_W; j++) {
		put_pixel(i, j, j, 0x00FF0000);
	}
	for (int j = 0; j < 10; j++) {
		put_img(i, w[j], j * 10, j * 30);
	}
	mlx_loop_hook(s->cid, render, s);
	mlx_loop(s->cid);
	session_end(s);
	free(s);
}
