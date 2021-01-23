#ifndef _STAGE_0023_H_
#define _STAGE_0023_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.300
#define YSCALE 0.750
#define XOFF   0.000
#define YOFF   -0.400

static stage_t stage_0023 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 15,
  .coords = {
    0.1724 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.5074 * XSCALE + XOFF, 0.7191 * YSCALE + YOFF,
    0.8424 * XSCALE + XOFF, 0.1574 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, -0.4128 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.6798 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.3350 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.3350 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.6798 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.4128 * YSCALE + YOFF,
    -0.8424 * XSCALE + XOFF, 0.1574 * YSCALE + YOFF,
    -0.5074 * XSCALE + XOFF, 0.7191 * YSCALE + YOFF,
    -0.1724 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
  },
  .move_colours = 0,
  .colours = {
    0.050, 0.600, 0.000,
    0.200, 0.000, 0.500,
  },
  .line_colours = {0.90f, 0.00f, 0.00f, 0.40f, 0.00f, 0.00f},
  .start_position = 7,

  .mutant_tanker_rnd = 8,
  .mutant_flipper_rnd = 40,
  .spiker_num = 1,
  .spiker_delay = 2500,

  .stage_length = 36000,
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
      .count = 2,
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
