#include <android/log.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <sys/queue.h>
#include <inttypes.h>

#include <GLES2/gl2.h>

#include "debug.h"
#include "native_main.h"
#include "matrix.h"
#include "shaders.h"
#include "object.h"
#include "control.h"
#include "line.h"
#include "highscore.h"
#include "score.h"
#include "life.h"
#include "orthoquad.h"
#include "starfield.h"
#include "gamearea.h"
#include "powerup.h"
#include "banner.h"
#include "text.h"
#include "player.h"
#include "player_fire.h"
#include "player_death.h"
#include "superzapper.h"
#include "ai_droid.h"
#include "engine.h"
#include "stage.h"
#include "stage_defs.h"
#include "foe_flipper.h"
#include "foe_tanker_flipper.h"
#include "foe_spiker.h"
#include "foe_spikes.h"
#include "foe_demonhead.h"
#include "foe_fuseball.h"
#include "foe_pulsar.h"
#include "foe_tanker_fuseball.h"
#include "foe_tanker_pulsar.h"
#include "foe_mirror.h"
#include "foe_ufo.h"


gllight_t engine_light1 = {
  .type = GLLIGHT_T_DIRECTIONAL,
  .u = {
    .dir = {
      .direction = { 0.0f, 0.0f, 1.0f },
      .halfplane = { 0.0f, 0.0f, 1.0f }
    },
  },
  .ambient = { 0.5f, 0.5f, 0.5f, 1.0f },
  .diffuse = { 1.0f, 1.0f, 1.0f, 1.0f },
  .specular = { 1.0f, 1.0f, 1.0f, 1.0f }
};

// Follows light1, but more ambient. Used for gamearea.
gllight_t engine_light2 = {
  .type = GLLIGHT_T_DIRECTIONAL,
  .u = {
    .dir = {
      .direction = { 0.0f, 0.0f, 1.0f },
      .halfplane = { 0.0f, 0.0f, 1.0f }
    },
  },
  .ambient = { 0.7f, 0.7f, 0.7f, 1.0f },
  .diffuse = { 1.0f, 1.0f, 1.0f, 1.0f },
  .specular = { 1.0f, 1.0f, 1.0f, 1.0f }
};

static float light1_rot_x = 0.0f, light1_rot_y = 0.0f;

#define GLLIGHT_MAX 10
static gllight_t engine_lights[GLLIGHT_MAX];

static TAILQ_HEAD(, object_s) object_list;
static TAILQ_HEAD(, object_s) object_list_2; // Always drawn after object_list

#define SPIKER_MAX_NUM 10

static float engine_mtx_view[16];

static object_t *sf_obj, *ga_obj, *pl_obj, *pd_obj, *sz_obj;
static object_t *ctrl_obj, *fade_obj;
static object_t *score_obj, *pdf_obj, *text_obj, *life_obj;
static object_t **spikes_objs;
static int spiker_timers[SPIKER_MAX_NUM], spiker_active[SPIKER_MAX_NUM];
static int num_spiker;
static double avoid_spike_delay;
static stage_t *stage, *stages[100];
static GLfloat vpx, vpy, vpz;
static int num_stages, curr_stage, next_stage, start_stage;
static int has_zapped, jump_enabled, autofire_enabled;
static int has_entered_second_life, has_entered_second_score;
static uint64_t update_paused_msec, update_paused_prev_msec;
static uint64_t update_msec, update_prev_msec;
static uint64_t update_msec_offset, update_pause_msec;
static uint64_t init_msec;
static int num_foes, num_edgefoes, num_powerups, num_until_powerup;
static int powerup_ai_droid, got_ai_droid;
static uint64_t life_sound_delay;
static int total_powerups;
static int disable_pups;
static int pups_values[PUPS_ITEM_NUM];

static void engine_text_set(char *string, int move)
{
  if(text_obj) {
    text_obj->destroy_cb(text_obj);
    text_obj = NULL;
  }

  if(string)
    text_obj = text_create(ga_obj, string, move);
}

static void engine_reset(int full_reset)
{
  object_t *obj;
  int i;

  // Delete all old objects
  while((obj = TAILQ_FIRST(&object_list))) {
    engine_object_remove(obj);
    if(obj->destroy_cb)
      obj->destroy_cb(obj);
  }

  while((obj = TAILQ_FIRST(&object_list_2))) {
    engine_object_remove(obj);
    if(obj->destroy_cb)
      obj->destroy_cb(obj);
  }

  if(pd_obj) {
    pd_obj->destroy_cb(pd_obj);
    pd_obj = NULL;
  }

  if(pdf_obj) {
    pdf_obj->destroy_cb(pdf_obj);
    pdf_obj = NULL;
  }

  if(fade_obj) {
    fade_obj->destroy_cb(fade_obj);
    fade_obj = NULL;
  }

  if(sz_obj) {
    sz_obj->destroy_cb(sz_obj);
    sz_obj = NULL;
  }

  if(spikes_objs) {
    free(spikes_objs);
    spikes_objs = NULL;
  }

  sf_obj = NULL;
  ga_obj = NULL;
  pl_obj = NULL;

  for(i = 0 ; i < GLLIGHT_MAX ; i++)
    engine_gllight_destroy(&engine_lights[i]);

  engine_text_set(NULL, 0);

  num_foes = 0;
  num_edgefoes = 0;

  update_msec_offset = native_gettime();
  update_msec = 0;
  update_prev_msec = 0;
  update_pause_msec = 0;
  init_msec = 0;
  next_stage = 0;
  num_powerups = 0;
  num_until_powerup = 0;
  got_ai_droid = disable_pups ? 0 : engine_pup_get(PUPS_ITEM_DROID);
  has_zapped = 0;
  jump_enabled = disable_pups ? 0 : engine_pup_get(PUPS_ITEM_JUMP);
  life_sound_delay = 0;
  vpx = 0.0;
  vpy = 0.0;
  vpz = gamearea_viewdist();
  num_spiker = 0;
  avoid_spike_delay = 0;
  for(i = 0 ; i < SPIKER_MAX_NUM ; i++) {
    spiker_timers[i] = 0;
    spiker_active[i] = 0;
  }

  if(full_reset) {
    curr_stage = -1;

    if(score_obj)
      score_obj->destroy_cb(score_obj);
    if(life_obj)
      life_obj->destroy_cb(life_obj);

    score_obj = score_create();
    life_obj =
      life_create(3 + (disable_pups ? 0 : engine_pup_get(PUPS_ITEM_LIFE)));
  }

  control_enable_set(ctrl_obj,
		     (disable_pups ? 0 : engine_pup_get(PUPS_ITEM_JUMP)), 1);
  control_enable_exit_set(ctrl_obj, 0, 1);
}

static int engine_stage_step_cmp(const void *a, const void *b)
{
  stage_step_t *sa = (stage_step_t *)a;
  stage_step_t *sb = (stage_step_t *)b;

  return sa->entry - sb->entry;
}

static void engine_stage_build_steps(stage_t *stage)
{
  int i, c, s, numsteps;

  if(stage->inited)
    return;

  /* Count total number of steps. */
  numsteps = 0;
  for(i = 0 ; stage->foes[i].type != STAGE_E_END ; i++) {
    numsteps += stage->foes[i].count;
    stage->foes[i].interval = stage->stage_length / (stage->foes[i].count + 1);
  }

  stage->steps = calloc(numsteps + 1, sizeof(*stage->steps));

  /* Create the full list unsorted first. */

  s = 0;
  for(i = 0 ; stage->foes[i].type != STAGE_E_END ; i++) {
    for(c = 1 ; c <= stage->foes[i].count ; c++) {
      int rnd = 0;

      if(stage->foes[i].jitter)
	rnd = (rand() % stage->foes[i].jitter) - stage->foes[i].jitter / 2;

      stage->steps[s].event = stage->foes[i].type;
      stage->steps[s++].entry =
	stage->foes[i].initial_entry + c * stage->foes[i].interval + rnd;
    }
  }

  /* Sort on entry time. */
  qsort(stage->steps, numsteps, sizeof(*stage->steps), engine_stage_step_cmp);

  /* Add the END event. */
  stage->steps[s].event = STAGE_E_END;

  stage->inited = 1;
}

