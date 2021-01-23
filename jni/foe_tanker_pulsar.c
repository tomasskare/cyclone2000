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
#include "foe_tanker_pulsar.h"
#include "foe_pulsar.h"
#include "foe_explosion.h"
#include "foe_explosion_sphere.h"
#include "score.h"
#include "banner.h"
#include "object.h"

#include "game_graphics/foe_pulsar_data.h"

typedef struct foe_tanker_pulsar_s {
  object_t *ga_obj;

  int section;

  GLfloat tx, ty, tz;
  GLfloat angle, rotate;
  GLfloat verts[13 * 3];
  GLfloat normals[13 * 3];

  int pulsedir;
  double pulse;

  int num_vertices;
  int num_indices;
} foe_tanker_pulsar_t;


static void foe_tanker_pulsar_destroy(object_t *obj)
{
  foe_tanker_pulsar_t *ftp = (foe_tanker_pulsar_t *)obj->object;

  free(ftp);
  engine_globject_free(GLOBJ(0));
  free(obj->globjs);
  free(obj);
}

static int foe_tanker_pulsar_hit(object_t *obj, object_t *srcobj)
{
  foe_tanker_pulsar_t *ftp = (foe_tanker_pulsar_t *)obj->object;
  object_t *nobj;

  // Killed by superzapper
  if(FLAG_TST(obj->flags, OBJECT_F_FORCE_KILL)) {
    nobj = foe_explosion_create(ftp->ga_obj, ftp->section, ftp->tz, 1);
    engine_object_add(nobj, 1);
    score_increase(400);
    return 1;
  }

  nobj = foe_explosion_sphere_create(ftp->ga_obj, ftp->section, ftp->tz);
  engine_object_add(nobj, 1);

  nobj = foe_pulsar_create(ftp->ga_obj, ftp->section, ftp->tz, 0, -1);
  engine_object_add(nobj, 1);

  nobj = foe_pulsar_create(ftp->ga_obj, ftp->section, ftp->tz, 0, 1);
  engine_object_add(nobj, 1);

  score_increase(100);

  return 1;
}

static int foe_tanker_pulsar_update(object_t *obj, uint64_t prev_msec,
				    uint64_t msec)
{
  foe_tanker_pulsar_t *ftp = (foe_tanker_pulsar_t *)obj->object;
  float diff = msec - prev_msec;
  int i;

  DEBUG(DEBUG_UPDATE, " ");

  ftp->rotate -= diff * 360 / 4000;
  while(ftp->rotate < 0.0)
    ftp->rotate += 360.0;

  if(ftp->pulsedir > 0) {
    ftp->pulse += diff / 1000;
    if(ftp->pulse > 1.0) {
      ftp->pulse = 1.0;
      ftp->pulsedir = -1;
    }
  } else {
    ftp->pulse -= diff / 1000;
    if(ftp->pulse < 0.0) {
      ftp->pulse = 0.0;
      ftp->pulsedir = 1;
    }
  }

  // Only Y needs to be changed
  if(gfx_details >= GFX_DETAILS_HIGH) {
    for(i = 0 ; i < high_num_vertices ; i++) {
      ftp->verts[i * 3 + 1] = (high_vertices_pulse[i * 3 + 1] * ftp->pulse +
			       high_vertices_flat[i * 3 + 1] *
			       (1.0 - ftp->pulse));
      ftp->normals[i * 3 + 0] = (high_normals_pulse[i * 3 + 0] * ftp->pulse +
				 high_normals_flat[i * 3 + 0] *
				 (1.0 - ftp->pulse));
      ftp->normals[i * 3 + 1] = (high_normals_pulse[i * 3 + 1] * ftp->pulse +
				 high_normals_flat[i * 3 + 1] *
				 (1.0 - ftp->pulse));
      ftp->normals[i * 3 + 2] = (high_normals_pulse[i * 3 + 2] * ftp->pulse +
				 high_normals_flat[i * 3 + 2] *
				 (1.0 - ftp->pulse));
    }
  } else {
    for(i = 0 ; i < low_num_vertices ; i++) {
      ftp->verts[i * 3 + 1] = (low_vertices_pulse[i * 3 + 1] * ftp->pulse +
			       low_vertices_flat[i * 3 + 1] *
			       (1.0 - ftp->pulse));
    }
  }

  /* Update pulsar vertices and normals buffer. */
  engine_globject_update_verts(GLOBJ(0), ftp->num_vertices, ftp->verts);
  if(gfx_details >= GFX_DETAILS_HIGH)
    engine_globject_update_normals(GLOBJ(0), ftp->num_vertices, ftp->normals);

  ftp->tz += diff / 900;

  if(ftp->tz >= 0.0f && engine_collision_detect(obj,
						OBJECT_F_PLAYER, ftp->section,
						-1.0, (double)ftp->tz))
    return 0;

  // When reaching the edge, split up.
  if(ftp->tz > GAMEAREA_FOE_EDGE_ZPOS) {
    ftp->tz = GAMEAREA_FOE_EDGE_ZPOS;
    foe_tanker_pulsar_hit(obj, NULL);
    return 0;
  }

  return 1;
}

