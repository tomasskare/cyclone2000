#ifndef _STAGE_0094_H_
#define _STAGE_0094_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.000
#define YSCALE 1.200
#define XOFF   0.000
#define YOFF   -0.400

static stage_t stage_0094 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 16,
  .coords = {
    -0.6689 * XSCALE + XOFF, 0.3602 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.6708 * YSCALE + YOFF,
    -0.6419 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    -0.2568 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.1014 * XSCALE + XOFF, 0.8385 * YSCALE + YOFF,
    0.4595 * XSCALE + XOFF, 0.6708 * YSCALE + YOFF,
    0.8176 * XSCALE + XOFF, 0.5031 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, 0.1615 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, -0.1615 * YSCALE + YOFF,
    0.8176 * XSCALE + XOFF, -0.5031 * YSCALE + YOFF,
    0.4595 * XSCALE + XOFF, -0.6708 * YSCALE + YOFF,
    0.1014 * XSCALE + XOFF, -0.8385 * YSCALE + YOFF,
    -0.2568 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.6419 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.6708 * YSCALE + YOFF,
    -0.6689 * XSCALE + XOFF, -0.3602 * YSCALE + YOFF,
  },
  .move_colours = 0,
  .colours = {
    0.200, 0.450, 0.050,
    0.425, 0.250, 0.025,
    0.650, 0.050, 0.000,
  },
  .line_colours = {0.90f, 0.70f, 0.30f, 0.40f, 0.20f, 0.10f},
  .start_position = 12,

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
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 7,
    },
    {
      .type = STAGE_E_FOE_PULSAR,
      .initial_entry = 0,
      .jitter = 200,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_MIRROR,
      .initial_entry = 1200,
      .jitter = 200,
      .count = 4,
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
      .count = 1,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 20000,
      .jitter = 2000,
      .count = 1,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
