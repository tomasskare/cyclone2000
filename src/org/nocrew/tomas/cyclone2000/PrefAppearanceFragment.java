package org.nocrew.tomas.cyclone2000;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Build;
import android.util.Log;
import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.KeyEvent;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.widget.AdapterView.AdapterContextMenuInfo;

import android.support.v7.widget.RecyclerView;
import android.support.v7.preference.Preference;
import android.support.v7.preference.PreferenceScreen;
import android.support.v7.preference.ListPreference;
import android.support.v7.preference.PreferenceFragmentCompat;

import org.nocrew.tomas.cyclone2000.R;


public class PrefAppearanceFragment extends C2PrefFragmentCompat
    implements SharedPreferences.OnSharedPreferenceChangeListener {

    @Override
    public void onCreatePreferences(Bundle bundle, String s) {
	addPreferencesFromResource(R.xml.pref_appearance);
	updatePreferenceSummaries();
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
	    (ListPreference)findPreference("pref_gfx");
	listPreference.setSummary(listPreference.getEntry());

	listPreference =
	    (ListPreference)findPreference("pref_fullscreen_opt");
	if(listPreference != null)
	    listPreference.setSummary(listPreference.getEntry());

	listPreference =
	    (ListPreference)findPreference("pref_vr_opt");
	if(listPreference != null)
	    listPreference.setSummary(listPreference.getEntry());
    }
}
