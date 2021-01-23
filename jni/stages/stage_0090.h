#ifndef _STAGE_0090_H_
#define _STAGE_0090_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.300
#define YSCALE 0.700
#define XOFF   0.000
#define YOFF   0.700

static stage_t stage_0090 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 16,
  .coords = {
    0.0000 * XSCALE + XOFF, 0.7514 * YSCALE + YOFF,
    0.2739 * XSCALE + XOFF, 0.5141 * YSCALE + YOFF,
    0.5669 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.7134 * XSCALE + XOFF, 0.5141 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, 0.2655 * YSCALE + YOFF,
    0.7070 * XSCALE + XOFF, -0.0056 * YSCALE + YOFF,
    0.5796 * XSCALE + XOFF, -0.4802 * YSCALE + YOFF,
    0.2866 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.2866 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.5796 * XSCALE + XOFF, -0.4802 * YSCALE + YOFF,
    -0.7707 * XSCALE + XOFF, -0.0056 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.2655 * YSCALE + YOFF,
    -0.7134 * XSCALE + XOFF, 0.5141 * YSCALE + YOFF,
    -0.5669 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    -0.2739 * XSCALE + XOFF, 0.5141 * YSCALE + YOFF,
  },
  .convex = {
    [1] = 1,
    [3] = 1,
    [5] = 1,
    [11] = 1,
    [13] = 1,
    [15] = 1,
  },
  .move_colours = 0,
  .colours = {
    0.500, 0.000, 0.000,
    0.000, 0.000, 0.500,
  },
  .line_colours = {0.90f, 0.70f, 0.30f, 0.40f, 0.20f, 0.10f},
  .start_position = 8,

  .mutant_flipper_rnd = 1,
  .mutant_tanker_rnd = 1,
  .spiker_num = 1,
  .spiker_delay = 1000,

  .stage_length = 55000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 34,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_MIRROR,
      .initial_entry = 1200,
      .jitter = 3000,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_UFO,
      .initial_entry = 1200,
      .jitter = 200,
      .count = 6,
    },
    {
      .type = STAGE_E_FOE_TANKER_PULSAR,
      .initial_entry = 1300,
      .jitter = 200,
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_TANKER_FUSEBALL,
      .initial_entry = 5000,
      .jitter = 200,
      .count = 2,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
