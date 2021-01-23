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
#include "superzapper.h"
#include "foe_fire.h"
#include "foe_explosion.h"
#include "score.h"
#include "object.h"

#define NUM_LIGHTS 4

typedef struct superzapper_s {
  object_t *ga_obj;

  GLfloat ox1, oy1, oz1, ox2, oy2, oz2;
  GLfloat ex, ey;
  double ez;
  GLfloat l1[26 * 3];
  GLfloat l2[26 * 3];

  uint64_t timer_change_beam;
  uint64_t timer_next_update;
  uint64_t timer_next_foe;

  int numfoes;
  int foecnt;

  gllight_t *lightsources[NUM_LIGHTS];
} superzapper_t;

#define GLOBJ_1   0
#define GLOBJ_2   1
#define GLOBJ_NUM 2

static const int num_vertices = 26;

static void superzapper_destroy(object_t *obj)
{
  superzapper_t *sz = (superzapper_t *)obj->object;
  int i;

  for(i = 0 ; i < NUM_LIGHTS ; i++)
    engine_gllight_destroy(sz->lightsources[i]);
  free(sz);
  for(i = 0 ; i < obj->num_globjs ; i++)
    engine_globject_free(GLOBJ(i));
  free(obj->globjs);
  free(obj);
}

static int superzapper_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  superzapper_t *sz = (superzapper_t *)obj->object;
  float diff = msec - prev_msec;
  object_t *foe_obj;
  int section, edge1, edge2, i, get_foe = 0;
  double dx, dy, dz;
  float lightmtx[16];
  int lnum = 0;

  sz->timer_change_beam += diff;
  sz->timer_next_update += diff;
  sz->timer_next_foe += diff;

  // Change focus of beam
  if(sz->timer_change_beam > 150) {
    sz->timer_change_beam = 0;
    sz->timer_next_update = 10000;
    get_foe = 1;
  }

  // Kill more foes
  if(!get_foe && sz->timer_next_foe > 800 / sz->numfoes) {
    sz->timer_next_foe = 0;
    get_foe = 2;
  }

  if(get_foe) {
    foe_obj = engine_object_first_foe_get(OBJECT_F_NOZAP);
    if(!foe_obj)
      return 0;

    sz->foecnt++;
    if(sz->foecnt > sz->numfoes)
      return 0;

    if(get_foe == 1) {
      foe_obj->posget_cb(foe_obj, &section, &sz->ez);
      gamearea_section_pos_get(sz->ga_obj, section, &sz->ex, &sz->ey);
    }

    engine_kill_foe(foe_obj);
  }


  if(sz->timer_next_update < 20) {
    return 1;
  }

  // Get player position edges
  edge1 = gamearea_player_section_get(sz->ga_obj);
  gamearea_edge_pos_get(sz->ga_obj, edge1, &sz->ox1, &sz->oy1);
  edge2 = gamearea_edge_offset_get(sz->ga_obj, edge1, 1);
  gamearea_edge_pos_get(sz->ga_obj, edge2, &sz->ox2, &sz->oy2);
  sz->oz1 = sz->oz2 = gamearea_player_zpos_get(sz->ga_obj);

  // Split the lines into 25 sections, and randomize some offsets.
  dx = (sz->ex - sz->ox1) / 25;
  dy = (sz->ey - sz->oy1) / 25;
  dz = (sz->ez - sz->oz1) / 25;
  for(i = 0 ; i < 26 ; i++) {
    sz->l1[i * 3 + 0] = sz->ox1 + i * dx;
    sz->l1[i * 3 + 1] = sz->oy1 + i * dy;
    sz->l1[i * 3 + 2] = sz->oz1 + i * dz;
    if(i < 25) {
      sz->l1[i * 3 + 0] += (0.14 * ((double)rand() / (double)RAND_MAX) - 0.07);
      sz->l1[i * 3 + 1] += (0.14 * ((double)rand() / (double)RAND_MAX) - 0.07);
      sz->l1[i * 3 + 2] += (0.14 * ((double)rand() / (double)RAND_MAX) - 0.07);
    }

    if(i == 20 || i == 24) {
      matrix_set_translate(lightmtx,
			   sz->l1[i * 3 + 0],
			   sz->l1[i * 3 + 1],
			   sz->l1[i * 3 + 2]);
      engine_gllight_point_update(sz->lightsources[lnum++], 1.0f, lightmtx);
    }

  }

  dx = (sz->ex - sz->ox2) / 25;
  dy = (sz->ey - sz->oy2) / 25;
  dz = (sz->ez - sz->oz2) / 25;
  for(i = 0 ; i < 26 ; i++) {
    sz->l2[i * 3 + 0] = sz->ox2 + i * dx;
    sz->l2[i * 3 + 1] = sz->oy2 + i * dy;
    sz->l2[i * 3 + 2] = sz->oz2 + i * dz;
    if(i < 25) {
      sz->l2[i * 3 + 0] += (0.14 * ((double)rand() / (double)RAND_MAX) - 0.07);
      sz->l2[i * 3 + 1] += (0.14 * ((double)rand() / (double)RAND_MAX) - 0.07);
      sz->l2[i * 3 + 2] += (0.14 * ((double)rand() / (double)RAND_MAX) - 0.07);
    }

    if(i == 21 || i == 25) {
      matrix_set_translate(lightmtx,
			   sz->l2[i * 3 + 0],
			   sz->l2[i * 3 + 1],
			   sz->l2[i * 3 + 2]);
      engine_gllight_point_update(sz->lightsources[lnum++], 1.0f, lightmtx);
    }
  }

  engine_globject_update_verts(GLOBJ(GLOBJ_1), num_vertices, sz->l1);
  engine_globject_update_verts(GLOBJ(GLOBJ_2), num_vertices, sz->l2);

  return 1;
}

