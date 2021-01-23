#ifndef _STAGE_0097_H_
#define _STAGE_0097_H_

static stage_t stage_0097 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 16,
  .coords = {
    -0.196f, 1.000f,
    0.202f, 0.999f,
    0.569f, 0.847f,
    0.849f, 0.567f,
    1.000f, 0.202f,
    0.999f, -0.192f,
    0.845f, -0.555f,
    0.563f, -0.833f,
    0.196f, -0.983f,
    -0.202f, -0.982f,
    -0.569f, -0.830f,
    -0.849f, -0.550f,
    -1.000f, -0.186f,
    -0.999f, 0.209f,
    -0.845f, 0.572f,
    -0.563f, 0.850f,
  },
  .move_colours = 1,
  .colours = {0.00f, 0.25f, 0.65f,
	      0.00f, 0.15f, 0.45f},
  .line_colours = {0.90f, 0.90f, 0.90f, 0.40f, 0.40f, 0.40f},
  .start_position = 8,

  .mutant_flipper_rnd = 1,
  .mutant_tanker_rnd = 1,
  .spiker_num = 1,
  .spiker_delay = 1000,

  .stage_length = 60000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 40,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_PULSAR,
      .initial_entry = 0,
      .jitter = 200,
      .count = 1,
    },
    {
      .type = STAGE_E_FOE_MIRROR,
      .initial_entry = 1200,
      .jitter = 200,
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_UFO,
      .initial_entry = 1200,
      .jitter = 200,
      .count = 6,
    },
    {
      .type = STAGE_E_FOE_TANKER_PULSAR,
      .initial_entry = 1300,
      .jitter = 200,
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_TANKER_FUSEBALL,
      .initial_entry = 5000,
      .jitter = 200,
      .count = 1,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
