#ifndef _STAGE_0070_H_
#define _STAGE_0070_H_

static stage_t stage_0070 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 16,
  .coords = {
    1.600f, 1.100f,
    1.600f, 0.720f,
    1.600f, 0.340f,
    1.600f, -0.040f,
    1.400f, -0.420f,
    1.000f, -0.880f,
    0.600f, -1.100f,
    0.200f, -1.100f,
    -0.200f, -1.100f,
    -0.600f, -1.100f,
    -1.000f, -0.880f,
    -1.400f, -0.420f,
    -1.600f, -0.040f,
    -1.600f, 0.340f,
    -1.600f, 0.720f,
    -1.600f, 1.100f,
  },
  .move_colours = 0,
  .colours = {0.05f, 0.00f, 0.65f,
	      0.40f, 0.00f, 0.60f,
	      0.05f, 0.00f, 0.65f},
  .line_colours = {0.90f, 0.00f, 0.60f, 0.40f, 0.00f, 0.30f},
  .start_position = 7,

  .mutant_flipper_rnd = 1,

  .stage_length = 38000,
  .foes = {
    {
      .type = STAGE_E_FOE_UFO,
      .initial_entry = 2000,
      .jitter = 500,
      .count = 4,
    },
    {
      .type = STAGE_E_FOE_MIRROR,
      .initial_entry = 3000,
      .jitter = 500,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 3000,
      .jitter = 500,
      .count = 7,
    },
    {
      .type = STAGE_E_FOE_TANKER_PULSAR,
      .initial_entry = 200,
      .jitter = 500,
      .count = 30,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
