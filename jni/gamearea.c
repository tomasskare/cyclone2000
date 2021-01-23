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
#include "player_fire.h"
#include "object.h"

typedef struct gamearea_s {
  int closed_area;

  int num_edges;
  int num_sections;
  GLfloat *player_positions;
  GLfloat *player_angles;
  GLfloat *strip_normals;
  GLfloat *vertex_normals;
  GLfloat *strip_lengths;
  GLfloat *edge_angles;
  int *edge_convex;

  int num_vertices;
  GLfloat *vertices;
  GLfloat *poly_colours;
  GLfloat *colour_buffer;
  float poly_colours_offset;
  float poly_colours_fade;
  float poly_colours_fade_dir;
  int move_colours;
  int transparent;

  int no_highlight;
  int move_line_colours;
  int line_colours_offset;
  int num_line_colours;
  GLfloat *line_colours_orig;
  GLfloat *line_colours;
  float line_colours_lastmove;

  int num_line_indices;
  GLushort *line_indices;

  int num_poly_indices;
  GLushort *poly_indices;

  float player_speed;
  int player_shoot;
  int player_jump;

  int player_jump_step;
  int player_jump_enabled;
  GLfloat player_zpos;
  int player_shoot_type;
  uint64_t player_shoot_last;
  uint64_t player_shoot_last_sound;
  int player_shoot_numrow;
  float player_move_dir;
  float player_float_pos;
  int player_pos;

  int exiting;
  GLfloat exiting_speed;
  GLfloat exit_wait;
  int exit_sound_played;

  GLfloat init_timer;
  GLfloat init_tz;
  GLfloat init_fade;
  GLfloat init_rx, init_ry, init_rz;
} gamearea_t;

#define GLOBJ_POLYS   0
#define GLOBJ_LINES   1
#define GLOBJ_NUM     2

static void gamearea_destroy(object_t *obj)
{
  gamearea_t *ga = (gamearea_t *)obj->object;
  int i;

  free(ga->player_positions);
  free(ga->player_angles);
  free(ga->strip_normals);
  free(ga->vertex_normals);
  free(ga->strip_lengths);
  free(ga->edge_angles);
  free(ga->edge_convex);
  free(ga->vertices);
  free(ga->line_indices);
  free(ga->poly_indices);
  free(ga->poly_colours);
  free(ga->colour_buffer);
  free(ga->line_colours_orig);
  free(ga->line_colours);
  free(ga);
  for(i = 0 ; i < obj->num_globjs ; i++)
    engine_globject_free(GLOBJ(i));
  free(obj->globjs);
  free(obj);
}

