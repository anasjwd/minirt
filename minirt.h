#ifndef MINIRT_H
#define MINIRT_H

#include "mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define T_VEC3 t_vec3
#define T_POINT3 t_vec3
#define T_COLOR	t_vec3

typedef struct s_vec3 {
	double	x;
	double	y;
	double	z;
}				t_vec3;

typedef struct s_ray {
	T_POINT3	*orig;
	T_POINT3	*dir;
}				t_ray;

t_vec3	*scalar_op(double t, t_vec3 *vec);
t_vec3	*division_op(double t, t_vec3 *v);
t_vec3	*addition_op(t_vec3 *v, t_vec3 *u);
t_vec3	*subtraction_op(t_vec3 *v, t_vec3 *u);
t_vec3	*hadamard_product(t_vec3 *v, t_vec3 *u);
double	dot_product(t_vec3 *v, t_vec3 *u);
t_vec3	*cross_product(t_vec3 *v, t_vec3 *u);
double	vector_length(t_vec3 *vec);
t_vec3	*unit_vector(t_vec3 *vector);

#endif
