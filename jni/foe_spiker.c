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
#include "matrix.h"
#include "shaders.h"
#include "gamearea.h"
#include "foe_spiker.h"
#include "foe_spikes.h"
#include "foe_fire.h"
#include "foe_explosion.h"
#include "score.h"
#include "object.h"

typedef struct foe_spiker_s {
  object_t *ga_obj;

  int section;
  int isbuilding;
  int ishidden;
  int num;

  GLfloat tx, ty, tz;
  GLfloat angle, rotate;
  GLfloat length;

  double timer;

  int currstage;
} foe_spiker_t;

#define SCALEX 0.070f
#define SCALEY 0.120f
#define SCALEZ 0.100f

static GLfloat vertices[] = {
    0.007f * SCALEX, -0.115f * SCALEY, 0.000f * SCALEZ,
    0.250f * SCALEX, -0.579f * SCALEY, -0.500f * SCALEZ,
    0.721f * SCALEX, 0.410f * SCALEY, -0.500f * SCALEZ,
    -0.757f * SCALEX, -0.111f * SCALEY, -0.500f * SCALEZ,
    -0.215f * SCALEX, 0.280f * SCALEY, -0.500f * SCALEZ,
    -0.630f * SCALEX, 0.206f * SCALEY, -0.151f * SCALEZ,
    -1.000f * SCALEX, 0.407f * SCALEY, -0.500f * SCALEZ,
    -0.522f * SCALEX, 0.510f * SCALEY, -0.500f * SCALEZ,
    -0.737f * SCALEX, 0.671f * SCALEY, -0.233f * SCALEZ,
    -0.753f * SCALEX, 0.873f * SCALEY, -0.500f * SCALEZ,
    -0.457f * SCALEX, 0.722f * SCALEY, -0.500f * SCALEZ,
    -0.404f * SCALEX, 0.889f * SCALEY, -0.377f * SCALEZ,
    -0.117f * SCALEX, 1.000f * SCALEY, -0.500f * SCALEZ,
    -0.036f * SCALEX, 0.925f * SCALEY, -0.500f * SCALEZ,
    0.425f * SCALEX, 0.982f * SCALEY, -0.603f * SCALEZ,
};
static int num_vertices = 15;

static GLfloat poly_colours[] = {
  0.00f, 0.85f, 0.00f,
  0.00f, 0.50f, 0.00f,
  0.00f, 0.50f, 0.00f,
  0.00f, 0.60f, 0.00f,
  0.00f, 0.70f, 0.00f,
  0.00f, 0.80f, 0.00f,
  0.00f, 0.70f, 0.00f,
  0.00f, 0.60f, 0.00f,
  0.00f, 0.80f, 0.00f,
  0.00f, 0.70f, 0.00f,
  0.00f, 0.50f, 0.00f,
  0.00f, 0.80f, 0.00f,
  0.00f, 0.70f, 0.00f,
  0.00f, 0.70f, 0.00f,
  0.00f, 0.65f, 0.00f,
};
static const int num_colours = 15;

static GLushort indices[] = {
    0, 2, 1,
    0, 1, 3,
    2, 0, 4,
    4, 0, 5,
    0, 3, 5,
    5, 3, 6,
    4, 5, 7,
    8, 5, 6,
    7, 5, 8,
    8, 6, 9,
    10, 7, 8,
    8, 9, 11,
    8, 11, 10,
    11, 9, 12,
    13, 10, 11,
    13, 11, 12,
    13, 12, 14,
};
static const int num_poly_indices = 17 * 3;

static void spiker_reset(object_t *obj)
{
  foe_spiker_t *fs = (foe_spiker_t *)obj->object;
  object_t *fss;

  fs->ishidden = 0;
  fs->isbuilding = 1;
  fs->section = rand() % gamearea_num_sections_get(fs->ga_obj);
  gamearea_section_pos_get(fs->ga_obj, fs->section, &fs->tx, &fs->ty);
  fs->tz = -9.0;
  fs->angle = gamearea_section_angle_get(fs->ga_obj, fs->section);

  fss = engine_spikes_get_or_create(fs->section, 1);
  fs->length = foe_spikes_length(fss);
  fs->length += 1.0 + 2.5 * (GLfloat)rand() / (GLfloat)RAND_MAX;
}


static void foe_spiker_destroy(object_t *obj)
{
  foe_spiker_t *fs = (foe_spiker_t *)obj->object;

  free(fs);
  engine_globject_free(GLOBJ(0));
  free(obj->globjs);
  free(obj);
}

