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
#include "foe_tanker_fuseball.h"
#include "foe_fuseball.h"
#include "foe_explosion_sphere.h"
#include "score.h"
#include "banner.h"
#include "object.h"

#include "game_graphics/foe_fuseball_data.h"

typedef struct foe_tanker_fuseball_s {
  object_t *ga_obj;

  int section;

  GLfloat tx, ty, tz;
  GLfloat angle, rotate;

  int num_indices;
} foe_tanker_fuseball_t;

static void foe_tanker_fuseball_destroy(object_t *obj)
{
  foe_tanker_fuseball_t *ftb = (foe_tanker_fuseball_t *)obj->object;
  int i;

  free(ftb);
  for(i = 0 ; i < obj->num_globjs ; i++)
    engine_globject_free(GLOBJ(i));
  free(obj->globjs);
  free(obj);
}

static int foe_tanker_fuseball_hit(object_t *obj, object_t *srcobj)
{
  foe_tanker_fuseball_t *ftb = (foe_tanker_fuseball_t *)obj->object;
  object_t *nobj;
  int edge2;

  // Killed by superzapper
  if(FLAG_TST(obj->flags, OBJECT_F_FORCE_KILL)) {
    object_t *ban;
    banner_coords_t *bn = &score_banners[SCORE_BANNER_750];

    score_increase(750);
    ban = banner_create(ftb->ga_obj, native_tex_id(TEX_FUSEBALL_SCORE),
			0, ftb->tx, ftb->ty, ftb->tz,
			bn->tx1, bn->tx2, bn->ty1, bn->ty2, bn->w, bn->h,
			BANNER_F_DONT_COMPENSATE);
    engine_object_add(ban, 2);
    native_callback(CALLBACK_PLAY_SOUND, SOUND_EXPLOSION, 0, 0);
    return 1;
  }

  nobj = foe_explosion_sphere_create(ftb->ga_obj, ftb->section, ftb->tz);
  engine_object_add(nobj, 1);

  nobj = foe_fuseball_create(ftb->ga_obj, ftb->section, ftb->tz);
  engine_object_add(nobj, 1);

  edge2 = gamearea_section_offset_get(ftb->ga_obj, ftb->section, 1);
  nobj = foe_fuseball_create(ftb->ga_obj, edge2, ftb->tz);
  engine_object_add(nobj, 1);

  score_increase(100);
  return 1;
}

static int foe_tanker_fuseball_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  foe_tanker_fuseball_t *ftb = (foe_tanker_fuseball_t *)obj->object;
  float diff = msec - prev_msec;

  DEBUG(DEBUG_UPDATE, " ");

  ftb->rotate -= diff * 360 / 4000;
  while(ftb->rotate < 0.0)
    ftb->rotate += 360.0;

  ftb->tz += diff / 900;

  if(ftb->tz >= 0.0f && engine_collision_detect(obj,
						OBJECT_F_PLAYER, ftb->section,
						-1.0, (double)ftb->tz))
    return 0;

  // When reaching the edge, split up.
  if(ftb->tz > GAMEAREA_FOE_EDGE_ZPOS) {
    ftb->tz = GAMEAREA_FOE_EDGE_ZPOS;
    foe_tanker_fuseball_hit(obj, NULL);
    return 0;
  }

  return 1;
}

static void foe_tanker_fuseball_draw(object_t *obj)
{
  foe_tanker_fuseball_t *ftb = (foe_tanker_fuseball_t *)obj->object;
  float tmpmtx[16];
  int i;

  DEBUG(DEBUG_DRAW, " ");

  matrix_set_translate(tmpmtx, ftb->tx, ftb->ty, ftb->tz);
  matrix_rotate(tmpmtx, MTX_AXIS_Z, ftb->angle);
  matrix_translate(tmpmtx, 0.0f, 0.22f, 0.0f);

  for(i = 0 ; i < 5 ; i++) {
    matrix_rotate_to(GLOBJ(i)->mtx_model, tmpmtx, MTX_AXIS_Z,
		     ftb->rotate + 72.0f * i);
    matrix_copy(GLOBJ(i + 5)->mtx_model, GLOBJ(i)->mtx_model);

    engine_globject_draw_prepare(GLOBJ(i));
    glDrawElements(GL_TRIANGLES, ftb->num_indices, GL_UNSIGNED_SHORT, 0);
    engine_globject_draw_cleanup(GLOBJ(i));

    engine_globject_draw_prepare(GLOBJ(i + 5));
    glDrawElements(GL_TRIANGLES, ftb->num_indices, GL_UNSIGNED_SHORT, 0);
    engine_globject_draw_cleanup(GLOBJ(i + 5));
  }
}

