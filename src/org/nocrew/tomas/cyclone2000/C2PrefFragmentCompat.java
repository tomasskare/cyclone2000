package org.nocrew.tomas.cyclone2000;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.ViewGroup;

import android.support.v7.widget.RecyclerView;
import android.support.v7.preference.PreferenceFragmentCompat;

public abstract class C2PrefFragmentCompat extends PreferenceFragmentCompat {
    @Override
    public RecyclerView onCreateRecyclerView(LayoutInflater inflater,
					     ViewGroup parent,
					     Bundle savedInstanceState) {
	RecyclerView recyclerView = (RecyclerView) inflater
	    .inflate(R.layout.preference_recyclerview, parent, false);

	recyclerView.setLayoutManager(onCreateLayoutManager());
	recyclerView
	    .setDescendantFocusability(ViewGroup.FOCUS_AFTER_DESCENDANTS);
	recyclerView.setFocusable(true);
	recyclerView.setFocusableInTouchMode(true);
	recyclerView.requestFocus();
	recyclerView.requestFocusFromTouch();
	recyclerView.focusableViewAvailable(recyclerView);

	return recyclerView;
    }
}
