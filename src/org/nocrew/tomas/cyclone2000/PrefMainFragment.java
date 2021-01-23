package org.nocrew.tomas.cyclone2000;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.v7.preference.Preference;
import android.support.v7.preference.PreferenceFragmentCompat;
import android.util.Log;

import org.nocrew.tomas.cyclone2000.R;


public class PrefMainFragment extends C2PrefFragmentCompat {

    @Override
    public void onCreatePreferences(Bundle bundle, String s) {
	addPreferencesFromResource(R.xml.pref_main);
    }
}
