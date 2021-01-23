package org.nocrew.tomas.cyclone2000;
import org.nocrew.tomas.cyclone2000.R;
import org.nocrew.tomas.cyclone2000.C2GLSurfaceView;
import org.nocrew.tomas.cyclone2000.KeyMapCode;

import android.os.Build;
import android.content.Context;
import android.content.res.Resources;
import android.content.res.TypedArray;
import android.preference.PreferenceManager;
import android.content.SharedPreferences;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;
import android.view.KeyEvent;
import android.view.MenuItem;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.ListView;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.util.TypedValue;
import android.graphics.drawable.Drawable;
import android.util.Log;
import java.util.ArrayList;
import java.util.Collections;
import java.lang.Comparable;
import java.beans.PropertyChangeListener;

class KeyMapItem extends RelativeLayout implements Comparable {
    public int index;
    public int keycode;
    public int control;

    RelativeLayout.LayoutParams rl;

    public KeyMapItem(Context context, int idx, int key, int ctrl) {
	super(context);

	index = idx;
	keycode = key;
	control = ctrl;

	TextView t1 = new TextView(context);
	t1.setTextSize(TypedValue.COMPLEX_UNIT_SP, 20);
	t1.setText(KeyMapCode.keyMapString(keycode));
	t1.setPadding(dipToPx(10), dipToPx(10), dipToPx(10), dipToPx(10));
	addView(t1);
	rl = (RelativeLayout.LayoutParams)t1.getLayoutParams();
	rl.addRule(RelativeLayout.ALIGN_PARENT_LEFT);
	t1.setLayoutParams(rl);

	TextView t2 = new TextView(context);
	t2.setTextSize(TypedValue.COMPLEX_UNIT_SP, 20);
	t2.setText(C2GLSurfaceView.CONTROLEVENTS[control]);
	t2.setPadding(dipToPx(10), dipToPx(10), dipToPx(10), dipToPx(10));
	addView(t2);
	rl = (RelativeLayout.LayoutParams)t2.getLayoutParams();
	rl.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
	t2.setLayoutParams(rl);
    }

    private int dipToPx(float dip) {
	Resources r = getResources();
	float px = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP,
					     dip, r.getDisplayMetrics());

	return (int)px;
   }

    public int compareTo(Object o) {
	KeyMapItem kmi = (KeyMapItem)o;
        return this.keycode - kmi.keycode;
    }
}

class KeyMapAdapter extends BaseAdapter {
    public int type = 0;

    int numItems = 0;
    ArrayList<KeyMapItem> item = null;

    public KeyMapAdapter(Context context) {
	super();

	SharedPreferences prefs =
	    PreferenceManager.getDefaultSharedPreferences(context);
	item = new ArrayList();

	int numkeys = prefs.getInt("keymap_num", 0);

	for(int i = 0 ; i < numkeys ; i++) {
	    int key = prefs.getInt("keymap_keycode[" + i + "]", 0);
	    int ctrl = prefs.getInt("keymap_control[" + i + "]", 0);

	    if(key == 0)
		continue;

	    item.add(new KeyMapItem(context, i, key, ctrl));
	}

	Collections.sort(item);

	numItems = item.size();
    }

    @Override public boolean areAllItemsEnabled() {
	return true;
    }

    @Override public int getViewTypeCount() {
	return FileInfoView.TYPE_MAX;
    }

    @Override public boolean hasStableIds() {
	return true;
    }

    @Override public boolean isEmpty() {
	return numItems == 0;
    }

    @Override public boolean isEnabled(int position) {
	return (0 <= position) && (position < numItems);
    }

    @Override public int getCount() {
	return numItems;
    }

    @Override public Object getItem(int position) {
	return item.get(position);
    }

    @Override public long getItemId(int position) {
	return position;
    }

    @Override public View getView(int position, View convertView,
				  ViewGroup parent) {
	return item.get(position);
    }
}

public class KeyMapView extends ListView {
    PropertyChangeListener propertyChangeListener = null;
    Context mContext;

    public KeyMapView(Context context) {
	super(context);
	initialise(context);
    }