static int gamearea_update(object_t *obj, uint64_t prev_msec, uint64_t msec)
{
  gamearea_t *ga = (gamearea_t *)obj->object;
  float diff = msec - prev_msec;
  GLfloat vx, vy, vz;
  int i;

  DEBUG(DEBUG_UPDATE, " ");

  engine_viewpoint_get(&vx, &vy, &vz);
  
  // Possibly move line colours
  if(ga->move_line_colours &&
     msec - ga->line_colours_lastmove > 150.0f) {
    float movediff = msec - ga->line_colours_lastmove;
    ga->line_colours_lastmove = msec;
    ga->line_colours_offset += (int)(movediff / 150.0f);
    ga->line_colours_offset %= ga->num_edges;
  }

  // Stage init special treatment
  if(FLAG_TST(obj->flags, OBJECT_F_STAGE_INIT)) {
    int off = ga->line_colours_offset;
    int num = ga->num_edges;

    for(i = 0 ; i < ga->num_edges ; i++) {
      ga->line_colours[i * 6 + 0] =
	ga->line_colours_orig[((i + off) % num) * 6 + 0] * ga->init_fade;
      ga->line_colours[i * 6 + 1] =
	ga->line_colours_orig[((i + off) % num) * 6 + 1] * ga->init_fade;
      ga->line_colours[i * 6 + 2] =
	ga->line_colours_orig[((i + off) % num) * 6 + 2] * ga->init_fade;
      ga->line_colours[i * 6 + 3] =
	ga->line_colours_orig[((i + off) % num) * 6 + 3] * ga->init_fade;
      ga->line_colours[i * 6 + 4] =
	ga->line_colours_orig[((i + off) % num) * 6 + 4] * ga->init_fade;
      ga->line_colours[i * 6 + 5] =
	ga->line_colours_orig[((i + off) % num) * 6 + 5] * ga->init_fade;
    }

    ga->init_timer += diff;

    if(ga->init_timer < 500)
      return 1;

    ga->init_tz += 20.0f * diff / 2000;
    if(ga->init_tz > 0.0f)
      ga->init_tz = 0.0f;

    ga->init_rx += 180.0f * diff / 2000;
    if(ga->init_rx > 180.0f)
      ga->init_rx = 180.0f;

    ga->init_ry += 180.0f * diff / 2000;
    if(ga->init_ry > 180.0f)
      ga->init_ry = 180.0f;

    ga->init_rz += 180.0f * diff / 2000;
    if(ga->init_rz > 180.0f)
      ga->init_rz = 180.0f;

    // Fade up wireframe
    ga->init_fade += diff / 1500;
    if(ga->init_fade > 1.0f)
      ga->init_fade = 1.0f;

    if(ga->init_timer < 2500)
      return 1;

    GLfloat px = ga->player_positions[ga->player_pos * 2 + 0] * 0.6f;
    GLfloat py = ga->player_positions[ga->player_pos * 2 + 1] * 0.6f;
    GLfloat dx = px - vx;
    GLfloat dy = py - vy;

    vx += dx / 20;
    vy += dy / 20;

    engine_viewpoint_set(vx, vy, vz);

    // Fade up rest of area
    GLOBJ(GLOBJ_POLYS)->alpha += diff / 500;
    if(GLOBJ(GLOBJ_POLYS)->alpha > 1.0f)
      GLOBJ(GLOBJ_POLYS)->alpha = 1.0f;

    GLOBJ(GLOBJ_LINES)->spec_colour[0] += 0.7f * diff / 500;
    GLOBJ(GLOBJ_LINES)->spec_colour[1] += 0.7f * diff / 500;
    GLOBJ(GLOBJ_LINES)->spec_colour[2] += 0.7f * diff / 500;
    if(GLOBJ(GLOBJ_LINES)->spec_colour[0] > 1.0f) {
      GLOBJ(GLOBJ_LINES)->spec_colour[0] = 1.0f;
      GLOBJ(GLOBJ_LINES)->spec_colour[1] = 1.0f;
      GLOBJ(GLOBJ_LINES)->spec_colour[2] = 1.0f;
    }

    if(ga->init_timer < 3250)
      return 1;

    GLOBJ(GLOBJ_POLYS)->alpha = 1.0f;

    return 1;
  }


  // Calculate new player position
  ga->player_float_pos += ga->player_move_dir *
    ((ga->player_speed * diff) / 2200.0);

  if(ga->closed_area) {
    while(ga->player_float_pos >= ga->num_sections)
      ga->player_float_pos -= ga->num_sections;
    while(ga->player_float_pos < 0)
      ga->player_float_pos += ga->num_sections;
  } else {
    if(ga->player_float_pos >= ga->num_sections)
      ga->player_float_pos = ga->num_sections - 1;
    if(ga->player_float_pos < 0)
      ga->player_float_pos = 0;
  }

  ga->player_pos = (int)ga->player_float_pos;


  // Highlight player section or move colours
  for(i = 0 ; i < ga->num_edges ; i++) {
    int edge1 = ga->player_pos;
    int edge2 = gamearea_edge_offset_get(obj, ga->player_pos, 1);
    int off = ga->line_colours_offset;
    int num = ga->num_edges;

    if(!ga->no_highlight && (i == edge1 || i == edge2)) {
      ga->line_colours[i * 6 + 0] = 0.8;
      ga->line_colours[i * 6 + 1] = 0.8;
      ga->line_colours[i * 6 + 2] = 0.0;
      ga->line_colours[i * 6 + 3] = 0.6;
      ga->line_colours[i * 6 + 4] = 0.6;
      ga->line_colours[i * 6 + 5] = 0.0;
    } else {
      ga->line_colours[i * 6 + 0] =
	ga->line_colours_orig[((i + off) % num) * 6 + 0];
      ga->line_colours[i * 6 + 1] =
	ga->line_colours_orig[((i + off) % num) * 6 + 1];
      ga->line_colours[i * 6 + 2] =
	ga->line_colours_orig[((i + off) % num) * 6 + 2];
      ga->line_colours[i * 6 + 3] =
	ga->line_colours_orig[((i + off) % num) * 6 + 3];
      ga->line_colours[i * 6 + 4] =
	ga->line_colours_orig[((i + off) % num) * 6 + 4];
      ga->line_colours[i * 6 + 5] =
	ga->line_colours_orig[((i + off) % num) * 6 + 5];
    }
  }

  // Create fire object, if shooting
  if(ga->player_shoot && ga->player_zpos > -9.1) {
    GLfloat pf_x, pf_y, pf_z, pf_a;
    pf_x = ga->player_positions[ga->player_pos * 2 + 0];
    pf_y = ga->player_positions[ga->player_pos * 2 + 1];
    pf_z = ga->player_zpos;
    pf_a = ga->player_angles[ga->player_pos];

    if(ga->player_shoot_type == PLAYER_FIRE_T_LASER &&
       msec - ga->player_shoot_last > 90) {

      if(msec - ga->player_shoot_last_sound > 180) {
	ga->player_shoot_last_sound = msec;
	native_callback(CALLBACK_PLAY_SOUND, SOUND_LASER, 0, 0);
      }

      ga->player_shoot_last = msec;
      engine_object_add(player_fire_create(obj, PLAYER_FIRE_T_LASER,
					   ga->player_pos,
					   pf_x, pf_y, pf_z, pf_a), 1);

    } else if(ga->player_shoot_type == PLAYER_FIRE_T_NORMAL) {

      if(ga->player_shoot_numrow < 8 && msec - ga->player_shoot_last > 100) {
	if(msec - ga->player_shoot_last_sound > 100) {
	  ga->player_shoot_last_sound = msec;
	  native_callback(CALLBACK_PLAY_SOUND, SOUND_SHOT, 0, 0);
	}

	ga->player_shoot_last = msec;
	ga->player_shoot_numrow++;
	engine_object_add(player_fire_create(obj, PLAYER_FIRE_T_NORMAL,
					     ga->player_pos,
					     pf_x, pf_y, pf_z, pf_a), 1);
      } else if(ga->player_shoot_numrow >= 8 &&
		msec - ga->player_shoot_last > 400) {
	if(msec - ga->player_shoot_last_sound > 100) {
	  ga->player_shoot_last_sound = msec;
	  native_callback(CALLBACK_PLAY_SOUND, SOUND_SHOT, 0, 0);
	}
	ga->player_shoot_last = msec;
	ga->player_shoot_numrow = 0;
	engine_object_add(player_fire_create(obj, PLAYER_FIRE_T_NORMAL,
					     ga->player_pos,
					     pf_x, pf_y, pf_z, pf_a), 1);
      }

    }
  }


  // Rotate gamearea colours
  if(ga->move_colours) {
    ga->poly_colours_offset += diff / 200;
    if(ga->poly_colours_offset > ga->num_sections)
      ga->poly_colours_offset -= ga->num_sections;
  }

  // Fade colours
  ga->poly_colours_fade += ga->poly_colours_fade_dir * diff / 5000;
  if(ga->poly_colours_fade_dir < 0 && ga->poly_colours_fade < 0.5f) {
    ga->poly_colours_fade = 0.5f;
    ga->poly_colours_fade_dir = 1.0f;
  } else if(ga->poly_colours_fade_dir > 0 && ga->poly_colours_fade > 1.0f) {
    ga->poly_colours_fade = 1.0f;
    ga->poly_colours_fade_dir = -1.0f;
  }

  for(i = 0 ; i < ga->num_edges * 2 * 2 ; i++) {
    ga->poly_colours[i * 3 + 0] =
      ga->colour_buffer[i * 3 + 0] * ga->poly_colours_fade;
    ga->poly_colours[i * 3 + 1] =
      ga->colour_buffer[i * 3 + 1] * ga->poly_colours_fade;
    ga->poly_colours[i * 3 + 2] =
      ga->colour_buffer[i * 3 + 2] * ga->poly_colours_fade;
  }


  if(ga->exiting)
    ga->exit_wait += diff;

  if(ga->exiting && ga->player_jump_step == 0 && ga->exit_wait >= 1800) {

    // Stage finished, exiting

    if(!ga->exit_sound_played) {
      native_callback(CALLBACK_PLAY_SOUND, SOUND_ENDSTAGE, 0, 0);
      ga->exit_sound_played = 1;
    }

    // Fade area to only wireframe
    GLOBJ(GLOBJ_POLYS)->alpha -= diff / 1000;
    if(GLOBJ(GLOBJ_POLYS)->alpha < 0.0f)
      GLOBJ(GLOBJ_POLYS)->alpha = 0.0f;

    ga->exiting_speed += diff / 1000000;

    vz -= 10.0f * diff * ga->exiting_speed;
    if(vz < -200.0f)
      engine_stage_next();

    engine_viewpoint_set(vx, vy, vz);

    return 1;

  } else {

    // Handle jump sequence, if jumping and not exiting

    if(ga->player_jump_step == 0 && ga->player_jump && ga->player_jump_enabled)
      ga->player_jump_step++;

    if(ga->player_jump_step > 0) {
      ga->player_zpos = 3.0f * sin((double)ga->player_jump_step * 0.05);
      if(ga->player_zpos < 0.0f) {
	ga->player_zpos = 0.0f;
	ga->player_jump_step = 0;
      } else {
	ga->player_jump_step++;
      }
    } else {
      ga->player_zpos = 0.0f;
    }

  }


  // Calculate viewpoint based on player position
  GLfloat px = ga->player_positions[ga->player_pos * 2 + 0] * 0.6f;
  GLfloat py = ga->player_positions[ga->player_pos * 2 + 1] * 0.6f;
  GLfloat dx = px - vx;
  GLfloat dy = py - vy;

  vx += dx / 20;
  vy += dy / 20;
  vz = ga->player_zpos + gamearea_viewdist();

  engine_viewpoint_set(vx, vy, vz);

  return 1;
}

