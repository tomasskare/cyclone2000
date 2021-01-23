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
#include "foe_fuseball.h"
#include "score.h"
#include "banner.h"
#include "object.h"

#include "game_graphics/foe_fuseball_data.h"

typedef struct foe_fuseball_s {
  object_t *ga_obj;

  int edge, toedge, section;
  double last_move, timer;

  int armflip[5];
  double last_flip;

  GLfloat tx, ty, tz;
  GLfloat totx, toty;
  GLfloat fromtx, fromty;
  GLfloat toangle, fromangle;
  GLfloat angle, rotate;
  GLfloat offset;

  int num_indices;
} foe_fuseball_t;

static int foe_fuseball_get_section(foe_fuseball_t *fb, int oedge)
{
  int edge = oedge == -1 ? fb->edge : oedge;

  if(fb->section != -1)
    return fb->section;
  else
    return gamearea_section_by_edge(fb->ga_obj, edge);
}

static void foe_fuseball_destroy(object_t *obj)
{
  foe_fuseball_t *fb = (foe_fuseball_t *)obj->object;
  int i;

  free(fb);
  for(i = 0 ; i < obj->num_globjs ; i++)
    engine_globject_free(GLOBJ(i));
  free(obj->globjs);
  free(obj);
}

static int foe_fuseball_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  foe_fuseball_t *fb = (foe_fuseball_t *)obj->object;
  float diff = msec - prev_msec;
  int i;

  DEBUG(DEBUG_UPDATE, " ");

  if(fb->last_flip + 50 < msec) {
    i = rand() % 5;
    fb->armflip[i] = !fb->armflip[i];
    fb->last_flip = msec;
  }

  fb->rotate -= diff * 360 / 4000;
  while(fb->rotate < 0.0)
    fb->rotate += 360.0;

  if(!FLAG_TST(obj->flags, OBJECT_F_FOE_EDGE)) {
    if(fb->section == -1)
      fb->tz += diff / 900;

    // Change behaviour at rim
    if(fb->tz >= GAMEAREA_FOE_EDGE_ZPOS) {
      fb->tz = GAMEAREA_FOE_EDGE_ZPOS;
      engine_object_onedge(obj);
    }
  } else {
    fb->offset -= diff / 2500;
    if(fb->offset < 0.0)
      fb->offset = 0.0;

    // Fuseball fries on both sides of the edge it's on.
    // If it's moving, fries on the section.
    if(engine_collision_detect(obj,
			       OBJECT_F_PLAYER,
			       foe_fuseball_get_section(fb, -1),
			       -1.0, (double)fb->tz))
      return 0;

    // If on edge, check the other section too.
    if(fb->section == -1) {
      int oedge = gamearea_edge_offset_get(fb->ga_obj, fb->edge, -1);
      if(engine_collision_detect(obj,
				 OBJECT_F_PLAYER,
				 foe_fuseball_get_section(fb, oedge),
				 -1.0, (double)fb->tz))
	return 0;
    }
  }

  if(fb->section != -1) {
    GLfloat dtx = (fb->totx - fb->fromtx) * diff / 800;
    GLfloat dty = (fb->toty - fb->fromty) * diff / 800;
    GLfloat angdiff = fb->toangle - fb->fromangle;
    GLfloat dang;

    fb->timer += diff;

    if(angdiff > 180.0)
      angdiff -= 360.0;
    else if(angdiff < -180.0)
      angdiff = -angdiff - 360;

    dang = angdiff * diff / 800;

    if(fabs(fb->totx - fb->tx) < fabs(dtx) ||
       fabs(fb->toty - fb->ty) < fabs(dty)) {
      fb->tx = fb->totx;
      fb->ty = fb->toty;
      fb->angle = fb->toangle;
      fb->edge = fb->toedge;
      fb->section = -1;
    } else {
      fb->tx += dtx;
      fb->ty += dty;
      fb->angle += dang;
    }
  }

  if(fb->last_move + 2500 < msec) {
    // Start move toward player.
    int closed = gamearea_closed_get(fb->ga_obj);
    int num_edges = gamearea_num_edges_get(fb->ga_obj);
    int player_section = gamearea_player_section_get(fb->ga_obj);
    int move_dir = player_section -
      gamearea_section_by_edge(fb->ga_obj, fb->edge);

    if(closed && (move_dir > num_edges / 2 ||
		  move_dir < -num_edges / 2))
      move_dir = -move_dir;

    if(move_dir < 0)
      move_dir = -1;
    else
      move_dir = 1;

    fb->toedge = gamearea_edge_offset_get(fb->ga_obj, fb->edge,
					  move_dir);
    fb->fromtx = fb->tx;
    fb->fromty = fb->ty;
    gamearea_edge_pos_get(fb->ga_obj, fb->toedge, &fb->totx, &fb->toty);
    fb->fromangle = fb->angle;
    fb->toangle = gamearea_edge_angle_get(fb->ga_obj, fb->toedge);
    fb->section = gamearea_section_by_edge(fb->ga_obj, (move_dir < 0 ?
							fb->toedge :
							fb->edge));
    fb->timer = 0.0;
    fb->last_move = msec;
  }

  return 1;
}

