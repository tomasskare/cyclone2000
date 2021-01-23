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
#include "control.h"
#include "object.h"

typedef struct control_s {
  int type;
  int autofire;
  GLfloat ctrlsize;
  GLfloat buttonsize;
  GLfloat jump_alpha;
  GLfloat zap_alpha;
  GLfloat exit_alpha;
  int jump_enabled;
  int zap_enabled;
  int exit_enabled;
} control_t;

static void control_destroy(object_t *obj)
{
  control_t *co = (control_t *)obj->object;

  free(co);
  free(obj);
}

static int control_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  control_t *co = (control_t *)obj->object;
  double diff = msec - prev_msec;

  if(co->jump_enabled && co->jump_alpha < 1.0f) {
    co->jump_alpha += diff / 500;
    if(co->jump_alpha > 1.0f)
      co->jump_alpha = 1.0f;
  } else if(!co->jump_enabled && co->jump_alpha > 0.2f) {
    co->jump_alpha -= diff / 500;
    if(co->jump_alpha < 0.2f)
      co->jump_alpha = 0.2f;
  }

  if(co->zap_enabled && co->zap_alpha < 1.0f) {
    co->zap_alpha += diff / 500;
    if(co->zap_alpha > 1.0f)
      co->zap_alpha = 1.0f;
  } else if(!co->zap_enabled && co->zap_alpha > 0.2f) {
    co->zap_alpha -= diff / 500;
    if(co->zap_alpha < 0.2f)
      co->zap_alpha = 0.2f;
  }

  if(co->exit_enabled && co->exit_alpha < 1.0f) {
    co->exit_alpha += diff / 400;
    if(co->exit_alpha > 1.0f)
      co->exit_alpha = 1.0f;
  } else if(!co->exit_enabled && co->exit_alpha > 0.0f) {
    co->exit_alpha -= diff / 400;
    if(co->exit_alpha < 0.0f)
      co->exit_alpha = 0.0f;
  }

  return 1;
}

