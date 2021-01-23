#include <android/log.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <sys/queue.h>
#include <inttypes.h>

#include <GLES2/gl2.h>

#include "debug.h"
#include "native_main.h"
#include "matrix.h"
#include "shaders.h"
#include "line.h"
#include "gameover.h"
#include "object.h"
#include "starfield.h"
#include "orthoquad.h"
#include "char_coords.h"

#define NUM_ROWS 10

static float gameover_mtx_view[16];

static uint64_t update_msec, update_prev_msec;
static GLfloat text_alpha;
static line_t *title_line;
static GLfloat title_scale;
static line_t *row_lines[NUM_ROWS];
static GLfloat row_scale, row_ty;
static int row_texts_num;
static double timer;
static int chars_tex_id;

static object_t *sf_obj, *fade_obj;

static char *row_texts1[] = {
  "",
  "",
  "",
  "YOU HAVE COMPLETED",
  "ALL 99 LEVELS",
  "IN CYCLONE 2000."
};
static int row_texts1_num = 6;

static char *row_texts2[] = {
  "",
  "",
  "",
  "YOU STILL HAVE SOME",
  "LEVELS TO BEAT IN",
  "CYCLONE 2000."
};
static int row_texts2_num = 6;

void gameover_init(int died)
{
  int i;
  char **row_texts;

  DEBUG(DEBUG_GAMEOVER, "died=%d", died);

  chars_tex_id = native_tex_id(TEX_CHARS);
  timer = 0.0;
  text_alpha = 0.0;
  update_msec = 0.0;

  IFFREE(title_line);

  if(died) {
    title_line = line_build("GAME OVER!");
    title_scale = 0.4;
    row_texts = row_texts2;
    row_texts_num = row_texts2_num;
  } else {
    title_line = line_build("CONGRATULATIONS!");
    title_scale = 0.32;
    row_texts = row_texts1;
    row_texts_num = row_texts1_num;
  }

  row_ty = -3.30;
  row_scale = 0.26;
  for(i = 0 ; i < row_texts_num ; i++) {
    IFFREE(row_lines[i]);
    row_lines[i] = line_build(row_texts[i]);
  }

  if(sf_obj)
    sf_obj->destroy_cb(sf_obj);
  sf_obj = starfield_create(NULL, 1200, gameover_mtx_view);
  FLAG_SET(sf_obj->flags, OBJECT_F_STAGE_RESTART);

  if(fade_obj)
    fade_obj->destroy_cb(fade_obj);

  fade_obj = orthoquad_create(-1, 0, 0, screen_width, screen_height,
			      0.0f, 0.0f, 1.0f, 1.0f, -200,
			      0.8f, 0.8f, 0.7f);
  FLAG_SET(fade_obj->flags, OBJECT_F_STAGE_INIT);
}

int gameover_update(uint64_t msec)
{
  double diff;

  DEBUG(DEBUG_GAMEOVER, " ");

  update_prev_msec = update_msec;
  update_msec = msec;
  if(!update_prev_msec) {
    update_prev_msec = update_msec;
  } else if(update_prev_msec > update_msec) {
    DEBUG(DEBUG_TEMP, "WARNING: update_prev_msec %"PRIu64" is larger"
	  " than update_msec %"PRIu64,
	  update_prev_msec, update_msec);
    update_msec = update_prev_msec;
  }


  sf_obj->update_cb(sf_obj, update_prev_msec, update_msec);

  if(fade_obj) {
    if(!fade_obj->update_cb(fade_obj, update_prev_msec, update_msec)) {
      fade_obj->destroy_cb(fade_obj);
      fade_obj = NULL;
    }
  }


  diff = update_msec - update_prev_msec;

  timer += diff;

  row_ty += 3.30 * diff / 7000;
  if(row_ty > 0.0)
    row_ty = 0.0;

  if(timer < 1000) {
    text_alpha += diff / 1000;
    if(text_alpha > 1.0)
      text_alpha = 1.0;
    return 1;
  }

  text_alpha = 1.0;

  return 1;
}

void gameover_draw(int viewmode)
{
  int i;

  DEBUG(DEBUG_DRAW, " ");

  if(viewmode == VIEWMODE_FULL) {
    matrix_set_translate(gameover_mtx_view, 0.0f, 0.0f, -4.0f);
  } else {
    float ox;
    float tmtx[16];

    if(viewmode == VIEWMODE_LEFT)
      ox = VR_EYE_OFFSET;
    else
      ox = -VR_EYE_OFFSET;

    matrix_set_translate(gameover_mtx_view, ox, 0.0f, 0.0f);
    matrix_multiply(tmtx, gameover_mtx_view, headview_mtx);
    matrix_translate_to(gameover_mtx_view, tmtx, 0.0f, 0.0f,
			native_is_vrtv() ? -5.5f : -4.0f);
  }


  if(fade_obj) {
    fade_obj->draw_cb(fade_obj);
    return;
  }


  sf_obj->draw_cb(sf_obj);


  glFrontFace(GL_CW);
  glEnable(GL_BLEND);

  glBindTexture(GL_TEXTURE_2D, chars_tex_id);

  glDepthRangef(0.0, 0.3);


  line_draw(title_line,
	    -title_line->width * title_scale / 2.0, 1.3,
	    title_scale, title_scale, text_alpha,
	    gameover_mtx_view, global_mtx_proj);

  for(i = 0 ; i < row_texts_num ; i++) {
    if(row_lines[i]->len == 0)
      continue;
    line_draw(row_lines[i],
	      -row_lines[i]->width * row_scale / 2.0,
	      row_ty + 0.8 - i * row_scale,
	      row_scale, row_scale, text_alpha,
	      gameover_mtx_view, global_mtx_proj);
  }


  glDepthRangef(0.0, 1.0);

  glDisable(GL_BLEND);
}
