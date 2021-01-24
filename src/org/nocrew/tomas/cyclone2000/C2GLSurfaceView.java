package org.nocrew.tomas.cyclone2000;

import java.io.IOException;
import java.io.InputStream;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.Manifest;
import android.os.Handler;
import android.os.Build;
import android.os.Environment;
import android.os.AsyncTask;
import android.content.Context;
import android.content.Intent;
import android.preference.PreferenceManager;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.content.DialogInterface;
import android.app.Activity;
import android.app.ProgressDialog;
import android.app.AlertDialog;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.DashPathEffect;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Paint.Style;
import android.graphics.LinearGradient;
import android.graphics.Shader;
import android.graphics.Region;
import android.opengl.GLSurfaceView;
import android.view.View;
import android.view.MotionEvent;
import android.view.KeyEvent;
import android.view.Display;
import android.view.WindowManager;
import android.view.InputDevice;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector;
import android.view.SurfaceHolder;
import android.util.Log;
import android.media.SoundPool;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.net.Uri;
import android.widget.Toast;
import android.util.Base64;
import android.util.DisplayMetrics;

import javax.crypto.*;
import javax.crypto.spec.DESKeySpec;

import com.google.vrtoolkit.cardboard.sensors.NfcSensor;
import com.google.vrtoolkit.cardboard.CardboardDeviceParams;

