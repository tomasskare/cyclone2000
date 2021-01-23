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
#include "ai_droid.h"
#include "player_fire.h"
#include "object.h"

#include "game_graphics/ai_droid_data.h"

typedef struct ai_droid_s {
  object_t *ga_obj;

  GLfloat dup_offset;
  GLfloat init_timer;
  GLfloat fade_timer;
  uint64_t last_shot;
  uint64_t last_shot_sound;

  int move_dir;
  int section_from;
  int section_to;
  double section_move;

  GLfloat angle;
  GLfloat cr, cg, cb;
  GLfloat tx, ty, tz;
  GLfloat rx, ry, rz;

  GLfloat colours[8 * 3];
} ai_droid_t;

#define FADE_SPEED 3000.0


static void ai_droid_destroy(object_t *obj)
{
  ai_droid_t *ai = (ai_droid_t *)obj->object;

  free(ai);
  engine_globject_free(GLOBJ(0));
  free(obj->globjs);
  free(obj);
}

static int ai_droid_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  ai_droid_t *ai = (ai_droid_t *)obj->object;
  object_t *foe_obj;
  float diff = msec - prev_msec;
  GLfloat tx1, ty1;
  GLfloat tx2, ty2;
  int section_shoot, shoot_type;
  GLfloat exiting_speed = gamearea_exiting_speed(ai->ga_obj);
  int colidx, i;
  GLfloat colfade;

  DEBUG(DEBUG_UPDATE, " ");

  ai->init_timer += diff;
  ai->fade_timer += diff;
  while(ai->fade_timer > num_fade_colours * FADE_SPEED)
    ai->fade_timer -= num_fade_colours * FADE_SPEED;

  // Fade and "zoom" in player
  ai->dup_offset -= 3.0 * diff / 1500;
  if(ai->dup_offset < 0.0f)
    ai->dup_offset = 0.0f;

  ai->tz -= 2.0f * diff / 1500;
  if(ai->tz < GAMEAREA_DROID_EDGE_ZPOS)
    ai->tz = GAMEAREA_DROID_EDGE_ZPOS;

  GLOBJ(0)->alpha += diff / 1500;
  if(GLOBJ(0)->alpha > 1.0f)
    GLOBJ(0)->alpha = 1.0f;

  if(ai->section_move == 0.0) {
    /* Move toward closest enemy */
    foe_obj = engine_object_first_foe_get(0);
    if(foe_obj) {
      int foe_section, closed, num_sections, move_dir;

      foe_obj->posget_cb(foe_obj, &foe_section, NULL);
      closed = gamearea_closed_get(ai->ga_obj);
      num_sections = gamearea_num_sections_get(ai->ga_obj);
      move_dir = foe_section - ai->section_from;
      if(closed && (move_dir > num_sections / 2 ||
		    move_dir < -num_sections / 2))
	move_dir = -move_dir;

      if(move_dir < 0)
	ai->move_dir = -1;
      else
	ai->move_dir = 1;
    }

    ai->section_to = gamearea_section_offset_get(ai->ga_obj, ai->section_from,
						 ai->move_dir);
  }

  ai->section_move += diff / 600;

  if(ai->section_move >= 1.0) {
    ai->section_from = ai->section_to;
    ai->section_move = 0.0;
  }

  gamearea_section_pos_get(ai->ga_obj, ai->section_from, &tx1, &ty1);
  gamearea_section_pos_get(ai->ga_obj, ai->section_to, &tx2, &ty2);
  ai->angle = gamearea_section_angle_get(ai->ga_obj, ai->section_from);

  ai->tx = tx2 * ai->section_move + tx1 * (1.0 - ai->section_move);
  ai->ty = ty2 * ai->section_move + ty1 * (1.0 - ai->section_move);


  /* Rotate */
  ai->rx += diff / 7;
  if(ai->rx > 360.0)
    ai->rx -= 360.0;
  ai->ry += diff / 11;
  if(ai->ry > 360.0)
    ai->ry -= 360.0;
  ai->rz += diff / 17;
  if(ai->rz > 360.0)
    ai->rz -= 360.0;


  /* Fade colour */
  colidx = (int)(ai->fade_timer / FADE_SPEED);
  colfade = (ai->fade_timer / FADE_SPEED) - (GLfloat)colidx;

  ai->cr = (fade_colours[colidx       * 3 + 0] * (1.0 - colfade) +
	    fade_colours[(colidx + 1) * 3 + 0] * colfade);
  ai->cg = (fade_colours[colidx       * 3 + 1] * (1.0 - colfade) +
	    fade_colours[(colidx + 1) * 3 + 1] * colfade);
  ai->cb = (fade_colours[colidx       * 3 + 2] * (1.0 - colfade) +
	    fade_colours[(colidx + 1) * 3 + 2] * colfade);

  if(gfx_details >= GFX_DETAILS_HIGH) {
    for(i = 0 ; i < num_vertices ; i++) {
      ai->colours[i * 3 + 0] = ai->cr;
      ai->colours[i * 3 + 1] = ai->cg;
      ai->colours[i * 3 + 2] = ai->cb;
    }
    engine_globject_update_cols(GLOBJ(0), num_vertices, ai->colours);
  }

  /* Don't shoot when initializing or exiting stage. */
  if(ai->init_timer < 2000 || exiting_speed > 0)
    return 1;


  if(ai->section_to == ai->section_from)
    section_shoot = ai->section_from;
  else if(ai->section_move < 0.35)
    section_shoot = ai->section_from;
  else if(ai->section_move > 0.65)
    section_shoot = ai->section_to;
  else
    section_shoot = -1;

  /* shoot all the time */
  shoot_type = gamearea_shoot_type_get(ai->ga_obj) + PLAYER_FIRE_T_DROID_OFFSET;
  if(msec - ai->last_shot > (shoot_type == PLAYER_FIRE_T_DROID_LASER ?
			     100 : 200)) {
    if(msec - ai->last_shot_sound > 200) {
      ai->last_shot_sound = msec;
      native_callback(CALLBACK_PLAY_SOUND,
		      (shoot_type == PLAYER_FIRE_T_DROID_LASER ?
		       SOUND_LASER : SOUND_SHOT), 0, 0);
    }

    ai->last_shot = msec;
    engine_object_add(player_fire_create(obj, shoot_type, section_shoot,
					 ai->tx, ai->ty, ai->tz, ai->angle), 1);
  }

  return 1;
}

