#ifndef _PLAYER_DATA_H_
#define _PLAYER_DATA_H_

#define SCALEX 0.16f
#define SCALEY 0.16f
#define SCALEZ 0.16f

static GLfloat low_vertices[] = {
  1.000f * SCALEX, -0.067f * SCALEY, -0.500f * SCALEZ,
  0.753f * SCALEX,  0.039f * SCALEY,  0.000f * SCALEZ,
  0.008f * SCALEX, -1.000f * SCALEY, -0.500f * SCALEZ,
  0.009f * SCALEX, -0.660f * SCALEY,  0.000f * SCALEZ,
  0.507f * SCALEX,  0.146f * SCALEY, -0.500f * SCALEZ,
  0.010f * SCALEX, -0.320f * SCALEY, -0.500f * SCALEZ,
  -0.977f * SCALEX, -0.054f * SCALEY, -0.500f * SCALEZ,
  -0.730f * SCALEX,  0.049f * SCALEY,  0.000f * SCALEZ,
  -0.482f * SCALEX,  0.153f * SCALEY, -0.500f * SCALEZ,
  -0.290f * SCALEX,  0.911f * SCALEY, -0.500f * SCALEZ,
  0.321f * SCALEX,  0.907f * SCALEY, -0.500f * SCALEZ,
};
static int low_num_vertices = 11;

static GLfloat low_colours[] = {
  0.75f, 0.65f, 0.00f, // outer right joint
  0.80f, 0.70f, 0.00f, // middle right joint
  0.80f, 0.40f, 0.00f, // outer center
  0.70f, 0.30f, 0.00f, // middle center
  0.50f, 0.40f, 0.00f, // inner right joint
  0.50f, 0.30f, 0.00f, // inner center
  0.75f, 0.65f, 0.00f, // outer left joint
  0.80f, 0.70f, 0.00f, // middle left joint
  0.50f, 0.40f, 0.00f, // inner left joint
  0.50f, 0.40f, 0.20f, // left tip
  0.50f, 0.40f, 0.20f, // right tip
};

static GLushort low_indices[] = {
  0, 2, 1,
  1, 2, 3,
  1, 3, 4,
  4, 3, 5,
  2, 6, 3,
  3, 6, 7,
  3, 7, 5,
  5, 7, 8,
  6, 9, 7,
  7, 9, 8,
  10, 0, 1,
  10, 1, 4,
};
static int low_num_indices = 12 * 3;



static GLfloat high_vertices[] = {
    1.000f * SCALEX, -0.067f * SCALEY, -0.500f * SCALEZ,
    0.753f * SCALEX, 0.039f * SCALEY, 0.000f * SCALEZ,
    0.008f * SCALEX, -1.000f * SCALEY, -0.500f * SCALEZ,
    0.009f * SCALEX, -0.660f * SCALEY, 0.000f * SCALEZ,
    0.507f * SCALEX, 0.146f * SCALEY, -0.500f * SCALEZ,
    0.010f * SCALEX, -0.320f * SCALEY, -0.500f * SCALEZ,
    0.753f * SCALEX, 0.039f * SCALEY, -1.000f * SCALEZ,
    0.009f * SCALEX, -0.660f * SCALEY, -1.000f * SCALEZ,
    -0.977f * SCALEX, -0.054f * SCALEY, -0.500f * SCALEZ,
    -0.730f * SCALEX, 0.049f * SCALEY, 0.000f * SCALEZ,
    -0.482f * SCALEX, 0.153f * SCALEY, -0.500f * SCALEZ,
    -0.730f * SCALEX, 0.049f * SCALEY, -1.000f * SCALEZ,
    -0.290f * SCALEX, 0.911f * SCALEY, -0.500f * SCALEZ,
    0.321f * SCALEX, 0.907f * SCALEY, -0.500f * SCALEZ,
};
static int high_num_vertices = 14;

