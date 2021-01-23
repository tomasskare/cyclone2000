#ifndef _STAGE_0055_H_
#define _STAGE_0055_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.300
#define YSCALE 0.750
#define XOFF   0.000
#define YOFF   -0.400

static stage_t stage_0055 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 15,
  .coords = {
    0.1724 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    0.5074 * XSCALE + XOFF, 0.7191 * YSCALE + YOFF,
    0.8424 * XSCALE + XOFF, 0.1574 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, -0.4128 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.6798 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.3350 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    0.0000 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.3350 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.6798 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.4128 * YSCALE + YOFF,
    -0.8424 * XSCALE + XOFF, 0.1574 * YSCALE + YOFF,
    -0.5074 * XSCALE + XOFF, 0.7191 * YSCALE + YOFF,
    -0.1724 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
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
  .mutant_flipper_rnd = 3,
  .spiker_num = 1,
  .spiker_delay = 1000,

  .stage_length = 40000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 20,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 1500,
      .jitter = 400,
      .count = 4,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 0,
      .jitter = 200,
      .count = 11,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 1000,
      .jitter = 200,
      .count = 4,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
