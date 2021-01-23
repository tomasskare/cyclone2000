package org.nocrew.tomas.cyclone2000;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.Build;
import android.view.Window;
import android.view.WindowManager;
import android.media.AudioManager;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.view.View;
import android.view.Display;
import android.view.WindowManager;
import android.view.ViewGroup;
import android.content.pm.ActivityInfo;
import android.content.Intent;
import android.os.Bundle;
import android.app.Activity;

import org.nocrew.tomas.cyclone2000.C2GLSurfaceView;
import org.nocrew.tomas.cyclone2000.C2Preferences;

import org.nocrew.tomas.cyclone2000.R;

public class C2Activity extends Activity {
    private C2GLSurfaceView mGLView;
    private boolean isTV = false;

    public int screenWidth;
    public int screenHeight;

    protected void onCreate(boolean tv, Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

	getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

	isTV = tv;

        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
			     WindowManager.LayoutParams.FLAG_FULLSCREEN);
	setVolumeControlStream(AudioManager.STREAM_MUSIC);

	int[] drawables = {
	    R.drawable.points_2000,
	    R.drawable.up_up,
	    R.drawable.up_1_glow,
	    R.drawable.points_fuseball,
	    R.drawable.prices_and_lifecount,
	    R.drawable.characters,
	    R.drawable.characters_light,
	    R.drawable.menu_icons,
	};

	int[] sounds = {
	    0,
	    R.raw.laser,
	    R.raw.explosion,
	    R.raw.powerup,
	    R.raw.player_explosion,
	    R.raw.endstage,
	    R.raw.shot,
	    R.raw.cling,
	    R.raw.life,
	    R.raw.whooo,
	    R.raw.superzapper,
	    R.raw.yesyesyes,
	};


        mGLView = new C2GLSurfaceView(this, C2Preferences.class,
				      GamepadGuideActivity.class,
				      isTV, drawables, sounds);
        setContentView(mGLView);

	/*
	Display d =
	    ((WindowManager)Cyclone2000Main.this.
	     getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();

	screenWidth = d.getWidth();
	screenHeight = d.getHeight();
	ViewGroup.LayoutParams lp =
	    new ViewGroup.LayoutParams(screenWidth, screenHeight);
	*/

