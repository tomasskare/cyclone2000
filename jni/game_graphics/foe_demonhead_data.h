#ifndef _FOE_DEMONHEAD_DATA_H_
#define _FOE_DEMONHEAD_DATA_H_

#define BODYSCALEX 0.200f
#define BODYSCALEY 0.133f
#define BODYSCALEZ 0.020f

#define HORNSCALEX 0.110f
#define HORNSCALEY 0.170f
#define HORNSCALEZ 0.010f

#define TOOTHSCALEX 0.060f
#define TOOTHSCALEY 0.180f
#define TOOTHSCALEZ 0.010f

static GLfloat low_body_vertices[] = {
    -0.020f * BODYSCALEX, 0.439f * BODYSCALEY, -0.295f * BODYSCALEZ,
    -0.348f * BODYSCALEX, 0.963f * BODYSCALEY, -1.000f * BODYSCALEZ,
    -0.995f * BODYSCALEX, 0.454f * BODYSCALEY, -0.295f * BODYSCALEZ,
    0.330f * BODYSCALEX, 0.974f * BODYSCALEY, -1.000f * BODYSCALEZ,
    0.974f * BODYSCALEX, 0.474f * BODYSCALEY, -0.295f * BODYSCALEZ,
    -0.329f * BODYSCALEX, -1.000f * BODYSCALEY, -0.614f * BODYSCALEZ,
    -0.013f * BODYSCALEX, -0.479f * BODYSCALEY, -0.295f * BODYSCALEZ,
    -1.000f * BODYSCALEX, -0.508f * BODYSCALEY, -0.295f * BODYSCALEZ,
    0.309f * BODYSCALEX, -1.000f * BODYSCALEY, -0.614f * BODYSCALEZ,
    0.984f * BODYSCALEX, -0.468f * BODYSCALEY, -0.295f * BODYSCALEZ,
    -0.277f * BODYSCALEX, 0.008f * BODYSCALEY, -0.159f * BODYSCALEZ,
    -0.245f * BODYSCALEX, 0.440f * BODYSCALEY, 0.000f * BODYSCALEZ,
    -0.650f * BODYSCALEX, 0.456f * BODYSCALEY, -0.159f * BODYSCALEZ,
    0.679f * BODYSCALEX, 0.456f * BODYSCALEY, -0.159f * BODYSCALEZ,
    0.316f * BODYSCALEX, 0.473f * BODYSCALEY, 0.000f * BODYSCALEZ,
    0.306f * BODYSCALEX, 0.008f * BODYSCALEY, -0.159f * BODYSCALEZ,
};
static int low_num_body_vertices = 16;

static GLfloat low_body_colours[] = {
  // body
  0.60f, 0.60f, 0.55f,
  0.40f, 0.40f, 0.40f,
  0.55f, 0.55f, 0.50f,
  0.40f, 0.40f, 0.40f,
  0.55f, 0.55f, 0.50f,
  0.40f, 0.40f, 0.40f,
  0.55f, 0.55f, 0.50f,
  0.53f, 0.53f, 0.48f,
  0.40f, 0.40f, 0.40f,
  0.53f, 0.53f, 0.48f,

  // eyes
  0.50f, 0.00f, 0.00f,
  0.88f, 0.00f, 0.00f,
  0.75f, 0.00f, 0.00f,

  0.75f, 0.00f, 0.00f,
  0.88f, 0.00f, 0.00f,
  0.50f, 0.00f, 0.00f,
};

static GLushort low_body_indices[] = {
    0, 2, 1,
    0, 1, 3,
    4, 0, 3,
    5, 7, 6,
    8, 5, 6,
    9, 8, 6,
    7, 2, 0,
    6, 7, 0,
    4, 6, 0,
    6, 4, 9,
    10, 12, 11,
    13, 15, 14,
};
static const int low_num_body_indices = 12 * 3;


static GLfloat low_horn_l_vertices[] = {
    -0.693f * HORNSCALEX, 0.352f * HORNSCALEY, -0.500f * HORNSCALEZ,
    -0.686f * HORNSCALEX, -0.329f * HORNSCALEY, -0.833f * HORNSCALEZ,
    -0.011f * HORNSCALEX, -0.006f * HORNSCALEY, -0.833f * HORNSCALEZ,
    -1.000f * HORNSCALEX, 0.006f * HORNSCALEY, -0.500f * HORNSCALEZ,
    -0.994f * HORNSCALEX, 1.000f * HORNSCALEY, 0.000f * HORNSCALEZ,
};