static void gamearea_draw(object_t *obj)
{
  gamearea_t *ga = (gamearea_t *)obj->object;
  float mainmtx[16];

  DEBUG(DEBUG_DRAW, " ");

  matrix_identity(mainmtx);

  if(ga->exiting || FLAG_TST(obj->flags, OBJECT_F_STAGE_INIT))
    glEnable(GL_BLEND);

  if(FLAG_TST(obj->flags, OBJECT_F_STAGE_INIT))
    matrix_translate(mainmtx, 0.0f, 0.0f, ga->init_tz);

  if(FLAG_TST(obj->flags, OBJECT_F_STAGE_RESTART)) {
    matrix_translate(mainmtx, 0.0f, 0.0f, -4.5f);
    matrix_rotate(mainmtx, MTX_AXIS_X, ga->init_rx + 180.0f);
    matrix_rotate(mainmtx, MTX_AXIS_Y, ga->init_ry + 180.0f);
    matrix_rotate(mainmtx, MTX_AXIS_Z, ga->init_rz + 180.0f);
    matrix_translate(mainmtx, 0.0f, 0.0f, 4.5f);
  }


  // Draw polygons
  if(!ga->transparent) {
    glPolygonOffset(screen_density, screen_density);
    glEnable(GL_POLYGON_OFFSET_FILL);

    matrix_copy(GLOBJ(GLOBJ_POLYS)->mtx_model, mainmtx);
    engine_globject_draw_prepare(GLOBJ(GLOBJ_POLYS));
    engine_globject_update_cols(GLOBJ(GLOBJ_POLYS), ga->num_vertices,
				ga->poly_colours +
				(int)ga->poly_colours_offset * 2 * 3);
    glDrawElements(GL_TRIANGLES, ga->num_poly_indices, GL_UNSIGNED_SHORT, 0);
    engine_globject_draw_cleanup(GLOBJ(GLOBJ_POLYS));
  }

  glDisable(GL_BLEND);


  // Draw lines
  glDisable(GL_POLYGON_OFFSET_FILL);
  if(screen_density > 1.5f) {
    glLineWidth(screen_density + 1.0f);
  }

  matrix_copy(GLOBJ(GLOBJ_LINES)->mtx_model, mainmtx);
  engine_globject_draw_prepare(GLOBJ(GLOBJ_LINES));
  engine_globject_update_cols(GLOBJ(GLOBJ_LINES), ga->num_vertices,
			      ga->line_colours);
  glDrawElements(GL_LINES, ga->num_line_indices, GL_UNSIGNED_SHORT, 0);
  engine_globject_draw_cleanup(GLOBJ(GLOBJ_LINES));

  glLineWidth(screen_density + 1.5f);
}

