#ifndef _STAGE_EMPTY_H_
#define _STAGE_EMPTY_H_

static stage_t stage_empty __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 16,
  .coords = {
    0.000f,  1.000f,
    0.383f,  0.925f,
    0.707f,  0.707f,
    0.925f,  0.383f,
    1.000f,  0.000f,
    0.925f, -0.383f,
    0.707f, -0.707f,
    0.383f, -0.925f,
    0.000f, -1.000f,
    -0.383f, -0.925f,
    -0.707f, -0.707f,
    -0.925f, -0.383f,
    -1.000f,  0.000f,
    -0.925f,  0.383f,
    -0.707f,  0.707f,
    -0.383f,  0.925f,
  },
  .colours = {0.00f, 0.15f, 0.65f, 0.00f, 0.35f, 0.45f},
  .line_colours = {0.15f, 0.15f, 0.65f, 0.05f, 0.05f, 0.40f},
  .start_position = 8,

  .steps = {
    {
      .entry = 0,
      .jitter = 0,
      .event = STAGE_E_END,
    }
  }
};

#endif
