#ifndef _LINE_H_
#define _LINE_H_

typedef struct char_s {
  int idx;
  GLfloat x, y;
} char_t;

typedef struct line_s {
  int len;
  GLfloat width;
  char_t chars[0];
} line_t;

line_t *line_build(char *string);
void line_draw(line_t *line, GLfloat tx, GLfloat ty,
	       GLfloat sx, GLfloat sy, GLfloat alpha,
	       float *mtx_view, float *mtx_proj);

#endif /* _LINE_H_ */
