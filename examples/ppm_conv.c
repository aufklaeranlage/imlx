#include "imlx.h"

#include <unistd.h>

int	render(t_session *s) {
	return (session_clean(s));
}

int	main(int ac, char **argv) {
	t_session	s;
	t_win		*w;
	t_img		*i;
	
	if (ac != 2)
		return (write(1, "Usage: <cmd> <filename>\n", 24), 1);
	session_init(&s);
	i = add_img(&s, SCR_W, SCR_H, 0);
	w = add_win(&s, SCR_W, SCR_H, "!!!Easy PPM Converter !!! Guarantee NO SCAM!!!");
	for (int j = 0; j < SCR_W; j++) {
		put_pixel(i, j, j, 0x00FF0000);
		put_pixel(i, i->w - j, j, 0x00FF00FF);
	}
	put_img(i, w, 0, 0);
	img_2ppm(i, argv[1]);
	mlx_loop_hook(s.cid, render, &s);
	mlx_loop(s.cid);
	session_end(&s);
	return (0);
}
