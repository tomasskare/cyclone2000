#ifndef _PLAYER_FIRE_H_
#define _PLAYER_FIRE_H_

#include "object.h"

#define PLAYER_FIRE_T_NORMAL        1
#define PLAYER_FIRE_T_LASER         2
#define PLAYER_FIRE_T_DROID_NORMAL  3
#define PLAYER_FIRE_T_DROID_LASER   4
#define PLAYER_FIRE_T_DROID_OFFSET  2

object_t *player_fire_create(object_t *ga_obj, int type, int section,
			     GLfloat x, GLfloat y, GLfloat z, GLfloat angle);

#endif
