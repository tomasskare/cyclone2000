#ifndef _STAGE_0063_H_
#define _STAGE_0063_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.000
#define YSCALE 1.000
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_0063 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 16,
  .coords = {
    0.0000 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, 0.6000 * YSCALE + YOFF,
    0.2000 * XSCALE + XOFF, 0.2000 * YSCALE + YOFF,
    0.6000 * XSCALE + XOFF, 0.0000 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, 0.0000 * YSCALE + YOFF,
    0.6000 * XSCALE + XOFF, 0.0000 * YSCALE + YOFF,
    0.2000 * XSCALE + XOFF, -0.2000 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, -0.6000 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, -0.6000 * YSCALE + YOFF,
    -0.2000 * XSCALE + XOFF, -0.2000 * YSCALE + YOFF,
    -0.6000 * XSCALE + XOFF, 0.0000 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.0000 * YSCALE + YOFF,
    -0.6000 * XSCALE + XOFF, 0.0000 * YSCALE + YOFF,
    -0.2000 * XSCALE + XOFF, 0.2000 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, 0.6000 * YSCALE + YOFF,
  },
  .convex = {
    [1] = 1,
    [2] = 1,
    [3] = 1,
    [5] = 1,
    [6] = 1,
    [7] = 1,
    [9] = 1,
    [10] = 1,
    [11] = 1,
    [13] = 1,
    [14] = 1,
    [15] = 1,
  },
  .transparent = 1,
  .no_highlight = 1,
  .move_line_colours = 1,
  .line_colours = {
    0.450f, 0.840f, 0.060f, 0.200f, 0.373f, 0.027f,
    0.622f, 0.724f, 0.004f, 0.277f, 0.322f, 0.002f,
    0.768f, 0.566f, 0.015f, 0.341f, 0.252f, 0.007f,
    0.866f, 0.391f, 0.093f, 0.385f, 0.174f, 0.041f,
    0.900f, 0.225f, 0.225f, 0.400f, 0.100f, 0.100f,
    0.866f, 0.093f, 0.391f, 0.385f, 0.041f, 0.174f,
    0.768f, 0.015f, 0.566f, 0.341f, 0.007f, 0.252f,
    0.622f, 0.004f, 0.724f, 0.277f, 0.002f, 0.322f,
    0.450f, 0.060f, 0.840f, 0.200f, 0.027f, 0.373f,
    0.278f, 0.176f, 0.896f, 0.123f, 0.078f, 0.398f,
    0.132f, 0.334f, 0.885f, 0.059f, 0.148f, 0.393f,
    0.034f, 0.509f, 0.807f, 0.015f, 0.226f, 0.359f,
    0.000f, 0.675f, 0.675f, 0.000f, 0.300f, 0.300f,
    0.034f, 0.807f, 0.509f, 0.015f, 0.359f, 0.226f,
    0.132f, 0.885f, 0.334f, 0.059f, 0.393f, 0.148f,
    0.278f, 0.896f, 0.176f, 0.123f, 0.398f, 0.078f,
  },
  .start_position = 9,

  .spiker_num = 1,
  .spiker_delay = 0,

  .stage_length = 37000,
  .foes = {
    {
      .type = STAGE_E_FOE_TANKER_PULSAR,
      .initial_entry = 0,
      .jitter = 200,
      .count = 7,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 0,
      .jitter = 200,
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_MIRROR,
      .initial_entry = 2000,
      .jitter = 200,
      .count = 5,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
