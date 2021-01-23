#ifndef _FOE_FLIPPER_DATA_H_
#define _FOE_FLIPPER_DATA_H_

#define SCALEX 0.140f
#define SCALEY 0.085f
#define SCALEZ 0.140f

static GLfloat low_vertices[] = {
  0.241f * SCALEX, -0.000f * SCALEY, 0.000f * SCALEZ,
  0.511f * SCALEX, 0.007f * SCALEY, -0.140f * SCALEZ,
  0.989f * SCALEX, 0.976f * SCALEY, -0.500f * SCALEZ,
  1.000f * SCALEX, -0.976f * SCALEY, -0.500f * SCALEZ,
  0.000f * SCALEX, 0.007f * SCALEY, -0.500f * SCALEZ,
  -0.239f * SCALEX, 0.017f * SCALEY, 0.000f * SCALEZ,
  -0.510f * SCALEX, 0.012f * SCALEY, -0.140f * SCALEZ,
  -0.991f * SCALEX, -0.953f * SCALEY, -0.500f * SCALEZ,
  -0.994f * SCALEX, 1.000f * SCALEY, -0.500f * SCALEZ,
};
static const int low_num_vertices = 9;

static GLfloat low_colours_1[] = {
  0.80f, 0.00f, 0.00f, // rightish
  0.60f, 0.00f, 0.00f, // rightisher
  0.60f, 0.00f, 0.00f, // right up
  0.60f, 0.00f, 0.00f, // right down
  0.90f, 0.30f, 0.30f, // middle center
  0.80f, 0.00f, 0.00f, // leftish
  0.60f, 0.00f, 0.00f, // leftisher
  0.60f, 0.00f, 0.00f, // left down
  0.60f, 0.00f, 0.00f, // left up
};

static GLfloat low_colours_2[] = {
  0.00f, 0.80f, 0.80f, // rightish
  0.00f, 0.60f, 0.60f, // rightisher
  0.00f, 0.60f, 0.60f, // right up
  0.00f, 0.60f, 0.60f, // right down
  0.30f, 0.90f, 0.90f, // middle center
  0.00f, 0.80f, 0.80f, // leftish
  0.00f, 0.60f, 0.60f, // leftisher
  0.00f, 0.60f, 0.60f, // left down
  0.00f, 0.60f, 0.60f, // left up
};

static GLfloat low_colours_3[] = {
  0.80f, 0.80f, 0.00f, // rightish
  0.60f, 0.60f, 0.00f, // rightisher
  0.60f, 0.60f, 0.00f, // right up
  0.60f, 0.60f, 0.00f, // right down
  0.90f, 0.90f, 0.30f, // middle center
  0.80f, 0.80f, 0.00f, // leftish
  0.60f, 0.60f, 0.00f, // leftisher
  0.60f, 0.60f, 0.00f, // left down
  0.60f, 0.60f, 0.00f, // left up
};

static GLushort low_indices[] = {
  0, 2, 1,
  1, 3, 0,
  0, 4, 2,
  3, 4, 0,
  5, 7, 6,
  6, 8, 5,
  5, 4, 7,
  8, 4, 5,
};
static const int low_num_indices = 8 * 3;


static GLfloat low_mutant_vertices_1[] = {
    0.765f * SCALEX, -0.000f * SCALEY, 0.000f * SCALEZ,
    0.480f * SCALEX, -0.000f * SCALEY, -0.140f * SCALEZ,
    0.000f * SCALEX, -1.000f * SCALEY, -0.500f * SCALEZ,
    0.000f * SCALEX, 1.000f * SCALEY, -0.500f * SCALEZ,
    1.000f * SCALEX, -0.000f * SCALEY, -0.500f * SCALEZ,
    -0.765f * SCALEX, -0.000f * SCALEY, -0.000f * SCALEZ,
    -0.480f * SCALEX, -0.000f * SCALEY, -0.140f * SCALEZ,
    -1.000f * SCALEX, -0.000f * SCALEY, -0.500f * SCALEZ,
};
static const int low_mutant_num_vertices_1 = 8;

static GLfloat low_mutant_colours_1[] = {
  0.00f, 0.70f, 0.70f, // second rightmost
  0.00f, 0.50f, 0.50f, // third rightmost
  0.00f, 0.50f, 0.50f, // top
  0.00f, 0.50f, 0.50f, // bottom
  0.20f, 0.80f, 0.80f, // rightmost
  0.00f, 0.70f, 0.70f, // second leftmost
  0.00f, 0.50f, 0.50f, // third leftmost
  0.20f, 0.80f, 0.80f, // leftmost
};

