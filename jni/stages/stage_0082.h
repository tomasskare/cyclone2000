#ifndef _STAGE_0082_H_
#define _STAGE_0082_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.100
#define YSCALE 1.100
#define XOFF   0.000
#define YOFF   -0.400

static stage_t stage_0082 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 16,
  .coords = {
    -0.0184 * XSCALE + XOFF, 0.1852 * YSCALE + YOFF,
    -0.3364 * XSCALE + XOFF, 0.3426 * YSCALE + YOFF,
    -0.3364 * XSCALE + XOFF, 0.6620 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.3318 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.6682 * XSCALE + XOFF, 0.8519 * YSCALE + YOFF,
    0.8341 * XSCALE + XOFF, 0.5093 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, 0.1759 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, -0.1620 * YSCALE + YOFF,
    0.8341 * XSCALE + XOFF, -0.4907 * YSCALE + YOFF,
    0.4977 * XSCALE + XOFF, -0.8333 * YSCALE + YOFF,
    0.1659 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.1797 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.5023 * XSCALE + XOFF, -0.8333 * YSCALE + YOFF,
    -0.8433 * XSCALE + XOFF, -0.6620 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.3287 * YSCALE + YOFF,
  },
  .move_colours = 0,
  .colours = {
    0.050, 0.600, 0.100,
    0.250, 0.300, 0.400,
    0.350, 0.000, 0.600,
  },
  .line_colours = {0.90f, 0.70f, 0.30f, 0.40f, 0.20f, 0.10f},
  .start_position = 11,

  .mutant_flipper_rnd = 1,
  .mutant_tanker_rnd = 1,
  .spiker_num = 1,
  .spiker_delay = 1000,

  .stage_length = 40000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 30,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 3000,
      .jitter = 200,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 5000,
      .jitter = 200,
      .count = 1,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_PULSAR,
      .initial_entry = 0,
      .jitter = 200,
      .count = 1,
    },
    {
      .type = STAGE_E_FOE_MIRROR,
      .initial_entry = 1200,
      .jitter = 200,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_UFO,
      .initial_entry = 1200,
      .jitter = 200,
      .count = 4,
    },
    {
      .type = STAGE_E_FOE_TANKER_PULSAR,
      .initial_entry = 1300,
      .jitter = 200,
      .count = 4,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
