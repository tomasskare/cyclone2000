#ifndef _STAGE_0079_H_
#define _STAGE_0079_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.300
#define YSCALE 1.000
#define XOFF   0.000
#define YOFF   -1.000

static stage_t stage_0079 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 16,
  .coords = {
    0.7536 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, 0.6600 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, 0.3133 * YSCALE + YOFF,
    0.7536 * XSCALE + XOFF, -0.0200 * YSCALE + YOFF,
    0.5072 * XSCALE + XOFF, -0.0200 * YSCALE + YOFF,
    0.3816 * XSCALE + XOFF, -0.3467 * YSCALE + YOFF,
    0.2609 * XSCALE + XOFF, -0.6733 * YSCALE + YOFF,
    0.1353 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.1353 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.2609 * XSCALE + XOFF, -0.6733 * YSCALE + YOFF,
    -0.3816 * XSCALE + XOFF, -0.3467 * YSCALE + YOFF,
    -0.5072 * XSCALE + XOFF, -0.0200 * YSCALE + YOFF,
    -0.7536 * XSCALE + XOFF, -0.0200 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.3133 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.6600 * YSCALE + YOFF,
    -0.7536 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
  },
  .convex = {
    [4] = 1,
    [11] = 1,
  },
  .move_colours = 0,
  .colours = {
    0.000, 0.150, 0.400,
    0.350, 0.100, 0.450,
    0.000, 0.150, 0.400,
  },
  .line_colours = {0.90f, 0.00f, 0.60f, 0.40f, 0.00f, 0.30f},
  .start_position = 7,

  .mutant_flipper_rnd = 1,
  .mutant_tanker_rnd = 1,
  .spiker_num = 1,
  .spiker_delay = 1000,

  .stage_length = 40000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 33,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 5000,
      .jitter = 200,
      .count = 1,
    },
    {
      .type = STAGE_E_FOE_MIRROR,
      .initial_entry = 3000,
      .jitter = 500,
      .count = 4,
    },
    {
      .type = STAGE_E_FOE_UFO,
      .initial_entry = 2000,
      .jitter = 500,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 200,
      .jitter = 500,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_TANKER_FUSEBALL,
      .initial_entry = 6000,
      .jitter = 500,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_PULSAR,
      .initial_entry = 5000,
      .jitter = 500,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 3000,
      .jitter = 500,
      .count = 1,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