static void control_draw(object_t *obj)
{
  control_t *co = (control_t *)obj->object;
  GLfloat x, y, w, h;

  static GLfloat tex_coords[] = { 0.0f,        0.0f,
				  0.0f + 1.0f, 0.0f,
				  0.0f,        0.0f + 1.0f,
				  0.0f + 1.0f, 0.0f + 1.0f };

  DEBUG(DEBUG_DRAW, " ");

  if(co->type == CONTROL_TYPE_HIDDEN || native_is_vr())
    return;

  glDepthRangef(0.0, 0.2);

  glEnable(GL_BLEND);

  // FIXME: Add preference for control position

  x = 0.0f;
  y = screen_height - co->ctrlsize;
  w = co->ctrlsize;
  h = co->ctrlsize;

  if(co->type == CONTROL_TYPE_SINGLE_ARROWS ||
     co->type == CONTROL_TYPE_MULTI_ARROWS)
    glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_CONTROL_FULLARROWS));
  else if(co->type == CONTROL_TYPE_MULTI_CIRCULAR)
    glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_CONTROL_CROSS));
  else if(co->type == CONTROL_TYPE_MULTI_SWIPE ||
	  co->type == CONTROL_TYPE_DUAL_SWIPE) {
    GLfloat size = co->ctrlsize * 256.0 / 320.0;

    glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_CONTROL_SWIPE));
    x = 0.0f;
    y = screen_height - size;
    w = size * 4;
    h = size;
  }
  
  GLfloat vertices0[] = { x,     y,     0.0f,
			  x + w, y,     0.0f,
			  x,     y + h, 0.0f,
			  x + w, y + h, 0.0f };

  engine_gldirect_draw_texbox(SHADERS_TEXTURE, 1.0f,
			      4, vertices0, tex_coords,
			      global_mtx_identity,
			      global_mtx_identity,
			      global_mtx_ortho);


  if(co->type == CONTROL_TYPE_MULTI_ARROWS ||
     co->type == CONTROL_TYPE_MULTI_CIRCULAR ||
     co->type == CONTROL_TYPE_MULTI_SWIPE) {
    if(!co->autofire) {
      x = screen_width - co->buttonsize;
      y = screen_height - co->buttonsize;
      w = co->buttonsize;
      h = co->buttonsize;
      GLfloat vertices2[] = { x,     y,     0.0f,
			      x + w, y,     0.0f,
			      x,     y + h, 0.0f,
			      x + w, y + h, 0.0f };

      glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_CONTROL_SHOOT));
      engine_gldirect_draw_texbox(SHADERS_TEXTURE, 1.0f,
				  4, vertices2, tex_coords,
				  global_mtx_identity,
				  global_mtx_identity,
				  global_mtx_ortho);
    }

    x = screen_width - co->buttonsize;
    if(co->autofire)
      y = screen_height - co->buttonsize;
    else
      y = screen_height - co->buttonsize * 2;
    w = co->buttonsize;
    h = co->buttonsize;
    GLfloat vertices3[] = { x,     y,     0.0f,
			    x + w, y,     0.0f,
			    x,     y + h, 0.0f,
			    x + w, y + h, 0.0f };

    glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_CONTROL_JUMP));
    engine_gldirect_draw_texbox(SHADERS_TEXTURE, co->jump_alpha,
				4, vertices3, tex_coords,
				global_mtx_identity,
				global_mtx_identity,
				global_mtx_ortho);

    x = screen_width - co->buttonsize * 2;
    y = screen_height - co->buttonsize;
    w = co->buttonsize;
    h = co->buttonsize;
    GLfloat vertices4[] = { x,     y,     0.0f,
			    x + w, y,     0.0f,
			    x,     y + h, 0.0f,
			    x + w, y + h, 0.0f };

    glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_CONTROL_ZAP));
    engine_gldirect_draw_texbox(SHADERS_TEXTURE, co->zap_alpha,
				4, vertices4, tex_coords,
				global_mtx_identity,
				global_mtx_identity,
				global_mtx_ortho);
  } else if(co->type == CONTROL_TYPE_DUAL_SWIPE) {
    GLfloat size = co->ctrlsize * 256.0 / 320.0;
    x = size * 1.4;
    y = screen_height - size;
    w = size;
    h = size;
    GLfloat vertices2[] = { x,     y,     0.0f,
			    x + w, y,     0.0f,
			    x,     y + h, 0.0f,
			    x + w, y + h, 0.0f };

    glDepthRangef(0.0, 0.15);
    glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_CONTROL_SWIPEJUMP));
    engine_gldirect_draw_texbox(SHADERS_TEXTURE, co->jump_alpha,
				4, vertices2, tex_coords,
				global_mtx_identity,
				global_mtx_identity,
				global_mtx_ortho);
    glDepthRangef(0.0, 0.2);

    x = screen_width - co->buttonsize;
    y = screen_height - co->buttonsize;
    w = co->buttonsize;
    h = co->buttonsize;
    GLfloat vertices3[] = { x,     y,     0.0f,
			    x + w, y,     0.0f,
			    x,     y + h, 0.0f,
			    x + w, y + h, 0.0f };

    glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_CONTROL_ZAP));
    engine_gldirect_draw_texbox(SHADERS_TEXTURE, co->zap_alpha,
				4, vertices3, tex_coords,
				global_mtx_identity,
				global_mtx_identity,
				global_mtx_ortho);
  } else if(co->type == CONTROL_TYPE_SINGLE_ARROWS) {
    x = 0.0f;
    y = screen_height - co->ctrlsize;
    w = co->ctrlsize;
    h = co->ctrlsize;
    GLfloat vertices2[] = { x,     y,     0.0f,
			    x + w, y,     0.0f,
			    x,     y + h, 0.0f,
			    x + w, y + h, 0.0f };

    glDepthRangef(0.0, 0.17);
    glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_CONTROL_ARROWSJUMP));
    engine_gldirect_draw_texbox(SHADERS_TEXTURE, co->jump_alpha,
				4, vertices2, tex_coords,
				global_mtx_identity,
				global_mtx_identity,
				global_mtx_ortho);

    glDepthRangef(0.0, 0.15);
    glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_CONTROL_ARROWSZAP));
    engine_gldirect_draw_texbox(SHADERS_TEXTURE, co->zap_alpha,
				4, vertices2, tex_coords,
				global_mtx_identity,
				global_mtx_identity,
				global_mtx_ortho);
    glDepthRangef(0.0, 0.2);
  }


  x = screen_width - co->buttonsize;
  y = 0.0f;
  w = co->buttonsize;
  h = co->buttonsize;
  GLfloat vertices1[] = { x,     y,     0.0f,
			  x + w, y,     0.0f,
			  x,     y + h, 0.0f,
			  x + w, y + h, 0.0f };

  glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_CONTROL_PAUSE));
  engine_gldirect_draw_texbox(SHADERS_TEXTURE, 1.0f,
			      4, vertices1, tex_coords,
			      global_mtx_identity,
			      global_mtx_identity,
			      global_mtx_ortho);


  if(co->exit_alpha > 0.0f) {
    x = screen_width - co->buttonsize;
    y = co->buttonsize;
    w = co->buttonsize;
    h = co->buttonsize;
    GLfloat vertices2[] = { x,     y,     0.0f,
			    x + w, y,     0.0f,
			    x,     y + h, 0.0f,
			    x + w, y + h, 0.0f };

    glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_CONTROL_EXIT));
    engine_gldirect_draw_texbox(SHADERS_TEXTURE, co->exit_alpha,
				4, vertices2, tex_coords,
				global_mtx_identity,
				global_mtx_identity,
				global_mtx_ortho);
  }


  glDisable(GL_BLEND);

  glDepthRangef(0.0, 1.0);
}


object_t *control_create(int type, int autofire, int ctrlsize, int buttonsize)
{
  object_t *obj;
  control_t *co;

  DEBUG(DEBUG_CREATE, " ");

  co = calloc(1, sizeof(*co));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = control_destroy;
  obj->update_cb = control_update;
  obj->draw_cb = control_draw;
  obj->object = co;

  co->type = type;
  co->autofire = autofire;
  co->ctrlsize = (GLfloat)ctrlsize;
  co->buttonsize = (GLfloat)buttonsize;
  co->jump_alpha = 0.2f;
  co->zap_alpha = 1.0f;
  co->exit_alpha = 0.0f;

  return obj;
}


void control_type_set(object_t *obj, int type, int autofire,
		      int ctrlsize, int buttonsize)
{
  control_t *co = (control_t *)obj->object;

  co->type = type;
  co->autofire = autofire;
  co->ctrlsize = (GLfloat)ctrlsize;
  co->buttonsize = (GLfloat)buttonsize;
}


void control_enable_set(object_t *obj, int jump_enable, int zap_enable)
{
  control_t *co = (control_t *)obj->object;

  co->jump_enabled = jump_enable;
  co->zap_enabled = zap_enable;
}


void control_enable_exit_set(object_t *obj, int exit_enable, int clear_alpha)
{
  control_t *co = (control_t *)obj->object;

  co->exit_enabled = exit_enable;
  if(clear_alpha)
    co->exit_alpha = 0.0f;
}
