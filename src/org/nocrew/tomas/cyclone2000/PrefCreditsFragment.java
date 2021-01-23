package org.nocrew.tomas.cyclone2000;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.ColorDrawable;

import android.support.v7.preference.PreferenceFragmentCompat;

import org.nocrew.tomas.cyclone2000.R;


public class PrefCreditsFragment extends C2PrefFragmentCompat {

    @Override
    public void onCreatePreferences(Bundle bundle, String s) {
	addPreferencesFromResource(R.xml.pref_credits);
    }

    @Override
    public void setDivider(Drawable divider) {
	super.setDivider(new ColorDrawable(Color.TRANSPARENT));
    }

    @Override
    public void setDividerHeight(int height) {
	super.setDividerHeight(0);
    }
}
