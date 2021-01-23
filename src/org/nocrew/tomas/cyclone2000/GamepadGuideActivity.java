package org.nocrew.tomas.cyclone2000;

import android.app.Activity;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.content.pm.ActivityInfo;
import android.content.Intent;
import android.view.View;
import android.view.Display;
import android.view.Window;
import android.view.WindowManager;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.os.Bundle;
import android.os.Build;
import android.view.ViewGroup;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.preference.PreferenceManager;
import android.content.SharedPreferences;

import org.nocrew.tomas.cyclone2000.R;
import org.nocrew.tomas.cyclone2000.C2GLSurfaceView;

public class GamepadGuideActivity extends Activity {

    public int screenWidth;
    public int screenHeight;

    public boolean showBottomText = true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

	getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

	showBottomText = getIntent().getExtras().getBoolean("showBottomText");

        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
			     WindowManager.LayoutParams.FLAG_FULLSCREEN);

	Display d =
	    ((WindowManager)getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
	screenWidth = d.getWidth();
	screenHeight = d.getHeight();
	ViewGroup.LayoutParams lp =
	    new ViewGroup.LayoutParams(screenWidth, screenHeight);

	setContentView(new GamepadGuideView(this), lp);
    }

    public boolean onKeyDown(int keyCode, KeyEvent event) {
	finish();
	return true;
    }

    public boolean onTouchEvent(final MotionEvent event) {
	int action = event.getAction() & MotionEvent.ACTION_MASK;

	if(action == MotionEvent.ACTION_DOWN) {
	    finish();
	}

	return true;
    }

    public class GamepadGuideView extends View {

	private static final int GP_CO_BUTTON_A    = 0;
	private static final int GP_CO_BUTTON_B    = 1;
	private static final int GP_CO_BUTTON_X    = 2;
	private static final int GP_CO_BUTTON_Y    = 3;
	private static final int GP_CO_BUTTON_L1   = 4;
	private static final int GP_CO_BUTTON_R1   = 5;
	private static final int GP_CO_DPAD_UP     = 6;
	private static final int GP_CO_DPAD_DOWN   = 7;
	private static final int GP_CO_DPAD_LEFT   = 8;
	private static final int GP_CO_DPAD_RIGHT  = 9;
	private static final int GP_CO_START       = 10;
	private static final int GP_CO_BACK        = 11;
	private static final int GP_CO_AXIS_L      = 12;
	private static final int GP_CO_NUM         = 13;

	private final int gp_keycodes[] = {
	    KeyEvent.KEYCODE_BUTTON_A,
	    KeyEvent.KEYCODE_BUTTON_B,
	    KeyEvent.KEYCODE_BUTTON_X,
	    KeyEvent.KEYCODE_BUTTON_Y,
	    KeyEvent.KEYCODE_BUTTON_L1,
	    KeyEvent.KEYCODE_BUTTON_R1,
	    KeyEvent.KEYCODE_DPAD_UP,
	    KeyEvent.KEYCODE_DPAD_DOWN,
	    KeyEvent.KEYCODE_DPAD_LEFT,
	    KeyEvent.KEYCODE_DPAD_RIGHT,
	    KeyEvent.KEYCODE_BUTTON_START,
	    0,
	    0,
	};

	// X = offsetX + imgScale * ($0 * bitmapWidth + $1),
	// Y = offsetY + imgScale * ($2 * bitmapHeight + $3f),
	// $4 == 0 for left align, != 0 for right align.
	private final float gp_coords[] = {
	    1.0f,  10.0f, 0.470f, 0.0f, 0f,   // BUTTON_A
	    1.0f,  10.0f, 0.400f, 0.0f, 0f,   // BUTTON_B
	    1.0f,  10.0f, 0.334f, 0.0f, 0f,   // BUTTON_X
	    1.0f,  10.0f, 0.254f, 0.0f, 0f,   // BUTTON_Y
	    0.0f, -10.0f, 0.138f, 0.0f, 1f,   // BUTTON_L1
	    1.0f,  10.0f, 0.138f, 0.0f, 0f,   // BUTTON_R1
	    0.0f, -10.0f, 0.254f, 0.0f, 1f,   // DPAD_UP
	    0.0f, -10.0f, 0.470f, 0.0f, 1f,   // DPAD_DOWN
	    0.0f, -10.0f, 0.400f, 0.0f, 1f,   // DPAD_LEFT
	    0.0f, -10.0f, 0.334f, 0.0f, 1f,   // DPAD_RIGHT
	    1.0f,  10.0f, 0.660f, 0.0f, 0f,   // START
	    0.0f, -10.0f, 0.660f, 0.0f, 1f,   // BACK
	    0.0f, -10.0f, 0.815f, 0.0f, 1f,   // AXIS_L
	};

	private float bitmapWidth;
	private float bitmapHeight;
	private float offsetX;
	private float offsetY;
	private float imgScale;
	private float factor;

	public GamepadGuideView(Context context) {
	    super(context);
	}

	private void drawLabel(String text, int button,
			       Canvas canvas, Paint paint) {
	    if(gp_coords[5 * button + 4] == 0f)
		paint.setTextAlign(Paint.Align.LEFT);
	    else
		paint.setTextAlign(Paint.Align.RIGHT);
	    canvas.drawText(text,
			    offsetX + imgScale *
			    (gp_coords[5 * button + 0] * bitmapWidth +
			     factor * gp_coords[5 * button + 1]),
			    offsetY + imgScale *
			    (gp_coords[5 * button + 2] * bitmapHeight +
			     factor * gp_coords[5 * button + 3]),
			    paint);
	}


	@Override
	public void onDraw(Canvas canvas) {
	    Resources res = GamepadGuideActivity.this.getResources();
	    Bitmap bitmap =
		BitmapFactory.decodeResource(res,
					     R.drawable.gamepad_guide);
	    float imgWidth = (float)screenWidth * 0.75f;
	    bitmapWidth = (float)bitmap.getWidth();
	    bitmapHeight = (float)bitmap.getHeight();
	    factor = bitmapWidth / 800.0f;
	    float imgRatio = bitmapWidth / bitmapHeight;
	    float imgHeight = imgWidth / imgRatio;
	    if(imgHeight > screenHeight) {
		imgHeight = screenHeight * 0.9f;
		imgWidth = imgRatio * imgHeight;
	    }
	    offsetX = (screenWidth - imgWidth) / 2.0f;
	    offsetY = (screenHeight - imgHeight) / 2.0f;
	    imgScale = imgHeight / bitmapHeight;

	    bitmap =
		Bitmap.createScaledBitmap(bitmap,
					  (int)imgWidth, (int)imgHeight,
					  true);

	    Paint paint = new Paint();
	    paint.setAntiAlias(true);
	    paint.setARGB(255, 224, 224, 224);
	    paint.setTextSize(imgScale * 30.0f * factor);

	    canvas.drawColor(0xFF000000);
            canvas.drawBitmap(bitmap, (int)offsetX, (int)offsetY, paint);


	    SharedPreferences prefs =
		PreferenceManager
		.getDefaultSharedPreferences(GamepadGuideActivity.this);

	    int numKeys = prefs.getInt("keymap_num", 0);
	    for(int idx = 0 ; idx < numKeys ; idx++) {
		int keycode =
		    prefs.getInt("keymap_keycode[" + idx + "]", 0);
		int control =
		    prefs.getInt("keymap_control[" + idx + "]", 0);

		if(keycode == 0)
		    continue;

		for(int key = 0 ; key < GP_CO_NUM ; key++) {
		    if(gp_keycodes[key] == keycode) {
			String text = C2GLSurfaceView.CONTROLEVENTS[control];
			drawLabel(text, key, canvas, paint);
			break;
		    }
		}
	    }

	    drawLabel("Back", GP_CO_BACK, canvas, paint);
	    drawLabel("Analog move", GP_CO_AXIS_L, canvas, paint);

	    if(showBottomText) {
		paint.setTextAlign(Paint.Align.CENTER);
		paint.setARGB(200, 224, 224, 224);
		canvas.drawText("Controls can be reconfigured in the settings",
				screenWidth / 2,
				screenHeight - imgScale * 45.0f * factor,
				paint);
	    }
	}
    }
}
