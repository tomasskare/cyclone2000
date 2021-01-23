#ifndef _MENU_H_
#define _MENU_H_

#include "object.h"

void menu_init(int last_stage);
void menu_reinit(int doalloc);
int menu_update(uint64_t msec);
void menu_draw(int viewmode);
void menu_choice(int choice, int data2, int data3);

#endif
