#include "minirt.h"

t_vec3	*scalar_op(double t, t_vec3 *vec)
{
	t_vec3	*scaled_vec;

	scaled_vec = malloc(sizeof(t_vec3));
	if (scaled_vec == NULL)
		return (NULL);
	scaled_vec->x = t * vec->x;
	scaled_vec->y = t * vec->y;
	scaled_vec->z = t * vec->z;
	return (scaled_vec);
}

t_vec3	*division_op(double t, t_vec3 *v)
{
	return (scalar_op(1/t, v));
}

t_vec3	*addition_op(t_vec3 *v, t_vec3 *u)
{
	t_vec3	*resultant_vec;

	resultant_vec = malloc(sizeof(t_vec3));
	if (resultant_vec == NULL)
		return (NULL);
	resultant_vec->x = v->x + u->x;
	resultant_vec->y = v->y + u->y;
	resultant_vec->z = v->z + u->z;
	return (resultant_vec);
}

t_vec3	*subtraction_op(t_vec3 *v, t_vec3 *u)
{
	t_vec3	*difference_vec;

	difference_vec = malloc(sizeof(t_vec3));
	if (difference_vec == NULL)
		return (NULL);
	difference_vec->x = v->x - u->x;
	difference_vec->y = v->y - u->y;
	difference_vec->z = v->z - u->z;
	return (difference_vec);
}

t_vec3	*hadamard_product(t_vec3 *v, t_vec3 *u)
{
	t_vec3	*hadamard_product_vec;

	hadamard_product_vec = malloc(sizeof(t_vec3));
	if (hadamard_product_vec == NULL)
		return (NULL);
	hadamard_product_vec->x = v->x * u->x;
	hadamard_product_vec->y = v->y * u->y;
	hadamard_product_vec->z = v->z * u->z;
	return (hadamard_product_vec);
}

double	dot_product(t_vec3 *v, t_vec3 *u)
{
	return ((v->x * u->x) + (v->y * u->y) + (v->z * u->z));
}

t_vec3	*cross_product(t_vec3 *v, t_vec3 *u)
{
	t_vec3	*cross_product_vec;

	cross_product_vec = malloc(sizeof(t_vec3));
	if (cross_product_vec == NULL)
		return (NULL);
	cross_product_vec->x = (v->y * u->z) - (v->z * u->y);
	cross_product_vec->y = (v->z * u->x) - (v->x * u->z);
	cross_product_vec->z = (v->x * v->y) - (v->y * u->x);
	return (cross_product_vec);
}

double	vector_length_squared(t_vec3 *vec)
{
	double	length_squared;

	length_squared = (vec->x * vec->x) + (vec->y * vec->y) + (vec->z * vec->z);
	return (length_squared);
}

double	vector_length(t_vec3 *vec)
{
	return (sqrt(vector_length_squared(vec)));
}

t_vec3	*unit_vector(t_vec3 *vector)
{
	t_vec3	*unit_vector;

	unit_vector = division_op(vector_length(vector), vector);
	return (unit_vector);
}

T_POINT3	*ray_at(t_ray *ray, double t)
{
	return (addition_op(ray->orig, scalar_op(t, ray->dir)));
}

void	fill_vec3(t_vec3 *vec, double x, double y, double z)
{
	vec->x = x;
	vec->y = y;
	vec->z = z;
}

