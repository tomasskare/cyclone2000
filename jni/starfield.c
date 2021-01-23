#include <jni.h>
#include <android/log.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#include <GLES2/gl2.h>

#include "debug.h"
#include "native_main.h"
#include "engine.h"
#include "menu.h"
#include "matrix.h"
#include "shaders.h"
#include "gamearea.h"
#include "starfield.h"
#include "object.h"

#define NUM_STARS 300
#define SCALEX 10.0f
#define SCALEY 10.0f
#define SCALEZ (15.0f + gamearea_viewdist())

typedef struct starfield_s {
  object_t *ga_obj;

  double timer;

  GLfloat starcoords[3 * NUM_STARS];
  GLfloat speed_factor;
  GLfloat translate_factor;
  GLfloat alpha;
  double fadespeed;
  GLfloat pointsize;
  int shader_type;

  float *mtx_view;
} starfield_t;

static void starfield_destroy(object_t *obj)
{
  starfield_t *sf = (starfield_t *)obj->object;

  free(sf);
  free(obj);
}

static int starfield_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  starfield_t *sf = (starfield_t *)obj->object;
  float diff = msec - prev_msec;
  int i;
  GLfloat exiting_speed = sf->ga_obj ? gamearea_exiting_speed(sf->ga_obj) : 0.0;
  GLfloat vx, vy, vz;

  DEBUG(DEBUG_UPDATE, " ");

  sf->timer += diff;

  if(FLAG_TST(obj->flags, OBJECT_F_STAGE_INIT) &&
     !FLAG_TST(obj->flags, OBJECT_F_STAGE_RESTART)) {
    sf->translate_factor = 0.04;
    if(sf->speed_factor == 0.0f) {
      sf->speed_factor = 12.0f;
    } else {
      sf->speed_factor -= 11.0 * diff / 2000;
      if(sf->speed_factor < 1.0f) {
	sf->speed_factor = 1.0f;
	FLAG_CLR(obj->flags, OBJECT_F_STAGE_INIT);
      }
    }
  } else if(FLAG_TST(obj->flags, OBJECT_F_STAGE_RESTART)) {
    sf->speed_factor = 1.0f;
  } else if(exiting_speed > 0.0) {
    engine_viewpoint_get(&vx, &vy, &vz);
    sf->speed_factor += exiting_speed * 0.09;
    sf->translate_factor = 4.0;
  }

  for(i = 0 ; i < NUM_STARS ; i++) {
    sf->starcoords[i * 3 + 2] += sf->speed_factor * diff / 300;

    if(exiting_speed > 0.0) {
      if(sf->starcoords[i * 3 + 2] > gamearea_viewdist() + 5.0f + vz) {
	sf->starcoords[i * 3 + 0] =
	  (SCALEX * ((double)rand() / (double)RAND_MAX)) - SCALEX / 2;
	sf->starcoords[i * 3 + 1] =
	  (SCALEY * ((double)rand() / (double)RAND_MAX)) - SCALEY / 2;

	while(sf->starcoords[i * 3 + 2] > gamearea_viewdist() + 5.0f + vz)
	  sf->starcoords[i * 3 + 2] -= SCALEZ;
      }
    } else {
      while(sf->starcoords[i * 3 + 2] > gamearea_viewdist() + 5.0f)
	sf->starcoords[i * 3 + 2] -= SCALEZ;
    }
  }

  if(sf->fadespeed > 0 && sf->timer > 400) {
    sf->alpha += diff / sf->fadespeed;
    if(sf->alpha > 1.0)
      sf->alpha = 1.0;
  }

  return 1;
}

