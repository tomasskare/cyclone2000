#ifndef _FOE_PULSAR_DATA_H_
#define _FOE_PULSAR_DATA_H_

#define SCALEX 0.140f
#define SCALEY 0.140f
#define SCALEZ 0.040f

#define SCALEY_FLAT 0.060f

static GLfloat low_vertices_pulse[] = {
    0.000f * SCALEX, 1.000f * SCALEY, -0.500f * SCALEZ,
    0.333f * SCALEX, 0.000f * SCALEY, -0.500f * SCALEZ,
    0.000f * SCALEX, 0.500f * SCALEY, 0.000f * SCALEZ,
    -0.333f * SCALEX, 0.000f * SCALEY, -0.500f * SCALEZ,
    -0.666f * SCALEX, -1.000f * SCALEY, -0.500f * SCALEZ,
    -0.666f * SCALEX, -0.500f * SCALEY, 0.000f * SCALEZ,
    -1.000f * SCALEX, 0.000f * SCALEY, -0.500f * SCALEZ,
    1.000f * SCALEX, 0.000f * SCALEY, -0.500f * SCALEZ,
    0.666f * SCALEX, -1.000f * SCALEY, -0.500f * SCALEZ,
    0.666f * SCALEX, -0.500f * SCALEY, 0.000f * SCALEZ,
};

static GLfloat low_vertices_flat[] = {
    0.000f * SCALEX, 1.000f * SCALEY_FLAT, -0.500f * SCALEZ,
    0.333f * SCALEX, 0.000f * SCALEY_FLAT, -0.500f * SCALEZ,
    0.000f * SCALEX, -1.000f * SCALEY_FLAT, 0.000f * SCALEZ,
    -0.333f * SCALEX, 0.000f * SCALEY_FLAT, -0.500f * SCALEZ,
    -0.666f * SCALEX, -1.000f * SCALEY_FLAT, -0.500f * SCALEZ,
    -0.666f * SCALEX, 1.000f * SCALEY_FLAT, 0.000f * SCALEZ,
    -1.000f * SCALEX, 0.000f * SCALEY_FLAT, -0.500f * SCALEZ,
    1.000f * SCALEX, 0.000f * SCALEY_FLAT, -0.500f * SCALEZ,
    0.666f * SCALEX, -1.000f * SCALEY_FLAT, -0.500f * SCALEZ,
    0.666f * SCALEX, 1.000f * SCALEY_FLAT, 0.000f * SCALEZ,
};

static const int low_num_vertices = 10;

static GLushort low_indices[] = {
    0, 1, 2,
    2, 3, 0,
    3, 4, 5,
    4, 6, 5,
    7, 8, 9,
    8, 1, 9,
};
static const int low_num_indices = 6 * 3;

static GLfloat low_colours[] = {
  0.90f, 0.90f, 0.00f,
  0.70f, 0.70f, 0.00f,
  0.85f, 0.85f, 0.00f,
  0.70f, 0.70f, 0.00f,
  0.50f, 0.40f, 0.00f,
  0.55f, 0.45f, 0.00f,
  0.30f, 0.15f, 0.00f,
  0.30f, 0.15f, 0.00f,
  0.50f, 0.40f, 0.00f,
  0.55f, 0.45f, 0.00f,
};


static GLfloat high_vertices_pulse[] = {
    -0.002f * SCALEX, 1.000f * SCALEY, -0.500f * SCALEZ,
    0.324f * SCALEX, 0.008f * SCALEY, -0.500f * SCALEZ,
    0.015f * SCALEX, 0.504f * SCALEY, 0.000f * SCALEZ,
    -0.335f * SCALEX, 0.000f * SCALEY, -0.500f * SCALEZ,
    -0.665f * SCALEX, -0.983f * SCALEY, -0.500f * SCALEZ,
    -0.666f * SCALEX, -0.513f * SCALEY, -0.038f * SCALEZ,
    -1.000f * SCALEX, 0.008f * SCALEY, -0.500f * SCALEZ,
    1.000f * SCALEX, 0.008f * SCALEY, -0.500f * SCALEZ,
    0.665f * SCALEX, -1.000f * SCALEY, -0.500f * SCALEZ,
    0.659f * SCALEX, -0.521f * SCALEY, -0.038f * SCALEZ,
    -0.662f * SCALEX, -0.421f * SCALEY, -0.250f * SCALEZ,
    -0.000f * SCALEX, 0.408f * SCALEY, -0.250f * SCALEZ,
    0.662f * SCALEX, -0.408f * SCALEY, -0.250f * SCALEZ,
};

