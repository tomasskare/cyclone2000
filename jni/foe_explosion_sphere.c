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
#include "foe_explosion_sphere.h"
#include "object.h"

#include "game_graphics/points_data.h"

typedef struct foe_explosion_sphere_s {
  object_t *ga_obj;

  int section;

  uint64_t timer;

  GLfloat tx, ty, tz;
  GLfloat s, r, a;

  GLfloat angle;
} foe_explosion_sphere_t;

static void foe_explosion_sphere_destroy(object_t *obj)
{
  foe_explosion_sphere_t *fes = (foe_explosion_sphere_t *)obj->object;

  free(fes);
  engine_globject_free(GLOBJ(0));
  free(obj->globjs);
  free(obj);
}

static int foe_explosion_sphere_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  foe_explosion_sphere_t *fes = (foe_explosion_sphere_t *)obj->object;
  float diff = msec - prev_msec;

  DEBUG(DEBUG_UPDATE, " ");

  fes->timer += diff;

  if(fes->timer > 600)
    return 0;

  fes->s += diff / 300;
  fes->r += diff / 20;
  GLOBJ(0)->alpha -= diff / 600;
  if(GLOBJ(0)->alpha < 0.0)
    GLOBJ(0)->alpha = 0.0;

  return 1;
}

static void foe_explosion_sphere_draw(object_t *obj)
{
  foe_explosion_sphere_t *fes = (foe_explosion_sphere_t *)obj->object;

  DEBUG(DEBUG_DRAW, " ");

  glDepthRangef(0.0, 0.95);
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);

  matrix_set_translate(GLOBJ(0)->mtx_model, fes->tx, fes->ty, fes->tz);
  matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Z, fes->angle);
  matrix_scale(GLOBJ(0)->mtx_model, fes->s, fes->s, fes->s);
  matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Z, fes->r);
  engine_globject_draw_prepare(GLOBJ(0));
  engine_globject_set_colour(GLOBJ(0), 0.2f, 0.7f, 0.5f);
  glDrawArrays(GL_POINTS, 0, num_points_sphere);
  engine_globject_draw_cleanup(GLOBJ(0));

  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  glDepthRangef(0.0, 1.0);
}

static void foe_explosion_sphere_reinit(object_t *obj, int doalloc)
{
  int shader_type;

  if(doalloc) {
    engine_globject_alloc(GLOBJ(0));
  }

  if(gfx_details >= GFX_DETAILS_HIGH)
    shader_type = SHADERS_POINT_ROUND;
  else
    shader_type = SHADERS_POINT;

  engine_globject_init(GLOBJ(0), shader_type,
		       num_points_sphere, points_sphere, 0,
		       0, NULL, 0,
		       0, NULL, 0);
}

object_t *foe_explosion_sphere_create(object_t *ga_obj, int section, double z)
{
  object_t *obj;
  foe_explosion_sphere_t *fes;

  DEBUG(DEBUG_CREATE, " ");

  fes = calloc(1, sizeof(*fes));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = foe_explosion_sphere_destroy;
  obj->update_cb = foe_explosion_sphere_update;
  obj->draw_cb = foe_explosion_sphere_draw;
  obj->reinit_cb = foe_explosion_sphere_reinit;
  obj->object = fes;

  fes->ga_obj = ga_obj;

  obj->num_globjs = 1;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  foe_explosion_sphere_reinit(obj, 1);

  // Start at given section and z position
  fes->section = section;
  gamearea_section_pos_get(fes->ga_obj, fes->section, &fes->tx, &fes->ty);
  fes->tz = z;
  fes->r = 0;
  fes->s = 0.02;
  GLOBJ(0)->alpha = 1.0;

  fes->angle = gamearea_section_angle_get(fes->ga_obj, fes->section);

  native_callback(CALLBACK_PLAY_SOUND, SOUND_EXPLOSION, 0, 0);

  return obj;
}
