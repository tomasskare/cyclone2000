#ifndef _SHADERS_H_
#define _SHADERS_H_

#define SHADERS_GOURAUD        0
#define SHADERS_LINE           1
#define SHADERS_POINT          2
#define SHADERS_TEXTURE        3
#define SHADERS_TEXTURE_DIST   4
#define SHADERS_LIGHT1         5
#define SHADERS_LIGHT_MULTI    6
#define SHADERS_POINT_ROUND    7
#define SHADERS_NUM            8

GLuint shaders_get_program(int type);

void shaders_init(void);

#endif /* _SHADERS_H_ */
