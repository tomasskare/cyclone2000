#ifndef _STAGE_0042_H_
#define _STAGE_0042_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 0.800
#define YSCALE 0.800
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_0042 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 16,
  .coords = {
    0.2342 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.7472 * XSCALE + XOFF, 0.7612 * YSCALE + YOFF,
    0.9851 * XSCALE + XOFF, 0.2612 * YSCALE + YOFF,
    0.5019 * XSCALE + XOFF, 0.0149 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, -0.2313 * YSCALE + YOFF,
    0.7472 * XSCALE + XOFF, -0.7388 * YSCALE + YOFF,
    0.2342 * XSCALE + XOFF, -0.9925 * YSCALE + YOFF,
    0.0037 * XSCALE + XOFF, -0.5000 * YSCALE + YOFF,
    -0.2491 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.7472 * XSCALE + XOFF, -0.7463 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.2612 * YSCALE + YOFF,
    -0.5093 * XSCALE + XOFF, 0.0075 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.2537 * YSCALE + YOFF,
    -0.7472 * XSCALE + XOFF, 0.7537 * YSCALE + YOFF,
    -0.2416 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    -0.0037 * XSCALE + XOFF, 0.5149 * YSCALE + YOFF,
  },
  .convex = {
    [3] = 1,
    [7] = 1,
    [11] = 1,
    [15] = 1,
  },
  .move_colours = 0,
  .colours = {
    0.000, 0.150, 0.550,
    0.350, 0.100, 0.450,
    0.000, 0.150, 0.550,
  },
  .line_colours = {0.90f, 0.90f, 0.00f, 0.40f, 0.40f, 0.00f},
  .start_position = 7,

  .mutant_tanker_rnd = 5,
  .mutant_flipper_rnd = 10,
  .spiker_num = 1,
  .spiker_delay = 1000,

  .stage_length = 37000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 20,
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
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 2000,
      .jitter = 500,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_TANKER_FUSEBALL,
      .initial_entry = 0,
      .jitter = 500,
      .count = 1,
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
      .count = 3,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
