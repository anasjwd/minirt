#include "minirt.h"

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

double	random_double_interval(double min, double max)
{
	return (min + (max - min) * random_double());
}

T_VEC3	*random_vec(void)
{
	return (create_vec3(random_double(), random_double(), random_double()));
}

T_VEC3	*random_vec_interval(double min, double max)
{
	return (create_vec3(random_double_interval(min, max),
				random_double_interval(min, max),
				random_double_interval(min, max)));
}

T_VEC3	*random_unit_vector(void)
{
	T_VEC3	*ruv;
	double	length_squared;

	while (TRUE)
	{
		ruv = random_vec_interval(-1, 1);
		length_squared = vector_length_squared(ruv);
		if (1e-160 < length_squared && length_squared <= 1)
			return (unit_vector(ruv));
	}
}
