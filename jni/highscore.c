#include <android/log.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <sys/queue.h>
#include <errno.h>
#include <inttypes.h>

#include <GLES2/gl2.h>

#include "debug.h"
#include "native_main.h"
#include "engine.h"
#include "matrix.h"
#include "shaders.h"
#include "line.h"
#include "highscore.h"
#include "object.h"
#include "starfield.h"
#include "orthoquad.h"
#include "char_coords.h"

#define HS_FILE_NAME "highscore.txt"

/* Fake Y values for tap gesture */
#define HS_FAKE_UP -1000
#define HS_FAKE_BACK -2000

#define NUM_ROWS 10

hs_entry_t hs_entries[HS_NUM];

static float hs_mtx_view[16];

static uint64_t update_msec, update_prev_msec;
static int inited = 0;
static line_t *title_line, *place_line, *control_line, *name_line;
static GLfloat text_alpha;
static double timer;
static int chars_tex_id, chars_tex_id_light;
static int last_score=1234567, last_stage=12;
static char last_name[HS_NAME_MAX + 1];
static int ending;
static int hs_dirty;

static object_t *sf_obj, *fade_obj;

static char *clist = "ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789.!";
static GLfloat ct[64], cs[64], curve[128];
static GLfloat ccenter;
static int cpos = 0;
static float cposf = 0.0, cposdf = 0.0;
static int down_chars;

static float charsMinx;
static float charsMaxx;
static float charsMiny;
static float charsMaxy;
static float uparrowMinx;
static float uparrowMaxx;
static float uparrowMiny;
static float uparrowMaxy;
static float backarrowMinx;
static float backarrowMaxx;
static float backarrowMiny;
static float backarrowMaxy;
static float doneMinx;
static float doneMaxx;
static float doneMiny;
static float doneMaxy;

void highscore_init(int hspos)
{
  int i, num;

  DEBUG(DEBUG_HIGHSCORE, " ");

  chars_tex_id = native_tex_id(TEX_CHARS);
  chars_tex_id_light = native_tex_id(TEX_CHARS_LIGHT);
  timer = 0.0;
  text_alpha = 0.0;
  update_msec = 0.0;
  ending = 0;

  if(!inited) {
    char *sfx, line[32];
    sfx = (hspos == 0 ? "ST" :
	   (hspos == 1 ? "ND" :
	    (hspos == 2 ? "RD" : "TH")));
    title_line = line_build("CONGRATULATIONS!");
    snprintf(line, 32, "YOU MADE %d%s PLACE", hspos + 1, sfx);
    place_line = line_build(line);
    control_line = line_build("-   ^   /");

    float r0 = 720.0f / 1196.0f;
    float r1 = (double)screen_height / (double)screen_width;
    float rd = (r1 / r0);
    rd += (1.0f - rd) / 2.0f;
    charsMinx = 0.0f;
    charsMaxx = screen_width;
    charsMiny = screen_height * 530.0f / 720.0f;
    charsMaxy = screen_height;
    uparrowMinx = rd * screen_width * 540.0f / 1196.0f;
    uparrowMaxx = rd * screen_width * 660.0f / 1196.0f;
    uparrowMiny = screen_height * 400.0f / 720.0f;
    uparrowMaxy = charsMiny;
    backarrowMinx = rd * screen_width * 370.0f / 1196.0f;
    backarrowMaxx = rd * screen_width * 510.0f / 1196.0f;
    backarrowMiny = screen_height * 400.0f / 720.0f;
    backarrowMaxy = charsMiny;
    doneMinx = rd * screen_width * 690.0f / 1196.0f;
    doneMaxx = rd * screen_width * 830.0f / 1196.0f;
    doneMiny = screen_height * 400.0f / 720.0f;
    doneMaxy = charsMiny;

    inited = 1;
  }

  if(sf_obj)
    sf_obj->destroy_cb(sf_obj);
  sf_obj = starfield_create(NULL, 1200, hs_mtx_view);
  FLAG_SET(sf_obj->flags, OBJECT_F_STAGE_RESTART);

  if(fade_obj)
    fade_obj->destroy_cb(fade_obj);

  fade_obj = orthoquad_create(-1, 0, 0, screen_width, screen_height,
			      0.0f, 0.0f, 1.0f, 1.0f, -200,
			      0.8f, 0.8f, 0.7f);
  FLAG_SET(fade_obj->flags, OBJECT_F_STAGE_INIT);


  /* Calculate size curve */

  num = strlen(clist);

  for(i = 0 ; i < num - 5 ; i++)
    curve[i] = 0.2;

  for(; i < num - 1 ; i++) {
    float a = (float)num - i - 1;
    curve[i] = (cos(a / 1.6) + 1) * 0.15 + 0.2;
  }

  curve[i++] = 0.5;

  for(; i < num + 5 ; i++) {
    float a = (float)num - i - 1;
    curve[i] = (cos(a / 1.6) + 1) * 0.15 + 0.2;
  }

  for(; i < num * 2 ; i++)
    curve[i] = 0.2;
}

