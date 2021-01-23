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
#include "line.h"
#include "menu.h"
#include "engine.h"
#include "matrix.h"
#include "shaders.h"
#include "highscore.h"
#include "object.h"
#include "starfield.h"
#include "orthoquad.h"
#include "char_coords.h"
#include "menu_graphics/menu_logo.h"
#include "menu_graphics/menu_2000.h"

static char *pups_titles[PUPS_ITEM_NUM] = {
  "PARTICLE LASER",
  "JUMP ENABLED",
  "A.I. DROID",
  "EXTRA LIFE",
};

static char *pups_descs[PUPS_ITEM_NUM] = {
  "START EACH STAGE WITH PARTICLE LASER",
  "START EACH STAGE WITH JUMP ENABLED",
  "START EACH STAGE WITH THE A.I. DROID",
  "EXTRA LIFE WHEN STARTING NEW GAME",
};

#define PUPS_1000 0
#define PUPS_2000 1
#define PUPS_3000 2
#define PUPS_5000 3
#define PUPS_DONE 4

static int pups_prices[PUPS_ITEM_NUM] = {
  PUPS_2000,
  PUPS_3000,
  PUPS_5000,
  PUPS_1000,
};

static int pups_onetime[PUPS_ITEM_NUM] = {
  1,
  1,
  1,
  0,
};

static int pups_price_values[] = {
  1000,
  2000,
  3000,
  5000,
  0,
};

static GLfloat pups_prices_tex[] = {
  /* 1000 */
  0.000f, 0.000f,
  0.633f, 0.000f,
  0.000f, 0.250f,
  0.633f, 0.250f,

  /* 2000 */
  0.000f, 0.250f,
  0.633f, 0.250f,
  0.000f, 0.500f,
  0.633f, 0.500f,

  /* 3000 */
  0.000f, 0.500f,
  0.633f, 0.500f,
  0.000f, 0.750f,
  0.633f, 0.750f,

  /* 5000 */
  0.000f, 0.750f,
  0.633f, 0.750f,
  0.000f, 1.000f,
  0.633f, 1.000f,

  /* Done */
  1.000f, 0.000f,
  1.000f, 0.633f,
  0.750f, 0.000f,
  0.750f, 0.633f,
};

static GLfloat pups_pup_tex[] = {
  0.638f, 0.761f,
  0.755f, 0.761f,
  0.638f, 1.000f,
  0.755f, 1.000f,
};

#define MENU_ICON_LEADERBOARD      0
#define MENU_ICON_POWERUPS         1
#define MENU_ICON_SETTINGS         2
#define MENU_HLICON_LEADERBOARD    3
#define MENU_HLICON_POWERUPS       4
#define MENU_HLICON_SETTINGS       5

static GLfloat menu_icons_tex[] = {
  /* leaderboard normal */
  0.391f, 0.000f,
  0.586f, 0.000f,
  0.391f, 0.391f,
  0.586f, 0.391f,

  /* powerups normal */
  0.586f, 0.000f,
  0.781f, 0.000f,
  0.586f, 0.391f,
  0.781f, 0.391f,

  /* settings normal */
  0.781f, 0.000f,
  0.977f, 0.000f,
  0.781f, 0.391f,
  0.977f, 0.391f,


  /* leaderboard highlighted */
  0.391f, 0.391f,
  0.586f, 0.391f,
  0.391f, 0.781f,
  0.586f, 0.781f,

  /* powerups highlighted */
  0.586f, 0.391f,
  0.781f, 0.391f,
  0.586f, 0.781f,
  0.781f, 0.781f,

  /* settings highlighted */
  0.781f, 0.391f,
  0.977f, 0.391f,
  0.781f, 0.781f,
  0.977f, 0.781f,
};

static float menu_mtx_view[16];

static line_t *hs_titles[HS_COL_NUM];
static hs_lines_t hs_lines;

static uint64_t update_msec, update_prev_msec;
static GLfloat logo_scale[7];
static double logo_speed[7];
static GLfloat l2k_tx;
static GLfloat l2k_scale;
static double l2k_speed;
static GLfloat text_alpha, text_alpha_disabled, text_dx, text_ddx;
static GLfloat ptext_alpha, ptext_alpha_disabled, ptext_dx, ptext_ddx;
static GLfloat ptext_ty, ptext_dty;
static line_t *continue_line;
static line_t *stage_line;
static line_t *stagesel_line;
static line_t *stageselnum_line;
static line_t *new_line;
static line_t *backarrow_line;
static line_t *arrowleft_line;
static line_t *arrowright_line;
static line_t *plus_line;
static line_t *pups_title_lines[PUPS_ITEM_NUM];
static line_t *pups_desc_lines[PUPS_ITEM_NUM];
static int stageselnum = 1;
static double timer, logo_timer;
static int logo_bump = 0;
static int starting;
static int first_menu = 1;
static int continue_disabled;
static int is_resume, is_pups, is_hs;
static int hover_selection = -1;
static int phover_selection = -2;

static object_t *sf_obj, *fade_obj;

static globject_t *globjs;
#define GLOBJ_LOGO    0
#define GLOBJ_2000    1
#define GLOBJ_NUM     2

#define STARTING_NONE      0
#define STARTING_NEWGAME   1
#define STARTING_CONTINUE  2
#define STARTING_RESUME    3
#define STARTING_PUPS      4
#define STARTING_PUPSBACK  5
#define STARTING_HS        6
#define STARTING_HSBACK    7

