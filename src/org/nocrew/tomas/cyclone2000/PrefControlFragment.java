package org.nocrew.tomas.cyclone2000;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.content.Intent;
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


public class PrefControlFragment extends C2PrefFragmentCompat
    implements IPrefFragment,
	       MenuItem.OnMenuItemClickListener,
	       SharedPreferences.OnSharedPreferenceChangeListener {

    SeekBarPreference seekBarSensitivity;
    KeyMapPreference keyMap;
    KeyMapView kmv;

    @Override
    public void onCreatePreferences(Bundle bundle, String s) {
	addPreferencesFromResource(R.xml.pref_control);
	updatePreferenceSummaries();
	seekBarSensitivity =
	    (SeekBarPreference)findPreference("pref_control_sensitivity");
	keyMap =
	    (KeyMapPreference)findPreference("pref_control_keymap");
	keyMap.setup(this);
    }

    @Override
    public void onStart() {
	super.onStart();
	getPreferenceManager().getSharedPreferences()
	    .registerOnSharedPreferenceChangeListener(this);
	updatePreferenceSummaries();
    }

    @Override
    public void onStop() {
	super.onStop();
	getPreferenceManager().getSharedPreferences()
	    .unregisterOnSharedPreferenceChangeListener(this);
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences,
					  String key) {
	updatePreferenceSummaries();
    }

    private void updatePreferenceSummaries() {
	ListPreference listPreference;

	listPreference =
	    (ListPreference)findPreference("pref_control");
	listPreference.setSummary(listPreference.getEntry());

	listPreference =
	    (ListPreference)findPreference("pref_control_size");
	listPreference.setSummary(listPreference.getEntry());

	listPreference =
	    (ListPreference)findPreference("pref_control_button_size");
	listPreference.setSummary(listPreference.getEntry());

	listPreference =
	    (ListPreference)findPreference("pref_control_hide_opt");
	listPreference.setSummary(listPreference.getEntry());
    }

    @Override
    public void keyDown(int keyCode) {
	int step = (keyCode == KeyEvent.KEYCODE_DPAD_LEFT ? -1 : 1);

	if(seekBarSensitivity.hasFocus()) {
	    seekBarSensitivity.progressStep(step);
	}
    }

    @Override
    public boolean onPreferenceTreeClick(Preference pref) {
	String key = pref.getKey();

	if(key.equals("pref_control_gamepad")) {
	    Intent gpIntent = new Intent(getActivity(), GamepadGuideActivity.class);
	    gpIntent.putExtra("showBottomText", false);
	    startActivity(gpIntent);
	    return true;
	}

	return false;
    }

    @Override
    public void onCreateContextMenu(ContextMenu menu, View v,
				    ContextMenuInfo menuInfo) {
	super.onCreateContextMenu(menu, v, menuInfo);
	MenuInflater inflater = getActivity().getMenuInflater();
	inflater.inflate(R.menu.keymapitem_context, menu);
	for(int i = 0 ; i < menu.size() ; i++) {
	    menu.getItem(i).setOnMenuItemClickListener(this);
	}

	kmv = (KeyMapView)v;
    }

    @Override
    public boolean onContextItemSelected(MenuItem item) {
	AdapterContextMenuInfo info =
	    (AdapterContextMenuInfo)item.getMenuInfo();

	switch (item.getItemId()) {
	case R.id.keymapitem_delete:
	    kmv.deleteItem(info.id);
	    return true;
	default:
	    return super.onContextItemSelected(item);
	}
    }

    @Override
    public boolean onMenuItemClick(MenuItem item) {
	return onContextItemSelected(item);
    }
}