int highscore_update(uint64_t msec)
{
  double diff;
  int i, num;
  GLfloat tx;

  DEBUG(DEBUG_HIGHSCORE, " ");

  if(ending)
    return 0;

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

  if(fade_obj) {
    if(!fade_obj->update_cb(fade_obj, update_prev_msec, update_msec)) {
      fade_obj->destroy_cb(fade_obj);
      fade_obj = NULL;
    }
  }

  diff = update_msec - update_prev_msec;

  if(cposdf != 0.0) {
    cposf -= diff * cposdf * 0.00003 * 720.0 / screen_height;
    if(cposf < 0.0) {
      cposf = 0.0;
      cposdf = 0.0;
    } else {
      cposdf *= 0.9;
    }
    if(fabs(cposdf) < 2)
      cposdf = 0.0;
  }

  if(cposf < 0.0)
    cposf = 0.0;
  else if(cposf > strlen(clist) - 1)
    cposf = strlen(clist) - 1;
  cpos = (int)cposf;


  num = strlen(clist);
  tx = 0.0;
  for(i = 0 ; i < num ; i++) {
    char_coords_t *cc = &chars[(int)clist[i]];
    char_coords_t *ccn = (i < num - 1 ? &chars[(int)clist[i + 1]] : NULL);
    GLfloat ccw = cc->w;
    GLfloat ccnw = ccn ? ccn->w : 0.0;

    cs[i] = curve[num - cpos - 1 + i];
    ct[i] = tx;
    if(i == cpos)
      ccenter = tx;
    tx += cs[i] * (ccw / 2.0 + ccnw / 2.0) + 0.05;
  }


  timer += diff;

  if(timer < 1000) {
    text_alpha += diff / 1000;
    if(text_alpha > 1.0)
      text_alpha = 1.0;
    return 1;
  }

  text_alpha = 1.0;

  return 1;
}

void draw_char(char c, GLfloat tx, GLfloat ty,
	       GLfloat sx, GLfloat sy)
{
  float mtx_model[16];

  matrix_set_translate(mtx_model, tx, ty, 0.0f);
  matrix_scale(mtx_model, sx, sy, 1.0f);

  char_coords_t *cc = &chars[(int)c];

  GLfloat tex_coords[] = { cc->tx2, cc->ty1,
			   cc->tx1, cc->ty1,
			   cc->tx2, cc->ty2,
			   cc->tx1, cc->ty2 };

  GLfloat vertices[] = {  cc->w / 2, -cc->h / 2, 0.0,
			  -cc->w / 2, -cc->h / 2, 0.0,
			  cc->w / 2,  cc->h / 2, 0.0,
			  -cc->w / 2,  cc->h / 2, 0.0 };

  engine_gldirect_draw_texbox(SHADERS_TEXTURE, 1.0f,
			      4, vertices, tex_coords,
			      mtx_model, hs_mtx_view, global_mtx_proj);
}