void menu_init(int last_stage)
{
  int i;

  DEBUG(DEBUG_MENU, "last_stage=%d", last_stage);

  globjs = calloc(GLOBJ_NUM, sizeof(*globjs));

  menu_reinit(1);

  globjs[GLOBJ_LOGO].alpha = 1.0f;
  globjs[GLOBJ_2000].alpha = 1.0f;

  starting = STARTING_NONE;
  hover_selection = -1;
  phover_selection = -2;

  if(last_stage >= 0)
    continue_disabled = 0;
  else
    continue_disabled = 1;

  is_pups = 0;
  is_hs = 0;
  ptext_alpha = 0.0;
  ptext_alpha_disabled = 0.0;
  ptext_dx = -8.0;
  ptext_ddx = 0.017;
  ptext_ty = 0.0;
  ptext_dty = 0.0;

  if(first_menu) {
    l2k_tx = 5.0;
    l2k_speed = 0.005;
    l2k_scale = 1.0;
    text_alpha = 0.0;
    text_alpha_disabled = 0.0;
    text_dx = 0.0;
    text_ddx = 0.0;
    timer = 0.0;

    for(i = 0 ; i < 7 ; i++) {
      logo_scale[i] = 0;
      logo_speed[i] = 0;
    }

    new_line = line_build("NEW GAME");
    continue_line = line_build("CONTINUE");
    is_resume = 0;
    stagesel_line = line_build("STAGE");
    arrowleft_line = line_build("<");
    arrowright_line = line_build(">");
    backarrow_line = line_build("-");

    for(i = 0 ; i < PUPS_ITEM_NUM ; i++) {
      pups_title_lines[i] = line_build(pups_titles[i]);
      pups_desc_lines[i] = line_build(pups_descs[i]);
    }
    plus_line = line_build("+");

    hs_titles[HS_COL_NAME] = line_build("NAME");
    hs_titles[HS_COL_SCORE] = line_build("SCORE");
    hs_titles[HS_COL_STAGE] = line_build("STAGE");

  } else {
    l2k_tx = 1.42;
    l2k_speed = 0.0;
    l2k_scale = 1.0;
    text_alpha = 0.8;
    text_alpha_disabled = 0.3;
    text_dx = 0.0;
    text_ddx = 0.0;
    timer = 10000.0;

    for(i = 0 ; i < 7 ; i++) {
      logo_scale[i] = 2.3;
      logo_speed[i] = 0;
    }

    IFFREE(continue_line);
    continue_line = line_build("RESUME");
    is_resume = 1;
  }

  highscore_line_build(&hs_lines);

  if(!continue_disabled) {
    char stagetxt[16];

    sprintf(stagetxt, "STAGE %d", last_stage + 1);
    IFFREE(stage_line);
    stage_line = line_build(stagetxt);
  }

  sf_obj = starfield_create(NULL, 1200, menu_mtx_view);
  FLAG_SET(sf_obj->flags, OBJECT_F_STAGE_RESTART);
}

void menu_reinit(int doalloc)
{
  if(doalloc) {
    engine_globject_alloc(&globjs[GLOBJ_LOGO]);
    engine_globject_alloc(&globjs[GLOBJ_2000]);
  }

  engine_globject_init(&globjs[GLOBJ_LOGO], SHADERS_GOURAUD,
		       menu_logo_num_vertices, menu_logo_vertices, 0,
		       menu_logo_num_colours, menu_logo_colours, 0,
		       menu_logo_index_off[7], menu_logo_indices, 0);

  engine_globject_init(&globjs[GLOBJ_2000], SHADERS_GOURAUD,
		       menu_2000_num_vertices, menu_2000_vertices, 0,
		       menu_2000_num_colours, menu_2000_colours, 0,
		       menu_2000_num_indices, menu_2000_indices, 0);
}

