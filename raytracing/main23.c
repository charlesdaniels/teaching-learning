#include "util.h"
#include "ray.h"
#include "vec.h"
#include "hit.h"

/* Copyright 2020 Charles Daniels
 *
 * See README.md for license information.
 *
 * Based on https://raytracing.github.io/
 *
 * This implements Listing 23 from Common Constants and Utility Functions
 */

color ray_color(ray r, hitobj* world) {
	hitrec rec;
	if (hitmany(world, r, 0, INFINITY, &rec)) {
		return float2color (
			0.5 * (rec.normal.x + 1),
			0.5 * (rec.normal.y + 1),
			0.5 * (rec.normal.z + 1)
		);
	}

	vec3 unit_direction = vec3unit(r.direction);
	double t = 0.5 * (unit_direction.y + 1.0);
	return float2color(
		(1.0-t) + t * 0.5,
		(1.0-t) + t * 0.7,
		(1.0-t) + t * 1.0
	);
}

int main(void) {
	const int image_width = 200;
	const int image_height = 100;

	image* im = alloc_image(image_width, image_height, (color) {.r = 0, .g = 0, .b = 0});	

	vec3 lower_left_corner = vec3make(-2, -1, -1);
	vec3 horizontal = vec3make(4, 0, 0);
	vec3 vertical = vec3make(0, 2, 0);
	vec3 origin = vec3make(0, 0, 0);

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
			double u = (1.0 * col) / im->width;
			double v = (1.0 * row) / im->height;
			ray r = raymake(origin,
					vec3sum(lower_left_corner,
						vec3sum(
							vec3mult(horizontal, u),
							vec3mult(vertical, v)
						)
					)
				);
			color c = ray_color(r, &(world[0]));
			*pix(im, row, col) = c;
		}
	}
	printf("\nDONE\n");

	write_image(im, OUTFILE);
	free_image(im);
}
