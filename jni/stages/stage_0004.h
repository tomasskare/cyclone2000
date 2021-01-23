#ifndef _STAGE_0004_H_
#define _STAGE_0004_H_

static stage_t stage_0004 __attribute__((unused)) = {
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
  .move_colours = 1,
  .colours = {0.00f, 0.15f, 0.55f,
	      0.35f, 0.10f, 0.45f,
	      0.00f, 0.15f, 0.55f},
  .line_colours = {0.15f, 0.15f, 0.65f, 0.05f, 0.05f, 0.40f},
  .start_position = 7,

  .mutant_tanker_rnd = 50,
  .spiker_num = 1,
  .spiker_delay = 4000,

  .stage_length = 40000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 300,
      .count = 22,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 4500,
      .jitter = 400,
      .count = 6,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
