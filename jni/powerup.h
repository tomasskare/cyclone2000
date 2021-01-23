#ifndef _POWERUP_H_
#define _POWERUP_H_

#include "object.h"


static char *powerup_texts[] __attribute__((unused)) = {
  "PARTICLE LASER!",
  "ZAPPO!",
  "JUMP ENABLED!",
  "OUTSTANDING!",
  "A.I. DROID!",
  "EXCELLENT!",
  "GREAT!",
  "ZAPPO!",
  "OUTSTANDING!",
  "EXCELLENT!",
  "ZAPPO!",
  "OUTSTANDING!",
  "EXCELLENT!",
  "ZAPPO!",
  "OUTSTANDING!",
  "EXCELLENT!",
  "ZAPPO!",
  "OUTSTANDING!",
  "EXCELLENT!",
  "ZAPPO!",
  "OUTSTANDING!",
  "EXCELLENT!",
  "ZAPPO!",
  "OUTSTANDING!",
  "EXCELLENT!",
  "ZAPPO!",
  "OUTSTANDING!",
  "EXCELLENT!",
  "ZAPPO!",
  "OUTSTANDING!",
  "EXCELLENT!",
  "ZAPPO!",
  "OUTSTANDING!",
  "EXCELLENT!",
};

object_t *powerup_create(object_t *ga_obj, int section, GLfloat z);
void powerup_taken(object_t *obj);

#endif
