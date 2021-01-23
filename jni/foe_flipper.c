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
#include "foe_flipper.h"
#include "foe_fire.h"
#include "foe_explosion.h"
#include "score.h"
#include "object.h"

#include "game_graphics/foe_flipper_data.h"

typedef struct foe_flipper_s {
  object_t *ga_obj;

  int ismutant;
  int section, tosection;
  int movedir, flipsteps;

  GLfloat tx, ty, tz;
  GLfloat rtx, rdir;
  GLfloat angle, rotate, toangle;
  GLfloat adiff;

  double timer;
  double skipcollision_timer;
  double flipspeed;

  GLfloat fire_distance, fire_distance2;
  int has_fired;

  int currstage;

  int numidxs;
} foe_flipper_t;

#define GLOBJ_NORMAL_1 0
#define GLOBJ_NORMAL_2 1
#define GLOBJ_NORMAL_3 2
#define GLOBJ_MUTANT_1 3
#define GLOBJ_MUTANT_2 4

static void foe_flipper_destroy(object_t *obj)
{
  foe_flipper_t *ff = (foe_flipper_t *)obj->object;

  free(ff);
  engine_globject_free(GLOBJ(0));
  free(obj->globjs);
  free(obj);
}

static int foe_flipper_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  foe_flipper_t *ff = (foe_flipper_t *)obj->object;
  float diff = msec - prev_msec;

  DEBUG(DEBUG_UPDATE, " ");

  // Has caught the player, special treatment
  if(FLAG_TST(obj->flags, OBJECT_F_PLAYER_CAUGHT)) {
    ff->tz -= 10.0f * diff / 1000;
    return 1;
  }

  if(ff->ismutant)
    ff->flipspeed = 300;
  else
    ff->flipspeed = 600;

  // When coming from tanker, rotate to right section
  if(FLAG_TST(obj->flags, OBJECT_F_NOKILL)) {
    ff->rotate -= ff->rdir * ff->adiff * diff / ff->flipspeed;
    if(ff->timer > ff->flipspeed || fabsf(ff->rotate) >= ff->adiff) {
      ff->section = ff->tosection;
      gamearea_section_pos_get(ff->ga_obj, ff->section, &ff->tx, &ff->ty);
      ff->angle = gamearea_section_angle_get(ff->ga_obj, ff->section);
      ff->timer = 0;
      ff->rotate = 0;

      ff->flipsteps--;
      if(ff->flipsteps) {
	ff->tosection = gamearea_section_offset_get(ff->ga_obj, ff->section,
						    ff->movedir);
	ff->rdir = -ff->movedir;
	ff->toangle = gamearea_section_angle_get(ff->ga_obj, ff->tosection);
	ff->rtx = gamearea_section_len_get(ff->ga_obj, ff->section);
	ff->adiff = gamearea_sections_angle_diff(ff->ga_obj, ff->section,
						 ff->movedir);
      } else {
	FLAG_CLR(obj->flags, OBJECT_F_NOKILL);
      }
    }

    ff->timer += diff;

    return 1;
  }


  // When on edge, flip toward player
  if(FLAG_TST(obj->flags, OBJECT_F_FOE_EDGE)) {

    if(ff->tosection == ff->section) {
      // Start a new flip

      // Move toward player.
      int closed = gamearea_closed_get(ff->ga_obj);
      int num_sections = gamearea_num_sections_get(ff->ga_obj);
      int player_section = gamearea_player_section_get(ff->ga_obj);
      int move_dir = player_section - ff->section;
      if(closed && (move_dir > num_sections / 2 ||
		    move_dir < -num_sections / 2))
	move_dir = -move_dir;

      if(move_dir != 0) {
	if(move_dir < 0)
	  move_dir = -1;
	else
	  move_dir = 1;

	ff->tosection = gamearea_section_offset_get(ff->ga_obj, ff->section,
						    move_dir);
	ff->rdir = -move_dir;
	ff->toangle = gamearea_section_angle_get(ff->ga_obj, ff->tosection);
	ff->rtx = gamearea_section_len_get(ff->ga_obj, ff->section);
	ff->timer = 0;
	ff->rotate = 0;
	ff->adiff = gamearea_sections_angle_diff(ff->ga_obj, ff->section,
						 move_dir);
      }

    } else {
      // Flipping fun
      ff->rotate -= ff->rdir * ff->adiff * diff / ff->flipspeed;

      if(ff->timer > ff->flipspeed || fabsf(ff->rotate) >= ff->adiff) {
	ff->section = ff->tosection;
	gamearea_section_pos_get(ff->ga_obj, ff->section, &ff->tx, &ff->ty);
	ff->angle = gamearea_section_angle_get(ff->ga_obj, ff->section);
	ff->skipcollision_timer = 80;
	native_callback(CALLBACK_PLAY_SOUND, SOUND_CLING, 0, 0);
      }

      ff->timer += diff;

    }

  } else {

    if(ff->ismutant)
      if(ff->currstage <= 64)
	ff->tz += diff / 400;
      else
	ff->tz += diff / 300;
    else
      ff->tz += diff / 500;

  }

  ff->skipcollision_timer -= diff;
  if(ff->skipcollision_timer < 0.0)
    ff->skipcollision_timer = 0.0;

  if(ff->skipcollision_timer == 0.0 && ff->tz >= 0.0f &&
     engine_collision_detect(obj,
			     OBJECT_F_PLAYER, ff->section,
			     -1.0, (double)ff->tz))
    return 0;


  if(!FLAG_TST(obj->flags, OBJECT_F_FOE_EDGE)) {
    // Change behaviour at rim
    if(ff->tz >= GAMEAREA_FOE_EDGE_ZPOS) {
      ff->tz = GAMEAREA_FOE_EDGE_ZPOS;
      engine_object_onedge(obj);
      return 1;
    }

    if(!ff->has_fired && ff->tz > ff->fire_distance && ff->tz < -6.0f) {
      object_t *fi_obj = foe_fire_create(ff->ga_obj, ff->section, ff->tz);
      engine_object_add(fi_obj, 1);
      ff->has_fired = 1;
    }

    if(ff->currstage > 16 && ff->has_fired == 1 &&
       ff->tz > ff->fire_distance2 && ff->tz < -4.0f) {
      object_t *fi_obj = foe_fire_create(ff->ga_obj, ff->section, ff->tz);
      engine_object_add(fi_obj, 1);
      ff->has_fired = 2;
    }
  }

  return 1;
}