int menu_update(uint64_t msec)
{
  double diff;
  int i;
  char stageselnumtxt[5];

  DEBUG(DEBUG_MENU, " ");

  if(highscore_is_dirty())
    highscore_line_build(&hs_lines);

  update_prev_msec = update_msec;
  update_msec = msec;
  if(!update_prev_msec) {
    update_prev_msec = update_msec;
  } else if(update_prev_msec > update_msec) {
    DEBUG(DEBUG_TEMP, "WARNING: update_prev_msec %"PRIu64" is larger"
	  " than update_msec %"PRIu64,
	  update_prev_msec, update_msec);
    update_msec = update_prev_msec;
  }


  sf_obj->update_cb(sf_obj, update_prev_msec, update_msec);


  diff = update_msec - update_prev_msec;

  timer += diff;


  sprintf(stageselnumtxt, "%d", stageselnum);
  IFFREE(stageselnum_line);
  stageselnum_line = line_build(stageselnumtxt);


  if(starting) {
    double timer_threshold = 1000.0;

    if(starting == STARTING_NEWGAME || starting == STARTING_CONTINUE) {

      for(i = 0 ; i < 7 ; i++) {
	logo_scale[i] -= diff * (8 - i) * 0.001;
	if(logo_scale[i] < 0.0)
	  logo_scale[i] = 0.0;
      }

      l2k_scale -= diff * 0.007;
      if(l2k_scale < 0.0)
	l2k_scale = 0.0;

      text_alpha -= diff * 0.001;
      if(text_alpha < 0.0)
	text_alpha = 0.0;
      text_alpha_disabled -= diff * 0.0008;
      if(text_alpha_disabled < 0.0)
	text_alpha_disabled = 0.0;

    } else if(starting == STARTING_RESUME) {

      timer_threshold = 50.0;

    } else if(starting == STARTING_PUPS ||
	      starting == STARTING_HS) {

      timer_threshold = 600.0;

      if(timer > timer_threshold) {
	if(starting == STARTING_PUPS)
	  is_pups = 1;
	else
	  is_hs = 1;
	starting = 0;
	text_dx = 7.0;
	text_ddx = 0.017;
	ptext_dx = 0.0;
	ptext_ddx = 0.0;
	text_alpha = 0.0;
	text_alpha_disabled = 0.0;
	ptext_alpha = 0.8;
	ptext_alpha_disabled = 0.3;

	native_callback(CALLBACK_GO_VIEW, is_pups ? VIEW_PUPS : VIEW_HS, 0, 0);
      } else {
	text_alpha -= diff * 0.0015;
	if(text_alpha < 0.0)
	  text_alpha = 0.0;
	text_alpha_disabled -= diff * 0.0008;
	if(text_alpha_disabled < 0.0)
	  text_alpha_disabled = 0.0;

	text_dx += diff * text_ddx;
	text_ddx += diff * 0.00003;

	ptext_alpha += diff * 0.0015;
	if(ptext_alpha > 0.8)
	  ptext_alpha = 0.8;
	ptext_alpha_disabled += diff * 0.0008;
	if(ptext_alpha_disabled > 0.3)
	  ptext_alpha_disabled = 0.3;

	ptext_dx += diff * ptext_ddx;
	ptext_ddx -= diff * 0.00001;
	if(ptext_ddx < 0.0005)
	  ptext_ddx = 0.0005;

	if(ptext_dx > 0.0)
	  ptext_dx = 0.0;
      }

      return 1;

    } else if(starting == STARTING_PUPSBACK ||
	      starting == STARTING_HSBACK) {

      timer_threshold = 600.0;

      if(timer > timer_threshold) {
	is_pups = 0;
	is_hs = 0;
	starting = 0;
	text_dx = 0.0;
	text_ddx = 0.0;
	ptext_dx = -8.0;
	ptext_ddx = 0.017;
	text_alpha = 0.8;
	text_alpha_disabled = 0.3;
	ptext_alpha = 0.0;
	ptext_alpha_disabled = 0.0;
	ptext_dty = 0.0;

	native_callback(CALLBACK_GO_VIEW, VIEW_MENU, 0, 0);
      } else {
	text_alpha += diff * 0.0015;
	if(text_alpha > 0.8)
	  text_alpha = 0.8;
	text_alpha_disabled += diff * 0.0008;
	if(text_alpha_disabled > 0.3)
	  text_alpha_disabled = 0.3;

	text_dx -= diff * text_ddx;
	text_ddx -= diff * 0.000016;
	if(text_ddx < 0.0005)
	  text_ddx = 0.0005;

	if(text_dx < 0.0)
	  text_dx = 0.0;

	ptext_alpha -= diff * 0.0015;
	if(ptext_alpha < 0.0)
	  ptext_alpha = 0.0;
	ptext_alpha_disabled -= diff * 0.0008;
	if(ptext_alpha_disabled < 0.0)
	  ptext_alpha_disabled = 0.0;

	ptext_dx -= diff * ptext_ddx;
	ptext_ddx += diff * 0.000037;
	if(ptext_ddx < 0.0005)
	  ptext_ddx = 0.0005;

	if(ptext_dx > 0.0)
	  ptext_dx = 0.0;
      }

      return 1;
    }

    if(!fade_obj && timer > timer_threshold) {
      fade_obj = orthoquad_create(-1, 0, 0, screen_width, screen_height,
				  0.0f, 0.0f, 1.0f, 1.0f, -200,
				  0.8f, 0.8f, 0.7f);
      FLAG_SET(fade_obj->flags, OBJECT_F_STAGE_INIT);
      return 1;
    }

    if(fade_obj) {
      if(!fade_obj->update_cb(fade_obj, update_prev_msec, update_msec)) {
	fade_obj->destroy_cb(fade_obj);
	sf_obj->destroy_cb(sf_obj);
	fade_obj = NULL;
	sf_obj = NULL;

	first_menu = 0;

	return 0;
      }
    }

    return 1;
  }


  if(ptext_dty != 0.0) {
    ptext_ty -= diff * ptext_dty * 0.000007 * 720.0 / screen_height;
    if(ptext_ty < 0.0) {
      ptext_ty = 0.0;
      ptext_dty = 0.0;
    } else {
      ptext_dty *= 0.9;
    }
    if(fabs(ptext_dty) < 2)
      ptext_dty = 0.0;
  }

  /* Right now, there is no need for scrolling for pups. */
  if(ptext_ty < 0.0)
    ptext_ty = 0.0;
  if(is_pups) {
    if(ptext_ty > 0.0)
      ptext_ty = 0.0;
  } else if(is_hs) {
    if(ptext_ty > 1.4)
      ptext_ty = 1.4;
  }


  /* CYCLONE logo */
  if(timer < 4000) {
    for(i = 0 ; i < 7 ; i++) {
      if(timer < i * 180)
	break;

      logo_scale[i] += diff * logo_speed[i];

      if(logo_speed[i] < 0 && logo_scale[i] < 2.3) {
	logo_scale[i] = 2.3;
      } else if(logo_scale[i] > 2.0) {
	logo_speed[i] -= 0.0003;
      } else {
	logo_speed[i] += 0.00014;
      }
    }
    logo_timer = 0;
  } else {
    logo_timer += diff;

    if(logo_timer > 5000) {
      logo_bump = 1;
      logo_timer = 0;

      for(i = 0 ; i < 7 ; i++) {
	logo_speed[i] = 0.0;
      }
    }

    if(logo_bump) {
      for(i = 0 ; i < 7 ; i++) {
	if(logo_timer < i * 80)
	  break;

	logo_scale[i] += diff * logo_speed[i];

	if(logo_speed[i] < 0 && logo_scale[i] < 2.3) {
	  logo_scale[i] = 2.3;
	  if(i == 6)
	    logo_bump = 0;
	} else if(logo_scale[i] > 2.4) {
	  logo_speed[i] -= 0.0003;
	} else {
	  logo_speed[i] += 0.0002;
	}
      }
    } else {
      for(i = 0 ; i < 7 ; i++) {
	logo_scale[i] = 2.3;
      }
    }
  }

  if(is_pups || is_hs)
    return 1;

  /* 2000 logo */
  if(timer > 800) {
    l2k_tx -= diff * l2k_speed;
    if(l2k_tx < 1.42)
      l2k_tx = 1.42;

    l2k_speed -= diff * 0.0000035;
    if(l2k_speed < 0.0001)
      l2k_speed = 0.0001;
  }

  /* Menu selection choices */
  if(timer > 1300) {
    text_alpha += diff / 1000;
    if(text_alpha > 0.8)
      text_alpha = 0.8;
    text_alpha_disabled += 0.4 * diff / 1000;
    if(text_alpha_disabled > 0.3)
      text_alpha_disabled = 0.3;
  }


  return 1;
}


