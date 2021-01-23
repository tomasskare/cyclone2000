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
#include "powerup.h"
#include "object.h"

#include "game_graphics/points_data.h"
#include "game_graphics/powerup_data.h"

typedef struct powerup_s {
  object_t *ga_obj;

  int section;

  GLfloat ca[4];
  GLfloat cs[4];
  GLfloat cz[4];
  GLfloat cr;
  GLfloat ccr[4], ccg[4], ccb[4];

  GLfloat tx, ty, tz;
  GLfloat rotate;
  GLfloat angle;

  GLfloat timer;
  int state;

  int num_indices;
  GLfloat pointsize;

  gllight_t *lightsource;
} powerup_t;

#define POWERUP_S_INIT   0
#define POWERUP_S_NORMAL 1
#define POWERUP_S_END    2

#define GLOBJ_POWERUP    0
#define GLOBJ_POINT_21   1
#define GLOBJ_POINT_12   2
#define GLOBJ_NUM        3

static void powerup_destroy(object_t *obj)
{
  powerup_t *pu = (powerup_t *)obj->object;
  int i;

  engine_gllight_destroy(pu->lightsource);
  free(pu);
  for(i = 0 ; i < obj->num_globjs ; i++)
    engine_globject_free(GLOBJ(i));
  free(obj->globjs);
  free(obj);
}

