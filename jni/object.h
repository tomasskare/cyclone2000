#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <sys/queue.h>

struct object_s;

typedef void object_destroy_f(struct object_s *obj);
typedef int object_update_f(struct object_s *obj,
			    uint64_t prev_msec, uint64_t msec);
typedef void object_draw_f(struct object_s *obj);
typedef int object_collision_f(struct object_s *obj, struct object_s *srcobj,
			       int section, double minz, double maxz);
typedef int object_hit_f(struct object_s *obj, struct object_s *srcobj);
typedef void object_posget_f(struct object_s *obj,
			     int *section, double *zpos);
typedef void object_reinit_f(struct object_s *obj, int doalloc);

#define GLOBJECT_VBO_VERTICES           0
#define GLOBJECT_VBO_NORMALS            1
#define GLOBJECT_VBO_COLOURS            2
#define GLOBJECT_VBO_INDICES            3
#define GLOBJECT_VBO_NUMS               4

#define GLOBJECT_VAR_A_POSITION         0
#define GLOBJECT_VAR_A_NORMAL           1
#define GLOBJECT_VAR_A_COLOUR           2
#define GLOBJECT_VAR_A_TEXCOORD         3
#define GLOBJECT_VAR_U_MODELMTX         4
#define GLOBJECT_VAR_U_VIEWMTX          5
#define GLOBJECT_VAR_U_PROJMTX          6
#define GLOBJECT_VAR_U_NORMALMTX        7
#define GLOBJECT_VAR_U_ALPHA            8
#define GLOBJECT_VAR_U_COLOUR           9
#define GLOBJECT_VAR_U_SPECCOLOUR       10
#define GLOBJECT_VAR_U_SPECSHININESS    11
#define GLOBJECT_VAR_U_POINTSIZE        12
#define GLOBJECT_VAR_U_TEXTURE          13
#define GLOBJECT_VAR_U_L1_DIRECTION     14
#define GLOBJECT_VAR_U_L1_HALFPLANE     15
#define GLOBJECT_VAR_U_L1_AMBIENT       16
#define GLOBJECT_VAR_U_L1_DIFFUSE       17
#define GLOBJECT_VAR_U_L1_SPECULAR      18
#define GLOBJECT_VAR_U_LP_MTX           19
#define GLOBJECT_VAR_U_LP_INTENSITY     20
#define GLOBJECT_VAR_U_LP_DIFFUSE       21
#define GLOBJECT_VAR_U_LP_SPECULAR      22
#define GLOBJECT_VAR_NUMS               23

typedef struct gllight_s {
  int type;
#define GLLIGHT_T_UNUSED      0
#define GLLIGHT_T_DIRECTIONAL 1
#define GLLIGHT_T_POINT       2
  union {
    struct {
      float direction[3];
      float halfplane[3];
    } dir;
    struct {
      float intensity;
      float modelmtx[16];
    } point;
  } u;
  float ambient[4];
  float diffuse[4];
  float specular[4];
#define dir_direction u.dir.direction
#define dir_halfplane u.dir.halfplane
#define point_intensity u.point.intensity
#define point_modelmtx u.point.modelmtx
} gllight_t;

typedef struct globject_s {
  int shader_type;
  GLuint program;
  GLuint vbo[GLOBJECT_VBO_NUMS];
  GLuint var[GLOBJECT_VAR_NUMS];

  float mtx_model[16];
  float alpha;

  float spec_colour[4];
  float spec_shininess;
  gllight_t *light1;
} globject_t;

typedef struct object_s {
  void *object;

  object_destroy_f *destroy_cb;
  object_update_f *update_cb;
  object_draw_f *draw_cb;
  object_collision_f *collision_cb;
  object_hit_f *hit_cb;
  object_posget_f *posget_cb;
  object_reinit_f *reinit_cb;

  uint32_t flags;
#define OBJECT_F_DESTROYED     0x0001  // Object will be destroyed
#define OBJECT_F_FOE           0x0002  // Object is a foe or foe fire
#define OBJECT_F_FOE_FIRE      0x0004  // Object is foe fire
#define OBJECT_F_PLAYER        0x0008  // Object is player or player fire
#define OBJECT_F_NOKILL        0x0010  // Object can't be killed
#define OBJECT_F_PLAYER_CAUGHT 0x0020  // Object is player or foe when caught
#define OBJECT_F_STAGE_INIT    0x0040  // Stage is in init state
#define OBJECT_F_POWERUP       0x0080  // Object is a powerup
#define OBJECT_F_STAGE_RESTART 0x0100  // Same stage restarted
#define OBJECT_F_DROID_FIRE    0x0200  // Object is AI droid or superzap fire
#define OBJECT_F_FOE_EDGE      0x0400  // Object is foe on the edge
#define OBJECT_F_FOE_SPIKER    0x0800  // Object is a spiker
#define OBJECT_F_FOE_SPIKES    0x1000  // Object is spikes
#define OBJECT_F_FOE_FRIER     0x2000  // Object will fry player
#define OBJECT_F_FORCE_KILL    0x4000  // Object is forced killed
#define OBJECT_F_PFIRE_LASER   0x8000  // Object is laser player/droid fire
#define OBJECT_F_NOZAP        0x10000  // Object is not affected by zapper

  TAILQ_ENTRY(object_s) node;

  int listnum;

  int num_globjs;
  globject_t *globjs;
} object_t;

#define GLOBJ(a) (&obj->globjs[a])

#define FLAG_SET(f,v) ((f)|=(v))
#define FLAG_CLR(f,v) ((f)&=~(v))
#define FLAG_TST(f,v) ((f)&(v))

#define IFFREE(a) do {				\
  void *_a = (void *)(a);			\
  if(_a)					\
    free(_a);					\
  a = NULL;					\
} while(0)

#endif
