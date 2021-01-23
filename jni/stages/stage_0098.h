#ifndef _STAGE_0098_H_
#define _STAGE_0098_H_

static stage_t stage_0098 __attribute__((unused)) = {
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
  .line_colours = {0.90f, 0.90f, 0.90f, 0.40f, 0.40f, 0.40f},
  .start_position = 10,

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
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 4,
    },
    {
      .type = STAGE_E_FOE_PULSAR,
      .initial_entry = 0,
      .jitter = 200,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_MIRROR,
      .initial_entry = 1200,
      .jitter = 200,
      .count = 6,
    },
    {
      .type = STAGE_E_FOE_UFO,
      .initial_entry = 500,
      .jitter = 200,
      .count = 6,
    },
    {
      .type = STAGE_E_FOE_TANKER_PULSAR,
      .initial_entry = 1300,
      .jitter = 200,
      .count = 4,
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
