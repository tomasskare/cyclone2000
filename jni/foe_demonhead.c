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
#include "foe_demonhead.h"
#include "foe_fire.h"
#include "foe_explosion.h"
#include "score.h"
#include "object.h"

#include "game_graphics/foe_demonhead_data.h"

typedef struct foe_demonhead_s {
  object_t *ga_obj;

  int section, tosection;
  int movedir;

  int mode, hitnum;

  GLfloat tx, ty, tz;
  GLfloat totx, toty;
  GLfloat dtx, dty;
  GLfloat rtx, rdir;
  GLfloat angle, rotate, toangle;
  GLfloat adiff;
  GLfloat alpha;

  double timer;
  double skipcollision_timer;

  GLfloat fire_distance, fire_distance2;
  int has_fired;

  int currstage;

  int num_body_indices;
  int num_horn_indices;
  int num_tooth_indices;
} foe_demonhead_t;

#define GLOBJF_BODY    0
#define GLOBJF_L_HORN  1
#define GLOBJF_R_HORN  2
#define GLOBJF_L_TOOTH 3
#define GLOBJF_R_TOOTH 4

#define GLOBJS_HORN  0
#define GLOBJS_TOOTH 1

static void foe_demonhead_destroy(object_t *obj)
{
  foe_demonhead_t *dh = (foe_demonhead_t *)obj->object;
  int i;

  free(dh);
  for(i = 0 ; i < obj->num_globjs ; i++)
    engine_globject_free(GLOBJ(i));
  free(obj->globjs);
  free(obj);
}

static int foe_demonhead_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  foe_demonhead_t *dh = (foe_demonhead_t *)obj->object;
  float diff = msec - prev_msec;

  DEBUG(DEBUG_UPDATE, " ");

  // Has caught the player, special treatment
  if(FLAG_TST(obj->flags, OBJECT_F_PLAYER_CAUGHT)) {
    dh->tz -= 10.0f * diff / 1000;
    return 1;
  }

  // When body is on edge, flip toward player
  if(dh->mode == DEMONHEAD_MODE_FULL &&
     FLAG_TST(obj->flags, OBJECT_F_FOE_EDGE)) {
    GLfloat flipspeed = 800;

    if(dh->tosection == dh->section) {
      // Start a new flip

      // Move toward player.
      int closed = gamearea_closed_get(dh->ga_obj);
      int num_sections = gamearea_num_sections_get(dh->ga_obj);
      int player_section = gamearea_player_section_get(dh->ga_obj);
      int move_dir = player_section - dh->section;
      if(closed && (move_dir > num_sections / 2 ||
		    move_dir < -num_sections / 2))
	move_dir = -move_dir;

      if(move_dir != 0) {
	if(move_dir < 0)
	  move_dir = -1;
	else
	  move_dir = 1;

	dh->tosection = gamearea_section_offset_get(dh->ga_obj, dh->section,
						    move_dir);
	dh->rdir = -move_dir;
	dh->toangle = gamearea_section_angle_get(dh->ga_obj, dh->tosection);
	dh->rtx = gamearea_section_len_get(dh->ga_obj, dh->section);
	gamearea_section_pos_get(dh->ga_obj, dh->tosection,
				 &dh->totx, &dh->toty);
	dh->timer = diff;
	dh->rotate = 0;
	dh->adiff = 180.0 + gamearea_sections_angle_diff(dh->ga_obj,
							 dh->section,
							 move_dir);
      }
    }

    // Flipping fun
    dh->rotate -= dh->rdir * dh->adiff * diff / flipspeed;
    dh->dtx += (dh->totx - dh->tx) * diff / flipspeed;
    dh->dty += (dh->toty - dh->ty) * diff / flipspeed;

    if(dh->timer > flipspeed) {
      dh->section = dh->tosection;
      gamearea_section_pos_get(dh->ga_obj, dh->section, &dh->tx, &dh->ty);
      dh->angle = gamearea_section_angle_get(dh->ga_obj, dh->section);
      dh->skipcollision_timer = 80;
      dh->dtx = 0;
      dh->dty = 0;
    }

    dh->timer += diff;

  } else if(dh->mode == DEMONHEAD_MODE_FULL) {

    dh->tz += diff / 500;

  } else {

    dh->tz += diff / 200;
    dh->rotate += 360.0 * diff / 1800;

    if(dh->tz > 0.0) {
      if(dh->tz > 1.2)
	return 0;

      dh->alpha -= 1.2 * diff / 200;
      if(dh->alpha < 0.0)
	dh->alpha = 0.0;
    }

    GLOBJ(GLOBJS_HORN)->alpha = dh->alpha;
    GLOBJ(GLOBJS_TOOTH)->alpha = dh->alpha;
  }

  dh->skipcollision_timer -= diff;
  if(dh->skipcollision_timer < 0.0)
    dh->skipcollision_timer = 0.0;

  if(dh->skipcollision_timer == 0.0 && dh->tz >= 0.0f &&
     engine_collision_detect(obj,
			     OBJECT_F_PLAYER, dh->section,
			     -1.0, (double)dh->tz))
    return 0;


  if(dh->mode == DEMONHEAD_MODE_FULL &&
     !FLAG_TST(obj->flags, OBJECT_F_FOE_EDGE)) {
    // Change behaviour at rim
    if(dh->tz >= GAMEAREA_FOE_EDGE_ZPOS) {
      dh->tz = GAMEAREA_FOE_EDGE_ZPOS;
      engine_object_onedge(obj);
      return 1;
    }

    if(!dh->has_fired && dh->tz > dh->fire_distance && dh->tz < -6.0f) {
      object_t *fi_obj = foe_fire_create(dh->ga_obj, dh->section, dh->tz);
      engine_object_add(fi_obj, 1);
      dh->has_fired = 1;
    }

    if(dh->currstage > 48 && dh->has_fired == 1 &&
       dh->tz > dh->fire_distance2 && dh->tz < -4.0f) {
      object_t *fi_obj = foe_fire_create(dh->ga_obj, dh->section, dh->tz);
      engine_object_add(fi_obj, 1);
      dh->has_fired = 2;
    }

  }

  return 1;
}

