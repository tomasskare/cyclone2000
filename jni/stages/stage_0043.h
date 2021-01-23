#ifndef _STAGE_0043_H_
#define _STAGE_0043_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.500
#define YSCALE 1.000
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_0043 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 14,
  .coords = {
    -1.0000 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.9079 * XSCALE + XOFF, -0.6631 * YSCALE + YOFF,
    -0.7895 * XSCALE + XOFF, -0.3047 * YSCALE + YOFF,
    -0.6711 * XSCALE + XOFF, 0.0968 * YSCALE + YOFF,
    -0.5482 * XSCALE + XOFF, 0.4552 * YSCALE + YOFF,
    -0.3246 * XSCALE + XOFF, 0.8136 * YSCALE + YOFF,
    -0.1096 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.1096 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.3246 * XSCALE + XOFF, 0.8136 * YSCALE + YOFF,
    0.5482 * XSCALE + XOFF, 0.4552 * YSCALE + YOFF,
    0.6711 * XSCALE + XOFF, 0.0968 * YSCALE + YOFF,
    0.7895 * XSCALE + XOFF, -0.3047 * YSCALE + YOFF,
    0.9079 * XSCALE + XOFF, -0.6631 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
  },
  .move_colours = 0,
  .colours = {
    0.350, 0.100, 0.450,
    0.000, 0.150, 0.550,
    0.350, 0.100, 0.450,
  },
  .line_colours = {0.90f, 0.90f, 0.00f, 0.40f, 0.40f, 0.00f},
  .start_position = 7,
  .reverse_direction = 1,

  .mutant_tanker_rnd = 5,
  .mutant_flipper_rnd = 10,
  .spiker_num = 1,
  .spiker_delay = 1000,

  .stage_length = 38000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 25,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 0,
      .jitter = 500,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_TANKER_FUSEBALL,
      .initial_entry = 0,
      .jitter = 500,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_TANKER_PULSAR,
      .initial_entry = 0,
      .jitter = 1000,
      .count = 4,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