static GLfloat high_normals[] = {
  0.9623f, -0.2419f, -0.1245f,
  0.2449f, -0.1543f, 0.9572f,
  -0.0056f, -1.0000f, 0.0004f,
  -0.4223f, 0.0023f, 0.9065f,
  -0.9160f, 0.3852f, 0.1120f,
  0.0062f, 1.0000f, 0.0004f,
  -0.3071f, 0.4219f, -0.8530f,
  0.4223f, -0.0024f, -0.9064f,
  -0.9647f, -0.2323f, 0.1240f,
  0.3117f, 0.4190f, 0.8528f,
  0.9200f, 0.3758f, -0.1113f,
  -0.2476f, -0.1515f, -0.9569f,
  0.3057f, 0.9521f, 0.0000f,
  -0.2996f, 0.9541f, 0.0000f,
};

static GLfloat high_colours[] = {
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.20f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.30f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
};

static GLushort high_indices[] = {
    0, 2, 1,
    1, 2, 3,
    1, 3, 4,
    4, 3, 5,
    4, 5, 6,
    6, 5, 7,
    6, 7, 0,
    0, 7, 2,
    2, 8, 3,
    3, 8, 9,
    3, 9, 5,
    5, 9, 10,
    5, 10, 7,
    7, 10, 11,
    7, 11, 2,
    2, 11, 8,
    8, 12, 9,
    9, 12, 10,
    10, 12, 11,
    11, 12, 8,
    13, 0, 1,
    13, 1, 4,
    13, 4, 6,
    13, 6, 0,
};
static int high_num_indices = 24 * 3;



#if 0

static GLfloat high_vertices[] = {
    0.998f * SCALEX, -0.086f * SCALEY, -0.467f * SCALEZ,
    0.956f * SCALEX, -0.014f * SCALEY, -0.262f * SCALEZ,
    0.604f * SCALEX, -0.620f * SCALEY, -0.504f * SCALEZ,
    0.822f * SCALEX, 0.040f * SCALEY, -0.023f * SCALEZ,
    0.471f * SCALEX, -0.391f * SCALEY, -0.027f * SCALEZ,
    0.470f * SCALEX, -0.566f * SCALEY, -0.266f * SCALEZ,
    0.687f * SCALEX, 0.095f * SCALEY, -0.262f * SCALEZ,
    0.552f * SCALEX, 0.150f * SCALEY, -0.500f * SCALEZ,
    0.323f * SCALEX, -0.138f * SCALEY, -0.504f * SCALEZ,
    0.322f * SCALEX, -0.312f * SCALEY, -0.266f * SCALEZ,
    0.687f * SCALEX, 0.095f * SCALEY, -0.738f * SCALEZ,
    0.822f * SCALEX, 0.040f * SCALEY, -0.977f * SCALEZ,
    0.457f * SCALEX, -0.367f * SCALEY, -0.981f * SCALEZ,
    0.457f * SCALEX, -0.193f * SCALEY, -0.742f * SCALEZ,
    0.956f * SCALEX, -0.014f * SCALEY, -0.738f * SCALEZ,
    0.605f * SCALEX, -0.446f * SCALEY, -0.742f * SCALEZ,
    0.008f * SCALEX, -1.000f * SCALEY, -0.504f * SCALEZ,
    0.009f * SCALEX, -0.826f * SCALEY, -0.266f * SCALEZ,
    -0.588f * SCALEX, -0.565f * SCALEY, -0.477f * SCALEZ,
    0.010f * SCALEX, -0.676f * SCALEY, -0.023f * SCALEZ,
    -0.452f * SCALEX, -0.338f * SCALEY, 0.000f * SCALEZ,
    -0.587f * SCALEX, -0.391f * SCALEY, -0.238f * SCALEZ,
    0.011f * SCALEX, -0.502f * SCALEY, -0.262f * SCALEZ,
    0.011f * SCALEX, -0.328f * SCALEY, -0.500f * SCALEZ,
    -0.295f * SCALEX, -0.168f * SCALEY, -0.473f * SCALEZ,
    -0.430f * SCALEX, -0.221f * SCALEY, -0.235f * SCALEZ,
    0.011f * SCALEX, -0.502f * SCALEY, -0.738f * SCALEZ,
    0.010f * SCALEX, -0.676f * SCALEY, -0.977f * SCALEZ,
    -0.431f * SCALEX, -0.395f * SCALEY, -0.950f * SCALEZ,
    -0.296f * SCALEX, -0.342f * SCALEY, -0.711f * SCALEZ,
    0.009f * SCALEX, -0.826f * SCALEY, -0.743f * SCALEZ,
    -0.453f * SCALEX, -0.512f * SCALEY, -0.715f * SCALEZ,
    -1.000f * SCALEX, -0.039f * SCALEY, -0.524f * SCALEZ,
    -0.931f * SCALEX, -0.003f * SCALEY, -0.262f * SCALEZ,
    -0.738f * SCALEX, 0.471f * SCALEY, -0.491f * SCALEZ,
    -0.796f * SCALEX, 0.050f * SCALEY, -0.023f * SCALEZ,
    -0.661f * SCALEX, 0.103f * SCALEY, -0.262f * SCALEZ,
    -0.603f * SCALEX, 0.524f * SCALEY, -0.253f * SCALEZ,
    -0.526f * SCALEX, 0.156f * SCALEY, -0.500f * SCALEZ,
    -0.661f * SCALEX, 0.103f * SCALEY, -0.738f * SCALEZ,
    -0.467f * SCALEX, 0.504f * SCALEY, -0.478f * SCALEZ,
    -0.796f * SCALEX, 0.050f * SCALEY, -0.977f * SCALEZ,
    -0.931f * SCALEX, -0.003f * SCALEY, -0.738f * SCALEZ,
    -0.602f * SCALEX, 0.451f * SCALEY, -0.716f * SCALEZ,
    0.350f * SCALEX, 0.930f * SCALEY, -0.500f * SCALEZ,
    0.619f * SCALEX, 0.461f * SCALEY, -0.268f * SCALEZ,
    0.767f * SCALEX, 0.431f * SCALEY, -0.515f * SCALEZ,
    0.485f * SCALEX, 0.515f * SCALEY, -0.506f * SCALEZ,
    0.632f * SCALEX, 0.485f * SCALEY, -0.753f * SCALEZ,
    -0.317f * SCALEX, 0.934f * SCALEY, -0.500f * SCALEZ,
};
static const int high_num_vertices = 50;