static void foe_demonhead_draw(object_t *obj)
{
  foe_demonhead_t *dh = (foe_demonhead_t *)obj->object;
  float mainmtx[16];

  DEBUG(DEBUG_DRAW, " ");

  glEnable(GL_BLEND);

  matrix_set_translate(mainmtx, dh->tx + dh->dtx, dh->ty + dh->dty, dh->tz);
  matrix_rotate(mainmtx, MTX_AXIS_Z, dh->angle);
  if(!FLAG_TST(obj->flags, OBJECT_F_FOE_EDGE))
    matrix_translate(mainmtx, 0.0f, 0.25f, 0.0f);

  if(dh->tosection != dh->section) {
    matrix_translate(mainmtx, dh->rdir * BODYSCALEX / 2, 0.0f, 0.0f);
    matrix_rotate(mainmtx, MTX_AXIS_Z, dh->rotate);
    matrix_translate(mainmtx, -dh->rdir * BODYSCALEX / 2, 0.0f, 0.0f);
  }

  if(dh->mode == DEMONHEAD_MODE_FULL) {

    matrix_copy(GLOBJ(GLOBJF_BODY)->mtx_model, mainmtx);
    engine_globject_draw_prepare(GLOBJ(GLOBJF_BODY));
    glDrawElements(GL_TRIANGLES, dh->num_body_indices, GL_UNSIGNED_SHORT, 0);
    engine_globject_draw_cleanup(GLOBJ(GLOBJF_BODY));

    if(dh->hitnum < 1) {
      matrix_translate_to(GLOBJ(GLOBJF_L_HORN)->mtx_model, mainmtx,
			  -0.12f, 0.10f, 0.00f);
      engine_globject_draw_prepare(GLOBJ(GLOBJF_L_HORN));
      glDrawElements(GL_TRIANGLES, dh->num_horn_indices, GL_UNSIGNED_SHORT, 0);
      engine_globject_draw_cleanup(GLOBJ(GLOBJF_L_HORN));

      matrix_translate_to(GLOBJ(GLOBJF_R_TOOTH)->mtx_model, mainmtx,
			  0.07f, -0.05f, 0.00f);
      engine_globject_draw_prepare(GLOBJ(GLOBJF_R_TOOTH));
      glDrawElements(GL_TRIANGLES, dh->num_tooth_indices, GL_UNSIGNED_SHORT, 0);
      engine_globject_draw_cleanup(GLOBJ(GLOBJF_R_TOOTH));
    }

    if(dh->hitnum < 2) {
      matrix_translate_to(GLOBJ(GLOBJF_R_HORN)->mtx_model, mainmtx,
			  0.12f, 0.10f, 0.00f);
      engine_globject_draw_prepare(GLOBJ(GLOBJF_R_HORN));
      glDrawElements(GL_TRIANGLES, dh->num_horn_indices, GL_UNSIGNED_SHORT, 0);
      engine_globject_draw_cleanup(GLOBJ(GLOBJF_R_HORN));

      matrix_translate_to(GLOBJ(GLOBJF_L_TOOTH)->mtx_model, mainmtx,
			  -0.07f, -0.05f, 0.00f);
      engine_globject_draw_prepare(GLOBJ(GLOBJF_L_TOOTH));
      glDrawElements(GL_TRIANGLES, dh->num_tooth_indices, GL_UNSIGNED_SHORT, 0);
      engine_globject_draw_cleanup(GLOBJ(GLOBJF_L_TOOTH));
    }

  } else {
    GLfloat sign = 1.0f;

    if(dh->mode == DEMONHEAD_MODE_LEFT)
      sign = -1.0f;

    matrix_rotate(mainmtx, MTX_AXIS_Z, dh->rotate);

    matrix_translate_to(GLOBJ(GLOBJS_HORN)->mtx_model, mainmtx,
			sign * 0.12f, 0.10f, 0.00f);
    engine_globject_draw_prepare(GLOBJ(GLOBJS_HORN));
    glDrawElements(GL_TRIANGLES, dh->num_horn_indices, GL_UNSIGNED_SHORT, 0);
    engine_globject_draw_cleanup(GLOBJ(GLOBJS_HORN));

    matrix_translate_to(GLOBJ(GLOBJS_TOOTH)->mtx_model, mainmtx,
			-sign * 0.07f, -0.05f, 0.00f);
    engine_globject_draw_prepare(GLOBJ(GLOBJS_TOOTH));
    glDrawElements(GL_TRIANGLES, dh->num_tooth_indices, GL_UNSIGNED_SHORT, 0);
    engine_globject_draw_cleanup(GLOBJ(GLOBJS_TOOTH));

  }

  glDisable(GL_BLEND);
}

