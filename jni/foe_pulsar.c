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
#include "foe_pulsar.h"
#include "foe_fire.h"
#include "foe_explosion.h"
#include "score.h"
#include "object.h"

#include "game_graphics/foe_pulsar_data.h"

typedef struct foe_pulsar_s {
  object_t *ga_obj;

  int section, tosection;

  GLfloat tx, ty, tz;
  GLfloat rtx, rdir;
  GLfloat totx, toty;
  GLfloat fromtx, fromty;
  GLfloat toangle, fromangle;
  GLfloat angle, rotate;
  GLfloat adiff;
  GLfloat verts[13 * 3];
  GLfloat normals[13 * 3];

  int movedir, flipdir;
  double timer;

  int currstage;

  int num_vertices;
  int num_indices;
} foe_pulsar_t;

static int pulsedir = 1;
static double pulse = 0.0;

/* Called once per update round. */
void foe_pulsar_global_update(uint64_t prev_msec, uint64_t msec)
{
  float diff = msec - prev_msec;
  int currstage = engine_state_stage_get() + 1;
  double pulsespeed = currstage < 49 ? 1000 : 800;

  if(pulsedir > 0) {
    pulse += diff / pulsespeed;
    if(pulse > 1.0) {
      pulse = 1.0;
      pulsedir = -1;
    }
  } else {
    pulse -= diff / pulsespeed;
    if(pulse < 0.0) {
      pulse = 0.0;
      pulsedir = 1;
    }
  }
}

static void foe_pulsar_destroy(object_t *obj)
{
  foe_pulsar_t *fp = (foe_pulsar_t *)obj->object;

  free(fp);
  engine_globject_free(GLOBJ(0));
  free(obj->globjs);
  free(obj);
}

