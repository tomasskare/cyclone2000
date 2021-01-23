package org.nocrew.tomas.cyclone2000;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.SharedPreferences;

import android.support.v7.preference.Preference;
import android.support.v7.preference.ListPreference;
import android.support.v7.preference.PreferenceManager;
import android.support.v7.preference.PreferenceFragmentCompat;

import org.nocrew.tomas.cyclone2000.R;


public class PrefHsFragment extends C2PrefFragmentCompat {

    @Override
    public void onCreatePreferences(Bundle bundle, String s) {
	addPreferencesFromResource(R.xml.pref_hs);
    }

    @Override
    public boolean onPreferenceTreeClick(Preference pref) {
	String key = pref.getKey();

	if(key.equals("pref_hs_reset")) {
	    AlertDialog.Builder builder =
		new AlertDialog.Builder(getActivity());
	    builder.setMessage(R.string.pref_hs_reset_confirm);
	    builder.setCancelable(true);
	    builder.setPositiveButton(R.string.dialog_ok,
				      new DialogInterface.OnClickListener() {
					  public void onClick(DialogInterface dialog, int id) {
					      SharedPreferences prefs = getPreferenceScreen().getSharedPreferences();
					      SharedPreferences.Editor editor = prefs.edit();
					      editor.putBoolean("reset_hs", true);
					      editor.commit();
					  }
				      });
	    builder.setNegativeButton(R.string.dialog_cancel,
				      new DialogInterface.OnClickListener() {
					  public void onClick(DialogInterface dialog, int id) {
					      dialog.cancel();
					  }
				      });
	    AlertDialog alert = builder.create();
	    alert.show();
	    return true;
	}

	return false;
    }
}
