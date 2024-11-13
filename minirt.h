#ifndef MINIRT_H
# define MINIRT_H

# include "mlx.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <math.h>

# define T_VEC3 t_vec3
# define T_POINT3 t_vec3
# define T_COLOR	t_vec3

# define BOOL int
# define FALSE 0
# define TRUE 1

# define MIN 0
# define MAX 1

# define PI 3.14159265358979323

# define SPHERE 0

typedef struct s_img {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_img;


typedef struct s_data {
	void	*mlx;
	void	*mlx_win;
	int		height;
	int		width;
	double	aspect_ratio;
	t_img	img;
}				t_data;

typedef struct s_vec3 {
	double	x;
	double	y;
	double	z;
}				t_vec3;

typedef struct s_ray {
	T_POINT3	*orig;
	T_POINT3	*dir;
}				t_ray;

typedef struct s_object_container {
	int							type;
	void						*object;
	struct s_object_container	*next;
}				t_object_container;

typedef struct s_sphere {
	T_POINT3	*center;
	double		radius;
}				t_sphere;

typedef struct s_setup3d
{
	T_POINT3			*camera_center;
	double				focal_length;
	t_ray				ray;
	double				viewport_height;
	double				viewport_width;
	T_VEC3				*viewport_u;
	T_VEC3				*viewport_v;
	T_POINT3			*viewport_upper_left;
	T_POINT3			*pixel00_loc;
	T_VEC3				*pixel_delta_u;
	T_VEC3				*pixel_delta_v;
	t_object_container	*world;
}				t_setup3d;

typedef struct s_hit_record {
	T_POINT3	*point;
	T_VEC3		*normal;
	double		t;
	BOOL		front_face;
}				t_hit_record;

typedef struct s_quadratic_eq {
	double	a;
	double	b;
	double	c;
	double	rep;
	double discriminant;
}				t_quadratic_eq;

typedef struct s_interval
{
	double	min;
	double	max;
}				t_interval;

t_vec3	*scalar_op(double t, t_vec3 *vec);
t_vec3	*division_op(double t, t_vec3 *v);
t_vec3	*addition_op(t_vec3 *v, t_vec3 *u);
t_vec3	*subtraction_op(t_vec3 *v, t_vec3 *u);
t_vec3	*hadamard_product(t_vec3 *v, t_vec3 *u);
double	dot_product(t_vec3 *v, t_vec3 *u);
t_vec3	*cross_product(t_vec3 *v, t_vec3 *u);
double	vector_length(t_vec3 *vec);
t_vec3	*unit_vector(t_vec3 *vector);
T_POINT3	*ray_at(t_ray *ray, double t);
double	vector_length_squared(t_vec3 *vec);
t_sphere	*create_sphere(T_POINT3 *center, double radius);
BOOL	hit_sphere(void *object, t_ray *ray, t_interval *ray_t,
		t_hit_record *rec);
void	set_face_normal(t_hit_record *hit_record,
					t_ray *ray, T_VEC3 *outward_normal);
void	fill_vec3(t_vec3 *vec, double x, double y, double z);

#endif