static int powerup_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  powerup_t *pu = (powerup_t *)obj->object;
  float diff = msec - prev_msec;
  int i;
  float lightmtx[16];

  DEBUG(DEBUG_UPDATE, " ");

  // Doesn't really loop, I just want to be able to break out.
  while(pu->state == POWERUP_S_INIT) {
    int colidx;
    GLfloat colfade;
    GLfloat timer;

#define INIT_LEN 2000.0f

    pu->timer += diff;
    if(pu->timer > INIT_LEN) {
      pu->state = POWERUP_S_NORMAL;
      for(i = 0 ; i < 4 ; i++) {
	pu->cs[i] = 0.17;
      }
      break;
    }

    // Fade up powerup to half
    GLOBJ(GLOBJ_POWERUP)->alpha += 0.5 * diff / INIT_LEN;
    if(GLOBJ(GLOBJ_POWERUP)->alpha > 0.5)
      GLOBJ(GLOBJ_POWERUP)->alpha = 0.5;


    pu->rotate += diff / 30;
    while(pu->rotate >= 360.0f)
      pu->rotate -= 360.0f;


    for(i = 0 ; i < 4 ; i++) {
      timer = pu->timer - i * 150.0;
      if(timer < 0.0)
	continue;

      pu->cr += 20 * diff / INIT_LEN;
      while(pu->cr >= 360.0f)
	pu->cr -= 360.0f;

      pu->cz[i] -= 2.0 * diff / INIT_LEN;
      pu->cs[i] -= (6.7 + i * 0.4) * (timer / INIT_LEN) * diff / INIT_LEN;
      if(pu->cs[i] < 0.17)
	pu->cs[i] = 0.17;

      pu->ca[i] += 1.0 * diff / (INIT_LEN / 3);
      if(pu->ca[i] > 1.0f)
	pu->ca[i] = 1.0f;

      colidx = (int)(3.0 * timer / INIT_LEN);
      colfade = (3.0 * timer / INIT_LEN) - (GLfloat)colidx;

      pu->ccr[i] = (point_21_colours[colidx       * 3 + 0] * (1.0 - colfade) +
		    point_21_colours[(colidx + 1) * 3 + 0] * colfade);
      pu->ccg[i] = (point_21_colours[colidx       * 3 + 1] * (1.0 - colfade) +
		    point_21_colours[(colidx + 1) * 3 + 1] * colfade);
      pu->ccb[i] = (point_21_colours[colidx       * 3 + 2] * (1.0 - colfade) +
		    point_21_colours[(colidx + 1) * 3 + 2] * colfade);
    }

    matrix_set_translate(lightmtx, pu->tx, pu->ty, pu->tz);
    matrix_rotate(lightmtx, MTX_AXIS_Z, pu->angle);
    matrix_translate(lightmtx, 0.0f, 0.2f, 0.0f);
    matrix_rotate(lightmtx, MTX_AXIS_Z, pu->rotate);
    engine_gllight_point_update(pu->lightsource, GLOBJ(GLOBJ_POWERUP)->alpha,
				lightmtx);

    break;
  }

  if(pu->state == POWERUP_S_NORMAL) {
    int colidx;
    GLfloat colfade;
    double minz;

    pu->cr += diff / 400;
    while(pu->cr >= 360.0f)
      pu->cr -= 360.0f;

    pu->rotate += diff / 10;
    while(pu->rotate >= 360.0f)
      pu->rotate -= 360.0f;

    // Fade up powerup to full
    GLOBJ(GLOBJ_POWERUP)->alpha += diff / 500;
    if(GLOBJ(GLOBJ_POWERUP)->alpha > 1.0)
      GLOBJ(GLOBJ_POWERUP)->alpha = 1.0;

    pu->tz += diff / 200;

    // If player is exiting game area, match all the way down.
    if(gamearea_exiting_speed(pu->ga_obj) > 0.0)
      minz = -9.0;
    else
      minz = pu->tz - 1.5;

    // Keep powerup after collision to run end sequence.
    if(engine_collision_detect(obj, OBJECT_F_PLAYER, pu->section,
			       minz, pu->tz + 0.8)) {
      engine_gllight_destroy(pu->lightsource);
      pu->lightsource = NULL;
      return 1;
    }
    
    for(i = 0 ; i < 4 ; i++) {
      colidx = (int)(3.0 * (-pu->tz) / 9.0);
      colfade = (3.0 * (-pu->tz) / 9.0) - (GLfloat)colidx;
      
      pu->ccr[i] = (point_12_colours[colidx       * 3 + 0] * (1.0 - colfade) +
		    point_12_colours[(colidx + 1) * 3 + 0] * colfade);
      pu->ccg[i] = (point_12_colours[colidx       * 3 + 1] * (1.0 - colfade) +
		    point_12_colours[(colidx + 1) * 3 + 1] * colfade);
      pu->ccb[i] = (point_12_colours[colidx       * 3 + 2] * (1.0 - colfade) +
		    point_12_colours[(colidx + 1) * 3 + 2] * colfade);
    }

    // Powerup is not taken
    if(pu->tz > 0.25f) {
      return 0;
    }

    matrix_set_translate(lightmtx, pu->tx, pu->ty, pu->tz);
    matrix_rotate(lightmtx, MTX_AXIS_Z, pu->angle);
    matrix_translate(lightmtx, 0.0f, 0.2f, 0.0f);
    matrix_rotate(lightmtx, MTX_AXIS_Z, pu->rotate);
    engine_gllight_point_update(pu->lightsource, GLOBJ(GLOBJ_POWERUP)->alpha,
				lightmtx);

  } else if(pu->state == POWERUP_S_END) {

#define END_LEN 2000

    pu->timer += diff;
    if(pu->timer > END_LEN)
      return 0;

    for(i = 0 ; i < 4 ; i++) {
      pu->cs[i] += 6.0f * diff / END_LEN;
      pu->ca[i] -= 1.0f * diff / END_LEN;
      if(pu->ca[i] < 0.0f)
	pu->ca[i] = 0.0f;
    }

  }

  return 1;
}

