#include "minirt.h"

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
