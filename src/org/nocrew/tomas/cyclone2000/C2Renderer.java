package org.nocrew.tomas.cyclone2000;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.os.Handler;
import android.content.Context;
import android.graphics.Bitmap;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.opengl.GLES20;
import android.view.Display;
import android.view.MotionEvent;
import android.view.KeyEvent;
import android.view.Window;
import android.view.WindowManager;
import android.util.DisplayMetrics;
import android.util.Log;

import com.google.vrtoolkit.cardboard.sensors.HeadTracker;

public class C2Renderer implements GLSurfaceView.Renderer {

    static {
        System.loadLibrary("cyclone2000");
    }

    private static native void nativeInit(Object tobject,
					  String dir,
					  int disp_mode, int gfx,
					  int ctrl_type, int ctrl_af,
					  int ctrl_size, int but_size,
					  int last_stage, int last_start_stage,
					  int last_life, int last_score,
					  int num_pups, int pups_disabled,
					  int[] pup_items,
					  int w, int h, float density,
					  int[] tex_arr,
					  int reset_hs, int reset_pups,
					  int is_tv);
    private static native void nativeResize(int w, int h);
    private static native void nativeRender(float[] headview);
    private static native void nativePlayerSetControl(float speed, int step,
						      int shoot, int jump,
						      int zap);
    private static native void nativeEvent(int event,
					   int data1, int data2,
					   int data3, int data4);

    // These constants are also in jni/native_main.h

    public static final int EVENT_PAUSE                    = 1;
    public static final int EVENT_RESUME                   = 2;
    public static final int EVENT_GAME_PAUSE               = 3;
    public static final int EVENT_GAME_RESUME              = 4;
    public static final int EVENT_CONTROL_TYPE             = 5;
    public static final int EVENT_MENU_CHOICE              = 6;
    public static final int EVENT_SOUND_SFX                = 7;
    public static final int EVENT_GO_VIEW                  = 8;
    public static final int EVENT_MENU_HOVER               = 9;
    public static final int EVENT_PUPS_PREFS               = 10;
    public static final int EVENT_HS_RESET                 = 11;
    public static final int EVENT_PUPS_ADD                 = 12;
    public static final int EVENT_DISPLAY_MODE             = 13;
    // Local between C2GLSurfaceView and C2Renderer.
    public static final int EVENT_LOAD_TEX                 = 1001;

    public static final int CONTROL_TYPE_AUTO_DETECT       = 0;
    public static final int CONTROL_TYPE_SINGLE_ARROWS     = 1;
    public static final int CONTROL_TYPE_MULTI_ARROWS      = 2;
    public static final int CONTROL_TYPE_MULTI_CIRCULAR    = 3;
    public static final int CONTROL_TYPE_MULTI_SWIPE       = 4;
    public static final int CONTROL_TYPE_DUAL_SWIPE        = 5;
    public static final int CONTROL_TYPE_HIDDEN            = 6;

    public static final int MENU_CHOICE_NEWGAME            = 1;
    public static final int MENU_CHOICE_CONTINUE           = 2;
    public static final int MENU_CHOICE_STAGE_CHANGE       = 3;
    public static final int MENU_CHOICE_HOVERSTEP          = 4;
    public static final int MENU_CHOICE_PUPS               = 5;
    public static final int MENU_CHOICE_HOVERSELECT        = 6;
    public static final int MENU_CHOICE_HIGHSCORE          = 7;
    public static final int MENU_GESTURE_CLOSE             = 8;
    public static final int MENU_GESTURE_SCROLL            = 9;
    public static final int MENU_GESTURE_FLING             = 10;
    public static final int MENU_GESTURE_DOWN              = 11;
    public static final int MENU_GESTURE_TAP               = 12;

    public static final int MENU_HOVER_CONTINUE            = 0;
    public static final int MENU_HOVER_NEWGAME             = 1;
    public static final int MENU_HOVER_LEADERBOARD         = 2;
    public static final int MENU_HOVER_POWERUPS            = 3;
    public static final int MENU_HOVER_SETTINGS            = 4;

    // These must not be changed, they are stored.
    public static final int PUPS_ITEM_LASER                = 0;
    public static final int PUPS_ITEM_JUMP                 = 1;
    public static final int PUPS_ITEM_DROID                = 2;
    public static final int PUPS_ITEM_LIFE                 = 3;
    public static final int PUPS_ITEM_NUM                  = 4;

    public static final int VIEW_NONE                      = 0;
    public static final int VIEW_MENU                      = 1;
    public static final int VIEW_GAME                      = 2;
    public static final int VIEW_GAMEOVER                  = 3;
    public static final int VIEW_PUPS                      = 4;
    public static final int VIEW_HS                        = 5;
    public static final int VIEW_HSENTER                   = 6;

