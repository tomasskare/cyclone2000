#ifndef _ORTHOQUAD_H_
#define _ORTHOQUAD_H_

#include "object.h"

object_t *orthoquad_create(int texture_id, float x, float y, float w, float h,
			   float tx, float ty, float tw, float th, int fade,
			   float cr, float cg, float cb);

#endif