static void powerup_draw(object_t *obj)
{
  powerup_t *pu = (powerup_t *)obj->object;
  int i;
  float glowmtx[16];

  DEBUG(DEBUG_DRAW, " ");

  glDepthRangef(0.0, 0.94);

  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);

  if(pu->state == POWERUP_S_INIT ||
     pu->state == POWERUP_S_NORMAL) {

    // Draw fading up or moving powerup


    matrix_set_translate(GLOBJ(GLOBJ_POWERUP)->mtx_model,
			 pu->tx, pu->ty, pu->tz);
    matrix_rotate(GLOBJ(GLOBJ_POWERUP)->mtx_model, MTX_AXIS_Z, pu->angle);
    matrix_translate(GLOBJ(GLOBJ_POWERUP)->mtx_model, 0.0f, 0.1f, 0.0f);
    matrix_rotate(GLOBJ(GLOBJ_POWERUP)->mtx_model, MTX_AXIS_Z, pu->rotate);
    engine_globject_draw_prepare(GLOBJ(GLOBJ_POWERUP));
    glDrawElements(GL_TRIANGLES, pu->num_indices, GL_UNSIGNED_SHORT, 0);
    engine_globject_draw_cleanup(GLOBJ(GLOBJ_POWERUP));

    if(gfx_details >= GFX_DETAILS_HIGH) {
      matrix_set_translate(glowmtx, pu->tx, pu->ty, pu->tz);
      matrix_rotate(glowmtx, MTX_AXIS_Z, pu->angle);
      matrix_translate(glowmtx, 0.0f, 0.1f, 0.0f);
      matrix_rotate(glowmtx, MTX_AXIS_Z, pu->rotate);
      matrix_scale(glowmtx, 2.0f * SCALEX, 2.0f * SCALEX, 0.0f);
      glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_1UP_1_GLOW));
      engine_gldirect_draw_texbox(SHADERS_TEXTURE,
				  0.7f * GLOBJ(GLOBJ_POWERUP)->alpha,
				  4, glow_vertices, glow_tex_coords,
				  glowmtx, NULL, NULL);
    }

  }

  if(pu->state == POWERUP_S_INIT) {
    // Draw points

    engine_globject_draw_prepare(GLOBJ(GLOBJ_POINT_21));

    for(i = 0 ; i < 4 ; i++) {
      if(pu->ca[i] == 0.0f)
	continue;

      matrix_set_translate(GLOBJ(GLOBJ_POINT_21)->mtx_model,
			   pu->tx, pu->ty, pu->cz[i]);
      matrix_scale(GLOBJ(GLOBJ_POINT_21)->mtx_model,
		   pu->cs[i], pu->cs[i], pu->cs[i]);
      matrix_rotate(GLOBJ(GLOBJ_POINT_21)->mtx_model, MTX_AXIS_Z, pu->cr);
      engine_globject_update_modelmtx(GLOBJ(GLOBJ_POINT_21));
      engine_globject_set_colour(GLOBJ(GLOBJ_POINT_21),
				 pu->ccr[i], pu->ccg[i], pu->ccb[i]);
      engine_globject_set_alpha(GLOBJ(GLOBJ_POINT_21), pu->ca[i]);
      engine_globject_set_pointsize(GLOBJ(GLOBJ_POINT_21), pu->pointsize);
      glDrawArrays(GL_POINTS, 0, 21);
    }

    engine_globject_draw_cleanup(GLOBJ(GLOBJ_POINT_21));

  }

  if(pu->state == POWERUP_S_NORMAL ||
     pu->state == POWERUP_S_END) {

    // Draw points around powerup

    engine_globject_draw_prepare(GLOBJ(GLOBJ_POINT_12));

    for(i = 0 ; i < 4 ; i++) {
      matrix_set_translate(GLOBJ(GLOBJ_POINT_12)->mtx_model,
			   pu->tx, pu->ty,
			   pu->tz - 0.7 + i * (0.7 * 2.0 / 3.0));
      matrix_scale(GLOBJ(GLOBJ_POINT_12)->mtx_model,
		   pu->cs[i], pu->cs[i], pu->cs[i]);
      matrix_rotate(GLOBJ(GLOBJ_POINT_12)->mtx_model, MTX_AXIS_Z, pu->angle);
      matrix_translate(GLOBJ(GLOBJ_POINT_12)->mtx_model, 0.0f, 0.1f, 0.0f);
      matrix_rotate(GLOBJ(GLOBJ_POINT_12)->mtx_model, MTX_AXIS_Z, pu->cr);
      engine_globject_update_modelmtx(GLOBJ(GLOBJ_POINT_12));
      engine_globject_set_colour(GLOBJ(GLOBJ_POINT_12),
				 pu->ccr[i], pu->ccg[i], pu->ccb[i]);
      engine_globject_set_alpha(GLOBJ(GLOBJ_POINT_12), pu->ca[i]);
      engine_globject_set_pointsize(GLOBJ(GLOBJ_POINT_12), pu->pointsize);
      glDrawArrays(GL_POINTS, 0, 12);
    }

    engine_globject_draw_cleanup(GLOBJ(GLOBJ_POINT_12));

  }

  glDepthRangef(0.0, 1.0);

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
}

