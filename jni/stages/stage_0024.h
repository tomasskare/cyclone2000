#ifndef _STAGE_0024_H_
#define _STAGE_0024_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.500
#define YSCALE 1.200
#define XOFF   0.000
#define YOFF   -0.300

static stage_t stage_0024 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 14,
  .coords = {
    1.0000 * XSCALE + XOFF, -0.9871 * YSCALE + YOFF,
    0.7667 * XSCALE + XOFF, -0.8457 * YSCALE + YOFF,
    0.5236 * XSCALE + XOFF, -0.8457 * YSCALE + YOFF,
    0.2804 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.0521 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.1811 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.4144 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.6526 * XSCALE + XOFF, -0.8457 * YSCALE + YOFF,
    -0.8859 * XSCALE + XOFF, -0.5370 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.2476 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.0675 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.3826 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.6849 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
  },
  .convex = {
    [1] = 1,
    [2] = 1,
  },
  .move_colours = 0,
  .colours = {
    0.300, 0.000, 0.500,
    0.350, 0.000, 0.250,
    0.200, 0.000, 0.000,
  },
  .line_colours = {0.90f, 0.00f, 0.00f, 0.40f, 0.00f, 0.00f},
  .start_position = 4,

  .mutant_tanker_rnd = 7,
  .mutant_flipper_rnd = 40,
  .spiker_num = 1,
  .spiker_delay = 2500,

  .stage_length = 36000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 15,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 1500,
      .jitter = 400,
      .count = 4,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 800,
      .jitter = 200,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_PULSAR,
      .initial_entry = 0,
      .jitter = 200,
      .count = 3,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