static int foe_demonhead_collision(object_t *obj, object_t *srcobj,
				   int section, double minz, double maxz)
{
  foe_demonhead_t *dh = (foe_demonhead_t *)obj->object;

  /* Laser should go through horn/teeth. */
  if(dh->mode != DEMONHEAD_MODE_FULL &&
     FLAG_TST(srcobj->flags, OBJECT_F_PFIRE_LASER))
    return 0;

  // Miss during flip, but not when it's almost started or finished.
  if((dh->section != section || dh->timer > 100) &&
     (dh->tosection != section || dh->timer < 500))
    return 0;

  if((double)dh->tz < minz || (double)dh->tz > maxz)
    return 0;

  return 1;
}

static int foe_demonhead_hit(object_t *obj, object_t *srcobj)
{
  foe_demonhead_t *dh = (foe_demonhead_t *)obj->object;
  object_t *exobj;
  int section = dh->section;

  /* Horn/teeth shot by normal fire have no effect. */
  if(dh->mode != DEMONHEAD_MODE_FULL)
    return 0;

  dh->hitnum++;

  if(FLAG_TST(obj->flags, OBJECT_F_FORCE_KILL) ||
     FLAG_TST(obj->flags, OBJECT_F_FOE_EDGE) || dh->hitnum > 2) {

    /* On the edge or third hit, just explode. */

    if(dh->tosection != dh->section && dh->timer > 400)
      section = dh->tosection;

    exobj = foe_explosion_create(dh->ga_obj, section, dh->tz, 1);
    engine_object_add(exobj, 1);

    score_increase(150);

    return 1;

  } else {

    /* In the web, first and second hit, fire off the horns and teeth. */

    if(dh->hitnum == 1) {
      exobj = foe_demonhead_create(dh->ga_obj, section, dh->tz,
				   DEMONHEAD_MODE_LEFT);
    } else {
      exobj = foe_demonhead_create(dh->ga_obj, section, dh->tz,
				   DEMONHEAD_MODE_RIGHT);
    }
    engine_object_add(exobj, 1);

    return 0;

  }
}