static void ai_droid_draw(object_t *obj)
{
  ai_droid_t *ai = (ai_droid_t *)obj->object;
  float tmpmtx[16];

  DEBUG(DEBUG_DRAW, " ");

  glDepthRangef(0.0, 0.9);
  glEnable(GL_BLEND);

  matrix_set_translate(GLOBJ(0)->mtx_model, ai->tx, ai->ty, ai->tz);
  matrix_copy(tmpmtx, GLOBJ(0)->mtx_model);
  matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_X, ai->rx);
  matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Y, ai->ry);
  matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Z, ai->rz);

  engine_globject_draw_prepare(GLOBJ(0));
  engine_globject_set_colour(GLOBJ(0), ai->cr, ai->cg, ai->cb);

  glDrawElements(GL_LINES, num_line_indices, GL_UNSIGNED_SHORT, 0);

  if(ai->dup_offset > 0.0f) {
    int i;

    // Duplicate thrice
    for(i = 0 ; i < 3 ; i++) {
      matrix_translate_to(GLOBJ(0)->mtx_model, tmpmtx,
			  0.0f, 0.0f, i * ai->dup_offset);
      matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_X, ai->rx);
      matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Y, ai->ry);
      matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Z, ai->rz);
      engine_globject_update_modelmtx(GLOBJ(0));
      glDrawElements(GL_LINES, num_line_indices, GL_UNSIGNED_SHORT, 0);
    }
  }

  engine_globject_draw_cleanup(GLOBJ(0));

  glDisable(GL_BLEND);
  glDepthRangef(0.0, 1.0);
}

static void ai_droid_reinit(object_t *obj, int doalloc)
{
  ai_droid_t *ai = (ai_droid_t *)obj->object;

  if(doalloc) {
    engine_globject_alloc(GLOBJ(0));
  }

  if(gfx_details >= GFX_DETAILS_HIGH) {
    engine_globject_init_ex(GLOBJ(0), SHADERS_LIGHT1,
			    num_vertices, vertices, 0,
			    num_vertices, normals, 0,
			    num_vertices, ai->colours, 1,
			    num_line_indices, line_indices, 0);
  } else {
    engine_globject_init(GLOBJ(0), SHADERS_LINE,
			 num_vertices, vertices, 0,
			 0, NULL, 0,
			 num_line_indices, line_indices, 0);
  }
}

object_t *ai_droid_create(object_t *ga_obj)
{
  object_t *obj, *foe_obj;
  ai_droid_t *ai;
  int section;

  DEBUG(DEBUG_CREATE, " ");

  ai = calloc(1, sizeof(*ai));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = ai_droid_destroy;
  obj->update_cb = ai_droid_update;
  obj->draw_cb = ai_droid_draw;
  obj->reinit_cb = ai_droid_reinit;
  obj->object = ai;

  ai->ga_obj = ga_obj;

  obj->num_globjs = 1;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  ai_droid_reinit(obj, 1);

  GLOBJ(0)->alpha = 0.0f;

  GLOBJ(0)->spec_colour[0] = 0.5f;
  GLOBJ(0)->spec_colour[1] = 0.5f;
  GLOBJ(0)->spec_colour[2] = 0.5f;
  GLOBJ(0)->spec_colour[3] = 1.0f;

  GLOBJ(0)->spec_shininess = 16.0f;

  GLOBJ(0)->light1 = &engine_light1;

  // Reset for entry
  ai->cr = 1.0f;
  ai->cg = 1.0f;
  ai->cb = 1.0f;
  ai->tz = GAMEAREA_DROID_EDGE_ZPOS + 2.0f;
  ai->dup_offset = 3.0f;

  foe_obj = engine_object_first_foe_get(0);
  if(foe_obj) {
    foe_obj->posget_cb(foe_obj, &section, NULL);
  } else {
    section = gamearea_player_section_get(ai->ga_obj);
  }

  ai->section_from = ai->section_to = section;

  return obj;
}
