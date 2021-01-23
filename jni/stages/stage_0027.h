#ifndef _STAGE_0027_H_
#define _STAGE_0027_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 0.700
#define YSCALE 1.100
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_0027 __attribute__((unused)) = {
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
  .line_colours = {0.90f, 0.00f, 0.00f, 0.40f, 0.00f, 0.00f},
  .start_position = 7,

  .mutant_tanker_rnd = 5,
  .mutant_flipper_rnd = 20,
  .spiker_num = 1,
  .spiker_delay = 2500,

  .stage_length = 40000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 17,
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
      .initial_entry = 7000,
      .jitter = 200,
      .count = 1,
    },
    {
      .type = STAGE_E_FOE_PULSAR,
      .initial_entry = 0,
      .jitter = 200,
      .count = 4,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