static void starfield_draw(object_t *obj)
{
  starfield_t *sf = (starfield_t *)obj->object;
  float mtx_model[16];

  DEBUG(DEBUG_DRAW, " ");

  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);

  matrix_identity(mtx_model);
  engine_gldirect_draw_points(sf->shader_type, sf->pointsize,
			      NUM_STARS, sf->starcoords,
			      0.8f, 0.8f, 0.8f, sf->alpha,
			      mtx_model, sf->mtx_view, NULL);

  if(sf->speed_factor != 1.0) {
    GLfloat tz = sf->translate_factor * (sf->speed_factor - 1.0);

    if(gfx_details >= GFX_DETAILS_HIGH) {
      if(tz > 0.07)
	tz = 0.07;

      matrix_translate(mtx_model, 0.0f, 0.0f, tz);
      engine_gldirect_draw_points(sf->shader_type, sf->pointsize,
				  NUM_STARS, sf->starcoords,
				  0.8f, 0.8f, 0.8f, sf->alpha,
				  mtx_model, sf->mtx_view, NULL);

      if(tz > 0.01) {
	matrix_translate(mtx_model, 0.0f, 0.0f, tz);
	engine_gldirect_draw_points(sf->shader_type, sf->pointsize,
				    NUM_STARS, sf->starcoords,
				    0.8f, 0.8f, 0.8f, sf->alpha,
				    mtx_model, sf->mtx_view, NULL);
      }

      if(tz > 0.025) {
	matrix_translate(mtx_model, 0.0f, 0.0f, tz);
	engine_gldirect_draw_points(sf->shader_type, sf->pointsize,
				    NUM_STARS, sf->starcoords,
				    0.8f, 0.8f, 0.8f, sf->alpha,
				    mtx_model, sf->mtx_view, NULL);
      }

      if(tz > 0.04) {
	matrix_translate(mtx_model, 0.0f, 0.0f, tz);
	engine_gldirect_draw_points(sf->shader_type, sf->pointsize,
				    NUM_STARS, sf->starcoords,
				    0.8f, 0.8f, 0.8f, sf->alpha,
				    mtx_model, sf->mtx_view, NULL);
      }

      if(tz > 0.055) {
	matrix_translate(mtx_model, 0.0f, 0.0f, tz);
	engine_gldirect_draw_points(sf->shader_type, sf->pointsize,
				    NUM_STARS, sf->starcoords,
				    0.8f, 0.8f, 0.8f, sf->alpha,
				    mtx_model, sf->mtx_view, NULL);
      }
    } else {
      if(tz > 0.15)
	tz = 0.15;

      matrix_translate(mtx_model, 0.0f, 0.0f, tz);
      engine_gldirect_draw_points(sf->shader_type, sf->pointsize,
				  NUM_STARS, sf->starcoords,
				  0.8f, 0.8f, 0.8f, sf->alpha,
				  mtx_model, sf->mtx_view, NULL);

      if(tz > 0.02) {
	matrix_translate(mtx_model, 0.0f, 0.0f, tz);
	engine_gldirect_draw_points(sf->shader_type, sf->pointsize,
				    NUM_STARS, sf->starcoords,
				    0.8f, 0.8f, 0.8f, sf->alpha,
				    mtx_model, sf->mtx_view, NULL);
      }

      if(tz > 0.07) {
	matrix_translate(mtx_model, 0.0f, 0.0f, tz);
	engine_gldirect_draw_points(sf->shader_type, sf->pointsize,
				    NUM_STARS, sf->starcoords,
				    0.8f, 0.8f, 0.8f, sf->alpha,
				    mtx_model, sf->mtx_view, NULL);
      }
    }
  }

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
}

object_t *starfield_create(object_t *ga_obj, double fadespeed, float *mtx_view)
{
  object_t *obj;
  starfield_t *sf;
  int i;

  DEBUG(DEBUG_CREATE, " ");

  sf = calloc(1, sizeof(*sf));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = starfield_destroy;
  obj->update_cb = starfield_update;
  obj->draw_cb = starfield_draw;
  obj->object = sf;

  sf->ga_obj = ga_obj;

  for(i = 0 ; i < NUM_STARS ; i++) {
    double x = (SCALEX * ((double)rand() / (double)RAND_MAX)) - SCALEX / 2;
    double y = (SCALEY * ((double)rand() / (double)RAND_MAX)) - SCALEY / 2;

    if(x > -1.05 && x <= 0.0 && y > -1.05 && y < 1.05)
      x -= 1.05 + (SCALEX / 2) * ((double)rand() / (double)RAND_MAX);
    if(x < 1.05 && x >= 0.0 && y > -1.05 && y < 1.05)
      x += 1.05 + (SCALEX / 2) * ((double)rand() / (double)RAND_MAX);

    sf->starcoords[i * 3 + 0] = x;
    sf->starcoords[i * 3 + 1] = y;
    sf->starcoords[i * 3 + 2] = (gamearea_viewdist() + 5.0f) -
      (SCALEZ * ((double)rand() / (double)RAND_MAX));
  }

  sf->fadespeed = fadespeed;
  if(sf->fadespeed > 0.0)
    sf->alpha = 0.0;
  else
    sf->alpha = 1.0;

  if(gfx_details >= GFX_DETAILS_HIGH) {
    sf->pointsize = 3.0f * (screen_density + 1.0f);
    sf->shader_type = SHADERS_POINT_ROUND;
  } else {
    sf->pointsize = screen_density + 1.0f;
    sf->shader_type = SHADERS_POINT;
  }

  sf->mtx_view = mtx_view;

  return obj;
}