static GLfloat low_horn_r_vertices[] = {
    0.693f * HORNSCALEX, 0.352f * HORNSCALEY, -0.500f * HORNSCALEZ,
    0.686f * HORNSCALEX, -0.329f * HORNSCALEY, -0.833f * HORNSCALEZ,
    0.011f * HORNSCALEX, -0.006f * HORNSCALEY, -0.833f * HORNSCALEZ,
    1.000f * HORNSCALEX, 0.006f * HORNSCALEY, -0.500f * HORNSCALEZ,
    0.994f * HORNSCALEX, 1.000f * HORNSCALEY, 0.000f * HORNSCALEZ,
};
static int low_num_horn_vertices = 5;

static GLfloat low_horn_colours[] = {
  0.60f, 0.65f, 0.25f,
  0.70f, 0.75f, 0.30f,
  0.55f, 0.60f, 0.20f,
  0.80f, 0.85f, 0.35f,
  0.85f, 0.95f, 0.40f,
};

static GLushort low_horn_l_indices[] = {
    0, 2, 1,
    3, 0, 1,
    4, 0, 3,
};

static GLushort low_horn_r_indices[] = {
    0, 1, 2,
    3, 1, 0,
    4, 3, 0,
};

static const int low_num_horn_indices = 3 * 3;



static GLfloat low_tooth_l_vertices[] = {
    -0.033f * TOOTHSCALEX, -1.000f * TOOTHSCALEY, -1.000f * TOOTHSCALEZ,
    0.000f * TOOTHSCALEX, 0.006f * TOOTHSCALEY, 0.000f * TOOTHSCALEZ,
    -1.000f * TOOTHSCALEX, 0.005f * TOOTHSCALEY, 0.000f * TOOTHSCALEZ,
};

static GLfloat low_tooth_r_vertices[] = {
    0.033f * TOOTHSCALEX, -1.000f * TOOTHSCALEY, -1.000f * TOOTHSCALEZ,
    0.000f * TOOTHSCALEX, 0.006f * TOOTHSCALEY, 0.000f * TOOTHSCALEZ,
    1.000f * TOOTHSCALEX, 0.005f * TOOTHSCALEY, 0.000f * TOOTHSCALEZ,
};
static int low_num_tooth_vertices = 3;

static GLfloat low_tooth_colours[] = {
  0.85f, 0.75f, 0.95f,
  0.65f, 0.60f, 0.75f,
  0.60f, 0.55f, 0.70f,
};

static GLushort low_tooth_l_indices[] = {
    0, 2, 1,
};

static GLushort low_tooth_r_indices[] = {
    0, 1, 2,
};

static const int low_num_tooth_indices = 1 * 3;




