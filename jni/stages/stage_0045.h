#ifndef _STAGE_0045_H_
#define _STAGE_0045_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.250
#define YSCALE 1.000
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_0045 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 13,
  .coords = {
    1.0000 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.8440 * XSCALE + XOFF, 0.5548 * YSCALE + YOFF,
    0.6881 * XSCALE + XOFF, 0.1163 * YSCALE + YOFF,
    0.5413 * XSCALE + XOFF, -0.3355 * YSCALE + YOFF,
    0.3807 * XSCALE + XOFF, -0.7807 * YSCALE + YOFF,
    0.0642 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.2339 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.5459 * XSCALE + XOFF, -0.7741 * YSCALE + YOFF,
    -0.8440 * XSCALE + XOFF, -0.5681 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.1030 * YSCALE + YOFF,
    -0.8440 * XSCALE + XOFF, 0.3555 * YSCALE + YOFF,
    -0.7064 * XSCALE + XOFF, 0.7741 * YSCALE + YOFF,
    -0.3853 * XSCALE + XOFF, 0.9934 * YSCALE + YOFF,
  },
  .move_colours = 0,
  .colours = {
    0.000, 0.130, 0.500,
    0.000, 0.330, 0.470,
    0.150, 0.550, 0.450,
  },
  .line_colours = {0.90f, 0.90f, 0.00f, 0.40f, 0.40f, 0.00f},
  .start_position = 6,

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
      .count = 28,
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
      .count = 4,
    },
    {
      .type = STAGE_E_FOE_PULSAR,
      .initial_entry = 6000,
      .jitter = 1000,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_TANKER_PULSAR,
      .initial_entry = 0,
      .jitter = 1000,
      .count = 5,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