static void foe_fuseball_draw(object_t *obj)
{
  foe_fuseball_t *fb = (foe_fuseball_t *)obj->object;
  float tmpmtx[16];
  int i;

  DEBUG(DEBUG_DRAW, " ");

  matrix_set_translate(tmpmtx, fb->tx, fb->ty, fb->tz);
  matrix_rotate(tmpmtx, MTX_AXIS_Z, fb->angle);
  matrix_translate(tmpmtx, 0.0f, fb->offset, 0.0f);

  for(i = 0 ; i < 5 ; i++) {
    int globj = fb->armflip[i] ? i : i + 5;

    matrix_rotate_to(GLOBJ(globj)->mtx_model, tmpmtx, MTX_AXIS_Z,
		     fb->rotate + 72.0f * i);
    engine_globject_draw_prepare(GLOBJ(globj));
    glDrawElements(GL_TRIANGLES, fb->num_indices, GL_UNSIGNED_SHORT, 0);
    engine_globject_draw_cleanup(GLOBJ(globj));
  }
}

static int foe_fuseball_collision(object_t *obj, object_t *srcobj,
				  int section, double minz, double maxz)
{
  foe_fuseball_t *fb = (foe_fuseball_t *)obj->object;

  // Only hit when moving between edges and in the middle of the section.
  if(fb->section != section)
    return 0;

  if(fb->timer < 350.0 || fb->timer > 450.0)
    return 0;

  if((double)fb->tz < minz || (double)fb->tz > maxz)
    return 0;

  return 1;
}

static int foe_fuseball_hit(object_t *obj, object_t *srcobj)
{
  foe_fuseball_t *fb = (foe_fuseball_t *)obj->object;
  object_t *ban;
  banner_coords_t *bn;
  int rnd;

  /* Randomize score 250, 500 or 750. */
  rnd = rand() % 3;
  bn = &score_banners[rnd];
  score_increase(250 * (rnd + 1));

  ban = banner_create(fb->ga_obj, native_tex_id(TEX_FUSEBALL_SCORE),
		      0, fb->tx, fb->ty, fb->tz,
		      bn->tx1, bn->tx2, bn->ty1, bn->ty2, bn->w, bn->h,
		      BANNER_F_DONT_COMPENSATE);
  engine_object_add(ban, 2);

  native_callback(CALLBACK_PLAY_SOUND, SOUND_EXPLOSION, 0, 0);

  return 1;
}

static void foe_fuseball_posget(object_t *obj, int *section, double *zpos)
{
  foe_fuseball_t *fb = (foe_fuseball_t *)obj->object;

  if(section)
    *section = foe_fuseball_get_section(fb, -1);
  if(zpos)
    *zpos = fb->tz;
}

static void foe_fuseball_reinit(object_t *obj, int doalloc)
{
  foe_fuseball_t *fb = (foe_fuseball_t *)obj->object;
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

      fb->num_indices = high_num_indices;
    } else {
      engine_globject_init(GLOBJ(i), SHADERS_LIGHT1,
			   low_num_vertices, low_l_vertices, 0,
			   low_num_vertices, low_colours[i], 0,
			   low_num_indices, low_l_indices, 0);

      engine_globject_init(GLOBJ(i + 5), SHADERS_LIGHT1,
			   low_num_vertices, low_r_vertices, 0,
			   low_num_vertices, low_colours[i], 0,
			   low_num_indices, low_r_indices, 0);

      fb->num_indices = low_num_indices;
    }
  }
}

object_t *foe_fuseball_create(object_t *ga_obj, int edge, GLfloat zpos)
{
  object_t *obj;
  foe_fuseball_t *fb;
  int i;

  DEBUG(DEBUG_CREATE, " ");

  fb = calloc(1, sizeof(*fb));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = foe_fuseball_destroy;
  obj->update_cb = foe_fuseball_update;
  obj->draw_cb = foe_fuseball_draw;
  obj->collision_cb = foe_fuseball_collision;
  obj->hit_cb = foe_fuseball_hit;
  obj->posget_cb = foe_fuseball_posget;
  obj->reinit_cb = foe_fuseball_reinit;
  obj->object = fb;

  FLAG_SET(obj->flags, OBJECT_F_FOE);
  FLAG_SET(obj->flags, OBJECT_F_FOE_FRIER);

  /* Five colours, two flips */
  obj->num_globjs = 5 * 2;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  foe_fuseball_reinit(obj, 1);

  for(i = 0 ; i < 5 * 2 ; i++) {
    GLOBJ(i)->alpha = 1.0f;

    GLOBJ(i)->spec_colour[0] = 0.8f;
    GLOBJ(i)->spec_colour[1] = 0.8f;
    GLOBJ(i)->spec_colour[2] = 0.8f;
    GLOBJ(i)->spec_colour[3] = 1.0f;

    GLOBJ(i)->spec_shininess = 16.0f;

    GLOBJ(i)->light1 = &engine_light1;
  }

  fb->ga_obj = ga_obj;

  fb->edge = edge;
  gamearea_edge_pos_get(fb->ga_obj, fb->edge, &fb->tx, &fb->ty);
  fb->tz = zpos;
  fb->angle = gamearea_edge_angle_get(fb->ga_obj, fb->edge);

  fb->toedge = fb->edge;
  fb->section = -1;
  fb->offset = 0.22f;

  return obj;
}
