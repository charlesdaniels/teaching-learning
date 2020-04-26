/* Copyright 2020 Charles Daniels
 *
 * See README.md for license information.
 *
 * Based on https://raytracing.github.io/
 */

#include "camera.h"

ray camera_get_ray(camera cam, double u, double v) {
	return (ray) {
		.origin = cam.origin,
		.direction = vec3sub(
				vec3sum(
					cam.lower_left_corner,
					vec3sum(
						vec3mult(cam.horizontal, u),
						vec3mult(cam.vertical, v))),
				cam.origin)
	};
}
