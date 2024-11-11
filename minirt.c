#include "minirt.h"

void	pixel_put_in_img(t_img *img, int x, int y, T_COLOR *color)
{
	char	*dest;
	int		rgb;

	rgb = (int)(255.999 * color->x) << 16 | (int)(255.999 * color->y)
		<< 8 | (int)(255.999 * color->z);
	dest = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dest = rgb;
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
	return (0);
}

t_vec3	*create_vec3(double x, double y, double z)
{
	t_vec3	*v;

	/*TODO: malloc with ft_alloc*/
	/**
	 * note: malloc for things I will be using for a short time
	 * ft_alloc for things that will last till the end of program
	 **/
	v = malloc(sizeof(t_vec3));
	if (v == NULL)
		return (NULL);
	v->x = x;
	v->y = y;
	v->z = z;
	return (v);
}

int	setup_3d_world(t_data *win_data, t_setup3d *setup3d)
{
	setup3d->focal_length = 1.0;
	setup3d->viewport_height = 2.0;
	/*height * aspect_ratio*/
	setup3d->viewport_width = setup3d->viewport_height *
		((double)win_data->width / win_data->height);
	setup3d->camera_center = create_vec3(0, 0, 0);
	setup3d->viewport_u = create_vec3(setup3d->viewport_width, 0, 0);
	setup3d->viewport_v = create_vec3(0, -setup3d->viewport_height, 0);
	/*viewport_u / width, function takes them in reverse order*/
	setup3d->pixel_delta_u = division_op(win_data->width, setup3d->viewport_u);
	setup3d->pixel_delta_v = division_op(win_data->height, setup3d->viewport_v);
	/*Q = camera_center - focal_length - u/2 - v/2*/
	setup3d->viewport_upper_left = create_vec3(
			setup3d->camera_center->x - (setup3d->viewport_u->x / 2),
			setup3d->camera_center->y - (setup3d->viewport_v->y / 2),
			setup3d->camera_center->z - setup3d->focal_length);
	setup3d->pixel00_loc = create_vec3(
			setup3d->viewport_upper_left->x + (0.5 * setup3d->pixel_delta_u->x),
			setup3d->viewport_upper_left->y + (0.5 * setup3d->pixel_delta_v->y),
			setup3d->viewport_upper_left->z);
	return (0);
}

int	create_image(t_data *win_data)
{
	win_data->img.img = mlx_new_image(win_data->mlx,
			win_data->width, win_data->height);
	win_data->img.addr = mlx_get_data_addr(win_data->img.img,
			&win_data->img.bits_per_pixel, &win_data->img.line_length,
			&win_data->img.endian);
}

double	hit_sphere(T_POINT3	*center, double radius, t_ray *ray)
{
	T_VEC3	*oc;
	double	a;
	double	h;
	double	c;
	double	discriminant;

	oc = subtraction_op(center, ray->orig);
	a = vector_length_squared(ray->dir);
	h = dot_product(ray->dir, oc);
	c = vector_length_squared(oc) - (radius * radius);
	discriminant = h * h - a * c;
	if (discriminant < 0)
		return (-1.0);
	else
		return ((h - sqrt(discriminant)) / a);
}

T_COLOR	*ray_color(t_ray *ray)
{
	T_COLOR		white_color;
	T_COLOR		blue_color;
	T_VEC3		*unit_direction;
	T_COLOR		*temp1;
	T_COLOR		*temp2;
	T_COLOR		*color;
	T_POINT3	sphere_center;
	T_VEC3		*normal_vec;
	double		a;
	double		t;
	
	/*TODO: use libft's memset*/
	//memset(&sphere_center, 0, sizeof(T_POINT3));
	sphere_center.x = 0;
	sphere_center.y = 0;
	sphere_center.z = -1.0;
	t = hit_sphere(&sphere_center, 0.5, ray);
	if (t > 0.0)
	{
		normal_vec = unit_vector(subtraction_op(ray_at(ray, t), &sphere_center));
		color = malloc(sizeof(T_COLOR));
		if (color == NULL)
			return (NULL);
		color->x = 0.5 * (normal_vec->x + 1);
		color->y = 0.5 * (normal_vec->y + 1);
		color->z = 0.5 * (normal_vec->z + 1);
		return (color);
	}
	unit_direction = unit_vector(ray->dir);
	a = 0.5 * (unit_direction->y + 1.0);
	white_color.x = 1.0;
	white_color.y = 1.0;
	white_color.z = 1.0;
	blue_color.x = 0.5;
	blue_color.y = 0.7;
	blue_color.z = 1.0;
	temp1 = scalar_op(1.0 - a, &white_color);
	temp2 = scalar_op(a, &blue_color);
	color = addition_op(temp1, temp2);
	return (color);
}

int	render(t_data *win_data, t_setup3d *setup3d)
{
	int			idx;
	int			jdx;
	T_POINT3	*pixel_center;
	T_COLOR		*pixel_color;

	idx = 0;
	while (idx < win_data->height)
	{
		jdx = 0;
		while (jdx < win_data->width)
		{
			pixel_center = create_vec3(
					setup3d->pixel00_loc->x + (jdx * setup3d->pixel_delta_u->x),
					setup3d->pixel00_loc->y + (idx * setup3d->pixel_delta_v->y),
					setup3d->pixel00_loc->z);
			setup3d->ray.orig = setup3d->camera_center;
			setup3d->ray.dir = subtraction_op(
					pixel_center, setup3d->camera_center);
			pixel_color = ray_color(&setup3d->ray);
			pixel_put_in_img(&win_data->img, jdx, idx, pixel_color);
			jdx++;
		}
		idx++;
	}
	mlx_put_image_to_window(win_data->mlx, win_data->mlx_win, win_data->img.img,
			0, 0);
}

int	main(void)
{
	t_data		window_data;
	t_setup3d	setup3d;

	create_window(&window_data);
	setup_3d_world(&window_data, &setup3d);
	create_image(&window_data);
	render(&window_data, &setup3d);
	mlx_loop(window_data.mlx);
}