static GLfloat high_normals[] = {
  0.9743f, -0.2133f, -0.0719f,
  0.9060f, -0.0449f, 0.4209f,
  0.6752f, -0.7372f, -0.0262f,
  0.2553f, -0.0932f, 0.9624f,
  -0.0493f, -0.0809f, 0.9955f,
  0.5587f, -0.6667f, 0.4932f,
  -0.8201f, 0.2162f, 0.5299f,
  -0.9363f, 0.3365f, 0.1008f,
  -0.6608f, 0.7505f, 0.0037f,
  -0.5565f, 0.6524f, 0.5144f,
  -0.8259f, 0.2106f, -0.5230f,
  -0.3029f, 0.3678f, -0.8792f,
  0.0390f, 0.0709f, -0.9967f,
  -0.5824f, 0.6487f, -0.4899f,
  0.9107f, -0.0562f, -0.4091f,
  0.6031f, -0.6465f, -0.4671f,
  -0.0406f, -0.9992f, 0.0004f,
  -0.0363f, -0.8217f, 0.5688f,
  -0.6820f, -0.7308f, 0.0269f,
  -0.2981f, -0.0229f, 0.9543f,
  -0.0447f, 0.0797f, 0.9958f,
  -0.6143f, -0.6564f, 0.4379f,
  0.0386f, 0.8186f, 0.5731f,
  -0.0272f, 0.9996f, -0.0072f,
  0.6456f, 0.7636f, -0.0099f,
  0.5876f, 0.6628f, 0.4642f,
  -0.0802f, 0.8052f, -0.5876f,
  0.2535f, 0.0198f, -0.9671f,
  0.0370f, -0.0896f, -0.9953f,
  0.5617f, 0.6591f, -0.5002f,
  -0.0259f, -0.8301f, -0.5569f,
  -0.5909f, -0.6641f, -0.4580f,
  -0.9736f, -0.2152f, 0.0767f,
  -0.9029f, -0.0632f, 0.4251f,
  -0.8232f, 0.5643f, -0.0630f,
  0.3097f, 0.3821f, 0.8707f,
  0.8255f, 0.2049f, 0.5258f,
  0.0827f, 0.4146f, 0.9062f,
  0.9454f, 0.3114f, -0.0961f,
  0.8277f, 0.2182f, -0.5170f,
  0.9767f, -0.2127f, 0.0268f,
  -0.2437f, -0.1054f, -0.9641f,
  -0.8816f, -0.0529f, -0.4691f,
  0.1653f, 0.3980f, -0.9024f,
  -0.3520f, 0.9346f, 0.0515f,
  -0.1382f, 0.4096f, 0.9017f,
  0.8432f, 0.5370f, 0.0248f,
  -0.9754f, -0.2200f, -0.0152f,
  -0.0963f, 0.4149f, -0.9048f,
  0.3984f, 0.9087f, -0.1245f,
};

