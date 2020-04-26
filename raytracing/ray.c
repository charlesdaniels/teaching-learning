/* Copyright 2020 Charles Daniels
 *
 * See README.md for license information.
 *
 * Based on https://raytracing.github.io/
 *
 * This file mostly focuses on vectors and operations that can be performed on
 * them.
 */

#include "ray.h"

ray raymake(vec3 origin, vec3 direction) {
	return (ray) {.origin = origin, .direction=direction};
}

vec3 rayat(ray r, double t) {
	return vec3sum(r.origin, vec3mult(r.direction, t));
}

bool ray_hit_sphere(vec3 center, double radius, ray r) {
	vec3 oc = vec3sum(r.origin, vec3mult(center, -1));
	double a = vec3dot(r.direction, r.direction);
	double b = 2 * vec3dot(oc, r.direction);
	double c = vec3dot(oc, oc) - radius * radius;
	double discriminant = b * b - 4 * a * c;
	return (discriminant > 0);
}

double ray_hit_sphere_normal(vec3 center, double radius, ray r) {
	vec3 oc = vec3sum(r.origin, vec3mult(center, -1));
	double a = vec3dot(r.direction, r.direction);
	double b = 2 * vec3dot(oc, r.direction);
	double c = vec3dot(oc, oc) - radius * radius;
	double discriminant = b * b - 4 * a * c;

	if (discriminant < 0) {
		return -1;
	} else {
		return (-b - sqrt(discriminant)) / (2.0 * a);
	}
}