void highscore_draw(int viewmode)
{
  int i;

  DEBUG(DEBUG_HIGHSCORE, " ");

  if(viewmode == VIEWMODE_FULL) {
    matrix_set_translate(hs_mtx_view, 0.0f, 0.0f, -4.0f);
  } else {
    float ox;
    float tmtx[16];

    if(viewmode == VIEWMODE_LEFT)
      ox = VR_EYE_OFFSET;
    else
      ox = -VR_EYE_OFFSET;

    matrix_set_translate(hs_mtx_view, ox, 0.0f, 0.0f);
    matrix_multiply(tmtx, hs_mtx_view, headview_mtx);
    matrix_translate_to(hs_mtx_view, tmtx, 0.0f, 0.0f, -4.0f);
  }


  if(fade_obj) {
    fade_obj->draw_cb(fade_obj);
    return;
  }


  sf_obj->draw_cb(sf_obj);


  glFrontFace(GL_CW);
  glEnable(GL_BLEND);

  glBindTexture(GL_TEXTURE_2D, chars_tex_id);

  glDepthRangef(0.0, 0.3);

  line_draw(title_line,
	    -title_line->width * 0.35 / 2.0, 1.3,
	    0.35, 0.35, text_alpha,
	    hs_mtx_view, global_mtx_proj);
  line_draw(place_line,
	    -place_line->width * 0.25 / 2.0, 0.88,
	    0.25, 0.25, text_alpha,
	    hs_mtx_view, global_mtx_proj);
  line_draw(control_line,
	    -control_line->width * 0.3 / 2.0, -0.5,
	    0.3, 0.3, text_alpha,
	    hs_mtx_view, global_mtx_proj);
  if(name_line)
    line_draw(name_line,
	      -name_line->width * 0.4 / 2.0, 0.2,
	      0.4, 0.4, text_alpha,
	      hs_mtx_view, global_mtx_proj);

  for(i = 0 ; i < strlen(clist) ; i++) {
    if(i == cpos)
      glBindTexture(GL_TEXTURE_2D, chars_tex_id_light);
    else
      glBindTexture(GL_TEXTURE_2D, chars_tex_id);

    draw_char(clist[i], ct[i] - ccenter, -1.1, cs[i], cs[i]);
  }

  glDepthRangef(0.0, 1.0);

  glDisable(GL_BLEND);
}


void highscore_control(int choice, int data1, int data2)
{
  float x = data1;
  float y = data2;

  if(choice == MENU_GESTURE_CLOSE) {

    highscore_store(last_name, last_score, last_stage);
    ending = 1;
    timer = 0.0;

  } else if(choice == MENU_GESTURE_SCROLL) {

    if(down_chars) {
      cposdf = 0.0;
      cposf += (((GLfloat)data1) / 1000.0) * 0.02 * 720.0 / screen_height;
      if(cposf < 0.0)
	cposf = 0.0;
      else if(cposf > strlen(clist) - 1)
	cposf = strlen(clist) - 1;
    }

  } else if(choice == MENU_GESTURE_FLING) {

    if(down_chars) {
      cposdf = (((GLfloat)data1) / 1000.0);
    }

  } else if(choice == MENU_GESTURE_DOWN) {

    // y == -1000 is to fake down on chars.
    if((x >= charsMinx && x <= charsMaxx &&
	y >= charsMiny && y <= charsMaxy) || y == -1000) {
      cposdf = 0.0;
      down_chars = 1;
    } else {
      down_chars = 0;
    }

  } else if(choice == MENU_GESTURE_TAP) {

    if((x >= uparrowMinx && x <= uparrowMaxx &&
	y >= uparrowMiny && y <= uparrowMaxy) || y == HS_FAKE_UP) {
      if(strlen(last_name) < HS_NAME_MAX) {
	last_name[strlen(last_name)] = clist[cpos];
	IFFREE(name_line);
	name_line = line_build(last_name);
      }
    } else if((x >= backarrowMinx && x <= backarrowMaxx &&
	       y >= backarrowMiny && y <= backarrowMaxy) || y == HS_FAKE_BACK) {
      if(strlen(last_name) > 0) {
	last_name[strlen(last_name) - 1] = '\0';
	IFFREE(name_line);
	name_line = line_build(last_name);
      }
    } else if(x >= doneMinx && x <= doneMaxx &&
	      y >= doneMiny && y <= doneMaxy) {
      highscore_store(last_name, last_score, last_stage);
      ending = 1;
    }
  }
}


