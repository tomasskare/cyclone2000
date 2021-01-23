#ifndef _STAGE_0086_H_
#define _STAGE_0086_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.700
#define YSCALE 0.700
#define XOFF   0.000
#define YOFF   1.000

static stage_t stage_0086 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 13,
  .coords = {
    -1.0000 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.8907 * XSCALE + XOFF, -0.5507 * YSCALE + YOFF,
    -0.6921 * XSCALE + XOFF, -0.3406 * YSCALE + YOFF,
    -0.4967 * XSCALE + XOFF, -0.1159 * YSCALE + YOFF,
    -0.3013 * XSCALE + XOFF, 0.0942 * YSCALE + YOFF,
    -0.1026 * XSCALE + XOFF, 0.5290 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.1026 * XSCALE + XOFF, 0.5290 * YSCALE + YOFF,
    0.3013 * XSCALE + XOFF, 0.0942 * YSCALE + YOFF,
    0.4967 * XSCALE + XOFF, -0.1159 * YSCALE + YOFF,
    0.6921 * XSCALE + XOFF, -0.3406 * YSCALE + YOFF,
    0.8907 * XSCALE + XOFF, -0.5507 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
  },
  .convex = {
    [4] = 1,
    [5] = 1,
    [7] = 1,
    [8] = 1,
  },
  .move_colours = 0,
  .colours = {
    0.600, 0.600, 0.000,
    0.600, 0.400, 0.000,
    0.600, 0.600, 0.000,
  },
  .line_colours = {0.90f, 0.70f, 0.30f, 0.40f, 0.20f, 0.10f},
  .start_position = 5,
  .reverse_direction = 1,

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
      .count = 32,
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
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_PULSAR,
      .initial_entry = 0,
      .jitter = 200,
      .count = 2,
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
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_TANKER_PULSAR,
      .initial_entry = 1300,
      .jitter = 200,
      .count = 3,
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
