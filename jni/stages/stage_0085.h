#ifndef _STAGE_0085_H_
#define _STAGE_0085_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 0.600
#define YSCALE 0.600
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_0085 __attribute__((unused)) = {
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
  .line_colours = {0.90f, 0.70f, 0.30f, 0.40f, 0.20f, 0.10f},
  .start_position = 3,

  .mutant_flipper_rnd = 1,
  .mutant_tanker_rnd = 1,
  .spiker_num = 1,
  .spiker_delay = 1000,

  .stage_length = 39000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 31,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 8000,
      .jitter = 200,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 5000,
      .jitter = 200,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 6,
    },
    {
      .type = STAGE_E_FOE_PULSAR,
      .initial_entry = 0,
      .jitter = 200,
      .count = 4,
    },
    {
      .type = STAGE_E_FOE_MIRROR,
      .initial_entry = 1200,
      .jitter = 3000,
      .count = 1,
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
