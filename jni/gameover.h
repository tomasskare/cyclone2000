#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_

#include "object.h"

void gameover_init(int died);
int gameover_update(uint64_t msec);
void gameover_draw(int viewmode);

#endif
