#include "util.h"
#include "ray.h"
#include "vec.h"
#include "hit.h"
#include "camera.h"

/* Copyright 2020 Charles Daniels
 *
 * See README.md for license information.
 *
 * Based on https://raytracing.github.io/
 *
 * This implements Listing 29 from Antialiasing
 */

vec3 ray_color(ray r, hitobj* world) {
	hitrec rec;
	if (hitmany(world, r, 0, INFINITY, &rec)) {
		return vec3make (
			0.5 * (rec.normal.x + 1),
			0.5 * (rec.normal.y + 1),
			0.5 * (rec.normal.z + 1)
		);
	}

	vec3 unit_direction = vec3unit(r.direction);
	double t = 0.5 * (unit_direction.y + 1.0);
	return vec3make (
		(1.0-t) + t * 0.5,
		(1.0-t) + t * 0.7,
		(1.0-t) + t * 1.0
	);
}

int main(void) {
	const int image_width = 400;
	const int image_height = 200;
	const int samples_per_pixel = 100;

	image* im = alloc_image(image_width, image_height, (color) {.r = 0, .g = 0, .b = 0});

	camera cam = (camera) {
		.lower_left_corner = vec3make(-2, -1, -1),
		.horizontal = vec3make(4, 0, 0),
		.vertical = vec3make(0, 2, 0),
		.origin = vec3make(0, 0, 0)
	};

	hitobj world[3];
	world[0] = (hitobj) {
		.type = HITTABLE_SPHERE,
		.center = vec3make(0, 0, -1),
		.radius = 0.5
	};
	world[1] = (hitobj) {
		.type = HITTABLE_SPHERE,
		.center = vec3make(0, -100.5, -1),
		.radius = 100
	};
	world[2].type = HITTABLE_NULL;


	for (int row = im->height - 1 ; row >= 0; row--) {
		printf("\rscanlines remaining: %i    ", row);
		for (int col = 0 ; col < im->width ; col++) {
			vec3 veccolor = vec3make(0, 0, 0);

			for (int s = 0 ; s < samples_per_pixel; s++) {
				double u = (1.0 * col + drand()) / im->width;
				double v = (1.0 * row + drand()) / im->height;
				ray r = camera_get_ray(cam, u, v);
				veccolor = vec3sum(veccolor, ray_color(r, &(world[0])));
			}

			veccolor = vec3div(veccolor, samples_per_pixel);
			color c = float2color(veccolor.x, veccolor.y, veccolor.z);
			*pix(im, row, col) = c;
		}
	}
	printf("\nDONE\n");

	write_image(im, OUTFILE);
	free_image(im);
}
