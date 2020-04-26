/* Copyright 2020 Charles Daniels
 *
 * See README.md for license information.
 *
 * Based on https://raytracing.github.io/
 *
 * This file mostly focuses on vectors and operations that can be performed on
 * them.
 */

#ifndef VEC_H
#define VEC_H

#include "math.h"

typedef struct {
	double x;
	double y;
	double z;
} vec3;

vec3 vec3make(double x, double y, double z);
vec3 vec3sum(vec3 v1, vec3 v2);
vec3 vec3mult(vec3 v, double t);
double vec3lensq(vec3 v);
double vec3len(vec3 v);
double vec3dot(vec3 v1, vec3 v2);
vec3 vec3cross(vec3 v1, vec3 v2);
vec3 vec3unit(vec3 v);

#define vec3sub(v1, v2) vec3sum(v1, vec3mult(v2, -1))
#define vec3div(v, t) vec3mult(v, 1/(1.0*t))

#define vec3rand() ( (vec3) {.x = drand(), .y = drand(), .z = drand()} )
#define vec3randrange(_min_, _max_) ( (vec3) {.x = drandrange(_min_, _max_), \
		.y = drandrange(_min_, _max_), .z = drandrange(_min_, _max_)} )

#endif
