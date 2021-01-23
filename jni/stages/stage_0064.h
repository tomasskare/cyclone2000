#ifndef _STAGE_0064_H_
#define _STAGE_0064_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.200
#define YSCALE 1.200
#define XOFF   -0.400
#define YOFF   -1.000

static stage_t stage_0064 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 18,
  .coords = {
    -0.6681 * XSCALE + XOFF, -0.1790 * YSCALE + YOFF,
    -0.3362 * XSCALE + XOFF, 0.1616 * YSCALE + YOFF,
    -0.6681 * XSCALE + XOFF, 0.1616 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.1616 * YSCALE + YOFF,
    -0.6681 * XSCALE + XOFF, 0.3275 * YSCALE + YOFF,
    -0.3362 * XSCALE + XOFF, 0.4934 * YSCALE + YOFF,
    0.0044 * XSCALE + XOFF, 0.6681 * YSCALE + YOFF,
    0.3362 * XSCALE + XOFF, 0.8341 * YSCALE + YOFF,
    0.6681 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, 0.6681 * YSCALE + YOFF,
    0.8341 * XSCALE + XOFF, 0.3362 * YSCALE + YOFF,
    0.6681 * XSCALE + XOFF, 0.0044 * YSCALE + YOFF,
    0.4934 * XSCALE + XOFF, -0.3362 * YSCALE + YOFF,
    0.3275 * XSCALE + XOFF, -0.6681 * YSCALE + YOFF,
    0.1616 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.1616 * XSCALE + XOFF, -0.6769 * YSCALE + YOFF,
    0.1616 * XSCALE + XOFF, -0.3450 * YSCALE + YOFF,
    -0.1528 * XSCALE + XOFF, -0.6943 * YSCALE + YOFF,
  },
  .convex = {
    [1] = 1,
    [16] = 1,
  },
  .transparent = 1,
  .no_highlight = 1,
  .move_line_colours = 1,
  .line_colours = {
    0.450f, 0.840f, 0.060f, 0.200f, 0.373f, 0.027f,
    0.604f, 0.739f, 0.007f, 0.268f, 0.329f, 0.003f,
    0.739f, 0.604f, 0.007f, 0.329f, 0.268f, 0.003f,
    0.840f, 0.450f, 0.060f, 0.373f, 0.200f, 0.027f,
    0.893f, 0.296f, 0.161f, 0.397f, 0.132f, 0.071f,
    0.893f, 0.161f, 0.296f, 0.397f, 0.071f, 0.132f,
    0.840f, 0.060f, 0.450f, 0.373f, 0.027f, 0.200f,
    0.739f, 0.007f, 0.604f, 0.329f, 0.003f, 0.268f,
    0.604f, 0.007f, 0.739f, 0.268f, 0.003f, 0.329f,
    0.450f, 0.060f, 0.840f, 0.200f, 0.027f, 0.373f,
    0.296f, 0.161f, 0.893f, 0.132f, 0.071f, 0.397f,
    0.161f, 0.296f, 0.893f, 0.071f, 0.132f, 0.397f,
    0.060f, 0.450f, 0.840f, 0.027f, 0.200f, 0.373f,
    0.007f, 0.604f, 0.739f, 0.003f, 0.268f, 0.329f,
    0.007f, 0.739f, 0.604f, 0.003f, 0.329f, 0.268f,
    0.060f, 0.840f, 0.450f, 0.027f, 0.373f, 0.200f,
    0.161f, 0.893f, 0.296f, 0.071f, 0.397f, 0.132f,
    0.296f, 0.893f, 0.161f, 0.132f, 0.397f, 0.071f,
  },
  .start_position = 13,

  .stage_length = 30000,
  .foes = {
    {
      .type = STAGE_E_FOE_PULSAR,
      .initial_entry = 0,
      .jitter = 200,
      .count = 30,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
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