static GLfloat high_body_vertices[] = {
    -0.020f * BODYSCALEX, 0.410f * BODYSCALEY, -0.176f * BODYSCALEZ,
    -0.342f * BODYSCALEX, 0.889f * BODYSCALEY, -0.610f * BODYSCALEZ,
    -0.989f * BODYSCALEX, 0.415f * BODYSCALEY, -0.269f * BODYSCALEZ,
    0.327f * BODYSCALEX, 0.893f * BODYSCALEY, -0.621f * BODYSCALEZ,
    0.973f * BODYSCALEX, 0.438f * BODYSCALEY, -0.263f * BODYSCALEZ,
    -0.330f * BODYSCALEX, -0.962f * BODYSCALEY, -0.335f * BODYSCALEZ,
    -0.013f * BODYSCALEX, -0.447f * BODYSCALEY, -0.176f * BODYSCALEZ,
    -0.993f * BODYSCALEX, -0.473f * BODYSCALEY, -0.271f * BODYSCALEZ,
    0.290f * BODYSCALEX, -1.000f * BODYSCALEY, -0.253f * BODYSCALEZ,
    0.980f * BODYSCALEX, -0.437f * BODYSCALEY, -0.265f * BODYSCALEZ,
    -0.275f * BODYSCALEX, 0.008f * BODYSCALEY, -0.095f * BODYSCALEZ,
    -0.283f * BODYSCALEX, 0.209f * BODYSCALEY, -0.016f * BODYSCALEZ,
    -0.485f * BODYSCALEX, 0.217f * BODYSCALEY, -0.063f * BODYSCALEZ,
    0.677f * BODYSCALEX, 0.425f * BODYSCALEY, -0.095f * BODYSCALEZ,
    0.496f * BODYSCALEX, 0.433f * BODYSCALEY, -0.016f * BODYSCALEZ,
    0.491f * BODYSCALEX, 0.217f * BODYSCALEY, -0.063f * BODYSCALEZ,
    0.990f * BODYSCALEX, 0.420f * BODYSCALEY, -0.741f * BODYSCALEZ,
    1.000f * BODYSCALEX, -0.466f * BODYSCALEY, -0.730f * BODYSCALEZ,
    -0.300f * BODYSCALEX, -0.954f * BODYSCALEY, -0.638f * BODYSCALEZ,
    0.312f * BODYSCALEX, -0.964f * BODYSCALEY, -0.565f * BODYSCALEZ,
    0.338f * BODYSCALEX, 0.879f * BODYSCALEY, -0.865f * BODYSCALEZ,
    -0.326f * BODYSCALEX, 0.882f * BODYSCALEY, -0.870f * BODYSCALEZ,
    -0.976f * BODYSCALEX, -0.450f * BODYSCALEY, -0.747f * BODYSCALEZ,
    -0.973f * BODYSCALEX, 0.421f * BODYSCALEY, -0.742f * BODYSCALEZ,
    0.034f * BODYSCALEX, 0.410f * BODYSCALEY, -1.000f * BODYSCALEZ,
    0.024f * BODYSCALEX, -0.445f * BODYSCALEY, -0.911f * BODYSCALEZ,
    -0.244f * BODYSCALEX, 0.410f * BODYSCALEY, 0.000f * BODYSCALEZ,
    -0.469f * BODYSCALEX, 0.418f * BODYSCALEY, -0.016f * BODYSCALEZ,
    -0.648f * BODYSCALEX, 0.425f * BODYSCALEY, -0.095f * BODYSCALEZ,
    0.315f * BODYSCALEX, 0.441f * BODYSCALEY, 0.000f * BODYSCALEZ,
    0.310f * BODYSCALEX, 0.224f * BODYSCALEY, -0.016f * BODYSCALEZ,
    0.305f * BODYSCALEX, 0.008f * BODYSCALEY, -0.095f * BODYSCALEZ,
};
static const int high_num_body_vertices = 32;

static GLfloat high_body_normals[] = {
  -0.0213f, 0.3526f, 0.9355f,
  -0.2868f, 0.9020f, 0.3226f,
  -0.7789f, 0.4106f, 0.4740f,
  0.2291f, 0.9381f, 0.2599f,
  0.4979f, 0.4089f, 0.7648f,
  -0.2912f, -0.7440f, 0.6013f,
  -0.0098f, -0.0921f, 0.9957f,
  -0.5942f, -0.4448f, 0.6701f,
  0.2479f, -0.8758f, 0.4141f,
  0.7736f, -0.2321f, 0.5896f,
  -0.2247f, -0.3642f, 0.9038f,
  -0.1777f, -0.2135f, 0.9607f,
  -0.2878f, -0.2505f, 0.9243f,
  0.3839f, -0.2048f, 0.9004f,
  0.2379f, -0.1656f, 0.9571f,
  0.2833f, -0.2532f, 0.9250f,
  0.8199f, 0.3902f, -0.4189f,
  0.6244f, -0.4863f, -0.6113f,
  -0.2542f, -0.8805f, -0.4000f,
  0.2345f, -0.8085f, -0.5397f,
  0.2754f, 0.7105f, -0.6476f,
  -0.2139f, 0.8167f, -0.5360f,
  -0.7774f, -0.3176f, -0.5429f,
  -0.6388f, 0.3607f, -0.6796f,
  0.0368f, 0.0426f, -0.9984f,
  -0.0235f, -0.2762f, -0.9608f,
  -0.0731f, -0.0650f, 0.9952f,
  -0.2380f, -0.1508f, 0.9595f,
  -0.4031f, -0.1778f, 0.8977f,
  0.0845f, -0.0752f, 0.9936f,
  0.1838f, -0.2106f, 0.9601f,
  0.2241f, -0.3393f, 0.9136f,
};

