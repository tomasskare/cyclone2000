#ifndef _STAGE_0034_H_
#define _STAGE_0034_H_

static stage_t stage_0034 __attribute__((unused)) = {
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
  .line_colours = {0.90f, 0.90f, 0.00f, 0.40f, 0.40f, 0.00f},
  .start_position = 10,

  .mutant_flipper_rnd = 10,
  .spiker_num = 1,
  .spiker_delay = 1000,

  .stage_length = 30000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 20,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 0,
      .jitter = 500,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 4000,
      .jitter = 500,
      .count = 3,
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
