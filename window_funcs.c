#include "minirt.h"

int	destroy_window(void *ptr)
{
	(void)ptr;
	exit(1);
}

int	create_window(t_data *window_data)
{
	window_data->aspect_ratio = 16.0 / 9.0;
	window_data->width = 600;
	window_data->height = (int)(window_data->width / window_data->aspect_ratio);
	if (window_data->height <  1)
		window_data->height = 1;
	window_data->mlx = mlx_init();
	window_data->mlx_win = mlx_new_window(window_data->mlx, window_data->width,
			window_data->height, "Minirt");
	mlx_hook(window_data->mlx_win, 17, 0, destroy_window, NULL);
	return (0);
}
