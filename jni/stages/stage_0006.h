#ifndef _STAGE_0006_H_
#define _STAGE_0006_H_

static stage_t stage_0006 __attribute__((unused)) = {
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
  .move_colours = 1,
  .colours = {0.00f, 0.15f, 0.55f,
	      0.35f, 0.10f, 0.45f,
	      0.00f, 0.15f, 0.55f},
  .line_colours = {0.15f, 0.15f, 0.65f, 0.05f, 0.05f, 0.40f},
  .start_position = 7,

  .mutant_tanker_rnd = 10,
  .mutant_flipper_rnd = 200,
  .spiker_num = 1,
  .spiker_delay = 3500,

  .stage_length = 42000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 300,
      .count = 25,
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
