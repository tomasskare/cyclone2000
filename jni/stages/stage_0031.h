#ifndef _STAGE_0031_H_
#define _STAGE_0031_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.000
#define YSCALE 1.000
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_0031 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 16,
  .coords = {
    0.0000 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, 0.6000 * YSCALE + YOFF,
    0.2000 * XSCALE + XOFF, 0.2000 * YSCALE + YOFF,
    0.6000 * XSCALE + XOFF, 0.0000 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, 0.0000 * YSCALE + YOFF,
    0.6000 * XSCALE + XOFF, 0.0000 * YSCALE + YOFF,
    0.2000 * XSCALE + XOFF, -0.2000 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, -0.6000 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, -0.6000 * YSCALE + YOFF,
    -0.2000 * XSCALE + XOFF, -0.2000 * YSCALE + YOFF,
    -0.6000 * XSCALE + XOFF, 0.0000 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.0000 * YSCALE + YOFF,
    -0.6000 * XSCALE + XOFF, 0.0000 * YSCALE + YOFF,
    -0.2000 * XSCALE + XOFF, 0.2000 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, 0.6000 * YSCALE + YOFF,
  },
  .convex = {
    [1] = 1,
    [2] = 1,
    [3] = 1,
    [5] = 1,
    [6] = 1,
    [7] = 1,
    [9] = 1,
    [10] = 1,
    [11] = 1,
    [13] = 1,
    [14] = 1,
    [15] = 1,
  },
  .move_colours = 1,
  .colours = {
    0.100, 0.100, 0.500,
    0.350, 0.100, 0.400,
  },
  .line_colours = {0.90f, 0.00f, 0.00f, 0.40f, 0.00f, 0.00f},
  .start_position = 9,

  .mutant_tanker_rnd = 5,
  .mutant_flipper_rnd = 15,
  .spiker_num = 1,
  .spiker_delay = 2000,

  .stage_length = 39000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 20,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 1500,
      .jitter = 400,
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 800,
      .jitter = 200,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_TANKER_FUSEBALL,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_PULSAR,
      .initial_entry = 0,
      .jitter = 200,
      .count = 5,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
