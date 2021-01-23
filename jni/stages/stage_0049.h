#ifndef _STAGE_0049_H_
#define _STAGE_0049_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.300
#define YSCALE 1.000
#define XOFF   0.000
#define YOFF   -0.700

static stage_t stage_0049 __attribute__((unused)) = {
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
  .start_position = 9,

  .stage_length = 39000,
  .foes = {
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 1200,
      .jitter = 200,
      .count = 17,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
