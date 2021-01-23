#ifndef _AI_DROID_DATA_H_
#define _AI_DROID_DATA_H_

#define SCALEX 0.10f
#define SCALEY 0.10f
#define SCALEZ 0.10f

static GLfloat vertices[] = {
  1.0f * SCALEX,  1.0f * SCALEY,  1.0f * SCALEZ,
  1.0f * SCALEX,  1.0f * SCALEY, -1.0f * SCALEZ,
  1.0f * SCALEX, -1.0f * SCALEY,  1.0f * SCALEZ,
  1.0f * SCALEX, -1.0f * SCALEY, -1.0f * SCALEZ,
 -1.0f * SCALEX,  1.0f * SCALEY,  1.0f * SCALEZ,
 -1.0f * SCALEX,  1.0f * SCALEY, -1.0f * SCALEZ,
 -1.0f * SCALEX, -1.0f * SCALEY,  1.0f * SCALEZ,
 -1.0f * SCALEX, -1.0f * SCALEY, -1.0f * SCALEZ,
};
static int num_vertices = 8;

static GLfloat normals[] = {
  0.5774f, 0.5774f, 0.5774f,
  0.5774f, 0.5774f, -0.5774f,
  0.5774f, -0.5774f, 0.5774f,
  0.5774f, -0.5774f, -0.5774f,
  -0.5774f, 0.5774f, 0.5774f,
  -0.5774f, 0.5774f, -0.5774f,
  -0.5774f, -0.5774f, 0.5774f,
  -0.5774f, -0.5774f, -0.5774f,
};

static GLushort line_indices[] = {
  0, 2,
  2, 6,
  6, 4,
  4, 0,
  1, 3,
  3, 7,
  7, 5,
  5, 1,
  0, 1,
  2, 3,
  4, 5,
  6, 7,
};
static int num_line_indices = 12 * 2;

/* Colours to fade between. */
static GLfloat fade_colours[] = {
  0.20f, 0.20f, 0.70f,
  0.60f, 0.10f, 0.70f,
  0.70f, 0.60f, 0.10f,
  0.70f, 0.70f, 0.20f,
  0.20f, 0.70f, 0.20f,
  0.20f, 0.70f, 0.70f,
  0.20f, 0.20f, 0.70f,
};
static int num_fade_colours = 6;

#endif /* _AI_DROID_DATA_H_ */