    public static final int TEX_CONTROL_FULLARROWS         = 0;
    public static final int TEX_CONTROL_ARROWSJUMP         = 1;
    public static final int TEX_CONTROL_ARROWSZAP          = 2;
    public static final int TEX_CONTROL_CROSS              = 3;
    public static final int TEX_CONTROL_SWIPE              = 4;
    public static final int TEX_CONTROL_SWIPEJUMP          = 5;
    public static final int TEX_CONTROL_SHOOT              = 6;
    public static final int TEX_CONTROL_JUMP               = 7;
    public static final int TEX_CONTROL_ZAP                = 8;
    public static final int TEX_CONTROL_PAUSE              = 9;
    public static final int TEX_CONTROL_EXIT               = 10;
    public static final int TEX_POINT_2000                 = 11;
    public static final int TEX_PRICE_LIFE                 = 12;
    public static final int TEX_1UP_UP                     = 13;
    public static final int TEX_1UP_1_GLOW                 = 14;
    public static final int TEX_FUSEBALL_SCORE             = 15;
    public static final int TEX_CHARS                      = 16;
    public static final int TEX_CHARS_LIGHT                = 17;
    public static final int TEX_MENU_ICONS                 = 18;
    public static final int TEX_SCORE_NUMBERS_BASE         = 19;
    public static final int TEX_NUM = TEX_SCORE_NUMBERS_BASE + 10;

    public static final int CALLBACK_STATE_SAVE            = 1;
    public static final int CALLBACK_GO_GAMEOVER           = 2;
    public static final int CALLBACK_GO_ENDTEXT            = 3;
    public static final int CALLBACK_GO_VIEW               = 4;
    public static final int CALLBACK_PLAY_SOUND            = 5;
    public static final int CALLBACK_GAME_INFO             = 6;
    public static final int CALLBACK_MENU_HOVER            = 7;
    public static final int CALLBACK_PUPS_SAVE             = 8;
    public static final int CALLBACK_PUPS_ADD              = 9;
    // Local between C2GLSurfaceView and C2Renderer.
    public static final int CALLBACK_LOAD_TEX              = 1001;

    public static final int DISPLAY_MODE_FULL              = 0;
    public static final int DISPLAY_MODE_VR                = 1;
    public static final int DISPLAY_MODE_VRTV              = 2;

    public static final int GFX_DETAILS_LOW                = 0;
    public static final int GFX_DETAILS_HIGH               = 1;


    public Bitmap bitmap;
    private int[] textureIds;

    private boolean initiated = false;

    private C2GLSurfaceView uiView;
    private Handler uiHandler;
    private Object uiSyncObj;
    public HeadTracker headTracker;
    private float[] lastHeadView;

    private int prefDisplayMode;
    private int prefGfxDetails;
    private int prefControlType;
    private boolean prefControlAutofire;
    private int prefControlSize;
    private int prefControlButtonSize;
    private int prefSoundSfx;
    private int prefGameStage;
    private int prefGameStartStage;
    private int prefGameLife;
    private int prefGameScore;
    private int prefTotalPowerups;
    private boolean prefPupsDisabled;
    private int[] prefPupsItems;
    private boolean resetHs;
    private boolean resetPups;

    public boolean isTV;
    public Context viewcontext;
    public int screenWidth;
    public int screenHeight;
    public float screenDensity;
    public String filesDir;

    public C2Renderer(boolean tv, C2GLSurfaceView view,
		      Context context, Handler handler,
		      Object syncObj,
		      String dir,
		      int displayMode, int gfxDetails,
		      int controlType, boolean controlAutofire,
		      int controlSize, int controlButtonSize,
		      int lastStage, int lastStartStage,
		      int lastLife, int lastScore,
		      int totalPowerups, boolean pupsDisabled,
		      int[] pupsItems,
		      boolean resetHs, boolean resetPups) {
        super();

	//	Log.i("Cyclone2000", "C2Renderer");

	uiView = view;
	viewcontext = context;
	uiHandler = handler;
	uiSyncObj = syncObj;
	isTV = tv;
	filesDir = dir;
	prefDisplayMode = displayMode;
	prefGfxDetails = gfxDetails;
	prefControlType = controlType;
	prefControlAutofire = controlAutofire;
	prefControlSize = controlSize;
	prefControlButtonSize = controlButtonSize;
	prefGameStage = lastStage;
	prefGameStartStage = lastStartStage;
	prefGameLife = lastLife;
	prefGameScore = lastScore;
	prefTotalPowerups = totalPowerups;
	prefPupsDisabled = pupsDisabled;
	textureIds = new int[TEX_NUM];
	prefPupsItems = (int[])pupsItems.clone();
	this.resetHs = resetHs;
	this.resetPups = resetPups;
	headTracker = HeadTracker.createFromContext(context);
	lastHeadView = new float[16];
    }

