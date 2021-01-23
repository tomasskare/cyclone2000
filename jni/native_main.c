#include <jni.h>
#include <android/log.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/queue.h>

#include <GLES2/gl2.h>

#include "debug.h"
#include "native_main.h"
#include "matrix.h"
#include "shaders.h"
#include "line.h"
#include "engine.h"
#include "menu.h"
#include "gameover.h"
#include "highscore.h"

uint32_t debug = DEBUG_TEMP; // | DEBUG_FPS;
//uint32_t debug = DEBUG_ALL;

int screen_width;
int screen_height;
double screen_density;
GLfloat screen_margin_topbottom;
GLfloat screen_margin_leftright;
int gfx_details;
int gfx_tv;
char *files_dir;
GLfloat headview_mtx[16];
int use_multi_light;

float global_mtx_proj[16], global_mtx_ortho[16];
float global_mtx_identity[16];

static GLuint tex_ids[TEX_NUM];
static int view = VIEW_MENU;
static int running = 1;
static int paused = 0;
static int display_mode = DISPLAY_MODE_FULL;

static GLuint fbo;
static GLuint fbo_tex;
static int fbo_created = 0;

static JavaVM *jvm;
static jobject jniobject, jniclass;

static int gpu_model;

typedef void draw_f(int viewmode);

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
  jvm = vm;

  return JNI_VERSION_1_6;
}

void native_create_fbo(int width, int height)
{
  if(display_mode == DISPLAY_MODE_VR ||
     display_mode == DISPLAY_MODE_VRTV) {
    if(!fbo_created) {
      glGenTextures(1, &fbo_tex);
      glBindTexture(GL_TEXTURE_2D, fbo_tex);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width / 2, height,
		   0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

      glGenFramebuffers(1, &fbo);
      glBindFramebuffer(GL_FRAMEBUFFER, fbo);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			     GL_TEXTURE_2D, fbo_tex, 0);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glBindTexture(GL_TEXTURE_2D, 0);

      fbo_created = 1;
    }
  } else {
    if(fbo_created) {
      glDeleteTextures(1, &fbo_tex);
      glDeleteFramebuffers(1, &fbo);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glBindTexture(GL_TEXTURE_2D, 0);

      fbo_created = 0;
    }
  }
}

void
Java_org_nocrew_tomas_cyclone2000_C2Renderer_nativeInit(JNIEnv *env,
							jobject tclass,
							jobject tobject,
							jstring dir,
							jint disp_mode,
							jint gfx,
							jint ctrl_type,
							jint ctrl_af,
							jint ctrl_size,
							jint but_size,
							jint last_stage,
							jint start_stage,
							jint last_life,
							jint last_score,
							jint num_pups,
							jint pups_disabled,
							jintArray pup_items,
							jint width,
							jint height,
							jfloat density,
							jintArray tex_arr,
							jint reset_hs,
							jint reset_pups,
							jint is_tv)
{
  static int initiated = 0;
  jint *carr;
  const char *str;
  int i;

  DEBUG(DEBUG_NATIVE, "initiated=%d, running=%d, view=%d,"
	" screen_width=%d, screen_height=%d, density=%f,"
	" last_stage=%d, last_life=%d, last_score=%d, num_pups=%d,"
	" pups_disabled=%d",
	initiated, running, view,
	width, height, density,
	last_stage, last_life, last_score, num_pups, pups_disabled);

  running = 1;

  carr = (*env)->GetIntArrayElements(env, tex_arr, NULL);
  for(i = 0 ; i < TEX_NUM ; i++) {
    tex_ids[i] = carr[i];
  }
  (*env)->ReleaseIntArrayElements(env, tex_arr, carr, 0);

  carr = (*env)->GetIntArrayElements(env, pup_items, NULL);
  for(i = 0 ; i < PUPS_ITEM_NUM ; i++) {
    engine_pup_set(i, carr[i], 0);
  }
  (*env)->ReleaseIntArrayElements(env, pup_items, carr, 0);

  str = (*env)->GetStringUTFChars(env, dir, 0);
  files_dir = strdup(str);
  (*env)->ReleaseStringUTFChars(env, dir, str);

  jniobject = (*env)->NewGlobalRef(env, tobject);
  jniclass = (*env)->NewGlobalRef(env, tclass);

  matrix_identity(headview_mtx);

  display_mode = disp_mode;
  gfx_details = gfx;
  gfx_tv = is_tv;

  shaders_init();

  if(initiated) {
    menu_reinit(1);
    engine_reinit(1);
    fbo_created = 0;
    native_create_fbo(width, height);
    engine_control_type_set(ctrl_type, ctrl_af, ctrl_size, but_size);
    if(view == VIEW_GAME) {
      paused = engine_should_pause();
      if(!paused)
	native_callback(CALLBACK_GAME_INFO, 0, engine_state_stage_get(), 0);
    } else {
      engine_state_set(last_stage, start_stage,
		       last_life, last_score, num_pups, pups_disabled);
    }
    return;
  }

  native_create_fbo(width, height);

  screen_width = width;
  screen_height = height;
  screen_density = density;
  screen_margin_topbottom = gfx_tv ? (0.075f * (GLfloat)screen_height) : 0.0f;
  screen_margin_leftright = gfx_tv ? (0.075f * (GLfloat)screen_width) : 0.0f;

  srand((unsigned int)native_gettime());

  if(reset_hs) {
    highscore_reset();
  } else {
    highscore_file_read();
  }
  engine_init(ctrl_type, ctrl_af, ctrl_size, but_size);
  engine_state_set(last_stage, start_stage, last_life, last_score,
		   num_pups, pups_disabled);
  if(reset_pups) {
    engine_pup_reset();
  }
  menu_init(last_stage);

  initiated = 1;
}