static void gamearea_reinit(object_t *obj, int doalloc)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  if(doalloc) {
    engine_globject_alloc(GLOBJ(GLOBJ_POLYS));
    engine_globject_alloc(GLOBJ(GLOBJ_LINES));
  }

  if(gfx_details >= GFX_DETAILS_HIGH) {
    int shader = use_multi_light ? SHADERS_LIGHT_MULTI : SHADERS_LIGHT1;

    engine_globject_init_ex(GLOBJ(GLOBJ_POLYS), shader,
			    ga->num_vertices, ga->vertices, 0,
			    ga->num_vertices, ga->vertex_normals, 0,
			    ga->num_vertices, ga->poly_colours, 0,
			    ga->num_poly_indices, ga->poly_indices, 0);

    engine_globject_init_ex(GLOBJ(GLOBJ_LINES), shader,
			    ga->num_vertices, ga->vertices, 0,
			    ga->num_vertices, ga->vertex_normals, 0,
			    ga->num_vertices, ga->line_colours, 1,
			    ga->num_line_indices, ga->line_indices, 0);
  } else {
    engine_globject_init(GLOBJ(GLOBJ_POLYS), SHADERS_GOURAUD,
			 ga->num_vertices, ga->vertices, 0,
			 ga->num_vertices, ga->poly_colours, 0,
			 ga->num_poly_indices, ga->poly_indices, 0);

    engine_globject_init(GLOBJ(GLOBJ_LINES), SHADERS_GOURAUD,
			 ga->num_vertices, ga->vertices, 0,
			 ga->num_vertices, ga->line_colours, 1,
			 ga->num_line_indices, ga->line_indices, 0);
  }
}

