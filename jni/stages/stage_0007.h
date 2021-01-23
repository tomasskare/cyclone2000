#ifndef _STAGE_0007_H_
#define _STAGE_0007_H_

#undef XSCALE
#undef YSCALE

#define XSCALE 1.000
#define YSCALE 1.000

static stage_t stage_0007 __attribute__((unused)) = {
  .closed = 1,
  .num_coords = 14,
  .coords = {
    1.0000 * XSCALE, 1.0000 * YSCALE,
    1.0000 * XSCALE, 0.5660 * YSCALE,
    1.0000 * XSCALE, 0.1195 * YSCALE,
    0.7882 * XSCALE, -0.3208 * YSCALE,
    0.3209 * XSCALE, -0.7925 * YSCALE,
    -0.1028 * XSCALE, -1.0000 * YSCALE,
    -0.5452 * XSCALE, -1.0000 * YSCALE,
    -1.0000 * XSCALE, -1.0000 * YSCALE,
    -1.0000 * XSCALE, -0.5472 * YSCALE,
    -1.0000 * XSCALE, -0.1195 * YSCALE,
    -0.7570 * XSCALE, 0.3270 * YSCALE,
    -0.3333 * XSCALE, 0.7799 * YSCALE,
    0.1277 * XSCALE, 1.0000 * YSCALE,
    0.5576 * XSCALE, 1.0000 * YSCALE,
  },
  .move_colours = 0,
  .colours = {
    0.000, 0.000, 0.606,
    0.695, 0.393, 0.287,
    0.000, 0.000, 0.606,
  },
  .line_colours = {0.15f, 0.15f, 0.65f, 0.05f, 0.05f, 0.40f},
  .start_position = 5,

  .mutant_tanker_rnd = 8,
  .mutant_flipper_rnd = 150,
  .spiker_num = 1,
  .spiker_delay = 3500,

  .stage_length = 36000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 24,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 1500,
      .jitter = 200,
      .count = 6,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
