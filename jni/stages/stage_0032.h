#ifndef _STAGE_0032_H_
#define _STAGE_0032_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.200
#define YSCALE 1.200
#define XOFF   -0.400
#define YOFF   -1.000

static stage_t stage_0032 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 18,
  .coords = {
    -0.6681 * XSCALE + XOFF, -0.1790 * YSCALE + YOFF,
    -0.3362 * XSCALE + XOFF, 0.1616 * YSCALE + YOFF,
    -0.6681 * XSCALE + XOFF, 0.1616 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.1616 * YSCALE + YOFF,
    -0.6681 * XSCALE + XOFF, 0.3275 * YSCALE + YOFF,
    -0.3362 * XSCALE + XOFF, 0.4934 * YSCALE + YOFF,
    0.0044 * XSCALE + XOFF, 0.6681 * YSCALE + YOFF,
    0.3362 * XSCALE + XOFF, 0.8341 * YSCALE + YOFF,
    0.6681 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, 0.6681 * YSCALE + YOFF,
    0.8341 * XSCALE + XOFF, 0.3362 * YSCALE + YOFF,
    0.6681 * XSCALE + XOFF, 0.0044 * YSCALE + YOFF,
    0.4934 * XSCALE + XOFF, -0.3362 * YSCALE + YOFF,
    0.3275 * XSCALE + XOFF, -0.6681 * YSCALE + YOFF,
    0.1616 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.1616 * XSCALE + XOFF, -0.6769 * YSCALE + YOFF,
    0.1616 * XSCALE + XOFF, -0.3450 * YSCALE + YOFF,
    -0.1528 * XSCALE + XOFF, -0.6943 * YSCALE + YOFF,
  },
  .convex = {
    [1] = 1,
    [16] = 1,
  },
  .move_colours = 0,
  .colours = {
    0.600, 0.000, 0.000,
    0.000, 0.000, 0.500,
    0.600, 0.000, 0.000,
  },
  .line_colours = {0.90f, 0.00f, 0.00f, 0.40f, 0.00f, 0.00f},
  .start_position = 13,

  .mutant_tanker_rnd = 5,
  .mutant_flipper_rnd = 15,
  .spiker_num = 1,
  .spiker_delay = 2000,

  .stage_length = 39000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 20,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 1500,
      .jitter = 400,
      .count = 5,
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
      .initial_entry = 1000,
      .jitter = 200,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_PULSAR,
      .initial_entry = 0,
      .jitter = 200,
      .count = 5,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
