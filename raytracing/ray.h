/* Copyright 2020 Charles Daniels
 *
 * See README.md for license information.
 *
 * Based on https://raytracing.github.io/
 *
 * This file mostly focuses on vectors and operations that can be performed on
 * them.
 */

#ifndef RAY_H
#define RAY_H

#include <math.h>
#include <stdbool.h>
#include "vec.h"

typedef struct {
	vec3 origin;
	vec3 direction;
} ray;


ray raymake(vec3 origin, vec3 direction);
vec3 rayat(ray r, double t);

/* NOTE: these "hit" functions are from some of the earlier exercises and are
 * are left in for the benefit of them. See hit.h for better hit functions */
bool ray_hit_sphere(vec3 center, double radius, ray r);
double ray_hit_sphere_normal(vec3 center, double radius, ray r);

#endif /* RAY_H */
