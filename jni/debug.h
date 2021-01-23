#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>

extern uint32_t debug;

#define DEBUG_CREATE          0x0001
#define DEBUG_UPDATE          0x0002
#define DEBUG_DRAW            0x0004
#define DEBUG_FPS             0x0008
#define DEBUG_NATIVE          0x0010
#define DEBUG_MENU            0x0020
#define DEBUG_INIT            0x0040
#define DEBUG_GAMEOVER        0x0080
#define DEBUG_HIGHSCORE       0x0100
#define DEBUG_TEMP            0x1000
#define DEBUG_ALL             0xffff

#define DEBUG(f,fmt...) do {					\
  if(FLAG_TST(debug, f)) {					\
    char __str[1024];						\
    snprintf(__str, 1024, fmt);					\
    __android_log_print(ANDROID_LOG_INFO, "Cyclone2000",	\
			"%s:%d: %s",				\
			__FUNCTION__, __LINE__, __str);		\
  }								\
} while(0)

#endif /* _DEBUG_H_ */
