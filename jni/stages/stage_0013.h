#ifndef _STAGE_0013_H_
#define _STAGE_0013_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.250
#define YSCALE 1.000
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_0013 __attribute__((unused)) = {
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
    0.000, 0.180, 0.650,
    0.000, 0.380, 0.630,
    0.150, 0.650, 0.600,
  },
  .line_colours = {0.15f, 0.15f, 0.65f, 0.05f, 0.05f, 0.40f},
  .start_position = 6,

  .mutant_tanker_rnd = 9,
  .mutant_flipper_rnd = 200,
  .spiker_num = 1,
  .spiker_delay = 2300,

  .stage_length = 36000,
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
      .count = 10,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 0,
      .jitter = 200,
      .count = 1,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
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