static void native_main_glsetup(int width, int height)
{
  const GLubyte *vendor = glGetString(GL_VENDOR);
  const GLubyte *renderer = glGetString(GL_RENDERER);
  int value;

  if(!strcmp((const char *)vendor, GPU_QUALCOMM_ADRENO_320_VENDOR) &&
     !strcmp((const char *)renderer, GPU_QUALCOMM_ADRENO_320_RENDERER))
    gpu_model = GPU_QUALCOMM_ADRENO_320;
  else if(!strcmp((const char *)vendor, GPU_QUALCOMM_ADRENO_420_VENDOR) &&
	  !strcmp((const char *)renderer, GPU_QUALCOMM_ADRENO_420_RENDERER))
    gpu_model = GPU_QUALCOMM_ADRENO_420;
  else if(!strcmp((const char *)vendor, GPU_QUALCOMM_ADRENO_430_VENDOR) &&
	  !strcmp((const char *)renderer, GPU_QUALCOMM_ADRENO_430_RENDERER))
    gpu_model = GPU_QUALCOMM_ADRENO_430;

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glFrontFace(GL_CW);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glLineWidth(screen_density + 1.5f);
  glDisable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glActiveTexture(GL_TEXTURE0);

  matrix_identity(global_mtx_identity);

  /* Must check for actual number of varying that we use. */
  glGetIntegerv(GL_MAX_VARYING_VECTORS, &value);
  use_multi_light = (value >= 13) ? 1 : 0;

  /* Adreno 420 (e.g. Nexus 6) and Adreno 430 (e.g. Xperia Z5 Premium)
   * doesn't seem to handle the multi light shader very well for some
   * reason. For now, just don't use it.
   */
  if(gpu_model == GPU_QUALCOMM_ADRENO_420 ||
     gpu_model == GPU_QUALCOMM_ADRENO_430)
    use_multi_light = 0;
}

static void draw_fbo(int shader) {
  GLfloat w = screen_width / 2.0f;
  GLfloat h = screen_height;
  GLfloat vertices[] = { 0,     0,     0.0f,
			 0 + w, 0,     0.0f,
			 0,     0 + h, 0.0f,
			 0 + w, 0 + h, 0.0f };
#if 0
  GLfloat tex_coords[] = { 0.0f,        0.0f,
			   0.0f + 1.0f, 0.0f,
			   0.0f,        0.0f + 1.0f,
			   0.0f + 1.0f, 0.0f + 1.0f };
#else
  GLfloat tex_coords[] = { 0.0f,        0.0f + 1.0f,
			   0.0f + 1.0f, 0.0f + 1.0f,
			   0.0f,        0.0f,
			   0.0f + 1.0f, 0.0f };
#endif

  glEnable(GL_BLEND);
  glBindTexture(GL_TEXTURE_2D, fbo_tex);
  engine_gldirect_draw_texbox(shader, 1.0f,
			      4, vertices, tex_coords,
			      global_mtx_identity, global_mtx_identity,
			      global_mtx_ortho);
  glDisable(GL_BLEND);
}

