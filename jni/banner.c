#include <jni.h>
#include <android/log.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#include <GLES2/gl2.h>

#include "debug.h"
#include "engine.h"
#include "matrix.h"
#include "shaders.h"
#include "gamearea.h"
#include "banner.h"

typedef struct banner_s {
  object_t *ga_obj;

  int texture_id;
  int flags;

  double init_wait;

  GLfloat tex_coords[2 * 4];
  GLfloat vertices[3 * 4];

  GLfloat tx, ty, tz;
  GLfloat xoff, yoff, zoff;

  GLfloat timer;
  GLfloat scale;
  GLfloat alpha;

  GLfloat depth_range;
} banner_t;

static void banner_destroy(object_t *obj)
{
  banner_t *bn = (banner_t *)obj->object;

  free(bn);
  free(obj);
}

static int banner_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  banner_t *bn = (banner_t *)obj->object;
  double diff = msec - prev_msec;

  bn->timer += diff;

  if(bn->timer < bn->init_wait)
    return 1;

  if(bn->timer > bn->init_wait + 1000)
    return 0;

  bn->scale += diff / 300;

  bn->alpha -= diff / 1000;
  if(bn->alpha < 0.0)
    bn->alpha = 0.0;

  bn->depth_range -= 0.1 * diff / 1000;
  if(bn->depth_range < 0.5)
    bn->depth_range = 0.5;

  if(!FLAG_TST(bn->flags, BANNER_F_DONT_COMPENSATE))
    engine_viewpoint_get(&bn->tx, &bn->ty, &bn->tz);

  return 1;
}

static void banner_draw(object_t *obj)
{
  banner_t *bn = (banner_t *)obj->object;
  float mtx_model[16];

  DEBUG(DEBUG_DRAW, " ");

  if(bn->timer < bn->init_wait)
    return;

  glDepthRangef(0.0, bn->depth_range);

  glEnable(GL_BLEND);

  matrix_set_translate(mtx_model, bn->xoff, bn->yoff, bn->zoff);

  if(!FLAG_TST(bn->flags, BANNER_F_DONT_COMPENSATE))
    matrix_translate(mtx_model, bn->tx, bn->ty, 1.1);

  matrix_scale(mtx_model, bn->scale, bn->scale, bn->scale);

  glBindTexture(GL_TEXTURE_2D, bn->texture_id);

  engine_gldirect_draw_texbox(SHADERS_TEXTURE, bn->alpha,
			      4, bn->vertices, bn->tex_coords,
			      mtx_model, NULL, NULL);

  glDisable(GL_BLEND);
  glDepthRangef(0.0, 1.0);
}


object_t *banner_create(object_t *ga_obj, int texture_id, double init_wait,
			double xoff, double yoff, double zoff,
			double tx1, double tx2, double ty1, double ty2,
			double w, double h, int flags)
{
  object_t *obj;
  banner_t *bn;

  DEBUG(DEBUG_CREATE, " ");

  bn = calloc(1, sizeof(*bn));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = banner_destroy;
  obj->update_cb = banner_update;
  obj->draw_cb = banner_draw;
  obj->object = bn;

  bn->ga_obj = ga_obj;
  bn->texture_id = texture_id;
  bn->init_wait = init_wait;
  bn->alpha = 1.0;
  bn->xoff = xoff;
  bn->yoff = yoff;
  bn->zoff = zoff;
  bn->flags = flags;

  bn->depth_range = 0.6;

  bn->vertices[0 * 3 + 0] = -w / 2;
  bn->vertices[0 * 3 + 1] = h / 2;
  bn->vertices[0 * 3 + 2] = 0.0;
  bn->vertices[1 * 3 + 0] = w / 2;
  bn->vertices[1 * 3 + 1] = h / 2;
  bn->vertices[1 * 3 + 2] = 0.0;
  bn->vertices[2 * 3 + 0] = -w / 2;
  bn->vertices[2 * 3 + 1] = -h / 2;
  bn->vertices[2 * 3 + 2] = 0.0;
  bn->vertices[3 * 3 + 0] = w / 2;
  bn->vertices[3 * 3 + 1] = -h / 2;
  bn->vertices[3 * 3 + 2] = 0.0;

  bn->tex_coords[0 * 2 + 0] = tx1;
  bn->tex_coords[0 * 2 + 1] = ty1;
  bn->tex_coords[1 * 2 + 0] = tx2;
  bn->tex_coords[1 * 2 + 1] = ty1;
  bn->tex_coords[2 * 2 + 0] = tx1;
  bn->tex_coords[2 * 2 + 1] = ty2;
  bn->tex_coords[3 * 2 + 0] = tx2;
  bn->tex_coords[3 * 2 + 1] = ty2;

  return obj;
}
