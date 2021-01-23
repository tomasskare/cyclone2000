#ifndef _FOE_UFO_DATA_H_
#define _FOE_UFO_DATA_H_

#define SCALEX 0.150f
#define SCALEY 0.150f
#define SCALEZ 0.005f
#define HSCALEZ 0.010f

static GLfloat low_vertices[] = {
  -1.000f * SCALEX, -1.000f * SCALEY, -0.500f * SCALEZ,  // 0 red
  -1.000f * SCALEX, -1.000f * SCALEY, -0.500f * SCALEZ,  // 1 blue
  1.000f * SCALEX, -1.000f * SCALEY, -0.500f * SCALEZ,   // 2 green
  1.000f * SCALEX, -1.000f * SCALEY, -0.500f * SCALEZ,   // 3 blue
  1.000f * SCALEX, 1.000f * SCALEY, -0.500f * SCALEZ,    // 4 green
  1.000f * SCALEX, 1.000f * SCALEY, -0.500f * SCALEZ,    // 5 yellow
  -1.000f * SCALEX, 1.000f * SCALEY, -0.500f * SCALEZ,   // 6 red
  -1.000f * SCALEX, 1.000f * SCALEY, -0.500f * SCALEZ,   // 7 yellow
  -0.500f * SCALEX, -0.500f * SCALEY, -0.000f * SCALEZ,  // 8 dark red
  -0.500f * SCALEX, -0.500f * SCALEY, -0.000f * SCALEZ,	 // 9 dark blue
  0.500f * SCALEX, -0.500f * SCALEY, -0.000f * SCALEZ,   // 10 dark green
  0.500f * SCALEX, -0.500f * SCALEY, -0.000f * SCALEZ,   // 11 dark blue
  0.500f * SCALEX, 0.500f * SCALEY, -0.000f * SCALEZ,    // 12 dark green
  0.500f * SCALEX, 0.500f * SCALEY, -0.000f * SCALEZ,    // 13 dark yellow
  -0.500f * SCALEX, 0.500f * SCALEY, -0.000f * SCALEZ,   // 14 dark red
  -0.500f * SCALEX, 0.500f * SCALEY, -0.000f * SCALEZ,   // 15 dark yellow
};
static const int low_num_vertices = 16;

static GLushort low_indices[] = {
  /* red */
  0, 6, 8,
  8, 6, 14,

  /* green */
  4, 2, 12,
  12, 2, 10,

  /* blue */
  3, 1, 9,
  9, 11, 3,

  /* yellow */
  7, 5, 15,
  15, 5, 13,
};
static const int low_num_indices = 8 * 3;

static GLfloat low_colours[] = {
  0.90, 0.55, 0.50,  // 0 red
  0.65, 0.55, 0.95,  // 1 blue
  0.25, 0.50, 0.40,  // 2 green
  0.50, 0.50, 0.70,  // 3 blue
  0.30, 0.65, 0.50,  // 4 green
  0.45, 0.50, 0.20,  // 5 yellow
  0.80, 0.40, 0.40,  // 6 red
  0.60, 0.75, 0.25,  // 7 yellow
  0.75, 0.45, 0.40,  // 8 dark red
  0.50, 0.40, 0.75,  // 9 dark blue
  0.15, 0.40, 0.25,  // 10 dark green
  0.35, 0.35, 0.55,  // 11 dark blue
  0.10, 0.30, 0.20,  // 12 dark green
  0.20, 0.30, 0.10,  // 13 dark yellow
  0.55, 0.30, 0.30,  // 14 dark red
  0.40, 0.50, 0.15,  // 15 dark yellow
};



