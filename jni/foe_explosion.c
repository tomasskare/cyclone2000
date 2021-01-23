#include <jni.h>
#include <android/log.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#include <GLES2/gl2.h>

#include "debug.h"
#include "native_main.h"
#include "engine.h"
#include "matrix.h"
#include "shaders.h"
#include "gamearea.h"
#include "foe_explosion.h"
#include "object.h"

#include "game_graphics/points_data.h"

typedef struct foe_explosion_s {
  object_t *ga_obj;

  int section;
  int all;

  uint64_t timer;

  GLfloat tx, ty;
  GLfloat z1, z2, z3;
  GLfloat r1, r2, r3;
  GLfloat s1, s2, s3;
  GLfloat a1, a2, a3;

  GLfloat secrot;

  int numpoints[3];
} foe_explosion_t;

#define GLOBJ_R1 0
#define GLOBJ_R2 1
#define GLOBJ_R3 2

static void foe_explosion_destroy(object_t *obj)
{
  foe_explosion_t *fe = (foe_explosion_t *)obj->object;
  int i;

  free(fe);
  for(i = 0 ; i < obj->num_globjs ; i++)
    engine_globject_free(GLOBJ(i));
  free(obj->globjs);
  free(obj);
}

static int foe_explosion_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  foe_explosion_t *fe = (foe_explosion_t *)obj->object;
  float diff = msec - prev_msec;

  DEBUG(DEBUG_UPDATE, " ");

  fe->timer += diff;

  if(fe->timer > 1200)
    return 0;

  fe->z1 += diff / 800;
  fe->s1 += diff / 1800;
  fe->r1 += diff / 30;
  GLOBJ(GLOBJ_R1)->alpha -= diff / 1200;
  if(GLOBJ(GLOBJ_R1)->alpha < 0.0)
    GLOBJ(GLOBJ_R1)->alpha = 0.0;

  if(!fe->all)
    return 1;

  if(fe->timer < 200)
    return 1;

  fe->z2 += diff / 900;
  fe->s2 += diff / 2000;
  fe->r2 += diff / 40;
  GLOBJ(GLOBJ_R2)->alpha -= diff / 1000;
  if(GLOBJ(GLOBJ_R2)->alpha < 0.0)
    GLOBJ(GLOBJ_R2)->alpha = 0.0;

  if(fe->timer < 400)
    return 1;

  fe->z3 += diff / 1000;
  fe->s3 += diff / 2200;
  fe->r3 += diff / 50;
  GLOBJ(GLOBJ_R3)->alpha -= diff / 600;
  if(GLOBJ(GLOBJ_R3)->alpha < 0.0)
    GLOBJ(GLOBJ_R3)->alpha = 0.0;

  return 1;
}

static void foe_explosion_draw(object_t *obj)
{
  foe_explosion_t *fe = (foe_explosion_t *)obj->object;

  DEBUG(DEBUG_DRAW, " ");

  glDepthRangef(0.0, 0.95);
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);

  // smallest ring
  matrix_set_translate(GLOBJ(GLOBJ_R1)->mtx_model, fe->tx, fe->ty, fe->z1);
  matrix_rotate(GLOBJ(GLOBJ_R1)->mtx_model, MTX_AXIS_Z, fe->secrot);
  matrix_scale(GLOBJ(GLOBJ_R1)->mtx_model, fe->s1, fe->s1, fe->s1);
  matrix_rotate(GLOBJ(GLOBJ_R1)->mtx_model, MTX_AXIS_Z, fe->r1);
  engine_globject_draw_prepare(GLOBJ(GLOBJ_R1));
  engine_globject_set_colour(GLOBJ(GLOBJ_R1), 0.2f, 0.8f, 0.4f);
  glDrawArrays(GL_POINTS, 0, fe->numpoints[GLOBJ_R1]);
  engine_globject_draw_cleanup(GLOBJ(GLOBJ_R1));

  if(fe->all) {
    // middle ring
    matrix_set_translate(GLOBJ(GLOBJ_R2)->mtx_model, fe->tx, fe->ty, fe->z2);
    matrix_rotate(GLOBJ(GLOBJ_R2)->mtx_model, MTX_AXIS_Z, fe->secrot);
    matrix_scale(GLOBJ(GLOBJ_R2)->mtx_model, fe->s2, fe->s2, fe->s2);
    matrix_rotate(GLOBJ(GLOBJ_R2)->mtx_model, MTX_AXIS_Z, fe->r2);
    engine_globject_draw_prepare(GLOBJ(GLOBJ_R2));
    engine_globject_set_colour(GLOBJ(GLOBJ_R2), 0.2f, 0.8f, 0.4f);
    glDrawArrays(GL_POINTS, 0, fe->numpoints[GLOBJ_R2]);
    engine_globject_draw_cleanup(GLOBJ(GLOBJ_R2));

    // biggest ring
    matrix_set_translate(GLOBJ(GLOBJ_R3)->mtx_model, fe->tx, fe->ty, fe->z3);
    matrix_rotate(GLOBJ(GLOBJ_R3)->mtx_model, MTX_AXIS_Z, fe->secrot);
    matrix_scale(GLOBJ(GLOBJ_R3)->mtx_model, fe->s3, fe->s3, fe->s3);
    matrix_rotate(GLOBJ(GLOBJ_R3)->mtx_model, MTX_AXIS_Z, fe->r3);
    engine_globject_draw_prepare(GLOBJ(GLOBJ_R3));
    engine_globject_set_colour(GLOBJ(GLOBJ_R3), 0.2f, 0.8f, 0.4f);
    glDrawArrays(GL_POINTS, 0, fe->numpoints[GLOBJ_R3]);
    engine_globject_draw_cleanup(GLOBJ(GLOBJ_R3));
  }

  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  glDepthRangef(0.0, 1.0);
}

