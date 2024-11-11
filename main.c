#include "minirt.h"
#include <string.h>
#define HEIGHT 500
#define WIDTH 500

void	ft_mlx_pixel_put(t_img *img, int x, int y, T_COLOR *color)
{
	char	*dest;
	int		rgb;

	rgb = (int)(255.999 * color->x) << 16 | (int)(255.999 * color->y)
		<< 8 | (int)(255.999 * color->z);
	dest = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dest = rgb;
}

t_vec3	*create_vec3(int x, int y, int z)
{
	t_vec3	*vector;

	vector = malloc(sizeof(t_vec3));
	if (vector == NULL)
		return (NULL);
	vector->x = x;
	vector->y = y;
	vector->z = z;
	return (vector);
}

double	hit_sphere(T_POINT3 *center, double radius, t_ray *ray)
{
	t_vec3	*oc;
	double	a;
	double	b;
	double	c;
	double	discriminant;

	oc = subtraction_op(center, ray->orig);
	a = dot_product(ray->dir, ray->dir);
	b = -2.0 * dot_product(ray->dir, oc);
	c = dot_product(oc, oc) - radius * radius;
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (-1.0);
	else
		return ((-b - sqrt(discriminant)) / (2.0 * a));
}

T_COLOR	*ray_color(t_ray *ray)
{
	t_vec3	*unit_direction;
	double	a;
	T_COLOR	white;
	T_COLOR	blue;
	T_POINT3	sphere_center;
	T_COLOR	*color;
	double	t;
	t_vec3	*normal_vector;

	sphere_center.x = 0;
	sphere_center.y = 0;
	sphere_center.z = -1;
	t = hit_sphere(&sphere_center, 0.5, ray);
	if (t > 0.0)
	{
		normal_vector = unit_vector(subtraction_op(ray_at(ray, t), &sphere_center));
		color = malloc(sizeof(T_COLOR));
		color->x = 0.5 * (normal_vector->x + 1);
		color->y = 0.5 * (normal_vector->y + 1);
		color->z = 0.5 * (normal_vector->z + 1);
		return (color);
	}
	unit_direction = unit_vector(ray->dir);
	a = 0.5 * (1.0 + unit_direction->y);
	white.x = 1.0;
	white.y = 1.0;
	white.z = 1.0;
	blue.x = 0.5;
	blue.y = 0.7;
	blue.z = 1.0;
	return (addition_op(scalar_op(1.0 - a, &white), scalar_op(a, &blue)));
}

int	main(void)
{
	void	*mlx;
	void	*mlx_win;
	t_img	img;

	int	idx = 0;
	int	jdx = 0;

	double		focal_length;
	double		viewport_height;
	double		viewport_width;
	T_POINT3	camera_center;

	t_vec3	viewport_u;
	t_vec3	viewport_v;

	T_POINT3	*viewport_upper_left;
	T_POINT3	*pixel00_loc;

	T_POINT3	*pixel_delta_u;
	T_POINT3	*pixel_delta_v;
	
	T_POINT3	*pixel_center;
	T_POINT3	*ray_direction;
	t_ray		ray;
	T_COLOR		*pixel_color;
	

	//Camera
	focal_length = 1.0;
	viewport_height = 2.0;
	viewport_width = viewport_height * ((double)WIDTH / HEIGHT);
	memset(&camera_center, 0, sizeof(T_POINT3));

	//Vectors across horiz and down the vert viewport edges
	viewport_u.x = viewport_width;
	viewport_u.y = 0;
	viewport_u.z = 0;
	viewport_v.x = 0;
	viewport_v.y = -viewport_height;
	viewport_v.z = 0;

	//horiz and vert delta vectors from pix to pix
	pixel_delta_u = division_op(WIDTH, &viewport_u);
	pixel_delta_v = division_op(HEIGHT, &viewport_v);
	
	//calc location of the upper left pixel
	viewport_upper_left = subtraction_op(subtraction_op(subtraction_op(&camera_center, create_vec3(0, 0, focal_length)), division_op(2, &viewport_u)), division_op(2, &viewport_v));
	pixel00_loc = addition_op(viewport_upper_left, scalar_op(0.5, addition_op(pixel_delta_u, pixel_delta_v)));
	
	//create window
	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 500, 500, "MiniRt");

	//create image
	img.img = mlx_new_image(mlx, 500, 500);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
			&img.endian);
	while (idx < HEIGHT)
	{
		jdx = 0;
		while (jdx < WIDTH)
		{
			pixel_center = addition_op(pixel00_loc, addition_op(scalar_op(jdx, pixel_delta_u),
						scalar_op(idx, pixel_delta_v)));
			ray_direction = subtraction_op(pixel_center, &camera_center);
			ray.orig = &camera_center;
			ray.dir = ray_direction;
			pixel_color = ray_color(&ray);
			ft_mlx_pixel_put(&img, jdx, idx, pixel_color);
			jdx++;
		}
		idx++;
	}

	//put image
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);

	mlx_loop(mlx);
}