static GLushort low_mutant_indices_1[] = {
    0, 2, 1,
    1, 3, 0,
    0, 4, 2,
    3, 4, 0,
    3, 6, 5,
    3, 5, 7,
    6, 2, 5,
    7, 5, 2,
};
static const int low_mutant_num_indices_1 = 8 * 3;


static GLfloat low_mutant_vertices_2[] = {
    0.480f * SCALEX, -0.000f * SCALEY, -0.000f * SCALEZ,
    0.480f * SCALEX, -0.000f * SCALEY, -0.000f * SCALEZ,
    0.000f * SCALEX, -1.000f * SCALEY, -0.500f * SCALEZ,
    0.000f * SCALEX, -1.000f * SCALEY, -0.500f * SCALEZ,
    0.000f * SCALEX, 1.000f * SCALEY, -0.500f * SCALEZ,
    0.000f * SCALEX, 1.000f * SCALEY, -0.500f * SCALEZ,
    1.000f * SCALEX, -0.000f * SCALEY, -0.500f * SCALEZ,
    1.000f * SCALEX, -0.000f * SCALEY, -0.500f * SCALEZ,
    -0.480f * SCALEX, -0.000f * SCALEY, -0.000f * SCALEZ,
    -0.480f * SCALEX, -0.000f * SCALEY, -0.000f * SCALEZ,
    -1.000f * SCALEX, -0.000f * SCALEY, -0.500f * SCALEZ,
    -1.000f * SCALEX, -0.000f * SCALEY, -0.500f * SCALEZ,
};
static const int low_mutant_num_vertices_2 = 12;

static GLfloat low_mutant_colours_2[] = {
  0.20f, 0.50f, 0.20f, // third rightmost green
  0.50f, 0.00f, 0.40f, // third rightmost pink
  0.35f, 0.75f, 0.30f, // top green
  0.75f, 0.00f, 0.60f, // top pink
  0.35f, 0.75f, 0.30f, // bottom green
  0.75f, 0.00f, 0.60f, // bottom pink
  0.45f, 0.95f, 0.35f, // rightmost green
  0.95f, 0.00f, 0.75f, // rightmost pink
  0.20f, 0.50f, 0.20f, // third leftmost green
  0.50f, 0.00f, 0.40f, // third leftmost pink
  0.45f, 0.95f, 0.35f, // leftmost green
  0.95f, 0.00f, 0.75f, // leftmost pink
};

static GLfloat low_mutant_colours_2_white[] = {
  0.50f, 0.50f, 0.45f,
  0.50f, 0.40f, 0.40f,
  0.75f, 0.75f, 0.70f,
  0.75f, 0.60f, 0.60f,
  0.75f, 0.75f, 0.70f,
  0.75f, 0.60f, 0.60f,
  0.95f, 0.95f, 0.85f,
  0.95f, 0.75f, 0.75f,
  0.50f, 0.50f, 0.45f,
  0.50f, 0.40f, 0.40f,
  0.95f, 0.95f, 0.85f,
  0.95f, 0.75f, 0.75f,
};

static GLushort low_mutant_indices_2[] = {
    1, 7, 3,
    9, 11, 5,
    6, 0, 4,
    8, 2, 10,
};
static const int low_mutant_num_indices_2 = 4 * 3;




static GLfloat high_vertices[] = {
  -0.239f * SCALEX, 0.017f * SCALEY, 0.000f * SCALEZ,
  -0.527f * SCALEX, 0.044f * SCALEY, -0.176f * SCALEZ,
  -0.995f * SCALEX, -0.961f * SCALEY, -0.501f * SCALEZ,
  -0.991f * SCALEX, 0.981f * SCALEY, -0.500f * SCALEZ,
  -0.003f * SCALEX, 0.003f * SCALEY, -0.499f * SCALEZ,
  -0.239f * SCALEX, 0.000f * SCALEY, -0.998f * SCALEZ,
  -0.526f * SCALEX, 0.010f * SCALEY, -0.857f * SCALEZ,
  -0.457f * SCALEX, 0.003f * SCALEY, -0.508f * SCALEZ,
  0.234f * SCALEX, -0.002f * SCALEY, 0.000f * SCALEZ,
  0.522f * SCALEX, -0.020f * SCALEY, -0.176f * SCALEZ,
  0.975f * SCALEX, 1.000f * SCALEY, -0.501f * SCALEZ,
  1.000f * SCALEX, -0.941f * SCALEY, -0.500f * SCALEZ,
  0.233f * SCALEX, 0.015f * SCALEY, -0.998f * SCALEZ,
  0.520f * SCALEX, 0.014f * SCALEY, -0.857f * SCALEZ,
  0.451f * SCALEX, 0.019f * SCALEY, -0.508f * SCALEZ,
};
static const int high_num_vertices = 15;