    public void onSurfaceCreated(GL10 unused, EGLConfig config) {
	int i;

	//	Log.i("Cyclone2000", "onSurfaceCreated");

    	Display d =
	    ((WindowManager)viewcontext.
	     getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
	DisplayMetrics dm = new DisplayMetrics();

    	screenWidth = d.getWidth();
    	screenHeight = d.getHeight();
	d.getMetrics(dm);
	screenDensity = dm.density;

	nativeCallback(C2Renderer.CALLBACK_LOAD_TEX, 0, 0, 0, 0, 0);
    }

    private void loadTexture(int i, int texid, boolean last) {
	if(bitmap != null) {
	    textureIds[texid] = loadGLTexture(bitmap);
	    bitmap.recycle();
	    bitmap = null;
	}

	if(last) {
	    //	Log.i("Cyclone2000", "loaded all textures");
	    nativeInit(this, filesDir, prefDisplayMode, prefGfxDetails,
		       prefControlType, prefControlAutofire ? 1 : 0,
		       prefControlSize, prefControlButtonSize,
		       prefGameStage, prefGameStartStage,
		       prefGameLife, prefGameScore, prefTotalPowerups,
		       prefPupsDisabled ? 1 : 0, prefPupsItems,
		       screenWidth, screenHeight, screenDensity,
		       textureIds,
		       resetHs ? 1 : 0, resetPups ? 1 : 0,
		       isTV ? 1 : 0);
	    initiated = true;
	}

	nativeCallback(C2Renderer.CALLBACK_LOAD_TEX, i + 1, 0, 0, 0, 0);
    }

    public void onSurfaceChanged(GL10 unused, int w, int h) {
	//	Log.i("Cyclone2000", "onSurfaceChanged");
	screenWidth = w;
	screenHeight = h;
    	nativeResize(w, h);
    }

    public void onDrawFrame(GL10 unused) {
	//	Log.i("Cyclone2000", "onDrawFrame initiated=" + initiated);

	if(!initiated)
	    return;

	headTracker.getLastHeadView(lastHeadView, 0);

        nativeRender(lastHeadView);

	synchronized(uiSyncObj) {
	    uiSyncObj.notify();
	}

    }

    public void playerSetControl(float speed, int step,
				 int shoot, int jump, int zap) {
	nativePlayerSetControl(speed, step, shoot, jump, zap);
    }

    public void createEvent(int event, int data1, int data2,
			    int data3, int data4) {
	// Local event, during texture loading.
	if(event == C2Renderer.EVENT_LOAD_TEX) {
	    loadTexture(data1, data2, data3 != 0);
	    return;
	}

	if(event == C2Renderer.EVENT_CONTROL_TYPE) {
	    prefControlType = data1;
	    prefControlAutofire = (data2 == 0 ? false : true);
	    prefControlSize = data3;
	    prefControlButtonSize = data4;
	} else if(event == C2Renderer.EVENT_SOUND_SFX) {
	    prefSoundSfx = data1;
	} else if(event == C2Renderer.EVENT_PUPS_PREFS) {
	    prefPupsDisabled = (data1 == 0 ? false : true);
	    if(data2 != 0) {
		prefTotalPowerups = 0;
		for(int i = 0 ; i < C2Renderer.PUPS_ITEM_NUM ; i++)
		    prefPupsItems[i] = 0;
	    }
	} else if(event == C2Renderer.EVENT_PUPS_ADD) {
	    if(data1 != 0)
		prefTotalPowerups += data1;
	    else
		prefTotalPowerups = data2;
	} else if(event == C2Renderer.EVENT_DISPLAY_MODE) {
	    prefDisplayMode = data1;
	    prefGfxDetails = data2;
	} else if(event == C2Renderer.EVENT_PAUSE) {
	    headTracker.stopTracking();
	} else if(event == C2Renderer.EVENT_RESUME) {
	    headTracker.startTracking();
	}

	if(!initiated)
	    return;

	nativeEvent(event, data1, data2, data3, data4);
    }

    public void nativeCallback(int type, int data1, int data2,
			       int data3, int data4, int data5) {
	final int itype = type;
	final int idata1 = data1;
	final int idata2 = data2;
	final int idata3 = data3;
	final int idata4 = data4;
	final int idata5 = data5;

	// If playing sound, and the volume is 0, don't play at all.
	// This improves performance a lot.
	if(type == C2Renderer.CALLBACK_PLAY_SOUND && prefSoundSfx == 0)
	    return;

	// Snoop and update internal game state.
	if(type == C2Renderer.CALLBACK_STATE_SAVE) {
	    prefGameStage = data1;
	    prefGameLife = data2;
	    prefGameScore = data3;
	    prefGameStartStage = data4;
	    prefTotalPowerups = data5;
	}

	// Snoop and update pups state.
	if(type == C2Renderer.CALLBACK_PUPS_SAVE) {
	    prefPupsItems[data1] = data2;
	}

	uiHandler.post(new Runnable(){
		public void run() {
		    uiView.callback(itype, idata1, idata2, idata3,
				    idata4, idata5);
		}});
    }

    private int loadGLTexture(Bitmap bitmap) {
    	int[] textures = new int[1];
	GLES20.glGenTextures(1, textures, 0);

	GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textures[0]);

	GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
			       GLES20.GL_TEXTURE_MIN_FILTER,
			       GLES20.GL_NEAREST);
	GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
			       GLES20.GL_TEXTURE_MAG_FILTER,
			       GLES20.GL_NEAREST);

	GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
			       GLES20.GL_TEXTURE_WRAP_S,
			       GLES20.GL_CLAMP_TO_EDGE);
	GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
			       GLES20.GL_TEXTURE_WRAP_T,
			       GLES20.GL_CLAMP_TO_EDGE);

	GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0);

    	return textures[0];
    }
}
