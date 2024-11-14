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

void	pixel_put_in_img(t_img *img, int x, int y, T_COLOR *color)
{
	char	*dest;
	int		rgb;

	rgb = (int)(255.999 * color->x) << 16 | (int)(255.999 * color->y)
		<< 8 | (int)(255.999 * color->z);
	dest = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dest = rgb;
}

int	destroy_window(void *ptr)
{
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

void	add_object(t_object_container **world, t_object_container *new_object)
{
	t_object_container	*temp;

	if (world == NULL)
		return ;
	if (*world == NULL)
		*world = new_object;
	else
	{
		temp = *world;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_object;
	}
}

t_object_container	*create_object(int type, void *object)
{
	t_object_container	*node;

	node = malloc(sizeof(t_object_container));
	if (node == NULL)
		return (NULL);
	node->type = type;
	node->object = object;
	node->next = NULL;
	return (node);
}

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

int	create_image(t_data *win_data)
{
	win_data->img.img = mlx_new_image(win_data->mlx,
			win_data->width, win_data->height);
	win_data->img.addr = mlx_get_data_addr(win_data->img.img,
			&win_data->img.bits_per_pixel, &win_data->img.line_length,
			&win_data->img.endian);
}

BOOL	hit_any_object(t_object_container *world, t_ray *ray, t_interval *ray_t,
		t_hit_record *rec)
{
	t_hit_record		*temp_rec;
	t_interval			interval;
	BOOL				hit_anything;
	t_object_container	*container;

	hit_anything = FALSE;
	interval.min = ray_t->min;
	interval.max = ray_t->max;
	temp_rec = rec;
	container = world;
	while (container != NULL)
	{
		if (container->type == SPHERE)
		{
			if (hit_sphere(container->object, ray, &interval,
						temp_rec))
			{
				hit_anything = TRUE;
				interval.max = temp_rec->t;
				rec = temp_rec;
			}
		}
		container = container->next;
	}
	return (hit_anything);
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

int	render(t_data *win_data, t_setup3d *setup3d)
{
	int			idx;
	int			jdx;
	T_POINT3	*pixel_center;
	T_COLOR		*pixel_color;

	setup_3d_world(win_data, setup3d);
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
			pixel_color = ray_color(&setup3d->ray, setup3d->world);
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

	setup3d.world = NULL;
	add_object(&setup3d.world, create_object(SPHERE,
			create_sphere(create_vec3(0, 0, -1), 0.5)));
	add_object(&setup3d.world, create_object(SPHERE,
			create_sphere(create_vec3(0, -100.5, -1), 100)));
	create_window(&window_data);
	//setup_3d_world(&window_data, &setup3d);
	create_image(&window_data);
	render(&window_data, &setup3d);
	mlx_loop(window_data.mlx);
}
