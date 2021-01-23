#ifndef _NATIVE_MAIN_H_
#define _NATIVE_MAIN_H_

extern int screen_width;
extern int screen_height;
extern double screen_density;
extern GLfloat screen_margin_topbottom;
extern GLfloat screen_margin_leftright;
extern int gfx_details;
extern int gfx_tv;
extern char *files_dir;
extern float global_mtx_proj[16], global_mtx_ortho[16];
extern float global_mtx_identity[16];
extern GLfloat headview_mtx[16];
extern int use_multi_light;

#define EVENT_PAUSE                    1
#define EVENT_RESUME                   2
#define EVENT_GAME_PAUSE               3
#define EVENT_GAME_RESUME              4
#define EVENT_CONTROL_TYPE             5
#define EVENT_MENU_CHOICE              6
#define EVENT_SOUND_SFX                7
#define EVENT_GO_VIEW                  8
#define EVENT_MENU_HOVER               9
#define EVENT_PUPS_PREFS               10
#define EVENT_HS_RESET                 11
#define EVENT_PUPS_ADD                 12
#define EVENT_DISPLAY_MODE             13

#define CONTROL_TYPE_SINGLE_ARROWS     1
#define CONTROL_TYPE_MULTI_ARROWS      2
#define CONTROL_TYPE_MULTI_CIRCULAR    3
#define CONTROL_TYPE_MULTI_SWIPE       4
#define CONTROL_TYPE_DUAL_SWIPE        5
#define CONTROL_TYPE_HIDDEN            6

#define MENU_CHOICE_NEWGAME            1
#define MENU_CHOICE_CONTINUE           2
#define MENU_CHOICE_STAGE_CHANGE       3
#define MENU_CHOICE_HOVERSTEP          4
#define MENU_CHOICE_PUPS               5
#define MENU_CHOICE_HOVERSELECT        6
#define MENU_CHOICE_HIGHSCORE          7
#define MENU_GESTURE_CLOSE             8
#define MENU_GESTURE_SCROLL            9
#define MENU_GESTURE_FLING             10
#define MENU_GESTURE_DOWN              11
#define MENU_GESTURE_TAP               12

#define MENU_HOVER_CONTINUE            0
#define MENU_HOVER_NEWGAME             1
#define MENU_HOVER_LEADERBOARD         2
#define MENU_HOVER_POWERUPS            3
#define MENU_HOVER_SETTINGS            4

#define PUPS_ITEM_FIRST                0
#define PUPS_ITEM_LASER                0
#define PUPS_ITEM_JUMP                 1
#define PUPS_ITEM_DROID                2
#define PUPS_ITEM_LIFE                 3
#define PUPS_ITEM_NUM                  4

#define VIEW_NONE                      0
#define VIEW_MENU                      1
#define VIEW_GAME                      2
#define VIEW_GAMEOVER                  3
#define VIEW_PUPS                      4
#define VIEW_HS                        5
#define VIEW_HSENTER                   6

#define TEX_CONTROL_FULLARROWS         0
#define TEX_CONTROL_ARROWSJUMP         1
#define TEX_CONTROL_ARROWSZAP          2
#define TEX_CONTROL_CROSS              3
#define TEX_CONTROL_SWIPE              4
#define TEX_CONTROL_SWIPEJUMP          5
#define TEX_CONTROL_SHOOT              6
#define TEX_CONTROL_JUMP               7
#define TEX_CONTROL_ZAP                8
#define TEX_CONTROL_PAUSE              9
#define TEX_CONTROL_EXIT               10
#define TEX_POINT_2000                 11
#define TEX_PRICE_LIFE                 12
#define TEX_1UP_UP                     13
#define TEX_1UP_1_GLOW                 14
#define TEX_FUSEBALL_SCORE             15
#define TEX_CHARS                      16
#define TEX_CHARS_LIGHT                17
#define TEX_MENU_ICONS                 18
#define TEX_SCORE_NUMBERS_BASE         19
#define TEX_NUM                        TEX_SCORE_NUMBERS_BASE + 10

int native_tex_id(int id);


/* Callbacks from native to Java. */

#define CALLBACK_STATE_SAVE            1
#define CALLBACK_GO_GAMEOVER           2
#define CALLBACK_GO_ENDTEXT            3
#define CALLBACK_GO_VIEW               4
#define CALLBACK_PLAY_SOUND            5
#define CALLBACK_GAME_INFO             6
#define CALLBACK_MENU_HOVER            7
#define CALLBACK_PUPS_SAVE             8
#define CALLBACK_PUPS_ADD              9

/* Display modes */
#define DISPLAY_MODE_FULL              0
#define DISPLAY_MODE_VR                1
#define DISPLAY_MODE_VRTV              2

/* Gfx details */
#define GFX_DETAILS_LOW                0
#define GFX_DETAILS_HIGH               1

/* For draw callbacks */
#define VIEWMODE_FULL                  0
#define VIEWMODE_LEFT                  1
#define VIEWMODE_RIGHT                 2

#define VR_EYE_OFFSET                  0.05f
#define VR_Z_OFFSET                    1.9f

void native_callback5(int type, int data1, int data2, int data3,
		      int data4, int data5);
#define native_callback(t,d1,d2,d3) native_callback5((t),(d1),(d2),(d3),0,0)
#define native_callback0(t) native_callback5((t),0,0,0,0,0)
#define native_callback2(t,d1,d2) native_callback5((t),(d1),(d2),0,0,0)
#define native_callback4(t,d1,d2,d3,d4) native_callback5((t),(d1),(d2),(d3),(d4),0)


/* Sounds */
#define SOUND_LASER                    1
#define SOUND_EXPLOSION                2
#define SOUND_POWERUP                  3
#define SOUND_PLAYER_EXPLOSION         4
#define SOUND_ENDSTAGE                 5
#define SOUND_SHOT                     6
#define SOUND_CLING                    7
#define SOUND_LIFE                     8
#define SOUND_WHOOO                    9
#define SOUND_SUPERZAPPER              10
#define SOUND_YESYESYES                11


int native_is_paused(void);
int native_removeads(void);
int native_is_vr(void);
int native_is_vrtv(void);

uint64_t native_gettime(void);


/* Special GPUs Vendor + Renderer */
#define GPU_QUALCOMM_ADRENO_320           1
#define GPU_QUALCOMM_ADRENO_320_VENDOR    "Qualcomm"
#define GPU_QUALCOMM_ADRENO_320_RENDERER  "Adreno (TM) 320"
#define GPU_QUALCOMM_ADRENO_420           2
#define GPU_QUALCOMM_ADRENO_420_VENDOR    "Qualcomm"
#define GPU_QUALCOMM_ADRENO_420_RENDERER  "Adreno (TM) 420"
#define GPU_QUALCOMM_ADRENO_430           2
#define GPU_QUALCOMM_ADRENO_430_VENDOR    "Qualcomm"
#define GPU_QUALCOMM_ADRENO_430_RENDERER  "Adreno (TM) 430"

int native_get_gpu(void);

#endif /* _NATIVE_MAIN_H_ */
