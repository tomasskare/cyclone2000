#ifndef _STAGE_0092_H_
#define _STAGE_0092_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.000
#define YSCALE 1.500
#define XOFF   0.000
#define YOFF   -0.500

static stage_t stage_0092 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 15,
  .coords = {
    1.0000 * XSCALE + XOFF, -0.8067 * YSCALE + YOFF,
    0.7026 * XSCALE + XOFF, -0.9000 * YSCALE + YOFF,
    0.3846 * XSCALE + XOFF, -0.9067 * YSCALE + YOFF,
    0.0974 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.2410 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.3846 * XSCALE + XOFF, -0.8200 * YSCALE + YOFF,
    -0.3846 * XSCALE + XOFF, -0.6200 * YSCALE + YOFF,
    -0.5487 * XSCALE + XOFF, -0.4133 * YSCALE + YOFF,
    -0.5487 * XSCALE + XOFF, -0.2000 * YSCALE + YOFF,
    -0.6821 * XSCALE + XOFF, -0.0200 * YSCALE + YOFF,
    -0.6821 * XSCALE + XOFF, 0.2000 * YSCALE + YOFF,
    -0.8462 * XSCALE + XOFF, 0.4000 * YSCALE + YOFF,
    -0.8462 * XSCALE + XOFF, 0.5867 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.7933 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
  },
  .convex = {
    [2] = 1,
    [6] = 1,
    [8] = 1,
    [10] = 1,
    [12] = 1,
  },
  .move_colours = 0,
  .colours = {
    0.000, 0.150, 0.550,
    0.225, 0.125, 0.550,
    0.450, 0.100, 0.550,
  },
  .line_colours = {0.90f, 0.70f, 0.30f, 0.40f, 0.20f, 0.10f},
  .start_position = 3,

  .mutant_flipper_rnd = 1,
  .mutant_tanker_rnd = 1,
  .spiker_num = 1,
  .spiker_delay = 1000,

  .stage_length = 55000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 34,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 6,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 5,
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
      .count = 6,
    },
    {
      .type = STAGE_E_FOE_TANKER_PULSAR,
      .initial_entry = 1300,
      .jitter = 200,
      .count = 5,
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