static int foe_spiker_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  foe_spiker_t *fs = (foe_spiker_t *)obj->object;
  float diff = msec - prev_msec;
  GLfloat speed = fs->currstage < 49 ? 800 : 500;

  DEBUG(DEBUG_UPDATE, " ");

  if(fs->ishidden) {
    double wait = fs->currstage < 49 ? 500 : 200;

    fs->timer += diff;
    if(fs->timer < wait)
      return 1;

    spiker_reset(obj);
  }

  if(fs->isbuilding) {
    object_t *fss;
    GLfloat fss_len;

    fs->tz += diff / speed;

    fss = engine_spikes_get_or_create(fs->section, 0);
    if(fss) {
      fss_len = foe_spikes_length(fss);
      if(fss_len < fs->tz + 9.0 - 0.05)
	foe_spikes_grow(fss, fs->tz + 9.0 - 0.05 - fss_len);
    }

    if(fs->tz > -9.0 + fs->length) {
      fs->isbuilding = 0;
      engine_object_add(foe_fire_create(fs->ga_obj, fs->section, fs->tz), 1);
    }
  } else {
    fs->tz -= diff / speed;
    if(fs->tz < -9.0) {
      fs->ishidden = 1;
      fs->timer = 0;
      return 1;
    }
  }

  fs->rotate -= diff / 10;
  while(fs->rotate < 0.0)
    fs->rotate += 360.0;

  if(fs->tz < -8.5) {
    GLOBJ(0)->alpha = 2.0 * (fs->tz + 9.0);
    if(GLOBJ(0)->alpha < 0.0)
      GLOBJ(0)->alpha = 0.0;
  } else {
    GLOBJ(0)->alpha = 1.0;
  }

  return 1;
}

static void foe_spiker_draw(object_t *obj)
{
  foe_spiker_t *fs = (foe_spiker_t *)obj->object;

  DEBUG(DEBUG_DRAW, " ");

  if(fs->ishidden)
    return;

  glEnable(GL_BLEND);

  matrix_set_translate(GLOBJ(0)->mtx_model, fs->tx, fs->ty, fs->tz);
  matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Z, fs->angle);
  matrix_translate(GLOBJ(0)->mtx_model, 0.0f, 0.1f, 0.0f);
  matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Z, fs->rotate);

  engine_globject_draw_prepare(GLOBJ(0));
  glDrawElements(GL_TRIANGLES, num_poly_indices, GL_UNSIGNED_SHORT, 0);
  engine_globject_draw_cleanup(GLOBJ(0));

  glDisable(GL_BLEND);
}

static int foe_spiker_collision(object_t *obj, object_t *srcobj,
				int section, double minz, double maxz)
{
  foe_spiker_t *fs = (foe_spiker_t *)obj->object;

  if(section != fs->section)
    return 0;

  if((double)fs->tz < minz || (double)fs->tz > maxz)
    return 0;

  return 1;
}

static int foe_spiker_hit(object_t *obj, object_t *srcobj)
{
  foe_spiker_t *fs = (foe_spiker_t *)obj->object;
  object_t *exp;

  exp = foe_explosion_create(fs->ga_obj, fs->section, fs->tz, 1);
  engine_object_add(exp, 1);

  score_increase(50);

  return 1;
}

static void foe_spiker_posget(object_t *obj, int *section, double *zpos)
{
  foe_spiker_t *fs = (foe_spiker_t *)obj->object;

  if(section)
    *section = fs->section;
  if(zpos)
    *zpos = fs->tz;
}

static void foe_spiker_reinit(object_t *obj, int doalloc)
{
  if(doalloc) {
    engine_globject_alloc(GLOBJ(0));
  }

  engine_globject_init(GLOBJ(0), SHADERS_GOURAUD,
		       num_vertices, vertices, 0,
		       num_colours, poly_colours, 0,
		       num_poly_indices, indices, 0);
}

object_t *foe_spiker_create(object_t *ga_obj, int num)
{
  object_t *obj;
  foe_spiker_t *fs;

  DEBUG(DEBUG_CREATE, " ");

  fs = calloc(1, sizeof(*fs));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = foe_spiker_destroy;
  obj->update_cb = foe_spiker_update;
  obj->draw_cb = foe_spiker_draw;
  obj->collision_cb = foe_spiker_collision;
  obj->hit_cb = foe_spiker_hit;
  obj->posget_cb = foe_spiker_posget;
  obj->reinit_cb = foe_spiker_reinit;
  obj->object = fs;

  FLAG_SET(obj->flags, OBJECT_F_FOE);
  FLAG_SET(obj->flags, OBJECT_F_FOE_SPIKER);

  obj->num_globjs = 1;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  foe_spiker_reinit(obj, 1);

  GLOBJ(0)->alpha = 1.0f;

  fs->ga_obj = ga_obj;

  fs->num = num;
  spiker_reset(obj);

  fs->currstage = engine_state_stage_get() + 1;

  return obj;
}

int foe_spiker_get_num(object_t *obj)
{
  foe_spiker_t *fs = (foe_spiker_t *)obj->object;

  return fs->num;
}
