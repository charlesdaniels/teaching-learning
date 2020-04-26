#include "util.h"

/* Copyright 2020 Charles Daniels
 *
 * See README.md for license information.
 *
 * Based on https://raytracing.github.io/
 *
 * This implements Listing 1 from Output an Image
 */

int main(void) {
	const int image_width = 200;
	const int image_height = 100;

	image* im = alloc_image(image_width, image_height, (color) {.r = 0, .g = 0, .b = 0});	

	for (int row = im->height - 1 ; row >= 0; row--) {
		printf("\rscanlines remaining: %i    ", row);
		for (int col = 0 ; col < im->width ; col++) {
			double r = (1.0 * col) / im->width;
			double g = (1.0 * row) / im->height;
			double b = 0.2;
			int ir = (uint8_t) (255.999 * r);
			int ig = (uint8_t) (255.999 * g);
			int ib = (uint8_t) (255.999 * b);
			*pix(im, row, col) = (color) {.r = ir, g=ig, .b=ib};
		}
	}
	printf("\nDONE\n");

	write_image(im, OUTFILE);
	free_image(im);
}
