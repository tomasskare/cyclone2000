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
#include "player_fire.h"
#include "object.h"

#include "game_graphics/player_fire_data.h"

typedef struct player_fire_s {
  object_t *ga_obj;

  int type;
  int section;

  GLfloat cr, cg, cb;

  GLfloat tx, ty, tz;
  GLfloat angle;

  int num_indices;
  GLfloat pointsize;
} player_fire_t;


static void player_fire_destroy(object_t *obj)
{
  player_fire_t *pf = (player_fire_t *)obj->object;

  free(pf);
  engine_globject_free(GLOBJ(0));
  free(obj->globjs);
  free(obj);
}

static int player_fire_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  player_fire_t *pf = (player_fire_t *)obj->object;
  float diff = msec - prev_msec;
  double maxz = pf->tz + 0.5;
  int colidx;
  GLfloat colfade;

  DEBUG(DEBUG_UPDATE, " ");

  pf->tz -= diff / 50;
  if(pf->tz < -9.5f)
    return 0;

  colidx = (int)(3.0 * (-pf->tz) / 9.6);
  colfade = (3.0 * (-pf->tz) / 9.6) - (GLfloat)colidx;

  if(colidx < 0)
    colidx = 0;
  if(colidx > 3)
    colidx = 3;
  if(colfade < 0.0)
    colfade = 0.0;
  if(colfade > 1.0)
    colfade = 1.0;

  pf->cr = (points_4_colours[colidx       * 3 + 0] * (1.0 - colfade) +
	    points_4_colours[(colidx + 1) * 3 + 0] * colfade);
  pf->cg = (points_4_colours[colidx       * 3 + 1] * (1.0 - colfade) +
	    points_4_colours[(colidx + 1) * 3 + 1] * colfade);
  pf->cb = (points_4_colours[colidx       * 3 + 2] * (1.0 - colfade) +
	    points_4_colours[(colidx + 1) * 3 + 2] * colfade);

  // If the fire hits something, it should be removed too.
  if(pf->section >= 0) {
    if(engine_collision_detect(obj, OBJECT_F_FOE, pf->section,
			       (double)pf->tz, maxz))
      return 0;
  }

  return 1;
}

static void player_fire_draw(object_t *obj)
{
  player_fire_t *pf = (player_fire_t *)obj->object;
  float mainmtx[16];

  DEBUG(DEBUG_DRAW, " ");

  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);

  matrix_set_translate(mainmtx, pf->tx, pf->ty, pf->tz);
  matrix_rotate(mainmtx, MTX_AXIS_Z, pf->angle);
  matrix_translate(mainmtx, 0.0f, 0.12f, 0.0f);

  if(pf->type == PLAYER_FIRE_T_NORMAL ||
     pf->type == PLAYER_FIRE_T_DROID_NORMAL) {
    matrix_copy(GLOBJ(0)->mtx_model, mainmtx);
    engine_globject_draw_prepare(GLOBJ(0));
    glDrawElements(GL_TRIANGLES, pf->num_indices, GL_UNSIGNED_SHORT, 0);
    engine_globject_draw_cleanup(GLOBJ(0));

    if(gfx_details >= GFX_DETAILS_HIGH) {
      float glowmtx[16];

      matrix_scale_to(glowmtx, mainmtx, 4.0f * SCALEX, 4.0f * SCALEX, 1.0f);
      glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_1UP_1_GLOW));
      if(pf->type == PLAYER_FIRE_T_NORMAL) {
	engine_gldirect_draw_texbox(SHADERS_TEXTURE,
				    0.5f * GLOBJ(0)->alpha,
				    4, glow_vertices, glow_tex_coords,
				    glowmtx, NULL, NULL);
      } else {
	engine_gldirect_draw_texbox(SHADERS_TEXTURE,
				    0.5f * GLOBJ(0)->alpha,
				    4, glow_vertices, glow_droid_tex_coords,
				    glowmtx, NULL, NULL);
      }
    }

  } else {
    matrix_rotate_to(GLOBJ(0)->mtx_model, mainmtx,
		     MTX_AXIS_Z, -pf->tz * 120.0f / 9.0f);
    engine_globject_draw_prepare(GLOBJ(0));
    engine_globject_set_colour(GLOBJ(0), pf->cr, pf->cg, pf->cb);
    engine_globject_set_pointsize(GLOBJ(0), pf->pointsize);
    glDrawArrays(GL_POINTS, 0, pf->num_indices);
    engine_globject_draw_cleanup(GLOBJ(0));

    if(gfx_details >= GFX_DETAILS_HIGH) {
      float tmpmtx[16], glowmtx[16];
      int i;

      matrix_rotate_to(tmpmtx, mainmtx, MTX_AXIS_Z, -pf->tz * 120.0f / 9.0f);
      glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_1UP_1_GLOW));
      for(i = 0 ; i < 4 ; i++) {
	matrix_translate_to(glowmtx, tmpmtx,
			    points_4[i * 3 + 0], points_4[i * 3 + 1], 0.0f);
	matrix_rotate(glowmtx, MTX_AXIS_Z, pf->tz * 120.0f / 9.0f);
	matrix_scale(glowmtx, 1.5f * 0.18f, 1.5f * 0.18f, 1.0f);
	engine_gldirect_draw_texbox(SHADERS_TEXTURE,
				    0.2f * GLOBJ(0)->alpha,
				    4, glow_vertices, glow_laser_tex_coords,
				    glowmtx, NULL, NULL);
      }
    }
  }

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
}

