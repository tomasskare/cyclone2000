#ifndef _SCORE_H_
#define _SCORE_H_

#include "object.h"

object_t *score_create(void);
void score_increase(int num);
void score_increase_nolife(int add);
int score_current(void);
void score_set(int num);

#endif
