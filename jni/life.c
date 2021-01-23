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
#include "life.h"
#include "char_coords.h"

static GLfloat life_tex_coords[] = { 1.000f, 0.629f,
				     1.000f, 1.000f,
				     0.781f, 0.629f,
				     0.781f, 1.000f };

static int lifecount, lostlifecount;

static void life_destroy(object_t *obj)
{
  free(obj);
}

static void life_draw_normal(object_t *obj)
{
  int numchars, tmps, i, numarray[10];
  int numsize = (int)((double)screen_width * 0.04);
  GLfloat x = screen_margin_leftright, y = screen_margin_topbottom + numsize;
  GLfloat w = numsize * 2.3, h = numsize * 1.3;

  GLfloat life_vertices[] = { x,     y,     0.0f,
			      x + w, y,     0.0f,
			      x,     y + h, 0.0f,
			      x + w, y + h, 0.0f };

  glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_PRICE_LIFE));
  engine_gldirect_draw_texbox(SHADERS_TEXTURE, 1.0f,
			      4, life_vertices, life_tex_coords,
			      global_mtx_identity,
			      global_mtx_identity,
			      global_mtx_ortho);

  x += w * 1.09;
  y += numsize * 0.16;

  tmps = lifecount;
  numchars = 0;
  do {
    numarray[10 - numchars - 1] = tmps % 10;
    numchars++;
    tmps /= 10;
  } while(tmps > 0);

  glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_CHARS));

  for(i = 0 ; i < numchars ; i++) {
    int num = numarray[10 - numchars + i];
    char_coords_t *cc = &chars[num];

    GLfloat tex_coords[] = { cc->tx1, cc->ty2,
			     cc->tx2, cc->ty2,
			     cc->tx1, cc->ty1,
			     cc->tx2, cc->ty1 };

    w = cc->w * numsize;
    h = cc->h * numsize;

    GLfloat vertices[] = { x,     y,     0.0f,
			   x + w, y,     0.0f,
			   x,     y + h, 0.0f,
			   x + w, y + h, 0.0f };

    engine_gldirect_draw_texbox(SHADERS_TEXTURE, 1.0f,
				4, vertices, tex_coords,
				global_mtx_identity,
				global_mtx_identity,
				global_mtx_ortho);

    x += w + numsize * 0.07;
  }
}

static void life_draw_vr(object_t *obj)
{
  float mtx_model1[16], mtx_model2[16];
  float vx = 0.0f, vy = 0.0f, vz = 0.0f;
  int numchars, tmps, i, numarray[10];
  GLfloat x = 0.0f;

  GLfloat life_vertices[] = { -1.15f,  0.65f,  0.0f,
			       1.15f,  0.65f,  0.0f,
			      -1.15f, -0.65f,  0.0f,
			       1.15f, -0.65f,  0.0f };

  engine_viewpoint_get(&vx, &vy, &vz);
  matrix_set_translate(mtx_model1, vx - 1.34f, vy + 1.2f,
		       vz - gamearea_viewdist());
  matrix_scale(mtx_model1, 0.15f, 0.15f, 1.0f);

  glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_PRICE_LIFE));
  engine_gldirect_draw_texbox(SHADERS_TEXTURE, 1.0f,
			      4, life_vertices, life_tex_coords,
			      mtx_model1, NULL, NULL);

  matrix_translate(mtx_model1, 1.4f, 0.0f, 0.0f);

  tmps = lifecount;
  numchars = 0;
  do {
    numarray[10 - numchars - 1] = tmps % 10;
    numchars++;
    tmps /= 10;
  } while(tmps > 0);

  glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_CHARS));

  for(i = 0 ; i < numchars ; i++) {
    int num = numarray[10 - numchars + i];
    char_coords_t *cc = &chars[num];

    GLfloat tex_coords[] = { cc->tx2, cc->ty1,
			     cc->tx1, cc->ty1,
			     cc->tx2, cc->ty2,
			     cc->tx1, cc->ty2 };

    GLfloat vertices[] = {  cc->w / 2, -cc->h / 2, 0.0,
			    -cc->w / 2, -cc->h / 2, 0.0,
			    cc->w / 2,  cc->h / 2, 0.0,
			    -cc->w / 2,  cc->h / 2, 0.0 };

    matrix_translate_to(mtx_model2, mtx_model1, x + cc->w / 2, 0.0, 0.0);

    x += cc->w + 0.1f;

    engine_gldirect_draw_texbox(SHADERS_TEXTURE, 1.0f,
				4, vertices, tex_coords,
				mtx_model2, NULL, NULL);
  }
}

static void life_draw(object_t *obj)
{
  DEBUG(DEBUG_DRAW, " ");

  glDepthRangef(0.0, 0.2);

  glEnable(GL_BLEND);

  if(native_is_vr())
    life_draw_vr(obj);
  else
    life_draw_normal(obj);

  glDisable(GL_BLEND);

  glDepthRangef(0.0, 1.0);
}


object_t *life_create(int init_life)
{
  object_t *obj;

  DEBUG(DEBUG_CREATE, " ");

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = life_destroy;
  obj->draw_cb = life_draw;

  lifecount = init_life;
  lostlifecount = 0;

  return obj;
}


void life_increase(void)
{
  engine_banner_1up();
  lifecount++;
  engine_callback_state_save(1);
}

void life_decrease(void)
{
  lifecount--;
  lostlifecount++;
  engine_callback_state_save(1);
}

int life_current(void)
{
  return lifecount;
}

void life_set(int lifes)
{
  lifecount = lifes;
}

int life_lost(void)
{
  return lostlifecount;
}
