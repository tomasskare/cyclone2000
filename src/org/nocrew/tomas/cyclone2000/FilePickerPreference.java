package org.nocrew.tomas.cyclone2000;
import org.nocrew.tomas.cyclone2000.R;

import android.app.AlertDialog.Builder;
import android.app.AlertDialog;
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
import android.widget.RelativeLayout;
import android.widget.BaseAdapter;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;

import android.support.v7.preference.Preference;
import android.support.v7.preference.PreferenceViewHolder;

public class FilePickerPreference extends Preference implements PropertyChangeListener, DialogInterface.OnClickListener {
    private static final String	TAG = "FilePickerPreference";
    AlertDialog			dia;
    FilePicker			fp;
    String selectedfile = "";

    TextView mStatusText;

	public FilePickerPreference(Context context, AttributeSet attrs) {
		super(context, attrs);
		initialise(context, attrs);
	}

	public FilePickerPreference(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		initialise(context, attrs);
	}

	void initialise(Context context, AttributeSet attrs) {
	    setLayoutResource(R.layout.filepicker_preference);

	    fp = new FilePicker(context, attrs);
		fp.setPropertyChangeListener(this);

		AlertDialog.Builder b = new AlertDialog.Builder(context);
		b.setPositiveButton("OK", this);
		b.setNegativeButton("Cancel", this);
		b.setView(fp);
		b.setTitle(fp.getWorkingDir());

		dia = b.create();
	}

        @Override
	public void onBindViewHolder(PreferenceViewHolder holder) {
	    super.onBindViewHolder(holder);

	    mStatusText = (TextView)holder.findViewById(R.id.selectedfile);
	    mStatusText.setText(String.valueOf(selectedfile));
	}

        @Override
	protected void onSetInitialValue(boolean restoreValue, Object defaultValue) {

	    if(restoreValue) {
		selectedfile = getPersistedString(selectedfile);
		fp.setWorkingFile(selectedfile);
	    }
	    else {
		selectedfile = "";
		persistString(selectedfile);
	    }
	}


	public void propertyChange(PropertyChangeEvent event) {
		String propName = event.getPropertyName();
		if (propName.contentEquals("workingDir"))
			dia.setTitle((String)event.getNewValue());
	}

	@Override protected void onClick() {
		fp.clearChoices();
		dia.show();
	}

	public void onClick(DialogInterface dialog, int which) {
		if (which == DialogInterface.BUTTON_POSITIVE) {
			String[] files = fp.getSelectedFiles();
			int n = files.length;

			updateSelectedFile((n > 0) ? files[0] : "");
		}
	}


	public void updateSelectedFile(String filename) {
	    selectedfile = filename;
	    persistString(selectedfile);

	    if(mStatusText != null)
		mStatusText.setText(String.valueOf(selectedfile));
	}
}

// vim: set ai si sw=8 ts=8 noet:
