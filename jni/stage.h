#ifndef _STAGE_H_
#define _STAGE_H_

typedef enum stage_step_event_e {
  STAGE_E_FOE_FLIPPER = 1,
  STAGE_E_FOE_SPIKER,
  STAGE_E_FOE_FUSEBALL,
  STAGE_E_FOE_PULSAR,
  STAGE_E_FOE_TANKER_FLIPPER,
  STAGE_E_FOE_TANKER_FUSEBALL,
  STAGE_E_FOE_TANKER_PULSAR,
  STAGE_E_FOE_MIRROR,
  STAGE_E_FOE_DEMONHEAD,
  STAGE_E_FOE_UFO,
  STAGE_E_END = 999
} stage_step_event_t;

typedef struct stage_step_s {
  int entry;
  stage_step_event_t event;
} stage_step_t;

typedef struct stage_foes_s {
  stage_step_event_t type;
  int initial_entry;
  int jitter;
  int count;
  int interval;
} stage_foes_t;

typedef struct stage_s {
  int closed;
  int num_coords;
  float coords[32 * 2];
  int convex[32];
  int transparent;
  int no_highlight;
  int move_colours;
  int move_line_colours;
  float colours[3 * 3];
  float line_colours[3 * 32 * 2];
  int start_position;
  int reverse_direction;
  int mutant_tanker_rnd;
  int mutant_flipper_rnd;
  int spiker_num;
  int spiker_delay;
  int stage_length;
  int step;
  int inited;
  stage_step_t *steps;
  stage_foes_t foes[];
} stage_t;

#endif