static int foe_pulsar_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  foe_pulsar_t *fp = (foe_pulsar_t *)obj->object;
  float diff = msec - prev_msec;
  int i;
  double movespeed = fp->currstage < 49 ? 500 : 400;

  DEBUG(DEBUG_UPDATE, " ");

  // When coming from tanker, rotate to right section
  if(FLAG_TST(obj->flags, OBJECT_F_NOKILL)) {
    fp->rotate -= fp->rdir * fp->adiff * diff / 500;
    if(fp->timer > 500 || fabsf(fp->rotate) >= fp->adiff) {
      fp->section = fp->tosection;
      gamearea_section_pos_get(fp->ga_obj, fp->section, &fp->tx, &fp->ty);
      fp->angle = gamearea_section_angle_get(fp->ga_obj, fp->section);
      fp->timer = 0;
      fp->rotate = 0;

      FLAG_CLR(obj->flags, OBJECT_F_NOKILL);
    }

    fp->timer += diff;

    return 1;
  }

  if(!FLAG_TST(obj->flags, OBJECT_F_FOE_EDGE)) {
    // Only Y needs to be changed
    if(gfx_details >= GFX_DETAILS_HIGH) {
      for(i = 0 ; i < high_num_vertices ; i++) {
	fp->verts[i * 3 + 1] = (high_vertices_pulse[i * 3 + 1] * pulse +
				high_vertices_flat[i * 3 + 1] *
				(1.0 - pulse));
	fp->normals[i * 3 + 0] = (high_normals_pulse[i * 3 + 0] * pulse +
				  high_normals_flat[i * 3 + 0] *
				  (1.0 - pulse));
	fp->normals[i * 3 + 1] = (high_normals_pulse[i * 3 + 1] * pulse +
				  high_normals_flat[i * 3 + 1] *
				  (1.0 - pulse));
	fp->normals[i * 3 + 2] = (high_normals_pulse[i * 3 + 2] * pulse +
				  high_normals_flat[i * 3 + 2] *
				  (1.0 - pulse));
      }
    } else {
      for(i = 0 ; i < low_num_vertices ; i++) {
	fp->verts[i * 3 + 1] = (low_vertices_pulse[i * 3 + 1] * pulse +
				low_vertices_flat[i * 3 + 1] *
				(1.0 - pulse));
      }
    }

    fp->tz += diff / movespeed;

  } else {
    if(gfx_details >= GFX_DETAILS_HIGH) {
      for(i = 0 ; i < high_num_vertices ; i++) {
	fp->verts[i * 3 + 1] = high_vertices_pulse[i * 3 + 1];
	fp->normals[i * 3 + 0] = high_normals_pulse[i * 3 + 0];
	fp->normals[i * 3 + 1] = high_normals_pulse[i * 3 + 1];
	fp->normals[i * 3 + 2] = high_normals_pulse[i * 3 + 2];
      }
    } else {
      for(i = 0 ; i < low_num_vertices ; i++)
	fp->verts[i * 3 + 1] = low_vertices_pulse[i * 3 + 1];
    }

    // Move along the edge at a quick pace.
    if(fp->section != fp->tosection) {
      GLfloat dtx = (fp->totx - fp->fromtx) * diff / 80;
      GLfloat dty = (fp->toty - fp->fromty) * diff / 80;
      GLfloat angdiff = fp->toangle - fp->fromangle;
      GLfloat dang;

      if(angdiff > 180.0)
	angdiff -= 360.0;
      else if(angdiff < -180.0)
	angdiff = -angdiff - 360;

      dang = angdiff * diff / 80;

      if(fabs(fp->totx - fp->tx) < fabs(dtx) ||
	 fabs(fp->toty - fp->ty) < fabs(dty)) {
	fp->tx = fp->totx;
	fp->ty = fp->toty;
	fp->angle = fp->toangle;
	fp->section = fp->tosection;
      } else {
	fp->tx += dtx;
	fp->ty += dty;
	fp->angle += dang;
      }

    } else {
      fp->tosection = gamearea_section_offset_get(fp->ga_obj, fp->section,
						  fp->movedir);
      if(fp->tosection == fp->section) {
	// We hit an edge, turn around
	fp->movedir = -fp->movedir;
	fp->tosection = gamearea_section_offset_get(fp->ga_obj, fp->section,
						    fp->movedir);
      }
      fp->fromtx = fp->tx;
      fp->fromty = fp->ty;
      gamearea_section_pos_get(fp->ga_obj, fp->tosection, &fp->totx, &fp->toty);
      fp->fromangle = fp->angle;
      fp->toangle = gamearea_section_angle_get(fp->ga_obj, fp->tosection);
    }
  }

  /* Update pulsar vertices and normals buffer. */
  engine_globject_update_verts(GLOBJ(0), fp->num_vertices, fp->verts);
  if(gfx_details >= GFX_DETAILS_HIGH)
    engine_globject_update_normals(GLOBJ(0), fp->num_vertices, fp->normals);

  if((pulse > 0.9 || FLAG_TST(obj->flags, OBJECT_F_FOE_EDGE)) &&
     fp->tz >= -8.5) {
    if(engine_collision_detect(obj,
			       OBJECT_F_PLAYER, fp->section,
			       -8.5, GAMEAREA_PLAYER_EDGE_ZPOS + 0.05))
      return 1;
  }

  if(FLAG_TST(obj->flags, OBJECT_F_FOE_EDGE))
    return 1;

  // Change behaviour at rim
  if(fp->tz >= GAMEAREA_FOE_EDGE_ZPOS) {
    object_t *newobj;

    fp->tz = GAMEAREA_FOE_EDGE_ZPOS;
    engine_object_onedge(obj);

    fp->movedir = 1;

    // Split in two
    newobj = foe_pulsar_create(fp->ga_obj, fp->section, fp->tz, -1, 0);
    engine_object_add(newobj, 1);
    engine_object_onedge(newobj);

    return 1;
  }

  return 1;
}

static void foe_pulsar_draw(object_t *obj)
{
  foe_pulsar_t *fp = (foe_pulsar_t *)obj->object;

  DEBUG(DEBUG_DRAW, " ");

  matrix_set_translate(GLOBJ(0)->mtx_model, fp->tx, fp->ty, fp->tz);
  matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Z, fp->angle);

  if(!FLAG_TST(obj->flags, OBJECT_F_FOE_EDGE))
    matrix_translate(GLOBJ(0)->mtx_model, 0.0f, 0.1f, 0.0f);

  if(fp->tosection != fp->section) {
    matrix_translate(GLOBJ(0)->mtx_model, fp->rdir * fp->rtx / 2, 0, 0);
    matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Z, fp->rotate);
    matrix_translate(GLOBJ(0)->mtx_model, -fp->rdir * fp->rtx / 2, 0, 0);
  }

  engine_globject_draw_prepare(GLOBJ(0));
  glDrawElements(GL_TRIANGLES, fp->num_indices, GL_UNSIGNED_SHORT, 0);
  engine_globject_draw_cleanup(GLOBJ(0));
}

static int foe_pulsar_collision(object_t *obj, object_t *srcobj,
				int section, double minz, double maxz)
{
  foe_pulsar_t *fp = (foe_pulsar_t *)obj->object;

  // Miss during flip, but not when it's almost started or finished.
  if((fp->section != section || fp->timer > 100) &&
     (fp->tosection != section || fp->timer < 400))
    return 0;

  if((double)fp->tz < minz || (double)fp->tz > maxz)
    return 0;

  return 1;
}

