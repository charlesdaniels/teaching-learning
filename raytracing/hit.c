/* Copyright 2020 Charles Daniels
 *
 * See README.md for license information.
 *
 * Based on https://raytracing.github.io/
 */

#include "hit.h"

bool hit(hitobj h, ray r, double t_min, double t_max, hitrec* rec) {
	if (h.type == HITTABLE_SPHERE) {
		return hitsphere(h, r, t_min, t_max, rec);
	} else if (h.type == HITTABLE_NULL) {
		return false;
	} else {
		abort("unknown hittable type %i\n", h.type);
	}
}

bool hitmany(hitobj* h, ray r, double t_min, double t_max, hitrec* rec) {
	hitrec temprec;
	bool hitany = false;
	double closest = t_max;

	for (int i = 0 ; h[i].type != HITTABLE_NULL; i++ ) {
		if (hit(h[i], r, t_min, closest, &temprec)) {
			hitany = true;
			closest = temprec.t;
			if (rec != NULL) {
				*rec = temprec;
			}
		}
	}

	return hitany;
}

bool hitsphere(hitobj h, ray r, double t_min, double t_max, hitrec* rec) {
	if (h.type != HITTABLE_SPHERE) {
		abort("called on non-sphere object!\n%s", "");
	}

	vec3 oc = vec3sub(r.origin , h.center);
	double a = vec3lensq(r.direction);
	double half_b = vec3dot(oc, r.direction);
	double c = vec3lensq(oc) - h.radius * h.radius;
	double discriminant = half_b * half_b - a*c;

	if (discriminant > 0) {
		double root = sqrt(discriminant);
		double temp = (- half_b - root) / a;
		if (temp < t_max && temp > t_min) {
			if (rec != NULL) {
				rec->t = temp;
				rec->p = rayat(r, temp);
				vec3 outward_normal = vec3div(vec3sub(rec->p, h.center), h.radius);
				hitrec_set_face_normal(rec, r, outward_normal);
			}
			return true;
		}
		temp = (-half_b + root) / a;
		if (temp < t_max && temp > t_min) {
			if (rec != NULL) {
				rec->t = temp;
				rec->p = rayat(r, rec->t);
				vec3 outward_normal = vec3div(vec3sub(rec->p, h.center), h.radius);
				hitrec_set_face_normal(rec, r, outward_normal);
			}
			return true;
		}
	}
	return false;
}

void hitrec_set_face_normal(hitrec* rec, ray r, vec3 outward_normal) {
	rec->front_face = (vec3dot(r.direction, outward_normal) < 0);
	rec->normal = rec->front_face ? outward_normal : vec3mult(outward_normal, -1);
}