static GLfloat high_colours[] = {
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.20f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.30f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
  0.70f, 0.50f, 0.00f,
};

static GLushort high_indices[] = {
    0, 2, 1,
    3, 5, 4,
    3, 4, 6,
    7, 9, 8,
    7, 8, 10,
    11, 13, 12,
    11, 12, 14,
    0, 15, 2,
    16, 18, 17,
    19, 21, 20,
    19, 20, 22,
    23, 25, 24,
    23, 24, 26,
    27, 29, 28,
    27, 28, 30,
    16, 31, 18,
    32, 34, 33,
    35, 37, 36,
    38, 40, 39,
    41, 43, 42,
    44, 46, 45,
    44, 45, 47,
    44, 47, 48,
    44, 48, 46,
    3, 1, 5,
    16, 5, 2,
    1, 2, 5,
    19, 4, 17,
    16, 17, 5,
    4, 5, 17,
    7, 6, 9,
    19, 9, 4,
    6, 4, 9,
    23, 8, 22,
    19, 22, 9,
    8, 9, 22,
    11, 10, 13,
    23, 13, 8,
    10, 8, 13,
    27, 12, 26,
    23, 26, 13,
    12, 13, 26,
    0, 14, 15,
    27, 15, 12,
    14, 12, 15,
    16, 2, 30,
    27, 30, 15,
    2, 15, 30,
    19, 17, 21,
    32, 21, 18,
    17, 18, 21,
    35, 20, 33,
    32, 33, 21,
    20, 21, 33,
    23, 22, 25,
    35, 25, 20,
    22, 20, 25,
    38, 24, 36,
    35, 36, 25,
    24, 25, 36,
    27, 26, 29,
    38, 29, 24,
    26, 24, 29,
    41, 28, 39,
    38, 39, 29,
    28, 29, 39,
    16, 30, 31,
    41, 31, 28,
    30, 28, 31,
    32, 18, 42,
    41, 42, 31,
    18, 31, 42,
    35, 33, 37,
    49, 37, 34,
    33, 34, 37,
    38, 36, 40,
    49, 40, 37,
    36, 37, 40,
    41, 39, 43,
    49, 43, 40,
    39, 40, 43,
    32, 42, 34,
    49, 34, 43,
    42, 43, 34,
    3, 45, 1,
    0, 1, 46,
    45, 46, 1,
    7, 47, 6,
    3, 6, 45,
    47, 45, 6,
    11, 48, 10,
    7, 10, 47,
    48, 47, 10,
    0, 46, 14,
    11, 14, 48,
    46, 48, 14,
};
static const int high_num_indices = 96 * 3;

#endif


#endif /* _PLAYER_DATA_H_ */
