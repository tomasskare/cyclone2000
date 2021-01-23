#ifndef _STAGE_0053_H_
#define _STAGE_0053_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 0.600
#define YSCALE 0.600
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_0053 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 8,
  .coords = {
    0.3333 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, 0.3333 * YSCALE + YOFF,
    1.0000 * XSCALE + XOFF, -0.3333 * YSCALE + YOFF,
    0.3333 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -0.3333 * XSCALE + XOFF, -1.0000 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, -0.3333 * YSCALE + YOFF,
    -1.0000 * XSCALE + XOFF, 0.3333 * YSCALE + YOFF,
    -0.3333 * XSCALE + XOFF, 1.0000 * YSCALE + YOFF,
  },
  .transparent = 1,
  .no_highlight = 1,
  .move_line_colours = 1,
  .line_colours = {
    0.450f, 0.840f, 0.060f, 0.200f, 0.373f, 0.027f,
    0.768f, 0.566f, 0.015f, 0.341f, 0.252f, 0.007f,
    0.900f, 0.225f, 0.225f, 0.400f, 0.100f, 0.100f,
    0.768f, 0.015f, 0.566f, 0.341f, 0.007f, 0.252f,
    0.450f, 0.060f, 0.840f, 0.200f, 0.027f, 0.373f,
    0.132f, 0.334f, 0.885f, 0.059f, 0.148f, 0.393f,
    0.000f, 0.675f, 0.675f, 0.000f, 0.300f, 0.300f,
    0.132f, 0.885f, 0.334f, 0.059f, 0.393f, 0.148f,
  },
  .start_position = 3,

  .mutant_tanker_rnd = 2,
  .mutant_flipper_rnd = 5,
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
      .initial_entry = 1000,
      .jitter = 200,
      .count = 8,
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
