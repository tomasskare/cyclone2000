#ifndef _FOE_SPIKES_H_
#define _FOE_SPIKES_H_

#include "object.h"

object_t *foe_spikes_create(object_t *ga_obj, int section);
void foe_spikes_grow(object_t *obj, GLfloat length);
GLfloat foe_spikes_length(object_t *obj);

#endif