    public KeyMapView(Context context, AttributeSet attrs) {
	super(context, attrs);
	initialise(context);
    }

    void initialise(Context context) {
	mContext = context;

	SharedPreferences prefs =
	    PreferenceManager.getDefaultSharedPreferences(mContext);
	int numkeys = prefs.getInt("keymap_num", -1);
	if(numkeys == -1)
	    resetDefaults();

	setAdapter(new KeyMapAdapter(context));
    }

    public void setPropertyChangeListener(PropertyChangeListener l) {
	propertyChangeListener = l;
    }

    public void addKeyMap(int keycode, int control) {
	SharedPreferences prefs =
	    PreferenceManager.getDefaultSharedPreferences(mContext);
	SharedPreferences.Editor editor = prefs.edit();

	int numkeys = prefs.getInt("keymap_num", 0);
	int firstFree = -1;
	int found = -1;
	for(int i = 0 ; i < numkeys ; i++) {
	    int key = prefs.getInt("keymap_keycode[" + i + "]", 0);
	    if(key == 0 && firstFree == -1) {
		firstFree = i;
	    } else if(key == keycode) {
		found = i;
		break;
	    }
	}
	if(found == -1) {
	    if(firstFree == -1) {
		editor.putInt("keymap_num", numkeys + 1);
		found = numkeys;
	    } else {
		found = firstFree;
	    }
	}
	editor.putInt("keymap_keycode[" + found + "]", keycode);
	editor.putInt("keymap_control[" + found + "]", control);
	editor.commit();

	setAdapter(new KeyMapAdapter(mContext));
    }

    public boolean isKeyMap(int keycode) {
	SharedPreferences prefs =
	    PreferenceManager.getDefaultSharedPreferences(mContext);

	int numkeys = prefs.getInt("keymap_num", 0);
	for(int i = 0 ; i < numkeys ; i++) {
	    int key = prefs.getInt("keymap_keycode[" + i + "]", 0);
	    if(key == keycode) {
		return true;
	    }
	}

	return false;
    }

    public void resetDefaults() {
	SharedPreferences prefs =
	    PreferenceManager.getDefaultSharedPreferences(mContext);
	SharedPreferences.Editor editor = prefs.edit();

	int numkeys = prefs.getInt("keymap_num", 0);
	for(int i = 0 ; i < numkeys ; i++) {
	    editor.remove("keymap_keycode[" + i + "]");
	}
	editor.remove("keymap_num");
	editor.commit();

	editor = prefs.edit();
	int i = 0;
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_DPAD_LEFT);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_LEFT);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_DPAD_RIGHT);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_RIGHT);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_DPAD_UP);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_JUMP);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_DPAD_DOWN);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_ZAP);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_DPAD_CENTER);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_SHOOT);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_SPACE);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_SHOOT);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_BUTTON_A);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_SHOOT);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_BUTTON_X);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_JUMP);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_BUTTON_B);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_ZAP);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_BUTTON_R1);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_JUMP);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_BUTTON_L1);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_ZAP);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_BUTTON_START);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_PAUSE);
	editor.putInt("keymap_keycode[" + i + "]",
		      KeyEvent.KEYCODE_BUTTON_SELECT);
	editor.putInt("keymap_control[" + i++ + "]",
		      C2GLSurfaceView.CONTROL_EVENT_BACK);

	editor.putInt("keymap_num", i);
	editor.commit();

	setAdapter(new KeyMapAdapter(mContext));
    }

    public void deleteItem(long itemid) {
	KeyMapAdapter adapter = (KeyMapAdapter)getAdapter();
	KeyMapItem item = (KeyMapItem)adapter.getItem((int)itemid);
	SharedPreferences prefs =
	    PreferenceManager.getDefaultSharedPreferences(mContext);
	SharedPreferences.Editor editor = prefs.edit();
	editor.remove("keymap_keycode[" + item.index + "]");
	editor.remove("keymap_control[" + item.index + "]");
	editor.commit();

	setAdapter(new KeyMapAdapter(mContext));
    }
}
