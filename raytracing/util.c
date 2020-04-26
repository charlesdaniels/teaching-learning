#include "util.h"

/* Copyright 2020 Charles Daniels
 *
 * See README.md for license information.
 *
 * Based on https://raytracing.github.io/
 */

image* alloc_image(uint16_t width, uint16_t height, color def) {
	image* im = malloc(sizeof(image));
	im->data = malloc(sizeof(color) * width * height);
	im->width = width;
	im->height = height;
	for (int i = 0 ; i < (width * height) ; i++) {
		im->data[i] = def;
	}
	return im;
}

void free_image(image* im) {
	free(im->data);
	free(im);
}

void write_image(image* im, char* path) {
	/* write out an image magic in H2F 24bpp format (HIF24) */

	FILE* fp = fopen(path, "w");

	/* magic bytes */
	fputc('H', fp);
	fputc('e', fp);
	fputc('R', fp);
	fputc('C', fp);

	/* size */
	fputc((im->width & 0xff00) >> 8, fp);
	fputc((im->width & 0xff), fp);
	fputc((im->height & 0xff00) >> 8, fp);
	fputc((im->height & 0xff), fp);

	/* format 0 = hif24 */
	fputc(0, fp);

	/* reserved bytes */
	fputc(0, fp);
	fputc(0, fp);
	fputc(0, fp);
	fputc(0, fp);
	fputc(0, fp);
	fputc(0, fp);
	fputc(0, fp);

	for (int row = im->height-1 ; row >= 0 ; row--) {
		for (int col = 0 ; col < im->width ; col++) {
			fputc(pix(im, row, col)->r, fp);
			fputc(pix(im, row, col)->g, fp);
			fputc(pix(im, row, col)->b, fp);
		}
	}

	fclose(fp);
}

color float2color(double r, double g, double b) {

	/* clamping */
	if (r > 1.0) {r = 1.0;};
	if (r < 0.0) {r = 0.0;};
	if (g > 1.0) {g = 1.0;};
	if (g < 0.0) {g = 0.0;};
	if (b > 1.0) {b = 1.0;};
	if (b < 0.0) {b = 0.0;};

	return (color) {
		.r = (uint8_t) (255.99 * r),
		.g = (uint8_t) (255.99 * g),
		.b = (uint8_t) (255.99 * b),
	};
}
