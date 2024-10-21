#include "minirt.h"
#define HEIGHT 500
#define WIDTH 500

typedef struct s_img {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_img;

void	ft_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	char	*dest;

	dest = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dest = color;
}

void	color_image(t_img *img)
{
	int		idx;
	int		jdx;
	double	r, g, b;
	t_color	color;

	idx = 0;
	while (idx < HEIGHT)
	{
		jdx = 0;
		while (jdx < WIDTH)
		{
			r = (double)jdx / WIDTH;
			g = (double)idx / HEIGHT;
			b = 0;
			color.r = (int)(255 * r);
			color.g = (int)(255 * g);
			color.b = (int)(255 * b);
			color.rgb = (color.r << 16) | (color.g << 8) | color.b;
			ft_mlx_pixel_put(img, jdx, idx, color.rgb);
			jdx++;
		}
		idx++;
	}
}

int	main(void)
{
	void	*mlx;
	void	*mlx_win;
	t_img	img;

	int	idx = 0;
	int	jdx = 0;

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 500, 500, "MiniRt");
	img.img = mlx_new_image(mlx, 500, 500);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
			&img.endian);
	color_image(&img);
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	mlx_loop(mlx);
}
