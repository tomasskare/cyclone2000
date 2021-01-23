#ifndef _STAGE_0099_H_
#define _STAGE_0099_H_

static stage_t stage_0099 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 12,
  .coords = {
    2.000f, -1.500f,
    1.636f, -1.500f,
    1.273f, -1.500f,
    0.909f, -1.500f,
    0.545f, -1.500f,
    0.182f, -1.500f,
    -0.182f, -1.500f,
    -0.545f, -1.500f,
    -0.909f, -1.500f,
    -1.273f, -1.500f,
    -1.636f, -1.500f,
    -2.000f, -1.500f,
  },
  .move_colours = 0,
  .colours = {0.00f, 0.15f, 0.70f,
	      0.25f, 0.40f, 0.65f},
  .line_colours = {0.90f, 0.90f, 0.90f, 0.40f, 0.40f, 0.40f},
  .start_position = 5,

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
      .count = 4,
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
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_MIRROR,
      .initial_entry = 1200,
      .jitter = 200,
      .count = 7,
    },
    {
      .type = STAGE_E_FOE_UFO,
      .initial_entry = 500,
      .jitter = 200,
      .count = 7,
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
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 10000,
      .jitter = 200,
      .count = 1,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
