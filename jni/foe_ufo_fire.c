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
#include "foe_ufo.h"
#include "foe_ufo_fire.h"
#include "object.h"

typedef struct foe_ufo_fire_s {
  object_t *ga_obj;

  int colidx;
  int section;

  GLfloat li[26 * 3];
  GLfloat tx, ty, tz;
  GLfloat angle, rotate;

  uint64_t timer_change_beam;
  uint64_t timer;
} foe_ufo_fire_t;

static const int num_vertices = 26;

static GLfloat colours[] = {
  0.10, 0.20, 0.50,
  0.10, 0.60, 0.60,
  0.50, 0.80, 0.50,
  0.70, 0.65, 0.00,
  0.70, 0.40, 0.00,
  0.60, 0.10, 0.00,
  0.60, 0.00, 0.50,
  0.40, 0.00, 0.60,
};
static int numcols = 8;

static void foe_ufo_fire_destroy(object_t *obj)
{
  foe_ufo_fire_t *uf = (foe_ufo_fire_t *)obj->object;

  free(uf);
  engine_globject_free(GLOBJ(0));
  free(obj->globjs);
  free(obj);
}

static int foe_ufo_fire_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  foe_ufo_fire_t *uf = (foe_ufo_fire_t *)obj->object;
  float diff = msec - prev_msec;
  int i;
  double dz;

  uf->timer_change_beam += diff;
  uf->timer += diff;

  // End of beaming
  if(uf->timer >= FOE_UFO_FIRE_LEN - 100)
    return 0;

  // Change beam
  if(uf->timer_change_beam < 50)
    return 1;

  uf->timer_change_beam = 0;
  uf->colidx = (uf->colidx + 1) % numcols;

  // Split the line into 25 sections, and randomize some offsets.
  dz = (10.0f + FOE_UFO_OFFSET) / 25.0f;
  for(i = 0 ; i < num_vertices ; i++) {
    uf->li[i * 3 + 0] = uf->tx;
    uf->li[i * 3 + 1] = uf->ty;
    uf->li[i * 3 + 2] = FOE_UFO_OFFSET - i * dz;
    if(i > 0) {
      uf->li[i * 3 + 0] += (0.14 * ((double)rand() / (double)RAND_MAX) - 0.07);
      uf->li[i * 3 + 1] += (0.14 * ((double)rand() / (double)RAND_MAX) - 0.07);
      uf->li[i * 3 + 2] += (0.06 * ((double)rand() / (double)RAND_MAX) - 0.03);
    }
  }

  engine_globject_update_verts(GLOBJ(0), num_vertices, uf->li);

  if(engine_collision_detect(obj, OBJECT_F_PLAYER, uf->section,
			     0.0f, FOE_UFO_OFFSET - 0.1f))
    return 0;

  return 1;
}

static void foe_ufo_fire_draw(object_t *obj)
{
  foe_ufo_fire_t *uf = (foe_ufo_fire_t *)obj->object;

  DEBUG(DEBUG_DRAW, " ");

  glDepthRangef(0.0, 0.9);
  glEnable(GL_BLEND);

  engine_globject_draw_prepare(GLOBJ(0));
  engine_globject_set_colour(GLOBJ(0),
			     colours[uf->colidx * 3 + 0],
			     colours[uf->colidx * 3 + 1],
			     colours[uf->colidx * 3 + 2]);

  glDrawArrays(GL_LINE_STRIP, 0, num_vertices);

  engine_globject_draw_cleanup(GLOBJ(0));

  glDisable(GL_BLEND);
  glDepthRangef(0.0, 1.0);
}

static void foe_ufo_fire_reinit(object_t *obj, int doalloc)
{
  foe_ufo_fire_t *uf = (foe_ufo_fire_t *)obj->object;

  if(doalloc) {
    engine_globject_alloc(GLOBJ(0));
  }

  engine_globject_init(GLOBJ(0), SHADERS_LINE,
		       num_vertices, uf->li, 1,
		       0, NULL, 0,
		       0, NULL, 0);
}

object_t *foe_ufo_fire_create(object_t *ga_obj, int section)
{
  object_t *obj;
  foe_ufo_fire_t *uf;

  DEBUG(DEBUG_CREATE, " ");

  uf = calloc(1, sizeof(*uf));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = foe_ufo_fire_destroy;
  obj->update_cb = foe_ufo_fire_update;
  obj->draw_cb = foe_ufo_fire_draw;
  obj->reinit_cb = foe_ufo_fire_reinit;
  obj->object = uf;

  FLAG_SET(obj->flags, OBJECT_F_FOE_FRIER);

  obj->num_globjs = 1;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  foe_ufo_fire_reinit(obj, 1);

  GLOBJ(0)->alpha = 1.0f;
  matrix_identity(GLOBJ(0)->mtx_model);

  uf->ga_obj = ga_obj;

  DEBUG(DEBUG_TEMP, "section=%d", section);

  uf->section = section;
  gamearea_section_pos_get(uf->ga_obj, uf->section, &uf->tx, &uf->ty);
  uf->angle = gamearea_section_angle_get(uf->ga_obj, uf->section);
  uf->timer_change_beam = 1000;

  return obj;
}
