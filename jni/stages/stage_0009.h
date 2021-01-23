#ifndef _STAGE_0009_H_
#define _STAGE_0009_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.200
#define YSCALE 1.000
#define XOFF   -0.700
#define YOFF   0.000

static stage_t stage_0009 __attribute__((unused)) = {
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
    0.000, 0.000, 0.312,
    0.404, 0.145, 0.606,
    0.090, 0.244, 0.389,
  },
  .line_colours = {0.15f, 0.15f, 0.65f, 0.05f, 0.05f, 0.40f},
  .start_position = 6,
  .reverse_direction = 1,

  .mutant_tanker_rnd = 6,
  .mutant_flipper_rnd = 90,
  .spiker_num = 1,
  .spiker_delay = 3000,

  .stage_length = 34000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 22,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 1500,
      .jitter = 200,
      .count = 8,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
