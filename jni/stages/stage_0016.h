#ifndef _STAGE_0016_H_
#define _STAGE_0016_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.000
#define YSCALE 1.000
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_0016 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 12,
  .coords = {
    0.0000 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.2069 * XSCALE + XOFF, 0.6000 * YSCALE + YOFF,
    0.5977 * XSCALE + XOFF, 0.2057 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, 0.0000 * YSCALE + YOFF,
    0.5977 * XSCALE + XOFF, -0.2057 * YSCALE + YOFF,
    0.2069 * XSCALE + XOFF, -0.6000 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.2069 * XSCALE + XOFF, -0.6000 * YSCALE + YOFF,
    -0.5977 * XSCALE + XOFF, -0.2057 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.0000 * YSCALE + YOFF,
    -0.5977 * XSCALE + XOFF, 0.2057 * YSCALE + YOFF,
    -0.2069 * XSCALE + XOFF, 0.6000 * YSCALE + YOFF,
  },
  .convex = {
    [1] = 1,
    [2] = 1,
    [4] = 1,
    [5] = 1,
    [7] = 1,
    [8] = 1,
    [10] = 1,
    [11] = 1,
  },
  .move_colours = 0,
  .colours = {
    0.000, 0.150, 0.550,
    0.350, 0.100, 0.450,
    0.000, 0.150, 0.550,
  },
  .line_colours = {0.15f, 0.15f, 0.65f, 0.05f, 0.05f, 0.40f},
  .start_position = 7,

  .mutant_tanker_rnd = 8,
  .mutant_flipper_rnd = 180,
  .spiker_num = 1,
  .spiker_delay = 2000,

  .stage_length = 36000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 25,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 1500,
      .jitter = 200,
      .count = 10,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 0,
      .jitter = 200,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
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
