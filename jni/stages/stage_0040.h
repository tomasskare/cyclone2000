#ifndef _STAGE_0040_H_
#define _STAGE_0040_H_

#undef XSCALE
#undef YSCALE

#define XSCALE 1.600
#define YSCALE 1.000

static stage_t stage_0040 __attribute__((unused)) = {
  .closed = 0,
  .num_coords = 15,
  .coords = {
    1.0000 * XSCALE, 0.8413 * YSCALE,
    0.9006 * XSCALE, 0.3968 * YSCALE,
    0.6961 * XSCALE, 0.1958 * YSCALE,
    0.4917 * XSCALE, 0.0053 * YSCALE,
    0.2541 * XSCALE, -0.2169 * YSCALE,
    0.0497 * XSCALE, -0.4180 * YSCALE,
    -0.1547 * XSCALE, -0.6085 * YSCALE,
    -0.3481 * XSCALE, -0.7989 * YSCALE,
    -0.5691 * XSCALE, -1.0000 * YSCALE,
    -0.8011 * XSCALE, -1.0000 * YSCALE,
    -1.0000 * XSCALE, -0.5979 * YSCALE,
    -1.0000 * XSCALE, -0.1852 * YSCALE,
    -1.0000 * XSCALE, 0.1958 * YSCALE,
    -1.0000 * XSCALE, 0.5979 * YSCALE,
    -1.0000 * XSCALE, 1.0000 * YSCALE,
  },
  .move_colours = 0,
  .colours = {
    0.464, 0.413, 0.338,
    0.265, 0.067, 0.344,
    0.000, 0.000, 0.670,
  },
  .line_colours = {0.90f, 0.90f, 0.00f, 0.40f, 0.40f, 0.00f},
  .start_position = 8,

  .mutant_tanker_rnd = 5,
  .mutant_flipper_rnd = 10,
  .spiker_num = 1,
  .spiker_delay = 1000,

  .stage_length = 35000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 27,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 5,
    },
    {
      .type = STAGE_E_FOE_DEMONHEAD,
      .initial_entry = 0,
      .jitter = 500,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_FUSEBALL,
      .initial_entry = 2000,
      .jitter = 500,
      .count = 2,
    },
    {
      .type = STAGE_E_FOE_TANKER_PULSAR,
      .initial_entry = 0,
      .jitter = 1000,
      .count = 5,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
