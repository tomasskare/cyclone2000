#ifndef _STAGE_0071_H_
#define _STAGE_0071_H_

#undef XSCALE
#undef YSCALE

#define XSCALE 1.000
#define YSCALE 1.000

static stage_t stage_0071 __attribute__((unused)) = {
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
    0.000, 0.000, 0.606},
  .line_colours = {0.90f, 0.00f, 0.60f, 0.40f, 0.00f, 0.30f},
  .start_position = 5,

  .mutant_flipper_rnd = 1,

  .stage_length = 38000,
  .foes = {
    {
      .type = STAGE_E_FOE_UFO,
      .initial_entry = 2000,
      .jitter = 500,
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_MIRROR,
      .initial_entry = 3000,
      .jitter = 500,
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 3000,
      .jitter = 500,
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_TANKER_PULSAR,
      .initial_entry = 200,
      .jitter = 500,
      .count = 31,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