static void engine_stage_init(stage_t *newstage, int isnewstage)
{
  native_callback(CALLBACK_GAME_INFO, native_is_paused(), curr_stage,
		  isnewstage);

  stage = newstage;
  stage->step = 0;
  engine_stage_build_steps(stage);

  engine_reset(0);

  ga_obj = gamearea_create(stage);
  sf_obj = starfield_create(ga_obj, 0, engine_mtx_view);
  pl_obj = player_create(ga_obj);
  if(isnewstage) {
    fade_obj = orthoquad_create(-1, 0, 0, screen_width, screen_height,
				0.0f, 0.0f, 1.0f, 1.0f, -200,
				0.8f, 0.8f, 0.7f);
    FLAG_SET(fade_obj->flags, OBJECT_F_STAGE_INIT);

    // Extra bonus score the first time coming to second stage.
    if(curr_stage > start_stage && !has_entered_second_score) {
      int bonus = start_stage * 200 * (start_stage + 13);
      has_entered_second_score++;
      score_increase_nolife(bonus);
    }
  }

  FLAG_SET(ga_obj->flags, OBJECT_F_STAGE_INIT);
  FLAG_SET(sf_obj->flags, OBJECT_F_STAGE_INIT);
  FLAG_SET(pl_obj->flags, OBJECT_F_STAGE_INIT);
  if(!isnewstage) {
    FLAG_SET(ga_obj->flags, OBJECT_F_STAGE_RESTART);
    FLAG_SET(sf_obj->flags, OBJECT_F_STAGE_RESTART);
  }

  engine_object_add(sf_obj, 1);
  engine_object_add(ga_obj, 1);
  engine_object_add(pl_obj, 1);

  spikes_objs = calloc(gamearea_num_sections_get(ga_obj),
		       sizeof(*spikes_objs));

  if(!disable_pups && engine_pup_get(PUPS_ITEM_LASER))
    gamearea_shoot_type_set(ga_obj, PLAYER_FIRE_T_LASER);

  if(!disable_pups && engine_pup_get(PUPS_ITEM_JUMP))
    gamearea_jump_enable(ga_obj);

  if(isnewstage) {
    engine_text_set("SUPERZAPPER\nRECHARGE", 1);
    native_callback(CALLBACK_PLAY_SOUND, SOUND_SUPERZAPPER, 0, 0);
  }
}


void engine_init(int control_type, int autofire, int ctrlsize, int buttonsize)
{
  DEBUG(DEBUG_INIT, " ");

  TAILQ_INIT(&object_list);
  TAILQ_INIT(&object_list_2);

  num_stages = stage_defs_init(stages);

  autofire_enabled = autofire;

  ctrl_obj = control_create(control_type, autofire, ctrlsize, buttonsize);

  engine_reset(1);
}


void engine_reinit(int doalloc)
{
  object_t *obj;

#define REINIT(o) do {				\
  if(o && o->reinit_cb)				\
    o->reinit_cb(o, doalloc);			\
} while(0)

  REINIT(ctrl_obj);
  REINIT(pd_obj);
  REINIT(pdf_obj);
  REINIT(fade_obj);
  REINIT(text_obj);
  REINIT(sz_obj);
  REINIT(score_obj);
  REINIT(life_obj);

  TAILQ_FOREACH(obj, &object_list, node) {
    REINIT(obj);
  }

  TAILQ_FOREACH(obj, &object_list_2, node) {
    REINIT(obj);
  }

#undef REINIT
}

void engine_state_set(int last_stage, int last_start_stage,
		      int last_life, int last_score,
		      int num_pups, int pups_disabled)
{
  curr_stage = last_stage;
  start_stage = last_start_stage;
  if(curr_stage > start_stage) {
    has_entered_second_life = 1;
    has_entered_second_score = 1;
  } else {
    has_entered_second_life = 0;
    has_entered_second_score = 0;
  }
  score_set(last_score < 0 ? 0 : last_score);
  if(num_pups >= 0)
    total_powerups = num_pups;
  if(pups_disabled >= 0)
    disable_pups = pups_disabled;
  life_set(last_life < 0 ?
	   3 + (disable_pups ? 0 : engine_pup_get(PUPS_ITEM_LIFE)) :
	   last_life);
}

void engine_pup_set(int item, int value, int store)
{
  pups_values[item] = value;
  if(store)
    native_callback2(CALLBACK_PUPS_SAVE, item, value);
}

int engine_pup_get(int item)
{
  return pups_values[item];
}

void engine_pup_reset(void)
{
  int i;

  for(i = 0 ; i < PUPS_ITEM_NUM ; i++)
    engine_pup_set(i, 0, 0);

  total_powerups = 0;
}

void engine_pup_disable_set(int disable)
{
  disable_pups = disable;
}

int engine_state_stage_get(void)
{
  return curr_stage;
}

int engine_stage_num_get(void)
{
  return num_stages;
}

int engine_numpups_get(void)
{
  return total_powerups;
}

void engine_numpups_set(int num)
{
  total_powerups = num;
}

void engine_numpups_decrease(int num)
{
  total_powerups -= num;
}

void engine_numpups_increase(int num)
{
  total_powerups += num;
}


int engine_start(void)
{
  if(curr_stage < 0) {
    curr_stage = -curr_stage - 1;
    start_stage = curr_stage;
    has_entered_second_life = 0;
    has_entered_second_score = 0;
    engine_stage_init(stages[curr_stage], 0);
    return 0;
  } else if(ga_obj) {
    int dopause = 1;
    dopause = engine_resume(dopause);
    native_callback(CALLBACK_GAME_INFO, 1, curr_stage, 0);
    return dopause;
  } else {
    engine_stage_init(stages[curr_stage], 0);
    return 0;
  }
}

static void engine_powerup(void)
{
  int puptextid;

  total_powerups++;
  native_callback2(CALLBACK_PUPS_ADD, 1, 0);

  /* If a powerup is taken when the player is exiting, it should
   * be treated a bit differently.
   */
  if(gamearea_exiting_speed(ga_obj) > 0.0) {
    native_callback(CALLBACK_PLAY_SOUND, SOUND_YESYESYES, 0, 0);
    engine_text_set("YES! YES! YES!", 1);
    powerup_ai_droid = 1;
    return;
  }

  native_callback(CALLBACK_PLAY_SOUND, SOUND_POWERUP, 0, 0);

  /* Got powerup on exit last stage, do AI droid instead. */
  if(num_powerups == 0 && powerup_ai_droid && !got_ai_droid) {
    engine_text_set(powerup_texts[4], 1);
    powerup_ai_droid = 0;
    got_ai_droid = 1;
    engine_object_add(ai_droid_create(ga_obj), 1);
    return;
  }

  if(num_powerups == 4 && got_ai_droid)
    num_powerups++;
  if(num_powerups == 6)
    num_powerups++;

  puptextid = num_powerups;

  num_powerups++;

  switch(num_powerups) {
  case 1:
    if(gamearea_shoot_type_get(ga_obj) == PLAYER_FIRE_T_NORMAL) {
      gamearea_shoot_type_set(ga_obj, PLAYER_FIRE_T_LASER);
    } else {
      engine_banner_p2k();
      score_increase(2000);
      puptextid = 6;
    }
    break;

  case 3:
    if(!jump_enabled) {
      jump_enabled = 1;
      gamearea_jump_enable(ga_obj);
      control_enable_set(ctrl_obj, jump_enabled, has_zapped ? 0 : 1);
    } else {
      engine_banner_p2k();
      score_increase(2000);
      puptextid = 6;
    }
    break;

  case 5:
    if(!got_ai_droid) {
      engine_object_add(ai_droid_create(ga_obj), 1);
    } else {
      engine_banner_p2k();
      score_increase(2000);
      puptextid = 6;
    }
    break;

  case 6:
    if(sz_obj)
      sz_obj->destroy_cb(sz_obj);
    sz_obj = superzapper_create(ga_obj);
    break;

  default:
    engine_banner_p2k();
    score_increase(2000);
  }

  engine_text_set(powerup_texts[puptextid], 1);
}

