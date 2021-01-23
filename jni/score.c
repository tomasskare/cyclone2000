#include <jni.h>
#include <android/log.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#include <GLES2/gl2.h>

#include "debug.h"
#include "native_main.h"
#include "engine.h"
#include "matrix.h"
#include "shaders.h"
#include "gamearea.h"
#include "score.h"
#include "life.h"

static int score;

static GLfloat tex_coords[] = { 0.0f,        0.0f,
				0.0f + 1.0f, 0.0f,
				0.0f,        0.0f + 1.0f,
				0.0f + 1.0f, 0.0f + 1.0f };

static void score_destroy(object_t *obj)
{
  free(obj);
}

static void score_draw_normal(object_t *obj)
{
  int numsize = (int)((double)screen_width * 0.04);
  GLfloat x = 0, y = screen_margin_topbottom, w = numsize, h = numsize;
  int numchars, tmps, i;

  tmps = score;
  numchars = 1;
  while(tmps >= 10) {
    numchars++;
    tmps /= 10;
  }

  tmps = score;
  for(i = numchars - 1 ; i >= 0 ; i--) {
    int num = tmps % 10;

    tmps /= 10;

    x = i * w * 0.9 + screen_margin_leftright;

    GLfloat vertices[] = { x,     y,     0.0f,
			   x + w, y,     0.0f,
			   x,     y + h, 0.0f,
			   x + w, y + h, 0.0f };
    
    glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_SCORE_NUMBERS_BASE + num));
    engine_gldirect_draw_texbox(SHADERS_TEXTURE, 1.0f,
				4, vertices, tex_coords,
				global_mtx_identity,
				global_mtx_identity,
				global_mtx_ortho);
  }
}

static void score_draw_vr(object_t *obj)
{
  float mtx_model1[16], mtx_model2[16];
  int numchars, tmps, i;
  float vx = 0.0f, vy = 0.0f, vz = 0.0f;

  engine_viewpoint_get(&vx, &vy, &vz);
  matrix_set_translate(mtx_model1, vx - 1.4f, vy + 1.4f,
		       vz - gamearea_viewdist());
  matrix_scale(mtx_model1, 0.1f, 0.1f, 1.0f);

  tmps = score;
  numchars = 1;
  while(tmps >= 10) {
    numchars++;
    tmps /= 10;
  }

  tmps = score;
  for(i = numchars - 1 ; i >= 0 ; i--) {
    int num = tmps % 10;

    tmps /= 10;

    GLfloat vertices[] = { -1.0f,  1.0f,  0.0f,
			    1.0f,  1.0f,  0.0f,
			   -1.0f, -1.0f,  0.0f,
			    1.0f, -1.0f,  0.0f };

    glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_SCORE_NUMBERS_BASE + num));
    matrix_translate_to(mtx_model2, mtx_model1, i * 2.0f * 0.9f, 0.0f, 0.0f);
    engine_gldirect_draw_texbox(SHADERS_TEXTURE, 1.0f,
				4, vertices, tex_coords,
				mtx_model2, NULL, NULL);
  }
}

static void score_draw(object_t *obj)
{

  DEBUG(DEBUG_DRAW, " ");

  glDepthRangef(0.0, 0.2);

  glEnable(GL_BLEND);

  if(native_is_vr())
    score_draw_vr(obj);
  else
    score_draw_normal(obj);

  glDisable(GL_BLEND);

  glDepthRangef(0.0, 1.0);
}


object_t *score_create(void)
{
  object_t *obj;

  DEBUG(DEBUG_CREATE, " ");

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = score_destroy;
  obj->draw_cb = score_draw;

  score = 0;

  return obj;
}



void score_increase(int add)
{
  score += add;

  /* Extra life every 20000 points. */
  if((score - add) % 20000 + add >= 20000)
    life_increase();
}

void score_increase_nolife(int add)
{
  score += add;
}


int score_current(void)
{
  return score;
}

void score_set(int num)
{
  score = num;
}
