#include "minirt.h"

int	setup_3d_world(t_data *win_data, t_setup3d *setup3d)
{
	setup3d->focal_length = 1.0;
	setup3d->viewport_height = 2.0;
	setup3d->viewport_width = setup3d->viewport_height *
		((double)win_data->width / win_data->height);
	setup3d->camera_center = create_vec3(0, 0, 0);
	setup3d->viewport_u = create_vec3(setup3d->viewport_width, 0, 0);
	setup3d->viewport_v = create_vec3(0, -setup3d->viewport_height, 0);
	setup3d->pixel_delta_u = division_op(win_data->width, setup3d->viewport_u);
	setup3d->pixel_delta_v = division_op(win_data->height, setup3d->viewport_v);
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

T_COLOR	*ray_color(t_ray *ray, t_object_container *world)
{
	T_COLOR			white_color;
	T_COLOR			blue_color;
	double			a;
	t_interval		ray_t;
	t_hit_record	hit_record;
	
	ray_t.min = 0;
	ray_t.max = INFINITY;
	if (hit_any_object(world, ray, &ray_t, &hit_record))
		return (create_vec3(0.5 * (hit_record.normal->x + 1), 
							0.5 * (hit_record.normal->y + 1),
							0.5 * (hit_record.normal->z + 1)));
	a = 0.5 * (unit_vector(ray->dir)->y + 1.0);
	fill_vec3(&white_color, 1.0, 1.0, 1.0);
	fill_vec3(&blue_color, 0.5, 0.7, 1.0);
	return (addition_op(scalar_op(1.0 - a, &white_color),
			scalar_op(a, &blue_color)));
}

double random_double(void)
{
	static unsigned long int next = 12345;
    unsigned long int a;
    unsigned long int c;
    unsigned long int m;

	a = 1664525;
	c = 1013904223;
	m = 4294967296;
    next = (a * next + c) % m;
    return ((double)next / (double)m);
}

T_VEC3	*sample_square(void)
{
	return (create_vec3(random_double() - 0.5, random_double() - 0.5, 0));
}

t_ray	*get_ray(t_setup3d *setup3d, int jdx, int idx)
{
	T_VEC3		*offset;
	T_POINT3	*pixel_sample;
	t_ray		*ray;

	offset = sample_square();
	pixel_sample = create_vec3(
					setup3d->pixel00_loc->x + ((jdx + offset->x) * setup3d->pixel_delta_u->x),
					setup3d->pixel00_loc->y + ((idx + offset->y) * setup3d->pixel_delta_v->y),
					setup3d->pixel00_loc->z);
	ray = malloc(sizeof(t_ray));
	if (ray == NULL)
		return (NULL);
	ray->orig = setup3d->camera_center;
	ray->dir = subtraction_op(pixel_sample, ray->orig);
	return (ray);
}

int	render(t_data *win_data, t_setup3d *setup3d)
{
	int			idx;
	int			jdx;
	int			sample;
	T_POINT3	*pixel_center;
	T_COLOR		*pixel_color;

	setup_3d_world(win_data, setup3d);
	idx = 0;
	while (idx < win_data->height)
	{
		jdx = 0;
		while (jdx < win_data->width)
		{
			pixel_color = create_vec3(0, 0, 0);
			sample = 0;
			while (sample < setup3d->samples_per_pixel)
			{
				setup3d->ray = get_ray(setup3d, jdx, idx);
				pixel_color = addition_op(pixel_color,
						ray_color(setup3d->ray, setup3d->world));
				sample++;
			}
			pixel_put_in_img(&win_data->img, jdx, idx,
					scalar_op(setup3d->pixel_samples_scale, pixel_color));
			jdx++;
		}
		idx++;
	}
	mlx_put_image_to_window(win_data->mlx, win_data->mlx_win, win_data->img.img,
			0, 0);
}

/*pixel_center = create_vec3(
					setup3d->pixel00_loc->x + (jdx * setup3d->pixel_delta_u->x),
					setup3d->pixel00_loc->y + (idx * setup3d->pixel_delta_v->y),
					setup3d->pixel00_loc->z);
			setup3d->ray.orig = setup3d->camera_center;
			setup3d->ray.dir = subtraction_op(
					pixel_center, setup3d->camera_center);
			pixel_color = ray_color(&setup3d->ray, setup3d->world);*/
