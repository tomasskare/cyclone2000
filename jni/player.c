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
#include "player.h"
#include "object.h"

#include "game_graphics/player_data.h"

typedef struct player_s {
  object_t *ga_obj;

  int caught;
  int exiting;
  float movedown_timer;
  GLfloat exiting_speed_factor;
  GLfloat dup_offset;
  int fadestart;
  GLfloat init_timer;

  GLfloat tx, ty, tz;
  GLfloat rotate;

  int num_indices;
} player_t;


static void player_destroy(object_t *obj)
{
  player_t *pl = (player_t *)obj->object;

  free(pl);
  engine_globject_free(GLOBJ(0));
  free(obj->globjs);
  free(obj);
}

static int player_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  player_t *pl = (player_t *)obj->object;
  int pos;
  float diff = msec - prev_msec;
  GLfloat exiting_speed = gamearea_exiting_speed(pl->ga_obj);

  DEBUG(DEBUG_UPDATE, " ");

  // Stage init
  if(FLAG_TST(obj->flags, OBJECT_F_STAGE_INIT)) {
    pl->init_timer += diff;

    if(pl->init_timer < 2000)
      return 1;

    if(GLOBJ(0)->alpha == 0.0f) {
      pos = gamearea_player_section_get(pl->ga_obj);
      gamearea_section_pos_get(pl->ga_obj, pos, &pl->tx, &pl->ty);
      pl->rotate = gamearea_section_angle_get(pl->ga_obj, pos);
      pl->tz = gamearea_player_zpos_get(pl->ga_obj) +
	GAMEAREA_PLAYER_EDGE_ZPOS + 2.0f;
    }

    // Fade and "zoom" in player
    pl->dup_offset -= 3.0 * diff / 1000;
    if(pl->dup_offset < 0.0f)
      pl->dup_offset = 0.0f;

    pl->tz -= 2.0f * diff / 750;
    if(pl->tz < gamearea_player_zpos_get(pl->ga_obj) +
       GAMEAREA_PLAYER_EDGE_ZPOS)
      pl->tz = gamearea_player_zpos_get(pl->ga_obj) + GAMEAREA_PLAYER_EDGE_ZPOS;

    GLOBJ(0)->alpha += diff / 1000;
    if(GLOBJ(0)->alpha > 1.0f)
      GLOBJ(0)->alpha = 1.0f;

    return 1;
  }

  // Caught by enemy, special treatment.
  if(!pl->caught && FLAG_TST(obj->flags, OBJECT_F_PLAYER_CAUGHT))
    pl->caught = 1;


  if(pl->caught) {

    pl->movedown_timer += diff;

    if(pl->movedown_timer < 1000) {
      pl->tz -= 10.0f * diff / 1000;
    } else if(pl->movedown_timer < 2000) {
      if(!pl->fadestart) {
	engine_fade_out(900);
	pl->fadestart = 1;
      }

      GLfloat vx, vy, vz;
      engine_viewpoint_get(&vx, &vy, &vz);
      vz += diff / 70;
      engine_viewpoint_set(vx, vy, vz);
    } else {
      return 0;
    }

  } else if(exiting_speed > 0) {

    // Is exiting, move out..
    pl->tz -= 10.0f * diff * exiting_speed * pl->exiting_speed_factor;
    gamearea_player_zpos_set(pl->ga_obj, pl->tz);
    if(pl->tz > -10.0f) {
      // Move down gamearea, still controllable
      pos = gamearea_player_section_get(pl->ga_obj);
      gamearea_section_pos_get(pl->ga_obj, pos, &pl->tx, &pl->ty);
      pl->rotate = gamearea_section_angle_get(pl->ga_obj, pos);
    } else {
      // duplicate, rotate and move faster
      pl->exiting_speed_factor += 0.001;
      pl->rotate -= diff / 4;
      while(pl->rotate < 0.0f)
	pl->rotate += 360.0f;

      pl->dup_offset += diff / 1300;
      if(pl->dup_offset > 2.5f)
	pl->dup_offset = 2.5f;

      // fade out player
      GLOBJ(0)->alpha -= diff * pl->exiting_speed_factor * 0.0002;
      if(GLOBJ(0)->alpha < 0.0f)
	GLOBJ(0)->alpha = 0.0f;
    }

  } else {

    pos = gamearea_player_section_get(pl->ga_obj);
    gamearea_section_pos_get(pl->ga_obj, pos, &pl->tx, &pl->ty);
    pl->rotate = gamearea_section_angle_get(pl->ga_obj, pos);
    pl->tz = gamearea_player_zpos_get(pl->ga_obj) + GAMEAREA_PLAYER_EDGE_ZPOS;

  }

  return 1;
}