static void foe_demonhead_posget(object_t *obj, int *section, double *zpos)
{
  foe_demonhead_t *dh = (foe_demonhead_t *)obj->object;

  if(section)
    *section = dh->section;
  if(zpos)
    *zpos = dh->tz;
}

static void foe_demonhead_reinit(object_t *obj, int doalloc)
{
  foe_demonhead_t *dh = (foe_demonhead_t *)obj->object;

  if(gfx_details >= GFX_DETAILS_HIGH) {
    dh->num_body_indices = high_num_body_indices;
    dh->num_horn_indices = high_num_horn_indices;
    dh->num_tooth_indices = high_num_tooth_indices;
  } else {
    dh->num_body_indices = low_num_body_indices;
    dh->num_horn_indices = low_num_horn_indices;
    dh->num_tooth_indices = low_num_tooth_indices;
  }

  if(dh->mode == DEMONHEAD_MODE_FULL) {
    if(doalloc) {
      engine_globject_alloc(GLOBJ(GLOBJF_BODY));
      engine_globject_alloc(GLOBJ(GLOBJF_L_HORN));
      engine_globject_alloc(GLOBJ(GLOBJF_R_HORN));
      engine_globject_alloc(GLOBJ(GLOBJF_L_TOOTH));
      engine_globject_alloc(GLOBJ(GLOBJF_R_TOOTH));
    }

    if(gfx_details >= GFX_DETAILS_HIGH) {
      engine_globject_init_ex(GLOBJ(GLOBJF_BODY), SHADERS_LIGHT1,
			      high_num_body_vertices, high_body_vertices, 0,
			      high_num_body_vertices, high_body_normals, 0,
			      high_num_body_vertices, high_body_colours, 0,
			      high_num_body_indices, high_body_indices, 0);

      engine_globject_init_ex(GLOBJ(GLOBJF_L_HORN), SHADERS_LIGHT1,
			      high_num_horn_vertices, high_horn_l_vertices, 0,
			      high_num_horn_vertices, high_horn_l_normals, 0,
			      high_num_horn_vertices, high_horn_colours, 0,
			      high_num_horn_indices, high_horn_l_indices, 0);

      engine_globject_init_ex(GLOBJ(GLOBJF_R_HORN), SHADERS_LIGHT1,
			      high_num_horn_vertices, high_horn_r_vertices, 0,
			      high_num_horn_vertices, high_horn_r_normals, 0,
			      high_num_horn_vertices, high_horn_colours, 0,
			      high_num_horn_indices, high_horn_r_indices, 0);

      engine_globject_init_ex(GLOBJ(GLOBJF_L_TOOTH), SHADERS_LIGHT1,
			      high_num_tooth_vertices, high_tooth_l_vertices, 0,
			      high_num_tooth_vertices, high_tooth_l_normals, 0,
			      high_num_tooth_vertices, high_tooth_colours, 0,
			      high_num_tooth_indices, high_tooth_l_indices, 0);

      engine_globject_init_ex(GLOBJ(GLOBJF_R_TOOTH), SHADERS_LIGHT1,
			      high_num_tooth_vertices, high_tooth_r_vertices, 0,
			      high_num_tooth_vertices, high_tooth_r_normals, 0,
			      high_num_tooth_vertices, high_tooth_colours, 0,
			      high_num_tooth_indices, high_tooth_r_indices, 0);
    } else {
      engine_globject_init(GLOBJ(GLOBJF_BODY), SHADERS_GOURAUD,
			   low_num_body_vertices, low_body_vertices, 0,
			   low_num_body_vertices, low_body_colours, 0,
			   low_num_body_indices, low_body_indices, 0);

      engine_globject_init(GLOBJ(GLOBJF_L_HORN), SHADERS_GOURAUD,
			   low_num_horn_vertices, low_horn_l_vertices, 0,
			   low_num_horn_vertices, low_horn_colours, 0,
			   low_num_horn_indices, low_horn_l_indices, 0);

      engine_globject_init(GLOBJ(GLOBJF_R_HORN), SHADERS_GOURAUD,
			   low_num_horn_vertices, low_horn_r_vertices, 0,
			   low_num_horn_vertices, low_horn_colours, 0,
			   low_num_horn_indices, low_horn_r_indices, 0);

      engine_globject_init(GLOBJ(GLOBJF_L_TOOTH), SHADERS_GOURAUD,
			   low_num_tooth_vertices, low_tooth_l_vertices, 0,
			   low_num_tooth_vertices, low_tooth_colours, 0,
			   low_num_tooth_indices, low_tooth_l_indices, 0);

      engine_globject_init(GLOBJ(GLOBJF_R_TOOTH), SHADERS_GOURAUD,
			   low_num_tooth_vertices, low_tooth_r_vertices, 0,
			   low_num_tooth_vertices, low_tooth_colours, 0,
			   low_num_tooth_indices, low_tooth_r_indices, 0);
    }
  } else if(dh->mode == DEMONHEAD_MODE_LEFT) {
    if(doalloc) {
      engine_globject_alloc(GLOBJ(GLOBJS_HORN));
      engine_globject_alloc(GLOBJ(GLOBJS_TOOTH));
    }

    if(gfx_details >= GFX_DETAILS_HIGH) {
      engine_globject_init_ex(GLOBJ(GLOBJS_HORN), SHADERS_LIGHT1,
			      high_num_horn_vertices, high_horn_l_vertices, 0,
			      high_num_horn_vertices, high_horn_l_normals, 0,
			      high_num_horn_vertices, high_horn_colours, 0,
			      high_num_horn_indices, high_horn_l_indices, 0);

      engine_globject_init_ex(GLOBJ(GLOBJS_TOOTH), SHADERS_LIGHT1,
			      high_num_tooth_vertices, high_tooth_r_vertices, 0,
			      high_num_tooth_vertices, high_tooth_r_normals, 0,
			      high_num_tooth_vertices, high_tooth_colours, 0,
			      high_num_tooth_indices, high_tooth_r_indices, 0);
    } else {
      engine_globject_init(GLOBJ(GLOBJS_HORN), SHADERS_GOURAUD,
			   low_num_horn_vertices, low_horn_l_vertices, 0,
			   low_num_horn_vertices, low_horn_colours, 0,
			   low_num_horn_indices, low_horn_l_indices, 0);

      engine_globject_init(GLOBJ(GLOBJS_TOOTH), SHADERS_GOURAUD,
			   low_num_tooth_vertices, low_tooth_r_vertices, 0,
			   low_num_tooth_vertices, low_tooth_colours, 0,
			   low_num_tooth_indices, low_tooth_r_indices, 0);
    }
  } else {
    if(doalloc) {
      engine_globject_alloc(GLOBJ(GLOBJS_HORN));
      engine_globject_alloc(GLOBJ(GLOBJS_TOOTH));
    }

    if(gfx_details >= GFX_DETAILS_HIGH) {
      engine_globject_init_ex(GLOBJ(GLOBJS_HORN), SHADERS_LIGHT1,
			      high_num_horn_vertices, high_horn_r_vertices, 0,
			      high_num_horn_vertices, high_horn_r_normals, 0,
			      high_num_horn_vertices, high_horn_colours, 0,
			      high_num_horn_indices, high_horn_r_indices, 0);

      engine_globject_init_ex(GLOBJ(GLOBJS_TOOTH), SHADERS_LIGHT1,
			      high_num_tooth_vertices, high_tooth_l_vertices, 0,
			      high_num_tooth_vertices, high_tooth_l_normals, 0,
			      high_num_tooth_vertices, high_tooth_colours, 0,
			      high_num_tooth_indices, high_tooth_l_indices, 0);
    } else {
      engine_globject_init(GLOBJ(GLOBJS_HORN), SHADERS_GOURAUD,
			   low_num_horn_vertices, low_horn_r_vertices, 0,
			   low_num_horn_vertices, low_horn_colours, 0,
			   low_num_horn_indices, low_horn_r_indices, 0);

      engine_globject_init(GLOBJ(GLOBJS_TOOTH), SHADERS_GOURAUD,
			   low_num_tooth_vertices, low_tooth_l_vertices, 0,
			   low_num_tooth_vertices, low_tooth_colours, 0,
			   low_num_tooth_indices, low_tooth_l_indices, 0);
    }
  }
}

