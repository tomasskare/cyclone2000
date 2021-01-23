#ifndef _LIFE_H_
#define _LIFE_H_

#include "object.h"

object_t *life_create(int init_life);
void life_increase(void);
void life_decrease(void);
int life_current(void);
void life_set(int lifes);
int life_lost(void);

#endif
