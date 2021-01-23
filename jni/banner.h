#ifndef _BANNER_H_
#define _BANNER_H_

#include "object.h"

#define BANNER_F_DONT_COMPENSATE 0x01

object_t *banner_create(object_t *ga_obj, int texture_id, double init_wait,
			double xoff, double yoff, double zoff,
			double tx1, double tx2, double ty1, double ty2,
			double w, double h, int flags);

typedef struct banner_coords_s {
  GLfloat tx1;
  GLfloat ty1;
  GLfloat tx2;
  GLfloat ty2;
  GLfloat w;
  GLfloat h;
} banner_coords_t;

static banner_coords_t score_banners[] __attribute__((unused)) = {
  { 0.031, 0.688, 0.605, 0.988, 2.940, 1.540 },     // 250
  { 0.031, 0.361, 0.605, 0.664, 2.940, 1.550 },     // 500
  { 0.025, 0.025, 0.600, 0.328, 2.940, 1.550 },     // 750
};

#define SCORE_BANNER_250 0
#define SCORE_BANNER_500 1
#define SCORE_BANNER_750 2

#endif
