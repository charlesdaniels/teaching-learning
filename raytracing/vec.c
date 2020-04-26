
/* Copyright 2020 Charles Daniels
 *
 * See README.md for license information.
 *
 * Based on https://raytracing.github.io/
 */

#include "vec.h"

vec3 vec3make(double x, double y, double z) {
	return (vec3) {.x = x, .y = y, .z = z};
}

vec3 vec3sum(vec3 v1, vec3 v2) {
	vec3 r;
	r.x = v1.x + v2.x;
	r.y = v1.y + v2.y;
	r.z = v1.z + v2.z;
	return r;
}

vec3 vec3mult(vec3 v, double t) {
	vec3 r;
	r.x = v.x * t;
	r.y = v.y * t;
	r.z = v.z * t;
	return r;
}

double vec3lensq(vec3 v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

double vec3len(vec3 v) {
	return sqrt(vec3lensq(v));
}


double vec3dot(vec3 v1, vec3 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec3 vec3cross(vec3 v1, vec3 v2) {
	return (vec3) {
		.x = v1.y * v2.z - v1.z * v2.y,
		.y = v1.z * v2.x - v1.x * v2.z,
		.z = v1.x * v2.y - v1.y * v2.x
	};
}

vec3 vec3unit(vec3 v) {
	return vec3mult(v, 1 / vec3len(v));
}
