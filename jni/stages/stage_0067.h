#ifndef _STAGE_0067_H_
#define _STAGE_0067_H_

static stage_t stage_0067 __attribute__((unused)) = {
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
  .colours = {0.25f, 0.50f, 0.45f,
	      0.05f, 0.00f, 0.60f,
	      0.60f, 0.00f, 0.70f},
  .line_colours = {0.90f, 0.00f, 0.60f, 0.40f, 0.00f, 0.30f},
  .start_position = 5,

  .mutant_flipper_rnd = 1,

  .stage_length = 35000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 44,
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
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 1500,
      .jitter = 500,
      .count = 4,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
