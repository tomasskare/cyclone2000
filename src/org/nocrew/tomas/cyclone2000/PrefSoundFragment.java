package org.nocrew.tomas.cyclone2000;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URL;
import java.net.URLConnection;
import android.content.SharedPreferences;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Environment;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.AsyncTask;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.util.Log;
import android.app.ProgressDialog;
import android.app.AlertDialog;
import android.view.View;
import android.view.KeyEvent;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.widget.AdapterView.AdapterContextMenuInfo;

import android.support.v7.preference.Preference;
import android.support.v7.preference.ListPreference;
import android.support.v7.preference.PreferenceManager;
import android.support.v7.preference.PreferenceFragmentCompat;

import org.nocrew.tomas.cyclone2000.R;


public class PrefSoundFragment extends C2PrefFragmentCompat
    implements IPrefFragment {

    SeekBarPreference seekBarMusic;
    SeekBarPreference seekBarSfx;

    @Override
    public void onCreatePreferences(Bundle bundle, String s) {
	addPreferencesFromResource(R.xml.pref_sound);
	seekBarMusic = (SeekBarPreference)findPreference("pref_sound_music");
	seekBarSfx = (SeekBarPreference)findPreference("pref_sound_sfx");
    }

    @Override
    public void keyDown(int keyCode) {
	int step = (keyCode == KeyEvent.KEYCODE_DPAD_LEFT ? -1 : 1);

	if(seekBarMusic.hasFocus()) {
	    seekBarMusic.progressStep(step);
	} else if(seekBarSfx.hasFocus()) {
	    seekBarSfx.progressStep(step);
	}
    }
}
