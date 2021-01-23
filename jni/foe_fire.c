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
#include "foe_fire.h"
#include "foe_explosion.h"
#include "object.h"

#include "game_graphics/foe_fire_data.h"

typedef struct foe_fire_s {
  object_t *ga_obj;

  int section;

  GLfloat tx, ty, tz;
  GLfloat rotate;
  GLfloat angle;

  int num_indices;
} foe_fire_t;

static void foe_fire_destroy(object_t *obj)
{
  foe_fire_t *fi = (foe_fire_t *)obj->object;

  free(fi);
  engine_globject_free(GLOBJ(0));
  free(obj->globjs);
  free(obj);
}

static int foe_fire_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  foe_fire_t *fi = (foe_fire_t *)obj->object;
  float diff = msec - prev_msec;

  DEBUG(DEBUG_UPDATE, " ");

  fi->rotate -= diff / 5;
  while(fi->rotate < 0.0f)
    fi->rotate += 360.0f;

  fi->tz += diff / 200;
  // If the fire hits something, it should be removed too.
  if(fi->tz >= 0.0f && engine_collision_detect(obj,
					       OBJECT_F_PLAYER, fi->section,
					       -1.0, (double)fi->tz))
    return 0;

  if(fi->tz > 0.15f)
    return 0;

  return 1;
}

static void foe_fire_draw(object_t *obj)
{
  foe_fire_t *fi = (foe_fire_t *)obj->object;
  float tmpmtx[16];

  DEBUG(DEBUG_DRAW, " ");

  matrix_set_translate(tmpmtx, fi->tx, fi->ty, fi->tz);
  matrix_rotate(tmpmtx, MTX_AXIS_Z, fi->angle);
  matrix_translate(tmpmtx, 0.0f, 0.1f, 0.0f);

  matrix_rotate_to(GLOBJ(0)->mtx_model, tmpmtx,
		   MTX_AXIS_Z, fi->rotate);
  matrix_translate(GLOBJ(0)->mtx_model, 0.0f, -2.5f * SCALEY, 0.0f);
  engine_globject_draw_prepare(GLOBJ(0));
  glDrawElements(GL_TRIANGLES, fi->num_indices, GL_UNSIGNED_SHORT, 0);

  matrix_rotate_to(GLOBJ(0)->mtx_model, tmpmtx,
		   MTX_AXIS_Z, fi->rotate + 120.0f);
  matrix_translate(GLOBJ(0)->mtx_model, 0.0f, -2.5f * SCALEY, 0.0f);
  engine_globject_update_modelmtx(GLOBJ(0));
  glDrawElements(GL_TRIANGLES, fi->num_indices, GL_UNSIGNED_SHORT, 0);

  matrix_rotate_to(GLOBJ(0)->mtx_model, tmpmtx,
		   MTX_AXIS_Z, fi->rotate + 240.0f);
  matrix_translate(GLOBJ(0)->mtx_model, 0.0f, -2.5f * SCALEY, 0.0f);
  engine_globject_update_modelmtx(GLOBJ(0));
  glDrawElements(GL_TRIANGLES, fi->num_indices, GL_UNSIGNED_SHORT, 0);

  engine_globject_draw_cleanup(GLOBJ(0));
}

static int foe_fire_collision(object_t *obj, object_t *srcobj,
			      int section, double minz, double maxz)
{
  foe_fire_t *fi = (foe_fire_t *)obj->object;

  if(fi->section != section)
    return 0;

  if((double)fi->tz < minz || (double)fi->tz > maxz)
    return 0;

  return 1;
}

static int foe_fire_hit(object_t *obj, object_t *srcobj)
{
  foe_fire_t *fi = (foe_fire_t *)obj->object;

  object_t *exp = foe_explosion_create(fi->ga_obj, fi->section, fi->tz, 1);
  engine_object_add(exp, 1);

  return 1;
}

static void foe_fire_posget(object_t *obj, int *section, double *zpos)
{
  foe_fire_t *fi = (foe_fire_t *)obj->object;

  if(section)
    *section = fi->section;
  if(zpos)
    *zpos = fi->tz;
}

static void foe_fire_reinit(object_t *obj, int doalloc)
{
  foe_fire_t *fi = (foe_fire_t *)obj->object;

  if(doalloc) {
    engine_globject_alloc(GLOBJ(0));
  }

  if(gfx_details >= GFX_DETAILS_HIGH) {
    engine_globject_init_ex(GLOBJ(0), SHADERS_LIGHT1,
			    high_num_vertices, high_vertices, 0,
			    high_num_vertices, high_normals, 0,
			    high_num_vertices, high_colours, 0,
			    high_num_indices, high_indices, 0);
    fi->num_indices = high_num_indices;
  } else {
    engine_globject_init(GLOBJ(0), SHADERS_GOURAUD,
			 low_num_vertices, low_vertices, 0,
			 low_num_vertices, low_colours, 0,
			 low_num_indices, low_indices, 0);
    fi->num_indices = low_num_indices;
  }
}

object_t *foe_fire_create(object_t *ga_obj, int section, GLfloat z)
{
  object_t *obj;
  foe_fire_t *fi;

  DEBUG(DEBUG_CREATE, " ");

  fi = calloc(1, sizeof(*fi));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = foe_fire_destroy;
  obj->update_cb = foe_fire_update;
  obj->draw_cb = foe_fire_draw;
  obj->collision_cb = foe_fire_collision;
  obj->hit_cb = foe_fire_hit;
  obj->posget_cb = foe_fire_posget;
  obj->reinit_cb = foe_fire_reinit;
  obj->object = fi;

  FLAG_SET(obj->flags, OBJECT_F_FOE);
  FLAG_SET(obj->flags, OBJECT_F_FOE_FIRE);

  fi->ga_obj = ga_obj;
  fi->section = section;

  obj->num_globjs = 1;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  foe_fire_reinit(obj, 1);

  GLOBJ(0)->alpha = 1.0f;

  GLOBJ(0)->spec_colour[0] = 1.0f;
  GLOBJ(0)->spec_colour[1] = 1.0f;
  GLOBJ(0)->spec_colour[2] = 1.0f;
  GLOBJ(0)->spec_colour[3] = 1.0f;

  GLOBJ(0)->spec_shininess = 16.0f;

  GLOBJ(0)->light1 = &engine_light1;

  gamearea_section_pos_get(fi->ga_obj, fi->section, &fi->tx, &fi->ty);
  fi->angle = gamearea_section_angle_get(fi->ga_obj, fi->section);
  fi->tz = z + 0.01f;

  return obj;
}
