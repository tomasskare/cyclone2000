#ifndef _STAGE_0036_H_
#define _STAGE_0036_H_

static stage_t stage_0036 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 16,
  .coords = {
    1.570f, 1.300f,
    1.370f, 0.929f,
    1.170f, 0.557f,
    0.970f, 0.186f,
    0.770f, -0.186f,
    0.570f, -0.557f,
    0.370f, -0.929f,
    0.170f, -1.300f,
    -0.170f, -1.300f,
    -0.370f, -0.929f,
    -0.570f, -0.557f,
    -0.770f, -0.186f,
    -0.970f, 0.186f,
    -1.170f, 0.557f,
    -1.370f, 0.929f,
    -1.570f, 1.300f,
  },
  .move_colours = 0,
  .colours = {0.05f, 0.00f, 0.65f,
	      0.40f, 0.00f, 0.60f,
	      0.05f, 0.00f, 0.65f},
  .line_colours = {0.90f, 0.90f, 0.00f, 0.40f, 0.40f, 0.00f},
  .start_position = 7,

  .mutant_tanker_rnd = 7,
  .mutant_flipper_rnd = 10,
  .spiker_num = 1,
  .spiker_delay = 1000,

  .stage_length = 32000,
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
      .count = 3,
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
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_TANKER_PULSAR,
      .initial_entry = 0,
      .jitter = 1000,
      .count = 3,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
