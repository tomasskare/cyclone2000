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
#include "gamearea.h"
#include "text.h"
#include "char_coords.h"

#define MAXROWS 2

typedef struct char_s {
  int idx;
  GLfloat x, y;
} char_t;

typedef struct text_s {
  object_t *ga_obj;

  GLfloat tx, ty, tz;
  GLfloat sx, sy, sz;
  GLfloat rz, rdir;

  GLfloat timer;
  GLfloat alpha;
  GLfloat totalwidth[MAXROWS];

  int move;

  int numrows;
  int numchars[MAXROWS];
  char_t *chars[MAXROWS];
} text_t;

static void text_destroy(object_t *obj)
{
  text_t *txt = (text_t *)obj->object;
  int r;

  for(r = 0 ; r < txt->numrows ; r++)
    free(txt->chars[r]);
  free(txt);
  free(obj);
}

static int text_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  text_t *txt = (text_t *)obj->object;
  float diff = msec - prev_msec;

  txt->timer += diff;
  if(txt->timer > 3500)
    return 0;

  engine_viewpoint_get(&txt->tx, &txt->ty, &txt->tz);

  if(txt->timer < 2000) {
    txt->rz += txt->rdir * 30.0 * diff / 450;
    if((txt->rdir > 0.0 && txt->rz > 30.0) ||
       (txt->rdir < 0.0 && txt->rz < -30.0))
      txt->rdir = -txt->rdir;

    return 1;
  }

  txt->rz += 30.0 * diff / 2000;

  txt->sy += diff / 400;

  txt->alpha -= 0.7 * diff / 1200;
  if(txt->alpha < 0.0)
    txt->alpha = 0.0;

  return 1;
}

static void text_draw(object_t *obj)
{
  text_t *txt = (text_t *)obj->object;
  int r, i;

  DEBUG(DEBUG_DRAW, " ");

  glEnable(GL_BLEND);

  glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_CHARS));

  for(r = 0 ; r < txt->numrows ; r++) {
    float mtx_model1[16];

    glDepthRangef(0.0, 0.3 - 0.1 * r);

    matrix_set_translate(mtx_model1, txt->tx,
			 txt->ty + 0.4 - ((double)r * 0.13), txt->tz - 2.0);
    matrix_scale(mtx_model1, txt->sx, txt->sy, txt->sz);
    if(native_is_vr())
      matrix_scale(mtx_model1, 0.8f, 0.8f, 0.8f);
    matrix_translate(mtx_model1, -txt->totalwidth[r] / 2, 0.0, 0.0);

    for(i = 0 ; i < txt->numchars[r] ; i++) {
      char_t *ch = &(txt->chars[r])[i];
      char_coords_t *cc = &chars[ch->idx];

      GLfloat tex_coords[] = { cc->tx2, cc->ty1,
			       cc->tx1, cc->ty1,
			       cc->tx2, cc->ty2,
			       cc->tx1, cc->ty2 };

      GLfloat vertices[] = {  cc->w / 2, -cc->h / 2, 0.0,
			      -cc->w / 2, -cc->h / 2, 0.0,
			      cc->w / 2,  cc->h / 2, 0.0,
			      -cc->w / 2,  cc->h / 2, 0.0 };
      float mtx_model2[16];

      matrix_translate_to(mtx_model2, mtx_model1, ch->x + cc->w / 2, 0.0, 0.0);
      matrix_rotate(mtx_model2, MTX_AXIS_Z, txt->rz);

      engine_gldirect_draw_texbox(SHADERS_TEXTURE, txt->alpha,
				  4, vertices, tex_coords,
				  mtx_model2, NULL, NULL);
    }
  }

  glDisable(GL_BLEND);

  glDepthRangef(0.0, 1.0);
}


object_t *text_create(object_t *ga_obj, char *string, int move)
{
  object_t *obj;
  text_t *txt;
  char *strs[MAXROWS], *d;
  int r, i;

  DEBUG(DEBUG_CREATE, " ");

  txt = calloc(1, sizeof(*txt));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = text_destroy;
  obj->update_cb = text_update;
  obj->draw_cb = text_draw;
  obj->object = txt;

  txt->ga_obj = ga_obj;
  txt->alpha = 0.7;

  txt->move = move;
  if(!txt->move) {
    /* update is not called when paused, so we need to do
     * the equivalent here.
     */
    engine_viewpoint_get(&txt->tx, &txt->ty, &txt->tz);
    txt->sx = 0.2;
    txt->sy = 0.2;
  } else {
    txt->sx = 0.125;
    txt->sy = 0.125;
  }

  txt->sz = 1.0;
  txt->rdir = 1.0;

  r = 0;
  strs[r] = strdup(string);
  while((d = strchr(strs[r], '\n'))) {
    *d = '\0';
    r++;
    strs[r] = d + 1;
  }

  txt->numrows = r + 1;

  for(r = 0 ; r < txt->numrows ; r++) {
    txt->numchars[r] = strlen(strs[r]);
    txt->chars[r] = malloc(txt->numchars[r] * sizeof(**txt->chars));

    for(i = 0 ; i < txt->numchars[r] ; i++) {
      if(i == 0)
	txt->totalwidth[r] += 0.07;

      txt->chars[r][i].idx = strs[r][i];
      txt->chars[r][i].x = txt->totalwidth[r];
      txt->chars[r][i].y = 0.0;
      txt->totalwidth[r] += chars[(int)strs[r][i]].w;

      if(i == txt->numchars[r] - 1)
	txt->totalwidth[r] += 0.07;
    }
  }

  // There is only one strdup.
  free(strs[0]);

  return obj;
}
