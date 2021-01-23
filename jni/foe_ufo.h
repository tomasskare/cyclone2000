#ifndef _FOE_UFO_H_
#define _FOE_UFO_H_

#include "object.h"

#define FOE_UFO_OFFSET 1.6f

#define FOE_UFO_FIRE_LEN 1200
#define FOE_UFO_MOVE_LEN 1200

object_t *foe_ufo_create(object_t *ga_obj, int section);

#endif