static int foe_pulsar_hit(object_t *obj, object_t *srcobj)
{
  foe_pulsar_t *fp = (foe_pulsar_t *)obj->object;
  object_t *exp;
  int section = fp->section;

  if(fp->tosection != fp->section && fp->timer > 300)
    section = fp->tosection;

  exp = foe_explosion_create(fp->ga_obj, section, fp->tz, 1);
  engine_object_add(exp, 1);

  score_increase(200);

  return 1;
}

static void foe_pulsar_posget(object_t *obj, int *section, double *zpos)
{
  foe_pulsar_t *fp = (foe_pulsar_t *)obj->object;

  if(section)
    *section = fp->section;
  if(zpos)
    *zpos = fp->tz;
}

static void foe_pulsar_reinit(object_t *obj, int doalloc)
{
  foe_pulsar_t *fp = (foe_pulsar_t *)obj->object;

  if(doalloc) {
    engine_globject_alloc(GLOBJ(0));
  }

  if(gfx_details >= GFX_DETAILS_HIGH) {
    engine_globject_init_ex(GLOBJ(0), SHADERS_LIGHT1,
			    high_num_vertices, fp->verts, 1,
			    high_num_vertices, fp->normals, 1,
			    high_num_vertices, high_colours, 0,
			    high_num_indices, high_indices, 0);
    fp->num_vertices = high_num_vertices;
    fp->num_indices = high_num_indices;
  } else {
    engine_globject_init(GLOBJ(0), SHADERS_GOURAUD,
			 low_num_vertices, fp->verts, 1,
			 low_num_vertices, low_colours, 0,
			 low_num_indices, low_indices, 0);
    fp->num_vertices = low_num_vertices;
    fp->num_indices = low_num_indices;
  }
}

object_t *foe_pulsar_create(object_t *ga_obj, int section, GLfloat zpos,
			    int movedir, int flipdir)
{
  object_t *obj;
  foe_pulsar_t *fp;
  int i;

  DEBUG(DEBUG_CREATE, " ");

  fp = calloc(1, sizeof(*fp));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = foe_pulsar_destroy;
  obj->update_cb = foe_pulsar_update;
  obj->draw_cb = foe_pulsar_draw;
  obj->collision_cb = foe_pulsar_collision;
  obj->hit_cb = foe_pulsar_hit;
  obj->posget_cb = foe_pulsar_posget;
  obj->reinit_cb = foe_pulsar_reinit;
  obj->object = fp;

  FLAG_SET(obj->flags, OBJECT_F_FOE);
  FLAG_SET(obj->flags, OBJECT_F_FOE_FRIER);

  obj->num_globjs = 1;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  if(gfx_details >= GFX_DETAILS_HIGH) {
    for(i = 0 ; i < high_num_vertices * 3 ; i++) {
      fp->verts[i] = high_vertices_pulse[i];
      fp->normals[i] = high_normals_pulse[i];
    }
  } else {
    for(i = 0 ; i < low_num_vertices * 3 ; i++)
      fp->verts[i] = low_vertices_pulse[i];
  }

  foe_pulsar_reinit(obj, 1);

  GLOBJ(0)->alpha = 1.0f;

  GLOBJ(0)->spec_colour[0] = 0.8f;
  GLOBJ(0)->spec_colour[1] = 0.8f;
  GLOBJ(0)->spec_colour[2] = 0.8f;
  GLOBJ(0)->spec_colour[3] = 1.0f;

  GLOBJ(0)->spec_shininess = 32.0f;

  GLOBJ(0)->light1 = &engine_light1;

  fp->ga_obj = ga_obj;

  fp->section = section;
  fp->tosection = section;
  gamearea_section_pos_get(fp->ga_obj, fp->section, &fp->tx, &fp->ty);
  fp->tz = zpos;
  fp->angle = gamearea_section_angle_get(fp->ga_obj, fp->section);

  fp->movedir = movedir;
  fp->flipdir = flipdir;

  if(fp->flipdir) {
    fp->tosection = gamearea_section_offset_get(fp->ga_obj, section,
						fp->flipdir);
    fp->rdir = -fp->flipdir;
    fp->toangle = gamearea_section_angle_get(fp->ga_obj, fp->tosection);
    fp->rtx = gamearea_section_len_get(fp->ga_obj, section);
    FLAG_SET(obj->flags, OBJECT_F_NOKILL);
    fp->adiff = gamearea_sections_angle_diff(fp->ga_obj, fp->section,
					     fp->flipdir);
  }

  fp->currstage = engine_state_stage_get() + 1;

  return obj;
}