static GLfloat high_vertices[] = {
  -1.000f * SCALEX, -1.000f * SCALEY, -0.500f * HSCALEZ,  // 0 red
  -1.000f * SCALEX, -1.000f * SCALEY, -0.500f * HSCALEZ,  // 1 blue
  1.000f * SCALEX, -1.000f * SCALEY, -0.500f * HSCALEZ,   // 2 green
  1.000f * SCALEX, -1.000f * SCALEY, -0.500f * HSCALEZ,   // 3 blue
  1.000f * SCALEX, 1.000f * SCALEY, -0.500f * HSCALEZ,    // 4 green
  1.000f * SCALEX, 1.000f * SCALEY, -0.500f * HSCALEZ,    // 5 yellow
  -1.000f * SCALEX, 1.000f * SCALEY, -0.500f * HSCALEZ,   // 6 red
  -1.000f * SCALEX, 1.000f * SCALEY, -0.500f * HSCALEZ,   // 7 yellow
  -0.500f * SCALEX, -0.500f * SCALEY, -0.000f * HSCALEZ,  // 8 dark red
  -0.500f * SCALEX, -0.500f * SCALEY, -0.000f * HSCALEZ,  // 9 dark blue
  0.500f * SCALEX, -0.500f * SCALEY, -0.000f * HSCALEZ,   // 10 dark green
  0.500f * SCALEX, -0.500f * SCALEY, -0.000f * HSCALEZ,   // 11 dark blue
  0.500f * SCALEX, 0.500f * SCALEY, -0.000f * HSCALEZ,    // 12 dark green
  0.500f * SCALEX, 0.500f * SCALEY, -0.000f * HSCALEZ,    // 13 dark yellow
  -0.500f * SCALEX, 0.500f * SCALEY, -0.000f * HSCALEZ,   // 14 dark red
  -0.500f * SCALEX, 0.500f * SCALEY, -0.000f * HSCALEZ,   // 15 dark yellow
};
static const int high_num_vertices = 16;

static GLfloat high_normals[] = {
  -0.5547f, 0.0000f, 0.8321f,
  0.0000f, -0.5547f, 0.8321f,
  0.5547f, 0.0000f, 0.8321f,
  0.0000f, -0.5547f, 0.8321f,
  0.5547f, 0.0000f, 0.8321f,
  0.0000f, 0.5547f, 0.8321f,
  -0.5547f, 0.0000f, 0.8321f,
  0.0000f, 0.5547f, 0.8321f,
  -0.5547f, 0.0000f, 0.8321f,
  0.0000f, -0.5547f, 0.8321f,
  0.5547f, 0.0000f, 0.8321f,
  0.0000f, -0.5547f, 0.8321f,
  0.5547f, 0.0000f, 0.8321f,
  0.0000f, 0.5547f, 0.8321f,
  -0.5547f, 0.0000f, 0.8321f,
  0.0000f, 0.5547f, 0.8321f,
};


static GLushort high_indices[] = {
  /* red */
  0, 6, 8,
  8, 6, 14,

  /* green */
  4, 2, 12,
  12, 2, 10,

  /* blue */
  3, 1, 9,
  9, 11, 3,

  /* yellow */
  7, 5, 15,
  15, 5, 13,
};
static const int high_num_indices = 8 * 3;

static GLfloat high_colours[] = {
  0.90, 0.55, 0.50,  // 0 red
  0.65, 0.55, 0.95,  // 1 blue
  0.25, 0.50, 0.40,  // 2 green
  0.50, 0.50, 0.70,  // 3 blue
  0.30, 0.65, 0.50,  // 4 green
  0.45, 0.50, 0.20,  // 5 yellow
  0.80, 0.40, 0.40,  // 6 red
  0.60, 0.75, 0.25,  // 7 yellow
  0.75, 0.45, 0.40,  // 8 dark red
  0.50, 0.40, 0.75,  // 9 dark blue
  0.15, 0.40, 0.25,  // 10 dark green
  0.35, 0.35, 0.55,  // 11 dark blue
  0.10, 0.30, 0.20,  // 12 dark green
  0.20, 0.30, 0.10,  // 13 dark yellow
  0.55, 0.30, 0.30,  // 14 dark red
  0.40, 0.50, 0.15,  // 15 dark yellow
};

#endif /* _FOE_UFO_DATA_H_ */
