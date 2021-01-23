package org.nocrew.tomas.cyclone2000;

import android.Manifest;
import android.util.Log;
import android.util.AttributeSet;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.KeyEvent;
import android.widget.LinearLayout;
import android.widget.FrameLayout;
import android.widget.Toast;
import android.support.v4.app.Fragment;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.app.AppCompatDelegate;
import android.support.v7.preference.Preference;
import android.support.v7.preference.PreferenceManager;
import android.support.v7.preference.PreferenceFragmentCompat;

import org.nocrew.tomas.cyclone2000.R;
import org.nocrew.tomas.cyclone2000.C2GLSurfaceView;

public class C2Preferences extends AppCompatActivity
    implements PreferenceFragmentCompat.OnPreferenceStartFragmentCallback {

    // request codes used for permission requests
    private static final int RC_PERM_STORAGE = 6000;

    private boolean permStorage = false;

    static
    {
	AppCompatDelegate.setCompatVectorFromResourcesEnabled(true);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
	super.onCreate(savedInstanceState);

	getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

	setContentView(R.layout.settings);

	SharedPreferences prefs =
	    PreferenceManager
	    .getDefaultSharedPreferences(this);

	getSupportFragmentManager().beginTransaction()
	    .replace(R.id.settings_content, new PrefMainFragment(),
		     "pref_main")
	    .commit();
    }


    @Override
    public boolean onPreferenceStartFragment(PreferenceFragmentCompat caller,
					     Preference pref) {
	String prefKey = pref.getKey();

	PreferenceFragmentCompat fragment;

	if(prefKey.equals("pref_main_control")) {
	    fragment = new PrefControlFragment();
	} else if(prefKey.equals("pref_main_appearance")) {
	    fragment = new PrefAppearanceFragment();
	} else if(prefKey.equals("pref_main_sound")) {
	    fragment = new PrefSoundFragment();
	} else if(prefKey.equals("pref_main_hs")) {
	    fragment = new PrefHsFragment();
	} else if(prefKey.equals("pref_main_pups")) {
	    fragment = new PrefPupsFragment();
	} else if(prefKey.equals("pref_main_credits")) {
	    fragment = new PrefCreditsFragment();
	} else {
	    return false;
	}

	getSupportFragmentManager().beginTransaction()
	    .replace(R.id.settings_content, fragment, prefKey)
	    .addToBackStack(null)
	    .commit();

	return true;
    }

    @Override
    protected void onResume() {
	super.onResume();
    }

    @Override
    protected void onPause() {
	super.onPause();
    }

    @Override
    protected void onDestroy() {
	super.onDestroy();
    }

    public boolean onKeyDown(int keyCode, KeyEvent event) {
	if(keyCode != KeyEvent.KEYCODE_DPAD_LEFT &&
	   keyCode != KeyEvent.KEYCODE_DPAD_RIGHT)
	    return false;

	Fragment fragment = getSupportFragmentManager()
	    .findFragmentById(R.id.settings_content);
	if(!(fragment instanceof IPrefFragment)) {
	    return false;
	}

	IPrefFragment ipref = (IPrefFragment)fragment;
	ipref.keyDown(keyCode);

	return true;
    }

    public boolean onKeyUp(int keyCode, KeyEvent event) {
	if(keyCode == KeyEvent.KEYCODE_BACK) {
	    super.onBackPressed();
	    return true;
	}

	return false;
    }

    private void showAlert(int res, boolean positive) {
	AlertDialog.Builder builder =
	    new AlertDialog.Builder(this);
	builder.setMessage(res);
	builder.setCancelable(true);
	if(positive) {
	    builder.setPositiveButton(R.string.dialog_ok,
				      new DialogInterface.OnClickListener() {
					  public void onClick(DialogInterface dialog, int id) {
					      dialog.cancel();
					  }
				      });
	} else {
	    builder.setNegativeButton(R.string.dialog_cancel,
				      new DialogInterface.OnClickListener() {
					  public void onClick(DialogInterface dialog, int id) {
					      dialog.cancel();
					  }
				      });
	}
	AlertDialog alert = builder.create();
	alert.show();
    }

    public boolean checkStoragePermission(boolean haveAsked) {
	if(ContextCompat
	   .checkSelfPermission(this,
				Manifest.permission.READ_EXTERNAL_STORAGE)
	   != PackageManager.PERMISSION_GRANTED) {
	    if(!haveAsked &&
	       ActivityCompat
	       .shouldShowRequestPermissionRationale(this,
						     Manifest.permission.READ_EXTERNAL_STORAGE)) {
		AlertDialog.Builder builder =
		    new AlertDialog.Builder(this);
		builder.setMessage("Cyclone 2000 needs permission to read " +
				   "and write to the device storage for the " +
				   "music to the game. Please accept the " +
				   "permission request, or music will not " +
				   "be able to play.");
		builder.setPositiveButton(R.string.dialog_ok,
					  new DialogInterface.OnClickListener() {
					      public void onClick(DialogInterface dialog, int id) {
						  checkStoragePermission(true);
					      }
					  });
		AlertDialog alert = builder.create();
		alert.show();
		return false;
	    } else {
		ActivityCompat
		    .requestPermissions(this,
					new String[]{Manifest.permission.READ_EXTERNAL_STORAGE},
					RC_PERM_STORAGE);
		return false;
	    }
	} else {
	    permStorage = true;
	    return true;
	}
    }

    @Override
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
}
