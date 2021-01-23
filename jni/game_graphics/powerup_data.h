#ifndef _POWERUP_DATA_H_
#define _POWERUP_DATA_H_

#define SCALEX 0.09f
#define SCALEY 0.05f
#define SCALEZ 0.04f


static GLfloat point_12_colours[] = {
  0.80f, 0.80f, 0.20f,
  0.80f, 0.30f, 0.20f,
  0.70f, 0.10f, 0.70f,
  0.10f, 0.10f, 0.70f,
};

static GLfloat point_21_colours[] = {
  0.20f, 0.20f, 0.70f,
  0.20f, 0.70f, 0.70f,
  0.20f, 0.70f, 0.20f,
  0.70f, 0.60f, 0.10f,
};


static GLfloat low_vertices[] = {
  -0.017f * SCALEX, 0.033f * SCALEY, -1.000f * SCALEZ,
  -0.017f * SCALEX, 1.000f * SCALEY, 0.000f * SCALEZ,
  -1.000f * SCALEX, 0.017f * SCALEY, 0.000f * SCALEZ,
  -0.017f * SCALEX, -0.967f * SCALEY, 0.000f * SCALEZ,
  0.992f * SCALEX, 0.033f * SCALEY, 0.000f * SCALEZ,
};
static const int low_num_vertices = 5;

static GLfloat low_colours[] = {
  0.70f, 0.90f, 0.90f, // center
  0.50f, 0.60f, 0.60f, // top
  0.50f, 0.60f, 0.60f, // left
  0.50f, 0.60f, 0.60f, // bottom
  0.50f, 0.60f, 0.60f, // right
};

static GLushort low_indices[] = {
  0, 2, 1,
  2, 0, 3,
  0, 1, 4,
  3, 0, 4,
};
static const int low_num_indices = 4 * 3;


static GLfloat high_vertices[] = {
  -1.000f * SCALEX, 0.025f * SCALEY, -0.496f * SCALEZ,
  -0.009f * SCALEX, 1.000f * SCALEY, -0.495f * SCALEZ,
  -0.017f * SCALEX, 0.033f * SCALEY, -0.990f * SCALEZ,
  -0.009f * SCALEX, -0.967f * SCALEY, -0.495f * SCALEZ,
  0.983f * SCALEX, 0.025f * SCALEY, -0.493f * SCALEZ,
  -0.002f * SCALEX, 0.033f * SCALEY, 0.000f * SCALEZ,
};
static const int high_num_vertices = 6;

static GLfloat high_normals[] = {
  -0.9999f, 0.0127f, 0.0044f,
  -0.0008f, 1.0000f, -0.0009f,
  0.2903f, 0.0060f, -0.9569f,
  -0.0008f, -1.0000f, -0.0009f,
  0.9999f, 0.0127f, -0.0037f,
  -0.2910f, 0.0060f, 0.9567f,
};

static GLfloat high_colours[] = {
  0.70f, 0.90f, 0.90f,
  0.70f, 0.90f, 0.90f,
  0.70f, 0.90f, 0.90f,
  0.70f, 0.90f, 0.90f,
  0.70f, 0.90f, 0.90f,
  0.70f, 0.90f, 0.90f,
};

static GLushort high_indices[] = {
  0, 2, 1,
  3, 1, 2,
  2, 0, 3,
  1, 2, 4,
  2, 3, 4,
  4, 5, 1,
  3, 1, 5,
  5, 4, 3,
  1, 5, 0,
  5, 3, 0,
};
static const int high_num_indices = 10 * 3;


static GLfloat glow_vertices[] = { -1.000f,  1.000f, 0.0f,
				    1.000f,  1.000f, 0.0f,
				   -1.000f, -1.000f, 0.0f,
				    1.000f, -1.000f, 0.0f };

static GLfloat glow_tex_coords[] = { 0.215f, 0.000f,
				     0.492f, 0.000f,
				     0.215f, 0.559f,
				     0.492f, 0.559f };



#endif /* _POWERUP_DATA_H_ */
