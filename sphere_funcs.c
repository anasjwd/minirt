#include "minirt.h"
#define MIN 0
#define MAX 1

t_sphere	*create_sphere(const T_POINT3 *center, double radius)
{
	t_sphere	*sphere;

	sphere = malloc(sizeof(t_sphere));
	if (sphere == NULL)
		return (NULL);
	sphere->center = center;
	sphere->radius = radius;
	if (sphere->radius < 0)
		sphere->radius = 0;
	return (sphere);
}

BOOL	sphere_hit(const t_sphere *sphere, const t_ray *ray, int *ray_t,
		t_hit_record *rec)
{
	t_vec3			*oc;
	t_quadratic_eq	quadratic_eq;
	double			discriminant_sqrtd;
	double			root;

	/*TODO: checks if oc == NULL*/
	oc = subtraction_op(sphere->center, ray->orig);
	quadratic_eq.a = vector_length_squared(ray->dir);
	quadratic_eq.res = dot_product(ray->dir, oc);
	quadratic_eq.c = vector_length_squared(oc) - radius * radius;
	quadratic_eq.discriminant = h * h - a * c;
	if (discriminant < 0)
		return (FALSE);
	discriminant_sqrtd = sqrt(discriminant);
	rout = (h - discriminant_sqrtd) / a;
	if (root <= ray_t[MIN] || root >= ray_t[MAX])
	{
		root = (h + discriminant_sqrtd) / a;
		if (root <= ray_t[MIN] || root >= ray_t[MAX])
			return (FALSE);
	}
	rec->t = root;
	rec->point = ray_at(ray, root);
	rec->normal = division_op(radius,
			subtraction_op(rec->point, sphere->center));
	return (TRUE);
}
