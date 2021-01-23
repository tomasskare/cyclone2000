#ifndef _STAGE_0059_H_
#define _STAGE_0059_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 0.700
#define YSCALE 1.100
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_0059 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 15,
  .coords = {
    0.0000 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, 0.6896 * YSCALE + YOFF,
    0.2437 * XSCALE + XOFF, 0.3740 * YSCALE + YOFF,
    0.7479 * XSCALE + XOFF, 0.0687 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, -0.2265 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, -0.5420 * YSCALE + YOFF,
    0.7479 * XSCALE + XOFF, -0.8473 * YSCALE + YOFF,
    0.2353 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.2353 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.7479 * XSCALE + XOFF, -0.8473 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.5420 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.2265 * YSCALE + YOFF,
    -0.7479 * XSCALE + XOFF, 0.0687 * YSCALE + YOFF,
    -0.2437 * XSCALE + XOFF, 0.3740 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, 0.6896 * YSCALE + YOFF,
  },
  .convex = {
    [1] = 1,
    [2] = 1,
    [13] = 1,
    [14] = 1,
  },
  .transparent = 1,
  .no_highlight = 1,
  .move_line_colours = 1,
  .line_colours = {
    0.450f, 0.840f, 0.060f, 0.200f, 0.373f, 0.027f,
    0.633f, 0.715f, 0.002f, 0.281f, 0.318f, 0.001f,
    0.784f, 0.544f, 0.022f, 0.349f, 0.242f, 0.010f,
    0.878f, 0.356f, 0.116f, 0.390f, 0.158f, 0.051f,
    0.898f, 0.185f, 0.267f, 0.399f, 0.082f, 0.119f,
    0.840f, 0.060f, 0.450f, 0.373f, 0.027f, 0.200f,
    0.715f, 0.002f, 0.633f, 0.318f, 0.001f, 0.281f,
    0.544f, 0.022f, 0.784f, 0.242f, 0.010f, 0.349f,
    0.356f, 0.116f, 0.878f, 0.158f, 0.051f, 0.390f,
    0.185f, 0.267f, 0.898f, 0.082f, 0.119f, 0.399f,
    0.060f, 0.450f, 0.840f, 0.027f, 0.200f, 0.373f,
    0.002f, 0.633f, 0.715f, 0.001f, 0.281f, 0.318f,
    0.022f, 0.784f, 0.544f, 0.010f, 0.349f, 0.242f,
    0.116f, 0.878f, 0.356f, 0.051f, 0.390f, 0.158f,
    0.267f, 0.898f, 0.185f, 0.119f, 0.399f, 0.082f,
  },
  .start_position = 7,

  .spiker_num = 1,
  .spiker_delay = 0,

  .stage_length = 37000,
  .foes = {
    {
      .type = STAGE_E_FOE_PULSAR,
      .initial_entry = 0,
      .jitter = 200,
      .count = 10,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 0,
      .jitter = 200,
      .count = 3,
    },
    {
      .type = STAGE_E_FOE_MIRROR,
      .initial_entry = 2000,
      .jitter = 200,
      .count = 6,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