object_t *gamearea_create(stage_t *stage)
{
  object_t *obj;
  gamearea_t *ga;
  int i, j;

  DEBUG(DEBUG_CREATE, " ");

  ga = calloc(1, sizeof(*ga));

  obj = calloc(1, sizeof(*obj));
  obj->destroy_cb = gamearea_destroy;
  obj->update_cb = gamearea_update;
  obj->draw_cb = gamearea_draw;
  obj->reinit_cb = gamearea_reinit;
  obj->object = ga;

  ga->closed_area = stage->closed;
  ga->move_colours = stage->move_colours;
  ga->move_line_colours = stage->move_line_colours;
  ga->transparent = stage->transparent;
  ga->no_highlight = stage->no_highlight;
  ga->poly_colours_fade = 1.0f;
  ga->poly_colours_fade_dir = -1.0f;

  ga->player_float_pos = ga->player_pos = stage->start_position;
  ga->player_shoot_type = PLAYER_FIRE_T_NORMAL;
  ga->player_shoot = engine_is_autofire();

  ga->num_edges = stage->num_coords;
  ga->num_vertices = 2 * ga->num_edges;
  ga->vertices = malloc(3 * ga->num_vertices * sizeof(GLfloat));
  ga->poly_colours = malloc(2 * 3 * ga->num_vertices * sizeof(GLfloat));
  ga->colour_buffer = malloc(2 * 3 * ga->num_vertices * sizeof(GLfloat));
  ga->num_sections = ga->closed_area ? ga->num_edges : ga->num_edges - 1;
  ga->num_line_indices = 2 * (2 * ga->num_sections + ga->num_edges);
  ga->line_indices = malloc(ga->num_line_indices * sizeof(GLushort));
  ga->num_poly_indices = 3 * 2 * ga->num_sections;
  ga->poly_indices = malloc(ga->num_poly_indices * sizeof(GLushort));
  ga->player_positions = malloc(2 * ga->num_sections * sizeof(GLfloat));
  ga->player_angles = malloc(ga->num_sections * sizeof(GLfloat));
  ga->strip_normals = malloc(2 * ga->num_sections * sizeof(GLfloat));
  ga->vertex_normals = malloc(3 * ga->num_vertices * sizeof(GLfloat));
  ga->strip_lengths = malloc(ga->num_sections * sizeof(GLfloat));
  ga->edge_angles = malloc(ga->num_edges * sizeof(GLfloat));
  ga->edge_convex = malloc(ga->num_edges * sizeof(int));
  ga->num_line_colours = 2 * ga->num_edges;
  ga->line_colours_orig = malloc(3 * ga->num_line_colours * sizeof(GLfloat));
  ga->line_colours = malloc(3 * ga->num_line_colours * sizeof(GLfloat));

  for(i = 0 ; i < ga->num_edges ; i++) {
    ga->vertices[i * 6 + 0] = stage->coords[i * 2 + 0];
    ga->vertices[i * 6 + 1] = stage->coords[i * 2 + 1];
    ga->vertices[i * 6 + 2] = 0.000f;
    ga->vertices[i * 6 + 3] = stage->coords[i * 2 + 0];
    ga->vertices[i * 6 + 4] = stage->coords[i * 2 + 1];
    ga->vertices[i * 6 + 5] = -9.000f;
  }

  // Line indices for top and bottom edge lines.
  for(i = 0 ; i < ga->num_sections ; i++) {
    ga->line_indices[i * 4 + 0] = i * 2 + 0;      // top
    ga->line_indices[i * 4 + 2] = i * 2 + 1;      // bottom
    if(i == ga->num_sections - 1 && ga->closed_area) {
      ga->line_indices[i * 4 + 1] = 0;          // top
      ga->line_indices[i * 4 + 3] = 1;          // bottom
    } else {
      ga->line_indices[i * 4 + 1] = i * 2 + 2;  // top
      ga->line_indices[i * 4 + 3] = i * 2 + 3;  // bottom
    }
  }

  // Line indices for section lines.
  for(i = 0 ; i < ga->num_edges ; i++) {
    ga->line_indices[ga->num_sections * 2 * 2 + i * 2 + 0] = i * 2 + 0;
    ga->line_indices[ga->num_sections * 2 * 2 + i * 2 + 1] = i * 2 + 1;
  }

  // Polygon indices for sections.
  for(i = 0 ; i < ga->num_sections * 2 ; i += 2) {
    if(ga->closed_area) {
      ga->poly_indices[i * 3 + 0] = (short)((i + 0) % (ga->num_sections * 2));
      ga->poly_indices[i * 3 + 1] = (short)((i + 2) % (ga->num_sections * 2));
      ga->poly_indices[i * 3 + 2] = (short)((i + 1) % (ga->num_sections * 2));
      ga->poly_indices[i * 3 + 3] = (short)((i + 1) % (ga->num_sections * 2));
      ga->poly_indices[i * 3 + 4] = (short)((i + 2) % (ga->num_sections * 2));
      ga->poly_indices[i * 3 + 5] = (short)((i + 3) % (ga->num_sections * 2));
    } else {
      ga->poly_indices[i * 3 + 0] = (short)(i + 0);
      ga->poly_indices[i * 3 + 1] = (short)(i + 2);
      ga->poly_indices[i * 3 + 2] = (short)(i + 1);
      ga->poly_indices[i * 3 + 3] = (short)(i + 1);
      ga->poly_indices[i * 3 + 4] = (short)(i + 2);
      ga->poly_indices[i * 3 + 5] = (short)(i + 3);
    }
  }

  // Calculate player positions and angles on edge
  for(i = 0 ; i < ga->num_sections ; i++) {
    int ei1, ei2;
    ei1 = i;
    if(i == ga->num_sections - 1 && ga->closed_area)
      ei2 = 0;
    else
      ei2 = i + 1;

    double x1 = stage->coords[ei1 * 2 + 0];
    double y1 = stage->coords[ei1 * 2 + 1];
    double x2 = stage->coords[ei2 * 2 + 0];
    double y2 = stage->coords[ei2 * 2 + 1];

    ga->player_positions[i * 2 + 0] = (x1 + x2) / 2.0;
    ga->player_positions[i * 2 + 1] = (y1 + y2) / 2.0;

    double dx = x2 - x1;
    double dy = y2 - y1;
    double hyp = sqrt(dx * dx + dy * dy);

    // Normalized -1.0 -- 1.0
    double ax = dx / hyp;
    double ay = dy / hyp;

    // Calculate angle
    ga->player_angles[i] = (atan2(ay, ax) + M_PI) * 180.0 / M_PI;

    // Calculate strip normal
    ga->strip_normals[i * 2 + 0] = ay;
    ga->strip_normals[i * 2 + 1] = -ax;

    // Store strip lengths
    ga->strip_lengths[i] = hyp;
  }

  for(i = 0 ; i < ga->num_edges ; i++) {
    int s1, s2;
    double sa1, sa2;

    if(ga->closed_area) {
      s1 = (i == 0 ? ga->num_sections - 1 : i - 1);
      s2 = (i == ga->num_edges - 1 ? 0 : i);
    } else {
      s1 = (i == 0 ? 0 : i - 1);
      s2 = (i == ga->num_edges - 1 ? i - 1 : i);
    }

    sa1 = ga->player_angles[s1];
    sa2 = ga->player_angles[s2];

    if(fabs(sa1 - sa2) > 180.0) {
      if(sa1 > 180.0)
	sa1 -= 360.0;
      else
	sa2 -= 360.0;
    }

    ga->edge_angles[i] = (sa1 + sa2) / 2.0 + 360.0;
    if(ga->edge_angles[i] > 360.0)
      ga->edge_angles[i] -= 360.0;

    ga->edge_convex[i] = stage->convex[i];

    // Calculate vertex normal
    ga->vertex_normals[i * 6 + 0] = ga->vertex_normals[i * 6 + 3] =
      (ga->strip_normals[s1 * 2 + 0] + ga->strip_normals[s2 * 2 + 0]) / 2.0f;
    ga->vertex_normals[i * 6 + 1] = ga->vertex_normals[i * 6 + 4] =
      (ga->strip_normals[s1 * 2 + 1] + ga->strip_normals[s2 * 2 + 1]) / 2.0f;
    ga->vertex_normals[i * 6 + 2] = ga->vertex_normals[i * 6 + 5] = 0.0f;
  }

  // Calculate colours
  float dc[3];
  int cg, cgi;
  int half_num = (ga->num_edges + 1) / 2;
  for(cgi = 0 ; cgi < 2 ; cgi++) {
    cg = cgi + 1;
    if(ga->closed_area)
      cg %= 2;
    dc[0] = (stage->colours[cg * 3 + 0] - stage->colours[cgi * 3 + 0]) /
      half_num;
    dc[1] = (stage->colours[cg * 3 + 1] - stage->colours[cgi * 3 + 1]) /
      half_num;
    dc[2] = (stage->colours[cg * 3 + 2] - stage->colours[cgi * 3 + 2]) /
      half_num;
    for(i = 0 ; i < half_num ; i++) {
      int c = ((cgi * half_num) + i) * 3 * 2;
      ga->poly_colours[c + 0] =	stage->colours[cgi * 3 + 0] + dc[0] * i;
      ga->poly_colours[c + 1] = stage->colours[cgi * 3 + 1] + dc[1] * i;
      ga->poly_colours[c + 2] = stage->colours[cgi * 3 + 2] + dc[2] * i;
      ga->poly_colours[c + 3] = (stage->colours[cgi * 3 + 0] + dc[0] * i) * 0.6f;
      ga->poly_colours[c + 4] = (stage->colours[cgi * 3 + 1] + dc[1] * i) * 0.6f;
      ga->poly_colours[c + 5] = (stage->colours[cgi * 3 + 2] + dc[2] * i) * 0.6f;
    }
  }

  for(i = 0 ; i < ga->num_edges * 3 * 2 ; i++) {
    ga->poly_colours[i + ga->num_edges * 3 * 2] = ga->poly_colours[i];
    ga->colour_buffer[i] = ga->poly_colours[i];
    ga->colour_buffer[i + ga->num_edges * 3 * 2] = ga->poly_colours[i];
  }

  j = 0;
  for(i = 0 ; i < ga->num_edges ; i++) {
    ga->line_colours_orig[i * 3 * 2 + 0] = stage->line_colours[j * 6 + 0];
    ga->line_colours_orig[i * 3 * 2 + 1] = stage->line_colours[j * 6 + 1];
    ga->line_colours_orig[i * 3 * 2 + 2] = stage->line_colours[j * 6 + 2];
    ga->line_colours_orig[i * 3 * 2 + 3] = stage->line_colours[j * 6 + 3];
    ga->line_colours_orig[i * 3 * 2 + 4] = stage->line_colours[j * 6 + 4];
    ga->line_colours_orig[i * 3 * 2 + 5] = stage->line_colours[j * 6 + 5];
    if(stage->move_line_colours)
      j++;
  }


  obj->num_globjs = GLOBJ_NUM;
  obj->globjs = calloc(obj->num_globjs, sizeof(*obj->globjs));

  gamearea_reinit(obj, 1);

  // Reset for stage init
  GLOBJ(GLOBJ_POLYS)->alpha = 0.0f;
  GLOBJ(GLOBJ_LINES)->alpha = 1.0f;

  GLOBJ(GLOBJ_POLYS)->spec_colour[0] = 1.0f;
  GLOBJ(GLOBJ_POLYS)->spec_colour[1] = 1.0f;
  GLOBJ(GLOBJ_POLYS)->spec_colour[2] = 1.0f;
  GLOBJ(GLOBJ_POLYS)->spec_colour[3] = 1.0f;
  GLOBJ(GLOBJ_LINES)->spec_colour[0] = 0.3f;
  GLOBJ(GLOBJ_LINES)->spec_colour[1] = 0.3f;
  GLOBJ(GLOBJ_LINES)->spec_colour[2] = 0.3f;
  GLOBJ(GLOBJ_LINES)->spec_colour[3] = 1.0f;

  GLOBJ(GLOBJ_POLYS)->spec_shininess = 7.0f;
  GLOBJ(GLOBJ_LINES)->spec_shininess = 5.0f;

  GLOBJ(GLOBJ_POLYS)->light1 = &engine_light2;
  GLOBJ(GLOBJ_LINES)->light1 = &engine_light2;

  ga->init_fade = 0.0f;
  ga->init_tz = -20.0f;

  ga->init_rx = 0.0f;
  ga->init_ry = 0.0f;
  ga->init_rz = 0.0f;

  if(stage->reverse_direction)
    ga->player_move_dir = -1.0;
  else
    ga->player_move_dir = 1.0;

  return obj;
}