static void draw_price(int price,
		       GLfloat tx, GLfloat ty,
		       GLfloat sx, GLfloat sy,
		       GLfloat alpha)
{
  static GLfloat vertices[] = { -1.0f, 0.4f, 0.0f,
				1.0f, 0.4f, 0.0f,
				-1.0f, -0.4f, 0.0f,
				1.0f, -0.4f, 0.0f };
  float mtx_model[16];

  matrix_set_translate(mtx_model, tx, ty, 0.0f);
  matrix_scale(mtx_model, sx, sy, 1.0f);

  engine_gldirect_draw_texbox(SHADERS_TEXTURE, alpha,
			      4, vertices, &pups_prices_tex[price * 8],
			      mtx_model, menu_mtx_view, global_mtx_proj);
}

static void draw_pup(GLfloat alpha)
{
  int numchars, tmps, i, numarray[10];
  GLfloat x = 1.0;
  static GLfloat vertices[] = { -0.5f, 1.0f, 0.0f,
				0.5f, 1.0f, 0.0f,
				-0.5f, -1.0f, 0.0f,
				0.5f, -1.0f, 0.0f };
  float mtx_model1[16];

  matrix_set_translate(mtx_model1, -2.05f, 0.72f, 0.0f);
  matrix_scale(mtx_model1, 0.13f, 0.13f, 1.0f);

  engine_gldirect_draw_texbox(SHADERS_TEXTURE, alpha,
			      4, vertices, pups_pup_tex,
			      mtx_model1, menu_mtx_view, global_mtx_proj);

  tmps = engine_numpups_get();
  numchars = 0;
  do {
    numarray[10 - numchars - 1] = tmps % 10;
    numchars++;
    tmps /= 10;
  } while(tmps > 0);

  glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_CHARS));

  for(i = 0 ; i < numchars ; i++) {
    int num = numarray[10 - numchars + i];
    char_coords_t *cc = &chars[num];

    GLfloat tex_coords[] = { cc->tx1, cc->ty2,
			     cc->tx2, cc->ty2,
			     cc->tx1, cc->ty1,
			     cc->tx2, cc->ty1 };

    GLfloat w = cc->w * 1.5;
    GLfloat h = cc->h * 1.5;

    GLfloat vertices[] = { 0.0f, h,    0.0f,
			   w,    h,    0.0f,
			   0.0f, 0.0f, 0.0f,
			   w,    0.0f, 0.0f };
    float mtx_model2[16];

    matrix_translate_to(mtx_model2, mtx_model1, x, -0.7f, 0.0f);

    engine_gldirect_draw_texbox(SHADERS_TEXTURE, alpha,
				4, vertices, tex_coords,
				mtx_model2, menu_mtx_view, global_mtx_proj);

    x += w + 0.1;
  }
}

static void draw_life_plus(int exlife,
			   GLfloat tx, GLfloat ty,
			   GLfloat sx, GLfloat sy,
			   GLfloat alpha)
{
  int numchars, tmps, i, numarray[10];
  GLfloat x = 0.0;
  float mtx_model1[16];

  matrix_set_translate(mtx_model1, tx, ty, 0.0f);
  matrix_scale(mtx_model1, sx, sy, 1.0f);

  tmps = exlife;
  numchars = 0;
  do {
    numarray[10 - numchars - 1] = tmps % 10;
    numchars++;
    tmps /= 10;
  } while(tmps > 0);

  glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_CHARS));

  for(i = 0 ; i < numchars ; i++) {
    int num = numarray[10 - numchars + i];
    char_coords_t *cc = &chars[num];

    GLfloat tex_coords[] = { cc->tx1, cc->ty2,
			     cc->tx2, cc->ty2,
			     cc->tx1, cc->ty1,
			     cc->tx2, cc->ty1 };

    GLfloat w = cc->w * 1.5;
    GLfloat h = cc->h * 1.5;

    GLfloat vertices[] = { 0.0f, h,    0.0f,
			   w,    h,    0.0f,
			   0.0f, 0.0f, 0.0f,
			   w,    0.0f, 0.0f };
    float mtx_model2[16];

    matrix_translate_to(mtx_model2, mtx_model1, x, -0.7f, 0.0f);

    engine_gldirect_draw_texbox(SHADERS_TEXTURE, alpha,
				4, vertices, tex_coords,
				mtx_model2, menu_mtx_view, global_mtx_proj);

    x += w + 0.1;
  }
}

