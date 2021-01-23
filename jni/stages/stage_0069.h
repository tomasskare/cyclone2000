#ifndef _STAGE_0069_H_
#define _STAGE_0069_H_

static stage_t stage_0069 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 18,
  .coords = {
    0.000f, 1.000f,
    0.167f, 0.667f,
    0.333f, 0.333f,
    0.500f, 0.000f,
    0.667f, -0.333f,
    0.833f, -0.667f,
    1.000f, -1.000f,
    0.667f, -1.000f,
    0.333f, -1.000f,
    -0.000f, -1.000f,
    -0.333f, -1.000f,
    -0.667f, -1.000f,
    -1.000f, -1.000f,
    -0.833f, -0.667f,
    -0.667f, -0.333f,
    -0.500f, 0.000f,
    -0.333f, 0.333f,
    -0.167f, 0.667f,
  },
  .move_colours = 0,
  .colours = {0.05f, 0.00f, 0.65f,
	      0.40f, 0.00f, 0.60f},
  .line_colours = {0.90f, 0.00f, 0.60f, 0.40f, 0.00f, 0.30f},
  .start_position = 9,

  .mutant_flipper_rnd = 1,

  .stage_length = 35000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 45,
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
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_TANKER_PULSAR,
      .initial_entry = 1500,
      .jitter = 500,
      .count = 5,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