static void engine_event(stage_step_event_t event, int data)
{
  object_t *obj;
  int rnd, rnd2 = 0, rnd3 = 0;

  switch(event) {
  case STAGE_E_FOE_FLIPPER:
    rnd = rand() % gamearea_num_sections_get(ga_obj);
    if(stage->mutant_flipper_rnd)
      rnd2 = (rand() % stage->mutant_flipper_rnd) == 0;
    obj = foe_flipper_create(ga_obj, rnd2, rnd, -10.0f, 0);
    engine_object_add(obj, 1);
    break;

  case STAGE_E_FOE_SPIKER: {
    obj = foe_spiker_create(ga_obj, data);
    engine_object_add(obj, 1);
    break;
  }

  case STAGE_E_FOE_TANKER_FLIPPER:
    rnd = rand() % gamearea_num_sections_get(ga_obj);
    if(stage->mutant_tanker_rnd) {
      rnd2 = (rand() % stage->mutant_tanker_rnd) == 0;
      rnd3 = (rand() % stage->mutant_tanker_rnd) == 0;
    }
    obj = foe_tanker_flipper_create(ga_obj, rnd2, rnd3, rnd);
    engine_object_add(obj, 1);
    break;

  case STAGE_E_FOE_DEMONHEAD:
    rnd = rand() % gamearea_num_sections_get(ga_obj);
    obj = foe_demonhead_create(ga_obj, rnd, -10.0f, DEMONHEAD_MODE_FULL);
    engine_object_add(obj, 1);
    break;

  case STAGE_E_FOE_FUSEBALL:
    rnd = rand() % gamearea_num_edges_get(ga_obj);
    obj = foe_fuseball_create(ga_obj, rnd, -10.0f);
    engine_object_add(obj, 1);
    break;

  case STAGE_E_FOE_PULSAR:
    rnd = rand() % gamearea_num_sections_get(ga_obj);
    obj = foe_pulsar_create(ga_obj, rnd, -10.0f, 0, 0);
    engine_object_add(obj, 1);
    break;

  case STAGE_E_FOE_TANKER_FUSEBALL:
    rnd = rand() % gamearea_num_sections_get(ga_obj);
    obj = foe_tanker_fuseball_create(ga_obj, rnd);
    engine_object_add(obj, 1);
    break;

  case STAGE_E_FOE_TANKER_PULSAR:
    rnd = rand() % gamearea_num_sections_get(ga_obj);
    obj = foe_tanker_pulsar_create(ga_obj, rnd);
    engine_object_add(obj, 1);
    break;

  case STAGE_E_FOE_MIRROR:
    rnd = rand() % gamearea_num_sections_get(ga_obj);
    obj = foe_mirror_create(ga_obj, rnd);
    engine_object_add(obj, 1);
    break;

  case STAGE_E_FOE_UFO:
    rnd = rand() % gamearea_num_sections_get(ga_obj);
    obj = foe_ufo_create(ga_obj, rnd);
    engine_object_add(obj, 1);
    break;

  default:
    break;
  }
}

void engine_light_update(uint64_t update_prev_msec, uint64_t update_msec)
{
  float diff = (float)(update_msec - update_prev_msec);

  light1_rot_x += 0.9f * diff / 1000.0f;
  while(light1_rot_x > 360.0f)
    light1_rot_x -= 360.0f;

  light1_rot_y += 0.7f * diff / 1000.0f;
  while(light1_rot_y > 360.0f)
    light1_rot_y -= 360.0f;

  engine_light1.dir_direction[0] = 0.7f * sin(light1_rot_x);
  engine_light1.dir_direction[1] = 0.7f * sin(light1_rot_y);

  engine_light1.dir_halfplane[0] = 0.35f * sin(light1_rot_x);
  engine_light1.dir_halfplane[1] = 0.35f * sin(light1_rot_y);

  engine_light2.dir_direction[0] = engine_light1.dir_direction[0];
  engine_light2.dir_direction[1] = engine_light1.dir_direction[1];
  engine_light2.dir_halfplane[0] = engine_light1.dir_halfplane[0];
  engine_light2.dir_halfplane[1] = engine_light1.dir_halfplane[1];
}

void engine_update_paused(uint64_t msec)
{
  DEBUG(DEBUG_UPDATE, " ");

  update_paused_prev_msec = update_paused_msec;
  update_paused_msec = msec;
  if(!update_paused_prev_msec)
    update_paused_prev_msec = update_paused_msec;

  if(gfx_details >= GFX_DETAILS_HIGH)
    engine_light_update(update_paused_prev_msec, update_paused_msec);

  ctrl_obj->update_cb(ctrl_obj, update_paused_prev_msec, update_paused_msec);
}

