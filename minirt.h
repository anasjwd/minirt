#ifndef MINIRT_H
#define MINIRT_H

#include "mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct s_vec3 {
	double	x;
	double	y;
	double	z;
}				t_vec3;

typedef struct s_point3 {
	double	x;
	double	y;
	double	z;
}				t_point3;

typedef struct s_color {
	int	r;
	int	g;
	int	b;
	int	rgb;
}				t_color;

typedef struct s_ray {
	t_point3	orig;
	t_point3	dir;
}				t_ray;

#endif