static void player_draw(object_t *obj)
{
  player_t *pl = (player_t *)obj->object;

  DEBUG(DEBUG_DRAW, " ");

  glDepthRangef(0.0, 0.9);

  if(GLOBJ(0)->alpha < 1.0f)
    glEnable(GL_BLEND);

  matrix_set_translate(GLOBJ(0)->mtx_model, pl->tx, pl->ty, pl->tz);
  matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Z, pl->rotate);
  engine_globject_draw_prepare(GLOBJ(0));
  glDrawElements(GL_TRIANGLES, pl->num_indices, GL_UNSIGNED_SHORT, 0);

  if(pl->dup_offset > 0.0f) {
    int i;

    // Duplicate thrice
    for(i = 0 ; i < 3 ; i++) {
      matrix_translate(GLOBJ(0)->mtx_model, 0.0f, 0.0f, pl->dup_offset);
      engine_globject_update_modelmtx(GLOBJ(0));
      glDrawElements(GL_TRIANGLES, pl->num_indices, GL_UNSIGNED_SHORT, 0);
    }
  }

  engine_globject_draw_cleanup(GLOBJ(0));

  if(GLOBJ(0)->alpha < 1.0f)
    glDisable(GL_BLEND);

  glDepthRangef(0.0, 1.0);
}

static int player_collision(object_t *obj, object_t *srcobj,
			    int section, double minz, double maxz)
{
  player_t *pl = (player_t *)obj->object;

  int pl_section = gamearea_player_section_get(pl->ga_obj);
  GLfloat pl_zpos = gamearea_player_zpos_get(pl->ga_obj);

  if(pl_section != section)
    return 0;

  if((double)pl_zpos < minz || (double)pl_zpos > maxz)
    return 0;

  return 1;
}

static int player_hit(object_t *obj, object_t *srcobj)
{
  return 1;
}

static void player_reinit(object_t *obj, int doalloc)
{
  player_t *pl = (player_t *)obj->object;

  if(doalloc) {
    engine_globject_alloc(GLOBJ(0));
  }

  if(gfx_details >= GFX_DETAILS_HIGH) {
    engine_globject_init_ex(GLOBJ(0), SHADERS_LIGHT1,
			    high_num_vertices, high_vertices, 0,
			    high_num_vertices, high_normals, 0,
			    high_num_vertices, high_colours, 0,
			    high_num_indices, high_indices, 0);
    pl->num_indices = high_num_indices;
  } else {
    engine_globject_init(GLOBJ(0), SHADERS_GOURAUD,
			 low_num_vertices, low_vertices, 0,
			 low_num_vertices, low_colours, 0,
			 low_num_indices, low_indices, 0);
    pl->num_indices = low_num_indices;
  }
}

object_t *player_create(object_t *ga_obj)
{
  object_t *obj;
  player_t *pl;

  DEBUG(DEBUG_CREATE, " ");

  pl = calloc(1, sizeof(*pl));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = player_destroy;
  obj->update_cb = player_update;
  obj->draw_cb = player_draw;
  obj->collision_cb = player_collision;
  obj->hit_cb = player_hit;
  obj->reinit_cb = player_reinit;
  obj->object = pl;

  FLAG_SET(obj->flags, OBJECT_F_PLAYER);

  pl->ga_obj = ga_obj;
  pl->exiting_speed_factor = 1.0f;

  pl->dup_offset = 3.0f;

  obj->num_globjs = 1;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  player_reinit(obj, 1);

  // Reset colour alpha for stage init
  GLOBJ(0)->alpha = 0.0f;

  GLOBJ(0)->spec_colour[0] = 0.8f;
  GLOBJ(0)->spec_colour[1] = 0.8f;
  GLOBJ(0)->spec_colour[2] = 0.8f;
  GLOBJ(0)->spec_colour[3] = 1.0f;

  GLOBJ(0)->spec_shininess = 11.0f;

  GLOBJ(0)->light1 = &engine_light1;

  return obj;
}