static void draw_menu_icon(int icon,
			   GLfloat tx, GLfloat ty,
			   GLfloat sx, GLfloat sy,
			   GLfloat alpha)
{
  static GLfloat vertices[] = { -1.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 0.0f,
				-1.0f, -1.0f, 0.0f,
				1.0f, -1.0f, 0.0f };
  float mtx_model[16];

  matrix_set_translate(mtx_model, tx, ty, 0.0f);
  matrix_scale(mtx_model, sx, sy, 1.0f);

  engine_gldirect_draw_texbox(SHADERS_TEXTURE, alpha,
			      4, vertices, &menu_icons_tex[icon * 8],
			      mtx_model, menu_mtx_view, global_mtx_proj);
}


static void menu_normal_draw(void)
{
  int tex = native_tex_id(TEX_CHARS);
  int tex_hover = native_tex_id(TEX_CHARS_LIGHT);
  int tex_icons = native_tex_id(TEX_MENU_ICONS);

  glDepthRangef(0.0, 0.3);

  glBindTexture(GL_TEXTURE_2D,
		(!continue_disabled &&
		 hover_selection == MENU_HOVER_CONTINUE ?
		 tex_hover : tex));
  line_draw(continue_line, -2.2 - text_dx, 0.3, 0.35, 0.35,
	    continue_disabled ? text_alpha_disabled : text_alpha,
	    menu_mtx_view, global_mtx_proj);

  glBindTexture(GL_TEXTURE_2D,
		hover_selection == MENU_HOVER_NEWGAME ? tex_hover : tex);
  line_draw(new_line, -2.2 - text_dx, -0.2, 0.35, 0.35, text_alpha,
	    menu_mtx_view, global_mtx_proj);

  glBindTexture(GL_TEXTURE_2D, tex_icons);
  draw_menu_icon((hover_selection == MENU_HOVER_LEADERBOARD ?
		  MENU_HLICON_LEADERBOARD : MENU_ICON_LEADERBOARD),
		 -1.8 - text_dx, -0.9, 0.4, 0.4, text_alpha);
  draw_menu_icon((hover_selection == MENU_HOVER_POWERUPS ?
		  MENU_HLICON_POWERUPS : MENU_ICON_POWERUPS),
		 -0.94 - text_dx, -0.9, 0.4, 0.4, text_alpha);
  draw_menu_icon((hover_selection == MENU_HOVER_SETTINGS ?
		  MENU_HLICON_SETTINGS : MENU_ICON_SETTINGS),
		 -0.06 - text_dx, -0.9, 0.4, 0.4, text_alpha);

  glBindTexture(GL_TEXTURE_2D, tex);
  if(!continue_disabled) {
    line_draw(stage_line,
	      (is_resume ? -0.53 : -0.25) - text_dx,
	      0.235, 0.2, 0.2, text_alpha,
	      menu_mtx_view, global_mtx_proj);
  }
  line_draw(stagesel_line, -0.03 - text_dx, -0.265, 0.2, 0.2, text_alpha,
	    menu_mtx_view, global_mtx_proj);
  line_draw(stageselnum_line,
	    (1.3 - 0.2 * stageselnum_line->width / 2.0) - text_dx,
	    -0.265, 0.2, 0.2, text_alpha,
	    menu_mtx_view, global_mtx_proj);
  glBindTexture(GL_TEXTURE_2D, tex);
  line_draw(arrowleft_line, 0.8 - text_dx, -0.2, 0.3, 0.35, text_alpha,
	    menu_mtx_view, global_mtx_proj);
  line_draw(arrowright_line, 1.55 - text_dx, -0.2, 0.3, 0.35, text_alpha,
	    menu_mtx_view, global_mtx_proj);

  glDepthRangef(0.0, 1.0);
}

static void menu_pups_draw(void)
{
  int tex = native_tex_id(TEX_CHARS);
  int tex_hover = native_tex_id(TEX_CHARS_LIGHT);
  int i;
  int numpups = engine_numpups_get();

  glDepthRangef(0.0, 0.3);

  for(i = 0 ; i < PUPS_ITEM_NUM ; i++) {
    glBindTexture(GL_TEXTURE_2D, phover_selection == i ? tex_hover : tex);
    line_draw(pups_title_lines[i],
	      -2.2 - ptext_dx,
	      0.35 - i * 0.55 + ptext_ty, 0.3, 0.3, ptext_alpha,
	      menu_mtx_view, global_mtx_proj);

    glBindTexture(GL_TEXTURE_2D, tex);
    line_draw(pups_desc_lines[i],
	      -2.18 - ptext_dx,
	      0.12 - i * 0.55 + ptext_ty, 0.15, 0.15, ptext_alpha,
	      menu_mtx_view, global_mtx_proj);

    if(i == PUPS_ITEM_LIFE && engine_pup_get(PUPS_ITEM_LIFE) > 0) {
      line_draw(plus_line,
		0.2 - ptext_dx,
		0.35 - i * 0.55 + ptext_ty, 0.2, 0.2, ptext_alpha,
		menu_mtx_view, global_mtx_proj);
      draw_life_plus(engine_pup_get(PUPS_ITEM_LIFE),
		     0.42 - ptext_dx,
		     0.35 - i * 0.55 + ptext_ty, 0.13, 0.13, ptext_alpha);
    }

    glBindTexture(GL_TEXTURE_2D, native_tex_id(TEX_PRICE_LIFE));
    int value = engine_pup_get(i);
    int price = pups_prices[i];
    GLfloat alpha = (pups_price_values[pups_prices[i]] > numpups ?
		     ptext_alpha_disabled : ptext_alpha);
    if(pups_onetime[i] && value) {
      price = PUPS_DONE;
      alpha = ptext_alpha;
    }
    draw_price(price,
	       1.85 - ptext_dx,
	       0.32 - i * 0.55 + ptext_ty, 0.4, 0.4, alpha);
  }

  draw_pup(ptext_alpha);

  glBindTexture(GL_TEXTURE_2D, tex);
  line_draw(backarrow_line,
	    -2.55, 0.72, 0.22, 0.22, ptext_alpha,
	    menu_mtx_view, global_mtx_proj);

  glDepthRangef(0.0, 1.0);
}

static void menu_hs_draw(void)
{
  int tex = native_tex_id(TEX_CHARS);
  int i;

  glDepthRangef(0.0, 0.3);

  glBindTexture(GL_TEXTURE_2D, tex);

  line_draw(hs_titles[HS_COL_NAME],
	    -2.05 - ptext_dx,
	    0.35, 0.3, 0.3, ptext_alpha,
	    menu_mtx_view, global_mtx_proj);
  line_draw(hs_titles[HS_COL_SCORE],
	    -0.1 - ptext_dx,
	    0.35, 0.3, 0.3, ptext_alpha,
	    menu_mtx_view, global_mtx_proj);
  line_draw(hs_titles[HS_COL_STAGE],
	    1.4 - ptext_dx,
	    0.35, 0.3, 0.3, ptext_alpha,
	    menu_mtx_view, global_mtx_proj);

  glEnable(GL_SCISSOR_TEST);
  glScissor(0, 0, screen_width, screen_height * 0.56);

  for(i = 0 ; i < HS_NUM ; i++) {
    line_draw(hs_lines.lines[i][HS_COL_POS],
	      -2.4 + 0.02 - ptext_dx,
	      -i * 0.32 + ptext_ty, 0.22, 0.22, ptext_alpha,
	      menu_mtx_view, global_mtx_proj);
    line_draw(hs_lines.lines[i][HS_COL_NAME],
	      -2.05 + 0.02 - ptext_dx,
	      -i * 0.32 + ptext_ty, 0.22, 0.22, ptext_alpha,
	      menu_mtx_view, global_mtx_proj);
    line_draw(hs_lines.lines[i][HS_COL_SCORE],
	      -0.1 + 0.02 - ptext_dx,
	      -i * 0.32 + ptext_ty, 0.22, 0.22, ptext_alpha,
	      menu_mtx_view, global_mtx_proj);
    line_draw(hs_lines.lines[i][HS_COL_STAGE],
	      1.4 + 0.02 - ptext_dx,
	      -i * 0.32 + ptext_ty, 0.22, 0.22, ptext_alpha,
	      menu_mtx_view, global_mtx_proj);
  }

  glDisable(GL_SCISSOR_TEST);

  glBindTexture(GL_TEXTURE_2D, tex);
  line_draw(backarrow_line,
	    -2.55, 0.72, 0.22, 0.22, ptext_alpha,
	    menu_mtx_view, global_mtx_proj);

  glDepthRangef(0.0, 1.0);
}

void menu_draw(int viewmode)
{
  int i;
  float tmpmtx[16];

  DEBUG(DEBUG_MENU, " ");

  if(viewmode == VIEWMODE_FULL) {
    matrix_set_translate(menu_mtx_view, 0.0f, 0.0f, -4.0f);
  } else {
    float ox;
    float tmtx[16];

    if(viewmode == VIEWMODE_LEFT)
      ox = VR_EYE_OFFSET;
    else
      ox = -VR_EYE_OFFSET;

    matrix_set_translate(menu_mtx_view, ox, 0.0f, 0.0f);
    //    matrix_rotate(menu_mtx_view, MTX_AXIS_X, -90.0f);
    matrix_multiply(tmtx, menu_mtx_view, headview_mtx);
    matrix_translate_to(menu_mtx_view, tmtx, 0.0f, 0.0f,
			native_is_vrtv() ? -7.5f : -6.0f);
  }

  if(fade_obj) {
    fade_obj->draw_cb(fade_obj);
    return;
  }

  sf_obj->draw_cb(sf_obj);

  glFrontFace(GL_CW);
  glEnable(GL_BLEND);

  matrix_set_translate(tmpmtx, 0.0f, 1.3f, 0.0f);

  engine_globject_draw_prepare_mtx(&globjs[GLOBJ_LOGO],
				   menu_mtx_view, global_mtx_proj);
  for(i = 0 ; i < 7 ; i++) {
    matrix_scale_to(globjs[GLOBJ_LOGO].mtx_model, tmpmtx,
		    logo_scale[i], logo_scale[i] * 0.124f, 1.0f);
    engine_globject_update_modelmtx(&globjs[GLOBJ_LOGO]);
    glDrawElements(GL_TRIANGLES,
		   menu_logo_index_off[i + 1] - menu_logo_index_off[i],
		   GL_UNSIGNED_SHORT,
		   (GLvoid *)(sizeof(*menu_logo_indices) *
			      menu_logo_index_off[i]));
  }
  engine_globject_draw_cleanup(&globjs[GLOBJ_LOGO]);


  matrix_set_translate(tmpmtx, l2k_tx, 0.78f, 0.0f);
  matrix_scale_to(globjs[GLOBJ_2000].mtx_model, tmpmtx,
		  l2k_scale * 0.855f * 1.0f, l2k_scale * 0.855f * 0.229f, 1.0f);
  engine_globject_draw_prepare_mtx(&globjs[GLOBJ_2000],
				   menu_mtx_view, global_mtx_proj);
  glDrawElements(GL_TRIANGLES, menu_2000_num_indices, GL_UNSIGNED_SHORT, 0);
  engine_globject_draw_cleanup(&globjs[GLOBJ_2000]);


  if((!is_pups && !is_hs) ||
     starting == STARTING_PUPSBACK || starting == STARTING_HSBACK)
    menu_normal_draw();
  if(is_pups || starting == STARTING_PUPS)
    menu_pups_draw();
  if(is_hs || starting == STARTING_HS)
    menu_hs_draw();

  glDisable(GL_BLEND);
}

