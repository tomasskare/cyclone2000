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
#include "foe_mirror_fire.h"
#include "banner.h"
#include "score.h"
#include "object.h"

#include "game_graphics/foe_mirror_fire_data.h"

typedef struct foe_mirror_fire_s {
  object_t *ga_obj;

  int section;

  GLfloat tx, ty, tz;
  GLfloat angle;

  int num_indices;
} foe_mirror_fire_t;

static void foe_mirror_fire_destroy(object_t *obj)
{
  foe_mirror_fire_t *mf = (foe_mirror_fire_t *)obj->object;

  free(mf);
  engine_globject_free(GLOBJ(0));
  free(obj->globjs);
  free(obj);
}

static int foe_mirror_fire_update(object_t *obj, uint64_t prev_msec,
				  uint64_t msec)
{
  foe_mirror_fire_t *mf = (foe_mirror_fire_t *)obj->object;
  float diff = msec - prev_msec;

  DEBUG(DEBUG_UPDATE, " ");

  mf->tz += diff / 60;

  if(mf->tz >= 2.0)
    return 0;

  if(mf->tz >= 0.0) {
    GLOBJ(0)->alpha = (2.0 - mf->tz) / 2.0;
  }

  if(mf->tz >= 0.0f && engine_collision_detect(obj,
					       OBJECT_F_PLAYER, mf->section,
					       -1.0, (double)mf->tz))
    return 0;

  return 1;
}

static void foe_mirror_fire_draw(object_t *obj)
{
  foe_mirror_fire_t *mf = (foe_mirror_fire_t *)obj->object;

  DEBUG(DEBUG_DRAW, " ");

  glDepthRangef(0.0, 0.9);

  glEnable(GL_BLEND);

  matrix_set_translate(GLOBJ(0)->mtx_model, mf->tx, mf->ty, mf->tz);
  matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Z, mf->angle);
  engine_globject_draw_prepare(GLOBJ(0));
  glDrawElements(GL_TRIANGLES, mf->num_indices, GL_UNSIGNED_SHORT, 0);
  engine_globject_draw_cleanup(GLOBJ(0));

  glDisable(GL_BLEND);

  glDepthRangef(0.0, 1.0);
}

static int foe_mirror_fire_collision(object_t *obj, object_t *srcobj,
				int section, double minz, double maxz)
{
  return 0;
}

static int foe_mirror_fire_hit(object_t *obj, object_t *srcobj)
{
  return 0;
}

static void foe_mirror_fire_posget(object_t *obj, int *section, double *zpos)
{
  foe_mirror_fire_t *mf = (foe_mirror_fire_t *)obj->object;

  if(section)
    *section = mf->section;
  if(zpos)
    *zpos = mf->tz;
}

static void foe_mirror_fire_reinit(object_t *obj, int doalloc)
{
  foe_mirror_fire_t *mf = (foe_mirror_fire_t *)obj->object;

  if(doalloc) {
    engine_globject_alloc(GLOBJ(0));
  }

  if(gfx_details >= GFX_DETAILS_HIGH) {
    engine_globject_init_ex(GLOBJ(0), SHADERS_LIGHT1,
			    high_num_vertices, high_vertices, 0,
			    high_num_vertices, high_normals, 0,
			    high_num_vertices, high_colours, 0,
			    high_num_indices, high_indices, 0);
    mf->num_indices = high_num_indices;
  } else {
    engine_globject_init(GLOBJ(0), SHADERS_GOURAUD,
			 low_num_vertices, low_vertices, 0,
			 low_num_vertices, low_colours, 0,
			 low_num_indices, low_indices, 0);
    mf->num_indices = low_num_indices;
  }
}

object_t *foe_mirror_fire_create(object_t *ga_obj, int section, GLfloat zpos)
{
  object_t *obj;
  foe_mirror_fire_t *mf;

  DEBUG(DEBUG_CREATE, " ");

  mf = calloc(1, sizeof(*mf));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = foe_mirror_fire_destroy;
  obj->update_cb = foe_mirror_fire_update;
  obj->draw_cb = foe_mirror_fire_draw;
  obj->collision_cb = foe_mirror_fire_collision;
  obj->hit_cb = foe_mirror_fire_hit;
  obj->posget_cb = foe_mirror_fire_posget;
  obj->reinit_cb = foe_mirror_fire_reinit;
  obj->object = mf;

  FLAG_SET(obj->flags, OBJECT_F_FOE);
  FLAG_SET(obj->flags, OBJECT_F_FOE_FIRE);
  FLAG_SET(obj->flags, OBJECT_F_NOZAP);

  obj->num_globjs = 1;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  foe_mirror_fire_reinit(obj, 1);

  GLOBJ(0)->alpha = 1.0f;

  GLOBJ(0)->spec_colour[0] = 0.8f;
  GLOBJ(0)->spec_colour[1] = 0.8f;
  GLOBJ(0)->spec_colour[2] = 0.8f;
  GLOBJ(0)->spec_colour[3] = 1.0f;

  GLOBJ(0)->spec_shininess = 48.0f;

  GLOBJ(0)->light1 = &engine_light1;

  mf->ga_obj = ga_obj;

  mf->section = section;
  gamearea_section_pos_get(mf->ga_obj, mf->section, &mf->tx, &mf->ty);
  mf->tz = zpos;
  mf->angle = gamearea_section_angle_get(mf->ga_obj, mf->section);

  return obj;
}
