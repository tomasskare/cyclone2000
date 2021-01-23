#ifndef _FOE_TANKER_FLIPPER_DATA_H_
#define _FOE_TANKER_FLIPPER_DATA_H_

#define SCALEX 0.17f
#define SCALEY 0.10f
#define SCALEZ 0.16f

static GLfloat low_vertices[] = {
    -0.010f * SCALEX, 0.033f * SCALEY, -0.137f * SCALEZ,
    1.000f * SCALEX, 0.275f * SCALEY, -0.500f * SCALEZ,
    0.498f * SCALEX, 0.478f * SCALEY, -0.020f * SCALEZ,
    0.173f * SCALEX, -0.987f * SCALEY, -0.500f * SCALEZ,
    0.423f * SCALEX, -0.620f * SCALEY, -0.000f * SCALEZ,
    -0.997f * SCALEX, -0.208f * SCALEY, -0.500f * SCALEZ,
    -0.482f * SCALEX, -0.438f * SCALEY, -0.020f * SCALEZ,
    -0.129f * SCALEX, 1.000f * SCALEY, -0.500f * SCALEZ,
    -0.453f * SCALEX, 0.673f * SCALEY, -0.056f * SCALEZ,
    -0.913f * SCALEX, 0.736f * SCALEY, -0.499f * SCALEZ,
    0.305f * SCALEX, 0.861f * SCALEY, -0.507f * SCALEZ,
    0.921f * SCALEX, -0.846f * SCALEY, -0.507f * SCALEZ,
    -0.316f * SCALEX, -0.844f * SCALEY, -0.507f * SCALEZ,
    -0.002f * SCALEX, 0.814f * SCALEY, -0.701f * SCALEZ,
    -0.697f * SCALEX, 0.345f * SCALEY, -0.701f * SCALEZ,
    0.664f * SCALEX, -0.111f * SCALEY, -0.701f * SCALEZ,
    0.120f * SCALEX, 0.780f * SCALEY, -0.701f * SCALEZ,
    0.036f * SCALEX, -0.753f * SCALEY, -0.701f * SCALEZ,
    0.653f * SCALEX, -0.298f * SCALEY, -0.701f * SCALEZ,
    -0.712f * SCALEX, 0.163f * SCALEY, -0.701f * SCALEZ,
    -0.138f * SCALEX, -0.750f * SCALEY, -0.701f * SCALEZ,
};
static const int low_num_vertices = 21;

static GLushort low_indices[] = {
    0, 2, 1,
    0, 4, 3,
    0, 6, 5,
    0, 8, 7,
    0, 9, 8,
    9, 7, 8,
    10, 1, 2,
    0, 10, 2,
    11, 3, 4,
    12, 5, 6,
    12, 6, 0,
    4, 0, 11,
    0, 14, 13,
    15, 0, 16,
    17, 0, 18,
    19, 0, 20,
};
static const int low_num_indices = 16 * 3;

/*
 * wing 1 - left top
 * wing 2 - right top
 * wing 3 - right bottom
 * wing 4 - left bottom
 * back N - behind wing N
 */

static GLfloat low_colours_1[] = {
  0.70f, 0.10f, 0.10f, // center
  0.50f, 0.00f, 0.00f, // wing 2 right edge
  0.80f, 0.20f, 0.10f, // wing 2 top
  0.50f, 0.00f, 0.00f, // wing 3 right edge
  0.80f, 0.20f, 0.10f, // wing 3 top
  0.50f, 0.00f, 0.00f, // wing 4 right edge
  0.80f, 0.20f, 0.10f, // wing 4 top
  0.50f, 0.00f, 0.00f, // wing 1 right edge
  0.80f, 0.20f, 0.10f, // wing 1 top
  0.50f, 0.00f, 0.00f, // wing 1 left edge
  0.50f, 0.00f, 0.00f, // wing 2 left edge
  0.50f, 0.00f, 0.00f, // wing 3 left edge
  0.50f, 0.00f, 0.00f, // wing 4 left edge
  0.20f, 0.50f, 0.10f, // back 1 right edge
  0.10f, 0.40f, 0.10f, // back 1 left edge
  0.10f, 0.40f, 0.10f, // back 2 right edge
  0.20f, 0.50f, 0.10f, // back 2 left edge
  0.20f, 0.50f, 0.10f, // back 3 right edge
  0.10f, 0.40f, 0.10f, // back 3 left edge
  0.10f, 0.40f, 0.10f, // back 4 right edge
  0.20f, 0.50f, 0.10f, // back 4 left edge
};

