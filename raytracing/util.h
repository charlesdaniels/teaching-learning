#ifndef UTIL_H
#define UTIL_H

/* Copyright 2020 Charles Daniels
 *
 * See README.md for license information.
 *
 * Based on https://raytracing.github.io/
 *
 * This file mostly pertains to the image structure and functions that
 * operate it, such as writing it to disk.
 */

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} color;

typedef struct {
	color* data;
	uint16_t width;
	uint16_t height;
} image;

#define pix(_im, _row, _col) (&(_im->data[_row * _im->width + _col]))

#define abort(fmt, ...) do { \
		fprintf(stderr, "FATAL: aborted at %s:%i %s(): ", \
				__FILE__, __LINE__, __func__); \
		fprintf(stderr, fmt, __VA_ARGS__); \
		exit(1); \
	} while(0)

#define deg2rad(_deg_) ( (_deg_ * M_PI) / 180.0 )
#define dmin(a, b) { a <= b ? a : b; }
#define dmax(a, b) { a >= b ? a : b; }
#define drand() (random() / ( (1.0 *RAND_MAX) + 1.0))
#define drandrange(_min_, _max_) \
	(min + (max - min) * drand())
#define dclamp(_x_, _min_, _max_) (x < min ? min : x > max ? max : x)

image* alloc_image(uint16_t width, uint16_t height, color def);
void free_image(image* im);
void write_image(image* im, char* path);
color float2color(double r, double g, double b);

#endif /* UTIL_H */
