#ifndef _STAGE_0093_H_
#define _STAGE_0093_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.200
#define YSCALE 0.600
#define XOFF   0.000
#define YOFF   -0.300

static stage_t stage_0093 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 13,
  .coords = {
    0.0000 * XSCALE + XOFF, 0.6111 * YSCALE + YOFF,
    0.3256 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.6698 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, 0.6333 * YSCALE + YOFF,
    0.8279 * XSCALE + XOFF, -0.2000 * YSCALE + YOFF,
    0.4977 * XSCALE + XOFF, -0.6111 * YSCALE + YOFF,
    0.1581 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.1581 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.4977 * XSCALE + XOFF, -0.6111 * YSCALE + YOFF,
    -0.8279 * XSCALE + XOFF, -0.2000 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.6333 * YSCALE + YOFF,
    -0.6698 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    -0.3256 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
  },
  .convex = {
    [0] = 1,
  },
  .move_colours = 0,
  .colours = {
    0.400, 0.000, 0.450,
    0.600, 0.000, 0.100,
  },
  .line_colours = {0.90f, 0.70f, 0.30f, 0.40f, 0.20f, 0.10f},
  .start_position = 6,

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
      .count = 4,
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
      .jitter = 200,
      .count = 2,
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
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_TANKER_FUSEBALL,
      .initial_entry = 5000,
      .jitter = 200,
      .count = 3,
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