static void menu_normal_choice(int choice, int data1, int data2)
{
  if(choice == MENU_CHOICE_HOVERSTEP) {
    int step = data1;
    int upmost_choice = (continue_disabled ?
			 MENU_HOVER_NEWGAME : MENU_HOVER_CONTINUE);
    int downmost_choice;

    downmost_choice = MENU_HOVER_SETTINGS;

    hover_selection += step;

    if(hover_selection <= upmost_choice)
      hover_selection = upmost_choice;

    if(hover_selection >= downmost_choice)
      hover_selection = downmost_choice;

    native_callback(CALLBACK_MENU_HOVER, hover_selection, 0, 0);

  } else if(choice == MENU_CHOICE_CONTINUE) {

    if(continue_disabled) {
      native_callback(CALLBACK_GO_VIEW, VIEW_MENU, 0, 0);
      return;
    } else {
      starting = is_resume ? STARTING_RESUME : STARTING_CONTINUE;
      timer = 0.0;
    }

  } else if(choice == MENU_CHOICE_NEWGAME) {

    engine_state_set(-stageselnum, stageselnum, -1, -1, -1, -1);
    starting = STARTING_NEWGAME;
    timer = 0.0;

  } else if(choice == MENU_CHOICE_STAGE_CHANGE) {
    int num_stages = engine_stage_num_get();

    data2++;
    if(data2 >= num_stages)
      data2 = num_stages;

    stageselnum += data1;
    if(stageselnum < 1)
      stageselnum = 1;
    else if(stageselnum > data2)
      stageselnum = data2;

  } else if(choice == MENU_CHOICE_PUPS) {

    starting = STARTING_PUPS;
    timer = 0.0;
    ptext_ty = 0.0;

  } else if(choice == MENU_CHOICE_HIGHSCORE) {

    starting = STARTING_HS;
    timer = 0.0;

  }
}

