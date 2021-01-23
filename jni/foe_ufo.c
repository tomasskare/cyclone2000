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
#include "foe_explosion.h"
#include "banner.h"
#include "score.h"
#include "object.h"

#include "game_graphics/foe_ufo_data.h"

typedef struct foe_ufo_s {
  object_t *ga_obj;

  int section, tosection;
  int hitnum;
  int moving;
  int shooting;
  int movedir;
  uint64_t last_event;
  uint64_t timer;
  uint64_t bounce_timer;

  GLfloat tx, ty, tz;
  GLfloat totx, toty;
  GLfloat fromtx, fromty;
  GLfloat toangle, fromangle;
  GLfloat angle, rotate;

  int num_indices;
} foe_ufo_t;

static void foe_ufo_destroy(object_t *obj)
{
  foe_ufo_t *fu = (foe_ufo_t *)obj->object;

  free(fu);
  engine_globject_free(GLOBJ(0));
  free(obj->globjs);
  free(obj);
}

static int foe_ufo_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  foe_ufo_t *fu = (foe_ufo_t *)obj->object;
  float diff = msec - prev_msec;

  DEBUG(DEBUG_UPDATE, " ");

  if(fu->moving)
    fu->rotate += 360.0 * diff / 3600;
  else
    fu->rotate += 360.0 * diff / 7200;

  fu->timer += diff;

  /* Bouncing back */
  if(fu->bounce_timer > 0) {
    float bdiff = (float)fu->timer - (float)fu->bounce_timer;

    fu->tz -= 0.8f * diff / bdiff;
    if(bdiff > 800) {
      fu->bounce_timer = 0;
      return 1;
    }
  }

  /* Stop above the web. */
  if(fu->tz < FOE_UFO_OFFSET) {
    fu->tz += diff / 200;
    return 1;
  } else if(fu->hitnum < 10) {
    /* Above the web. From here, it only takes one hit.
     * It can also be killed by the super zapper now.
     */
    fu->tz = FOE_UFO_OFFSET;
    fu->hitnum = 10;
    fu->last_event = fu->timer;
    FLAG_CLR(obj->flags, OBJECT_F_NOZAP);
  }

  // Move along the edge at a quick pace.
  if(fu->section != fu->tosection) {
    GLfloat dtx = (fu->totx - fu->fromtx) * diff / FOE_UFO_MOVE_LEN;
    GLfloat dty = (fu->toty - fu->fromty) * diff / FOE_UFO_MOVE_LEN;
    GLfloat angdiff = fu->toangle - fu->fromangle;
    GLfloat dang;

    if(angdiff > 180.0)
      angdiff -= 360.0;
    else if(angdiff < -180.0)
      angdiff = -angdiff - 360;

    dang = angdiff * diff / FOE_UFO_MOVE_LEN;

    if(fabs(fu->totx - fu->tx) < fabs(dtx) ||
       fabs(fu->toty - fu->ty) < fabs(dty)) {
      fu->tx = fu->totx;
      fu->ty = fu->toty;
      fu->angle = fu->toangle;
      fu->section = fu->tosection;
    } else {
      fu->tx += dtx;
      fu->ty += dty;
      fu->angle += dang;
    }
  } else {
    if(fu->shooting && fu->timer - fu->last_event > FOE_UFO_FIRE_LEN) {

      fu->last_event = fu->timer;

      fu->shooting = 0;
      fu->moving = 1;

      fu->tosection = gamearea_section_offset_get(fu->ga_obj, fu->section,
						  fu->movedir);
      if(fu->tosection == fu->section) {
	// We hit an edge, turn around
	fu->movedir = -fu->movedir;
	fu->tosection = gamearea_section_offset_get(fu->ga_obj, fu->section,
						    fu->movedir);
      }
      fu->fromtx = fu->tx;
      fu->fromty = fu->ty;
      gamearea_section_pos_get(fu->ga_obj, fu->tosection, &fu->totx, &fu->toty);
      fu->fromangle = fu->angle;
      fu->toangle = gamearea_section_angle_get(fu->ga_obj, fu->tosection);

    } else if(!fu->shooting && fu->timer - fu->last_event > FOE_UFO_MOVE_LEN) {
      object_t *exp;

      fu->last_event = fu->timer;

      fu->shooting = 1;
      fu->moving = 0;
      exp = foe_ufo_fire_create(fu->ga_obj, fu->section);
      engine_object_add(exp, 1);

    }
  }

  return 1;
}