int engine_update(uint64_t msec)
{
  object_t *obj, *tmp;
  uint64_t diff;
  int i;

  DEBUG(DEBUG_UPDATE, " ");

  update_prev_msec = update_msec;
  update_msec = msec - update_msec_offset + update_pause_msec;
  if(!update_prev_msec) {
    update_prev_msec = update_msec;
  } else if(update_prev_msec > update_msec) {
    DEBUG(DEBUG_TEMP, "WARNING: update_prev_msec %"PRIu64" is larger"
	  " than update_msec %"PRIu64", update_msec_offset %"PRIu64
	  ", update_pause_msec %"PRIu64", msec %"PRIu64,
	  update_prev_msec, update_msec, update_msec_offset,
	  update_pause_msec, msec);
    update_msec = update_prev_msec;
  }

  diff = update_msec - update_prev_msec;

  if(gfx_details >= GFX_DETAILS_HIGH)
    engine_light_update(update_prev_msec, update_msec);

  ctrl_obj->update_cb(ctrl_obj, update_prev_msec, update_msec);

  // Stage init
  if(FLAG_TST(ga_obj->flags, OBJECT_F_STAGE_INIT)) {
    sf_obj->update_cb(sf_obj, update_prev_msec, update_msec);
    ga_obj->update_cb(ga_obj, update_prev_msec, update_msec);
    pl_obj->update_cb(pl_obj, update_prev_msec, update_msec);
    if(fade_obj) {
      if(FLAG_TST(fade_obj->flags, OBJECT_F_STAGE_INIT)) {
	fade_obj->update_cb(fade_obj, update_prev_msec, update_msec);
      } else {
	fade_obj->destroy_cb(fade_obj);
	fade_obj = NULL;
      }
    }

    if(text_obj)
      if(!text_obj->update_cb(text_obj, update_prev_msec, update_msec))
	engine_text_set(NULL, 0);

    init_msec += update_msec - update_prev_msec;
    if(init_msec > 3500) {
      FLAG_CLR(ga_obj->flags, OBJECT_F_STAGE_INIT);
      FLAG_CLR(sf_obj->flags, OBJECT_F_STAGE_INIT);
      FLAG_CLR(pl_obj->flags, OBJECT_F_STAGE_INIT);
      FLAG_CLR(ga_obj->flags, OBJECT_F_STAGE_RESTART);
      FLAG_CLR(sf_obj->flags, OBJECT_F_STAGE_RESTART);

      if(fade_obj) {
	fade_obj->destroy_cb(fade_obj);
	fade_obj = NULL;
      }

      if(!disable_pups && engine_pup_get(PUPS_ITEM_DROID)) {
	engine_object_add(ai_droid_create(ga_obj), 1);
	got_ai_droid = 1;
      }

      update_msec_offset = native_gettime();
      update_msec = 0;
      update_prev_msec = 0;
      update_pause_msec = 0;
    } else {
      return 1;
    }
  }


  // Player death sequence running
  if(pd_obj) {
    // Star field should keep running
    sf_obj->update_cb(sf_obj, update_prev_msec, update_msec);

    // Fade out image
    if(fade_obj)
      fade_obj->update_cb(fade_obj, update_prev_msec, update_msec);

    // Player death foe catcher, if caught.
    if(pdf_obj)
      pdf_obj->update_cb(pdf_obj, update_prev_msec, update_msec);

    // Run player death update, when it ends, restart stage
    if(!pd_obj->update_cb(pd_obj, update_prev_msec, update_msec)) {
      pd_obj->destroy_cb(pd_obj);
      pd_obj = NULL;
      if(pdf_obj) {
	pdf_obj->destroy_cb(pdf_obj);
	pdf_obj = NULL;
      }
      fade_obj->destroy_cb(fade_obj);
      fade_obj = NULL;

      if(life_current() == 0) {
	// No more lifes, show Game Over view
	int hscore = score_current();
	int hstage = curr_stage + 1;
	int hspos = highscore_store(NULL, hscore, hstage);

	engine_reset(1);
	engine_callback_state_save(0);
	native_callback5(CALLBACK_GO_VIEW, VIEW_GAMEOVER, 1, hspos,
			 hscore, hstage);
	return 0;
      }

      life_decrease();

      engine_stage_init(stages[curr_stage], 0);

      return 0;
    }

    if(text_obj)
      if(!text_obj->update_cb(text_obj, update_prev_msec, update_msec))
	engine_text_set(NULL, 0);

    return 1;
  }

  if(life_sound_delay && life_sound_delay < diff) {
    native_callback(CALLBACK_PLAY_SOUND, SOUND_LIFE, 0, 0);
    life_sound_delay = 0;
  } else if(life_sound_delay) {
    life_sound_delay -= diff;
  }

  // Extra life the first time coming to second stage.
  if(curr_stage > start_stage && !has_entered_second_life) {
    has_entered_second_life++;
    life_increase();
  }

  // Check if a new event should be triggered
  if(stage->steps[stage->step].event != STAGE_E_END) {
    int first_free = -1;

    // Check if a new spiker should be activated.
    for(i = 0 ; i < stage->spiker_num ; i++) {
      spiker_timers[i] += update_msec - update_prev_msec;
      if(first_free == -1 && !spiker_active[i])
	first_free = i;
    }
    if(first_free != -1 &&
       num_spiker < stage->spiker_num &&
       spiker_timers[first_free] > stage->spiker_delay) {
      engine_event(STAGE_E_FOE_SPIKER, first_free);
      spiker_active[first_free] = 1;
      num_spiker++;
    }

    if(update_msec > stage->steps[stage->step].entry) {
      engine_event(stage->steps[stage->step].event, 0);
      stage->step++;
    }
  } else if(num_foes == num_edgefoes && num_spiker == 0) {
    /* Stage is over, and there are no more enemies
     * in the web (may be on the edge).
     * Start end sequence.
     */
    gamearea_exit(ga_obj);

    if(stage->spiker_num && avoid_spike_delay == 0)
      avoid_spike_delay = 1500;

    if(avoid_spike_delay > 0)
      avoid_spike_delay -= update_msec - update_prev_msec;

    if(avoid_spike_delay < 0 && curr_stage < 16) {
      engine_text_set("AVOID THE SPIKES", 1);
      avoid_spike_delay = NAN;
    }
  }

  foe_pulsar_global_update(update_prev_msec, update_msec);

  TAILQ_FOREACH(obj, &object_list, node) {
    if(FLAG_TST(obj->flags, OBJECT_F_DESTROYED))
      continue;

    if(obj->update_cb) {
      if(!obj->update_cb(obj, update_prev_msec, update_msec)) {
	FLAG_SET(obj->flags, OBJECT_F_DESTROYED);
      }
    }
  }

  TAILQ_FOREACH(obj, &object_list_2, node) {
    if(FLAG_TST(obj->flags, OBJECT_F_DESTROYED))
      continue;

    if(obj->update_cb) {
      if(!obj->update_cb(obj, update_prev_msec, update_msec)) {
	FLAG_SET(obj->flags, OBJECT_F_DESTROYED);
      }
    }
  }

  // Destroy objects for real
  for(obj = TAILQ_FIRST(&object_list) ;
      tmp = obj ? TAILQ_NEXT(obj, node) : NULL, obj ;
      obj = tmp) {

    if(!FLAG_TST(obj->flags, OBJECT_F_DESTROYED))
      continue;

    // Spikers are respawned after a delay. Bummer.
    if(FLAG_TST(obj->flags, OBJECT_F_FOE_SPIKER)) {
      int num = foe_spiker_get_num(obj);
      spiker_active[num] = 0;
      spiker_timers[num] = 0;
      num_spiker--;
    }

    engine_object_remove(obj);
    if(obj->destroy_cb)
      obj->destroy_cb(obj);
  }

  for(obj = TAILQ_FIRST(&object_list_2) ;
      tmp = obj ? TAILQ_NEXT(obj, node) : NULL, obj ;
      obj = tmp) {

    if(!FLAG_TST(obj->flags, OBJECT_F_DESTROYED))
      continue;

    engine_object_remove(obj);
    if(obj->destroy_cb)
      obj->destroy_cb(obj);
  }

  if(sz_obj) {
    if(!sz_obj->update_cb(sz_obj, update_prev_msec, update_msec)) {
      sz_obj->destroy_cb(sz_obj);
      sz_obj = NULL;
    }
  }

  if(text_obj)
    if(!text_obj->update_cb(text_obj, update_prev_msec, update_msec))
      engine_text_set(NULL, 0);



  // Go to next stage, if we're done
  if(next_stage) {
    curr_stage++;

    if(curr_stage == num_stages) {
      int hscore = score_current();
      int hstage = curr_stage;
      int hspos = highscore_store(NULL, hscore, hstage);

      // Called to store that we have finished the last stage.
      engine_callback_state_save(1);

      // No more stages, show Congratulations view (same as Game Over)
      engine_reset(1);
      engine_callback_state_save(0);
      native_callback5(CALLBACK_GO_VIEW, VIEW_GAMEOVER, 0, hspos,
		       hscore, hstage);
      return 0;
    }

    engine_callback_state_save(1);

    engine_stage_init(stages[curr_stage], 1);
    return 0;
  }

  return 1;
}

void engine_draw(int viewmode)
{
  object_t *obj;

  DEBUG(DEBUG_DRAW, " ");

  if(viewmode == VIEWMODE_FULL) {
    matrix_set_translate(engine_mtx_view, -vpx, -vpy, -vpz);
  } else {
    float ox;
    float tmtx[16];

    if(viewmode == VIEWMODE_LEFT)
      ox = VR_EYE_OFFSET;
    else
      ox = -VR_EYE_OFFSET;

    matrix_set_translate(engine_mtx_view, ox, 0.0f, 0.0f);
    //    matrix_rotate(engine_mtx_view, MTX_AXIS_X, -90.0f);
    matrix_multiply(tmtx, engine_mtx_view, headview_mtx);
    matrix_translate_to(engine_mtx_view, tmtx, -vpx, -vpy, -vpz);
  }

  TAILQ_FOREACH(obj, &object_list, node) {
    if(obj->draw_cb)
      obj->draw_cb(obj);
  }

  if(pd_obj)
    pd_obj->draw_cb(pd_obj);

  if(pdf_obj)
    pdf_obj->draw_cb(pdf_obj);

  TAILQ_FOREACH(obj, &object_list_2, node) {
    if(obj->draw_cb)
      obj->draw_cb(obj);
  }

  if(sz_obj)
    sz_obj->draw_cb(sz_obj);

  if(fade_obj)
    fade_obj->draw_cb(fade_obj);

  if(text_obj)
    text_obj->draw_cb(text_obj);

  ctrl_obj->draw_cb(ctrl_obj);
  score_obj->draw_cb(score_obj);
  life_obj->draw_cb(life_obj);
}