static void foe_flipper_draw(object_t *obj)
{
  foe_flipper_t *ff = (foe_flipper_t *)obj->object;

  DEBUG(DEBUG_DRAW, " ");

  matrix_set_translate(GLOBJ(0)->mtx_model, ff->tx, ff->ty, ff->tz);
  matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Z, ff->angle);
  if(!FLAG_TST(obj->flags, OBJECT_F_FOE_EDGE))
    matrix_translate(GLOBJ(0)->mtx_model, 0.0f, 0.1f, 0.0f);

  if(ff->tosection != ff->section) {
    matrix_translate(GLOBJ(0)->mtx_model,
		     ff->rdir * ff->rtx / 2.0f, 0.0f, 0.0f);
    matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Z, ff->rotate);
    matrix_translate(GLOBJ(0)->mtx_model,
		     -ff->rdir * ff->rtx / 2.0f, 0.0f, 0.0f);
  }

  engine_globject_draw_prepare(GLOBJ(0));
  glDrawElements(GL_TRIANGLES, ff->numidxs, GL_UNSIGNED_SHORT, 0);
  engine_globject_draw_cleanup(GLOBJ(0));
}

static int foe_flipper_collision(object_t *obj, object_t *srcobj,
				 int section, double minz, double maxz)
{
  foe_flipper_t *ff = (foe_flipper_t *)obj->object;
  double margin = ff->ismutant ? 80 : 100;

  // Miss during flip, but not when it's almost started or finished.
  if((ff->section != section || ff->timer > margin) &&
     (ff->tosection != section || ff->timer < ff->flipspeed - margin))
    return 0;

  if((double)ff->tz < minz || (double)ff->tz > maxz)
    return 0;

  return 1;
}

