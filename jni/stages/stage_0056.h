#ifndef _STAGE_0056_H_
#define _STAGE_0056_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.500
#define YSCALE 1.200
#define XOFF   0.000
#define YOFF   -0.300

static stage_t stage_0056 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 14,
  .coords = {
    1.0000 * XSCALE + XOFF, -0.9871 * YSCALE + YOFF,
    0.7667 * XSCALE + XOFF, -0.8457 * YSCALE + YOFF,
    0.5236 * XSCALE + XOFF, -0.8457 * YSCALE + YOFF,
    0.2804 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.0521 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.1811 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.4144 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.6526 * XSCALE + XOFF, -0.8457 * YSCALE + YOFF,
    -0.8859 * XSCALE + XOFF, -0.5370 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.2476 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.0675 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.3826 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.6849 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
  },
  .convex = {
    [1] = 1,
    [2] = 1,
  },
  .transparent = 1,
  .no_highlight = 1,
  .move_line_colours = 1,
  .line_colours = {
    0.450f, 0.840f, 0.060f, 0.200f, 0.373f, 0.027f,
    0.645f, 0.703f, 0.001f, 0.287f, 0.313f, 0.001f,
    0.802f, 0.517f, 0.031f, 0.356f, 0.230f, 0.014f,
    0.889f, 0.317f, 0.144f, 0.395f, 0.141f, 0.064f,
    0.889f, 0.144f, 0.317f, 0.395f, 0.064f, 0.141f,
    0.802f, 0.031f, 0.517f, 0.356f, 0.014f, 0.230f,
    0.645f, 0.001f, 0.703f, 0.287f, 0.001f, 0.313f,
    0.450f, 0.060f, 0.840f, 0.200f, 0.027f, 0.373f,
    0.255f, 0.197f, 0.899f, 0.113f, 0.087f, 0.399f,
    0.098f, 0.383f, 0.869f, 0.044f, 0.170f, 0.386f,
    0.011f, 0.583f, 0.756f, 0.005f, 0.259f, 0.336f,
    0.011f, 0.756f, 0.583f, 0.005f, 0.336f, 0.259f,
    0.098f, 0.869f, 0.383f, 0.044f, 0.386f, 0.170f,
    0.255f, 0.899f, 0.197f, 0.113f, 0.399f, 0.087f,
  },
  .start_position = 4,

  .mutant_tanker_rnd = 2,
  .mutant_flipper_rnd = 3,
  .spiker_num = 1,
  .spiker_delay = 1000,

  .stage_length = 40000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 25,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 1500,
      .jitter = 400,
      .count = 6,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 0,
      .jitter = 200,
      .count = 9,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 3,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
