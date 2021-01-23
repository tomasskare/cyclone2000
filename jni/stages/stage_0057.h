#ifndef _STAGE_0057_H_
#define _STAGE_0057_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.100
#define YSCALE 1.100
#define XOFF   -0.800
#define YOFF   -0.500

static stage_t stage_0057 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 13,
  .coords = {
    1.0000 * XSCALE + XOFF, -0.8510 * YSCALE + YOFF,
    0.7075 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.3913 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.0672 * XSCALE + XOFF, -0.8510 * YSCALE + YOFF,
    -0.0672 * XSCALE + XOFF, -0.5451 * YSCALE + YOFF,
    -0.0672 * XSCALE + XOFF, -0.2314 * YSCALE + YOFF,
    0.0830 * XSCALE + XOFF, 0.0824 * YSCALE + YOFF,
    -0.2253 * XSCALE + XOFF, -0.0824 * YSCALE + YOFF,
    -0.5415 * XSCALE + XOFF, -0.0824 * YSCALE + YOFF,
    -0.8735 * XSCALE + XOFF, 0.0824 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.3647 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.7020 * YSCALE + YOFF,
    -0.8656 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
  },
  .convex = {
    [6] = 1,
  },
  .transparent = 1,
  .no_highlight = 1,
  .move_line_colours = 1,
  .line_colours = {
    0.450f, 0.840f, 0.060f, 0.200f, 0.373f, 0.027f,
    0.659f, 0.691f, 0.000f, 0.293f, 0.307f, 0.000f,
    0.820f, 0.486f, 0.043f, 0.365f, 0.216f, 0.019f,
    0.897f, 0.274f, 0.180f, 0.399f, 0.122f, 0.080f,
    0.871f, 0.101f, 0.378f, 0.387f, 0.045f, 0.168f,
    0.748f, 0.009f, 0.593f, 0.333f, 0.004f, 0.263f,
    0.558f, 0.018f, 0.775f, 0.248f, 0.008f, 0.344f,
    0.342f, 0.125f, 0.882f, 0.152f, 0.056f, 0.392f,
    0.152f, 0.307f, 0.891f, 0.067f, 0.137f, 0.396f,
    0.029f, 0.522f, 0.799f, 0.013f, 0.232f, 0.355f,
    0.003f, 0.720f, 0.626f, 0.001f, 0.320f, 0.278f,
    0.080f, 0.857f, 0.414f, 0.035f, 0.381f, 0.184f,
    0.241f, 0.900f, 0.209f, 0.107f, 0.400f, 0.093f,
  },
  .start_position = 7,

  .mutant_tanker_rnd = 1,
  .mutant_flipper_rnd = 1,

  .stage_length = 38000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 40,
    },
    {
      .type = STAGE_E_FOE_MIRROR,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 6,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