void engine_player_set_control(float speed, int step,
			       int shoot, int jump, int zap)
{
  if(!ga_obj)
    return;

  gamearea_player_set_control(ga_obj, speed, step,
			      autofire_enabled ? 1 : shoot, jump);

  if(!sz_obj && zap && !has_zapped) {
    has_zapped = 1;
    control_enable_set(ctrl_obj, jump_enabled, 0);
    sz_obj = superzapper_create(ga_obj);
    engine_text_set("EAT ELECTRIC\nDEATH!", 1);
  }
}

void engine_pause(void)
{
  engine_text_set("PAUSED", 0);
  control_enable_exit_set(ctrl_obj, 1, 0);

  update_paused_msec = 0;
  update_paused_prev_msec = 0;

  update_pause_msec = update_msec;
  engine_callback_state_save(1);
}

int engine_resume(int paused)
{
  if(paused) {
    engine_text_set("PAUSED", 0);
    control_enable_exit_set(ctrl_obj, 1, 0);
  } else {
    engine_text_set(NULL, 0);
    control_enable_exit_set(ctrl_obj, 0, 0);
  }

  update_msec_offset = native_gettime();

  return paused;
}

void engine_control_type_set(int type, int autofire,
			     int ctrlsize, int buttonsize)
{
  autofire_enabled = autofire;
  control_type_set(ctrl_obj, type, autofire, ctrlsize, buttonsize);
}

void engine_object_add(object_t *obj, int listnum)
{
  if(FLAG_TST(obj->flags, OBJECT_F_FOE) &&
     !FLAG_TST(obj->flags, OBJECT_F_FOE_SPIKES))
    num_foes++;

  obj->listnum = listnum;

  if(listnum == 1)
    TAILQ_INSERT_TAIL(&object_list, obj, node);
  else
    TAILQ_INSERT_HEAD(&object_list_2, obj, node);
}

void engine_object_remove(object_t *obj)
{
  if(FLAG_TST(obj->flags, OBJECT_F_FOE) &&
     !FLAG_TST(obj->flags, OBJECT_F_FOE_SPIKES))
    num_foes--;

  if(FLAG_TST(obj->flags, OBJECT_F_FOE_EDGE))
    num_edgefoes--;

  if(obj->listnum == 1)
    TAILQ_REMOVE(&object_list, obj, node);
  else
    TAILQ_REMOVE(&object_list_2, obj, node);
}

void engine_object_onedge(object_t *obj)
{
  FLAG_SET(obj->flags, OBJECT_F_FOE_EDGE);
  num_edgefoes++;
}

int engine_object_get_num_filtered(int filterin, int filterout)
{
  object_t *obj;
  int num = 0;

  TAILQ_FOREACH(obj, &object_list, node) {
    if(FLAG_TST(obj->flags, filterin) && !FLAG_TST(obj->flags, filterout))
      num++;
  }

  return num;
}

object_t *engine_object_first_foe_get(int filterout)
{
  object_t *obj;

  TAILQ_FOREACH(obj, &object_list, node) {
    if(FLAG_TST(obj->flags, OBJECT_F_FOE) && !FLAG_TST(obj->flags, filterout))
      return obj;
  }

  return NULL;
}

object_t *engine_spikes_get_or_create(int section, int create)
{
  if(!spikes_objs[section] && create) {
    spikes_objs[section] = foe_spikes_create(ga_obj, section);
    engine_object_add(spikes_objs[section], 1);
  }

  return spikes_objs[section];
}

void engine_spikes_free(int section)
{
  if(spikes_objs[section]) {
    spikes_objs[section] = NULL;
  }
}

// Check if any object is within minz/maxz on the section
// that matches the maskflag.
int engine_collision_detect(object_t *srcobj,
			    uint32_t maskflag, int section,
			    double minz, double maxz)
{
  object_t *obj;

  if(minz > maxz) {
    double tmp = minz;
    minz = maxz;
    maxz = tmp;
  }

  TAILQ_FOREACH(obj, &object_list, node) {

    if(FLAG_TST(obj->flags, OBJECT_F_NOKILL))
      continue;

    if(!FLAG_TST(obj->flags, maskflag))
      continue;

    if(obj->collision_cb &&
       obj->collision_cb(obj, srcobj, section, minz, maxz)) {
      if(obj->hit_cb(obj, srcobj)) {
	if(obj == pl_obj) {
	  if(FLAG_TST(srcobj->flags, OBJECT_F_POWERUP)) {
	    // Player gets powerup, yay!
	    powerup_taken(srcobj);
	    engine_powerup();
	  } else if(!sz_obj &&
		    (FLAG_TST(srcobj->flags, OBJECT_F_FOE_FIRE) ||
		     FLAG_TST(srcobj->flags, OBJECT_F_FOE_FRIER))) {
	    // Player died, run death sequence.
	    // It ends in engine_update
	    FLAG_SET(obj->flags, OBJECT_F_DESTROYED);
	    pd_obj = player_death_create(ga_obj, 1);
	    if(FLAG_TST(srcobj->flags, OBJECT_F_FOE_FIRE))
	      engine_text_set("SHOT YOU!", 1);
	    else
	      engine_text_set("FRIED YOU!", 1);
	  } else if(!sz_obj &&
		    FLAG_TST(srcobj->flags, OBJECT_F_FOE_SPIKES)) {
	    // Player died on spikes.
	    // It ends in engine_update
	    FLAG_SET(obj->flags, OBJECT_F_DESTROYED);
	    pd_obj = player_death_create(ga_obj, 0);
	  } else if(!sz_obj) {
	    /* Keep player, but simulate as pd_obj. */
	    native_callback(CALLBACK_PLAY_SOUND, SOUND_WHOOO, 0, 0);
	    FLAG_SET(obj->flags, OBJECT_F_PLAYER_CAUGHT);
	    FLAG_SET(srcobj->flags, OBJECT_F_PLAYER_CAUGHT);
	    engine_object_remove(obj);
	    engine_object_remove(srcobj);
	    pd_obj = obj;
	    pdf_obj = srcobj;
	    engine_text_set("CAUGHT YOU!", 1);
	  }
	} else {

	  /* Powerups are only triggered if player shoots the foe,
	   * and not the AI droid.
	   */
	  if(FLAG_TST(srcobj->flags, OBJECT_F_PLAYER) &&
	     num_until_powerup-- == 0) {
	    if(curr_stage < 5)
	      engine_text_set("COLLECT POWERUPS!", 1);

	    engine_object_add(powerup_create(ga_obj, section,
					     (maxz + minz) / 2), 1);
	    if(curr_stage < 49)
	      num_until_powerup = 4;
	    else
	      num_until_powerup = 9;
	  }

	  FLAG_SET(obj->flags, OBJECT_F_DESTROYED);
	}
      }
      return 1;
    }
  }

  return 0;
}

void engine_kill_foe(object_t *obj)
{
  FLAG_SET(obj->flags, OBJECT_F_FORCE_KILL);
  obj->hit_cb(obj, NULL);
  FLAG_SET(obj->flags, OBJECT_F_DESTROYED);
}