static void superzapper_draw(object_t *obj)
{
  DEBUG(DEBUG_DRAW, " ");

  glDepthRangef(0.0, 0.9);
  glEnable(GL_BLEND);

  engine_globject_draw_prepare(GLOBJ(GLOBJ_1));
  engine_globject_set_colour(GLOBJ(GLOBJ_1), 0.85f, 0.85f, 0.0f);
  glDrawArrays(GL_LINE_STRIP, 0, num_vertices);
  engine_globject_draw_cleanup(GLOBJ(GLOBJ_1));

  engine_globject_draw_prepare(GLOBJ(GLOBJ_2));
  engine_globject_set_colour(GLOBJ(GLOBJ_2), 0.85f, 0.85f, 0.0f);
  glDrawArrays(GL_LINE_STRIP, 0, num_vertices);
  engine_globject_draw_cleanup(GLOBJ(GLOBJ_2));

  glDisable(GL_BLEND);
  glDepthRangef(0.0, 1.0);
}

static void superzapper_reinit(object_t *obj, int doalloc)
{
  superzapper_t *sz = (superzapper_t *)obj->object;

  if(doalloc) {
    engine_globject_alloc(GLOBJ(GLOBJ_1));
    engine_globject_alloc(GLOBJ(GLOBJ_2));
  }

  engine_globject_init(GLOBJ(GLOBJ_1), SHADERS_LINE,
		       num_vertices, sz->l1, 1,
		       0, NULL, 0,
		       0, NULL, 0);

  engine_globject_init(GLOBJ(GLOBJ_2), SHADERS_LINE,
		       num_vertices, sz->l2, 1,
		       0, NULL, 0,
		       0, NULL, 0);
}

object_t *superzapper_create(object_t *ga_obj)
{
  object_t *obj;
  superzapper_t *sz;
  int i;

  DEBUG(DEBUG_CREATE, " ");

  sz = calloc(1, sizeof(*sz));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = superzapper_destroy;
  obj->update_cb = superzapper_update;
  obj->draw_cb = superzapper_draw;
  obj->reinit_cb = superzapper_reinit;
  obj->object = sz;

  // Used to prevent powerup counting
  FLAG_SET(obj->flags, OBJECT_F_DROID_FIRE);

  obj->num_globjs = GLOBJ_NUM;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  superzapper_reinit(obj, 1);

  GLOBJ(GLOBJ_1)->alpha = 1.0f;
  matrix_identity(GLOBJ(GLOBJ_1)->mtx_model);
  GLOBJ(GLOBJ_2)->alpha = 1.0f;
  matrix_identity(GLOBJ(GLOBJ_2)->mtx_model);

  sz->ga_obj = ga_obj;

  // Always trigger next foe in update
  sz->timer_change_beam = 10000;

  sz->numfoes = engine_object_get_num_filtered(OBJECT_F_FOE,
					       OBJECT_F_FOE_SPIKES);
  sz->foecnt = 0;

  for(i = 0 ; i < NUM_LIGHTS ; i++) {
    sz->lightsources[i] = engine_gllight_point_create(1.0, 0.8, 0.3,
						      1.0, 1.0, 0.8);
  }

  return obj;
}
