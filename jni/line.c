#include <android/log.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <sys/queue.h>

#include <GLES2/gl2.h>

#include "debug.h"
#include "line.h"
#include "engine.h"
#include "matrix.h"
#include "shaders.h"
#include "char_coords.h"

line_t *line_build(char *string)
{
  int len = strlen(string);
  line_t *line;
  int i;

  line = malloc(sizeof(*line) + sizeof(char_t) * len);
  line->len = len;
  line->width = 0.07;

  for(i = 0 ; i < len ; i++) {
    line->chars[i].idx = string[i];
    line->chars[i].x = line->width;
    line->chars[i].y = 0.0;
    line->width += chars[(int)string[i]].w;
  }

  return line;
}

void line_draw(line_t *line, GLfloat tx, GLfloat ty,
	       GLfloat sx, GLfloat sy, GLfloat alpha,
	       float *mtx_view, float *mtx_proj)
{
  int i;
  float mtx_model1[16];

  matrix_set_translate(mtx_model1, tx, ty, 0.0f);
  matrix_scale(mtx_model1, sx, sy, 1.0f);

  for(i = 0 ; i < line->len ; i++) {
    char_t *ch = &line->chars[i];
    char_coords_t *cc = &chars[ch->idx];

    GLfloat tex_coords[] = { cc->tx2, cc->ty1,
			     cc->tx1, cc->ty1,
			     cc->tx2, cc->ty2,
			     cc->tx1, cc->ty2 };

    GLfloat vertices[] = {  cc->w / 2, -cc->h / 2, 0.0,
			   -cc->w / 2, -cc->h / 2, 0.0,
			    cc->w / 2,  cc->h / 2, 0.0,
			   -cc->w / 2,  cc->h / 2, 0.0 };
    float mtx_model2[16];

    matrix_translate_to(mtx_model2, mtx_model1, ch->x + cc->w / 2, 0.0, 0.0);

    engine_gldirect_draw_texbox(SHADERS_TEXTURE, alpha,
				4, vertices, tex_coords,
				mtx_model2, mtx_view, mtx_proj);
  }
}
