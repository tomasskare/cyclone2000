#ifndef _STAGE_0003_H_
#define _STAGE_0003_H_

static stage_t stage_0003 __attribute__((unused)) = {
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
  .colours = {0.30f, 0.35f, 0.50f,
	      0.15f, 0.15f, 0.55f,
	      0.35f, 0.10f, 0.45f},
  .line_colours = {0.15f, 0.15f, 0.65f, 0.05f, 0.05f, 0.40f},
  .start_position = 5,

  .mutant_tanker_rnd = 100,

  .stage_length = 47000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 400,
      .count = 22,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 4000,
      .jitter = 1000,
      .count = 4,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
