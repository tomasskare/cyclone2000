#ifndef _GAMEAREA_H_
#define _GAMEAREA_H_

#include "stage.h"
#include "object.h"

object_t *gamearea_create(stage_t *stage);
void gamearea_player_set_control(object_t *obj,
				 float speed, int step,
				 int shoot, int jump);
int gamearea_player_section_get(object_t *obj);
GLfloat gamearea_player_zpos_get(object_t *obj);
void gamearea_player_zpos_set(object_t *obj, GLfloat tz);
void gamearea_section_pos_get(object_t *obj,
			      int section, GLfloat *tx, GLfloat *ty);
GLfloat gamearea_section_len_get(object_t *obj, int section);
GLfloat gamearea_section_angle_get(object_t *obj, int section);
int gamearea_num_sections_get(object_t *obj);
int gamearea_closed_get(object_t *obj);
int gamearea_section_offset_get(object_t *obj, int section, int offset);
int gamearea_num_edges_get(object_t *obj);
int gamearea_edge_offset_get(object_t *obj, int edge, int offset);
void gamearea_edge_pos_get(object_t *obj,
			   int edge, GLfloat *tx, GLfloat *ty);
GLfloat gamearea_edge_angle_get(object_t *obj, int section);
int gamearea_section_by_edge(object_t *obj, int edge);
GLfloat gamearea_sections_angle_diff(object_t *obj, int fromsection, int rdir);
void gamearea_exit(object_t *obj);
GLfloat gamearea_exiting_speed(object_t *obj);
int gamearea_shoot_type_get(object_t *obj);
void gamearea_shoot_type_set(object_t *obj, int type);
void gamearea_jump_enable(object_t *obj);
GLfloat gamearea_viewdist(void);

#define GAMEAREA_VIEWDIST_NORMAL 3.2f
#define GAMEAREA_VIEWDIST_VR (3.2f+VR_Z_OFFSET)
#define GAMEAREA_PLAYER_EDGE_ZPOS 0.1f
#define GAMEAREA_FOE_EDGE_ZPOS 0.05f
#define GAMEAREA_DROID_EDGE_ZPOS 0.8f

#endif
