#include <jni.h>
#include <android/log.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>

#include <GLES2/gl2.h>

#include "debug.h"
#include "native_main.h"
#include "engine.h"
#include "matrix.h"
#include "shaders.h"
#include "orthoquad.h"
#include "object.h"

typedef struct orthoquad_s {
  int texture_id;
  int fadedir;
  float fadetime;
  GLfloat vertices[3 * 4];
  GLfloat tex_coords[2 * 4];
  GLfloat alpha;
  GLfloat cr, cg, cb;
} orthoquad_t;

static void orthoquad_destroy(object_t *obj)
{
  orthoquad_t *oq = (orthoquad_t *)obj->object;

  free(oq);
  free(obj);
}

/* Only used for fading */
static int orthoquad_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  orthoquad_t *oq = (orthoquad_t *)obj->object;
  float diff = msec - prev_msec;

  DEBUG(DEBUG_UPDATE, " ");

  oq->alpha += oq->fadedir * (diff / oq->fadetime);

  if(oq->alpha > 1.0f)
    oq->alpha = 1.0f;

  if(oq->alpha < 0.0f)
    return 0;

  return 1;
}

static void orthoquad_draw(object_t *obj)
{
  orthoquad_t *oq = (orthoquad_t *)obj->object;

  DEBUG(DEBUG_DRAW, " ");

  if(oq->fadedir == 0)
    glDepthRangef(0.0, 0.8);

  glEnable(GL_BLEND);

  if(oq->fadedir == 0) {
    glBindTexture(GL_TEXTURE_2D, oq->texture_id);
    engine_gldirect_draw_texbox(SHADERS_TEXTURE, 1.0f,
				4, oq->vertices, oq->tex_coords,
				global_mtx_identity,
				global_mtx_identity,
				global_mtx_ortho);
  } else {
    engine_gldirect_draw_colbox(SHADERS_LINE,
				4, oq->vertices,
				oq->cr, oq->cg, oq->cb, oq->alpha,
				global_mtx_identity,
				global_mtx_identity,
				global_mtx_ortho);
  }

  glDisable(GL_BLEND);

  glDepthRangef(0.0, 1.0);
}


object_t *orthoquad_create(int texture_id, float x, float y, float w, float h,
			   float tx, float ty, float tw, float th, int fade,
			   float cr, float cg, float cb)
{
  object_t *obj;
  orthoquad_t *oq;

  DEBUG(DEBUG_CREATE, " ");

  oq = calloc(1, sizeof(*oq));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = orthoquad_destroy;
  if(fade != 0)
    obj->update_cb = orthoquad_update;
  obj->draw_cb = orthoquad_draw;
  obj->object = oq;

  oq->texture_id = texture_id;

  GLfloat vertices[] = { x,     y,     0.0f,
			 x + w, y,     0.0f,
			 x,     y + h, 0.0f,
			 x + w, y + h, 0.0f };

  GLfloat tex_coords[] = { tx,      ty,
			   tx + tw, ty,
			   tx,      ty + th,
			   tx + tw, ty + th };

  memcpy(oq->vertices, vertices, 3 * 4 * sizeof(GLfloat));
  memcpy(oq->tex_coords, tex_coords, 2 * 4 * sizeof(GLfloat));

  if(fade < 0) {
    // fade in
    oq->fadedir = -1;
    oq->alpha = 1.0f;
  } else if(fade > 0) {
    // fade out
    oq->fadedir = 1;
    oq->alpha = 0.0f;
  }

  oq->fadetime = abs(fade);

  oq->cr = cr;
  oq->cg = cg;
  oq->cb = cb;

  return obj;
}
