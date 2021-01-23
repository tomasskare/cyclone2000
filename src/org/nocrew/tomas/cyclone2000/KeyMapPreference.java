package org.nocrew.tomas.cyclone2000;
import org.nocrew.tomas.cyclone2000.R;

import android.app.AlertDialog.Builder;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences.Editor;
import android.content.SharedPreferences;
import android.content.res.TypedArray;
import android.util.AttributeSet;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewParent;
import android.view.KeyEvent;
import android.widget.RelativeLayout;
import android.widget.BaseAdapter;
import android.widget.ArrayAdapter;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Button;
import android.widget.Spinner;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.util.Random;

import android.support.v7.preference.Preference;
import android.support.v7.preference.PreferenceFragmentCompat;

import org.nocrew.tomas.cyclone2000.KeyMapView;
import org.nocrew.tomas.cyclone2000.C2GLSurfaceView;
import org.nocrew.tomas.cyclone2000.KeyMapCode;

class KeyMapReaderDialog extends AlertDialog implements DialogInterface.OnClickListener, AdapterView.OnItemSelectedListener {
    private Context context;
    private KeyMapView kmv;
    private AlertDialog dia;
    private Spinner spin;
    private int keyPressed = -1;
    private int selectedEvent = -1;

    public KeyMapReaderDialog(Context context, KeyMapView kmv) {
	super(context);
	this.context = context;
	this.kmv = kmv;

	setButton(DialogInterface.BUTTON_NEGATIVE, "Cancel", this);
	setTitle("Press hardware key or button");
    }

    public boolean onKeyDown(int keyCode, KeyEvent event) {
	if(keyCode == KeyEvent.KEYCODE_BACK ||
	   keyCode == KeyEvent.KEYCODE_MENU)
	    return false;

	keyPressed = keyCode;

	return true;
    }

    public boolean onKeyUp(int keyCode, KeyEvent event) {
	if(keyCode == KeyEvent.KEYCODE_BACK ||
	   keyCode == KeyEvent.KEYCODE_MENU)
	    return false;

	if(keyPressed != keyCode) {
	    keyPressed = -1;
	    return true;
	}

	dismiss();

	spin = new Spinner(context);
	ArrayAdapter aa = new ArrayAdapter(context,
					   android.R.layout.simple_spinner_item,
					   C2GLSurfaceView.CONTROLEVENTS);
	aa.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
	spin.setAdapter(aa);
	spin.setOnItemSelectedListener(this);

	AlertDialog.Builder b = new AlertDialog.Builder(context);
	b.setNegativeButton("Cancel", this);
	b.setPositiveButton("OK", this);
	if(kmv.isKeyMap(keyPressed)) {
	    b.setTitle("Modify event for " +
		       KeyMapCode.keyMapString(keyPressed));
	} else {
	    b.setTitle("Set event for key " +
		       KeyMapCode.keyMapString(keyPressed));
	}
	b.setView(spin);
	dia = b.create();
	dia.show();

	return true;
    }

    public void onClick(DialogInterface dialog, int which) {
	if(which == DialogInterface.BUTTON_NEGATIVE)
	    return;

	View view = spin.getSelectedView();

	if(selectedEvent == -1)
	    return;

	kmv.addKeyMap(keyPressed, selectedEvent);
    }

    @Override
    public void onItemSelected(AdapterView<?> parent, View view,
			       int pos, long id) {
	selectedEvent = pos;
    }

    @Override
    public void onNothingSelected(AdapterView<?> parent) {
	selectedEvent = -1;
    }
}

class KeyMapDialogListener implements View.OnClickListener {
    private Context context;
    private final Dialog dialog;
    private KeyMapView kmv;
    private KeyMapReaderDialog kmrd;

    public KeyMapDialogListener(Context context, Dialog dialog,
				KeyMapView kmv) {
	this.context = context;
        this.dialog = dialog;
	this.kmv = kmv;
    }

    @Override
    public void onClick(View v) {
	kmrd = new KeyMapReaderDialog(context, kmv);
	kmrd.show();
    }
}

class KeyMapResetDefaults implements View.OnClickListener {
    private Context context;
    private KeyMapView kmv;

    public KeyMapResetDefaults(Context context, KeyMapView kmv) {
	this.context = context;
	this.kmv = kmv;
    }

    @Override
    public void onClick(View v) {
	kmv.resetDefaults();
    }
}

public class KeyMapPreference extends Preference implements DialogInterface.OnClickListener {
    AlertDialog dia;
    KeyMapView kmv;
    Context context;
    AttributeSet attrs;

    public KeyMapPreference(Context context, AttributeSet attrs) {
	super(context, attrs);
	initialise(context, attrs);
    }

    public KeyMapPreference(Context context, AttributeSet attrs, int defStyle) {
	super(context, attrs, defStyle);
	initialise(context, attrs);
    }

    void initialise(Context context, AttributeSet attrs) {
	setLayoutResource(R.layout.keymap_preference);
	this.context = context;
	this.attrs = attrs;
    }

    public void setup(PreferenceFragmentCompat fragment) {
	kmv = new KeyMapView(context);
	fragment.registerForContextMenu(kmv);

	AlertDialog.Builder b = new AlertDialog.Builder(context);
	b.setNegativeButton("Back", this);
	b.setNeutralButton("Reset to defaults", this);
	b.setPositiveButton("New mapping", this);
	b.setView(kmv);
	b.setTitle("Configure key mappings");

	dia = b.create();
    }

    @Override
    protected void onSetInitialValue(boolean restoreValue, Object defaultValue) {
    }

    @Override
    protected void onClick() {
	dia.show();
	Button theButton = dia.getButton(DialogInterface.BUTTON_POSITIVE);
	theButton.setOnClickListener(new KeyMapDialogListener(context, dia, kmv));
	theButton = dia.getButton(DialogInterface.BUTTON_NEUTRAL);
	theButton.setOnClickListener(new KeyMapResetDefaults(context, kmv));
    }

    public void onClick(DialogInterface dialog, int which) {
    }
}
