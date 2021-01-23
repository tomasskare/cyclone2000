#ifndef _STAGE_0037_H_
#define _STAGE_0037_H_

static stage_t stage_0037 __attribute__((unused)) = {
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
  .line_colours = {0.90f, 0.90f, 0.00f, 0.40f, 0.40f, 0.00f},
  .start_position = 9,

  .mutant_tanker_rnd = 7,
  .mutant_flipper_rnd = 10,
  .spiker_num = 1,
  .spiker_delay = 1000,

  .stage_length = 35000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 20,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 4,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 0,
      .jitter = 500,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 2000,
      .jitter = 500,
      .count = 1,
    },
    {
      .type = STAGE_E_FOE_TANKER_PULSAR,
      .initial_entry = 0,
      .jitter = 1000,
      .count = 4,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
