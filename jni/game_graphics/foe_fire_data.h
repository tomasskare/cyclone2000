#ifndef _FOE_FIRE_DATA_H_
#define _FOE_FIRE_DATA_H_

#define SCALEX 0.03f
#define SCALEY 0.03f
#define SCALEZ 0.02f

static GLfloat low_vertices[] = {
  -1.000f * SCALEX, 0.978f * SCALEY, 0.000f * SCALEZ,
  0.000f * SCALEX, -1.000f * SCALEY, 0.000f * SCALEZ,
  0.628f * SCALEX, 0.761f * SCALEY, 0.000f * SCALEZ,
  -0.036f * SCALEX, 0.423f * SCALEY, 1.000f * SCALEZ,
};
static const int low_num_vertices = 4;

static GLushort low_indices[] = {
  0, 2, 3,
  1, 3, 2,
  0, 3, 1,
};
static const int low_num_indices = 3 * 3;

static GLfloat low_colours[] = {
  0.72f, 0.72f, 0.98f, // short tip
  0.70f, 0.70f, 1.00f, // long tip
  0.75f, 0.75f, 0.95f, // short tip
  0.90f, 0.90f, 0.90f, // top
};


static GLfloat high_vertices[] = {
    0.674f * SCALEX, 0.761f * SCALEY, 0.000f * SCALEZ,
    0.028f * SCALEX, -1.000f * SCALEY, 0.000f * SCALEZ,
    -1.000f * SCALEX, 0.978f * SCALEY, 0.000f * SCALEZ,
    -0.041f * SCALEX, 0.423f * SCALEY, 1.000f * SCALEZ,
};
static const int high_num_vertices = 4;

static GLfloat high_normals[] = {
  0.6751f, 0.4897f, -0.5518f,
  0.0818f, -0.7383f, -0.6695f,
  -0.5406f, 0.5285f, -0.6546f,
  0.0693f, 0.1022f, 0.9923f,
};

static GLfloat high_colours[] = {
  0.90f, 0.90f, 1.00f,
  0.90f, 0.90f, 1.00f,
  0.90f, 0.90f, 1.00f,
  0.90f, 0.90f, 1.00f,
};

static GLushort high_indices[] = {
    0, 2, 1,
    3, 2, 0,
    1, 3, 0,
    2, 3, 1,
    0, 2, 1,
};
static const int high_num_indices = 5 * 3;


#endif /* _FOE_FIRE_DATA_H_ */
