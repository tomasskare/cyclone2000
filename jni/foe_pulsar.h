#ifndef _FOE_PULSAR_H_
#define _FOE_PULSAR_H_

#include "object.h"

object_t *foe_pulsar_create(object_t *ga_obj, int section, GLfloat zpos,
			    int movedir, int flipdir);
void foe_pulsar_global_update(uint64_t prev_msec, uint64_t msec);

#endif
