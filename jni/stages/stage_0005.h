#ifndef _STAGE_0005_H_
#define _STAGE_0005_H_

static stage_t stage_0005 __attribute__((unused)) = {
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
  .move_colours = 1,
  .colours = {0.00f, 0.15f, 0.55f,
	      0.30f, 0.05f, 0.40f},
  .line_colours = {0.15f, 0.15f, 0.65f, 0.05f, 0.05f, 0.40f},
  .start_position = 9,

  .mutant_tanker_rnd = 20,
  .spiker_num = 1,
  .spiker_delay = 4000,

  .stage_length = 41000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 300,
      .count = 23,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 4500,
      .jitter = 400,
      .count = 7,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
