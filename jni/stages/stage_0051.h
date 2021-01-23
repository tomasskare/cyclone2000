#ifndef _STAGE_0051_H_
#define _STAGE_0051_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.000
#define YSCALE 1.000
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_0051 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 15,
  .coords = {
    0.0075 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.3317 * XSCALE + XOFF, 0.6716 * YSCALE + YOFF,
    0.6658 * XSCALE + XOFF, 0.3383 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, 0.0050 * YSCALE + YOFF,
    0.8404 * XSCALE + XOFF, -0.3333 * YSCALE + YOFF,
    0.6758 * XSCALE + XOFF, -0.6667 * YSCALE + YOFF,
    0.5062 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.1671 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.1721 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.5112 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.6758 * XSCALE + XOFF, -0.6667 * YSCALE + YOFF,
    -0.8404 * XSCALE + XOFF, -0.3333 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.0050 * YSCALE + YOFF,
    -0.6658 * XSCALE + XOFF, 0.3383 * YSCALE + YOFF,
    -0.3317 * XSCALE + XOFF, 0.6716 * YSCALE + YOFF,
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

  .mutant_tanker_rnd = 2,
  .mutant_flipper_rnd = 5,
  .spiker_num = 1,
  .spiker_delay = 1000,

  .stage_length = 39000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 17,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 1500,
      .jitter = 400,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 6,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_TANKER_PULSAR,
      .initial_entry = 3000,
      .jitter = 200,
      .count = 2,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
