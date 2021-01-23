#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "object.h"

object_t *control_create(int type, int autofire, int ctrlsize, int buttonsize);
void control_type_set(object_t *obj, int type, int autofire,
		      int ctrlsize, int buttonsize);
void control_enable_set(object_t *obj, int jump_enable, int zap_enable);
void control_enable_exit_set(object_t *obj, int exit_enable, int clear_alpha);

#endif