static void foe_explosion_reinit(object_t *obj, int doalloc)
{
  foe_explosion_t *fe = (foe_explosion_t *)obj->object;
  int shader_type;

  if(doalloc) {
    engine_globject_alloc(GLOBJ(GLOBJ_R1));
  }

  if(gfx_details >= GFX_DETAILS_HIGH) {
    shader_type = SHADERS_POINT_ROUND;

    engine_globject_init(GLOBJ(GLOBJ_R1), shader_type,
			 16, points_16, 0,
			 0, NULL, 0, 0, NULL, 0);
    fe->numpoints[GLOBJ_R1] = 16;

    if(fe->all) {
      if(doalloc) {
	engine_globject_alloc(GLOBJ(GLOBJ_R2));
	engine_globject_alloc(GLOBJ(GLOBJ_R3));
      }

      engine_globject_init(GLOBJ(GLOBJ_R2), shader_type,
			   18, points_18, 0,
			   0, NULL, 0,
			   0, NULL, 0);
      engine_globject_init(GLOBJ(GLOBJ_R3), shader_type,
			   20, points_20, 0,
			   0, NULL, 0,
			   0, NULL, 0);
      fe->numpoints[GLOBJ_R2] = 18;
      fe->numpoints[GLOBJ_R3] = 20;
    }
  } else {
    shader_type = SHADERS_POINT;

    engine_globject_init(GLOBJ(GLOBJ_R1), shader_type,
			 12, points_12, 0,
			 0, NULL, 0, 0, NULL, 0);
    fe->numpoints[GLOBJ_R1] = 12;

    if(fe->all) {
      if(doalloc) {
	engine_globject_alloc(GLOBJ(GLOBJ_R2));
	engine_globject_alloc(GLOBJ(GLOBJ_R3));
      }

      engine_globject_init(GLOBJ(GLOBJ_R2), shader_type,
			   14, points_14, 0,
			   0, NULL, 0,
			   0, NULL, 0);
      engine_globject_init(GLOBJ(GLOBJ_R3), shader_type,
			   16, points_16, 0,
			   0, NULL, 0,
			   0, NULL, 0);
      fe->numpoints[GLOBJ_R2] = 14;
      fe->numpoints[GLOBJ_R3] = 16;
    }
  }
}

object_t *foe_explosion_create(object_t *ga_obj, int section, double z, int all)
{
  object_t *obj;
  foe_explosion_t *fe;

  DEBUG(DEBUG_CREATE, " ");

  fe = calloc(1, sizeof(*fe));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = foe_explosion_destroy;
  obj->update_cb = foe_explosion_update;
  obj->draw_cb = foe_explosion_draw;
  obj->reinit_cb = foe_explosion_reinit;
  obj->object = fe;

  fe->ga_obj = ga_obj;

  if(all)
    obj->num_globjs = 3;
  else
    obj->num_globjs = 1;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  fe->all = all;

  foe_explosion_reinit(obj, 1);

  // Start at given section and z position
  fe->section = section;
  gamearea_section_pos_get(fe->ga_obj, fe->section, &fe->tx, &fe->ty);
  fe->z1 = fe->z2 = fe->z3 = z;
  fe->r1 = fe->r2 = fe->r3 = 0;
  fe->s1 = fe->s2 = fe->s3 = 0.05 - z / 100;
  GLOBJ(GLOBJ_R1)->alpha = 1.0 + z / 40;
  if(all) {
    GLOBJ(GLOBJ_R2)->alpha = 1.0 + z / 40;
    GLOBJ(GLOBJ_R3)->alpha = 1.0 + z / 40;
  }

  fe->secrot = gamearea_section_angle_get(fe->ga_obj, fe->section);

  native_callback(CALLBACK_PLAY_SOUND, SOUND_EXPLOSION, 0, 0);

  return obj;
}
