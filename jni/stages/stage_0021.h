#ifndef _STAGE_0021_H_
#define _STAGE_0021_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 0.600
#define YSCALE 0.600
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_0021 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 8,
  .coords = {
    0.3333 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, 0.3333 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, -0.3333 * YSCALE + YOFF,
    0.3333 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.3333 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.3333 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.3333 * YSCALE + YOFF,
    -0.3333 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
  },
  .move_colours = 1,
  .colours = {
    0.000, 0.150, 0.550,
    0.350, 0.100, 0.450,
    0.000, 0.150, 0.550,
  },
  .line_colours = {0.90f, 0.00f, 0.00f, 0.40f, 0.00f, 0.00f},
  .start_position = 3,

  .mutant_tanker_rnd = 20,
  .mutant_flipper_rnd = 100,
  .spiker_num = 1,
  .spiker_delay = 2500,

  .stage_length = 38000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 13,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 1500,
      .jitter = 400,
      .count = 2,
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
      .count = 2,
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
