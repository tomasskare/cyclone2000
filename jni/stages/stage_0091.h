#ifndef _STAGE_0091_H_
#define _STAGE_0091_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 0.700
#define YSCALE 1.100
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_0091 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 15,
  .coords = {
    0.0000 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, 0.6896 * YSCALE + YOFF,
    0.2437 * XSCALE + XOFF, 0.3740 * YSCALE + YOFF,
    0.7479 * XSCALE + XOFF, 0.0687 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, -0.2265 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, -0.5420 * YSCALE + YOFF,
    0.7479 * XSCALE + XOFF, -0.8473 * YSCALE + YOFF,
    0.2353 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.2353 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.7479 * XSCALE + XOFF, -0.8473 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.5420 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.2265 * YSCALE + YOFF,
    -0.7479 * XSCALE + XOFF, 0.0687 * YSCALE + YOFF,
    -0.2437 * XSCALE + XOFF, 0.3740 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, 0.6896 * YSCALE + YOFF,
  },
  .convex = {
    [1] = 1,
    [2] = 1,
    [13] = 1,
    [14] = 1,
  },
  .move_colours = 0,
  .colours = {
    0.250, 0.100, 0.450,
    0.050, 0.400, 0.350,
  },
  .line_colours = {0.90f, 0.70f, 0.30f, 0.40f, 0.20f, 0.10f},
  .start_position = 7,

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
      .count = 4,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_MIRROR,
      .initial_entry = 1200,
      .jitter = 3000,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_UFO,
      .initial_entry = 1200,
      .jitter = 200,
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_TANKER_PULSAR,
      .initial_entry = 1300,
      .jitter = 200,
      .count = 6,
    },
    {
      .type = STAGE_E_FOE_TANKER_FUSEBALL,
      .initial_entry = 5000,
      .jitter = 200,
      .count = 1,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 10000,
      .jitter = 200,
      .count = 1,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
