#ifndef _STAGE_0001_H_
#define _STAGE_0001_H_

static stage_t stage_0001 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 16,
  .coords = {
    -0.196f, 1.000f,
    0.202f, 0.999f,
    0.569f, 0.847f,
    0.849f, 0.567f,
    1.000f, 0.202f,
    0.999f, -0.192f,
    0.845f, -0.555f,
    0.563f, -0.833f,
    0.196f, -0.983f,
    -0.202f, -0.982f,
    -0.569f, -0.830f,
    -0.849f, -0.550f,
    -1.000f, -0.186f,
    -0.999f, 0.209f,
    -0.845f, 0.572f,
    -0.563f, 0.850f,
  },
  .move_colours = 1,
  .colours = {0.00f, 0.15f, 0.65f,
	      0.00f, 0.35f, 0.45f},
  .line_colours = {0.15f, 0.15f, 0.65f, 0.05f, 0.05f, 0.40f},
  .start_position = 8,

  .stage_length = 41000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 21,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
