#ifndef _STAGE_0002_H_
#define _STAGE_0002_H_

static stage_t stage_0002 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 16,
  .coords = {
    -0.700f,  0.700f,
    -0.350f,  0.700f,
    0.000f,  0.700f,
    0.350f,  0.700f,
    0.700f,  0.700f,
    0.700f,  0.350f,
    0.700f,  0.000f,
    0.700f, -0.350f,
    0.700f, -0.700f,
    0.350f, -0.700f,
    0.000f, -0.700f,
    -0.350f, -0.700f,
    -0.700f, -0.700f,
    -0.700f, -0.350f,
    -0.700f,  0.000f,
    -0.700f,  0.350f,
  },
  .move_colours = 1,
  .colours = {0.00f, 0.15f, 0.65f,
	      0.30f, 0.35f, 0.50f},
  .line_colours = {0.15f, 0.15f, 0.65f, 0.05f, 0.05f, 0.40f},
  .start_position = 10,

  .stage_length = 40000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 300,
      .count = 23,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