static int foe_flipper_hit(object_t *obj, object_t *srcobj)
{
  foe_flipper_t *ff = (foe_flipper_t *)obj->object;
  object_t *exp;
  int section = ff->section;
  double margin = ff->ismutant ? 100 : 200;

  if(ff->tosection != ff->section && ff->timer > ff->flipspeed - margin)
    section = ff->tosection;

  exp = foe_explosion_create(ff->ga_obj, section, ff->tz, 1);
  engine_object_add(exp, 1);

  score_increase(150);

  return 1;
}

static void foe_flipper_posget(object_t *obj, int *section, double *zpos)
{
  foe_flipper_t *ff = (foe_flipper_t *)obj->object;

  if(section)
    *section = ff->section;
  if(zpos)
    *zpos = ff->tz;
}

static void foe_flipper_reinit(object_t *obj, int doalloc)
{
  foe_flipper_t *ff = (foe_flipper_t *)obj->object;

  if(doalloc) {
    engine_globject_alloc(GLOBJ(0));
  }

  if(!ff->ismutant) {
    GLfloat *low_cols, *high_cols;
    if(ff->currstage <= 16) {
      low_cols = low_colours_1;
      high_cols = high_colours_1;
    } else if((ff->currstage >= 17 && ff->currstage <= 32) ||
	      (ff->currstage >= 49 && ff->currstage <= 64)) {
      low_cols = low_colours_2;
      high_cols = high_colours_2;
    } else {
      low_cols = low_colours_3;
      high_cols = high_colours_3;
    }
    if(gfx_details >= GFX_DETAILS_HIGH) {
      engine_globject_init_ex(GLOBJ(0), SHADERS_LIGHT1,
			      high_num_vertices, high_vertices, 0,
			      high_num_vertices, high_normals, 0,
			      high_num_vertices, high_cols, 0,
			      high_num_indices, high_indices, 0);
      ff->numidxs = high_num_indices;
    } else {
      engine_globject_init(GLOBJ(0), SHADERS_GOURAUD,
			   low_num_vertices, low_vertices, 0,
			   low_num_vertices, low_cols, 0,
			   low_num_indices, low_indices, 0);
      ff->numidxs = low_num_indices;
    }
  } else {
    if(ff->currstage <= 64) {
      if(gfx_details >= GFX_DETAILS_HIGH) {
	engine_globject_init_ex(GLOBJ(0), SHADERS_LIGHT1,
				high_mutant_num_vertices_1,
				high_mutant_vertices_1, 0,
				high_mutant_num_vertices_1,
				high_mutant_normals_1, 0,
				high_mutant_num_vertices_1,
				high_mutant_colours_1, 0,
				high_mutant_num_indices_1,
				high_mutant_indices_1, 0);
	ff->numidxs = high_mutant_num_indices_1;
      } else {
	engine_globject_init(GLOBJ(0), SHADERS_GOURAUD,
			     low_mutant_num_vertices_1,
			     low_mutant_vertices_1, 0,
			     low_mutant_num_vertices_1,
			     low_mutant_colours_1, 0,
			     low_mutant_num_indices_1,
			     low_mutant_indices_1, 0);
	ff->numidxs = low_mutant_num_indices_1;
      }
    } else if(ff->currstage <= 96) {
      if(gfx_details >= GFX_DETAILS_HIGH) {
	engine_globject_init_ex(GLOBJ(0), SHADERS_LIGHT1,
				high_mutant_num_vertices_2,
				high_mutant_vertices_2, 0,
				high_mutant_num_vertices_2,
				high_mutant_normals_2, 0,
				high_mutant_num_vertices_2,
				high_mutant_colours_2, 0,
				high_mutant_num_indices_2,
				high_mutant_indices_2, 0);
	ff->numidxs = high_mutant_num_indices_2;
      } else {
	engine_globject_init(GLOBJ(0), SHADERS_GOURAUD,
			     low_mutant_num_vertices_2,
			     low_mutant_vertices_2, 0,
			     low_mutant_num_vertices_2,
			     low_mutant_colours_2, 0,
			     low_mutant_num_indices_2,
			     low_mutant_indices_2, 0);
	ff->numidxs = low_mutant_num_indices_2;
      }
    } else {
      if(gfx_details >= GFX_DETAILS_HIGH) {
	engine_globject_init_ex(GLOBJ(0), SHADERS_LIGHT1,
				high_mutant_num_vertices_2,
				high_mutant_vertices_2, 0,
				high_mutant_num_vertices_2,
				high_mutant_normals_2, 0,
				high_mutant_num_vertices_2,
				high_mutant_colours_2_white, 0,
				high_mutant_num_indices_2,
				high_mutant_indices_2, 0);
	ff->numidxs = high_mutant_num_indices_2;
      } else {
	engine_globject_init(GLOBJ(0), SHADERS_GOURAUD,
			     low_mutant_num_vertices_2,
			     low_mutant_vertices_2, 0,
			     low_mutant_num_vertices_2,
			     low_mutant_colours_2_white, 0,
			     low_mutant_num_indices_2,
			     low_mutant_indices_2, 0);
	ff->numidxs = low_mutant_num_indices_2;
      }
    }
  }
}

