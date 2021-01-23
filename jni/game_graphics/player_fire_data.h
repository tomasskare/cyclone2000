#ifndef _PLAYER_FIRE_DATA_H_
#define _PLAYER_FIRE_DATA_H_

#define SCALEX 0.04f
#define SCALEY 0.04f
#define SCALEZ 0.04f

static GLfloat points_4[] = {
  -0.0900f, -0.0900f, 0.0000f,
   0.0900f, -0.0900f, 0.0000f,
   0.0900f,  0.0900f, 0.0000f,
  -0.0900f,  0.0900f, 0.0000f,
};
static const int num_points = 4;

static GLfloat points_4_colours[] = {
  0.80f, 0.80f, 0.20f,
  0.80f, 0.30f, 0.20f,
  0.70f, 0.10f, 0.70f,
  0.10f, 0.10f, 0.70f,
};


static GLfloat low_vertices[] = {
  0.000f * SCALEX, 1.000f * SCALEY, -0.500f * SCALEZ,
  -1.000f * SCALEX, 0.520f * SCALEY, -0.345f * SCALEZ,
  0.000f * SCALEX, 0.520f * SCALEY, 0.000f * SCALEZ,
  0.000f * SCALEX, -0.922f * SCALEY, -0.500f * SCALEZ,
  0.618f * SCALEX, -0.441f * SCALEY, -0.095f * SCALEZ,
  -0.618f * SCALEX, -0.441f * SCALEY, -0.095f * SCALEZ,
  1.000f * SCALEX, 0.520f * SCALEY, -0.345f * SCALEZ,
  -1.000f * SCALEX, -0.441f * SCALEY, -0.655f * SCALEZ,
  1.000f * SCALEX, -0.441f * SCALEY, -0.655f * SCALEZ,
};
static const int low_num_vertices = 9;

static GLushort low_indices[] = {
  0, 2, 1,
  3, 5, 4,
  0, 6, 2,
  3, 7, 5,
  3, 4, 8,
  2, 6, 4,
  1, 2, 5,
  8, 4, 6,
  5, 7, 1,
  4, 5, 2,
};
static const int low_num_indices = 10 * 3;

static GLfloat low_colours[] = {
  0.15f, 0.65f, 0.75f, // top
  0.15f, 0.65f, 0.75f, // left, upish
  0.20f, 0.70f, 0.80f, // middle, upish
  0.15f, 0.65f, 0.75f, // bottom
  0.20f, 0.70f, 0.80f, // rightish, downish
  0.10f, 0.55f, 0.65f, // leftish, downish
  0.15f, 0.65f, 0.75f, // right, upish
  0.10f, 0.50f, 0.60f, // left, downish
  0.15f, 0.65f, 0.75f, // right, downish
};

static GLfloat low_droid_colours[] = {
  0.65f, 0.15f, 0.75f, // top
  0.65f, 0.15f, 0.75f, // left, upish
  0.70f, 0.20f, 0.80f, // middle, upish
  0.65f, 0.15f, 0.75f, // bottom
  0.70f, 0.20f, 0.80f, // rightish, downish
  0.55f, 0.10f, 0.65f, // leftish, downish
  0.65f, 0.15f, 0.75f, // right, upish
  0.50f, 0.10f, 0.60f, // left, downish
  0.65f, 0.15f, 0.75f, // right, downish
};



static GLfloat high_vertices[] = {
    0.000f * SCALEX, 1.000f * SCALEY, -0.500f * SCALEZ,
    -1.000f * SCALEX, 0.520f * SCALEY, -0.345f * SCALEZ,
    0.000f * SCALEX, 0.520f * SCALEY, 0.000f * SCALEZ,
    0.000f * SCALEX, -0.922f * SCALEY, -0.500f * SCALEZ,
    0.618f * SCALEX, -0.441f * SCALEY, -0.095f * SCALEZ,
    -0.618f * SCALEX, -0.441f * SCALEY, -0.095f * SCALEZ,
    1.000f * SCALEX, 0.520f * SCALEY, -0.345f * SCALEZ,
    0.618f * SCALEX, 0.520f * SCALEY, -0.905f * SCALEZ,
    -0.618f * SCALEX, 0.520f * SCALEY, -0.905f * SCALEZ,
    -1.000f * SCALEX, -0.441f * SCALEY, -0.655f * SCALEZ,
    0.000f * SCALEX, -0.441f * SCALEY, -1.000f * SCALEZ,
    1.000f * SCALEX, -0.441f * SCALEY, -0.655f * SCALEZ,
};
static const int high_num_vertices = 12;

