/* Copyright 2020 Charles Daniels
 *
 * See README.md for license information.
 *
 * Based on https://raytracing.github.io/
 *
 * This file implements the necessarily logic to compute hits / intersections
 * with various shapes in a single abstracted API.
 */

#ifndef HIT_H
#define HIT_H

#include "vec.h"
#include "ray.h"
#include "util.h"

typedef struct {
	double t;
	vec3 p;
	vec3 normal;
	bool front_face;
} hitrec;

/* HITTABLE_NULL is used as a null terminator for arrays/lists of hitobj. 
 * Because null termination is a jolly good idea and we need more of that */
typedef enum {HITTABLE_NULL=0, HITTABLE_SPHERE} hittable_type;

/* note: not all fields are used for all types */
typedef struct hitobj_t {
	hittable_type type;
	vec3 center;		/* sphere */
	double radius;		/* sphere */
} hitobj;

/* rec can be NULL in whih case it will not be populated. If rec is non-null,
 * it will only be modified if the ray hits, otherwise it will be left as is.
 * */

bool hit(hitobj h, ray r, double t_min, double t_max, hitrec* rec);
bool hitmany(hitobj* h, ray r, double t_min, double t_max, hitrec* rec);
bool hitsphere(hitobj h, ray r, double t_min, double t_max, hitrec* rec);
void hitrec_set_face_normal(hitrec* rec, ray r, vec3 outward_normal);

#endif /* HIT_H */