object_t *foe_flipper_create(object_t *ga_obj, int ismutant, int section,
			     GLfloat zpos, int move_dir)
{
  object_t *obj;
  foe_flipper_t *ff;

  DEBUG(DEBUG_CREATE, " ");

  ff = calloc(1, sizeof(*ff));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = foe_flipper_destroy;
  obj->update_cb = foe_flipper_update;
  obj->draw_cb = foe_flipper_draw;
  obj->collision_cb = foe_flipper_collision;
  obj->hit_cb = foe_flipper_hit;
  obj->posget_cb = foe_flipper_posget;
  obj->reinit_cb = foe_flipper_reinit;
  obj->object = ff;

  FLAG_SET(obj->flags, OBJECT_F_FOE);

  ff->currstage = engine_state_stage_get() + 1;
  ff->ismutant = ismutant;

  obj->num_globjs = 1;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  foe_flipper_reinit(obj, 1);

  GLOBJ(0)->alpha = 1.0f;

  GLOBJ(0)->spec_colour[0] = 0.8f;
  GLOBJ(0)->spec_colour[1] = 0.8f;
  GLOBJ(0)->spec_colour[2] = 0.8f;
  GLOBJ(0)->spec_colour[3] = 1.0f;

  GLOBJ(0)->spec_shininess = 14.0f;

  GLOBJ(0)->light1 = &engine_light1;

  ff->ga_obj = ga_obj;

  ff->section = section;
  gamearea_section_pos_get(ff->ga_obj, ff->section, &ff->tx, &ff->ty);
  ff->tz = zpos;
  ff->angle = gamearea_section_angle_get(ff->ga_obj, ff->section);

  if(move_dir != 0) {
    if(ff->ismutant)
      ff->flipsteps = rand() % 3 + 1;
    else
      ff->flipsteps = 1;
    ff->movedir = move_dir;
    ff->tosection = gamearea_section_offset_get(ff->ga_obj, section,
						ff->movedir);
    ff->rdir = -ff->movedir;
    ff->toangle = gamearea_section_angle_get(ff->ga_obj, ff->tosection);
    ff->rtx = gamearea_section_len_get(ff->ga_obj, section);
    FLAG_SET(obj->flags, OBJECT_F_NOKILL);
    ff->adiff = gamearea_sections_angle_diff(ff->ga_obj, ff->section,
					     ff->movedir);
  } else {
    ff->toangle = ff->angle;
    ff->tosection = ff->section;
  }

  // Fire between -8.0 -- -6.0
  ff->fire_distance = 2.0 * ((double)rand() / (double)RAND_MAX) - 8.0f;

  // Fire between -6.0 -- -4.0 on higher stages sometimes
  if(ff->fire_distance > -6.5)
    ff->fire_distance2 = 2.0 * ((double)rand() / (double)RAND_MAX) - 6.0f;
  else
    ff->fire_distance2 = 0.0;

  return obj;
}
