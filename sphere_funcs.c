#include "minirt.h"
#define MIN 0
#define MAX 1

t_sphere	*create_sphere(T_POINT3 *center, double radius)
{
	t_sphere	*sphere;

	sphere = malloc(sizeof(t_sphere));
	if (sphere == NULL)
		return (NULL);
	sphere->type = SPHERE;
	sphere->center = center;
	sphere->radius = radius;
	sphere->hit = &sphere_hit;
	if (sphere->radius < 0)
		sphere->radius = 0;
	return (sphere);
}

BOOL	sphere_hit(t_sphere *sphere, t_ray *ray, int *ray_t,
		t_hit_record *rec)
{
	t_vec3			*oc;
	t_quadratic_eq	quadratic_eq;
	double			discriminant_sqrtd;
	double			root;
	T_VEC3			*outward_normal;

	/*TODO: checks if oc == NULL*/
	oc = subtraction_op(sphere->center, ray->orig);
	quadratic_eq.a = vector_length_squared(ray->dir);
	quadratic_eq.rep = dot_product(ray->dir, oc);
	quadratic_eq.c = vector_length_squared(oc) - sphere->radius * sphere->radius;
	quadratic_eq.discriminant = quadratic_eq.rep * quadratic_eq.rep
		- quadratic_eq.a * quadratic_eq.c;
	if (quadratic_eq.discriminant < 0)
		return (FALSE);
	discriminant_sqrtd = sqrt(quadratic_eq.discriminant);
	root = (quadratic_eq.rep - discriminant_sqrtd) / quadratic_eq.a;
	if (root <= ray_t[MIN] || root >= ray_t[MAX])
	{
		root = (quadratic_eq.rep + discriminant_sqrtd) / quadratic_eq.a;
		if (root <= ray_t[MIN] || root >= ray_t[MAX])
			return (FALSE);
	}
	rec->t = root;
	rec->point = ray_at(ray, root);
	outward_normal = division_op(sphere->radius,
		subtraction_op(rec->point, sphere->center));
	set_face_normal(rec, ray, outward_normal);
	return (TRUE);
}