static GLfloat high_vertices_flat[] = {
    -0.002f * SCALEX, 1.000f * SCALEY_FLAT, -0.500f * SCALEZ,
    0.324f * SCALEX, 0.000f * SCALEY_FLAT, -0.500f * SCALEZ,
    0.015f * SCALEX, -1.000f * SCALEY_FLAT, 0.000f * SCALEZ,
    -0.335f * SCALEX, 0.000f * SCALEY_FLAT, -0.500f * SCALEZ,
    -0.665f * SCALEX, -1.000f * SCALEY_FLAT, -0.500f * SCALEZ,
    -0.666f * SCALEX, 1.000f * SCALEY_FLAT, -0.038f * SCALEZ,
    -1.000f * SCALEX, 0.000f * SCALEY_FLAT, -0.500f * SCALEZ,
    1.000f * SCALEX, 0.000f * SCALEY_FLAT, -0.500f * SCALEZ,
    0.665f * SCALEX, -1.000f * SCALEY_FLAT, -0.500f * SCALEZ,
    0.659f * SCALEX, 1.000f * SCALEY_FLAT, -0.038f * SCALEZ,
    -0.662f * SCALEX, 0.900f * SCALEY_FLAT, -0.250f * SCALEZ,
    -0.000f * SCALEX, -0.900f * SCALEY_FLAT, -0.250f * SCALEZ,
    0.662f * SCALEX, 0.900f * SCALEY_FLAT, -0.250f * SCALEZ,
};

static const int high_num_vertices = 13;

static GLfloat high_normals_pulse[] = {
  -0.0148f, -0.1572f, 0.9875f,
  -0.7774f, -0.2812f, 0.5626f,
  -0.0459f, 0.5796f, -0.8136f,
  0.0257f, 0.1754f, 0.9842f,
  0.0187f, 0.1594f, 0.9870f,
  0.0127f, -0.5368f, -0.8436f,
  -0.6199f, -0.7838f, -0.0375f,
  0.5650f, -0.8181f, -0.1073f,
  -0.5382f, -0.1142f, 0.8350f,
  0.0096f, -0.5082f, -0.8612f,
  0.0146f, -0.9582f, 0.2858f,
  -0.0175f, 0.9455f, 0.3252f,
  -0.2957f, -0.8618f, 0.4122f,
};

static GLfloat high_normals_flat[] = {
  -0.0567f, -0.4646f, 0.8837f,
  -0.2423f, -0.0864f, 0.9663f,
  -0.0024f, 0.2748f, -0.9615f,
  0.0676f, -0.0283f, 0.9973f,
  0.0114f, 0.4882f, 0.8727f,
  -0.0155f, -0.3020f, -0.9532f,
  0.9228f, -0.2760f, 0.2687f,
  -0.9314f, -0.2736f, 0.2400f,
  -0.2952f, 0.0074f, 0.9554f,
  -0.0061f, -0.3074f, -0.9516f,
  -0.0033f, -0.4166f, 0.9091f,
  0.0137f, 0.4208f, 0.9070f,
  -0.1081f, -0.3404f, 0.9340f,
};

static GLushort high_indices[] = {
    0, 2, 1,
    2, 0, 3,
    3, 5, 4,
    4, 5, 6,
    7, 9, 8,
    8, 9, 1,
    5, 10, 6,
    3, 10, 5,
    11, 2, 3,
    11, 1, 2,
    12, 1, 9,
    7, 12, 9,
    1, 12, 8,
    12, 7, 8,
    1, 12, 8,
    11, 0, 1,
    0, 11, 3,
    10, 3, 4,
    10, 4, 6,
};
static const int high_num_indices = 19 * 3;

static GLfloat high_colours[] = {
  0.85f, 0.85f, 0.00f,
  0.85f, 0.85f, 0.00f,
  0.85f, 0.85f, 0.00f,
  0.85f, 0.85f, 0.00f,
  0.85f, 0.85f, 0.00f,
  0.85f, 0.85f, 0.00f,
  0.85f, 0.85f, 0.00f,
  0.85f, 0.85f, 0.00f,
  0.85f, 0.85f, 0.00f,
  0.85f, 0.85f, 0.00f,
  0.85f, 0.85f, 0.00f,
  0.85f, 0.85f, 0.00f,
  0.85f, 0.85f, 0.00f,
};



#endif /* _FOE_PULSAR_DATA_H_ */