object_t *foe_demonhead_create(object_t *ga_obj, int section,
			       GLfloat zpos, int mode)
{
  object_t *obj;
  foe_demonhead_t *dh;
  int i;

  DEBUG(DEBUG_CREATE, " ");

  dh = calloc(1, sizeof(*dh));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = foe_demonhead_destroy;
  obj->update_cb = foe_demonhead_update;
  obj->draw_cb = foe_demonhead_draw;
  obj->collision_cb = foe_demonhead_collision;
  obj->hit_cb = foe_demonhead_hit;
  obj->posget_cb = foe_demonhead_posget;
  obj->reinit_cb = foe_demonhead_reinit;
  obj->object = dh;

  FLAG_SET(obj->flags, OBJECT_F_FOE);
  if(mode != DEMONHEAD_MODE_FULL) {
    FLAG_SET(obj->flags, OBJECT_F_FOE_FIRE);
    obj->num_globjs = 2;
  } else {
    obj->num_globjs = 5;
  }

  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  dh->mode = mode;

  foe_demonhead_reinit(obj, 1);

  for(i = 0 ; i < obj->num_globjs ; i++) {
    GLOBJ(i)->alpha = 1.0f;

    GLOBJ(i)->spec_colour[0] = 0.6f;
    GLOBJ(i)->spec_colour[1] = 0.6f;
    GLOBJ(i)->spec_colour[2] = 0.6f;
    GLOBJ(i)->spec_colour[3] = 1.0f;

    GLOBJ(i)->spec_shininess = 32.0f;

    GLOBJ(i)->light1 = &engine_light1;
  }

  dh->ga_obj = ga_obj;

  dh->section = section;
  gamearea_section_pos_get(dh->ga_obj, dh->section, &dh->tx, &dh->ty);
  dh->tz = zpos;
  dh->angle = gamearea_section_angle_get(dh->ga_obj, dh->section);

  dh->toangle = dh->angle;
  dh->tosection = dh->section;

  dh->alpha = 1.0;

  // Fire between -9.0 -- -7.0
  dh->fire_distance = 2.0 * ((double)rand() / (double)RAND_MAX) - 9.0f;

  // Fire between -7.0 -- -4.0 on higher stages sometimes
  if(dh->fire_distance > -7.5)
    dh->fire_distance2 = 3.0 * ((double)rand() / (double)RAND_MAX) - 7.0f;
  else
    dh->fire_distance2 = 0.0;

  dh->currstage = engine_state_stage_get() + 1;

  return obj;
}
