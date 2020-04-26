/* Copyright 2020 Charles Daniels
 *
 * See README.md for license information.
 *
 * Based on https://raytracing.github.io/
 *
 * This file implements the camera data structures and necessary logic.
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "util.h"
#include "vec.h"
#include "ray.h"

typedef struct {
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 origin;
} camera;

ray camera_get_ray(camera cam, double u, double v);

#endif /* CAMERA_H */