static GLfloat high_body_colours[] = {
  // body
  0.60f, 0.60f, 0.55f,
  0.60f, 0.60f, 0.55f,
  0.60f, 0.60f, 0.55f,
  0.60f, 0.60f, 0.55f,
  0.60f, 0.60f, 0.55f,
  0.60f, 0.60f, 0.55f,
  0.60f, 0.60f, 0.55f,
  0.60f, 0.60f, 0.55f,
  0.60f, 0.60f, 0.55f,
  0.60f, 0.60f, 0.55f,

  // eyes
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,

  // more body
  0.60f, 0.60f, 0.55f,
  0.60f, 0.60f, 0.55f,
  0.60f, 0.60f, 0.55f,
  0.60f, 0.60f, 0.55f,
  0.60f, 0.60f, 0.55f,
  0.60f, 0.60f, 0.55f,
  0.60f, 0.60f, 0.55f,
  0.60f, 0.60f, 0.55f,
  0.60f, 0.60f, 0.55f,
  0.60f, 0.60f, 0.55f,

  // more eyes
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
  0.80f, 0.00f, 0.00f,
};

static GLushort high_body_indices[] = {
    0, 2, 1,
    0, 1, 3,
    4, 0, 3,
    5, 7, 6,
    8, 5, 6,
    9, 8, 6,
    7, 2, 0,
    6, 7, 0,
    4, 6, 0,
    6, 4, 9,
    10, 12, 11,
    13, 15, 14,
    4, 3, 16,
    4, 16, 9,
    17, 9, 16,
    17, 8, 9,
    8, 18, 5,
    18, 8, 19,
    19, 8, 17,
    3, 21, 20,
    21, 3, 1,
    3, 20, 16,
    22, 2, 7,
    2, 22, 23,
    5, 18, 7,
    22, 7, 18,
    23, 1, 2,
    1, 23, 21,
    24, 16, 20,
    24, 20, 21,
    23, 24, 21,
    19, 17, 25,
    18, 19, 25,
    22, 18, 25,
    17, 16, 24,
    25, 17, 24,
    23, 25, 24,
    25, 23, 22,
    26, 11, 27,
    28, 27, 12,
    11, 12, 27,
    29, 14, 30,
    31, 30, 15,
    14, 15, 30,
};
static const int high_num_body_indices = 44 * 3;


static GLfloat high_horn_l_vertices[] = {
    -0.011f * HORNSCALEX, -0.006f * HORNSCALEY, -0.833f * HORNSCALEZ,
    -0.686f * HORNSCALEX, -0.329f * HORNSCALEY, -0.833f * HORNSCALEZ,
    -0.693f * HORNSCALEX, 0.352f * HORNSCALEY, -0.500f * HORNSCALEZ,
    -1.000f * HORNSCALEX, 0.006f * HORNSCALEY, -0.500f * HORNSCALEZ,
    -0.994f * HORNSCALEX, 1.000f * HORNSCALEY, 0.000f * HORNSCALEZ,
    -0.245f * HORNSCALEX, -0.121f * HORNSCALEY, -0.196f * HORNSCALEZ,
    -0.823f * HORNSCALEX, 0.188f * HORNSCALEY, -0.205f * HORNSCALEZ,
};

static GLfloat high_horn_r_vertices[] = {
    0.011f * HORNSCALEX, -0.006f * HORNSCALEY, -0.833f * HORNSCALEZ,
    0.686f * HORNSCALEX, -0.329f * HORNSCALEY, -0.833f * HORNSCALEZ,
    0.693f * HORNSCALEX, 0.352f * HORNSCALEY, -0.500f * HORNSCALEZ,
    1.000f * HORNSCALEX, 0.006f * HORNSCALEY, -0.500f * HORNSCALEZ,
    0.994f * HORNSCALEX, 1.000f * HORNSCALEY, 0.000f * HORNSCALEZ,
    0.245f * HORNSCALEX, -0.121f * HORNSCALEY, -0.196f * HORNSCALEZ,
    0.823f * HORNSCALEX, 0.188f * HORNSCALEY, -0.205f * HORNSCALEZ,
};

static const int high_num_horn_vertices = 7;

