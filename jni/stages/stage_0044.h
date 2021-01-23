#ifndef _STAGE_0044_H_
#define _STAGE_0044_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.000
#define YSCALE 1.000
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_0044 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 16,
  .coords = {
    0.2000 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.2000 * XSCALE + XOFF, 0.6000 * YSCALE + YOFF,
    0.6000 * XSCALE + XOFF, 0.2000 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, 0.2000 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, -0.2000 * YSCALE + YOFF,
    0.6000 * XSCALE + XOFF, -0.2000 * YSCALE + YOFF,
    0.2000 * XSCALE + XOFF, -0.6000 * YSCALE + YOFF,
    0.2000 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.2000 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.2000 * XSCALE + XOFF, -0.6000 * YSCALE + YOFF,
    -0.6000 * XSCALE + XOFF, -0.2000 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.2000 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.2000 * YSCALE + YOFF,
    -0.6000 * XSCALE + XOFF, 0.2000 * YSCALE + YOFF,
    -0.2000 * XSCALE + XOFF, 0.6000 * YSCALE + YOFF,
    -0.2000 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
  },
  .convex = {
    [1] = 1,
    [2] = 1,
    [5] = 1,
    [6] = 1,
    [9] = 1,
    [10] = 1,
    [13] = 1,
    [14] = 1,
  },
  .move_colours = 1,
  .colours = {
    0.000, 0.050, 0.450,
    0.250, 0.000, 0.350,
    0.000, 0.050, 0.450,
  },
  .line_colours = {0.90f, 0.90f, 0.00f, 0.40f, 0.40f, 0.00f},
  .start_position = 7,

  .mutant_tanker_rnd = 5,
  .mutant_flipper_rnd = 10,
  .spiker_num = 1,
  .spiker_delay = 1000,

  .stage_length = 38000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 25,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 0,
      .jitter = 500,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_TANKER_FUSEBALL,
      .initial_entry = 0,
      .jitter = 500,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_PULSAR,
      .initial_entry = 6000,
      .jitter = 1000,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_TANKER_PULSAR,
      .initial_entry = 0,
      .jitter = 1000,
      .count = 4,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
