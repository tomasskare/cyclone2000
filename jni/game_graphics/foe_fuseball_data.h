#ifndef _FOE_FUSEBALL_DATA_H_
#define _FOE_FUSEBALL_DATA_H_

#define SCALEX 0.075f
#define SCALEY 0.300f
#define SCALEZ 0.020f


static GLfloat low_l_vertices[] = {
    -1.000f * SCALEX, 0.500f * SCALEY, 0.000f * SCALEZ,
    -0.300f * SCALEX, 0.500f * SCALEY, 0.000f * SCALEZ,
    -0.000f * SCALEX, 1.000f * SCALEY, 0.000f * SCALEZ,
    -0.000f * SCALEX, 0.000f * SCALEY, 0.000f * SCALEZ,
};
static GLfloat low_r_vertices[] = {
    1.000f * SCALEX, 0.500f * SCALEY, 0.000f * SCALEZ,
    0.300f * SCALEX, 0.500f * SCALEY, 0.000f * SCALEZ,
    0.000f * SCALEX, 1.000f * SCALEY, 0.000f * SCALEZ,
    0.000f * SCALEX, 0.000f * SCALEY, 0.000f * SCALEZ,
};

static const int low_num_vertices = 4;

static GLushort low_l_indices[] = {
    0, 2, 1,
    3, 0, 1,
};
static GLushort low_r_indices[] = {
    0, 1, 2,
    3, 1, 0,
};
static const int low_num_indices = 2 * 3;


// clockwise: yellow, cyan, red, green, magenta

static GLfloat low_colours_yellow[] = {
  0.60f, 0.60f, 0.00f,
  0.55f, 0.55f, 0.00f,
  0.30f, 0.30f, 0.00f,
  0.90f, 0.90f, 0.00f,
};

static GLfloat low_colours_cyan[] = {
  0.00f, 0.60f, 0.60f,
  0.00f, 0.55f, 0.55f,
  0.00f, 0.30f, 0.30f,
  0.00f, 0.90f, 0.90f,
};

static GLfloat low_colours_red[] = {
  0.60f, 0.00f, 0.00f,
  0.55f, 0.00f, 0.00f,
  0.30f, 0.00f, 0.00f,
  0.90f, 0.00f, 0.00f,
};

static GLfloat low_colours_green[] = {
  0.00f, 0.60f, 0.00f,
  0.00f, 0.55f, 0.00f,
  0.00f, 0.30f, 0.00f,
  0.00f, 0.90f, 0.00f,
};

static GLfloat low_colours_magenta[] = {
  0.60f, 0.00f, 0.60f,
  0.55f, 0.00f, 0.55f,
  0.30f, 0.00f, 0.30f,
  0.90f, 0.00f, 0.90f,
};

static GLfloat *low_colours[] = {
  low_colours_yellow,
  low_colours_cyan,
  low_colours_red,
  low_colours_green,
  low_colours_magenta,
};


static GLfloat high_l_vertices[] = {
    -0.556f * SCALEX, 0.506f * SCALEY, 0.000f * SCALEZ,
    -0.032f * SCALEX, 0.004f * SCALEY, -0.487f * SCALEZ,
    -0.283f * SCALEX, 0.506f * SCALEY, -0.487f * SCALEZ,
    -1.000f * SCALEX, 0.504f * SCALEY, -0.487f * SCALEZ,
    -0.040f * SCALEX, 1.000f * SCALEY, -0.487f * SCALEZ,
    -0.630f * SCALEX, 0.501f * SCALEY, -1.000f * SCALEZ,
};
static GLfloat high_r_vertices[] = {
    0.556f * SCALEX, 0.506f * SCALEY, 0.000f * SCALEZ,
    0.032f * SCALEX, 0.004f * SCALEY, -0.487f * SCALEZ,
    0.283f * SCALEX, 0.506f * SCALEY, -0.487f * SCALEZ,
    1.000f * SCALEX, 0.504f * SCALEY, -0.487f * SCALEZ,
    0.040f * SCALEX, 1.000f * SCALEY, -0.487f * SCALEZ,
    0.630f * SCALEX, 0.501f * SCALEY, -1.000f * SCALEZ,
};

static const int high_num_vertices = 6;

static GLfloat high_l_normals[] = {
  0.4117f, 0.0041f, 0.9113f,
  0.6241f, -0.7813f, 0.0061f,
  0.9990f, 0.0030f, -0.0443f,
  -0.9965f, -0.0004f, 0.0832f,
  0.6241f, 0.7814f, -0.0021f,
  0.3689f, 0.0007f, -0.9295f,
};

static GLfloat high_r_normals[] = {
  -0.4117f, 0.0041f, 0.9113f,
  -0.6241f, -0.7813f, 0.0061f,
  -0.9990f, 0.0030f, -0.0443f,
  0.9965f, -0.0004f, 0.0832f,
  -0.6241f, 0.7814f, -0.0021f,
  -0.3689f, 0.0007f, -0.9295f,
};


static GLushort high_l_indices[] = {
    0, 2, 1,
    3, 0, 1,
    2, 0, 4,
    0, 3, 4,
    5, 3, 1,
    2, 5, 1,
    5, 2, 4,
    3, 5, 4,
};
static GLushort high_r_indices[] = {
    0, 1, 2,
    3, 1, 0,
    2, 4, 0,
    0, 4, 3,
    5, 1, 3,
    2, 1, 5,
    5, 4, 2,
    3, 4, 5,
};
static const int high_num_indices = 8 * 3;


// clockwise: yellow, cyan, red, green, magenta

static GLfloat high_colours_yellow[] = {
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
};

static GLfloat high_colours_cyan[] = {
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
};

static GLfloat high_colours_red[] = {
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
};

static GLfloat high_colours_green[] = {
  0.00f, 0.80f, 0.00f,
  0.00f, 0.80f, 0.00f,
  0.00f, 0.80f, 0.00f,
  0.00f, 0.80f, 0.00f,
  0.00f, 0.80f, 0.00f,
  0.00f, 0.80f, 0.00f,
};

static GLfloat high_colours_magenta[] = {
  0.80f, 0.00f, 0.80f,
  0.80f, 0.00f, 0.80f,
  0.80f, 0.00f, 0.80f,
  0.80f, 0.00f, 0.80f,
  0.80f, 0.00f, 0.80f,
  0.80f, 0.00f, 0.80f,
};

static GLfloat *high_colours[] = {
  high_colours_yellow,
  high_colours_cyan,
  high_colours_red,
  high_colours_green,
  high_colours_magenta,
};

#endif /* _FOE_FUSEBALL_DATA_H_ */