static GLfloat high_normals[] = {
  0.0000f, 1.0000f, -0.0084f,
  -0.7804f, 0.4910f, 0.3871f,
  0.0000f, 0.3420f, 0.9397f,
  0.0000f, -1.0000f, 0.0084f,
  0.3752f, -0.3885f, 0.8416f,
  -0.3752f, -0.3885f, 0.8416f,
  0.7804f, 0.4910f, 0.3871f,
  0.3751f, 0.3889f, -0.8415f,
  -0.3751f, 0.3889f, -0.8415f,
  -0.7807f, -0.4906f, -0.3872f,
  0.0000f, -0.3416f, -0.9398f,
  0.7807f, -0.4906f, -0.3872f,
};

static GLushort high_indices[] = {
    0, 2, 1,
    3, 5, 4,
    0, 6, 2,
    0, 7, 6,
    0, 8, 7,
    0, 1, 8,
    3, 9, 5,
    3, 10, 9,
    3, 11, 10,
    3, 4, 11,
    2, 6, 4,
    6, 7, 11,
    7, 8, 10,
    8, 1, 9,
    1, 2, 5,
    11, 4, 6,
    10, 11, 7,
    9, 10, 8,
    5, 9, 1,
    4, 5, 2,
};
static const int high_num_indices = 20 * 3;


static GLfloat high_colours[] = {
  0.20f, 0.70f, 0.80f,
  0.20f, 0.70f, 0.80f,
  0.20f, 0.70f, 0.80f,
  0.20f, 0.70f, 0.80f,
  0.20f, 0.70f, 0.80f,
  0.20f, 0.70f, 0.80f,
  0.20f, 0.70f, 0.80f,
  0.20f, 0.70f, 0.80f,
  0.20f, 0.70f, 0.80f,
  0.20f, 0.70f, 0.80f,
  0.20f, 0.70f, 0.80f,
  0.20f, 0.70f, 0.80f,
};

static GLfloat high_droid_colours[] = {
  0.70f, 0.20f, 0.80f,
  0.70f, 0.20f, 0.80f,
  0.70f, 0.20f, 0.80f,
  0.70f, 0.20f, 0.80f,
  0.70f, 0.20f, 0.80f,
  0.70f, 0.20f, 0.80f,
  0.70f, 0.20f, 0.80f,
  0.70f, 0.20f, 0.80f,
  0.70f, 0.20f, 0.80f,
  0.70f, 0.20f, 0.80f,
  0.70f, 0.20f, 0.80f,
  0.70f, 0.20f, 0.80f,
};


static GLfloat glow_vertices[] = { -1.000f,  1.000f, 0.0f,
				    1.000f,  1.000f, 0.0f,
				   -1.000f, -1.000f, 0.0f,
				    1.000f, -1.000f, 0.0f };

static GLfloat glow_tex_coords[] = { 0.492f, 0.000f,
				     0.718f, 0.000f,
				     0.492f, 0.459f,
				     0.718f, 0.459f };

static GLfloat glow_droid_tex_coords[] = { 0.492f, 0.459f,
					   0.718f, 0.459f,
					   0.492f, 0.914f,
					   0.718f, 0.914f };

static GLfloat glow_laser_tex_coords[] = { 0.280f, 0.600f,
					   0.449f, 0.600f,
					   0.280f, 0.934f,
					   0.449f, 0.934f };

#endif /* _PLAYER_FIRE_DATA_H_ */