static GLfloat high_normals[] = {
  0.1221f, 0.0035f, 0.9925f,
  -0.8993f, 0.0014f, 0.4374f,
  -0.9247f, -0.3772f, -0.0521f,
  -0.9232f, 0.3839f, -0.0156f,
  0.0427f, -0.9990f, 0.0105f,
  0.1851f, -0.0006f, -0.9827f,
  -0.8745f, 0.0045f, -0.4850f,
  -1.0000f, 0.0011f, -0.0062f,
  -0.1210f, -0.0099f, 0.9926f,
  0.8994f, 0.0143f, 0.4369f,
  0.9190f, 0.3907f, -0.0531f,
  0.9295f, -0.3684f, -0.0153f,
  -0.1845f, -0.0067f, -0.9828f,
  0.8743f, 0.0109f, -0.4852f,
  0.9998f, 0.0179f, -0.0067f,
};

static GLfloat high_colours_1[] = {
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
};

static GLfloat high_colours_2[] = {
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
  0.00f, 0.80f, 0.80f,
};

static GLfloat high_colours_3[] = {
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
  0.80f, 0.80f, 0.00f,
};

static GLushort high_indices[] = {
    0, 2, 1,
    1, 3, 0,
    0, 4, 2,
    3, 4, 0,
    5, 3, 6,
    6, 2, 5,
    5, 4, 3,
    2, 4, 5,
    6, 3, 7,
    7, 3, 1,
    2, 6, 7,
    2, 7, 1,
    8, 10, 9,
    9, 11, 8,
    8, 4, 10,
    11, 4, 8,
    12, 11, 13,
    13, 10, 12,
    12, 4, 11,
    10, 4, 12,
    13, 11, 14,
    14, 11, 9,
    10, 13, 14,
    10, 14, 9,
};
static const int high_num_indices = 24 * 3;


static GLfloat high_mutant_vertices_1[] = {
    0.763f * SCALEX, 0.017f * SCALEY, 0.000f * SCALEZ,
    0.474f * SCALEX, 0.055f * SCALEY, -0.176f * SCALEZ,
    -0.009f * SCALEX, -0.967f * SCALEY, -0.500f * SCALEZ,
    0.012f * SCALEX, 1.000f * SCALEY, -0.500f * SCALEZ,
    1.000f * SCALEX, -0.006f * SCALEY, -0.499f * SCALEZ,
    0.762f * SCALEX, -0.000f * SCALEY, -0.998f * SCALEZ,
    0.475f * SCALEX, 0.020f * SCALEY, -0.857f * SCALEZ,
    0.544f * SCALEX, 0.011f * SCALEY, -0.508f * SCALEZ,
    -0.759f * SCALEX, -0.009f * SCALEY, 0.000f * SCALEZ,
    -0.470f * SCALEX, -0.038f * SCALEY, -0.176f * SCALEZ,
    -0.997f * SCALEX, 0.005f * SCALEY, -0.499f * SCALEZ,
    -0.759f * SCALEX, 0.008f * SCALEY, -0.998f * SCALEZ,
    -0.472f * SCALEX, -0.003f * SCALEY, -0.857f * SCALEZ,
    -0.541f * SCALEX, 0.004f * SCALEY, -0.508f * SCALEZ,
};
static const int high_mutant_num_vertices_1 = 14;

static GLfloat high_mutant_normals_1[] = {
  0.1240f, 0.0028f, 0.9923f,
  -0.8988f, 0.0080f, 0.4382f,
  -0.0116f, -0.9958f, -0.0909f,
  0.0123f, 0.9955f, -0.0937f,
  0.9999f, -0.0121f, -0.0010f,
  0.1851f, -0.0011f, -0.9827f,
  -0.8743f, 0.0117f, -0.4852f,
  -1.0000f, 0.0067f, -0.0065f,
  -0.1237f, -0.0114f, 0.9922f,
  0.8993f, -0.0038f, 0.4372f,
  -1.0000f, -0.0030f, -0.0002f,
  -0.1851f, -0.0070f, -0.9827f,
  0.8744f, -0.0073f, -0.4852f,
  1.0000f, 0.0008f, -0.0075f,
};

