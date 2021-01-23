#ifndef _STAGE_0025_H_
#define _STAGE_0025_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.100
#define YSCALE 1.100
#define XOFF   -0.800
#define YOFF   -0.500

static stage_t stage_0025 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 13,
  .coords = {
    1.0000 * XSCALE + XOFF, -0.8510 * YSCALE + YOFF,
    0.7075 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.3913 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.0672 * XSCALE + XOFF, -0.8510 * YSCALE + YOFF,
    -0.0672 * XSCALE + XOFF, -0.5451 * YSCALE + YOFF,
    -0.0672 * XSCALE + XOFF, -0.2314 * YSCALE + YOFF,
    0.0830 * XSCALE + XOFF, 0.0824 * YSCALE + YOFF,
    -0.2253 * XSCALE + XOFF, -0.0824 * YSCALE + YOFF,
    -0.5415 * XSCALE + XOFF, -0.0824 * YSCALE + YOFF,
    -0.8735 * XSCALE + XOFF, 0.0824 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.3647 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.7020 * YSCALE + YOFF,
    -0.8656 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
  },
  .convex = {
    [6] = 1,
  },
  .move_colours = 0,
  .colours = {
    0.000, 0.150, 0.550,
    0.175, 0.125, 0.500,
    0.350, 0.100, 0.450,
  },
  .line_colours = {0.90f, 0.00f, 0.00f, 0.40f, 0.00f, 0.00f},
  .start_position = 7,

  .mutant_tanker_rnd = 7,
  .mutant_flipper_rnd = 30,
  .spiker_num = 1,
  .spiker_delay = 2500,

  .stage_length = 38000,
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
      .count = 3,
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