static GLfloat low_colours_2[] = {
  0.10f, 0.70f, 0.70f, // center
  0.00f, 0.50f, 0.50f, // wing 2 right edge
  0.20f, 0.80f, 0.70f, // wing 2 top
  0.00f, 0.50f, 0.50f, // wing 3 right edge
  0.20f, 0.80f, 0.70f, // wing 3 top
  0.00f, 0.50f, 0.50f, // wing 4 right edge
  0.20f, 0.80f, 0.70f, // wing 4 top
  0.00f, 0.50f, 0.50f, // wing 1 right edge
  0.20f, 0.80f, 0.70f, // wing 1 top
  0.00f, 0.50f, 0.50f, // wing 1 left edge
  0.00f, 0.50f, 0.50f, // wing 2 left edge
  0.00f, 0.50f, 0.50f, // wing 3 left edge
  0.00f, 0.50f, 0.50f, // wing 4 left edge
  0.50f, 0.20f, 0.10f, // back 1 right edge
  0.40f, 0.10f, 0.10f, // back 1 left edge
  0.40f, 0.10f, 0.10f, // back 2 right edge
  0.50f, 0.20f, 0.10f, // back 2 left edge
  0.50f, 0.20f, 0.10f, // back 3 right edge
  0.40f, 0.10f, 0.10f, // back 3 left edge
  0.40f, 0.10f, 0.10f, // back 4 right edge
  0.50f, 0.20f, 0.10f, // back 4 left edge
};



static GLfloat high_vertices[] = {
    0.020f * SCALEX, -0.007f * SCALEY, -0.246f * SCALEZ,
    0.997f * SCALEX, 0.266f * SCALEY, -0.512f * SCALEZ,
    0.500f * SCALEX, 0.475f * SCALEY, -0.020f * SCALEZ,
    0.180f * SCALEX, -0.960f * SCALEY, -0.510f * SCALEZ,
    0.424f * SCALEX, -0.616f * SCALEY, 0.000f * SCALEZ,
    -1.000f * SCALEX, -0.209f * SCALEY, -0.508f * SCALEZ,
    -0.483f * SCALEX, -0.435f * SCALEY, -0.020f * SCALEZ,
    -0.124f * SCALEX, 1.000f * SCALEY, -0.507f * SCALEZ,
    -0.455f * SCALEX, 0.669f * SCALEY, -0.056f * SCALEZ,
    -0.936f * SCALEX, 0.729f * SCALEY, -0.444f * SCALEZ,
    0.298f * SCALEX, 0.852f * SCALEY, -0.499f * SCALEZ,
    0.925f * SCALEX, -0.844f * SCALEY, -0.520f * SCALEZ,
    -0.299f * SCALEX, -0.833f * SCALEY, -0.481f * SCALEZ,
    -0.002f * SCALEX, 0.809f * SCALEY, -0.701f * SCALEZ,
    -0.699f * SCALEX, 0.343f * SCALEY, -0.701f * SCALEZ,
    0.666f * SCALEX, -0.110f * SCALEY, -0.701f * SCALEZ,
    0.120f * SCALEX, 0.775f * SCALEY, -0.701f * SCALEZ,
    0.036f * SCALEX, -0.749f * SCALEY, -0.701f * SCALEZ,
    0.655f * SCALEX, -0.296f * SCALEY, -0.701f * SCALEZ,
    -0.714f * SCALEX, 0.162f * SCALEY, -0.701f * SCALEZ,
    -0.138f * SCALEX, -0.745f * SCALEY, -0.701f * SCALEZ,
};
static const int high_num_vertices = 21;