public class C2GLSurfaceView extends GLSurfaceView
    implements OnGestureListener, NfcSensor.OnCardboardNfcListener {

    public static final int CONTROL_EVENT_LEFT  = 0;
    public static final int CONTROL_EVENT_RIGHT = 1;
    public static final int CONTROL_EVENT_SHOOT = 2;
    public static final int CONTROL_EVENT_JUMP  = 3;
    public static final int CONTROL_EVENT_ZAP   = 4;
    public static final int CONTROL_EVENT_PAUSE = 5;
    public static final int CONTROL_EVENT_BACK  = 6;
    public static final String[] CONTROLEVENTS = {
	"Step left",
	"Step right",
	"Shoot",
	"Jump",
	"Superzap",
	"Pause",
	"Back"
    };

    private Activity mActivity;
    private Context activityContext;
    private Object preferencesClass;
    private C2Renderer mRenderer;
    private Handler handler;
    private SoundPool soundPool;
    private MediaPlayer mediaPlayer;
    private String currentMusic;
    public float screenDensity;

    private boolean pressedSignIn = false;

    // request codes used for permission requests
    private static final int RC_PERM_STORAGE = 6000;

    private boolean permStorage = false;

    private NfcSensor nfcSensor;
    private boolean inCardboard;

    private Object syncObj;

    private int[] textureList;
    private int textureListLen;

    private static int view = C2Renderer.VIEW_MENU;

    private boolean suspended;
    private static boolean game_paused;
    private static boolean runAsTV;
    private static int curr_stage;
    private int highscorePos;
    private int highscore;
    private int highstage;
    private int currentStagePups = 0;

    public static final int DEFAULT_SOUND_MUSIC = 100;
    public static final int DEFAULT_SOUND_SFX = 50;
    public static final float DEFAULT_CONTROL_SENSITIVITY = 5.0f;

    private int prefDisplayMode = C2Renderer.DISPLAY_MODE_FULL;
    private boolean useNfc = false;
    private int prefControlType = C2Renderer.CONTROL_TYPE_SINGLE_ARROWS;
    private boolean prefControlAutofire = false;
    private int prefControlSize = -1;
    private int prefControlButtonSize = -1;
    private int prefGfxDetails = C2Renderer.GFX_DETAILS_HIGH;
    private String prefControlHide = "auto";
    private String prefFullScreen = "auto";
    private boolean useControlHide = false;
    private boolean useFullScreen = false;
    private boolean haveImmersive = false;
    private float prefControlSensitivity = DEFAULT_CONTROL_SENSITIVITY;
    private int prefSoundMusic = DEFAULT_SOUND_MUSIC;
    private int prefSoundSfx = DEFAULT_SOUND_SFX;
    private String[] prefMusic;
    private int prefGameStage = -1;
    private int prefGameStartStage = -1;
    private int prefGameLife = -1;
    private int prefGameScore = -1;
    private int prefGameMaxStage = 0;
    private int prefTotalPowerups = 0;
    private boolean prefPupsDisabled = false;
    private int[] prefPupsItems;
    private int prefKeyCodes[];
    private int prefControlEvents[];
    private int prefNumKeys;
    private boolean resetHs = false;
    private boolean resetPups = false;

    private int[] controlSizes = {64, 128, 192, 256, 320};
    private static final int[] controlTexSizes = {128, 128, 256, 256, 512};
    private int[] buttonSizes = {32, 64, 96, 128, 160};
    private static final int[] buttonTexSizes = {64, 64, 128, 128, 256};

    private static final float[] arrowsSensFactor =
    {0.05f, 0.07f, 0.10f, 0.14f, 0.19f, 0.27f, 0.40f, 0.60f, 0.95f, 1.60f};
    private static final float[] arrowsSensExp =
    {3.35f, 2.65f, 2.16f, 1.85f, 1.63f, 1.44f, 1.27f, 1.13f, 1.01f, 0.90f};
    private static final float[] joySensFactor =
    {0.028f, 0.031f, 0.035f, 0.040f, 0.046f, 0.055f, 0.065f, 0.080f, 0.100f, 0.130f};
    private static final float[] joySensExp =
    {7.80f, 6.55f, 5.47f, 4.65f, 4.00f, 3.40f, 3.00f, 2.60f, 2.28f, 1.99f};

    private Handler menuHandler = new Handler();

    private long menuStageDownLastTime = -1;
    private long menuStageUpLastTime = -1;
    private long menuStageInterval = 400;
    private Runnable menuStageUpdateTask;
    private int menuHoverSelection;

    private Runnable hsListUpdateTask, hsEnterUpdateTask;
    private int hsListDir = 0;
    private int hsListStep = 1;
    private int hsEnterDir = 0;
    private int hsEnterStep = 1;
    private final static int HS_FAKE_UP = -1000;
    private final static int HS_FAKE_BACK = -2000;

    private int controlPointerID = -1;
    private int sjzPointerID = -1;

    private float controlPrevX, controlPrevY;
    private float controlCurrX, controlCurrY;
    private double controlPrevAngle, controlCurrAngle;

    private float currentSpeed;
    private int currentStep;
    private int currentShoot;
    private int currentJump;
    private int currentZap;
    private int ongoingShoot;

    private boolean haveGamePad = false;
    private long gamePadStepDownTime = -1;
    private Handler gamePadStepHandler = new Handler();
    private Runnable gamePadStepUpdateTask;
    private boolean gamePadStepOnQueue = false;
    private int gamePadStepLeft;
    private int gamePadStepRight;
    private float gamePadJoySpeed = 0.0f;
    private long gamePadJoyDownTime = -1;
    private int gamePadShoot;
    private int gamePadJump;
    private int gamePadZap;

    private GestureDetector gestureScanner;

    private int sounds[];
    private int streams[];
    private float volumes[];
    private static final int SOUND_LASER               = 1;
    private static final int SOUND_EXPLOSION           = 2;
    private static final int SOUND_POWERUP             = 3;
    private static final int SOUND_PLAYER_EXPLOSION    = 4;
    private static final int SOUND_ENDSTAGE            = 5;
    private static final int SOUND_SHOT                = 6;
    private static final int SOUND_CLING               = 7;
    private static final int SOUND_LIFE                = 8;
    private static final int SOUND_WHOOO               = 9;
    private static final int SOUND_SUPERZAPPER         = 10;
    private static final int SOUND_YESYESYES           = 11;
    private static final int SOUND_NUM                 = 12;

    private int[] drawables;
    private static final int DRAWABLE_P2K              = 0;
    private static final int DRAWABLE_UP               = 1;
    private static final int DRAWABLE_1_GLOW           = 2;
    private static final int DRAWABLE_FBSCORE          = 3;
    private static final int DRAWABLE_PRICE_LIFE       = 4;
    private static final int DRAWABLE_CHARS            = 5;
    private static final int DRAWABLE_CHARS_LIGHT      = 6;
    private static final int DRAWABLE_MENU_ICONS       = 7;

    private String musicDirectory;

    private NavHider navHider;

    public C2GLSurfaceView(Activity activity, Object prefClass, Object gpClass,
			   boolean isTV,
			   int[] resDrawables, int[] resSounds) {
        super((Context)activity);

	setFocusable(true);
	setFocusableInTouchMode(true);

	setEGLContextClientVersion(2);
	setEGLConfigChooser(8, 8, 8, 8, 16, 0);

	mActivity = activity;
	activityContext = (Context)activity;
	preferencesClass = prefClass;

	Display d =
	    ((WindowManager)activityContext.
	     getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
	DisplayMetrics dm = new DisplayMetrics();

	d.getMetrics(dm);
	screenDensity = dm.density;

	if(screenDensity > 2.0) {
	    for(int i = 0 ; i < controlSizes.length ; i++) {
		controlSizes[i] *= screenDensity * 0.5;
		buttonSizes[i] *= screenDensity * 0.5;
	    }
	}

	resetDefaultsController();

	convPrefValues();

	prefMusic = new String[8];
	prefPupsItems = new int[C2Renderer.PUPS_ITEM_NUM];

	int devids[] = InputDevice.getDeviceIds();
	for(int devid : devids) {
	    InputDevice device = InputDevice.getDevice(devid);
	    int sources = device.getSources();

	    /* Skip fingerprint reader, it registers as a joystick. */
	    if(device.getName().equals("uinput-fpc")) {
		continue;
	    }

	    if((sources & InputDevice.SOURCE_GAMEPAD) ==
	       InputDevice.SOURCE_GAMEPAD ||
	       (sources & InputDevice.SOURCE_JOYSTICK) ==
	       InputDevice.SOURCE_JOYSTICK)
		haveGamePad = true;
	    /*
	    Log.d("Cyclone2000", "input device " + device.getName() +
		  " desc " + device.getDescriptor() + ", sources=" + sources +
		  ", haveGamePad=" + haveGamePad);
	    */
	}

	haveImmersive = true;

	nfcSensor = NfcSensor.getInstance(activityContext);
	nfcSensor.onNfcIntent(mActivity.getIntent());

	cachePrefValues();

	/* First time app is launched, show gamepad guide. */
	if(haveGamePad) {
	    SharedPreferences prefs =
		PreferenceManager
		.getDefaultSharedPreferences(activityContext);

	    boolean haveShownGuide =
		prefs.getBoolean("have_shown_gpguide", false);
	    if(!haveShownGuide) {
		SharedPreferences.Editor editor = prefs.edit();
		editor.putBoolean("have_shown_gpguide", true);
		editor.commit();

		Intent gpIntent = new Intent(activityContext, (Class)gpClass);
		gpIntent.putExtra("showBottomText", true);
		activity.startActivity(gpIntent);
	    }
	}


	menuHoverSelection = -1;
	drawables = resDrawables;

	musicDirectory = Environment.getExternalStorageDirectory().getPath() +
	    "/Cyclone2000";

	navHider = new NavHider(this, useFullScreen, haveImmersive);

	gestureScanner = new GestureDetector(activityContext, this);
	gestureScanner.setIsLongpressEnabled(false);

	handler = new Handler();
	syncObj = new Object();

        mRenderer = new C2Renderer(isTV, this, activityContext, handler,
				   syncObj,
				   activityContext.getFilesDir().
				   getAbsolutePath(),
				   prefDisplayMode,
				   prefGfxDetails,
				   (useControlHide ?
				    C2Renderer.CONTROL_TYPE_HIDDEN :
				    prefControlType), prefControlAutofire,
				   prefControlSize, prefControlButtonSize,
				   prefGameStage, prefGameStartStage,
				   prefGameLife, prefGameScore,
				   prefTotalPowerups, prefPupsDisabled,
				   prefPupsItems,
				   resetHs, resetPups);
        setRenderer(mRenderer);

	suspended = false;
	if(view == C2Renderer.VIEW_GAME)
	    game_paused = true;

	runAsTV = isTV;

	// Initialize SoundPool and load sounds.
	soundPool = new SoundPool(16, AudioManager.STREAM_MUSIC, 0);
	sounds = new int[SOUND_NUM];
	streams = new int[SOUND_NUM];
	volumes = new float[SOUND_NUM];

	sounds[SOUND_LASER] =
	    soundPool.load(activityContext, resSounds[SOUND_LASER], 1);
	volumes[SOUND_LASER] = 0.6f;
	sounds[SOUND_EXPLOSION] =
	    soundPool.load(activityContext, resSounds[SOUND_EXPLOSION], 1);
	volumes[SOUND_EXPLOSION] = 0.7f;
	sounds[SOUND_POWERUP] =
	    soundPool.load(activityContext, resSounds[SOUND_POWERUP], 1);
	volumes[SOUND_POWERUP] = 0.8f;
	sounds[SOUND_PLAYER_EXPLOSION] =
	    soundPool.load(activityContext, resSounds[SOUND_PLAYER_EXPLOSION], 1);
	volumes[SOUND_PLAYER_EXPLOSION] = 0.9f;
	sounds[SOUND_ENDSTAGE] =
	    soundPool.load(activityContext, resSounds[SOUND_ENDSTAGE], 1);
	volumes[SOUND_ENDSTAGE] = 1.0f;
	sounds[SOUND_SHOT] =
	    soundPool.load(activityContext, resSounds[SOUND_SHOT], 1);
	volumes[SOUND_SHOT] = 0.9f;
	sounds[SOUND_CLING] =
	    soundPool.load(activityContext, resSounds[SOUND_CLING], 1);
	volumes[SOUND_CLING] = 0.6f;
	sounds[SOUND_LIFE] =
	    soundPool.load(activityContext, resSounds[SOUND_LIFE], 1);
	volumes[SOUND_LIFE] = 1.0f;
	sounds[SOUND_WHOOO] =
	    soundPool.load(activityContext, resSounds[SOUND_WHOOO], 1);
	volumes[SOUND_WHOOO] = 0.7f;
	sounds[SOUND_SUPERZAPPER] =
	    soundPool.load(activityContext, resSounds[SOUND_SUPERZAPPER], 1);
	volumes[SOUND_SUPERZAPPER] = 0.9f;
	sounds[SOUND_YESYESYES] =
	    soundPool.load(activityContext, resSounds[SOUND_YESYESYES], 1);
	volumes[SOUND_YESYESYES] = 0.5f;

	// Timer for when changing stage in menu.
	menuStageUpdateTask = new Runnable() {
		public void run() {
		    long now = System.currentTimeMillis();

		    if(menuStageDownLastTime != -1 &&
		       now - menuStageDownLastTime >= menuStageInterval) {
			menuStageDownLastTime = now;
			sendEvent(C2Renderer.EVENT_MENU_CHOICE,
				  C2Renderer.MENU_CHOICE_STAGE_CHANGE, -1,
				  prefGameMaxStage);
		    } else if(menuStageUpLastTime != -1 &&
			      now - menuStageUpLastTime >= menuStageInterval) {
			menuStageUpLastTime = now;
			sendEvent(C2Renderer.EVENT_MENU_CHOICE,
				  C2Renderer.MENU_CHOICE_STAGE_CHANGE, 1,
				  prefGameMaxStage);
		    }
		    menuStageInterval -= 50;
		    if(menuStageInterval < 50)
			menuStageInterval = 50;

		    menuHandler.postDelayed(menuStageUpdateTask,
					    menuStageInterval / 2);
		}
	    };

	// Timer for scrolling in highscore list.
	hsListUpdateTask = new Runnable() {
		public void run() {
		    if(hsListDir < 0) {
			sendEvent(C2Renderer.EVENT_MENU_CHOICE,
				  C2Renderer.MENU_GESTURE_SCROLL,
				  0, hsListStep * 1000);
		    } else if(hsListDir > 0) {
			sendEvent(C2Renderer.EVENT_MENU_CHOICE,
				  C2Renderer.MENU_GESTURE_SCROLL,
				  0, -hsListStep * 1000);
		    }
		    hsListStep += 1;
		    if(hsListStep > 20)
			hsListStep = 20;

		    menuHandler.postDelayed(hsListUpdateTask, 16);
		}
	    };

	// Timer for scrolling in highscore enter view.
	hsEnterUpdateTask = new Runnable() {
		public void run() {
		    if(hsEnterDir < 0) {
			sendEvent(C2Renderer.EVENT_MENU_CHOICE,
				  C2Renderer.MENU_GESTURE_SCROLL,
				  -hsEnterStep * 600, 0);
		    } else if(hsEnterDir > 0) {
			sendEvent(C2Renderer.EVENT_MENU_CHOICE,
				  C2Renderer.MENU_GESTURE_SCROLL,
				  hsEnterStep * 600, 0);
		    }
		    hsEnterStep += 1;
		    if(hsEnterStep > 60)
			hsEnterStep = 60;

		    menuHandler.postDelayed(hsEnterUpdateTask, 16);
		}
	    };

	// Timer for moving in game.
	gamePadStepUpdateTask = new Runnable() {
		public void run() {
		    gamePadStepOnQueue = false;
		    boolean keep = sendGamePadControls();
		    if(keep && !gamePadStepOnQueue) {
			gamePadStepHandler.postDelayed(gamePadStepUpdateTask,
						       50);
			gamePadStepOnQueue = true;
		    }
		}
	    };

	checkPermissions();
    }

    private void checkPermissions() {
	if(activityContext
	   .checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE)
	   != PackageManager.PERMISSION_GRANTED) {
	    if(mActivity
	       .shouldShowRequestPermissionRationale(Manifest.permission.READ_EXTERNAL_STORAGE)) {
		AlertDialog.Builder builder =
		    new AlertDialog.Builder(activityContext);
		builder.setMessage("Cyclone 2000 needs permission to read " +
				   "and write to the device storage for the " +
				   "music to the game. Please accept the " +
				   "permission request, or music will not " +
				   "be able to play.");
		builder.setPositiveButton("OK",
					  new DialogInterface.OnClickListener() {
					      public void onClick(DialogInterface dialog, int id) {
						  checkPermissions();
					      }
					  });
		AlertDialog alert = builder.create();
		alert.show();
	    } else {
		mActivity.requestPermissions(new String[]{Manifest.permission.READ_EXTERNAL_STORAGE},
					     RC_PERM_STORAGE);
	    }
	} else {
	    permStorage = true;
	}
    }

    public void onRequestPermissionsResult(int requestCode,
					   String permissions[],
					   int[] grantResults) {
	switch(requestCode) {
	case RC_PERM_STORAGE: {
	    // If request is cancelled, the result arrays are empty.
	    if (grantResults.length > 0
		&& grantResults[0] == PackageManager.PERMISSION_GRANTED) {
		permStorage = true;
	    } else {
		permStorage = false;
	    }
	    return;
	}
	}
    }

    // FIXME: Almost exact copy of resetDefaults() in KeyMapView.
    // There should be only one!
    public void resetDefaultsController() {
	SharedPreferences prefs =
	    PreferenceManager.getDefaultSharedPreferences(activityContext);
	SharedPreferences.Editor editor = prefs.edit();

	int numkeys = prefs.getInt("keymap_num", 0);

	/* Only do this first time. */
	if(numkeys != 0)
	    return;

	for(int i = 0 ; i < numkeys ; i++) {
	    editor.remove("keymap_keycode[" + i + "]");
	}
	editor.remove("keymap_num");
	editor.commit();

	editor = prefs.edit();
	int i = 0;
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_DPAD_LEFT);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_LEFT);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_DPAD_RIGHT);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_RIGHT);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_DPAD_UP);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_JUMP);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_DPAD_DOWN);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_ZAP);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_DPAD_CENTER);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_SHOOT);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_SPACE);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_SHOOT);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_BUTTON_A);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_SHOOT);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_BUTTON_X);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_JUMP);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_BUTTON_B);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_ZAP);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_BUTTON_R1);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_JUMP);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_BUTTON_L1);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_ZAP);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_BUTTON_START);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_PAUSE);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_BUTTON_SELECT);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_BACK);

	editor.putInt("keymap_num", i);
	editor.commit();
    }

    private void makeTextureList() {
	textureList = new int[C2Renderer.TEX_NUM];
	int i = 0;

	if(!useControlHide) {
	    if(prefControlType == C2Renderer.CONTROL_TYPE_SINGLE_ARROWS ||
	       prefControlType == C2Renderer.CONTROL_TYPE_MULTI_ARROWS) {
		textureList[i++] = C2Renderer.TEX_CONTROL_FULLARROWS;
	    }

	    if(prefControlType == C2Renderer.CONTROL_TYPE_SINGLE_ARROWS) {
		textureList[i++] = C2Renderer.TEX_CONTROL_ARROWSJUMP;
		textureList[i++] = C2Renderer.TEX_CONTROL_ARROWSZAP;
	    }

	    if(prefControlType == C2Renderer.CONTROL_TYPE_MULTI_CIRCULAR) {
		textureList[i++] = C2Renderer.TEX_CONTROL_CROSS;
	    }

	    if(prefControlType == C2Renderer.CONTROL_TYPE_MULTI_SWIPE ||
	       prefControlType == C2Renderer.CONTROL_TYPE_DUAL_SWIPE) {
		textureList[i++] = C2Renderer.TEX_CONTROL_SWIPE;
	    }

	    if(prefControlType == C2Renderer.CONTROL_TYPE_DUAL_SWIPE) {
		textureList[i++] = C2Renderer.TEX_CONTROL_SWIPEJUMP;
	    }

	    if(prefControlType == C2Renderer.CONTROL_TYPE_MULTI_SWIPE ||
	       prefControlType == C2Renderer.CONTROL_TYPE_MULTI_ARROWS ||
	       prefControlType == C2Renderer.CONTROL_TYPE_MULTI_CIRCULAR) {
		textureList[i++] = C2Renderer.TEX_CONTROL_SHOOT;
		textureList[i++] = C2Renderer.TEX_CONTROL_JUMP;
	    }

	    if(prefControlType == C2Renderer.CONTROL_TYPE_MULTI_SWIPE ||
	       prefControlType == C2Renderer.CONTROL_TYPE_MULTI_ARROWS ||
	       prefControlType == C2Renderer.CONTROL_TYPE_MULTI_CIRCULAR ||
	       prefControlType == C2Renderer.CONTROL_TYPE_DUAL_SWIPE) {
		textureList[i++] = C2Renderer.TEX_CONTROL_ZAP;
	    }

	    textureList[i++] = C2Renderer.TEX_CONTROL_PAUSE;
	    textureList[i++] = C2Renderer.TEX_CONTROL_EXIT;
	}

	textureList[i++] = C2Renderer.TEX_POINT_2000;
	textureList[i++] = C2Renderer.TEX_1UP_UP;
	textureList[i++] = C2Renderer.TEX_1UP_1_GLOW;
	textureList[i++] = C2Renderer.TEX_FUSEBALL_SCORE;
	textureList[i++] = C2Renderer.TEX_PRICE_LIFE;
	textureList[i++] = C2Renderer.TEX_CHARS;
	textureList[i++] = C2Renderer.TEX_CHARS_LIGHT;
	textureList[i++] = C2Renderer.TEX_MENU_ICONS;

	for(int j = 0 ; j < 10 ; j++) {
	    textureList[i++] = C2Renderer.TEX_SCORE_NUMBERS_BASE + j;
	}

	textureListLen = i;
    }

    private void resumeRenderer() {
	sendEvent(C2Renderer.EVENT_CONTROL_TYPE,
		  (useControlHide ? C2Renderer.CONTROL_TYPE_HIDDEN :
		   prefControlType),
		  prefControlAutofire ? 1 : 0,
		  controlSizes[prefControlSize],
		  buttonSizes[prefControlButtonSize]);
	sendEvent(C2Renderer.EVENT_DISPLAY_MODE,
		  prefDisplayMode, prefGfxDetails, 0);
	sendEvent(C2Renderer.EVENT_PUPS_PREFS, prefPupsDisabled ? 1 : 0,
		  resetPups ? 1 : 0, 0);
	if(resetHs)
	    sendEvent(C2Renderer.EVENT_HS_RESET, 1, 0, 0);
	sendEvent(C2Renderer.EVENT_SOUND_SFX, prefSoundSfx, 0, 0);

	suspended = false;

	if(view == C2Renderer.VIEW_GAME)
	    game_paused = true;

	this.setRenderMode(RENDERMODE_CONTINUOUSLY);
	sendEvent(C2Renderer.EVENT_RESUME, 0, 0, 0);
    }

    private void loadBitmapFromFile(int drawableid) {
	InputStream is = activityContext.getResources().
	    openRawResource(drawables[drawableid]);
	mRenderer.bitmap = BitmapFactory.decodeStream(is);
	try {
	    is.close();
	} catch(IOException e) {
	    // Ignore.
	}
    }

    private void loadBitmaps(int texid) {
	//	Log.i("Cyclone2000", "loadBitmaps texid=" + texid +
	//	      ", texture=" + textureList[texid]);

	if(texid == textureListLen) {
	    resumeRenderer();
	    return;
	}

        Canvas canvas = new Canvas();
	InputStream is;
	int ctrlSize = controlTexSizes[prefControlSize];
	int buttonSize = buttonTexSizes[prefControlButtonSize];

	switch(textureList[texid]) {
	case C2Renderer.TEX_CONTROL_FULLARROWS:
	    mRenderer.bitmap = Bitmap.createBitmap(ctrlSize, ctrlSize,
						   Bitmap.Config.ARGB_8888);
	    canvas.setBitmap(mRenderer.bitmap);
	    drawFullArrows(canvas, (float)ctrlSize);
	    break;

	case C2Renderer.TEX_CONTROL_ARROWSJUMP:
	    mRenderer.bitmap = Bitmap.createBitmap(ctrlSize, ctrlSize,
						   Bitmap.Config.ARGB_8888);
	    canvas.setBitmap(mRenderer.bitmap);
	    drawArrowsJump(canvas, (float)ctrlSize);
	    break;

	case C2Renderer.TEX_CONTROL_ARROWSZAP:
	    mRenderer.bitmap = Bitmap.createBitmap(ctrlSize, ctrlSize,
						   Bitmap.Config.ARGB_8888);
	    canvas.setBitmap(mRenderer.bitmap);
	    drawArrowsZap(canvas, (float)ctrlSize);
	    break;

	case C2Renderer.TEX_CONTROL_CROSS:
	    mRenderer.bitmap = Bitmap.createBitmap(ctrlSize, ctrlSize,
						   Bitmap.Config.ARGB_8888);
	    canvas.setBitmap(mRenderer.bitmap);
	    drawCross(canvas, (float)ctrlSize);
	    break;

	case C2Renderer.TEX_CONTROL_SWIPE:
	    mRenderer.bitmap = Bitmap.createBitmap(ctrlSize * 2, ctrlSize / 2,
						   Bitmap.Config.ARGB_8888);
	    canvas.setBitmap(mRenderer.bitmap);
	    drawSwipe(canvas, (float)ctrlSize);
	    break;

	case C2Renderer.TEX_CONTROL_SWIPEJUMP:
	    mRenderer.bitmap = Bitmap.createBitmap(buttonSize, buttonSize,
						   Bitmap.Config.ARGB_8888);
	    canvas.setBitmap(mRenderer.bitmap);
	    drawSwipeJump(canvas, (float)buttonSize);
	    break;

	case C2Renderer.TEX_CONTROL_SHOOT:
	    mRenderer.bitmap = Bitmap.createBitmap(buttonSize, buttonSize,
						   Bitmap.Config.ARGB_8888);
	    canvas.setBitmap(mRenderer.bitmap);
	    drawShoot(canvas, (float)buttonSize);
	    break;

	case C2Renderer.TEX_CONTROL_JUMP:
	    mRenderer.bitmap = Bitmap.createBitmap(buttonSize, buttonSize,
						   Bitmap.Config.ARGB_8888);
	    canvas.setBitmap(mRenderer.bitmap);
	    drawJump(canvas, (float)buttonSize);
	    break;

	case C2Renderer.TEX_CONTROL_ZAP:
	    mRenderer.bitmap = Bitmap.createBitmap(buttonSize, buttonSize,
						   Bitmap.Config.ARGB_8888);
	    canvas.setBitmap(mRenderer.bitmap);
	    drawZap(canvas, (float)buttonSize);
	    break;

	case C2Renderer.TEX_CONTROL_PAUSE:
	    mRenderer.bitmap = Bitmap.createBitmap(buttonSize, buttonSize,
						   Bitmap.Config.ARGB_8888);
	    canvas.setBitmap(mRenderer.bitmap);
	    drawPause(canvas, (float)buttonSize);
	    break;

	case C2Renderer.TEX_CONTROL_EXIT:
	    mRenderer.bitmap = Bitmap.createBitmap(buttonSize, buttonSize,
						   Bitmap.Config.ARGB_8888);
	    canvas.setBitmap(mRenderer.bitmap);
	    drawExit(canvas, (float)buttonSize);
	    break;

	case C2Renderer.TEX_POINT_2000:
	    loadBitmapFromFile(DRAWABLE_P2K);
	    break;

	case C2Renderer.TEX_1UP_UP:
	    loadBitmapFromFile(DRAWABLE_UP);
	    break;

	case C2Renderer.TEX_1UP_1_GLOW:
	    loadBitmapFromFile(DRAWABLE_1_GLOW);
	    break;

	case C2Renderer.TEX_FUSEBALL_SCORE:
	    loadBitmapFromFile(DRAWABLE_FBSCORE);
	    break;

	case C2Renderer.TEX_PRICE_LIFE:
	    loadBitmapFromFile(DRAWABLE_PRICE_LIFE);
	    break;

	case C2Renderer.TEX_CHARS:
	    loadBitmapFromFile(DRAWABLE_CHARS);
	    break;

	case C2Renderer.TEX_CHARS_LIGHT:
	    loadBitmapFromFile(DRAWABLE_CHARS_LIGHT);
	    break;

	case C2Renderer.TEX_MENU_ICONS:
	    loadBitmapFromFile(DRAWABLE_MENU_ICONS);
	    break;

	case C2Renderer.TEX_SCORE_NUMBERS_BASE + 0:
	case C2Renderer.TEX_SCORE_NUMBERS_BASE + 1:
	case C2Renderer.TEX_SCORE_NUMBERS_BASE + 2:
	case C2Renderer.TEX_SCORE_NUMBERS_BASE + 3:
	case C2Renderer.TEX_SCORE_NUMBERS_BASE + 4:
	case C2Renderer.TEX_SCORE_NUMBERS_BASE + 5:
	case C2Renderer.TEX_SCORE_NUMBERS_BASE + 6:
	case C2Renderer.TEX_SCORE_NUMBERS_BASE + 7:
	case C2Renderer.TEX_SCORE_NUMBERS_BASE + 8:
	case C2Renderer.TEX_SCORE_NUMBERS_BASE + 9:
	    int num = textureList[texid] - C2Renderer.TEX_SCORE_NUMBERS_BASE;
	    mRenderer.bitmap =
		Bitmap.createBitmap(128, 256, Bitmap.Config.ARGB_8888);
	    canvas.setBitmap(mRenderer.bitmap);
	    drawNumber(canvas, num);
	    break;
	}

	sendEvent(C2Renderer.EVENT_LOAD_TEX, texid, textureList[texid],
		  texid < textureListLen - 1 ? 0 : 1);
    }

    private String prefIntEncrypt(String seed, int value) {
	String valueString = String.valueOf(value);
	String cryptoPass = "prefValue" + seed;

	try {
	    DESKeySpec keySpec = new DESKeySpec(cryptoPass.getBytes("UTF8"));
	    SecretKeyFactory keyFactory = SecretKeyFactory.getInstance("DES");
	    SecretKey key = keyFactory.generateSecret(keySpec);

	    byte[] clearText = valueString.getBytes("UTF8");
	    Cipher cipher = Cipher.getInstance("DES");
	    cipher.init(Cipher.ENCRYPT_MODE, key);

	    String encryptedValue =
		Base64.encodeToString(cipher.doFinal(clearText),
				      Base64.DEFAULT);

	    return encryptedValue;
	} catch (Exception e) {
	}

	return null;
    }

    private int prefIntDecrypt(String seed, String valueString) {
	String cryptoPass = "prefValue" + seed;

	try {
	    DESKeySpec keySpec = new DESKeySpec(cryptoPass.getBytes("UTF8"));
	    SecretKeyFactory keyFactory = SecretKeyFactory.getInstance("DES");
	    SecretKey key = keyFactory.generateSecret(keySpec);

	    byte[] encryptedPwdBytes =
		Base64.decode(valueString, Base64.DEFAULT);
	    Cipher cipher = Cipher.getInstance("DES");
	    cipher.init(Cipher.DECRYPT_MODE, key);
	    byte[] decryptedValueBytes = (cipher.doFinal(encryptedPwdBytes));

	    String decryptedValue = new String(decryptedValueBytes);

	    return Integer.valueOf(decryptedValue).intValue();
	} catch (Exception e) {
	}

	return -1;
    }

    private void convPrefValues() {
	SharedPreferences prefs =
	    PreferenceManager.getDefaultSharedPreferences(activityContext);
	SharedPreferences.Editor editor = prefs.edit();

	final int CURRVERSION = 1;

	int prefVersion = prefs.getInt("version", 0);
	if(prefVersion < 1) {
	    /* Encrypt game_pref things. */
	    int prefInt;
	    String prefEncryptString;

	    prefInt = prefs.getInt("game_stage", -1);
	    prefEncryptString = prefIntEncrypt("gs", prefInt);
	    editor.putString("c_game_stage", prefEncryptString);
	    editor.remove("game_stage");

	    prefInt = prefs.getInt("game_start_stage", -1);
	    prefEncryptString = prefIntEncrypt("gss", prefInt);
	    editor.putString("c_game_start_stage", prefEncryptString);
	    editor.remove("game_start_stage");

	    prefInt = prefs.getInt("game_life", -1);
	    prefEncryptString = prefIntEncrypt("gl", prefInt);
	    editor.putString("c_game_life", prefEncryptString);
	    editor.remove("game_life");

	    prefInt = prefs.getInt("game_score", -1);
	    prefEncryptString = prefIntEncrypt("gsc", prefInt);
	    editor.putString("c_game_score", prefEncryptString);
	    editor.remove("game_score");

	    prefInt = prefs.getInt("game_max_stage", 0);
	    prefEncryptString = prefIntEncrypt("gms", prefInt);
	    editor.putString("c_game_max_stage", prefEncryptString);
	    editor.remove("game_max_stage");

	    prefInt = prefs.getInt("game_total_powerups", 0);
	    prefEncryptString = prefIntEncrypt("gtp", prefInt);
	    editor.putString("c_game_total_powerups", prefEncryptString);
	    editor.remove("game_total_powerups");

	    for(int idx = 0 ; idx < C2Renderer.PUPS_ITEM_NUM ; idx++) {
		String key = "game_pup[" + idx + "]";

		prefInt = prefs.getInt(key, 0);
		prefEncryptString = prefIntEncrypt("gp", prefInt);
		editor.putString("c_" + key, prefEncryptString);
		editor.remove(key);
	    }
	}

	editor.putInt("version", CURRVERSION);

	editor.commit();
    }

    private void cachePrefValues() {
	SharedPreferences prefs =
	    PreferenceManager.getDefaultSharedPreferences(activityContext);

	resetHs = prefs.getBoolean("reset_hs", false);
	resetPups = prefs.getBoolean("reset_pups", false);

	prefPupsDisabled = prefs.getBoolean("pref_pups_disabled", false);

	if(resetHs) {
	    SharedPreferences.Editor editor = prefs.edit();
	    editor.remove("reset_hs");
	    editor.commit();
	}
	if(resetPups) {
	    SharedPreferences.Editor editor = prefs.edit();
	    editor.remove("c_game_total_powerups");
	    for(int idx = 0 ; idx < C2Renderer.PUPS_ITEM_NUM ; idx++) {
		editor.remove("c_game_pup[" + idx + "]");
	    }
	    editor.remove("reset_pups");
	    editor.commit();
	}

	String typeString = prefs.getString("pref_control", "autoDetect");

	if(typeString.equals("singleArrows"))
	    prefControlType = C2Renderer.CONTROL_TYPE_SINGLE_ARROWS;
	else if(typeString.equals("multiArrows"))
	    prefControlType = C2Renderer.CONTROL_TYPE_MULTI_ARROWS;
	else if(typeString.equals("multiCircular"))
	    prefControlType = C2Renderer.CONTROL_TYPE_MULTI_CIRCULAR;
	else if(typeString.equals("multiSwipe"))
	    prefControlType = C2Renderer.CONTROL_TYPE_MULTI_SWIPE;
	else if(typeString.equals("dualSwipe"))
	    prefControlType = C2Renderer.CONTROL_TYPE_DUAL_SWIPE;
	else
	    prefControlType = C2Renderer.CONTROL_TYPE_AUTO_DETECT;

	prefControlAutofire = prefs.getBoolean("pref_control_autofire", false);

	Integer i = Integer.valueOf(-1);
	prefControlSize =
	    Integer.parseInt(prefs.getString("pref_control_size",
					     i.toString()));
	prefControlButtonSize =
	    Integer.parseInt(prefs.getString("pref_control_button_size",
					     i.toString()));

	String gfxString = prefs.getString("pref_gfx", "high");
	if(gfxString.equals("low"))
	    prefGfxDetails = C2Renderer.GFX_DETAILS_LOW;
	else
	    prefGfxDetails = C2Renderer.GFX_DETAILS_HIGH;

	prefControlHide = prefs.getString("pref_control_hide_opt", null);
	prefFullScreen = prefs.getString("pref_fullscreen_opt", null);

	/* Convert old hide/fullscreen settings to new
	 * If unchecked, set to auto. If checked, set to hide/fullscreen.
	 */
	if(prefControlHide == null) {
	    SharedPreferences.Editor editor = prefs.edit();
	    boolean hide = prefs.getBoolean("pref_control_hide", false);
	    if(hide)
		prefControlHide = "hide";
	    else
		prefControlHide = "auto";
	    editor.putString("pref_control_hide_opt", prefControlHide);
	    editor.remove("pref_control_hide");
	    editor.commit();
	}
	if(prefFullScreen == null) {
	    SharedPreferences.Editor editor = prefs.edit();
	    boolean fullscreen = prefs.getBoolean("pref_fullscreen", false);
	    if(fullscreen)
		prefFullScreen = "nonfull";
	    else
		prefFullScreen = "auto";
	    editor.putString("pref_fullscreen_opt", prefFullScreen);
	    editor.remove("pref_fullscreen");
	    editor.commit();
	}

	if(prefControlHide.equals("hide")) {
	    useControlHide = true;
	} else if(prefControlHide.equals("show")) {
	    useControlHide = false;
	} else if(prefControlHide.equals("auto")) {
	    useControlHide = haveGamePad;
	}

	if(prefFullScreen.equals("full")) {
	    useFullScreen = true;
	} else if(prefFullScreen.equals("nonfull")) {
	    useFullScreen = false;
	} else if(prefFullScreen.equals("auto")) {
	    if(haveImmersive)
		useFullScreen = true;
	    else
		useFullScreen = haveGamePad;
	}

	String modeString = prefs.getString("pref_vr_opt", "auto");
	if(modeString.equals("vrsbs")) {
	    prefDisplayMode = C2Renderer.DISPLAY_MODE_VR;
	    useNfc = false;
	} else if(modeString.equals("vrsbstv")) {
	    prefDisplayMode = C2Renderer.DISPLAY_MODE_VRTV;
	    useNfc = false;
	} else if(modeString.equals("normal")) {
	    prefDisplayMode = C2Renderer.DISPLAY_MODE_FULL;
	    useNfc = false;
	} else if(modeString.equals("auto")) {
	    useNfc = true;
	    if(nfcSensor.isDeviceInCardboard())
		prefDisplayMode = C2Renderer.DISPLAY_MODE_VR;
	    else
		prefDisplayMode = C2Renderer.DISPLAY_MODE_FULL;
	}

	/* In VR mode, always set to fullscreen. */
	if(prefDisplayMode == C2Renderer.DISPLAY_MODE_VR |
	   prefDisplayMode == C2Renderer.DISPLAY_MODE_VRTV) {
	    useFullScreen = true;
	    // FIXME: Disable volume controls
	}


	prefControlSensitivity =
	    (float)prefs.getInt("pref_control_sensitivity",
				(int)DEFAULT_CONTROL_SENSITIVITY);

	prefSoundMusic = prefs.getInt("pref_sound_music", DEFAULT_SOUND_MUSIC);
	prefSoundSfx = prefs.getInt("pref_sound_sfx", DEFAULT_SOUND_SFX);

	prefMusic[0] = prefs.getString("pref_music_01",
				       musicDirectory + "/01.mp3");
	prefMusic[1] = prefs.getString("pref_music_02",
				       musicDirectory + "/02.mp3");
	prefMusic[2] = prefs.getString("pref_music_03",
				       musicDirectory + "/03.mp3");
	prefMusic[3] = prefs.getString("pref_music_04",
				       musicDirectory + "/04.mp3");
	prefMusic[4] = prefs.getString("pref_music_05",
				       musicDirectory + "/05.mp3");
	prefMusic[5] = prefs.getString("pref_music_06",
				       musicDirectory + "/06.mp3");
	prefMusic[6] = prefs.getString("pref_music_07",
				       musicDirectory + "/07.mp3");

	String prefEncryptString;

	prefEncryptString = prefs.getString("c_game_stage", null);
	if(prefEncryptString == null)
	    prefGameStage = -1;
	else
	    prefGameStage = prefIntDecrypt("gs", prefEncryptString);

	prefEncryptString = prefs.getString("c_game_start_stage", null);
	if(prefEncryptString == null)
	    prefGameStartStage = -1;
	else
	    prefGameStartStage = prefIntDecrypt("gss", prefEncryptString);

	prefEncryptString = prefs.getString("c_game_life", null);
	if(prefEncryptString == null)
	    prefGameLife = -1;
	else
	    prefGameLife = prefIntDecrypt("gl", prefEncryptString);

	prefEncryptString = prefs.getString("c_game_score", null);
	if(prefEncryptString == null)
	    prefGameScore = -1;
	else
	    prefGameScore = prefIntDecrypt("gsc", prefEncryptString);

	prefEncryptString = prefs.getString("c_game_max_stage", null);
	if(prefEncryptString == null)
	    prefGameMaxStage = 0;
	else
	    prefGameMaxStage = prefIntDecrypt("gms", prefEncryptString);

	prefEncryptString = prefs.getString("c_game_total_powerups", null);
	if(prefEncryptString == null)
	    prefTotalPowerups = 0;
	else
	    prefTotalPowerups = prefIntDecrypt("gtp", prefEncryptString);

	for(int idx = 0 ; idx < C2Renderer.PUPS_ITEM_NUM ; idx++) {
	    prefEncryptString =
		prefs.getString("c_game_pup[" + idx + "]", null);
	    if(prefEncryptString == null)
		prefPupsItems[idx] = 0;
	    else
		prefPupsItems[idx] = prefIntDecrypt("gp", prefEncryptString);
	}

	prefNumKeys = prefs.getInt("keymap_num", 0);
	prefKeyCodes = new int[prefNumKeys];
	prefControlEvents = new int[prefNumKeys];
	for(int idx = 0 ; idx < prefNumKeys ; idx++) {
	    prefKeyCodes[idx] =
		prefs.getInt("keymap_keycode[" + idx + "]", 0);
	    prefControlEvents[idx] =
		prefs.getInt("keymap_control[" + idx + "]", 0);
	}

	/*
	Log.i("Cyclone2000", "cache prefs type=" + prefControlType +
	      ", ctrlsize=" + prefControlSize +
	      ", buttonsize=" + prefControlButtonSize +
	      ", music=" + prefSoundMusic + ", sfx=" + prefSoundSfx +
	      ", music[0]=" + prefMusic[0] +
	      ", music[1]=" + prefMusic[1] +
	      ", lastStage=" + prefGameStage +
	      ", lastLife=" + prefGameLife +
	      ", lastScore=" + prefGameScore +
	      ", maxStage=" + prefGameMaxStage);
	*/

	if(prefControlType == C2Renderer.CONTROL_TYPE_AUTO_DETECT ||
	   prefControlSize == -1 || prefControlButtonSize == -1) {
	    SharedPreferences.Editor editor = prefs.edit();
	    PackageManager pm = activityContext.getPackageManager();
	    Display d =
		((WindowManager)activityContext.
		 getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
	    int screenWidth = d.getWidth();

	    if(prefControlType == C2Renderer.CONTROL_TYPE_AUTO_DETECT) {
		boolean hasmulti =
		    pm.hasSystemFeature(PackageManager.FEATURE_TOUCHSCREEN_MULTITOUCH);
		boolean hasmultidistinct =
		    pm.hasSystemFeature(PackageManager.FEATURE_TOUCHSCREEN_MULTITOUCH_DISTINCT);

		if(hasmulti) {
		    if(hasmultidistinct) {
			editor.putString("pref_control", "multiSwipe");
			prefControlType = C2Renderer.CONTROL_TYPE_MULTI_SWIPE;
		    } else {
			editor.putString("pref_control", "dualSwipe");
			prefControlType = C2Renderer.CONTROL_TYPE_DUAL_SWIPE;
		    }
		} else {
		    editor.putString("pref_control", "singleArrows");
		    prefControlType = C2Renderer.CONTROL_TYPE_SINGLE_ARROWS;
		}
	    }


	    if(prefControlSize == -1) {
		int size = (int)(0.23f * (float)screenWidth);
		int id = controlSizes.length - 1;
		for(int j = 0 ; j < controlSizes.length - 1 ; j++) {
		    if(Math.abs(controlSizes[j] - size) <
		       Math.abs(controlSizes[j + 1] - size)) {
			id = j;
			break;
		    }
		}
		editor.putString("pref_control_size",
				 String.valueOf(id));
		prefControlSize = id;
	    }

	    if(prefControlButtonSize == -1) {
		int size = (int)(0.11f * (float)screenWidth);
		int id = buttonSizes.length - 1;
		for(int j = 0 ; j < buttonSizes.length - 1 ; j++) {
		    if(Math.abs(buttonSizes[j] - size) <
		       Math.abs(buttonSizes[j + 1] - size)) {
			id = j;
			break;
		    }
		}
		editor.putString("pref_control_button_size",
				 String.valueOf(id));
		prefControlButtonSize = id;
	    }

	    editor.commit();
	}
    }

    private void storePrefValues(int gameStage, int gameLife, int gameScore,
				 int gameStartStage, int totalPowerups) {
	SharedPreferences prefs =
	    PreferenceManager.getDefaultSharedPreferences(activityContext);
	SharedPreferences.Editor editor = prefs.edit();

	/*
	Log.i("Cyclone2000", "store prefs lastStage=" + gameStage +
	      ", lastLife=" + gameLife +
	      ", lastScore=" + gameScore);
	*/

	prefGameStage = gameStage;
	prefGameLife = gameLife;
	prefGameScore = gameScore;
	prefGameStartStage = gameStartStage;
	prefTotalPowerups = totalPowerups;

	if(gameStage == -1)
	    editor.remove("c_game_stage");
	else
	    editor.putString("c_game_stage",
			     prefIntEncrypt("gs", gameStage));

	if(gameStartStage == -1)
	    editor.remove("c_game_start_stage");
	else
	    editor.putString("c_game_start_stage",
			     prefIntEncrypt("gss", gameStartStage));

	if(gameLife == -1)
	    editor.remove("c_game_life");
	else
	    editor.putString("c_game_life",
			     prefIntEncrypt("gl", gameLife));

	if(gameScore == -1)
	    editor.remove("c_game_score");
	else
	    editor.putString("c_game_score",
			     prefIntEncrypt("gsc", gameScore));

	if(gameStage > prefGameMaxStage) {
	    prefGameMaxStage = gameStage;
	    editor.putString("c_game_max_stage",
			     prefIntEncrypt("gms", prefGameMaxStage));
	}

	editor.putString("c_game_total_powerups",
			 prefIntEncrypt("gtp", totalPowerups));

	editor.commit();
    }

    private void storePrefPupValue(int pup, int value) {
	SharedPreferences prefs =
	    PreferenceManager.getDefaultSharedPreferences(activityContext);
	SharedPreferences.Editor editor = prefs.edit();

	//	Log.i("Cyclone2000", "store pup prefs pup=" + pup + ", value=" + value);

	prefPupsItems[pup] = value;
	editor.putString("c_game_pup[" + pup + "]",
			 prefIntEncrypt("gp", value));

	editor.commit();
    }

    private void sendEvent(int event, int data1, int data2,
			   int data3, int data4) {
	final int ievent = event;
	final int idata1 = data1;
	final int idata2 = data2;
	final int idata3 = data3;
	final int idata4 = data4;

	queueEvent(new Runnable(){
		public void run() {
		    mRenderer.createEvent(ievent, idata1, idata2,
					  idata3, idata4);
		}});
    }

    private void sendEvent(int event, int data1, int data2, int data3) {
	sendEvent(event, data1, data2, data3, 0);
    }

    private void startMusic() {
	int musicId = curr_stage / 16;

	if(prefSoundMusic == 0) {
	    stopMusic();
	    return;
	}

	if(mediaPlayer == null) {
	    mediaPlayer = new MediaPlayer();
	    currentMusic = new String("");
	}

	if(mediaPlayer != null) {
	    if(!currentMusic.equals(prefMusic[musicId])) {
		currentMusic = prefMusic[musicId];
		mediaPlayer.reset();
		try {
		    mediaPlayer.setDataSource(currentMusic);
		    mediaPlayer.prepare();
		} catch(IOException exception) {
		    mediaPlayer = null;
		}
	    }

	    if(mediaPlayer != null) {
		mediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
		mediaPlayer.setLooping(true);
		mediaPlayer.setVolume((float)prefSoundMusic / 100.0f,
				      (float)prefSoundMusic / 100.0f);
		mediaPlayer.start();
	    }
	}
    }

    void stopMusic() {
	if(mediaPlayer == null)
	    return;

	mediaPlayer.release();
	mediaPlayer = null;
    }

    void pauseMusic() {
	if(mediaPlayer == null || !mediaPlayer.isPlaying())
	    return;

	mediaPlayer.pause();
    }


    public void callback(int type, int data1, int data2,
			 int data3, int data4, int data5) {
	//	Log.i("Cyclone2000", "UI callback: type=" + type +
	//	      ", data1=" + data1 + ", data2=" + data2 + ", data3=" + data3);
	float sfxvol = ((float)prefSoundSfx) / 100.0f;

	switch(type) {
	    // Local callback from Renderer
	case C2Renderer.CALLBACK_LOAD_TEX:
	    loadBitmaps(data1);
	    break;

	    // Callbacks from native code

	case C2Renderer.CALLBACK_STATE_SAVE:
	    storePrefValues(data1, data2, data3, data4, data5);
	    break;

	case C2Renderer.CALLBACK_PUPS_SAVE:
	    storePrefPupValue(data1, data2);
	    break;

	case C2Renderer.CALLBACK_PLAY_SOUND:
	    soundPool.stop(streams[data1]);
	    streams[data1] = soundPool.play(sounds[data1],
					    sfxvol * volumes[data1],
					    sfxvol * volumes[data1],
					    0, 0, 1.0f);
	    break;

	case C2Renderer.CALLBACK_GO_VIEW:
	    view = data1;
	    if(view == C2Renderer.VIEW_GAMEOVER) {
		highscorePos = data3;
		highscore = data4;
		highstage = data5;
		pauseMusic();
	    }
	    break;

	case C2Renderer.CALLBACK_GAME_INFO:
	    game_paused = (data1 == 0 ? false : true);
	    curr_stage = data2;
	    int isnewstage = data3;
	    if(!game_paused)
		startMusic();
	    break;

	case C2Renderer.CALLBACK_MENU_HOVER:
	    menuHoverSelection = data1;
	    break;

	case C2Renderer.CALLBACK_PUPS_ADD:
	    currentStagePups += data1;
	    break;

	default:
	    break;
	}
    }

    public void onPause() {
	for(int i = 1 ; i < SOUND_NUM ; i++)
	    soundPool.pause(streams[i]);
	pauseMusic();

    	super.onPause();

	//	Log.i("Cyclone2000", "onPause");

	nfcSensor.onPause(mActivity);

	suspended = true;
    	this.setRenderMode(RENDERMODE_WHEN_DIRTY);
	sendEvent(C2Renderer.EVENT_PAUSE, 0, 0, 0);
    }

    public void onResume() {
	nfcSensor.onResume(mActivity);

	cachePrefValues();

	navHider.setFull(useFullScreen);

	makeTextureList();

    	super.onResume();

	//	Log.i("Cyclone2000", "onResume view=" + view);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        super.surfaceDestroyed(holder);
    }

    private void openOptions(boolean goToMusic) {
	// Start Preferences activity
	Intent prefIntent = new Intent(activityContext,
			       (Class)preferencesClass);
	if(goToMusic)
	    prefIntent.putExtra("goToMusic", true);
	activityContext.startActivity(prefIntent);
    }

    private void openPups() {
	view = C2Renderer.VIEW_NONE;
	sendEvent(C2Renderer.EVENT_MENU_CHOICE,
		  C2Renderer.MENU_CHOICE_PUPS, 0, 0);
    }

    private void closePups() {
	view = C2Renderer.VIEW_NONE;
	sendEvent(C2Renderer.EVENT_MENU_CHOICE,
		  C2Renderer.MENU_GESTURE_CLOSE, 0, 0);
    }

    private void openHighscore() {
	view = C2Renderer.VIEW_NONE;
	sendEvent(C2Renderer.EVENT_MENU_CHOICE,
		  C2Renderer.MENU_CHOICE_HIGHSCORE, 0, 0);
    }

    private void closeHighscore() {
	view = C2Renderer.VIEW_NONE;
	sendEvent(C2Renderer.EVENT_MENU_CHOICE,
		  C2Renderer.MENU_GESTURE_CLOSE, 0, 0);
    }

    private int keyToControl(int keyCode) {
	for(int i = 0 ; i < prefNumKeys ; i++) {
	    if(prefKeyCodes[i] == keyCode)
		return prefControlEvents[i];
	}

	return -1;
    }

    private boolean sendGamePadControls() {
	float speed;

	speed = gamePadJoySpeed;
	if(gamePadStepDownTime != -1) {
	    float factor = 12.5f + (10 - prefControlSensitivity) * 2.5f;

	    speed = (400 + System.currentTimeMillis() -
		     gamePadStepDownTime) / factor;
	    if(speed > 60.0f)
		speed = 60.0f;
	    if(gamePadStepRight == 1)
		speed = -speed;
	} else if(gamePadJoyDownTime != -1) {
	    float downTime = System.currentTimeMillis() - gamePadJoyDownTime;
	    float maxTime = 1500.0f - (float)prefControlSensitivity * 50.0f;
	    if(downTime > maxTime)
		downTime = maxTime;
	    float sense = (float)prefControlSensitivity;
	    float speedFactor = 0.6f * downTime / maxTime + 0.4f;

	    speed *= speedFactor;
	}

	final float ispeed = speed;
	final int istep = 0;
	final int ishoot = gamePadShoot;
	final int ijump = gamePadJump;
	final int izap = gamePadZap;
	queueEvent(new Runnable(){
		public void run() {
		    mRenderer.playerSetControl(ispeed, istep,
					       ishoot, ijump, izap);
		}});

	if(speed == 0.0)
	    return false;
	else
	    return true;
    }

    public boolean onKeyDown(int keyCode, KeyEvent event) {
	//	Log.i("Cyclone2000", "onKeyDown view=" + view);

	if(view == C2Renderer.VIEW_NONE) {
	    return true;
	} else if(view == C2Renderer.VIEW_MENU) {
	    long now = System.currentTimeMillis();

	    switch(keyCode) {
	    case KeyEvent.KEYCODE_DPAD_LEFT:
		if(menuStageDownLastTime == -1) {
		    menuStageDownLastTime = now - menuStageInterval;
		    menuHandler.postDelayed(menuStageUpdateTask, 50);
		}
		return true;
	    case KeyEvent.KEYCODE_DPAD_RIGHT:
		if(menuStageUpLastTime == -1) {
		    menuStageUpLastTime = now - menuStageInterval;
		    menuHandler.postDelayed(menuStageUpdateTask, 50);
		}
		return true;
	    }
	} else if(view == C2Renderer.VIEW_HS) {
	    long now = System.currentTimeMillis();

	    switch(keyCode) {
	    case KeyEvent.KEYCODE_DPAD_DOWN:
		if(hsListDir == 0) {
		    hsListDir = -1;
		    menuHandler.postDelayed(hsListUpdateTask, 16);
		}
		return true;
	    case KeyEvent.KEYCODE_DPAD_UP:
		if(hsListDir == 0) {
		    hsListDir = 1;
		    menuHandler.postDelayed(hsListUpdateTask, 16);
		}
		return true;
	    }
	} else if(view == C2Renderer.VIEW_HSENTER) {
	    long now = System.currentTimeMillis();

	    //	    Log.d("Cyclone2000", "keyCode " + KeyMapCode.keyMapString(keyCode));

	    switch(keyCode) {
	    case KeyEvent.KEYCODE_DPAD_LEFT:
		if(hsEnterDir == 0) {
		    hsEnterDir = -1;
		    sendEvent(C2Renderer.EVENT_MENU_CHOICE,
			      C2Renderer.MENU_GESTURE_DOWN,
			      0, -1000);
		    menuHandler.postDelayed(hsEnterUpdateTask, 16);
		}
		return true;
	    case KeyEvent.KEYCODE_DPAD_RIGHT:
		if(hsEnterDir == 0) {
		    hsEnterDir = 1;
		    sendEvent(C2Renderer.EVENT_MENU_CHOICE,
			      C2Renderer.MENU_GESTURE_DOWN,
			      0, -1000);
		    menuHandler.postDelayed(hsEnterUpdateTask, 16);
		}
		return true;
	    }
	} else if(view == C2Renderer.VIEW_GAME) {
	    if(keyCode == KeyEvent.KEYCODE_BACK)
		return true;

	    boolean ignoreDpad = false;

	    if(keyCode == KeyEvent.KEYCODE_DPAD_UP ||
	       keyCode == KeyEvent.KEYCODE_DPAD_DOWN ||
	       keyCode == KeyEvent.KEYCODE_DPAD_LEFT ||
	       keyCode == KeyEvent.KEYCODE_DPAD_RIGHT) {
		if(gamePadJoySpeed != 0.0)
		    ignoreDpad = true;
	    }

	    int control = keyToControl(keyCode);

	    boolean oldstep = (gamePadStepLeft == 1 || gamePadStepRight == 1);

	    switch(control) {
	    case C2GLSurfaceView.CONTROL_EVENT_LEFT:
		if(gamePadJoySpeed == 0.0) {
		    if(!oldstep)
			gamePadStepDownTime = System.currentTimeMillis();
		    gamePadStepLeft = 1;
		    gamePadJoyDownTime = -1;
		} else {
		    gamePadStepLeft = 0;
		    gamePadStepRight = 0;
		    gamePadStepDownTime = -1;
		}
		break;
	    case C2GLSurfaceView.CONTROL_EVENT_RIGHT:
		if(gamePadJoySpeed == 0.0) {
		    if(!oldstep)
			gamePadStepDownTime = System.currentTimeMillis();
		    gamePadStepRight = 1;
		    gamePadJoyDownTime = -1;
		} else {
		    gamePadStepLeft = 0;
		    gamePadStepRight = 0;
		    gamePadStepDownTime = -1;
		}
		break;
	    case C2GLSurfaceView.CONTROL_EVENT_SHOOT:
		if(!ignoreDpad)
		    gamePadShoot = 1;
		break;
	    case C2GLSurfaceView.CONTROL_EVENT_JUMP:
		if(!ignoreDpad)
		    gamePadJump = 1;
		break;
	    case C2GLSurfaceView.CONTROL_EVENT_ZAP:
		if(!ignoreDpad)
		    gamePadZap = 1;
		break;
	    case C2GLSurfaceView.CONTROL_EVENT_PAUSE:
		if(!ignoreDpad)
		    touchEventPause();
		break;
	    case C2GLSurfaceView.CONTROL_EVENT_BACK:
		break;
	    default:
		return false;
	    }

	    if(!gamePadStepOnQueue) {
		gamePadStepHandler.postDelayed(gamePadStepUpdateTask, 1);
		gamePadStepOnQueue = true;
	    }

	    return true;
	}

	return false;
    }

    public boolean onKeyUp(int keyCode, KeyEvent event) {
	//	Log.i("Cyclone2000", "onKeyUp view=" + view +
	//	      ", keyCode=" + keyCode +
	//	      " (" + KeyMapCode.keyMapString(keyCode) + ")");

	// Always go to options on Menu key.
	if(keyCode == KeyEvent.KEYCODE_MENU) {
	    openOptions(false);
	    return true;
	}

	if(view == C2Renderer.VIEW_NONE) {
	    return true;
	} else if(view == C2Renderer.VIEW_MENU) {
	    switch(keyCode) {
	    case KeyEvent.KEYCODE_DPAD_UP:
		sendEvent(C2Renderer.EVENT_MENU_CHOICE,
			  C2Renderer.MENU_CHOICE_HOVERSTEP, -1, 0);
		return true;
	    case KeyEvent.KEYCODE_DPAD_DOWN:
		sendEvent(C2Renderer.EVENT_MENU_CHOICE,
			  C2Renderer.MENU_CHOICE_HOVERSTEP, 1, 0);
		return true;
	    case KeyEvent.KEYCODE_DPAD_CENTER:
		if(menuHoverSelection ==
		   C2Renderer.MENU_HOVER_LEADERBOARD) {
		    openHighscore();
		} else if(menuHoverSelection ==
			  C2Renderer.MENU_HOVER_POWERUPS) {
		    openPups();
		} else if(menuHoverSelection ==
			  C2Renderer.MENU_HOVER_SETTINGS) {
		    openOptions(false);
		} else if(menuHoverSelection ==
			  C2Renderer.MENU_HOVER_CONTINUE) {
		    view = C2Renderer.VIEW_GAME;
		    sendEvent(C2Renderer.EVENT_MENU_CHOICE,
			      C2Renderer.MENU_CHOICE_CONTINUE, 0, 0);
		} else if(menuHoverSelection ==
			  C2Renderer.MENU_HOVER_NEWGAME) {
		    view = C2Renderer.VIEW_GAME;
		    sendEvent(C2Renderer.EVENT_MENU_CHOICE,
			      C2Renderer.MENU_CHOICE_NEWGAME, 0, 0);
		}
		return true;
	    case KeyEvent.KEYCODE_DPAD_LEFT:
		menuStageDownLastTime = -1;
		menuStageInterval = 400;
		menuHandler.removeCallbacks(menuStageUpdateTask);
		return true;
	    case KeyEvent.KEYCODE_DPAD_RIGHT:
		menuStageUpLastTime = -1;
		menuStageInterval = 400;
		menuHandler.removeCallbacks(menuStageUpdateTask);
		return true;
	    case KeyEvent.KEYCODE_BACK:
		AlertDialog.Builder builder =
		    new AlertDialog.Builder(activityContext);
		builder.setMessage("Are you sure you want to exit?");
		builder.setCancelable(true);
		builder.setPositiveButton("Yes",
					  new DialogInterface.OnClickListener() {
					      public void onClick(DialogInterface dialog, int id) {
						  mActivity.finish();
					      }
					  });
		builder.setNegativeButton("No",
					  new DialogInterface.OnClickListener() {
					      public void onClick(DialogInterface dialog, int id) {
						  dialog.cancel();
					      }
					  });
		AlertDialog alert = builder.create();
		alert.show();
		return true;
	    }
	} else if(view == C2Renderer.VIEW_PUPS) {
	    switch(keyCode) {
	    case KeyEvent.KEYCODE_DPAD_UP:
		sendEvent(C2Renderer.EVENT_MENU_CHOICE,
			  C2Renderer.MENU_CHOICE_HOVERSTEP, -1, 0);
		return true;
	    case KeyEvent.KEYCODE_DPAD_DOWN:
		sendEvent(C2Renderer.EVENT_MENU_CHOICE,
			  C2Renderer.MENU_CHOICE_HOVERSTEP, 1, 0);
		return true;
	    case KeyEvent.KEYCODE_DPAD_CENTER:
		sendEvent(C2Renderer.EVENT_MENU_CHOICE,
			  C2Renderer.MENU_CHOICE_HOVERSELECT, 1, 0);
		return true;
	    case KeyEvent.KEYCODE_BACK:
		closePups();
		return true;
	    }
	} else if(view == C2Renderer.VIEW_HS) {
	    switch(keyCode) {
	    case KeyEvent.KEYCODE_DPAD_DOWN:
		hsListDir = 0;
		hsListStep = 1;
		menuHandler.removeCallbacks(hsListUpdateTask);
		return true;
	    case KeyEvent.KEYCODE_DPAD_UP:
		hsListDir = 0;
		hsListStep = 1;
		menuHandler.removeCallbacks(hsListUpdateTask);
		return true;
	    case KeyEvent.KEYCODE_BACK:
		closeHighscore();
		return true;
	    }
	} else if(view == C2Renderer.VIEW_HSENTER) {
	    switch(keyCode) {
	    case KeyEvent.KEYCODE_DPAD_LEFT:
		hsEnterDir = 0;
		hsEnterStep = 1;
		menuHandler.removeCallbacks(hsEnterUpdateTask);
		return true;
	    case KeyEvent.KEYCODE_DPAD_RIGHT:
		hsEnterDir = 0;
		hsEnterStep = 1;
		menuHandler.removeCallbacks(hsEnterUpdateTask);
		return true;
	    case KeyEvent.KEYCODE_DPAD_UP:
	    case KeyEvent.KEYCODE_BUTTON_X:
		sendEvent(C2Renderer.EVENT_MENU_CHOICE,
			  C2Renderer.MENU_GESTURE_TAP,
			  0, HS_FAKE_UP);
		return true;
	    case KeyEvent.KEYCODE_DPAD_DOWN:
	    case KeyEvent.KEYCODE_BUTTON_Y:
		sendEvent(C2Renderer.EVENT_MENU_CHOICE,
			  C2Renderer.MENU_GESTURE_TAP,
			  0, HS_FAKE_BACK);
		return true;
	    case KeyEvent.KEYCODE_BACK:
		sendEvent(C2Renderer.EVENT_MENU_CHOICE,
			  C2Renderer.MENU_GESTURE_CLOSE, 0, 0);
		return true;
	    }
	} else if(view == C2Renderer.VIEW_GAME) {
	    boolean gamePad = false;
	    boolean joystick = false;
	    boolean doBack = false;

	    if((event.getSource() & InputDevice.SOURCE_GAMEPAD) != 0)
		gamePad = true;
	    else if((event.getSource() & InputDevice.SOURCE_JOYSTICK) != 0)
		joystick = true;

	    if(keyCode == KeyEvent.KEYCODE_BACK)
		doBack = true;

	    int control = keyToControl(keyCode);

	    if(gamePad || joystick) {
		switch(control) {
		case C2GLSurfaceView.CONTROL_EVENT_LEFT:
		    gamePadStepLeft = 0;
		    if(gamePadStepRight == 0)
			gamePadStepDownTime = -1;
		    if(joystick) {
			gamePadJoySpeed = 0;
			gamePadJoyDownTime = -1;
		    }
		    break;
		case C2GLSurfaceView.CONTROL_EVENT_RIGHT:
		    gamePadStepRight = 0;
		    if(gamePadStepLeft == 0)
			gamePadStepDownTime = -1;
		    if(joystick) {
			gamePadJoySpeed = 0;
			gamePadJoyDownTime = -1;
		    }
		    break;
		case C2GLSurfaceView.CONTROL_EVENT_SHOOT:
		    gamePadShoot = 0;
		    break;
		case C2GLSurfaceView.CONTROL_EVENT_JUMP:
		    gamePadJump = 0;
		    break;
		case C2GLSurfaceView.CONTROL_EVENT_ZAP:
		    gamePadZap = 0;
		    break;
		case C2GLSurfaceView.CONTROL_EVENT_BACK:
		    doBack = true;
		    break;
		default:
		    break;
		}

		if(!gamePadStepOnQueue) {
		    gamePadStepHandler.postDelayed(gamePadStepUpdateTask, 1);
		    gamePadStepOnQueue = true;
		}
	    } else {
		switch(keyCode) {
		case KeyEvent.KEYCODE_BACK:
		    doBack = true;
		    break;
		}
	    }

	    if(doBack) {
		touchEventExit();
		return true;
	    }

	    if(!gamePad && !joystick)
		return false;

	} else if(view == C2Renderer.VIEW_GAMEOVER) {
	    switch(keyCode) {
	    case KeyEvent.KEYCODE_DPAD_CENTER:
	    case KeyEvent.KEYCODE_BACK:
		if(highscorePos >= 0) {
		    view = C2Renderer.VIEW_HSENTER;
		    sendEvent(C2Renderer.EVENT_GO_VIEW, view,
			      highscorePos, 0);
		} else {
		    view = C2Renderer.VIEW_MENU;
		    sendEvent(C2Renderer.EVENT_GO_VIEW, view, 0, 0);
		}
		return true;
	    }
	}

	return false;
    }

    // For GamePad analog control
    public boolean onGenericMotionEvent(MotionEvent event) {
	if(view != C2Renderer.VIEW_GAME)
	    return false;

	if((event.getSource() & InputDevice.SOURCE_JOYSTICK) == 0)
	    return false;

	// FIXME: Cache this and use InputDeviceManager
	int deviceId = event.getDeviceId();
	InputDevice device = InputDevice.getDevice(deviceId);
	InputDevice.MotionRange motionRange =
	    device.getMotionRange(MotionEvent.AXIS_X, event.getSource());
	float flatValue = motionRange.getFlat();
	if(flatValue < 0.1f)
	    flatValue = 0.1f;

	float axisValue = event.getAxisValue(MotionEvent.AXIS_X);
	if(Math.abs(axisValue) < motionRange.getFlat()) {
	    gamePadJoySpeed = 0;
	    gamePadJoyDownTime = -1;
	} else {
	    float currentSpeed = 60.0f * axisValue;
	    float sign = Math.signum(currentSpeed);
	    currentSpeed = Math.abs(currentSpeed);

	    gamePadJoySpeed = -sign *
		(float)Math.pow(currentSpeed *
				joySensFactor[(int)prefControlSensitivity-1],
				joySensExp[(int)prefControlSensitivity-1]);
	    gamePadStepRight = 0;
	    gamePadStepLeft = 0;
	    gamePadStepDownTime = -1;
	    gamePadJoyDownTime = System.currentTimeMillis();
	}

	if(!gamePadStepOnQueue) {
	    gamePadStepHandler.postDelayed(gamePadStepUpdateTask, 1);
	    gamePadStepOnQueue = true;
	}

	return false;
    }

    // Main touch event handler.
    public boolean onTouchEvent(final MotionEvent event) {
	boolean ret = false;

	//	Log.i("Cyclone2000", "onTouchEvent view=" + view);

	if(view == C2Renderer.VIEW_MENU) {
	    ret = touchEventMenu(event);
	} else if(view == C2Renderer.VIEW_GAME) {
	    ret = touchEventGame(event);
	} else if(view == C2Renderer.VIEW_GAMEOVER) {
	    ret = touchEventGameOver(event);
	} else if(view == C2Renderer.VIEW_PUPS ||
		  view == C2Renderer.VIEW_HS ||
		  view == C2Renderer.VIEW_HSENTER) {
	    ret = touchEventGesture(event);
	}

	synchronized(syncObj) {
	    try {
		syncObj.wait(100L);
	    } catch(InterruptedException e) {
	    }
	}

	return ret;
    }

    private boolean touchEventMenu(final MotionEvent event) {
	int action = event.getAction() & MotionEvent.ACTION_MASK;
	float x = event.getX();
	float y = event.getY();

	float width = mRenderer.screenWidth;
	float height = mRenderer.screenHeight;
	float r0 = 720.0f / 1196.0f;
	float r1 = height / width;
	float rd = (r1 / r0);
	float rd0 = (1.0f - rd) / 2.0f;
	rd += rd0;

	float continueMiny = height * 245.0f / 720.0f;
	float continueMaxy = height * 349.0f / 720.0f;
	float continueMaxx = rd * width * 760.0f / 1196.0f;
	float playMiny = height * 349.0f / 720.0f;
	float playMaxy = height * 457.0f / 720.0f;
	float playMaxx = rd * width * 590.0f / 1196.0f;
	float playDownMinx = rd * width * 750.0f / 1196.0f;
	float playDownMaxx = rd * width * 860.0f / 1196.0f;
	float playUpMinx = rd * width * 915.0f / 1196.0f;
	float playUpMaxx = rd * width * 1025.0f / 1196.0f;
	float optionsMiny = height * 457.0f / 720.0f;
	float optionsMaxy = height * 565.0f / 720.0f;
	float optionsMaxx = rd * width * 512.0f / 1196.0f;
	float highscoreMiny = optionsMiny;
	float highscoreMaxy = optionsMaxy;
	float highscoreMinx = optionsMaxx;
	float bfgMiny = height * 565.0f / 720.0f;
	float bfgMaxy = height;
	float iconsMiny = height * 460.0f / 720.0f;
	float iconsMaxy = height * 680.0f / 720.0f;
	float leaderboardMinx = rd * width * 100.0f / 1196.0f;
	float leaderboardMaxx = rd * width * 315.0f / 1196.0f;
	float powerupsMinx = rd * width * 315.0f / 1196.0f;
	float powerupsMaxx = rd * width * 470.0f / 1196.0f;
	float settingsMinx = rd * width * 470.0f / 1196.0f;
	float settingsMaxx = rd * width * 680.0f / 1196.0f;
	long now = System.currentTimeMillis();

	if(action == MotionEvent.ACTION_DOWN) {
	    menuStageDownLastTime = -1;
	    menuStageUpLastTime = -1;
	    menuStageInterval = 400;
	    menuHandler.removeCallbacks(menuStageUpdateTask);

	    if(y > continueMiny && y < continueMaxy) {
		view = C2Renderer.VIEW_GAME;
		sendEvent(C2Renderer.EVENT_MENU_CHOICE,
			  C2Renderer.MENU_CHOICE_CONTINUE, 0, 0);
	    } else if(y > playMiny && y < playMaxy) {
		if(x > playDownMinx && x < playDownMaxx) {
		    menuStageDownLastTime = now - menuStageInterval;
		    menuHandler.postDelayed(menuStageUpdateTask, 50);
		} else if(x > playUpMinx && x < playUpMaxx) {
		    menuStageUpLastTime = now - menuStageInterval;
		    menuHandler.postDelayed(menuStageUpdateTask, 50);
		} else if(x < playMaxx) {
		    view = C2Renderer.VIEW_GAME;
		    sendEvent(C2Renderer.EVENT_MENU_CHOICE,
			      C2Renderer.MENU_CHOICE_NEWGAME, 0, 0);
		}
	    } else if(y > iconsMiny && y < iconsMaxy) {
		if(x > leaderboardMinx && x < leaderboardMaxx) {
		    openHighscore();
		} else if(x > powerupsMinx && x < powerupsMaxx) {
		    openPups();
		} else if(x > settingsMinx && x < settingsMaxx) {
		    openOptions(false);
		}
	    }
	} else if(action == MotionEvent.ACTION_UP) {
	    menuStageDownLastTime = -1;
	    menuStageUpLastTime = -1;
	    menuHandler.removeCallbacks(menuStageUpdateTask);
	} else if(action == MotionEvent.ACTION_MOVE) {
	    if(menuStageDownLastTime != -1 &&
	       (x < playDownMinx || x > playDownMaxx ||
		y < playMiny || y > playMaxy)) {
		menuStageDownLastTime = -1;
		menuHandler.removeCallbacks(menuStageUpdateTask);
	    } else if(menuStageUpLastTime != -1 &&
		      (x < playUpMinx || x > playUpMaxx ||
		       y < playMiny || y > playMaxy)) {
		menuStageUpLastTime = -1;
		menuHandler.removeCallbacks(menuStageUpdateTask);
	    }
	}

	return true;
    }

    private boolean touchEventGameOver(final MotionEvent event) {
	int action = event.getAction() & MotionEvent.ACTION_MASK;

	if(action == MotionEvent.ACTION_DOWN) {
	    if(highscorePos >= 0) {
		view = C2Renderer.VIEW_HSENTER;
		sendEvent(C2Renderer.EVENT_GO_VIEW, view, highscorePos, 0);
	    } else {
		view = C2Renderer.VIEW_MENU;
		sendEvent(C2Renderer.EVENT_GO_VIEW, view, 0, 0);
	    }
	}

	return true;
    }

    private boolean touchEventGame(final MotionEvent event) {
	currentSpeed = 0.0f;
	currentStep = 0;
	currentShoot = ongoingShoot;
	currentJump = 0;
	currentZap = 0;

	touchEventButtons(event);

	if(prefControlType == C2Renderer.CONTROL_TYPE_MULTI_ARROWS)
	    touchEventMultiArrows(event);
	else if(prefControlType == C2Renderer.CONTROL_TYPE_MULTI_CIRCULAR)
	    touchEventMultiCircular(event);
	else if(prefControlType == C2Renderer.CONTROL_TYPE_MULTI_SWIPE)
	    touchEventMultiSwipe(event);
	else if(prefControlType == C2Renderer.CONTROL_TYPE_DUAL_SWIPE)
	    touchEventDualSwipe(event);
	else
	    touchEventSingleArrows(event);

	ongoingShoot = currentShoot;

	final float ispeed = currentSpeed;
	final int istep = currentStep;
	final int ishoot = currentShoot;
	final int ijump = currentJump;
	final int izap = currentZap;

	queueEvent(new Runnable(){
		public void run() {
		    mRenderer.playerSetControl(ispeed, istep,
					       ishoot, ijump, izap);
		}});

	return true;
    }

    private void touchEventButtons(final MotionEvent event) {
	int action = event.getAction() & MotionEvent.ACTION_MASK;
	float width = mRenderer.screenWidth;
	float height = mRenderer.screenHeight;
	float buttonsize = buttonSizes[prefControlButtonSize];
	float x = event.getX();
	float y = event.getY();

	switch(action) {
	case MotionEvent.ACTION_DOWN:
	case MotionEvent.ACTION_POINTER_DOWN:
	    if(x > width - buttonsize &&
	       y < buttonsize) {
		touchEventPause();
	    } else if(game_paused &&
		      x > width - buttonsize &&
		      y > buttonsize && y < buttonsize * 2) {
		touchEventExit();
	    }
	}
    }

    private boolean touchEventSingleArrows(final MotionEvent event) {
	int action = event.getAction() & MotionEvent.ACTION_MASK;

	float width = mRenderer.screenWidth;
	float height = mRenderer.screenHeight;

	float ctrlsize = controlSizes[prefControlSize];
	float movesize = ctrlsize * 0.385f;
	float jumpmovesize = ctrlsize * 0.769f;

    	float x = event.getX();
    	float y = event.getY();

	currentShoot = 0;

    	if(action == MotionEvent.ACTION_DOWN ||
    	   action == MotionEvent.ACTION_MOVE) {
	    // Check if outside control area
	    if(x < ctrlsize && y > height - jumpmovesize) {
		// An area in the middle is no-move
		currentSpeed = 140.0f * (ctrlsize / 2 - x) / ctrlsize;
		float sign = Math.signum(currentSpeed);
		currentSpeed = Math.abs(currentSpeed);
		if(currentSpeed < 15.0f) {
		    currentSpeed = 0;
		} else {
		    currentSpeed =
			sign *
			(float)Math.pow(currentSpeed *
					arrowsSensFactor[(int)prefControlSensitivity-1],
					arrowsSensExp[(int)prefControlSensitivity-1]);
		}
	    }

	    // If in arrow or jump area, shoot.
	    if(x < ctrlsize && y > height - jumpmovesize) {
		currentShoot = 1;
	    }

	    // If in jump area, set jumping
	    if(x < ctrlsize && y < height - movesize &&
	       y > height - jumpmovesize) {
		currentJump = 1;
	    }

	    // If in zap area, zap!
	    if(x < ctrlsize && y < height - jumpmovesize &&
	       y > height - ctrlsize) {
		currentZap = 1;
	    }
    	}

	return true;
    }



    private boolean touchEventMultiArrows(final MotionEvent event) {
	int action = event.getAction() & MotionEvent.ACTION_MASK;
	int pointerID = (event.getAction() &
			 MotionEvent.ACTION_POINTER_ID_MASK) >>
	    MotionEvent.ACTION_POINTER_ID_SHIFT;
	float x, y;

	float width = mRenderer.screenWidth;
	float height = mRenderer.screenHeight;

	float ctrlsize = controlSizes[prefControlSize];
	float movesize = ctrlsize * 0.5f;
	float buttonsize = buttonSizes[prefControlButtonSize];

	switch(action) {
	case MotionEvent.ACTION_DOWN:
	case MotionEvent.ACTION_POINTER_DOWN:
	    x = event.getX(pointerID);
	    y = event.getY(pointerID);

	    if(x < ctrlsize &&
	       y > height - movesize)
		touchEventMultiArrowsControl(ctrlsize, movesize, x, y);
	    else if(x > width - buttonsize * 2 &&
		    y > height - buttonsize * 2)
		touchEventMultiSJZ(buttonsize, x, y);
	    break;

	case MotionEvent.ACTION_MOVE:
	    for(int i = 0 ; i < event.getPointerCount() ; i++) {
		pointerID = event.getPointerId(i);
		x = event.getX(i);
		y = event.getY(i);
		if(x < ctrlsize &&
		   y > height - movesize)
		    touchEventMultiArrowsControl(ctrlsize, movesize, x, y);
		else if(x > width - buttonsize * 2 &&
			y > height - buttonsize * 2)
		    touchEventMultiSJZ(buttonsize, x, y);
	    }
	    break;
	}

	return true;
    }


    public boolean touchEventMultiCircular(MotionEvent event) {
	int action = event.getAction() & MotionEvent.ACTION_MASK;
	int pointerID = (event.getAction() &
			 MotionEvent.ACTION_POINTER_ID_MASK) >>
	    MotionEvent.ACTION_POINTER_ID_SHIFT;
	float x, y;

	float width = mRenderer.screenWidth;
	float height = mRenderer.screenHeight;

	float ctrlsize = controlSizes[prefControlSize];
	float buttonsize = buttonSizes[prefControlButtonSize];

	switch(action) {
	case MotionEvent.ACTION_DOWN:
	case MotionEvent.ACTION_POINTER_DOWN:
	    x = event.getX(pointerID);
	    y = event.getY(pointerID);

	    if(x < ctrlsize + 30 &&
	       y > height - ctrlsize - 30) {
		controlPointerID = pointerID;
		touchEventMultiCircularControl(MotionEvent.ACTION_DOWN,
					       ctrlsize, x, y);
	    } else if(x > width - buttonsize * 2 &&
		      y > height - buttonsize * 2) {
		sjzPointerID = pointerID;
		touchEventMultiSJZ(buttonsize, x, y);
	    }
	    break;

	case MotionEvent.ACTION_POINTER_UP:
	    if(sjzPointerID == pointerID) {
		currentShoot = 0;
		sjzPointerID = -1;
	    } else if(controlPointerID == pointerID) {
		touchEventMultiCircularControl(MotionEvent.ACTION_UP,
					       ctrlsize, 0, 0);
	    }
	    break;

	case MotionEvent.ACTION_UP:
	case MotionEvent.ACTION_CANCEL:
	    touchEventMultiCircularControl(MotionEvent.ACTION_UP,
					   ctrlsize, 0, 0);
	    currentShoot = 0;
	    sjzPointerID = -1;
	    break;

	case MotionEvent.ACTION_MOVE:
	    for(int i = 0 ; i < event.getPointerCount() ; i++) {
		pointerID = event.getPointerId(i);
		x = event.getX(i);
		y = event.getY(i);
		if(x < ctrlsize + 30 &&
		   y > height - ctrlsize - 30) {
		    controlPointerID = pointerID;
		    touchEventMultiCircularControl(MotionEvent.ACTION_MOVE,
						   ctrlsize, x, y);
		} else if(x > width - buttonsize * 2 &&
			  y > height - buttonsize * 2) {
		    sjzPointerID = pointerID;
		    touchEventMultiSJZ(buttonsize, x, y);
		}
	    }
	    break;
	}

	return true;
    }


    public boolean touchEventMultiSwipe(MotionEvent event) {
	int action = event.getAction() & MotionEvent.ACTION_MASK;
	int pointerID = (event.getAction() &
			 MotionEvent.ACTION_POINTER_ID_MASK) >>
	    MotionEvent.ACTION_POINTER_ID_SHIFT;
	float x, y;

	float width = mRenderer.screenWidth;
	float height = mRenderer.screenHeight;

	float ctrlwidth = width * 900.0f / 1196.0f;
	float ctrlheight = height * 200.0f / 720.0f;
	float buttonsize = buttonSizes[prefControlButtonSize];

	switch(action) {
	case MotionEvent.ACTION_DOWN:
	case MotionEvent.ACTION_POINTER_DOWN:
	    x = event.getX(pointerID);
	    y = event.getY(pointerID);

	    if(x < ctrlwidth &&
	       y > height - ctrlheight) {
		controlPointerID = pointerID;
		touchEventSwipeControl(MotionEvent.ACTION_DOWN, x, y);
	    } else if(x > width - buttonsize * 2 &&
		      y > height - buttonsize * 2) {
		sjzPointerID = pointerID;
		touchEventMultiSJZ(buttonsize, x, y);
	    }
	    break;

	case MotionEvent.ACTION_POINTER_UP:
	    if(sjzPointerID == pointerID) {
		currentShoot = 0;
		sjzPointerID = -1;
	    } else if(controlPointerID == pointerID) {
		touchEventSwipeControl(MotionEvent.ACTION_UP, 0, 0);
	    }
	    break;

	case MotionEvent.ACTION_UP:
	case MotionEvent.ACTION_CANCEL:
	    touchEventSwipeControl(MotionEvent.ACTION_UP, 0, 0);
	    currentShoot = 0;
	    sjzPointerID = -1;
	    break;

	case MotionEvent.ACTION_MOVE:
	    for(int i = 0 ; i < event.getPointerCount() ; i++) {
		pointerID = event.getPointerId(i);
		x = event.getX(i);
		y = event.getY(i);
		if(x < ctrlwidth &&
		   y > height - ctrlheight) {
		    controlPointerID = pointerID;
		    touchEventSwipeControl(MotionEvent.ACTION_MOVE, x, y);
		} else if(x > width - buttonsize * 2 &&
			  y > height - buttonsize * 2) {
		    sjzPointerID = pointerID;
		    touchEventMultiSJZ(buttonsize, x, y);
		}
	    }
	    break;
	}

	return true;
    }


    public boolean touchEventDualSwipe(MotionEvent event) {
	int action = event.getAction() & MotionEvent.ACTION_MASK;
	int pointerID = (event.getAction() &
			 MotionEvent.ACTION_POINTER_ID_MASK) >>
	    MotionEvent.ACTION_POINTER_ID_SHIFT;
	float x, y;

	float width = mRenderer.screenWidth;
	float height = mRenderer.screenHeight;

	float ctrlwidth = width * 900.0f / 1196.0f;
	float ctrlheight = height * 200.0f / 720.0f;
	float buttonsize = buttonSizes[prefControlButtonSize];

	switch(action) {
	case MotionEvent.ACTION_DOWN:
	case MotionEvent.ACTION_POINTER_DOWN:
	    x = event.getX(pointerID);
	    y = event.getY(pointerID);

	    if(x < ctrlwidth &&
	       y > height - ctrlheight) {
		controlPointerID = pointerID;
		currentShoot = 1;
		touchEventSwipeControl(MotionEvent.ACTION_DOWN, x, y);
	    } else if(x > width - buttonsize &&
		      y > height - buttonsize) {
		currentZap = 1;
	    }
	    break;

	case MotionEvent.ACTION_POINTER_UP:
	    if(controlPointerID == pointerID) {
		touchEventSwipeControl(MotionEvent.ACTION_UP, 0, 0);
		currentShoot = 0;
	    }
	    break;

	case MotionEvent.ACTION_UP:
	case MotionEvent.ACTION_CANCEL:
	    touchEventSwipeControl(MotionEvent.ACTION_UP, 0, 0);
	    currentShoot = 0;
	    break;

	case MotionEvent.ACTION_MOVE:
	    for(int i = 0 ; i < event.getPointerCount() ; i++) {
		pointerID = event.getPointerId(i);
		x = event.getX(i);
		y = event.getY(i);
		if(x < ctrlwidth &&
		   y > height - ctrlheight) {
		    controlPointerID = pointerID;
		    touchEventSwipeControl(MotionEvent.ACTION_MOVE, x, y);
		} else if(x > width - buttonsize &&
			  y > height - buttonsize) {
		    currentZap = 1;
		}
	    }
	    break;
	}

	return true;
    }



    private void touchEventMultiArrowsControl(float ctrlsize,
					      float movesize,
					      float x, float y) {

	float width = mRenderer.screenWidth;
	float height = mRenderer.screenHeight;

	// Check if outside control area
	if(x > ctrlsize && y < height - movesize)
	    return;

	// An area in the middle is no-move
	currentSpeed = 140.0f * (ctrlsize / 2 - x) / ctrlsize;
	float sign = Math.signum(currentSpeed);
	currentSpeed = Math.abs(currentSpeed);
	if(currentSpeed < 15.0f) {
	    currentSpeed = 0;
	} else {
	    currentSpeed =
		sign *
		(float)Math.pow(currentSpeed *
				arrowsSensFactor[(int)prefControlSensitivity-1],
				arrowsSensExp[(int)prefControlSensitivity-1]);
	}
    }


    private void touchEventMultiCircularControl(int action,
						float ctrlsize,
						float x, float y) {

	float width = mRenderer.screenWidth;
	float height = mRenderer.screenHeight;

	if(action == MotionEvent.ACTION_DOWN) {
	    boolean isOut = calcCircCoords(ctrlsize,
					   ctrlsize / 2, height - ctrlsize / 2,
					   x, y);
	    if(!isOut) {
		controlPrevX = controlCurrX;
		controlPrevY = controlCurrY;
		controlPrevAngle = controlCurrAngle;
	    } else {
		controlPrevX = 0;
		controlPrevY = 0;
		controlPrevAngle = 0;
	    }
	    return;
	} else if(action == MotionEvent.ACTION_UP) {
	    controlPrevX = 0;
	    controlPrevY = 0;
	    controlPrevAngle = 0;
	    return;
        }

	// action is always MOVE here

	boolean isOut = calcCircCoords(ctrlsize,
				       ctrlsize / 2, height - ctrlsize / 2,
				       x, y);

	if(!isOut && (controlPrevX == 0 && controlPrevY == 0)) {
	    // Simulated ACTION_DOWN
	    controlPrevX = controlCurrX;
	    controlPrevY = controlCurrY;
	    controlPrevAngle = controlCurrAngle;
	    return;
	}

	if(isOut) {
	    // Simulated ACTION_UP
	    controlPrevX = 0;
	    controlPrevY = 0;
	    controlPrevAngle = 0;
	    return;
	}

	double angleDiff = controlCurrAngle - controlPrevAngle;
	if(angleDiff < -180.0)
	    angleDiff += 360.0;
	else if(angleDiff > 180.0)
	    angleDiff -= 360.0;

	double factor = (10 - prefControlSensitivity) * 2.5 + 10;
	double step = angleDiff / factor;
	if(Math.abs(step) > 1.0) {
	    currentStep = Math.round((float)step);
	    controlPrevX = controlCurrX;
	    controlPrevY = controlCurrY;
	    controlPrevAngle = controlCurrAngle;
	} else {
	    currentStep = 0;
	}
    }


    private void touchEventSwipeControl(int action,
					float x, float y) {

	float width = mRenderer.screenWidth;
	float height = mRenderer.screenHeight;

	if(action == MotionEvent.ACTION_DOWN) {
	    controlPrevX = x;
	    controlPrevY = y;
	    return;
	} else if(action == MotionEvent.ACTION_UP) {
	    controlPrevX = 0;
	    controlPrevY = 0;
	    return;
        }

	// action is always MOVE here

	double sensfactor = (10 - prefControlSensitivity) * 2.5 + 17.5;
	double factor = width * (sensfactor / 1196.0);
	double xstep = (x - controlPrevX) / factor;
	if(Math.abs(xstep) > 1.0) {
	    currentStep = -Math.round((float)xstep);
	    controlPrevX = x;
	} else {
	    currentStep = 0;
	}

	if(prefControlType == C2Renderer.CONTROL_TYPE_DUAL_SWIPE) {
	    double ystep = (y - controlPrevY) / factor;
	    if(ystep > 0.0) {
		controlPrevY = y;
	    } else if(ystep < -4.0) {
		currentJump = 1;
	    }
	}
    }


    private void touchEventMultiSJZ(float buttonsize,
				    float x, float y) {
	float width = mRenderer.screenWidth;
	float height = mRenderer.screenHeight;
	float jumpy1 = prefControlAutofire ? buttonsize : buttonsize * 2;
	float jumpy2 = prefControlAutofire ? 0 : buttonsize;

	currentShoot = 0;

	if(x > width - buttonsize &&
	   y > height - buttonsize &&
	   !prefControlAutofire) {
	    currentShoot = 1;
	} else if(x > width - buttonsize &&
		  y > height - jumpy1 &&
		  y < height - jumpy2) {
	    currentJump = 1;
	} else if(x > width - buttonsize * 2 &&
		  x < width - buttonsize &&
		  y > height - buttonsize) {
	    currentZap = 1;
	}
    }

    private void touchEventPause() {
	if(game_paused) {
	    for(int i = 1 ; i < SOUND_NUM ; i++)
		soundPool.resume(streams[i]);
	    startMusic();
	    game_paused = false;
	    sendEvent(C2Renderer.EVENT_GAME_RESUME, 0, 0, 0);
	} else {
	    for(int i = 1 ; i < SOUND_NUM ; i++)
		soundPool.pause(streams[i]);
	    pauseMusic();
	    game_paused = true;
	    sendEvent(C2Renderer.EVENT_GAME_PAUSE, 0, 0, 0);
	}
    }

    private void touchEventExit() {
	game_paused = true;
	view = C2Renderer.VIEW_MENU;
	sendEvent(C2Renderer.EVENT_GO_VIEW, view, 0, 0);
	pauseMusic();
    }

    private boolean calcCircCoords(float ctrlsize, float cx, float cy,
				   float x, float y) {
	float min = ctrlsize / 2 - ctrlsize * 0.25f;
	float max = ctrlsize / 2 + 30;
	double relX = x - cx;
	double relY = y - cy;
	double hyp = Math.sqrt(relX * relX + relY * relY);

	if(hyp < min || hyp > max)
		return true;

	// Normalized -1.0 -- 1.0
	double ax = relX / hyp;
	double ay = relY / hyp;

	// Calculate angle
	controlCurrAngle = (Math.atan2(ay, ax) + Math.PI) * 180.0 / Math.PI;

	// Calculate coordinates for show
	double factor = ctrlsize / hyp;

	controlCurrX = (float)(relX * factor + cx);
	controlCurrY = (float)(relY * factor + cy);

	return false;
    }


    private boolean touchEventGesture(final MotionEvent event) {
	hsListDir = 0;
	hsListStep = 1;
	hsEnterDir = 0;
	hsEnterStep = 1;
	menuHandler.removeCallbacks(hsListUpdateTask);
	menuHandler.removeCallbacks(hsEnterUpdateTask);

	return gestureScanner.onTouchEvent(event);
    }

    // Gesture detection

    @Override
    public boolean onDown(MotionEvent e) {
	//        Log.d("Cyclone2000", "Gesture: DOWN");
	sendEvent(C2Renderer.EVENT_MENU_CHOICE, C2Renderer.MENU_GESTURE_DOWN,
		  (int)e.getX(), (int)e.getY());
        return true;
    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2,
			   float velocityX, float velocityY) {
	//        Log.d("Cyclone2000", "Gesture: FLING, vx=" + velocityX +
	//	      ", vy=" + velocityY);
	sendEvent(C2Renderer.EVENT_MENU_CHOICE, C2Renderer.MENU_GESTURE_FLING,
		  (int)(velocityX * 1000), (int)(velocityY * 1000));
        return true;
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2,
			    float distanceX, float distanceY) {
	//        Log.d("Cyclone2000", "Gesture: SCROLL, dx=" + distanceX +
	//	      ", dy=" + distanceY);
	sendEvent(C2Renderer.EVENT_MENU_CHOICE, C2Renderer.MENU_GESTURE_SCROLL,
		  (int)(distanceX * 1000), (int)(distanceY * 1000));
        return true;
    }

    @Override
    public void onShowPress(MotionEvent e) {
	//        Log.d("Cyclone2000", "Gesture: SHOW PRESS");
    }

    @Override
    public void onLongPress(MotionEvent e) {
	//        Log.d("Cyclone2000", "Gesture: LONG PRESS");
    }

    @Override
    public boolean onSingleTapUp(MotionEvent e) {
	//        Log.d("Cyclone2000", "Gesture: SINGLE TAP UP, x=" + e.getX() +
	//	      ", y=" + e.getY());
	sendEvent(C2Renderer.EVENT_MENU_CHOICE, C2Renderer.MENU_GESTURE_TAP,
		  (int)e.getX(), (int)e.getY());
        return true;
    }



    // Draw single touch jump for arrows control
    private void drawArrowsJump(Canvas canvas, float size) {
	float factor = size / 256.0f;

        canvas.drawColor(Color.TRANSPARENT);

	Paint paint = new Paint();

	paint.setAntiAlias(true);
        paint.setARGB(255, 170, 170, 0);
        paint.setStyle(Style.STROKE);
	if(factor * 2.0f <= 1.0f)
	    paint.setStrokeWidth(factor * 3.0f);
	else
	    paint.setStrokeWidth(factor * 2.0f);

        Path path = new Path();

        // Jump arrow
        path.addRect(factor * 120, factor * 169,
		     factor * 136, factor * 176, Path.Direction.CW);
        path.addRect(factor * 119, factor * 150,
		     factor * 137, factor * 164, Path.Direction.CW);
        path.moveTo(factor * 118, factor * 145);
        path.lineTo(factor * 118, factor * 105);
        path.lineTo(factor * 108, factor * 105);
        path.lineTo(factor * 128, factor * 85);
        path.lineTo(factor * 148, factor * 105);
        path.lineTo(factor * 138, factor * 105);
        path.lineTo(factor * 138, factor * 145);
        path.close();

        canvas.drawPath(path, paint);
    }

    // Draw single touch zap for arrows control
    private void drawArrowsZap(Canvas canvas, float size) {
	float factor = size / 512.0f;

        canvas.drawColor(Color.TRANSPARENT);

	Paint paint = new Paint();

	paint.setAntiAlias(true);
        paint.setARGB(255, 170, 170, 0);
        paint.setStyle(Style.STROKE);
	if(factor * 4.0f <= 1.0f)
	    paint.setStrokeWidth(factor * 6.0f);
	else
	    paint.setStrokeWidth(factor * 4.0f);

        Path path = new Path();

	// Zap
        path.moveTo(factor * 270, factor * 4);
        path.lineTo(factor * 308, factor * 23);
        path.lineTo(factor * 265, factor * 108);
        path.lineTo(factor * 254, factor * 83);
        path.lineTo(factor * 218, factor * 158);
        path.lineTo(factor * 241, factor * 41);
        path.lineTo(factor * 260, factor * 66);
        path.close();

        canvas.drawPath(path, paint);
    }


    // Draw single touch arrows control
    private void drawFullArrows(Canvas canvas, float size) {
	float factor = size / 256.0f;

        canvas.drawColor(Color.TRANSPARENT);

	Paint paint = new Paint();

	paint.setAntiAlias(true);
        paint.setARGB(255, 170, 170, 0);
        paint.setStyle(Style.STROKE);
	if(factor * 2.0f <= 1.0f)
	    paint.setStrokeWidth(factor * 3.0f);
	else
	    paint.setStrokeWidth(factor * 2.0f);

        Path path = new Path();

        // Right rects and arrow
        path.addRect(factor * 142, factor * 188,
		     factor * 151, factor * 230, Path.Direction.CW);
        path.addRect(factor * 157, factor * 186,
		     factor * 173, factor * 232, Path.Direction.CW);
        path.moveTo(factor * 179, factor * 184);
        path.lineTo(factor * 212, factor * 184);
        path.lineTo(factor * 212, factor * 164);
        path.lineTo(factor * 255, factor * 209);
        path.lineTo(factor * 212, factor * 254);
        path.lineTo(factor * 212, factor * 234);
        path.lineTo(factor * 179, factor * 234);
        path.close();

        // Left rects and arrow
        path.addRect(size - factor * 142, factor * 188,
		     size - factor * 151, factor * 230, Path.Direction.CW);
        path.addRect(size - factor * 157, factor * 186,
		     size - factor * 173, factor * 232, Path.Direction.CW);
        path.moveTo(size - factor * 179, factor * 184);
        path.lineTo(size - factor * 212, factor * 184);
        path.lineTo(size - factor * 212, factor * 164);
        path.lineTo(size - factor * 255, factor * 209);
        path.lineTo(size - factor * 212, factor * 254);
        path.lineTo(size - factor * 212, factor * 234);
        path.lineTo(size - factor * 179, factor * 234);
        path.close();

        canvas.drawPath(path, paint);
    }


    // Draw circular control
    private void drawCross(Canvas canvas, float size) {
	float factor = size / 512.0f;
	float radius = size / 2.0f;

        canvas.drawColor(Color.TRANSPARENT);

	Paint paint = new Paint();

        paint.setAntiAlias(true);
        paint.setARGB(255, 170, 170, 0);
        paint.setStyle(Style.STROKE);
	if(factor * 4.0f <= 1.0f)
	    paint.setStrokeWidth(factor * 6.0f);
	else
	    paint.setStrokeWidth(factor * 4.0f);

	canvas.drawCircle(radius, radius, radius - factor * 40, paint);
	canvas.drawCircle(radius, radius, radius - factor * 60, paint);
	canvas.drawLine(0, radius, size, radius, paint);
	canvas.drawLine(radius, 0, radius, size, paint);
    }


    // Draw swipe control
    private void drawSwipe(Canvas canvas, float size) {
	float factor = size / 512.0f;
	float width = size * 2.0f;
	float height = size / 2.0f;

	canvas.drawColor(Color.TRANSPARENT);

	Paint paint = new Paint();

	paint.setAntiAlias(true);
	paint.setARGB(255, 170, 170, 0);
	paint.setStyle(Style.FILL_AND_STROKE);
	paint.setTextSize(factor * 48.0f);

	paint.setStrokeWidth(factor * 4.0f);
	canvas.drawLine(factor * 10, height - factor * 10,
			factor * 10, factor * 70, paint);
	paint.setStrokeWidth(factor * 3.0f);
	canvas.drawLine(factor * 10, factor * 50,
			factor * 10, factor * 30, paint);
	paint.setStrokeWidth(factor * 2.0f);
	canvas.drawLine(factor * 10, factor * 10,
			factor * 10, factor * 0, paint);

	paint.setStrokeWidth(factor * 4.0f);
	canvas.drawLine(factor * 10, height - factor * 10,
			width - factor * 70, height - factor * 10, paint);
	paint.setStrokeWidth(factor * 3.0f);
	canvas.drawLine(width - factor * 50, height - factor * 10,
			width - factor * 30, height - factor * 10, paint);
	paint.setStrokeWidth(factor * 2.0f);
	canvas.drawLine(width - factor * 10, height - factor * 10,
			width - factor * 0, height - factor * 10, paint);

	paint.setStrokeWidth(factor * 5.0f);
	canvas.drawLine(factor * 40,  height - factor * 64,
			factor * 140, height - factor * 64, paint);
	canvas.drawLine(factor * 40,  height - factor * 64,
			factor * 64,  height - factor * 80, paint);
	canvas.drawLine(factor * 40,  height - factor * 64,
			factor * 64,  height - factor * 48, paint);
	canvas.drawLine(factor * 140, height - factor * 64,
			factor * 116, height - factor * 80, paint);
	canvas.drawLine(factor * 140, height - factor * 64,
			factor * 116, height - factor * 48, paint);

	paint.setStrokeWidth(factor * 2.0f);
	canvas.drawText("Move", factor * 164, height - factor * 50, paint);
    }


    // Draw jump in swipe control
    private void drawSwipeJump(Canvas canvas, float size) {
	float factor = size / 256.0f;

	canvas.drawColor(Color.TRANSPARENT);

	Paint paint = new Paint();

	paint.setAntiAlias(true);
	paint.setARGB(255, 170, 170, 0);
	paint.setStyle(Style.FILL_AND_STROKE);
	paint.setTextSize(factor * 48.0f);

	paint.setStrokeWidth(factor * 5.0f);
	canvas.drawLine(factor * 16, size - factor * 32,
			factor * 16, size - factor * 112, paint);
	canvas.drawLine(factor * 16, size - factor * 112,
			factor * 0,  size - factor * 88, paint);
	canvas.drawLine(factor * 16, size - factor * 112,
			factor * 32, size - factor * 88, paint);

	paint.setStrokeWidth(factor * 2.0f);
	canvas.drawText("Jump", factor * 46, size - factor * 50, paint);
    }



    // Draw pause button
    private void drawPause(Canvas canvas, float size) {
	float factor = size / 256.0f;

        canvas.drawColor(Color.TRANSPARENT);

	Paint paint = new Paint();

	paint.setAntiAlias(true);
	paint.setARGB(255, 170, 170, 0);
	paint.setStyle(Style.STROKE);
	paint.setStrokeWidth(factor * 6.0f);

	Path path = new Path();
	path.addCircle(factor * 128, factor * 128,
		       factor * 112, Path.Direction.CW);
	path.addRect(factor * 76, factor * 72,
		     factor * 116, factor * 184, Path.Direction.CW);
	path.addRect(factor * 140, factor * 72,
		     factor * 180, factor * 184, Path.Direction.CW);

	canvas.drawPath(path, paint);
    }


    // Draw exit button
    private void drawExit(Canvas canvas, float size) {
	float factor = size / 256.0f;

        canvas.drawColor(Color.TRANSPARENT);

	Paint paint = new Paint();

	paint.setAntiAlias(true);
	paint.setARGB(255, 170, 170, 0);
	paint.setStyle(Style.STROKE);
	paint.setStrokeWidth(factor * 6.0f);

	Path path = new Path();
	path.addCircle(factor * 128, factor * 128,
		       factor * 112, Path.Direction.CW);

        path.moveTo(factor * 116, factor * 109);
        path.lineTo(factor * 116, factor * 72);
        path.lineTo(factor * 180, factor * 72);
        path.lineTo(factor * 180, factor * 184);
        path.lineTo(factor * 116, factor * 184);
        path.lineTo(factor * 116, factor * 147);

        path.moveTo(factor * 148, factor * 128);
        path.lineTo(factor * 64, factor * 128);
        path.lineTo(factor * 88, factor * 104);
        path.moveTo(factor * 64, factor * 128);
        path.lineTo(factor * 88, factor * 152);

	canvas.drawPath(path, paint);
    }

    // Draw standalone shoot button
    private void drawShoot(Canvas canvas, float size) {
	float factor = size / 256.0f;

        canvas.drawColor(Color.TRANSPARENT);

	Paint paint = new Paint();

	paint.setAntiAlias(true);
	paint.setARGB(255, 170, 170, 0);
	paint.setStyle(Style.STROKE);
	paint.setStrokeWidth(factor * 6.0f);

	Path path = new Path();
	path.addCircle(factor * 128, factor * 128,
		       factor * 112, Path.Direction.CW);

	// Player
        path.moveTo(factor * 121, factor * 104);
        path.lineTo(factor * 64,  factor * 118);
        path.lineTo(factor * 76,  factor * 184);
        path.lineTo(factor * 143, factor * 183);
        path.lineTo(factor * 151, factor * 135);
        path.lineTo(factor * 130, factor * 165);
        path.lineTo(factor * 96,  factor * 160);
        path.lineTo(factor * 85,  factor * 133);
        path.close();

	// Shots
	path.addCircle(factor * 123, factor * 131,
		       factor * 10, Path.Direction.CW);
	path.addCircle(factor * 143, factor * 113,
		       factor * 9, Path.Direction.CW);
	path.addCircle(factor * 161, factor * 93,
		       factor * 8, Path.Direction.CW);

	canvas.drawPath(path, paint);
    }


    // Draw standalone jump button
    private void drawJump(Canvas canvas, float size) {
	float factor = size / 256.0f;

        canvas.drawColor(Color.TRANSPARENT);

	Paint paint = new Paint();

	paint.setAntiAlias(true);
	paint.setARGB(255, 170, 170, 0);
	paint.setStyle(Style.STROKE);
	paint.setStrokeWidth(factor * 6.0f);

	Path path = new Path();
	path.addCircle(factor * 128, factor * 128,
		       factor * 112, Path.Direction.CW);

        // Jump arrow
        path.addRect(factor * 116f, factor * 189.5f,
		     factor * 140f, factor * 200f, Path.Direction.CW);
        path.addRect(factor * 114.5f, factor * 161f,
		     factor * 141.5f, factor * 182f, Path.Direction.CW);
        path.moveTo(factor * 113f, factor * 153.5f);
        path.lineTo(factor * 113f, factor * 93.5f);
        path.lineTo(factor * 98f,  factor * 93.5f);
        path.lineTo(factor * 128f, factor * 63.5f);
        path.lineTo(factor * 158f, factor * 93.5f);
        path.lineTo(factor * 143f, factor * 93.5f);
        path.lineTo(factor * 143f, factor * 153.5f);
        path.close();

        canvas.drawPath(path, paint);
    }


    // Draw standalone zap button
    private void drawZap(Canvas canvas, float size) {
	float factor = size / 256.0f;

        canvas.drawColor(Color.TRANSPARENT);

	Paint paint = new Paint();

	paint.setAntiAlias(true);
	paint.setARGB(255, 170, 170, 0);
	paint.setStyle(Style.STROKE);
	paint.setStrokeWidth(factor * 6.0f);

	Path path = new Path();
	path.addCircle(factor * 128, factor * 128,
		       factor * 112, Path.Direction.CW);

        path.moveTo(factor * 142, factor * 46);
        path.lineTo(factor * 180, factor * 65);
        path.lineTo(factor * 137, factor * 150);
        path.lineTo(factor * 126, factor * 125);
        path.lineTo(factor * 90,  factor * 200);
        path.lineTo(factor * 113, factor * 83);
        path.lineTo(factor * 132, factor * 108);
        path.close();

        canvas.drawPath(path, paint);
    }



    // Draw numbers, 128x256
    private void drawNumber(Canvas canvas, int number) {
	float path_clip_no[] = {};

	float path_clip_0[] = {
	    1, 40, 52,
	    0, 44, 56,
	    0, 60, 56,
	    0, 64, 52,
	    0, 64, 196,
	    0, 60, 200,
	    0, 44, 200,
	    0, 40, 196,
	};

	float path_draw_0[] = {
	    1, 8, 48,
	    0, 24, 16,
	    0, 80, 16,
	    0, 96, 48,
	    0, 96, 208,
	    0, 80, 240,
	    0, 24, 240,
	    0, 8, 208,
	};



	float path_draw_1[] = {
	    1, 16, 64,
	    0, 36, 16,
	    0, 68, 16,
	    0, 68, 196,
	    0, 88, 196,
	    0, 88, 240,
	    0, 16, 240,
	    0, 16, 196,
	    0, 36, 196,
	    0, 36, 64,
	};



	float path_draw_2[] = {
	    1, 8, 16,
	    0, 80, 16,
	    0, 96, 48,
	    0, 96, 96,
	    0, 52, 196,
	    0, 96, 196,
	    0, 96, 240,
	    0, 8, 240,
	    0, 8, 196,
	    0, 68, 60,
	    0, 8, 60,
	};



	float path_draw_3[] = {
	    1, 8, 48,
	    0, 24, 16,
	    0, 80, 16,
	    0, 96, 48,
	    0, 96, 208,
	    0, 80, 240,
	    0, 24, 240,
	    0, 8, 208,
	    0, 8, 164,
	    0, 32, 164,
	    0, 34, 196,
	    0, 70, 196,
	    0, 70, 144,
	    0, 40, 144,
	    0, 40, 112,
	    0, 70, 112,
	    0, 70, 60,
	    0, 34, 60,
	    0, 32, 92,
	    0, 8, 92,
	};


	float path_clip_4[] = {
	    1, 34, 112,
	    0, 54, 48,
	    0, 54, 112,
	};

	float path_draw_4[] = {
	    1, 8, 112,
	    0, 50, 16,
	    0, 80, 16,
	    0, 80, 128,
	    0, 96, 128,
	    0, 96, 176,
	    0, 80, 176,
	    0, 80, 240,
	    0, 54, 240,
	    0, 54, 176,
	    0, 8, 176,
	};


	float path_draw_5[] = {
	    1, 8, 16,
	    0, 96, 16,
	    0, 96, 60,
	    0, 36, 60,
	    0, 36, 96,
	    0, 80, 96,
	    0, 96, 128,
	    0, 96, 208,
	    0, 80, 240,
	    0, 24, 240,
	    0, 8, 208,
	    0, 8, 164,
	    0, 32, 164,
	    0, 34, 196,
	    0, 66, 196,
	    0, 68, 180,
	    0, 68, 156,
	    0, 66, 140,
	    0, 8, 140,
	};


	float path_clip_6[] = {
	    1, 36, 172,
	    0, 38, 150,
	    0, 66, 150,
	    0, 68, 172,
	    0, 68, 180,
	    0, 66, 196,
	    0, 38, 196,
	    0, 36, 180,
	};

	float path_draw_6[] = {
	    1, 8, 48,
	    0, 24, 16,
	    0, 80, 16,
	    0, 96, 48,
	    0, 96, 76,
	    0, 68, 76,
	    0, 66, 60,
	    0, 38, 60,
	    0, 36, 76,
	    0, 36, 80,
	    0, 38, 96,
	    0, 80, 96,
	    0, 96, 128,
	    0, 96, 208,
	    0, 80, 240,
	    0, 24, 240,
	    0, 8, 208,
	};



	float path_draw_7[] = {
	    1, 8, 16,
	    0, 96, 16,
	    0, 96, 96,
	    0, 74, 144,
	    0, 74, 240,
	    0, 48, 240,
	    0, 48, 112,
	    0, 72, 60,
	    0, 34, 60,
	    0, 32, 76,
	    0, 8, 76,
	};



	float path_clip_8[] = {
	    1, 36, 84,
	    0, 38, 100,
	    0, 66, 100,
	    0, 68, 84,
	    0, 68, 76,
	    0, 66, 60,
	    0, 38, 60,
	    0, 36, 76,

	    1, 36, 172,
	    0, 38, 150,
	    0, 66, 150,
	    0, 68, 172,
	    0, 68, 180,
	    0, 66, 196,
	    0, 38, 196,
	    0, 36, 180,
	};

	float path_draw_8[] = {
	    1, 8, 48,
	    0, 24, 16,
	    0, 80, 16,
	    0, 96, 48,
	    0, 96, 104,
	    0, 80, 128,
	    0, 96, 152,
	    0, 96, 208,
	    0, 80, 240,
	    0, 24, 240,
	    0, 8, 208,
	    0, 8, 152,
	    0, 24, 128,
	    0, 8, 104,
	};



	float path_clip_9[] = {
	    1, 68, 84,
	    0, 66, 106,
	    0, 38, 106,
	    0, 36, 84,
	    0, 36, 76,
	    0, 38, 60,
	    0, 66, 60,
	    0, 68, 76,
	};

	float path_draw_9[] = {
	    1, 96, 208,
	    0, 80, 240,
	    0, 24, 240,
	    0, 8, 208,
	    0, 8, 180,
	    0, 36, 180,
	    0, 38, 196,
	    0, 66, 196,
	    0, 68, 180,
	    0, 68, 176,
	    0, 66, 160,
	    0, 24, 160,
	    0, 8, 128,
	    0, 8, 48,
	    0, 24, 16,
	    0, 80, 16,
	    0, 96, 48,
	};



	float[][] path_clips = {
	    path_clip_0,
	    path_clip_no,
	    path_clip_no,
	    path_clip_no,
	    path_clip_4,
	    path_clip_no,
	    path_clip_6,
	    path_clip_no,
	    path_clip_8,
	    path_clip_9,
	};

	float[][] path_draws = {
	    path_draw_0,
	    path_draw_1,
	    path_draw_2,
	    path_draw_3,
	    path_draw_4,
	    path_draw_5,
	    path_draw_6,
	    path_draw_7,
	    path_draw_8,
	    path_draw_9,
	};

        canvas.drawColor(Color.TRANSPARENT);
	canvas.save();

	Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG |
				Paint.FILTER_BITMAP_FLAG);
	Path path = new Path();

	if(path_clips[number].length > 0) {
	    path.rewind();
	    for(int i = 0 ; i < path_clips[number].length ; i += 3) {
		if(path_clips[number][i] == 1)
		    path.moveTo(path_clips[number][i + 1] + 12,
				path_clips[number][i + 2] + 12);
		else
		    path.lineTo(path_clips[number][i + 1] + 12,
				path_clips[number][i + 2] + 12);
	    }
	    path.close();

	    canvas.clipPath(path, Region.Op.DIFFERENCE);
	}

	path.rewind();
	for(int i = 0 ; i < path_draws[number].length ; i += 3) {
	    if(path_draws[number][i] == 1)
		path.moveTo(path_draws[number][i + 1] + 12,
			    path_draws[number][i + 2] + 12);
	    else
		path.lineTo(path_draws[number][i + 1] + 12,
			    path_draws[number][i + 2] + 12);
	}
	path.close();

	paint.setStyle(Paint.Style.FILL);
	paint.setColor(0xff606060);
	paint.setShader(new LinearGradient(0, 128, 32, 256,
					   0xff606060, 0xffa07070,
					   Shader.TileMode.CLAMP));

	canvas.drawPath(path, paint);


	canvas.restore();
	canvas.save();

	if(path_clips[number].length > 0) {
	    path.rewind();
	    for(int i = 0 ; i < path_clips[number].length ; i += 3) {
		if(path_clips[number][i] == 1)
		    path.moveTo(path_clips[number][i + 1],
				path_clips[number][i + 2]);
		else
		    path.lineTo(path_clips[number][i + 1],
				path_clips[number][i + 2]);
	    }
	    path.close();

	    canvas.clipPath(path, Region.Op.DIFFERENCE);
	}

	path.rewind();
	for(int i = 0 ; i < path_draws[number].length ; i += 3) {
	    if(path_draws[number][i] == 1)
		path.moveTo(path_draws[number][i + 1],
			    path_draws[number][i + 2]);
	    else
		path.lineTo(path_draws[number][i + 1],
			    path_draws[number][i + 2]);
	}
	path.close();

	paint.setStyle(Paint.Style.FILL_AND_STROKE);
	paint.setColor(0xff808080);
        paint.setStrokeWidth(5.0f);
	paint.setShader(new LinearGradient(0, 0, 32, 256,
					   0xffffff00, 0xff00ffff,
					   Shader.TileMode.CLAMP));
	canvas.drawPath(path, paint);

	canvas.restore();
    }



    public void onStart(Activity act) {
	nfcSensor.addOnCardboardNfcListener(this);
    }

    public void onStop() {
	stopMusic();
	nfcSensor.removeOnCardboardNfcListener(this);
    }

    private byte[] localToByteArray() {
	ByteBuffer bb = ByteBuffer.allocate(4 * (2 + C2Renderer.PUPS_ITEM_NUM)).order(ByteOrder.BIG_ENDIAN);
	int index = 0;
	bb.putInt(index, prefGameMaxStage);
	index += 4;
	bb.putInt(index, prefTotalPowerups);
	index += 4;
	for(int i = 0 ; i < C2Renderer.PUPS_ITEM_NUM ; i++) {
	    bb.putInt(index, prefPupsItems[i]);
	    index += 4;
	}
	return bb.array();
    }

    private String localToDescription() {
	return "Maximum stage: " + prefGameMaxStage +
	    ", Powerups: " + prefTotalPowerups;
    }

    private void byteArrayToLocal(byte [] byteArray){
	ByteBuffer bb = ByteBuffer.wrap(byteArray).order(ByteOrder.BIG_ENDIAN);
	int value;
	int index = 0;
	int limit = bb.limit();
	SharedPreferences prefs =
	    PreferenceManager.getDefaultSharedPreferences(activityContext);
	SharedPreferences.Editor editor = prefs.edit();

	try {
	    value = bb.getInt(index);
	    if(value > prefGameMaxStage) {
		prefGameMaxStage = value;
		editor.putString("c_game_max_stage",
				 prefIntEncrypt("gms", prefGameMaxStage));
	    }
	    index += 4;

	    value = bb.getInt(index);
	    if(value > prefTotalPowerups) {
		prefTotalPowerups = value;
		editor.putString("c_game_total_powerups",
				 prefIntEncrypt("gtp", prefTotalPowerups));
		sendEvent(C2Renderer.EVENT_PUPS_ADD, 0, prefTotalPowerups, 0);
	    }
	    index += 4;
	} catch(IndexOutOfBoundsException e) {
	    editor.commit();
	    return;
	}

	editor.commit();

	try {
	    for(int i = 0 ; i < C2Renderer.PUPS_ITEM_NUM ; i++) {
		value = bb.getInt(index);
		if(value > prefPupsItems[i])
		    storePrefPupValue(i, value);
		index += 4;
	    }
	} catch(IndexOutOfBoundsException e) {
	    return;
	}
    }

    /*
    final protected static char[] hexArray = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    public static String bytesToHex(byte[] bytes) {
	char[] hexChars = new char[bytes.length * 2];
	int v;
	for ( int j = 0; j < bytes.length; j++ ) {
	    v = bytes[j] & 0xFF;
	    hexChars[j * 2] = hexArray[v >>> 4];
	    hexChars[j * 2 + 1] = hexArray[v & 0x0F];
	}
	return new String(hexChars);
    }
    */

    // Enable VR mode when phone is inserted into Cardboard
    @Override 
    public void onInsertedIntoCardboard(CardboardDeviceParams params) {
	if(!useNfc)
	    return;

	Log.d("Cyclone2000", "onInsertedIntoCardboard");
	sendEvent(C2Renderer.EVENT_DISPLAY_MODE, C2Renderer.DISPLAY_MODE_VR,
		  0, 0);
    }

    // Disable VR mode when phone is removed from Cardboard
    @Override 
    public void onRemovedFromCardboard() {
	if(!useNfc)
	    return;

	Log.d("Cyclone2000", "onRemovedFromCardboard");
	sendEvent(C2Renderer.EVENT_DISPLAY_MODE, C2Renderer.DISPLAY_MODE_FULL,
		  0, 0);
    }

}