	//addContentView(new PupsMenuGridView(this), lp);
	//addContentView(new FullMenuGridView(this), lp);
	//addContentView(new HSEnterGridView(this), lp);
    }

    @Override
    protected void onStart() {
        super.onStart();
	mGLView.onStart(this);
    }

    @Override
    protected void onStop() {
        super.onStop();
	mGLView.onStop();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mGLView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mGLView.onResume();
    }

    @Override
    protected void onActivityResult(int request, int response, Intent data) {
	super.onActivityResult(request, response, data);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
					   String permissions[],
					   int[] grantResults) {
	mGLView.onRequestPermissionsResult(requestCode, permissions,
					   grantResults);
    }

    public class PupsMenuGridView extends View {
	public PupsMenuGridView(Context context) {
	    super(context);
	}

	@Override
	public void onDraw(Canvas canvas) {
	    Paint paint = new Paint();
	    paint.setAntiAlias(true);
	    paint.setARGB(170, 170, 170, 170);
	    paint.setStyle(Paint.Style.STROKE);
	    paint.setStrokeWidth(4.0f);

	    float width = screenWidth;
	    float height = screenHeight;
	    float r0 = 720.0f / 1196.0f;
	    float r1 = height / width;
	    float rd = (r1 / r0);
	    float rd0 = (1.0f - rd) / 2.0f;
	    rd += rd0;

	    float buyMinx = rd * width * 350.0f / 1196.0f;
	    float buyMaxx = rd * width * 700.0f / 1196.0f;
	    float buyMiny = height * 150.0f / 720.0f;
	    float buyMaxy = height * 245.0f / 720.0f;
	    float backMinx = rd * width * 20.0f / 1196.0f;
	    float backMaxx = rd * width * 160.0f / 1196.0f;
	    float backMiny = buyMiny;
	    float backMaxy = buyMaxy;
	    float priceMinx = rd * width * 900.0f / 1196.0f;
	    float priceMaxx = rd * width * 1100.0f / 1196.0f;
	    float laserMiny = height * 245.0f / 720.0f;
	    float laserMaxy = height * 330.0f / 720.0f;
	    float jumpMiny = height * 370.0f / 720.0f;
	    float jumpMaxy = height * 450.0f / 720.0f;
	    float droidMiny = height * 495.0f / 720.0f;
	    float droidMaxy = height * 570.0f / 720.0f;
	    float lifeMiny = height * 615.0f / 720.0f;
	    float lifeMaxy = height * 700.0f / 720.0f;

	    canvas.drawRect(buyMinx, buyMiny, buyMaxx, buyMaxy, paint);
	    canvas.drawRect(backMinx, backMiny, backMaxx, backMaxy, paint);
	    canvas.drawRect(priceMinx, laserMiny, priceMaxx, laserMaxy, paint);
	    canvas.drawRect(priceMinx, jumpMiny, priceMaxx, jumpMaxy, paint);
	    canvas.drawRect(priceMinx, droidMiny, priceMaxx, droidMaxy, paint);
	    canvas.drawRect(priceMinx, lifeMiny, priceMaxx, lifeMaxy, paint);
	}
    }

    public class FullMenuGridView extends View {
	public FullMenuGridView(Context context) {
	    super(context);
	}

	@Override
	public void onDraw(Canvas canvas) {
	    Paint paint = new Paint();
	    paint.setAntiAlias(true);
	    paint.setARGB(170, 170, 170, 170);
	    paint.setStyle(Paint.Style.STROKE);
	    paint.setStrokeWidth(4.0f);

	    float width = screenWidth;
	    float height = screenHeight;
	    float r0 = 720.0f / 1196.0f;
	    float r1 = height / width;
	    float rd = (r1 / r0);
	    float rd0 = (1.0f - rd) / 2.0f;
	    rd += rd0;

	    float minx = 0;
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
	    float iconsMiny = height * 460.0f / 720.0f;
	    float iconsMaxy = height * 680.0f / 720.0f;
	    float leaderboardMinx = rd * width * 100.0f / 1196.0f;
	    float leaderboardMaxx = rd * width * 315.0f / 1196.0f;
	    float powerupsMinx = rd * width * 315.0f / 1196.0f;
	    float powerupsMaxx = rd * width * 470.0f / 1196.0f;
	    float settingsMinx = rd * width * 470.0f / 1196.0f;
	    float settingsMaxx = rd * width * 680.0f / 1196.0f;

	    canvas.drawRect(minx, continueMiny, continueMaxx, continueMaxy, paint);
	    canvas.drawRect(minx, playMiny, playMaxx, playMaxy, paint);
	    canvas.drawRect(playDownMinx, playMiny, playDownMaxx, playMaxy, paint);
	    canvas.drawRect(playUpMinx, playMiny, playUpMaxx, playMaxy, paint);
	    canvas.drawRect(leaderboardMinx, iconsMiny, leaderboardMaxx, iconsMaxy, paint);
	    canvas.drawRect(powerupsMinx, iconsMiny, powerupsMaxx, iconsMaxy, paint);
	    canvas.drawRect(settingsMinx, iconsMiny, settingsMaxx, iconsMaxy, paint);
	}
    }

    public class HSEnterGridView extends View {
	public HSEnterGridView(Context context) {
	    super(context);
	}

	@Override
	public void onDraw(Canvas canvas) {
	    Paint paint = new Paint();
	    paint.setAntiAlias(true);
	    paint.setARGB(170, 170, 170, 170);
	    paint.setStyle(Paint.Style.STROKE);
	    paint.setStrokeWidth(4.0f);

	    float width = screenWidth;
	    float height = screenHeight;
	    float r0 = 720.0f / 1196.0f;
	    float r1 = height / width;
	    float rd = (r1 / r0);
	    float rd0 = (1.0f - rd) / 2.0f;
	    rd += rd0;

	    float charsMinx = 0.0f;
	    float charsMaxx = width;
	    float charsMiny = height * 530.0f / 720.0f;
	    float charsMaxy = height;
	    float uparrowMinx = rd * width * 540.0f / 1196.0f;
	    float uparrowMaxx = rd * width * 660.0f / 1196.0f;
	    float uparrowMiny = height * 400.0f / 720.0f;
	    float uparrowMaxy = charsMiny;
	    float backarrowMinx = rd * width * 370.0f / 1196.0f;
	    float backarrowMaxx = rd * width * 510.0f / 1196.0f;
	    float backarrowMiny = height * 400.0f / 720.0f;
	    float backarrowMaxy = charsMiny;
	    float doneMinx = rd * width * 690.0f / 1196.0f;
	    float doneMaxx = rd * width * 830.0f / 1196.0f;
	    float doneMiny = height * 400.0f / 720.0f;
	    float doneMaxy = charsMiny;

	    canvas.drawRect(charsMinx, charsMiny,
			    charsMaxx, charsMaxy, paint);
	    canvas.drawRect(uparrowMinx, uparrowMiny,
			    uparrowMaxx, uparrowMaxy, paint);
	    canvas.drawRect(backarrowMinx, backarrowMiny,
			    backarrowMaxx, backarrowMaxy, paint);
	    canvas.drawRect(doneMinx, doneMiny,
			    doneMaxx, doneMaxy, paint);
	}
    }
}
