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
#include "player_death.h"
#include "object.h"

typedef struct player_death_s {
  object_t *ga_obj;

  int type;
  int showsparks;

  GLfloat coords[10 * 3];

  GLfloat tx, ty;
  GLfloat z[4];
  GLfloat s[4];
  int num;

  uint64_t last_update;
  uint64_t timer;
} player_death_t;

static const int num_vertices = 10;

#define OUTER_SCALE 0.3
#define INNER_SCALE 0.1
#define OUTER_RAND  0.5  // Random is before scaling (-1.0 - 1.0)
#define INNER_RAND  0.4

static GLfloat star_coordinates[] = {
  1.0000f, 0.0000f,
  0.8090f, 0.5878f,
  0.3090f, 0.9511f,
  -0.3090f, 0.9511f,
  -0.8090f, 0.5878f,
  -1.0000f, 0.0000f,
  -0.8090f, -0.5878f,
  -0.3090f, -0.9511f,
  0.3090f, -0.9511f,
  0.8090f, -0.5878f,
};

static void player_death_destroy(object_t *obj)
{
  player_death_t *pd = (player_death_t *)obj->object;

  free(pd);
  free(obj);
}

static int player_death_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  player_death_t *pd = (player_death_t *)obj->object;
  float diff = msec - prev_msec;
  GLfloat vx, vy, vz;
  int i;

  DEBUG(DEBUG_UPDATE, " ");

  engine_viewpoint_get(&vx, &vy, &vz);

  pd->timer += diff;
  if(!pd->showsparks) {
    if(pd->timer > 2500)
      return 0;

    if(pd->timer > 1000) {
      if(pd->num) {
	engine_fade_out(1000);
	pd->num = 0;
      }
    } else {
      GLfloat dx = pd->tx - vx;
      GLfloat dy = pd->ty - vy;
      vx += dx / 100;
      vy += dy / 100;
    }

    vz += diff / 70;
    engine_viewpoint_set(vx, vy, vz);

    return 1;
  }

  if(pd->timer < 2500) {
    // Move viewpoint to above explosion
    GLfloat dx = pd->tx - vx;
    GLfloat dy = pd->ty - vy;
    vx += dx / 25;
    vy += dy / 25;
    vz += diff / 800;

    for(i = 0 ; i < pd->num ; i++) {
      pd->z[i] += i * diff / 400;
    }
  } else if(pd->timer < 3500) {
    if(pd->num) {
      engine_fade_out(1500);
      pd->num = 0;
    }
    vz += diff / 70;
  } else {
    return 0;
  }

  engine_viewpoint_set(vx, vy, vz);


  if(pd->last_update && msec - pd->last_update < 20) {
    return 1;
  }
  pd->last_update = msec;

  for(i = 0 ; i < 10 ; i += 2) {
    // outer
    pd->coords[3 * i + 0] = OUTER_SCALE *
      (star_coordinates[2 * i + 0] +
       (OUTER_RAND * ((double)rand() / (double)RAND_MAX) - OUTER_RAND/2));
    pd->coords[3 * i + 1] = OUTER_SCALE *
      (star_coordinates[2 * i + 1] +
       (OUTER_RAND * ((double)rand() / (double)RAND_MAX) - OUTER_RAND/2));
    pd->coords[3 * i + 2] = 0.0f;

    // inner
    pd->coords[3 * i + 3] = INNER_SCALE *
      (star_coordinates[2 * i + 2] +
       (INNER_RAND * ((double)rand() / (double)RAND_MAX) - OUTER_RAND/2));
    pd->coords[3 * i + 4] = INNER_SCALE *
      (star_coordinates[2 * i + 3] +
       (INNER_RAND * ((double)rand() / (double)RAND_MAX) - OUTER_RAND/2));
    pd->coords[3 * i + 5] = 0.0f;
  }

  engine_globject_update_verts(GLOBJ(0), num_vertices, pd->coords);

  return 1;
}

static void player_death_draw(object_t *obj)
{
  player_death_t *pd = (player_death_t *)obj->object;
  int i;

  DEBUG(DEBUG_DRAW, " ");

  if(!pd->showsparks)
    return;

  glDepthRangef(0.0, 0.9);
  glEnable(GL_BLEND);

  engine_globject_draw_prepare(GLOBJ(0));
  engine_globject_set_colour(GLOBJ(0), 0.85f, 0.85f, 0.0f);

  for(i = 0 ; i < pd->num ; i++) {
    matrix_set_translate(GLOBJ(0)->mtx_model, pd->tx, pd->ty, pd->z[i]);
    engine_globject_update_modelmtx(GLOBJ(0));
    glDrawArrays(GL_LINE_LOOP, 0, num_vertices);
  }

  engine_globject_draw_cleanup(GLOBJ(0));

  glDisable(GL_BLEND);
  glDepthRangef(0.0, 1.0);
}

void player_death_reinit(object_t *obj, int doalloc)
{
  player_death_t *pd = (player_death_t *)obj->object;

  if(doalloc) {
    engine_globject_alloc(GLOBJ(0));
  }

  engine_globject_init(GLOBJ(0), SHADERS_LINE,
		       num_vertices, pd->coords, 1,
		       0, NULL, 0,
		       0, NULL, 0);
}

object_t *player_death_create(object_t *ga_obj, int showsparks)
{
  object_t *obj;
  player_death_t *pd;
  int i, pos;

  DEBUG(DEBUG_CREATE, " ");

  pd = calloc(1, sizeof(*pd));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = player_death_destroy;
  obj->update_cb = player_death_update;
  obj->draw_cb = player_death_draw;
  obj->reinit_cb = player_death_reinit;
  obj->object = pd;

  obj->num_globjs = 1;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  player_death_reinit(obj, 1);

  GLOBJ(0)->alpha = 1.0f;

  pd->ga_obj = ga_obj;

  pos = gamearea_player_section_get(pd->ga_obj);
  gamearea_section_pos_get(pd->ga_obj, pos, &pd->tx, &pd->ty);
  GLfloat z = gamearea_player_zpos_get(pd->ga_obj) + GAMEAREA_PLAYER_EDGE_ZPOS;

  pd->showsparks = showsparks;
  pd->num = 4;
  for(i = 0 ; i < pd->num ; i++) {
    pd->z[i] = z;
    pd->s[i] = 1.0f;
  }

  native_callback(CALLBACK_PLAY_SOUND, SOUND_PLAYER_EXPLOSION, 0, 0);

  return obj;
}