static void draw_something(draw_f drawfunc) {
  if(display_mode == DISPLAY_MODE_FULL) {
    glViewport(0, 0, screen_width, screen_height);
    matrix_set_perspective(global_mtx_proj,
			   45, (float)screen_width / screen_height,
			   0.5f, 150.0f);
    matrix_set_ortho(global_mtx_ortho, 0, screen_width, screen_height,
		     0, -1, 1);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    drawfunc(VIEWMODE_FULL);
  } else if(display_mode == DISPLAY_MODE_VR ||
	    display_mode == DISPLAY_MODE_VRTV) {
    int shader = (display_mode == DISPLAY_MODE_VR ?
		  SHADERS_TEXTURE_DIST : SHADERS_TEXTURE);
    float half_width = (float)screen_width / 2.0f;

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, half_width, screen_height);
    matrix_set_perspective(global_mtx_proj,
			   45, (float)half_width / screen_height,
			   0.5f, 150.0f);
    matrix_set_ortho(global_mtx_ortho, 0, half_width, screen_height,
		     0, -1, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    drawfunc(VIEWMODE_LEFT);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, half_width, screen_height);
    matrix_set_perspective(global_mtx_proj,
			   45, (float)half_width / screen_height,
			   0.5f, 150.0f);
    matrix_set_ortho(global_mtx_ortho, 0, half_width, screen_height,
		     0, -1, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    draw_fbo(shader);



    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, half_width, screen_height);
    matrix_set_perspective(global_mtx_proj,
			   45, (float)half_width / screen_height,
			   0.5f, 150.0f);
    matrix_set_ortho(global_mtx_ortho, 0, half_width, screen_height,
		     0, -1, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    drawfunc(VIEWMODE_RIGHT);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(half_width, 0, half_width, screen_height);
    matrix_set_perspective(global_mtx_proj,
			   45, (float)half_width / screen_height,
			   0.5f, 150.0f);
    matrix_set_ortho(global_mtx_ortho, 0, half_width, screen_height,
		     0, -1, 1);
    draw_fbo(shader);
  }
}

void
Java_org_nocrew_tomas_cyclone2000_C2Renderer_nativeResize(JNIEnv *env,
							  jobject this,
							  jint width,
							  jint height)
{
  DEBUG(DEBUG_NATIVE, "width=%d, height=%d", width, height);

  screen_width = width;
  screen_height = height;

  native_main_glsetup(width, height);
}

void
Java_org_nocrew_tomas_cyclone2000_C2Renderer_nativeRender(JNIEnv *env,
							  jobject this,
							  jfloatArray hv)
{
  static uint64_t time1, time2;
  static double fps[50];
  static int fpsp = 0;
  jfloat *hvarr;
  int i;

  //  DEBUG(DEBUG_NATIVE, "running=%d paused=%d view=%d", running, paused, view);

  if(!running)
    return;

  time2 = time1;
  time1 = native_gettime();

  if(display_mode == DISPLAY_MODE_VR) {
    hvarr = (*env)->GetFloatArrayElements(env, hv, NULL);
    for(i = 0 ; i < 16 ; i++) {
      headview_mtx[i] = hvarr[i];
    }
    (*env)->ReleaseFloatArrayElements(env, hv, hvarr, 0);
  }

  if(view == VIEW_MENU || view == VIEW_PUPS || view == VIEW_HS) {

    if(!menu_update(time1)) {
      if(view == VIEW_MENU) {
	view = VIEW_GAME;
	native_main_glsetup(screen_width, screen_height);
	paused = engine_start();
	if(!paused)
	  native_callback(CALLBACK_GAME_INFO, 0, engine_state_stage_get(), 0);
      } else {
	view = VIEW_MENU;
      }
      return;
    }
    draw_something(menu_draw);

  } else if(view == VIEW_HSENTER) {

    if(!highscore_update(time1)) {
      native_callback(CALLBACK_GO_VIEW, VIEW_MENU, 0, 0);
      return;
    }
    draw_something(highscore_draw);

  } else if(view == VIEW_GAMEOVER) {

    gameover_update(time1);
    draw_something(gameover_draw);

  } else if(view == VIEW_GAME) {

    if(paused) {
      engine_update_paused(time1);
    } else {
      if(!engine_update(time1))
	return;
    }
    draw_something(engine_draw);

  }

  if(FLAG_TST(debug, DEBUG_FPS)) {
    uint64_t msec = time1 - time2;
    if(msec == 0)
      msec = 1;

    fps[fpsp++] = 1000.0 / (double)msec;
    if(fpsp == 50) {
      fpsp = 0;
      double avg = 0.0;
      int i;
      for(i = 0 ; i < 50 ; i++) {
	avg += fps[i];
      }
      avg /= 50;

      DEBUG(DEBUG_FPS, "fps=%f", avg);
    }
  }
}

void
Java_org_nocrew_tomas_cyclone2000_C2Renderer_nativePlayerSetControl(JNIEnv *env,
								    jobject this,
								    jfloat speed,
								    jint step,
								    jint shoot,
								    jint jump,
								    jint zap)
{
  DEBUG(DEBUG_NATIVE, "speed=%f, step=%d, shoot=%d, jump=%d, zap=%d",
	speed, step, shoot, jump, zap);

  if(paused)
    return;

  engine_player_set_control(speed, step, shoot, jump, zap);
}

