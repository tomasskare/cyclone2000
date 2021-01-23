#ifndef _FOE_DEMONHEAD_H_
#define _FOE_DEMONHEAD_H_

#include "object.h"

#define DEMONHEAD_MODE_FULL  1
#define DEMONHEAD_MODE_LEFT  2
#define DEMONHEAD_MODE_RIGHT 3

object_t *foe_demonhead_create(object_t *ga_obj, int section,
			       GLfloat zpos, int mode);

#endif