/* Begin fade out. Should take fadetime msec to complete. */
void engine_fade_out(int fadetime)
{
  fade_obj = orthoquad_create(-1, 0, 0, screen_width, screen_height,
			      0.0f, 0.0f, 1.0f, 1.0f, fadetime,
			      0.0f, 0.0f, 0.0f);
}

void engine_stage_next(void)
{
  next_stage = 1;
}


void engine_viewpoint_set(GLfloat vx, GLfloat vy, GLfloat vz)
{
  if(native_is_vr() && !native_is_vrtv()) {
    vpx = 0.0f;
    vpy = 0.0f;
  } else {
    vpx = vx;
    vpy = vy;
  }
  vpz = vz;
}

void engine_viewpoint_get(GLfloat *vx, GLfloat *vy, GLfloat *vz)
{
  *vx = vpx;
  *vy = vpy;
  *vz = vpz;
}


void engine_banner_p2k(void)
{
  object_t *obj;

  obj = banner_create(ga_obj, native_tex_id(TEX_POINT_2000), 0, 0.0, 0.0, 0.0,
		      0.0, 1.0, 0.078, 0.782, 2.0, 0.7, 0);

  engine_object_add(obj, 2);
}

void engine_banner_1up(void)
{
  object_t *obj;

  life_sound_delay = 400;

  obj = banner_create(ga_obj, native_tex_id(TEX_1UP_1_GLOW),
		      400, 0.0, 0.16, 0.0,
		      0.000, 0.215, 0.000, 0.660, 0.48, 0.74, 0);

  engine_object_add(obj, 2);

  obj = banner_create(ga_obj, native_tex_id(TEX_1UP_UP), 500, 0.0, 0.0, 0.0,
		      0.048, 0.940, 0.000, 1.000, 2.00, 1.11, 0);

  engine_object_add(obj, 2);
}

int engine_should_pause(void)
{
  return 1;
}

int engine_is_autofire(void)
{
  return autofire_enabled;
}

void engine_callback_state_save(int set)
{
  if(set)
    native_callback5(CALLBACK_STATE_SAVE, curr_stage,
		     life_current(), score_current(), start_stage,
		     total_powerups);
  else
    native_callback5(CALLBACK_STATE_SAVE, -1, -1, -1, -1, total_powerups);
}

void engine_globject_alloc(globject_t *globj)
{
  glGenBuffers(GLOBJECT_VBO_NUMS, globj->vbo);
}