void gamearea_player_set_control(object_t *obj,
				 float speed, int step,
				 int shoot, int jump)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  ga->player_speed = speed;
  ga->player_shoot = shoot;
  ga->player_jump = jump;

  // If step is set, move it directly.
  if(step) {
    ga->player_float_pos += ga->player_move_dir * (float)step;

    if(ga->closed_area) {
      while(ga->player_float_pos >= ga->num_sections)
	ga->player_float_pos -= ga->num_sections;
      while(ga->player_float_pos < 0)
	ga->player_float_pos += ga->num_sections;
    } else {
      if(ga->player_float_pos >= ga->num_sections)
	ga->player_float_pos = ga->num_sections - 1;
      if(ga->player_float_pos < 0)
	ga->player_float_pos = 0;
    }

    ga->player_pos = (int)ga->player_float_pos;
  }
}

int gamearea_player_section_get(object_t *obj)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  return ga->player_pos;
}

GLfloat gamearea_player_zpos_get(object_t *obj)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  return ga->player_zpos;
}

void gamearea_player_zpos_set(object_t *obj, GLfloat tz)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  ga->player_zpos = tz;
}

GLfloat gamearea_section_angle_get(object_t *obj, int section)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  return ga->player_angles[section];
}

void gamearea_section_pos_get(object_t *obj,
			      int section, GLfloat *tx, GLfloat *ty)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  *tx = ga->player_positions[section * 2 + 0];
  *ty = ga->player_positions[section * 2 + 1];
}

