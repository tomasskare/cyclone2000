#ifndef _STAGE_0017_H_
#define _STAGE_0017_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.300
#define YSCALE 1.000
#define XOFF   0.000
#define YOFF   -0.700

static stage_t stage_0017 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 18,
  .coords = {
    0.0000 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.2966 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.5720 * XSCALE + XOFF, 0.8072 * YSCALE + YOFF,
    0.8602 * XSCALE + XOFF, 0.5964 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, 0.1988 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, -0.1988 * YSCALE + YOFF,
    0.8602 * XSCALE + XOFF, -0.5964 * YSCALE + YOFF,
    0.5720 * XSCALE + XOFF, -0.8072 * YSCALE + YOFF,
    0.2966 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.2966 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.5720 * XSCALE + XOFF, -0.8072 * YSCALE + YOFF,
    -0.8602 * XSCALE + XOFF, -0.5964 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.1988 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.1988 * YSCALE + YOFF,
    -0.8602 * XSCALE + XOFF, 0.5964 * YSCALE + YOFF,
    -0.5720 * XSCALE + XOFF, 0.8072 * YSCALE + YOFF,
    -0.2966 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
  },
  .move_colours = 0,
  .colours = {
    0.350, 0.000, 0.600,
    0.050, 0.600, 0.100,
    0.350, 0.000, 0.600,
  },
  .line_colours = {0.90f, 0.00f, 0.00f, 0.40f, 0.00f, 0.00f},
  .start_position = 9,

  .spiker_num = 1,
  .spiker_delay = 2500,

  .stage_length = 35000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 14,
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
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_PULSAR,
      .initial_entry = 0,
      .jitter = 200,
      .count = 2,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
