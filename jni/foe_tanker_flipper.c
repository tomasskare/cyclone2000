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
#include "foe_tanker_flipper.h"
#include "foe_flipper.h"
#include "foe_explosion.h"
#include "foe_explosion_sphere.h"
#include "score.h"
#include "object.h"

#include "game_graphics/foe_tanker_flipper_data.h"

typedef struct foe_tanker_flipper_s {
  object_t *ga_obj;

  int ismutant1, ismutant2;
  int section;

  GLfloat tx, ty, tz;
  GLfloat rotate, angle;

  int currstage;
  int num_indices;
} foe_tanker_flipper_t;


static int foe_tanker_flipper_hit(object_t *obj, object_t *srcobj)
{
  foe_tanker_flipper_t *ftf = (foe_tanker_flipper_t *)obj->object;
  object_t *nobj;

  // Killed by superzapper
  if(FLAG_TST(obj->flags, OBJECT_F_FORCE_KILL)) {
    nobj = foe_explosion_create(ftf->ga_obj, ftf->section, ftf->tz, 1);
    engine_object_add(nobj, 1);
    score_increase(400);
    return 1;
  }

  nobj = foe_explosion_sphere_create(ftf->ga_obj, ftf->section, ftf->tz);
  engine_object_add(nobj, 1);

  nobj = foe_flipper_create(ftf->ga_obj, ftf->ismutant1,
			    ftf->section, ftf->tz, -1);
  engine_object_add(nobj, 1);

  nobj = foe_flipper_create(ftf->ga_obj, ftf->ismutant2,
			    ftf->section, ftf->tz, 1);
  engine_object_add(nobj, 1);

  score_increase(100);

  return 1;
}

static void foe_tanker_flipper_destroy(object_t *obj)
{
  foe_tanker_flipper_t *ftf = (foe_tanker_flipper_t *)obj->object;

  free(ftf);
  engine_globject_free(GLOBJ(0));
  free(obj->globjs);
  free(obj);
}

static int foe_tanker_flipper_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  foe_tanker_flipper_t *ftf = (foe_tanker_flipper_t *)obj->object;
  float diff = msec - prev_msec;

  DEBUG(DEBUG_UPDATE, " ");

  // Has caught the player, special treatment
  if(FLAG_TST(obj->flags, OBJECT_F_PLAYER_CAUGHT)) {
    ftf->tz -= 10.0f * diff / 1000;
    return 1;
  }


  ftf->rotate -= diff / 4;
  while(ftf->rotate < 0.0f)
    ftf->rotate += 360.0f;

  ftf->tz += diff / 500;

  if(ftf->tz >= 0.0f && engine_collision_detect(obj,
						OBJECT_F_PLAYER, ftf->section,
						-1.0, (double)ftf->tz))
    return 0;

  // When reaching the edge, split up.
  if(ftf->tz > GAMEAREA_FOE_EDGE_ZPOS) {
    ftf->tz = GAMEAREA_FOE_EDGE_ZPOS;
    foe_tanker_flipper_hit(obj, NULL);
    return 0;
  }

  return 1;
}

static void foe_tanker_flipper_draw(object_t *obj)
{
  foe_tanker_flipper_t *ftf = (foe_tanker_flipper_t *)obj->object;

  DEBUG(DEBUG_DRAW, " ");

  matrix_set_translate(GLOBJ(0)->mtx_model, ftf->tx, ftf->ty, ftf->tz);
  matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Z, ftf->angle);
  matrix_translate(GLOBJ(0)->mtx_model, 0.0f, 0.1f, 0.0f);
  matrix_rotate(GLOBJ(0)->mtx_model, MTX_AXIS_Z, ftf->rotate);

  engine_globject_draw_prepare(GLOBJ(0));
  glDrawElements(GL_TRIANGLES, ftf->num_indices, GL_UNSIGNED_SHORT, 0);
  engine_globject_draw_cleanup(GLOBJ(0));
}

static int foe_tanker_flipper_collision(object_t *obj, object_t *srcobj,
					int section, double minz, double maxz)
{
  foe_tanker_flipper_t *ftf = (foe_tanker_flipper_t *)obj->object;

  if(ftf->section != section)
    return 0;

  if((double)ftf->tz < minz || (double)ftf->tz > maxz)
    return 0;

  return 1;
}

static void foe_tanker_flipper_posget(object_t *obj, int *section, double *zpos)
{
  foe_tanker_flipper_t *ftf = (foe_tanker_flipper_t *)obj->object;

  if(section)
    *section = ftf->section;
  if(zpos)
    *zpos = ftf->tz;
}

static void foe_tanker_flipper_reinit(object_t *obj, int doalloc)
{
  foe_tanker_flipper_t *ftf = (foe_tanker_flipper_t *)obj->object;
  GLfloat *cols;

  if(doalloc) {
    engine_globject_alloc(GLOBJ(0));
  }

  if(gfx_details >= GFX_DETAILS_HIGH) {
    if(ftf->currstage <= 16)
      cols = high_colours_1;
    else
      cols = high_colours_2;

    engine_globject_init_ex(GLOBJ(0), SHADERS_LIGHT1,
			    high_num_vertices, high_vertices, 0,
			    high_num_vertices, high_normals, 0,
			    high_num_vertices, cols, 0,
			    high_num_indices, high_indices, 0);
    ftf->num_indices = high_num_indices;
  } else {
    if(ftf->currstage <= 16)
      cols = low_colours_1;
    else
      cols = low_colours_2;

    engine_globject_init(GLOBJ(0), SHADERS_GOURAUD,
			 low_num_vertices, low_vertices, 0,
			 low_num_vertices, cols, 0,
			 low_num_indices, low_indices, 0);
    ftf->num_indices = low_num_indices;
  }
}

object_t *foe_tanker_flipper_create(object_t *ga_obj, int ismutant1,
				    int ismutant2, int section)
{
  object_t *obj;
  foe_tanker_flipper_t *ftf;

  DEBUG(DEBUG_CREATE, " ");

  ftf = calloc(1, sizeof(*ftf));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = foe_tanker_flipper_destroy;
  obj->update_cb = foe_tanker_flipper_update;
  obj->draw_cb = foe_tanker_flipper_draw;
  obj->collision_cb = foe_tanker_flipper_collision;
  obj->hit_cb = foe_tanker_flipper_hit;
  obj->posget_cb = foe_tanker_flipper_posget;
  obj->reinit_cb = foe_tanker_flipper_reinit;
  obj->object = ftf;

  FLAG_SET(obj->flags, OBJECT_F_FOE);

  ftf->currstage = engine_state_stage_get() + 1;

  obj->num_globjs = 1;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  foe_tanker_flipper_reinit(obj, 1);

  GLOBJ(0)->alpha = 1.0f;

  GLOBJ(0)->spec_colour[0] = 0.6f;
  GLOBJ(0)->spec_colour[1] = 0.5f;
  GLOBJ(0)->spec_colour[2] = 0.5f;
  GLOBJ(0)->spec_colour[3] = 1.0f;

  GLOBJ(0)->spec_shininess = 32.0f;

  GLOBJ(0)->light1 = &engine_light1;

  ftf->ga_obj = ga_obj;

  ftf->ismutant1 = ismutant1;
  ftf->ismutant2 = ismutant2;
  ftf->section = section;
  gamearea_section_pos_get(ftf->ga_obj, ftf->section, &ftf->tx, &ftf->ty);
  ftf->tz = -10.0f;
  ftf->angle = gamearea_section_angle_get(ftf->ga_obj, ftf->section);
  ftf->rotate = 0.0f;

  return obj;
}
