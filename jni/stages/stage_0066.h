#ifndef _STAGE_0066_H_
#define _STAGE_0066_H_

static stage_t stage_0066 __attribute__((unused)) = {
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
  .move_colours = 0,
  .colours = {0.00f, 0.15f, 0.70f,
	      0.25f, 0.40f, 0.65f},
  .line_colours = {0.90f, 0.00f, 0.60f, 0.40f, 0.00f, 0.30f},
  .start_position = 10,

  .mutant_flipper_rnd = 1,

  .stage_length = 35000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 43,
    },
    {
      .type = STAGE_E_FOE_UFO,
      .initial_entry = 2000,
      .jitter = 500,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_MIRROR,
      .initial_entry = 3000,
      .jitter = 500,
      .count = 1,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