static void menu_pups_choice(int choice, int data1, int data2)
{
  if(choice == MENU_CHOICE_HOVERSTEP) {
    int step = data1;

    phover_selection += step;

    if(phover_selection <= PUPS_ITEM_FIRST)
      phover_selection = PUPS_ITEM_FIRST;

    if(phover_selection >= PUPS_ITEM_NUM)
      phover_selection = PUPS_ITEM_NUM - 1;

    /* FIXME: scroll if necessary */

  } else if(choice == MENU_GESTURE_CLOSE) {

    starting = STARTING_PUPSBACK;
    timer = 0.0;

  } else if(choice == MENU_GESTURE_SCROLL) {

    ptext_dty = 0.0;
    ptext_ty += (((GLfloat)data2) / 1000.0) * 0.0047 * 720.0 / screen_height;
    /* Right now, there is no need for scrolling. */
    if(ptext_ty < 0.0)
      ptext_ty = 0.0;
    else if(ptext_ty > 0.0)
      ptext_ty = 0.0;

  } else if(choice == MENU_GESTURE_FLING) {

    if(data1 > 2000000) {
      starting = STARTING_PUPSBACK;
      timer = 0.0;
      ptext_dty = 0.0;
    } else {
      ptext_dty = (((GLfloat)data2) / 1000.0);
    }

  } else if(choice == MENU_GESTURE_DOWN) {

    ptext_dty = 0.0;

  } else if(choice == MENU_GESTURE_TAP || choice == MENU_CHOICE_HOVERSELECT) {

    /* FIXME: Handle scrolled offset, if any. */

    float width = screen_width;
    float height = screen_height;
    float r0 = 720.0f / 1196.0f;
    float r1 = height / width;
    float rd = (r1 / r0);
    float rd0 = (1.0f - rd) / 2.0f;
    rd += rd0;

    float back_minx = rd * width * 20.0f / 1196.0f;
    float back_maxx = rd * width * 160.0f / 1196.0f;
    float back_miny = height * 150.0f / 720.0f;
    float back_maxy = height * 245.0f / 720.0f;
    float price_minx = rd * width * 900.0f / 1196.0f;
    float price_maxx = rd * width * 1100.0f / 1196.0f;
    float laser_miny = height * 245.0f / 720.0f;
    float laser_maxy = height * 330.0f / 720.0f;
    float jump_miny = height * 370.0f / 720.0f;
    float jump_maxy = height * 450.0f / 720.0f;
    float droid_miny = height * 495.0f / 720.0f;
    float droid_maxy = height * 570.0f / 720.0f;
    float life_miny = height * 615.0f / 720.0f;
    float life_maxy = height * 700.0f / 720.0f;

    float x = data1;
    float y = data2;

    int numpups = engine_numpups_get();

    // Back arrow
    if(choice == MENU_GESTURE_TAP &&
       x >= back_minx && x <= back_maxx &&
       y >= back_miny && y <= back_maxy) {
      starting = STARTING_PUPSBACK;
      timer = 0.0;
      return;
    }

    // x limits: 925 - 1100
    if(choice == MENU_GESTURE_TAP && (x < price_minx || x > price_maxx))
      return;

    if((choice == MENU_CHOICE_HOVERSELECT &&
	phover_selection == PUPS_ITEM_LASER) ||
       (choice == MENU_GESTURE_TAP && y >= laser_miny && y <= laser_maxy)) {
      // laser: 260 - 330
      if(numpups < pups_price_values[pups_prices[PUPS_ITEM_LASER]] ||
	 engine_pup_get(PUPS_ITEM_LASER))
	return;
      engine_pup_set(PUPS_ITEM_LASER, 1, 1);
      engine_numpups_decrease(pups_price_values[pups_prices[PUPS_ITEM_LASER]]);
    } else if((choice == MENU_CHOICE_HOVERSELECT &&
	       phover_selection == PUPS_ITEM_JUMP) ||
	      (choice == MENU_GESTURE_TAP && y >= jump_miny && y <= jump_maxy)) {
      // jump:  375 - 450
      if(numpups < pups_price_values[pups_prices[PUPS_ITEM_JUMP]] ||
	 engine_pup_get(PUPS_ITEM_JUMP))
	return;
      engine_pup_set(PUPS_ITEM_JUMP, 1, 1);
      engine_numpups_decrease(pups_price_values[pups_prices[PUPS_ITEM_JUMP]]);
    } else if((choice == MENU_CHOICE_HOVERSELECT &&
	       phover_selection == PUPS_ITEM_DROID) ||
	      (choice == MENU_GESTURE_TAP && y >= droid_miny && y <= droid_maxy)) {
      // droid: 500 - 570
      if(numpups < pups_price_values[pups_prices[PUPS_ITEM_DROID]] ||
	 engine_pup_get(PUPS_ITEM_DROID))
	return;
      engine_pup_set(PUPS_ITEM_DROID, 1, 1);
      engine_numpups_decrease(pups_price_values[pups_prices[PUPS_ITEM_DROID]]);
    } else if((choice == MENU_CHOICE_HOVERSELECT &&
	       phover_selection == PUPS_ITEM_LIFE) ||
	      (choice == MENU_GESTURE_TAP && y >= life_miny && y <= life_maxy)) {
      // life:  620 - 700
      if(numpups < pups_price_values[pups_prices[PUPS_ITEM_LIFE]])
	return;
      int exlife = engine_pup_get(PUPS_ITEM_LIFE) + 1;
      engine_pup_set(PUPS_ITEM_LIFE, exlife, 1);
      engine_numpups_decrease(pups_price_values[pups_prices[PUPS_ITEM_LIFE]]);
    }
  }
}

static void menu_hs_choice(int choice, int data1, int data2)
{
  if(choice == MENU_GESTURE_CLOSE) {

    starting = STARTING_HSBACK;
    timer = 0.0;

  } else if(choice == MENU_GESTURE_SCROLL) {

    ptext_dty = 0.0;
    ptext_ty += (((GLfloat)data2) / 1000.0) * 0.0047 * 720.0 / screen_height;
    if(ptext_ty < 0.0)
      ptext_ty = 0.0;
    else if(ptext_ty > 1.4)
      ptext_ty = 1.4;

  } else if(choice == MENU_GESTURE_FLING) {

    if(data1 > 2000000) {
      starting = STARTING_HSBACK;
      timer = 0.0;
      ptext_dty = 0.0;
    } else {
      ptext_dty = (((GLfloat)data2) / 1000.0);
    }

  } else if(choice == MENU_GESTURE_DOWN) {

    ptext_dty = 0.0;

  } else if(choice == MENU_GESTURE_TAP) {

    float width = screen_width;
    float height = screen_height;
    float r0 = 720.0f / 1196.0f;
    float r1 = height / width;
    float rd = (r1 / r0);
    float rd0 = (1.0f - rd) / 2.0f;
    rd += rd0;

    float back_minx = rd * width * 20.0f / 1196.0f;
    float back_maxx = rd * width * 160.0f / 1196.0f;
    float back_miny = height * 150.0f / 720.0f;
    float back_maxy = height * 245.0f / 720.0f;

    float x = data1;
    float y = data2;

    // Back arrow
    if(choice == MENU_GESTURE_TAP &&
       x >= back_minx && x <= back_maxx &&
       y >= back_miny && y <= back_maxy) {
      starting = STARTING_HSBACK;
      timer = 0.0;
      return;
    }
  }
}

void menu_choice(int choice, int data1, int data2)
{
  if(is_pups)
    menu_pups_choice(choice, data1, data2);
  else
  if(is_hs)
    menu_hs_choice(choice, data1, data2);
  else
    menu_normal_choice(choice, data1, data2);
}