static void player_fire_reinit(object_t *obj, int doalloc)
{
  player_fire_t *pf = (player_fire_t *)obj->object;

  if(doalloc) {
    engine_globject_alloc(GLOBJ(0));
  }

  if(gfx_details >= GFX_DETAILS_HIGH) {
    if(pf->type == PLAYER_FIRE_T_NORMAL) {
      engine_globject_init_ex(GLOBJ(0), SHADERS_LIGHT1,
			      high_num_vertices, high_vertices, 0,
			      high_num_vertices, high_normals, 0,
			      high_num_vertices, high_colours, 0,
			      high_num_indices, high_indices, 0);
      pf->num_indices = high_num_indices;
    } else if(pf->type == PLAYER_FIRE_T_DROID_NORMAL) {
      engine_globject_init_ex(GLOBJ(0), SHADERS_LIGHT1,
			      high_num_vertices, high_vertices, 0,
			      high_num_vertices, high_normals, 0,
			      high_num_vertices, high_droid_colours, 0,
			      high_num_indices, high_indices, 0);
      pf->num_indices = high_num_indices;
    } else {
      engine_globject_init(GLOBJ(0), SHADERS_POINT_ROUND,
			   num_points, points_4, 0,
			   0, NULL, 0, 0, NULL, 0);
      pf->num_indices = num_points;
      if(screen_density <= 1.0)
	pf->pointsize = 2.0f * (screen_density + 0.5);
      else
	pf->pointsize = 2.0f * (screen_density + 2.0f);
    }
  } else {
    if(pf->type == PLAYER_FIRE_T_NORMAL) {
      engine_globject_init(GLOBJ(0), SHADERS_GOURAUD,
			   low_num_vertices, low_vertices, 0,
			   low_num_vertices, low_colours, 0,
			   low_num_indices, low_indices, 0);
      pf->num_indices = low_num_indices;
    } else if(pf->type == PLAYER_FIRE_T_DROID_NORMAL) {
      engine_globject_init(GLOBJ(0), SHADERS_GOURAUD,
			   low_num_vertices, low_vertices, 0,
			   low_num_vertices, low_droid_colours, 0,
			   low_num_indices, low_indices, 0);
      pf->num_indices = low_num_indices;
    } else {
      engine_globject_init(GLOBJ(0), SHADERS_POINT,
			   num_points, points_4, 0,
			   0, NULL, 0, 0, NULL, 0);
      pf->num_indices = num_points;
      if(screen_density <= 1.0)
	pf->pointsize = screen_density + 0.5;
      else
	pf->pointsize = screen_density + 2.0f;
    }
  }
}

object_t *player_fire_create(object_t *ga_obj, int type, int section,
			     GLfloat x, GLfloat y, GLfloat z, GLfloat angle)
{
  object_t *obj;
  player_fire_t *pf;

  DEBUG(DEBUG_CREATE, " ");

  pf = calloc(1, sizeof(*pf));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = player_fire_destroy;
  obj->update_cb = player_fire_update;
  obj->draw_cb = player_fire_draw;
  obj->reinit_cb = player_fire_reinit;
  obj->object = pf;

  if(type > PLAYER_FIRE_T_DROID_OFFSET)
    FLAG_SET(obj->flags, OBJECT_F_DROID_FIRE);
  else
    FLAG_SET(obj->flags, OBJECT_F_PLAYER);

  if(type == PLAYER_FIRE_T_LASER ||
     type == PLAYER_FIRE_T_DROID_LASER)
    FLAG_SET(obj->flags, OBJECT_F_PFIRE_LASER);

  pf->ga_obj = ga_obj;
  pf->type = type;

  obj->num_globjs = 1;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  player_fire_reinit(obj, 1);

  GLOBJ(0)->alpha = 1.0f;

  GLOBJ(0)->spec_colour[0] = 0.8f;
  GLOBJ(0)->spec_colour[1] = 0.8f;
  GLOBJ(0)->spec_colour[2] = 0.8f;
  GLOBJ(0)->spec_colour[3] = 1.0f;

  GLOBJ(0)->spec_shininess = 16.0f;

  GLOBJ(0)->light1 = &engine_light1;

  pf->section = section;
  pf->tx = x;
  pf->ty = y;
  pf->tz = z;
  pf->angle = angle;

  return obj;
}