GLfloat gamearea_section_len_get(object_t *obj, int section)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  return ga->strip_lengths[section];
}

int gamearea_num_sections_get(object_t *obj)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  return ga->num_sections;
}

int gamearea_closed_get(object_t *obj)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  return ga->closed_area;
}

int gamearea_section_offset_get(object_t *obj, int section, int offset)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  section += offset;
  if(!ga->closed_area && section < 0)
    section = 0;
  else if(!ga->closed_area && section >= ga->num_sections)
    section = ga->num_sections - 1;
  else if(ga->closed_area && section < 0)
    section += ga->num_sections;
  else if(ga->closed_area && section >= ga->num_sections)
    section -= ga->num_sections;

  return section;
}


int gamearea_num_edges_get(object_t *obj)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  return ga->num_edges;
}

int gamearea_edge_offset_get(object_t *obj, int edge, int offset)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  edge += offset;
  if(!ga->closed_area && edge < 0)
    edge = 0;
  else if(!ga->closed_area && edge >= ga->num_edges)
    edge = ga->num_edges - 1;
  else if(ga->closed_area && edge < 0)
    edge += ga->num_edges;
  else if(ga->closed_area && edge >= ga->num_edges)
    edge -= ga->num_edges;

  return edge;
}

void gamearea_edge_pos_get(object_t *obj,
			   int edge, GLfloat *tx, GLfloat *ty)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  *tx = ga->vertices[edge * 6 + 0];
  *ty = ga->vertices[edge * 6 + 1];
}

