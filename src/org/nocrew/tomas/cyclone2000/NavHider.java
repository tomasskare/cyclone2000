package org.nocrew.tomas.cyclone2000;

import android.os.Handler;
import android.os.Build;
import android.view.View;
import android.util.Log;

public class NavHider implements View.OnSystemUiVisibilityChangeListener {
    private Handler navHiderHandler = new Handler();
    private Runnable navHider;
    private View view;
    private int flags = View.SYSTEM_UI_FLAG_LOW_PROFILE;
    private int toggleFlags;
    private boolean haveImmersive;

    public NavHider(View view, boolean full, boolean immersive) {
	haveImmersive = immersive;
	this.view = view;
	navHider = new Runnable() {
		@Override
		public void run() {
		    hideNav();
		}
	    };

	toggleFlags |= View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
	if(haveImmersive)
	    toggleFlags |= View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;

	if(full)
	    flags |= toggleFlags;

	view.setOnSystemUiVisibilityChangeListener(this);
	hideNav();
    }

    public void setFull(boolean full) {
	if(full)
	    flags |= toggleFlags;
	else
	    flags &= ~toggleFlags;
	hideNav();
    }

    @Override
    public void onSystemUiVisibilityChange(int visibility) {
	if(visibility != flags)
	    navHiderHandler.postDelayed(navHider, 1000);
    }

    public void hideNav() {
	navHiderHandler.removeCallbacks(navHider);
	view.setSystemUiVisibility(flags);
    }
}