static void foe_tanker_pulsar_draw(object_t *obj)
{
  foe_tanker_pulsar_t *ftp = (foe_tanker_pulsar_t *)obj->object;
  float tmpmtx[16];

  DEBUG(DEBUG_DRAW, " ");


  matrix_set_translate(tmpmtx, ftp->tx, ftp->ty, ftp->tz);
  matrix_rotate(tmpmtx, MTX_AXIS_Z, ftp->angle);

  if(!FLAG_TST(obj->flags, OBJECT_F_FOE_EDGE))
    matrix_translate(tmpmtx, 0.0f, 0.1f, 0.0f);

  matrix_rotate_to(GLOBJ(0)->mtx_model, tmpmtx,
		   MTX_AXIS_Z, ftp->rotate);
  engine_globject_draw_prepare(GLOBJ(0));
  glDrawElements(GL_TRIANGLES, ftp->num_indices, GL_UNSIGNED_SHORT, 0);

  matrix_rotate_to(GLOBJ(0)->mtx_model, tmpmtx,
		   MTX_AXIS_Z, ftp->rotate + 180.0f);
  engine_globject_update_modelmtx(GLOBJ(0));
  glDrawElements(GL_TRIANGLES, ftp->num_indices, GL_UNSIGNED_SHORT, 0);

  engine_globject_draw_cleanup(GLOBJ(0));
}

static int foe_tanker_pulsar_collision(object_t *obj, object_t *srcobj,
				       int section, double minz, double maxz)
{
  foe_tanker_pulsar_t *ftp = (foe_tanker_pulsar_t *)obj->object;

  if(ftp->section != section)
    return 0;

  if((double)ftp->tz < minz || (double)ftp->tz > maxz)
    return 0;

  return 1;
}

static void foe_tanker_pulsar_posget(object_t *obj, int *section, double *zpos)
{
  foe_tanker_pulsar_t *ftp = (foe_tanker_pulsar_t *)obj->object;

  if(section)
    *section = ftp->section;
  if(zpos)
    *zpos = ftp->tz;
}

static void foe_tanker_pulsar_reinit(object_t *obj, int doalloc)
{
  foe_tanker_pulsar_t *ftp = (foe_tanker_pulsar_t *)obj->object;

  if(doalloc) {
    engine_globject_alloc(GLOBJ(0));
  }

  if(gfx_details >= GFX_DETAILS_HIGH) {
    engine_globject_init_ex(GLOBJ(0), SHADERS_LIGHT1,
			    high_num_vertices, ftp->verts, 1,
			    high_num_vertices, ftp->normals, 1,
			    high_num_vertices, high_colours, 0,
			    high_num_indices, high_indices, 0);
    ftp->num_vertices = high_num_vertices;
    ftp->num_indices = high_num_indices;
  } else {
    engine_globject_init(GLOBJ(0), SHADERS_GOURAUD,
			 low_num_vertices, ftp->verts, 1,
			 low_num_vertices, low_colours, 0,
			 low_num_indices, low_indices, 0);
    ftp->num_vertices = low_num_vertices;
    ftp->num_indices = low_num_indices;
  }
}

object_t *foe_tanker_pulsar_create(object_t *ga_obj, int section)
{
  object_t *obj;
  foe_tanker_pulsar_t *ftp;
  int i;

  DEBUG(DEBUG_CREATE, " ");

  ftp = calloc(1, sizeof(*ftp));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = foe_tanker_pulsar_destroy;
  obj->update_cb = foe_tanker_pulsar_update;
  obj->draw_cb = foe_tanker_pulsar_draw;
  obj->collision_cb = foe_tanker_pulsar_collision;
  obj->hit_cb = foe_tanker_pulsar_hit;
  obj->posget_cb = foe_tanker_pulsar_posget;
  obj->reinit_cb = foe_tanker_pulsar_reinit;
  obj->object = ftp;

  FLAG_SET(obj->flags, OBJECT_F_FOE);
  FLAG_SET(obj->flags, OBJECT_F_FOE_FRIER);

  obj->num_globjs = 1;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  if(gfx_details >= GFX_DETAILS_HIGH) {
    for(i = 0 ; i < high_num_vertices * 3 ; i++) {
      ftp->verts[i] = high_vertices_pulse[i];
      ftp->normals[i] = high_normals_pulse[i];
    }
  } else {
    for(i = 0 ; i < low_num_vertices * 3 ; i++)
      ftp->verts[i] = low_vertices_pulse[i];
  }

  foe_tanker_pulsar_reinit(obj, 1);

  GLOBJ(0)->alpha = 1.0f;

  GLOBJ(0)->spec_colour[0] = 0.8f;
  GLOBJ(0)->spec_colour[1] = 0.8f;
  GLOBJ(0)->spec_colour[2] = 0.8f;
  GLOBJ(0)->spec_colour[3] = 1.0f;

  GLOBJ(0)->spec_shininess = 32.0f;

  GLOBJ(0)->light1 = &engine_light1;

  ftp->ga_obj = ga_obj;

  ftp->section = section;
  gamearea_section_pos_get(ftp->ga_obj, ftp->section, &ftp->tx, &ftp->ty);
  ftp->tz = -10.0f;
  ftp->angle = gamearea_section_angle_get(ftp->ga_obj, ftp->section);
  ftp->pulse = 0.0;
  ftp->pulsedir = 1;

  return obj;
}