static GLfloat high_mutant_colours_1[] = {
  0.00f, 0.70f, 0.70f,
  0.00f, 0.70f, 0.70f,
  0.00f, 0.70f, 0.70f,
  0.00f, 0.70f, 0.70f,
  0.00f, 0.70f, 0.70f,
  0.00f, 0.70f, 0.70f,
  0.00f, 0.70f, 0.70f,
  0.00f, 0.70f, 0.70f,
  0.00f, 0.70f, 0.70f,
  0.00f, 0.70f, 0.70f,
  0.00f, 0.70f, 0.70f,
  0.00f, 0.70f, 0.70f,
  0.00f, 0.70f, 0.70f,
  0.00f, 0.70f, 0.70f,
};

static GLushort high_mutant_indices_1[] = {
    0, 2, 1,
    1, 3, 0,
    0, 4, 2,
    3, 4, 0,
    5, 3, 6,
    6, 2, 5,
    5, 4, 3,
    2, 4, 5,
    6, 3, 7,
    7, 3, 1,
    2, 6, 7,
    2, 7, 1,
    8, 3, 9,
    9, 2, 8,
    8, 10, 3,
    2, 10, 8,
    11, 2, 12,
    12, 3, 11,
    11, 10, 2,
    3, 10, 11,
    12, 2, 13,
    13, 2, 9,
    3, 12, 13,
    3, 13, 9,
};
static const int high_mutant_num_indices_1 = 24 * 3;



static GLfloat high_mutant_vertices_2[] = {
    0.763f * SCALEX, 0.017f * SCALEY, -0.003f * SCALEZ,
    0.474f * SCALEX, 0.055f * SCALEY, -0.178f * SCALEZ,
    -0.009f * SCALEX, -0.967f * SCALEY, -0.500f * SCALEZ,
    0.483f * SCALEX, 0.041f * SCALEY, -0.175f * SCALEZ,
    0.772f * SCALEX, 0.003f * SCALEY, -0.000f * SCALEZ,
    0.022f * SCALEX, 0.987f * SCALEY, -0.498f * SCALEZ,
    1.000f * SCALEX, -0.006f * SCALEY, -0.499f * SCALEZ,
    0.013f * SCALEX, 1.000f * SCALEY, -0.498f * SCALEZ,
    0.763f * SCALEX, 0.017f * SCALEY, -0.000f * SCALEZ,
    1.000f * SCALEX, -0.006f * SCALEY, -0.496f * SCALEZ,
    0.772f * SCALEX, -0.014f * SCALEY, -0.992f * SCALEZ,
    0.484f * SCALEX, 0.007f * SCALEY, -0.852f * SCALEZ,
    0.475f * SCALEX, 0.020f * SCALEY, -0.855f * SCALEZ,
    0.762f * SCALEX, -0.000f * SCALEY, -0.995f * SCALEZ,
    0.762f * SCALEX, -0.000f * SCALEY, -0.992f * SCALEZ,
    0.475f * SCALEX, 0.020f * SCALEY, -0.852f * SCALEZ,
    0.544f * SCALEX, 0.011f * SCALEY, -0.505f * SCALEZ,
    0.474f * SCALEX, 0.055f * SCALEY, -0.175f * SCALEZ,
    0.544f * SCALEX, 0.011f * SCALEY, -0.508f * SCALEZ,
    -0.759f * SCALEX, -0.009f * SCALEY, 0.000f * SCALEZ,
    -0.470f * SCALEX, -0.038f * SCALEY, -0.175f * SCALEZ,
    0.013f * SCALEX, 1.000f * SCALEY, -0.498f * SCALEZ,
    -0.470f * SCALEX, -0.037f * SCALEY, -0.178f * SCALEZ,
    -0.759f * SCALEX, -0.009f * SCALEY, -0.003f * SCALEZ,
    -0.009f * SCALEX, -0.967f * SCALEY, -0.501f * SCALEZ,
    -0.997f * SCALEX, 0.005f * SCALEY, -0.496f * SCALEZ,
    -0.997f * SCALEX, 0.005f * SCALEY, -0.499f * SCALEZ,
    -0.759f * SCALEX, 0.008f * SCALEY, -0.995f * SCALEZ,
    -0.472f * SCALEX, -0.003f * SCALEY, -0.855f * SCALEZ,
    -0.471f * SCALEX, -0.003f * SCALEY, -0.852f * SCALEZ,
    -0.759f * SCALEX, 0.008f * SCALEY, -0.992f * SCALEZ,
    -0.541f * SCALEX, 0.004f * SCALEY, -0.508f * SCALEZ,
    -0.541f * SCALEX, 0.004f * SCALEY, -0.505f * SCALEZ,
};
static const int high_mutant_num_vertices_2 = 33;

