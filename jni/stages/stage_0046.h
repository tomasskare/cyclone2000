#ifndef _STAGE_0046_H_
#define _STAGE_0046_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.300
#define YSCALE 1.000
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_0046 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 16,
  .coords = {
    1.0000 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.8701 * XSCALE + XOFF, 0.7016 * YSCALE + YOFF,
    0.7532 * XSCALE + XOFF, 0.4188 * YSCALE + YOFF,
    0.6364 * XSCALE + XOFF, 0.1361 * YSCALE + YOFF,
    0.5195 * XSCALE + XOFF, -0.1466 * YSCALE + YOFF,
    0.4026 * XSCALE + XOFF, -0.4293 * YSCALE + YOFF,
    0.1688 * XSCALE + XOFF, -0.4293 * YSCALE + YOFF,
    0.0476 * XSCALE + XOFF, -0.7173 * YSCALE + YOFF,
    -0.0693 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.2987 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.4156 * XSCALE + XOFF, -0.7173 * YSCALE + YOFF,
    -0.5325 * XSCALE + XOFF, -0.4346 * YSCALE + YOFF,
    -0.6494 * XSCALE + XOFF, -0.1518 * YSCALE + YOFF,
    -0.7662 * XSCALE + XOFF, 0.1309 * YSCALE + YOFF,
    -0.8831 * XSCALE + XOFF, 0.4136 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.6963 * YSCALE + YOFF,
  },
  .convex = {
    [6] = 1,
  },
  .move_colours = 0,
  .colours = {
    0.000, 0.150, 0.550,
    0.350, 0.100, 0.450,
    0.000, 0.150, 0.550,
  },
  .line_colours = {0.90f, 0.90f, 0.00f, 0.40f, 0.40f, 0.00f},
  .start_position = 8,

  .mutant_tanker_rnd = 4,
  .mutant_flipper_rnd = 9,
  .spiker_num = 1,
  .spiker_delay = 1000,

  .stage_length = 38000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 28,
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
      .count = 4,
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
      .count = 5,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
