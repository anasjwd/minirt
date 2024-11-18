#include "minirt.h"

void	set_face_normal(t_hit_record *hit_record,
					t_ray *ray, T_VEC3 *outward_normal)
{
	/**
	 * sets the hit record normal vector to always be against the ray dir
	 * NOTE: the parameter outward_normal is assumed to have unit length
	 **/
	if (dot_product(ray->dir, outward_normal) > 0.0)
	{
		hit_record->normal = scalar_op(-1, outward_normal);
		hit_record->front_face = FALSE; //not front face so ray hit from inside
	}
	else
	{
		hit_record->normal = outward_normal;
		hit_record->front_face = TRUE;
	}
}

double	clamp(t_interval *interval, double var)
{
	if (var < interval->min)
		return (interval->min);
	else if (var > interval->max)
		return (interval->max);
	else
		return (var);
}

void	pixel_put_in_img(t_img *img, int x, int y, T_COLOR *color)
{
	char		*dest;
	int			rgb;
	t_interval	interval;

	interval.min = 0.000;
	interval.max = 0.999;
	rgb = (int)(256 * clamp(&interval, color->x)) << 16
		| (int)(256 * clamp(&interval, color->y)) << 8
		| (int)(256 * clamp(&interval, color->z));
	dest = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dest = rgb;
}

int	create_image(t_data *win_data)
{
	win_data->img.img = mlx_new_image(win_data->mlx,
			win_data->width, win_data->height);
	win_data->img.addr = mlx_get_data_addr(win_data->img.img,
			&win_data->img.bits_per_pixel, &win_data->img.line_length,
			&win_data->img.endian);
	return (1);
}

int	main(void)
{
	t_data		window_data;
	t_setup3d	setup3d;

	setup3d.samples_per_pixel = 100;
	setup3d.pixel_samples_scale = 1.0 / setup3d.samples_per_pixel;
	setup3d.world = NULL;
	add_object(&setup3d.world, create_object(SPHERE,
			create_sphere(create_vec3(0, 0, -1), 0.5)));
	add_object(&setup3d.world, create_object(SPHERE,
			create_sphere(create_vec3(0, -100.5, -1), 100)));
	create_window(&window_data);
	create_image(&window_data);
	render(&window_data, &setup3d);
	mlx_loop(window_data.mlx);
}