static GLfloat high_horn_l_normals[] = {
  0.7914f, 0.2797f, -0.5436f,
  -0.4321f, -0.8500f, -0.3012f,
  0.2390f, 0.8994f, -0.3660f,
  -0.9662f, -0.1563f, -0.2049f,
  -0.6601f, 0.4089f, 0.6302f,
  0.5558f, -0.0971f, 0.8256f,
  -0.0162f, -0.3494f, 0.9368f,
};

static GLfloat high_horn_r_normals[] = {
  -0.7914f, 0.2797f, -0.5436f,
  0.4321f, -0.8500f, -0.3012f,
  -0.2390f, 0.8994f, -0.3660f,
  0.9662f, -0.1563f, -0.2049f,
  0.6601f, 0.4089f, 0.6302f,
  -0.5558f, -0.0971f, 0.8256f,
  0.0162f, -0.3494f, 0.9368f,
};


static GLfloat high_horn_colours[] = {
  0.80f, 0.85f, 0.35f,
  0.80f, 0.85f, 0.35f,
  0.80f, 0.85f, 0.35f,
  0.80f, 0.85f, 0.35f,
  0.80f, 0.85f, 0.35f,
  0.80f, 0.85f, 0.35f,
  0.80f, 0.85f, 0.35f,
};

static GLushort high_horn_l_indices[] = {
    0, 2, 1,
    2, 3, 1,
    2, 4, 3,
    1, 5, 0,
    5, 2, 0,
    6, 2, 5,
    1, 6, 5,
    6, 1, 3,
    2, 6, 4,
    6, 3, 4,
};

static GLushort high_horn_r_indices[] = {
    2, 0, 1,
    3, 2, 1,
    4, 2, 3,
    5, 1, 0,
    2, 5, 0,
    2, 6, 5,
    6, 1, 5,
    1, 6, 3,
    6, 2, 4,
    3, 6, 4,
};

static const int high_num_horn_indices = 10 * 3;


static GLfloat high_tooth_l_vertices[] = {
    -1.000f * TOOTHSCALEX, 0.000f * TOOTHSCALEY, -1.000f * TOOTHSCALEZ,
    0.000f * TOOTHSCALEX, 0.001f * TOOTHSCALEY, -1.000f * TOOTHSCALEZ,
    -0.033f * TOOTHSCALEX, -1.000f * TOOTHSCALEY, -0.365f * TOOTHSCALEZ,
    -0.500f * TOOTHSCALEX, -0.004f * TOOTHSCALEY, -0.452f * TOOTHSCALEZ,
};

static GLfloat high_tooth_r_vertices[] = {
    1.000f * TOOTHSCALEX, 0.000f * TOOTHSCALEY, -1.000f * TOOTHSCALEZ,
    0.000f * TOOTHSCALEX, 0.001f * TOOTHSCALEY, -1.000f * TOOTHSCALEZ,
    0.033f * TOOTHSCALEX, -1.000f * TOOTHSCALEY, -0.365f * TOOTHSCALEZ,
    0.500f * TOOTHSCALEX, -0.004f * TOOTHSCALEY, -0.452f * TOOTHSCALEZ,
};

static const int high_num_tooth_vertices = 4;

static GLfloat high_tooth_l_normals[] = {
  -0.9364f, 0.2468f, -0.2496f,
  0.6195f, 0.7616f, -0.1901f,
  -0.0444f, -0.7134f, 0.6993f,
  -0.0170f, 0.6506f, 0.7592f,
};

static GLfloat high_tooth_r_normals[] = {
  0.9364f, 0.2468f, -0.2496f,
  -0.6195f, 0.7616f, -0.1901f,
  0.0444f, -0.7134f, 0.6993f,
  0.0170f, 0.6506f, 0.7592f,
};

static GLfloat high_tooth_colours[] = {
  0.85f, 0.75f, 0.90f,
  0.85f, 0.75f, 0.90f,
  0.85f, 0.75f, 0.90f,
  0.85f, 0.75f, 0.90f,
};

static GLushort high_tooth_l_indices[] = {
    0, 2, 1,
    1, 3, 0,
    2, 3, 1,
    3, 2, 0,
};

static GLushort high_tooth_r_indices[] = {
    2, 0, 1,
    3, 1, 0,
    3, 2, 1,
    2, 3, 0,
};

static const int high_num_tooth_indices = 4 * 3;



#endif /* _FOE_DEMONHEAD_DATA_H_ */