static void foe_ufo_draw(object_t *obj)
{
  foe_ufo_t *fu = (foe_ufo_t *)obj->object;

  DEBUG(DEBUG_DRAW, " ");

  glDepthRangef(0.0, 0.9);

  matrix_set_translate(GLOBJ(0)->mtx_model, fu->tx, fu->ty, fu->tz);
  matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Z, fu->angle + fu->rotate);
  engine_globject_draw_prepare(GLOBJ(0));
  glDrawElements(GL_TRIANGLES, fu->num_indices, GL_UNSIGNED_SHORT, 0);
  engine_globject_draw_cleanup(GLOBJ(0));

  glDepthRangef(0.0, 1.0);
}

static int foe_ufo_collision(object_t *obj, object_t *srcobj,
				int section, double minz, double maxz)
{
  foe_ufo_t *fu = (foe_ufo_t *)obj->object;

  if(fu->section != section)
    return 0;

  if((double)fu->tz < minz || (double)fu->tz > maxz)
    return 0;

  return 1;
}

static int foe_ufo_hit(object_t *obj, object_t *srcobj)
{
  foe_ufo_t *fu = (foe_ufo_t *)obj->object;

  if(fu->hitnum > 6) {

    /* Die on seventh hit. */

    object_t *ban;
    banner_coords_t *bn;
    int rnd;

    /* Randomize score 250, 500 or 750. */
    rnd = rand() % 3;
    bn = &score_banners[rnd];
    score_increase(250 * (rnd + 1));

    ban = banner_create(fu->ga_obj, native_tex_id(TEX_FUSEBALL_SCORE),
			0, fu->tx, fu->ty, fu->tz,
			bn->tx1, bn->tx2, bn->ty1, bn->ty2, bn->w, bn->h,
			BANNER_F_DONT_COMPENSATE);
    engine_object_add(ban, 2);

    native_callback(CALLBACK_PLAY_SOUND, SOUND_EXPLOSION, 0, 0);

    return 1;

  } else {

    /* Hits 1-6, bounce back */
    fu->hitnum++;
    fu->bounce_timer = fu->timer;
    engine_object_add(foe_explosion_create(fu->ga_obj, fu->section,
					   fu->tz, 0), 1);

    return 0;

  }
}

static void foe_ufo_posget(object_t *obj, int *section, double *zpos)
{
  foe_ufo_t *fu = (foe_ufo_t *)obj->object;

  if(section)
    *section = fu->section;
  if(zpos)
    *zpos = fu->tz;
}

static void foe_ufo_reinit(object_t *obj, int doalloc)
{
  foe_ufo_t *fu = (foe_ufo_t *)obj->object;

  if(doalloc) {
    engine_globject_alloc(GLOBJ(0));
  }

  if(gfx_details >= GFX_DETAILS_HIGH) {
    engine_globject_init_ex(GLOBJ(0), SHADERS_LIGHT1,
			    high_num_vertices, high_vertices, 0,
			    high_num_vertices, high_normals, 0,
			    high_num_vertices, high_colours, 0,
			    high_num_indices, high_indices, 0);
    fu->num_indices = high_num_indices;
  } else {
    engine_globject_init(GLOBJ(0), SHADERS_GOURAUD,
			 low_num_vertices, low_vertices, 0,
			 low_num_vertices, low_colours, 0,
			 low_num_indices, low_indices, 0);
    fu->num_indices = low_num_indices;
  }
}

object_t *foe_ufo_create(object_t *ga_obj, int section)
{
  object_t *obj;
  foe_ufo_t *fu;

  DEBUG(DEBUG_CREATE, " ");

  fu = calloc(1, sizeof(*fu));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = foe_ufo_destroy;
  obj->update_cb = foe_ufo_update;
  obj->draw_cb = foe_ufo_draw;
  obj->collision_cb = foe_ufo_collision;
  obj->hit_cb = foe_ufo_hit;
  obj->posget_cb = foe_ufo_posget;
  obj->reinit_cb = foe_ufo_reinit;
  obj->object = fu;

  FLAG_SET(obj->flags, OBJECT_F_FOE);
  FLAG_SET(obj->flags, OBJECT_F_NOZAP);

  obj->num_globjs = 1;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  foe_ufo_reinit(obj, 1);

  GLOBJ(0)->alpha = 1.0f;

  GLOBJ(0)->spec_colour[0] = 0.8f;
  GLOBJ(0)->spec_colour[1] = 0.8f;
  GLOBJ(0)->spec_colour[2] = 0.8f;
  GLOBJ(0)->spec_colour[3] = 1.0f;

  GLOBJ(0)->spec_shininess = 48.0f;

  GLOBJ(0)->light1 = &engine_light1;

  fu->ga_obj = ga_obj;

  fu->tosection = fu->section = section;
  gamearea_section_pos_get(fu->ga_obj, fu->section, &fu->tx, &fu->ty);
  fu->tz = -10.0;
  fu->angle = gamearea_section_angle_get(fu->ga_obj, fu->section);
  fu->movedir = 1;

  return obj;
}