/* Functions to handle the highscore list, shown by menu.c. */

void highscore_line_build(hs_lines_t *hs_lines)
{
  int i;

  for(i = 0 ; i < HS_NUM ; i++) {
    char line[128];
    IFFREE(hs_lines->lines[i][HS_COL_POS]);
    IFFREE(hs_lines->lines[i][HS_COL_NAME]);
    IFFREE(hs_lines->lines[i][HS_COL_SCORE]);
    IFFREE(hs_lines->lines[i][HS_COL_STAGE]);
    snprintf(line, 128, "%d", i + 1);
    hs_lines->lines[i][HS_COL_POS] = line_build(line);
    hs_lines->lines[i][HS_COL_NAME] = line_build(hs_entries[i].name);
    snprintf(line, 128, "%d", hs_entries[i].score);
    hs_lines->lines[i][HS_COL_SCORE] = line_build(line);
    snprintf(line, 128, "%d", hs_entries[i].stage);
    hs_lines->lines[i][HS_COL_STAGE] = line_build(line);
  }

  hs_dirty = 0;
}

void highscore_file_read(void)
{
  char filename[256];
  FILE *fp;
  int i, num;
  char line[256];

  snprintf(filename, 256, "%s/%s", files_dir, HS_FILE_NAME);

  for(i = 0 ; i < HS_NUM ; i++) {
    snprintf(hs_entries[i].name, HS_NAME_MAX + 1, "TOMAS");
    hs_entries[i].score = (10 - i) * 10000;
    hs_entries[i].stage = (10 - i);
  }

  fp = fopen(filename, "r");
  if(fp == NULL)
    return;

  for(i = 0 ; i < HS_NUM ; i++) {
    if(feof(fp))
      break;
    fgets(line, 256, fp);
    // FIXME: HS_NAME_MAX
    num = sscanf(line, "%8c %d %d", hs_entries[i].name,
		 &hs_entries[i].score, &hs_entries[i].stage);
    if(num < 3)
      break;

    while(hs_entries[i].name[strlen(hs_entries[i].name) - 1] == ' ')
      hs_entries[i].name[strlen(hs_entries[i].name) - 1] = '\0';
  }    
  fclose(fp);
}

void highscore_file_write(void)
{
  char filename[256];
  FILE *fp;
  int i;

  snprintf(filename, 256, "%s/%s", files_dir, HS_FILE_NAME);

  fp = fopen(filename, "w");
  if(fp == NULL)
    return;

  for(i = 0 ; i < HS_NUM ; i++) {
    fprintf(fp, "%-*s %d %d\n", HS_NAME_MAX, hs_entries[i].name,
	    hs_entries[i].score, hs_entries[i].stage);
  }    
  fclose(fp);
}

/* Return value -1 means that the player didn't enter the
 * highscore list.
 */
int highscore_store(char *name, int score, int stage)
{
  int i;

  last_score = score;
  last_stage = stage;

  for(i = 0 ; i < HS_NUM ; i++) {
    if(score > hs_entries[i].score ||
       (score == hs_entries[i].score && stage > hs_entries[i].stage))
      break;
  }

  if(i == HS_NUM)
    return -1;

  if(name == NULL)
    return i;

  memmove(&hs_entries[i + 1], &hs_entries[i],
	  sizeof(*hs_entries) * (HS_NUM - i - 1));

  hs_entries[i].score = score;
  hs_entries[i].stage = stage;
  if(name != NULL) {
    strcpy(hs_entries[i].name, name);
    highscore_file_write();
  } else {
    hs_entries[i].name[0] = '\0';
  }

  return i;
}

void highscore_reset(void)
{
  char filename[256];

  snprintf(filename, 256, "%s/%s", files_dir, HS_FILE_NAME);
  unlink(filename);
  highscore_file_read();
  hs_dirty = 1;
}

int highscore_is_dirty(void)
{
  return hs_dirty;
}