static GLfloat high_normals[] = {
  0.0184f, 0.0013f, 0.9998f,
  0.9798f, -0.1998f, 0.0086f,
  0.1746f, 0.2767f, 0.9450f,
  -0.6142f, -0.7835f, 0.0947f,
  0.0324f, -0.3368f, 0.9410f,
  -0.9639f, 0.2624f, 0.0461f,
  -0.1690f, -0.2489f, 0.9537f,
  0.4260f, 0.8886f, 0.1698f,
  -0.0784f, 0.3096f, 0.9476f,
  -0.9930f, 0.0559f, 0.1039f,
  -0.2877f, 0.9569f, -0.0402f,
  0.9960f, 0.0271f, -0.0847f,
  0.2352f, -0.9718f, 0.0182f,
  -0.3058f, 0.4574f, 0.8350f,
  -0.3058f, 0.4574f, 0.8350f,
  0.5755f, 0.3551f, 0.7367f,
  0.5755f, 0.3551f, 0.7367f,
  0.3538f, -0.4834f, 0.8007f,
  0.3538f, -0.4834f, 0.8007f,
  -0.5505f, -0.3496f, 0.7582f,
  -0.5505f, -0.3496f, 0.7582f,
};

static GLushort high_indices[] = {
    0, 2, 1,
    0, 4, 3,
    0, 6, 5,
    0, 8, 7,
    0, 9, 8,
    9, 7, 8,
    10, 1, 2,
    0, 10, 2,
    11, 3, 4,
    12, 5, 6,
    12, 6, 0,
    4, 0, 11,
    0, 14, 13,
    15, 0, 16,
    17, 0, 18,
    19, 0, 20,
    0, 7, 9,
    0, 5, 12,
    0, 3, 11,
    0, 1, 10,
};
static const int high_num_indices = 20 * 3;

/*
 * wing 1 - left top
 * wing 2 - right top
 * wing 3 - right bottom
 * wing 4 - left bottom
 * back N - behind wing N
 */

static GLfloat high_colours_1[] = {
  0.80f, 0.20f, 0.10f, // wing
  0.80f, 0.20f, 0.10f, // wing
  0.80f, 0.20f, 0.10f, // wing
  0.80f, 0.20f, 0.10f, // wing
  0.80f, 0.20f, 0.10f, // wing
  0.80f, 0.20f, 0.10f, // wing
  0.80f, 0.20f, 0.10f, // wing
  0.80f, 0.20f, 0.10f, // wing
  0.80f, 0.20f, 0.10f, // wing
  0.80f, 0.20f, 0.10f, // wing
  0.80f, 0.20f, 0.10f, // wing
  0.80f, 0.20f, 0.10f, // wing
  0.80f, 0.20f, 0.10f, // wing
  0.20f, 0.50f, 0.10f, // back
  0.20f, 0.50f, 0.10f, // back
  0.20f, 0.50f, 0.10f, // back
  0.20f, 0.50f, 0.10f, // back
  0.20f, 0.50f, 0.10f, // back
  0.20f, 0.50f, 0.10f, // back
  0.20f, 0.50f, 0.10f, // back
  0.20f, 0.50f, 0.10f, // back
};

static GLfloat high_colours_2[] = {
  0.20f, 0.80f, 0.70f, // wing
  0.20f, 0.80f, 0.70f, // wing
  0.20f, 0.80f, 0.70f, // wing
  0.20f, 0.80f, 0.70f, // wing
  0.20f, 0.80f, 0.70f, // wing
  0.20f, 0.80f, 0.70f, // wing
  0.20f, 0.80f, 0.70f, // wing
  0.20f, 0.80f, 0.70f, // wing
  0.20f, 0.80f, 0.70f, // wing
  0.20f, 0.80f, 0.70f, // wing
  0.20f, 0.80f, 0.70f, // wing
  0.20f, 0.80f, 0.70f, // wing
  0.20f, 0.80f, 0.70f, // wing
  0.50f, 0.20f, 0.10f, // back
  0.50f, 0.20f, 0.10f, // back
  0.50f, 0.20f, 0.10f, // back
  0.50f, 0.20f, 0.10f, // back
  0.50f, 0.20f, 0.10f, // back
  0.50f, 0.20f, 0.10f, // back
  0.50f, 0.20f, 0.10f, // back
  0.50f, 0.20f, 0.10f, // back
};


#endif /* _FOE_TANKER_FLIPPER_DATA_H_ */