void engine_globject_init_ex(globject_t *globj, int shader_type,
			     int numverts, GLfloat *verts, int vdyn,
			     int numnorms, GLfloat *normals, int ndyn,
			     int numcols, GLfloat *cols, int cdyn,
			     int numidxs, GLushort *idxs, int idyn)
{
  globj->shader_type = shader_type;

  glBindBuffer(GL_ARRAY_BUFFER, globj->vbo[GLOBJECT_VBO_VERTICES]);
  glBufferData(GL_ARRAY_BUFFER, numverts * 3 * sizeof(GLfloat), verts,
	       vdyn ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

  /* Single colour updated from engine_object_set_colour() */
  if(globj->shader_type == SHADERS_GOURAUD ||
     globj->shader_type == SHADERS_LIGHT1 ||
     globj->shader_type == SHADERS_LIGHT_MULTI) {
    glBindBuffer(GL_ARRAY_BUFFER, globj->vbo[GLOBJECT_VBO_COLOURS]);
    glBufferData(GL_ARRAY_BUFFER, numcols * 3 * sizeof(GLfloat), cols,
		 cdyn ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  }

  if(globj->shader_type == SHADERS_LIGHT1 ||
     globj->shader_type == SHADERS_LIGHT_MULTI) {
    glBindBuffer(GL_ARRAY_BUFFER, globj->vbo[GLOBJECT_VBO_NORMALS]);
    glBufferData(GL_ARRAY_BUFFER, numnorms * 3 * sizeof(GLfloat), normals,
		 ndyn ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  if(globj->shader_type != SHADERS_POINT &&
     globj->shader_type != SHADERS_POINT_ROUND) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globj->vbo[GLOBJECT_VBO_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numidxs * sizeof(GLushort), idxs,
		 idyn ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  globj->program = shaders_get_program(globj->shader_type);

  globj->var[GLOBJECT_VAR_A_POSITION] =
    glGetAttribLocation(globj->program, "a_Position");

  if(globj->shader_type == SHADERS_LIGHT1 ||
     globj->shader_type == SHADERS_LIGHT_MULTI) {
    globj->var[GLOBJECT_VAR_A_NORMAL] =
      glGetAttribLocation(globj->program, "a_Normal");
  }

  if(globj->shader_type == SHADERS_GOURAUD ||
     globj->shader_type == SHADERS_LIGHT1 ||
     globj->shader_type == SHADERS_LIGHT_MULTI) {
    globj->var[GLOBJECT_VAR_A_COLOUR] =
      glGetAttribLocation(globj->program, "a_Colour");
  } else {
    globj->var[GLOBJECT_VAR_U_COLOUR] =
      glGetUniformLocation(globj->program, "u_Colour");
  }

  if(globj->shader_type == SHADERS_POINT ||
     globj->shader_type == SHADERS_POINT_ROUND) {
    globj->var[GLOBJECT_VAR_U_POINTSIZE] =
      glGetUniformLocation(globj->program, "u_PointSize");
  }

  globj->var[GLOBJECT_VAR_U_ALPHA] =
    glGetUniformLocation(globj->program, "u_Alpha");

  globj->var[GLOBJECT_VAR_U_PROJMTX] =
    glGetUniformLocation(globj->program, "u_ProjMatrix");
  globj->var[GLOBJECT_VAR_U_VIEWMTX] =
    glGetUniformLocation(globj->program, "u_ViewMatrix");
  globj->var[GLOBJECT_VAR_U_MODELMTX] =
    glGetUniformLocation(globj->program, "u_ModelMatrix");

  if(globj->shader_type == SHADERS_LIGHT1 ||
     globj->shader_type == SHADERS_LIGHT_MULTI) {
    globj->var[GLOBJECT_VAR_U_NORMALMTX] =
      glGetUniformLocation(globj->program, "u_NormalMatrix");
    globj->var[GLOBJECT_VAR_U_SPECCOLOUR] =
      glGetUniformLocation(globj->program, "u_SpecularColour");
    globj->var[GLOBJECT_VAR_U_SPECSHININESS] =
      glGetUniformLocation(globj->program, "u_SpecularShininess");
    globj->var[GLOBJECT_VAR_U_L1_DIRECTION] =
      glGetUniformLocation(globj->program, "u_Light1.direction");
    globj->var[GLOBJECT_VAR_U_L1_HALFPLANE] =
      glGetUniformLocation(globj->program, "u_Light1.halfplane");
    globj->var[GLOBJECT_VAR_U_L1_AMBIENT] =
      glGetUniformLocation(globj->program, "u_Light1.ambient");
    globj->var[GLOBJECT_VAR_U_L1_DIFFUSE] =
      glGetUniformLocation(globj->program, "u_Light1.diffuse");
    globj->var[GLOBJECT_VAR_U_L1_SPECULAR] =
      glGetUniformLocation(globj->program, "u_Light1.specular");
  }

  if(globj->shader_type == SHADERS_LIGHT_MULTI) {
    globj->var[GLOBJECT_VAR_U_LP_MTX] =
      glGetUniformLocation(globj->program, "u_LightMtx");
    globj->var[GLOBJECT_VAR_U_LP_INTENSITY] =
      glGetUniformLocation(globj->program, "u_LightIntensity");
    globj->var[GLOBJECT_VAR_U_LP_DIFFUSE] =
      glGetUniformLocation(globj->program, "u_LightDiffuse");
    globj->var[GLOBJECT_VAR_U_LP_SPECULAR] =
      glGetUniformLocation(globj->program, "u_LightSpecular");
  }
}

void engine_globject_free(globject_t *globj)
{
  engine_globject_draw_cleanup(globj);
  glDeleteBuffers(GLOBJECT_VBO_NUMS, globj->vbo);
}

void engine_globject_draw_prepare(globject_t *globj)
{
  glUseProgram(globj->program);

  glBindBuffer(GL_ARRAY_BUFFER, globj->vbo[GLOBJECT_VBO_VERTICES]);
  glVertexAttribPointer(globj->var[GLOBJECT_VAR_A_POSITION],
			3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(globj->var[GLOBJECT_VAR_A_POSITION]);

  /* Single colour updated from engine_globject_set_colour() */
  if(globj->shader_type == SHADERS_GOURAUD ||
     globj->shader_type == SHADERS_LIGHT1 ||
     globj->shader_type == SHADERS_LIGHT_MULTI) {
    glBindBuffer(GL_ARRAY_BUFFER, globj->vbo[GLOBJECT_VBO_COLOURS]);
    glVertexAttribPointer(globj->var[GLOBJECT_VAR_A_COLOUR],
			  3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(globj->var[GLOBJECT_VAR_A_COLOUR]);
  }

  if(globj->shader_type == SHADERS_LIGHT1 ||
     globj->shader_type == SHADERS_LIGHT_MULTI) {
    glBindBuffer(GL_ARRAY_BUFFER, globj->vbo[GLOBJECT_VBO_NORMALS]);
    glVertexAttribPointer(globj->var[GLOBJECT_VAR_A_NORMAL],
			  3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(globj->var[GLOBJECT_VAR_A_NORMAL]);
  }

  if(globj->shader_type != SHADERS_POINT &&
     globj->shader_type != SHADERS_POINT_ROUND)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globj->vbo[GLOBJECT_VBO_INDICES]);

  glUniformMatrix4fv(globj->var[GLOBJECT_VAR_U_VIEWMTX], 1,
		     GL_FALSE, engine_mtx_view);
  glUniformMatrix4fv(globj->var[GLOBJECT_VAR_U_PROJMTX], 1,
		     GL_FALSE, global_mtx_proj);

  glUniform1f(globj->var[GLOBJECT_VAR_U_ALPHA], globj->alpha);

  engine_globject_update_modelmtx(globj);

  if(globj->shader_type == SHADERS_POINT)
    engine_globject_set_pointsize(globj, screen_density + 2.0);
  else if(globj->shader_type == SHADERS_POINT_ROUND)
    engine_globject_set_pointsize(globj, 2.0f * (screen_density + 2.0));

  if(globj->shader_type == SHADERS_LIGHT1 ||
     globj->shader_type == SHADERS_LIGHT_MULTI) {
    glUniform4fv(globj->var[GLOBJECT_VAR_U_SPECCOLOUR], 1, globj->spec_colour);
    glUniform1f(globj->var[GLOBJECT_VAR_U_SPECSHININESS],
		globj->spec_shininess);

    glUniform3fv(globj->var[GLOBJECT_VAR_U_L1_DIRECTION], 1,
		 globj->light1->dir_direction);
    glUniform3fv(globj->var[GLOBJECT_VAR_U_L1_HALFPLANE], 1,
		 globj->light1->dir_halfplane);
    glUniform4fv(globj->var[GLOBJECT_VAR_U_L1_AMBIENT], 1,
		 globj->light1->ambient);
    glUniform4fv(globj->var[GLOBJECT_VAR_U_L1_DIFFUSE], 1,
		 globj->light1->diffuse);
    glUniform4fv(globj->var[GLOBJECT_VAR_U_L1_SPECULAR], 1,
		 globj->light1->specular);
  }

  if(globj->shader_type == SHADERS_LIGHT_MULTI) {
    GLfloat modelmtx[10 * 16];
    GLfloat intensity[10] = {0.0f};
    GLfloat diffuse[10 * 4] = {0.0f};
    GLfloat specular[10 * 4] = {0.0f};
    int i;

    for(i = 0 ; i < GLLIGHT_MAX ; i++) {
      if(engine_lights[i].type != GLLIGHT_T_POINT)
	continue;

      matrix_copy(&modelmtx[i * 16], engine_lights[i].point_modelmtx);
      intensity[i] = engine_lights[i].point_intensity;
      diffuse[i * 4 + 0] = engine_lights[i].diffuse[0];
      diffuse[i * 4 + 1] = engine_lights[i].diffuse[1];
      diffuse[i * 4 + 2] = engine_lights[i].diffuse[2];
      diffuse[i * 4 + 3] = engine_lights[i].diffuse[3];
      specular[i * 4 + 0] = engine_lights[i].specular[0];
      specular[i * 4 + 1] = engine_lights[i].specular[1];
      specular[i * 4 + 2] = engine_lights[i].specular[2];
      specular[i * 4 + 3] = engine_lights[i].specular[3];
    }

    glUniformMatrix4fv(globj->var[GLOBJECT_VAR_U_LP_MTX],
		       GLLIGHT_MAX, GL_FALSE, modelmtx);
    glUniform1fv(globj->var[GLOBJECT_VAR_U_LP_INTENSITY],
		 GLLIGHT_MAX, intensity);
    glUniform4fv(globj->var[GLOBJECT_VAR_U_LP_DIFFUSE],
		 GLLIGHT_MAX, diffuse);
    glUniform4fv(globj->var[GLOBJECT_VAR_U_LP_SPECULAR],
		 GLLIGHT_MAX, specular);
  }
}

void engine_globject_draw_prepare_mtx(globject_t *globj,
				      float *mtx_view, float *mtx_proj)
{
  engine_globject_draw_prepare(globj);

  glUniformMatrix4fv(globj->var[GLOBJECT_VAR_U_VIEWMTX], 1,
		     GL_FALSE, mtx_view);
  glUniformMatrix4fv(globj->var[GLOBJECT_VAR_U_PROJMTX], 1,
		     GL_FALSE, mtx_proj);
}

void engine_globject_draw_cleanup(globject_t *globj)
{
  glDisableVertexAttribArray(globj->var[GLOBJECT_VAR_A_POSITION]);
  if(globj->shader_type == SHADERS_GOURAUD ||
     globj->shader_type == SHADERS_LIGHT1 ||
     globj->shader_type == SHADERS_LIGHT_MULTI) {
    glDisableVertexAttribArray(globj->var[GLOBJECT_VAR_A_COLOUR]);
  }
  if(globj->shader_type == SHADERS_LIGHT1 ||
     globj->shader_type == SHADERS_LIGHT_MULTI) {
    glDisableVertexAttribArray(globj->var[GLOBJECT_VAR_A_NORMAL]);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void engine_globject_update_modelmtx(globject_t *globj)
{
  glUniformMatrix4fv(globj->var[GLOBJECT_VAR_U_MODELMTX], 1,
		     GL_FALSE, globj->mtx_model);

  if(globj->shader_type == SHADERS_LIGHT1 ||
     globj->shader_type == SHADERS_LIGHT_MULTI) {
    float normalmtx[9];
    matrix_normal_to(normalmtx, globj->mtx_model, engine_mtx_view);
    glUniformMatrix3fv(globj->var[GLOBJECT_VAR_U_NORMALMTX], 1,
		       GL_FALSE, normalmtx);
  }
}

void engine_globject_update_verts(globject_t *globj,
				  int numverts, GLfloat *verts)
{
  glBindBuffer(GL_ARRAY_BUFFER, globj->vbo[GLOBJECT_VBO_VERTICES]);
  glBufferSubData(GL_ARRAY_BUFFER, 0, numverts * 3 * sizeof(GLfloat), verts);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void engine_globject_update_normals(globject_t *globj,
				    int numnormals, GLfloat *normals)
{
  glBindBuffer(GL_ARRAY_BUFFER, globj->vbo[GLOBJECT_VBO_NORMALS]);
  glBufferSubData(GL_ARRAY_BUFFER, 0, numnormals * 3 * sizeof(GLfloat),
		  normals);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void engine_globject_update_cols(globject_t *globj,
				 int numcols, GLfloat *cols)
{
  glBindBuffer(GL_ARRAY_BUFFER, globj->vbo[GLOBJECT_VBO_COLOURS]);
  glBufferSubData(GL_ARRAY_BUFFER, 0, numcols * 3 * sizeof(GLfloat), cols);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void engine_globject_set_colour(globject_t *globj,
				GLfloat r, GLfloat g, GLfloat b)
{
  glUniform4f(globj->var[GLOBJECT_VAR_U_COLOUR], r, g, b, 1.0f);
}

void engine_globject_set_alpha(globject_t *globj, GLfloat a)
{
  glUniform1f(globj->var[GLOBJECT_VAR_U_ALPHA], a);
}

void engine_globject_set_pointsize(globject_t *globj, GLfloat ps)
{
  glUniform1f(globj->var[GLOBJECT_VAR_U_POINTSIZE], ps);
}


void engine_gldirect_draw_texbox(int shader_type, GLfloat alpha,
				 int numvertices, GLfloat *vertices,
				 GLfloat *texcoords,
				 float *mtx_model,
				 float *mtx_view,
				 float *mtx_proj)
{
  int glprogram = shaders_get_program(shader_type);
  int a_Position = glGetAttribLocation(glprogram, "a_Position");
  int a_TexCoord = glGetAttribLocation(glprogram, "a_TexCoord");
  int u_ModelMatrix = glGetUniformLocation(glprogram, "u_ModelMatrix");
  int u_ViewMatrix = glGetUniformLocation(glprogram, "u_ViewMatrix");
  int u_ProjMatrix = glGetUniformLocation(glprogram, "u_ProjMatrix");
  int u_Alpha = glGetUniformLocation(glprogram, "u_Alpha");
  int u_Texture = glGetUniformLocation(glprogram, "u_Texture");

  if(!mtx_view)
    mtx_view = engine_mtx_view;

  if(!mtx_proj)
    mtx_proj = global_mtx_proj;

  glUseProgram(glprogram);

  glVertexAttribPointer(a_Position, 3, GL_FLOAT, GL_FALSE, 0, vertices);
  glEnableVertexAttribArray(a_Position);

  glVertexAttribPointer(a_TexCoord, 2, GL_FLOAT, GL_FALSE, 0, texcoords);
  glEnableVertexAttribArray(a_TexCoord);

  glUniform1f(u_Alpha, alpha);
  glUniform1i(u_Texture, 0);

  glUniformMatrix4fv(u_ModelMatrix, 1, GL_FALSE, mtx_model);
  glUniformMatrix4fv(u_ViewMatrix, 1, GL_FALSE, mtx_view);
  glUniformMatrix4fv(u_ProjMatrix, 1, GL_FALSE, mtx_proj);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, numvertices);

  glDisableVertexAttribArray(a_Position);
  glDisableVertexAttribArray(a_TexCoord);
}

void engine_gldirect_draw_colbox(int shader_type,
				 int numvertices, GLfloat *vertices,
				 GLfloat r, GLfloat g, GLfloat b, GLfloat a,
				 float *mtx_model,
				 float *mtx_view,
				 float *mtx_proj)
{
  int glprogram = shaders_get_program(shader_type);
  int a_Position = glGetAttribLocation(glprogram, "a_Position");
  int u_ModelMatrix = glGetUniformLocation(glprogram, "u_ModelMatrix");
  int u_ViewMatrix = glGetUniformLocation(glprogram, "u_ViewMatrix");
  int u_ProjMatrix = glGetUniformLocation(glprogram, "u_ProjMatrix");
  int u_Alpha = glGetUniformLocation(glprogram, "u_Alpha");
  int u_Colour = glGetUniformLocation(glprogram, "u_Colour");

  if(!mtx_view)
    mtx_view = engine_mtx_view;

  if(!mtx_proj)
    mtx_proj = global_mtx_proj;

  glUseProgram(glprogram);

  glVertexAttribPointer(a_Position, 3, GL_FLOAT, GL_FALSE, 0, vertices);
  glEnableVertexAttribArray(a_Position);

  glUniform4f(u_Colour, r, g, b, 1.0f);
  glUniform1f(u_Alpha, a);

  glUniformMatrix4fv(u_ModelMatrix, 1, GL_FALSE, mtx_model);
  glUniformMatrix4fv(u_ViewMatrix, 1, GL_FALSE, mtx_view);
  glUniformMatrix4fv(u_ProjMatrix, 1, GL_FALSE, mtx_proj);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, numvertices);

  glDisableVertexAttribArray(a_Position);
}

void engine_gldirect_draw_points(int shader_type, GLfloat pointsize,
				 int numpoints, GLfloat *points,
				 GLfloat r, GLfloat g, GLfloat b, GLfloat a,
				 float *mtx_model,
				 float *mtx_view,
				 float *mtx_proj)
{
  int glprogram = shaders_get_program(shader_type);
  int a_Position = glGetAttribLocation(glprogram, "a_Position");
  int u_ModelMatrix = glGetUniformLocation(glprogram, "u_ModelMatrix");
  int u_ViewMatrix = glGetUniformLocation(glprogram, "u_ViewMatrix");
  int u_ProjMatrix = glGetUniformLocation(glprogram, "u_ProjMatrix");
  int u_Colour = glGetUniformLocation(glprogram, "u_Colour");
  int u_Alpha = glGetUniformLocation(glprogram, "u_Alpha");
  int u_PointSize = glGetUniformLocation(glprogram, "u_PointSize");

  if(!mtx_view)
    mtx_view = engine_mtx_view;

  if(!mtx_proj)
    mtx_proj = global_mtx_proj;

  glUseProgram(glprogram);

  glVertexAttribPointer(a_Position, 3, GL_FLOAT, GL_FALSE, 0, points);
  glEnableVertexAttribArray(a_Position);

  glUniform4f(u_Colour, r, g, b, 1.0f);
  glUniform1f(u_Alpha, a);
  glUniform1f(u_PointSize, pointsize);

  glUniformMatrix4fv(u_ModelMatrix, 1, GL_FALSE, mtx_model);
  glUniformMatrix4fv(u_ViewMatrix, 1, GL_FALSE, mtx_view);
  glUniformMatrix4fv(u_ProjMatrix, 1, GL_FALSE, mtx_proj);

  glDrawArrays(GL_POINTS, 0, numpoints);

  glDisableVertexAttribArray(a_Position);
}

gllight_t *engine_gllight_point_create(GLfloat dr, GLfloat dg, GLfloat db,
				       GLfloat sr, GLfloat sg, GLfloat sb)
{
  int i;

  for(i = 0 ; i < GLLIGHT_MAX ; i++) {
    if(engine_lights[i].type != GLLIGHT_T_UNUSED)
      continue;

    engine_lights[i].type = GLLIGHT_T_POINT;
    engine_lights[i].diffuse[0] = dr;
    engine_lights[i].diffuse[1] = dg;
    engine_lights[i].diffuse[2] = db;
    engine_lights[i].specular[0] = sr;
    engine_lights[i].specular[1] = sg;
    engine_lights[i].specular[2] = sb;

    return &engine_lights[i];
  }

  return NULL;
}

void engine_gllight_destroy(gllight_t *light)
{
  if(!light)
    return;

  light->type = GLLIGHT_T_UNUSED;
}

void engine_gllight_point_update(gllight_t *light,
				 GLfloat intensity, float *modelmtx)
{
  if(!light)
    return;

  light->point_intensity = intensity;
  matrix_copy(light->point_modelmtx, modelmtx);
}