GLfloat gamearea_edge_angle_get(object_t *obj, int edge)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  return ga->edge_angles[edge];
}

int gamearea_section_by_edge(object_t *obj, int edge)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  if(ga->closed_area) {
    return edge;
  } else {
    return edge == ga->num_edges - 1 ? edge - 1 : edge;
  }
}

GLfloat gamearea_sections_angle_diff(object_t *obj, int fromsection, int rdir)
{
  gamearea_t *ga = (gamearea_t *)obj->object;
  int tosection = gamearea_section_offset_get(obj, fromsection, rdir);
  GLfloat fromangle = ga->player_angles[fromsection];
  GLfloat toangle = ga->player_angles[tosection];
  GLfloat diff;
  int edge;

  if(rdir > 0)
    edge = tosection;
  else
    edge = fromsection;

  diff = fabs(toangle - fromangle);

  if(diff > 180.0)
    diff = 360.0 - diff;

  if(ga->edge_convex[edge])
    diff += 180.0;
  else
    diff = 180.0 - diff;

  return diff;
}

void gamearea_exit(object_t *obj)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  ga->exiting = 1;
}

GLfloat gamearea_exiting_speed(object_t *obj)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  return ga->exiting_speed;
}

int gamearea_shoot_type_get(object_t *obj)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  return ga->player_shoot_type;
}

void gamearea_shoot_type_set(object_t *obj, int type)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  ga->player_shoot_type = type;
}

void gamearea_jump_enable(object_t *obj)
{
  gamearea_t *ga = (gamearea_t *)obj->object;

  ga->player_jump_enabled = 1;
}

GLfloat gamearea_viewdist(void)
{
  if(native_is_vr())
    return GAMEAREA_VIEWDIST_VR;
  else
    return GAMEAREA_VIEWDIST_NORMAL;
}
