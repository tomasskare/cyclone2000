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
#include "foe_spikes.h"
#include "foe_fire.h"
#include "foe_explosion.h"
#include "score.h"
#include "object.h"

typedef struct foe_spikes_s {
  object_t *ga_obj;

  int section;

  GLfloat tx, ty;
  GLfloat angle, length;
} foe_spikes_t;

static void foe_spikes_destroy(object_t *obj)
{
  foe_spikes_t *fs = (foe_spikes_t *)obj->object;

  engine_spikes_free(fs->section);
  free(fs);
  engine_globject_free(GLOBJ(0));
  free(obj->globjs);
  free(obj);
}

static int foe_spikes_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  foe_spikes_t *fs = (foe_spikes_t *)obj->object;

  if(engine_collision_detect(obj,
			     OBJECT_F_PLAYER, fs->section,
			     -9.0 + fs->length, -9.0))
    return 0;

  return 1;
}

static void foe_spikes_draw(object_t *obj)
{
  foe_spikes_t *fs = (foe_spikes_t *)obj->object;

  DEBUG(DEBUG_DRAW, " ");

  matrix_set_translate(GLOBJ(0)->mtx_model, fs->tx, fs->ty, -9.0);
  matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Z, fs->angle);
  matrix_translate(GLOBJ(0)->mtx_model, 0.0f, 0.05f, 0.0f);

  engine_globject_draw_prepare(GLOBJ(0));
  glDrawArrays(GL_LINES, 0, 2);
  engine_globject_draw_cleanup(GLOBJ(0));
}

static int foe_spikes_collision(object_t *obj, object_t *srcobj,
				int section, double minz, double maxz)
{
  foe_spikes_t *fs = (foe_spikes_t *)obj->object;

  if(section != fs->section)
    return 0;

  if(-9.0 + fs->length < minz || -9.0 + fs->length > maxz)
    return 0;

  return 1;
}

static int foe_spikes_hit(object_t *obj, object_t *srcobj)
{
  foe_spikes_t *fs = (foe_spikes_t *)obj->object;

  if(fs->length < 0.4) {
    engine_object_add(foe_explosion_create(fs->ga_obj, fs->section,
					   -9.0, 1), 1);
    return 1;
  } else {
    engine_object_add(foe_explosion_create(fs->ga_obj, fs->section,
					   -9.0 + fs->length, 0), 1);
    fs->length -= 0.4;
  }

  score_increase(10);

  return 0;
}

static void foe_spikes_posget(object_t *obj, int *section, double *zpos)
{
  foe_spikes_t *fs = (foe_spikes_t *)obj->object;

  if(section)
    *section = fs->section;
  if(zpos)
    *zpos = fs->length - 9.0;
}

static void foe_spikes_reinit(object_t *obj, int doalloc)
{
  foe_spikes_t *fs = (foe_spikes_t *)obj->object;

  GLfloat line_vertices[] = {
    0.0, 0.0, 0.0,
    0.0, 0.0, fs->length,
  };

  GLfloat line_colours[] = {
    0.00f, 0.50f, 0.00f,
    0.00f, fs->length > 2.0 ? 0.85 : 0.35 * fs->length / 2.0 + 0.5, 0.00f,
  };

  if(doalloc) {
    engine_globject_alloc(GLOBJ(0));
  }

  engine_globject_init(GLOBJ(0), SHADERS_GOURAUD,
		       2, line_vertices, 1,
		       2, line_colours, 1,
		       0, NULL, 0);
}

object_t *foe_spikes_create(object_t *ga_obj, int section)
{
  object_t *obj;
  foe_spikes_t *fs;

  DEBUG(DEBUG_CREATE, "section=%d", section);

  fs = calloc(1, sizeof(*fs));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = foe_spikes_destroy;
  obj->update_cb = foe_spikes_update;
  obj->draw_cb = foe_spikes_draw;
  obj->collision_cb = foe_spikes_collision;
  obj->hit_cb = foe_spikes_hit;
  obj->posget_cb = foe_spikes_posget;
  obj->reinit_cb = foe_spikes_reinit;
  obj->object = fs;

  FLAG_SET(obj->flags, OBJECT_F_FOE);
  FLAG_SET(obj->flags, OBJECT_F_FOE_SPIKES);
  FLAG_SET(obj->flags, OBJECT_F_NOZAP);

  obj->num_globjs = 1;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  foe_spikes_reinit(obj, 1);

  GLOBJ(0)->alpha = 1.0f;

  fs->ga_obj = ga_obj;

  fs->section = section;
  gamearea_section_pos_get(fs->ga_obj, fs->section, &fs->tx, &fs->ty);
  fs->angle = gamearea_section_angle_get(fs->ga_obj, fs->section);

  return obj;
}

void foe_spikes_grow(object_t *obj, GLfloat length)
{
  foe_spikes_t *fs = (foe_spikes_t *)obj->object;

  fs->length += length;

  GLfloat line_vertices[] = {
    0.0, 0.0, 0.0,
    0.0, 0.0, fs->length,
  };

  GLfloat line_colours[] = {
    0.00f, 0.50f, 0.00f,
    0.00f, fs->length > 2.0 ? 0.85 : 0.35 * fs->length / 2.0 + 0.5, 0.00f,
  };

  engine_globject_update_verts(GLOBJ(0), 2, line_vertices);
  engine_globject_update_cols(GLOBJ(0), 2, line_colours);
}

GLfloat foe_spikes_length(object_t *obj)
{
  foe_spikes_t *fs = (foe_spikes_t *)obj->object;

  return fs->length;
}
