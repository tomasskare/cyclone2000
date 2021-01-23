#ifndef _STAGE_0020_H_
#define _STAGE_0020_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.000
#define YSCALE 1.500
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_0020 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 13,
  .coords = {
    1.0000 * XSCALE + XOFF, -0.6418 * YSCALE + YOFF,
    0.6391 * XSCALE + XOFF, -0.8856 * YSCALE + YOFF,
    0.2632 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.1053 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.4662 * XSCALE + XOFF, -0.8856 * YSCALE + YOFF,
    -0.8271 * XSCALE + XOFF, -0.6418 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.4080 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.1542 * YSCALE + YOFF,
    -0.8120 * XSCALE + XOFF, 0.0846 * YSCALE + YOFF,
    -0.4662 * XSCALE + XOFF, 0.3134 * YSCALE + YOFF,
    -0.0977 * XSCALE + XOFF, 0.5622 * YSCALE + YOFF,
    0.0977 * XSCALE + XOFF, 0.8060 * YSCALE + YOFF,
    0.0977 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
  },
  .convex = {
    [10] = 1,
    [11] = 1,
  },
  .move_colours = 0,
  .colours = {
    0.700, 0.185, 0.000,
    0.710, 0.450, 0.450,
    0.625, 0.625, 0.000,
  },
  .line_colours = {0.90f, 0.00f, 0.00f, 0.40f, 0.00f, 0.00f},
  .start_position = 2,

  .mutant_tanker_rnd = 20,
  .spiker_num = 1,
  .spiker_delay = 2500,

  .stage_length = 40000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 14,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 1500,
      .jitter = 400,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 3,
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