static void powerup_reinit(object_t *obj, int doalloc)
{
  powerup_t *pu = (powerup_t *)obj->object;

  if(doalloc) {
    engine_globject_alloc(GLOBJ(GLOBJ_POWERUP));
    engine_globject_alloc(GLOBJ(GLOBJ_POINT_21));
    engine_globject_alloc(GLOBJ(GLOBJ_POINT_12));
  }

  if(gfx_details >= GFX_DETAILS_HIGH) {
    engine_globject_init_ex(GLOBJ(GLOBJ_POWERUP), SHADERS_LIGHT1,
			    high_num_vertices, high_vertices, 0,
			    high_num_vertices, high_normals, 0,
			    high_num_vertices, high_colours, 0,
			    high_num_indices, high_indices, 0);
    pu->num_indices = high_num_indices;

    engine_globject_init(GLOBJ(GLOBJ_POINT_21), SHADERS_POINT_ROUND,
			 21, points_21, 0,
			 0, NULL, 0,
			 0, NULL, 0);

    engine_globject_init(GLOBJ(GLOBJ_POINT_12), SHADERS_POINT_ROUND,
			 12, points_12, 0,
			 0, NULL, 0,
			 0, NULL, 0);

    if(screen_density <= 1.0)
      pu->pointsize = 2.0f * (screen_density + 0.5);
    else
      pu->pointsize = 2.0f * (screen_density + 2.0f);
  } else {
    engine_globject_init(GLOBJ(GLOBJ_POWERUP), SHADERS_GOURAUD,
			 low_num_vertices, low_vertices, 0,
			 low_num_vertices, low_colours, 0,
			 low_num_indices, low_indices, 0);
    pu->num_indices = low_num_indices;

    engine_globject_init(GLOBJ(GLOBJ_POINT_21), SHADERS_POINT,
			 21, points_21, 0,
			 0, NULL, 0,
			 0, NULL, 0);

    engine_globject_init(GLOBJ(GLOBJ_POINT_12), SHADERS_POINT,
			 12, points_12, 0,
			 0, NULL, 0,
			 0, NULL, 0);

    if(screen_density <= 1.0)
      pu->pointsize = screen_density + 0.5;
    else
      pu->pointsize = screen_density + 2.0f;
  }

}

object_t *powerup_create(object_t *ga_obj, int section, GLfloat z)
{
  object_t *obj;
  powerup_t *pu;
  int i;

  DEBUG(DEBUG_CREATE, " ");

  pu = calloc(1, sizeof(*pu));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = powerup_destroy;
  obj->update_cb = powerup_update;
  obj->draw_cb = powerup_draw;
  obj->reinit_cb = powerup_reinit;
  obj->object = pu;

  FLAG_SET(obj->flags, OBJECT_F_POWERUP);

  pu->ga_obj = ga_obj;
  pu->section = section;

  gamearea_section_pos_get(pu->ga_obj, pu->section, &pu->tx, &pu->ty);
  pu->angle = gamearea_section_angle_get(pu->ga_obj, pu->section);
  pu->tz = z + 0.01f;
  if(pu->tz > 0.0f)
    pu->tz = -0.2f;

  pu->state = POWERUP_S_INIT;
  pu->timer = 0.0f;
  for(i = 0 ; i < 4 ; i++) {
    pu->ca[i] = 0.0f;
    pu->cs[i] = 3.0f;
    pu->cz[i] = pu->tz + 2.0f + i * 0.2f;
  }

  obj->num_globjs = GLOBJ_NUM;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  powerup_reinit(obj, 1);

  // Reset colour alpha for stage init
  GLOBJ(GLOBJ_POWERUP)->alpha = 0.0f;

  GLOBJ(GLOBJ_POWERUP)->spec_colour[0] = 1.0f;
  GLOBJ(GLOBJ_POWERUP)->spec_colour[1] = 1.0f;
  GLOBJ(GLOBJ_POWERUP)->spec_colour[2] = 1.0f;
  GLOBJ(GLOBJ_POWERUP)->spec_colour[3] = 1.0f;

  GLOBJ(GLOBJ_POWERUP)->spec_shininess = 9.0f;

  GLOBJ(GLOBJ_POWERUP)->light1 = &engine_light1;

  pu->lightsource = engine_gllight_point_create(1.0, 0.8, 0.3,
						1.0, 1.0, 0.8);

  return obj;
}


void powerup_taken(object_t *obj)
{
  powerup_t *pu = (powerup_t *)obj->object;

  pu->state = POWERUP_S_END;
  pu->timer = 0.0f;
}