static int foe_tanker_fuseball_collision(object_t *obj, object_t *srcobj,
					 int section, double minz, double maxz)
{
  foe_tanker_fuseball_t *ftb = (foe_tanker_fuseball_t *)obj->object;

  if(ftb->section != section)
    return 0;

  if((double)ftb->tz < minz || (double)ftb->tz > maxz)
    return 0;

  return 1;
}

static void foe_tanker_fuseball_posget(object_t *obj, int *section, double *zpos)
{
  foe_tanker_fuseball_t *ftb = (foe_tanker_fuseball_t *)obj->object;

  if(section)
    *section = ftb->section;
  if(zpos)
    *zpos = ftb->tz;
}

static void foe_tanker_fuseball_reinit(object_t *obj, int doalloc)
{
  foe_tanker_fuseball_t *ftb = (foe_tanker_fuseball_t *)obj->object;
  int i;

  /* First five are left flip, other five are right flip. */
  for(i = 0 ; i < 5 ; i++) {
    if(doalloc) {
      engine_globject_alloc(GLOBJ(i));
      engine_globject_alloc(GLOBJ(i + 5));
    }

    if(gfx_details >= GFX_DETAILS_HIGH) {
      engine_globject_init_ex(GLOBJ(i), SHADERS_LIGHT1,
			      high_num_vertices, high_l_vertices, 0,
			      high_num_vertices, high_l_normals, 0,
			      high_num_vertices, high_colours[i], 0,
			      high_num_indices, high_l_indices, 0);

      engine_globject_init_ex(GLOBJ(i + 5), SHADERS_LIGHT1,
			      high_num_vertices, high_r_vertices, 0,
			      high_num_vertices, high_r_normals, 0,
			      high_num_vertices, high_colours[i], 0,
			      high_num_indices, high_r_indices, 0);

      ftb->num_indices = high_num_indices;
    } else {
      engine_globject_init(GLOBJ(i), SHADERS_LIGHT1,
			   low_num_vertices, low_l_vertices, 0,
			   low_num_vertices, low_colours[i], 0,
			   low_num_indices, low_l_indices, 0);

      engine_globject_init(GLOBJ(i + 5), SHADERS_LIGHT1,
			   low_num_vertices, low_r_vertices, 0,
			   low_num_vertices, low_colours[i], 0,
			   low_num_indices, low_r_indices, 0);

      ftb->num_indices = low_num_indices;
    }
  }
}

object_t *foe_tanker_fuseball_create(object_t *ga_obj, int section)
{
  object_t *obj;
  foe_tanker_fuseball_t *ftb;
  int i;

  DEBUG(DEBUG_CREATE, " ");

  ftb = calloc(1, sizeof(*ftb));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = foe_tanker_fuseball_destroy;
  obj->update_cb = foe_tanker_fuseball_update;
  obj->draw_cb = foe_tanker_fuseball_draw;
  obj->collision_cb = foe_tanker_fuseball_collision;
  obj->hit_cb = foe_tanker_fuseball_hit;
  obj->posget_cb = foe_tanker_fuseball_posget;
  obj->reinit_cb = foe_tanker_fuseball_reinit;
  obj->object = ftb;

  FLAG_SET(obj->flags, OBJECT_F_FOE);
  FLAG_SET(obj->flags, OBJECT_F_FOE_FRIER);

  /* Five colours, two flips */
  obj->num_globjs = 5 * 2;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  foe_tanker_fuseball_reinit(obj, 1);

  for(i = 0 ; i < 5 * 2 ; i++) {
    GLOBJ(i)->alpha = 1.0f;

    GLOBJ(i)->spec_colour[0] = 0.8f;
    GLOBJ(i)->spec_colour[1] = 0.8f;
    GLOBJ(i)->spec_colour[2] = 0.8f;
    GLOBJ(i)->spec_colour[3] = 1.0f;

    GLOBJ(i)->spec_shininess = 16.0f;

    GLOBJ(i)->light1 = &engine_light1;
  }

  ftb->ga_obj = ga_obj;

  ftb->section = section;
  gamearea_section_pos_get(ftb->ga_obj, ftb->section, &ftb->tx, &ftb->ty);
  ftb->tz = -10.0f;
  ftb->angle = gamearea_section_angle_get(ftb->ga_obj, ftb->section);

  return obj;
}
