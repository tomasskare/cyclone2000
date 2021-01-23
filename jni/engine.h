#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "object.h"

void engine_init(int control_type, int autofire, int ctrlsize, int buttonsize);
void engine_reinit(int doalloc);
int engine_start(void);
int engine_update(uint64_t msec);
void engine_update_paused(uint64_t msec);
void engine_draw(int viewmode);
void engine_player_set_control(float speed, int step,
			       int shoot, int jump, int zap);
void engine_pause(void);
int engine_resume(int paused);
void engine_control_type_set(int type, int autofire,
			     int ctrlsize, int buttonsize);
void engine_state_set(int last_stage, int last_start_stage,
		      int last_life, int last_score,
		      int num_pups, int pups_disabled);
void engine_pup_set(int item, int value, int store);
int engine_pup_get(int item);
void engine_pup_disable_set(int disable);
void engine_pup_reset(void);
int engine_state_stage_get(void);
int engine_stage_num_get(void);
int engine_numpups_get(void);
void engine_numpups_set(int num);
void engine_numpups_increase(int num);
void engine_numpups_decrease(int num);
void engine_object_add(object_t *obj, int listnum);
void engine_object_remove(object_t *obj);
void engine_object_onedge(object_t *obj);
int engine_object_get_num_filtered(int filterin, int filterout);
object_t *engine_object_first_foe_get(int filterout);
object_t *engine_spikes_get_or_create(int section, int create);
void engine_spikes_free(int section);
int engine_collision_detect(object_t *srcobj,
			    uint32_t maskflag, int section,
			    double z1, double z2);
void engine_kill_foe(object_t *obj);
void engine_fade_out(int fadetime);
void engine_stage_next(void);
void engine_viewpoint_set(GLfloat vx, GLfloat vy, GLfloat vz);
void engine_viewpoint_get(GLfloat *vx, GLfloat *vy, GLfloat *vz);
void engine_banner_p2k(void);
void engine_banner_1up(void);
int engine_should_pause(void);
int engine_is_autofire(void);

void engine_callback_state_save(int set);

void engine_globject_alloc(globject_t *globj);
void engine_globject_init_ex(globject_t *globj, int shader_type,
			     int numverts, GLfloat *verts, int vdyn,
			     int numnorms, GLfloat *normals, int ndyn,
			     int numcols, GLfloat *cols, int cdyn,
			     int numidxs, GLushort *idxs, int idyn);
#define engine_globject_init(gl,st,nv,v,vd,nc,c,cd,ni,i,id) \
  engine_globject_init_ex(gl,st,nv,v,vd,0,NULL,0,nc,c,cd,ni,i,id)
#define engine_globject_initf(gl,st,nf,v,vd,n,nd,c,cd,i,id)		\
  engine_globject_init_ex(gl,st,(nf*3),v,vd,(nf*3),n,nd,(nf*3),c,cd,(nf*3),i,id)
void engine_globject_free(globject_t *globj);
void engine_globject_draw_prepare(globject_t *globj);
void engine_globject_draw_prepare_mtx(globject_t *globj,
				      float *my_mtx_view, float *my_mtx_proj);
void engine_globject_draw_cleanup(globject_t *globj);
void engine_globject_update_modelmtx(globject_t *globj);
void engine_globject_update_verts(globject_t *globj,
				  int numverts, GLfloat *verts);
void engine_globject_update_normals(globject_t *globj,
				    int numnormals, GLfloat *normals);
void engine_globject_update_cols(globject_t *globj,
				 int numcols, GLfloat *cols);
void engine_globject_set_colour(globject_t *globj,
				GLfloat r, GLfloat g, GLfloat b);
void engine_globject_set_alpha(globject_t *globj, GLfloat a);
void engine_globject_set_pointsize(globject_t *globj, GLfloat ps);

void engine_gldirect_draw_texbox(int shader_type, GLfloat alpha,
				 int numvertices, GLfloat *vertices,
				 GLfloat *texcoords,
				 float *mtx_model,
				 float *mtx_view,
				 float *mtx_proj);
void engine_gldirect_draw_colbox(int shader_type,
				 int numvertices, GLfloat *vertices,
				 GLfloat r, GLfloat g, GLfloat b, GLfloat a,
				 float *mtx_model,
				 float *mtx_view,
				 float *mtx_proj);
void engine_gldirect_draw_points(int shader_type, GLfloat pointsize,
				 int numpoints, GLfloat *points,
				 GLfloat r, GLfloat g, GLfloat b, GLfloat a,
				 float *mtx_model,
				 float *mtx_view,
				 float *mtx_proj);

gllight_t *engine_gllight_point_create(GLfloat dr, GLfloat dg, GLfloat db,
				       GLfloat sr, GLfloat sg, GLfloat sb);
void engine_gllight_destroy(gllight_t *light);
void engine_gllight_point_update(gllight_t *light,
				 GLfloat intensity, float *modelmtx);

extern gllight_t engine_light1;
extern gllight_t engine_light2;

#endif