void
Java_org_nocrew_tomas_cyclone2000_C2Renderer_nativeEvent(JNIEnv *env,
							 jobject this,
							 jint event,
							 jint data1,
							 jint data2,
							 jint data3,
							 jint data4)
{
  DEBUG(DEBUG_NATIVE, "event=%d", event);

  if(view == VIEW_GAME)
    paused = 1;
  else
    paused = 0;

  switch(event) {
  case EVENT_PAUSE:
    running = 0;
    engine_pause();
    break;
  case EVENT_RESUME:
    running = 1;
    paused = engine_resume(paused);
    if(!paused && view == VIEW_GAME)
      native_callback(CALLBACK_GAME_INFO, 0, engine_state_stage_get(), 0);
    break;
  case EVENT_GAME_PAUSE:
    paused = 1;
    running = 1;
    engine_pause();
    break;
  case EVENT_GAME_RESUME:
    paused = 0;
    running = 1;
    paused = engine_resume(paused);
    if(!paused)
      native_callback(CALLBACK_GAME_INFO, 0, engine_state_stage_get(), 0);
    break;
  case EVENT_CONTROL_TYPE:
    engine_control_type_set(data1, data2, data3, data4);
    break;
  case EVENT_MENU_CHOICE:
    if(view == VIEW_MENU || view == VIEW_PUPS || view == VIEW_HS)
      menu_choice(data1, data2, data3);
    else if(view == VIEW_HSENTER)
      highscore_control(data1, data2, data3);
    break;
  case EVENT_GO_VIEW:
    if(data1 == VIEW_MENU) {
      engine_pause();
      menu_init(engine_state_stage_get());
      view = data1;
      running = 1;
      paused = 1;
    } else if(data1 == VIEW_HSENTER) {
      highscore_init(data2);
      view = data1;
      running = 1;
    }
    break;
  case EVENT_PUPS_PREFS:
    if(data2)
      engine_pup_reset();
    engine_pup_disable_set(data1);
    break;
  case EVENT_PUPS_ADD:
    if(data1)
      engine_numpups_increase(data1);
    else
      engine_numpups_set(data2);
    break;
  case EVENT_HS_RESET:
    highscore_reset();
    break;
  case EVENT_DISPLAY_MODE:
    display_mode = data1;
    if(gfx_details != data2) {
      gfx_details = data2;
      menu_reinit(0);
      engine_reinit(0);
    }
    native_create_fbo(screen_width, screen_height);
    paused = 1;
    running = 1;
    engine_pause();
    break;
  }
}


void native_callback5(int type, int data1, int data2,
		      int data3, int data4, int data5)
{
  jint jtype = type;
  jint jdata1 = data1;
  jint jdata2 = data2;
  jint jdata3 = data3;
  jint jdata4 = data4;
  jint jdata5 = data5;
  static jmethodID jnicallback;
  JNIEnv *jnienv;

  if((*jvm)->GetEnv(jvm, (void**)&jnienv, JNI_VERSION_1_6) != JNI_OK)
    return;

  DEBUG(DEBUG_NATIVE,
	"callback type=%d, data1=%d, data2=%d, data3=%d, data4=%d, data5=%d",
	type, data1, data2, data3, data4, data5);

  if(jnicallback == NULL) {
    jnicallback = (*jnienv)->GetMethodID(jnienv, jniclass,
					 "nativeCallback",
					 "(IIIIII)V");
  }

  (*jnienv)->CallVoidMethod(jnienv, jniobject, jnicallback,
			    jtype, jdata1, jdata2, jdata3, jdata4, jdata5);

  if(type == CALLBACK_GO_VIEW) {
    if(view != VIEW_MENU && data1 == VIEW_MENU) {
      view = VIEW_MENU;
      menu_init(engine_state_stage_get());
      running = 1;
    } else if(view != VIEW_GAMEOVER && data1 == VIEW_GAMEOVER) {
      view = VIEW_GAMEOVER;
      gameover_init(data2);
      running = 1;
    }
  }
}


int native_tex_id(int id)
{
  return tex_ids[id];
}

int native_is_paused(void)
{
  return paused;
}

int native_is_vr(void)
{
  return (display_mode == DISPLAY_MODE_VR ||
	  display_mode == DISPLAY_MODE_VRTV);
}

int native_is_vrtv(void)
{
  return (display_mode == DISPLAY_MODE_VRTV);
}

uint64_t native_gettime(void)
{
  struct timespec tp;

  clock_gettime(CLOCK_MONOTONIC, &tp);

  return (((uint64_t)tp.tv_sec) * 1000 + ((uint64_t)tp.tv_nsec) / 1000000);
}

int native_get_gpu(void)
{
  return gpu_model;
}