static GLfloat high_mutant_normals_2[] = {
  0.0919f, -0.5054f, 0.8579f,
  -0.8736f, 0.2897f, 0.3910f,
  -0.9300f, -0.3637f, -0.0532f,
  -0.5134f, 0.0424f, 0.8571f,
  -0.5134f, 0.0424f, 0.8571f,
  -0.9841f, 0.1727f, -0.0408f,
  0.6896f, -0.7241f, 0.0104f,
  -0.6957f, 0.7178f, 0.0279f,
  0.6818f, 0.6695f, 0.2947f,
  0.7138f, 0.7003f, -0.0117f,
  -0.4267f, 0.1225f, -0.8961f,
  -0.4267f, 0.1225f, -0.8961f,
  -0.8498f, 0.2498f, -0.4641f,
  0.1445f, -0.5397f, -0.8293f,
  0.6773f, 0.6639f, -0.3170f,
  -0.8691f, -0.4678f, 0.1607f,
  -0.8808f, -0.4731f, 0.0190f,
  -0.8747f, -0.4688f, -0.1230f,
  -0.8702f, 0.4918f, -0.0308f,
  -0.1018f, 0.5018f, 0.8589f,
  0.8749f, -0.2854f, 0.3912f,
  0.9294f, 0.3651f, -0.0542f,
  0.8529f, 0.2670f, 0.4486f,
  -0.1120f, -0.5260f, 0.8431f,
  0.9217f, -0.3875f, -0.0167f,
  -0.7017f, 0.7123f, 0.0108f,
  -0.7013f, -0.7128f, -0.0112f,
  -0.1580f, -0.5403f, -0.8265f,
  0.8473f, 0.2313f, -0.4782f,
  0.8502f, -0.2459f, -0.4656f,
  -0.1567f, 0.5353f, -0.8300f,
  0.8768f, 0.4805f, 0.0173f,
  0.8736f, -0.4857f, -0.0305f,
};

static GLfloat high_mutant_colours_2[] = {
  0.95f, 0.00f, 0.75f,
  0.95f, 0.00f, 0.75f,
  0.95f, 0.00f, 0.75f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.95f, 0.00f, 0.75f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.95f, 0.00f, 0.75f,
  0.95f, 0.00f, 0.75f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.95f, 0.00f, 0.75f,
  0.95f, 0.00f, 0.75f,
  0.95f, 0.00f, 0.75f,
  0.95f, 0.00f, 0.75f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.95f, 0.00f, 0.75f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.95f, 0.00f, 0.75f,
  0.95f, 0.00f, 0.75f,
  0.45f, 0.95f, 0.35f,
  0.95f, 0.00f, 0.75f,
};

static GLfloat high_mutant_colours_2_white[] = {
  0.95f, 0.00f, 0.75f,
  0.95f, 0.00f, 0.75f,
  0.95f, 0.00f, 0.75f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.95f, 0.00f, 0.75f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.95f, 0.00f, 0.75f,
  0.95f, 0.00f, 0.75f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.95f, 0.00f, 0.75f,
  0.95f, 0.00f, 0.75f,
  0.95f, 0.00f, 0.75f,
  0.95f, 0.00f, 0.75f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.95f, 0.00f, 0.75f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.45f, 0.95f, 0.35f,
  0.95f, 0.00f, 0.75f,
  0.95f, 0.00f, 0.75f,
  0.45f, 0.95f, 0.35f,
  0.95f, 0.00f, 0.75f,
};

static GLushort high_mutant_indices_2[] = {
    0, 2, 1,
    3, 5, 4,
    0, 6, 2,
    7, 9, 8,
    10, 5, 11,
    12, 2, 13,
    14, 9, 7,
    2, 6, 13,
    15, 7, 16,
    16, 7, 17,
    2, 12, 18,
    2, 18, 1,
    19, 21, 20,
    22, 24, 23,
    19, 25, 21,
    24, 26, 23,
    27, 24, 28,
    29, 21, 30,
    27, 26, 24,
    21, 25, 30,
    28, 24, 31,
    31, 24, 22,
    21, 29, 32,
    21, 32, 20,
};
static const int high_mutant_num_indices_2 = 24 * 3;




#endif /* _FOE_FLIPPER_DATA_H_ */
