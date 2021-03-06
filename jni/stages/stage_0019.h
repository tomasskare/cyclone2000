#ifndef _STAGE_0019_H_
#define _STAGE_0019_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.000
#define YSCALE 1.000
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_0019 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 15,
  .coords = {
    0.0075 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.3317 * XSCALE + XOFF, 0.6716 * YSCALE + YOFF,
    0.6658 * XSCALE + XOFF, 0.3383 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, 0.0050 * YSCALE + YOFF,
    0.8404 * XSCALE + XOFF, -0.3333 * YSCALE + YOFF,
    0.6758 * XSCALE + XOFF, -0.6667 * YSCALE + YOFF,
    0.5062 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.1671 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.1721 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.5112 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.6758 * XSCALE + XOFF, -0.6667 * YSCALE + YOFF,
    -0.8404 * XSCALE + XOFF, -0.3333 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.0050 * YSCALE + YOFF,
    -0.6658 * XSCALE + XOFF, 0.3383 * YSCALE + YOFF,
    -0.3317 * XSCALE + XOFF, 0.6716 * YSCALE + YOFF,
  },
  .move_colours = 0,
  .colours = {
    0.400, 0.500, 0.100,
    0.100, 0.200, 0.600,
    0.350, 0.450, 0.150,
  },
  .line_colours = {0.90f, 0.00f, 0.00f, 0.40f, 0.00f, 0.00f},
  .start_position = 7,

  .mutant_tanker_rnd = 20,
  .spiker_num = 1,
  .spiker_delay = 2500,

  .stage_length = 40000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 14,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 1500,
      .jitter = 400,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_PULSAR,
      .initial_entry = 0,
      .jitter = 200,
      .count = 3,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
