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
#include "foe_mirror.h"
#include "foe_mirror_fire.h"
#include "banner.h"
#include "score.h"
#include "object.h"

#include "game_graphics/foe_mirror_data.h"

typedef struct foe_mirror_s {
  object_t *ga_obj;

  int section;
  int hitnum;
  int should_fire;
  uint64_t last_shot;

  GLfloat tx, ty, tz;
  GLfloat angle, rotate;

  int num_indices;
} foe_mirror_t;

static void foe_mirror_destroy(object_t *obj)
{
  foe_mirror_t *mi = (foe_mirror_t *)obj->object;

  free(mi);
  engine_globject_free(GLOBJ(0));
  free(obj->globjs);
  free(obj);
}

static int foe_mirror_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  foe_mirror_t *mi = (foe_mirror_t *)obj->object;
  float diff = msec - prev_msec;

  DEBUG(DEBUG_UPDATE, " ");

  /* Stop half way up the web and stay there. */
  if(mi->tz <= -4.5f) {
    mi->tz += diff / 200;
  }

  mi->rotate -= 360.0 * diff / 3600;

  if(mi->should_fire && msec - mi->last_shot > 150) {
    object_t *exobj = foe_mirror_fire_create(mi->ga_obj, mi->section, mi->tz);
    engine_object_add(exobj, 1);
    mi->should_fire = 0;
    mi->last_shot = msec;
  }

  return 1;
}

static void foe_mirror_draw(object_t *obj)
{
  foe_mirror_t *mi = (foe_mirror_t *)obj->object;

  DEBUG(DEBUG_DRAW, " ");

  glDepthRangef(0.0, 0.9);

  matrix_set_translate(GLOBJ(0)->mtx_model, mi->tx, mi->ty, mi->tz);
  matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Z, mi->angle + mi->rotate);
  engine_globject_draw_prepare(GLOBJ(0));
  glDrawElements(GL_TRIANGLES, mi->num_indices, GL_UNSIGNED_SHORT, 0);
  engine_globject_draw_cleanup(GLOBJ(0));

  glDepthRangef(0.0, 1.0);
}

static int foe_mirror_collision(object_t *obj, object_t *srcobj,
				int section, double minz, double maxz)
{
  foe_mirror_t *mi = (foe_mirror_t *)obj->object;

  if(mi->section != section)
    return 0;

  if((double)mi->tz < minz || (double)mi->tz > maxz)
    return 0;

  return 1;
}

static int foe_mirror_hit(object_t *obj, object_t *srcobj)
{
  foe_mirror_t *mi = (foe_mirror_t *)obj->object;

  if(!mi->should_fire)
    mi->hitnum++;

  if(mi->hitnum > 5) {

    /* Die on sixth hit. */

    object_t *ban;
    banner_coords_t *bn;
    int rnd;

    /* Randomize score 250, 500 or 750. */
    rnd = rand() % 3;
    bn = &score_banners[rnd];
    score_increase(250 * (rnd + 1));

    ban = banner_create(mi->ga_obj, native_tex_id(TEX_FUSEBALL_SCORE),
			0, mi->tx, mi->ty, mi->tz,
			bn->tx1, bn->tx2, bn->ty1, bn->ty2, bn->w, bn->h,
			BANNER_F_DONT_COMPENSATE);
    engine_object_add(ban, 2);

    native_callback(CALLBACK_PLAY_SOUND, SOUND_EXPLOSION, 0, 0);

    return 1;

  } else {

    /* Hits 1-5, send a mirrored fire back. */

    mi->should_fire = 1;

    return 0;

  }
}

static void foe_mirror_posget(object_t *obj, int *section, double *zpos)
{
  foe_mirror_t *mi = (foe_mirror_t *)obj->object;

  if(section)
    *section = mi->section;
  if(zpos)
    *zpos = mi->tz;
}

static void foe_mirror_reinit(object_t *obj, int doalloc)
{
  foe_mirror_t *mi = (foe_mirror_t *)obj->object;

  if(doalloc) {
    engine_globject_alloc(GLOBJ(0));
  }

  if(gfx_details >= GFX_DETAILS_HIGH) {
    engine_globject_init_ex(GLOBJ(0), SHADERS_LIGHT1,
			    high_num_vertices, high_vertices, 0,
			    high_num_vertices, high_normals, 0,
			    high_num_vertices, high_colours, 0,
			    high_num_indices, high_indices, 0);
    mi->num_indices = high_num_indices;
  } else {
    engine_globject_init(GLOBJ(0), SHADERS_GOURAUD,
			 low_num_vertices, low_vertices, 0,
			 low_num_vertices, low_colours, 0,
			 low_num_indices, low_indices, 0);
    mi->num_indices = low_num_indices;
  }
}

object_t *foe_mirror_create(object_t *ga_obj, int section)
{
  object_t *obj;
  foe_mirror_t *mi;

  DEBUG(DEBUG_CREATE, " ");

  mi = calloc(1, sizeof(*mi));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = foe_mirror_destroy;
  obj->update_cb = foe_mirror_update;
  obj->draw_cb = foe_mirror_draw;
  obj->collision_cb = foe_mirror_collision;
  obj->hit_cb = foe_mirror_hit;
  obj->posget_cb = foe_mirror_posget;
  obj->reinit_cb = foe_mirror_reinit;
  obj->object = mi;

  FLAG_SET(obj->flags, OBJECT_F_FOE);
  FLAG_SET(obj->flags, OBJECT_F_NOZAP);

  obj->num_globjs = 1;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  foe_mirror_reinit(obj, 1);

  GLOBJ(0)->alpha = 1.0f;

  GLOBJ(0)->spec_colour[0] = 0.8f;
  GLOBJ(0)->spec_colour[1] = 0.8f;
  GLOBJ(0)->spec_colour[2] = 0.8f;
  GLOBJ(0)->spec_colour[3] = 1.0f;

  GLOBJ(0)->spec_shininess = 48.0f;

  GLOBJ(0)->light1 = &engine_light1;

  mi->ga_obj = ga_obj;

  mi->section = section;
  gamearea_section_pos_get(mi->ga_obj, mi->section, &mi->tx, &mi->ty);
  mi->tz = -10.0;
  mi->angle = gamearea_section_angle_get(mi->ga_obj, mi->section);

  return obj;
}
