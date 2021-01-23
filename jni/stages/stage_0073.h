#ifndef _STAGE_0073_H_
#define _STAGE_0073_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.200
#define YSCALE 1.000
#define XOFF   -0.700
#define YOFF   0.000

static stage_t stage_0073 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 14,
  .coords = {
    -0.7593 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.8843 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.6804 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.4601 * YSCALE + YOFF,
    -0.7593 * XSCALE + XOFF, -0.2452 * YSCALE + YOFF,
    -0.5062 * XSCALE + XOFF, -0.1460 * YSCALE + YOFF,
    -0.2840 * XSCALE + XOFF, -0.0468 * YSCALE + YOFF,
    -0.0494 * XSCALE + XOFF, 0.0523 * YSCALE + YOFF,
    0.1728 * XSCALE + XOFF, 0.2562 * YSCALE + YOFF,
    0.2963 * XSCALE + XOFF, 0.4711 * YSCALE + YOFF,
    0.4074 * XSCALE + XOFF, 0.6694 * YSCALE + YOFF,
    0.5247 * XSCALE + XOFF, 0.8898 * YSCALE + YOFF,
    0.7654 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
  },
  .convex = {
    [7] = 1,
    [8] = 1,
  },
  .move_colours = 0,
  .colours = {
    0.050, 0.000, 0.650,
    0.400, 0.000, 0.600,
    0.050, 0.000, 0.650},
  .line_colours = {0.90f, 0.00f, 0.60f, 0.40f, 0.00f, 0.30f},
  .start_position = 6,
  .reverse_direction = 1,

  .mutant_flipper_rnd = 1,
  .spiker_num = 1,
  .spiker_delay = 1000,

  .stage_length = 40000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 38,
    },
    {
      .type = STAGE_E_FOE_MIRROR,
      .initial_entry = 4000,
      .jitter = 500,
      .count = 1,
    },
    {
      .type = STAGE_E_FOE_UFO,
      .initial_entry = 3000,
      .jitter = 500,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 200,
      .jitter = 500,
      .count = 4,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
