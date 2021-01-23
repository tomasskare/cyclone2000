package org.nocrew.tomas.cyclone2000;

import android.content.Context;
import android.content.res.TypedArray;
import android.util.AttributeSet;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewParent;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

import android.support.v7.preference.Preference;
import android.support.v7.preference.PreferenceViewHolder;

import org.nocrew.tomas.cyclone2000.R;

public class SeekBarPreference extends Preference
    implements OnSeekBarChangeListener,
	       View.OnFocusChangeListener {
    
    private final String TAG = getClass().getName();
    
    private static final String ANDROIDNS =
	"http://schemas.android.com/apk/res/android";
    private static final String NOCREWNS =
	"http://nocrew.org";
    private static final int DEFAULT_VALUE = 50;
    
    private int mMaxValue      = 100;
    private int mMinValue      = 0;
    private int mInterval      = 1;
    private int mCurrentValue;
    private String mUnitsLeft  = "";
    private String mUnitsRight = "";
    private SeekBar mSeekBar;
    private boolean mHasFocus;
    
    private TextView mStatusText;
    private TextView mUnitsLeftText;
    private TextView mUnitsRightText;

    public SeekBarPreference(Context context, AttributeSet attrs) {
        super(context, attrs);
        initPreference(context, attrs);
    }

    public SeekBarPreference(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        initPreference(context, attrs);
    }

    private void initPreference(Context context, AttributeSet attrs) {
        setValuesFromXml(attrs);
        mSeekBar = new SeekBar(context, attrs);
        mSeekBar.setMax(mMaxValue - mMinValue);
        mSeekBar.setOnSeekBarChangeListener(this);
	setLayoutResource(R.layout.seek_bar_preference);
    }
    
    private void setValuesFromXml(AttributeSet attrs) {
        mMaxValue = attrs.getAttributeIntValue(ANDROIDNS, "max", 100);
        mMinValue = attrs.getAttributeIntValue(NOCREWNS, "min", 0);
        
        mUnitsLeft = getAttributeStringValue(attrs, NOCREWNS, "unitsLeft", "");
        String units = getAttributeStringValue(attrs, NOCREWNS, "units", "");
        mUnitsRight = getAttributeStringValue(attrs, NOCREWNS, "unitsRight", units);
        
        try {
            String newInterval = attrs.getAttributeValue(NOCREWNS, "interval");
            if(newInterval != null)
                mInterval = Integer.parseInt(newInterval);
        }
        catch(Exception e) {
            Log.e(TAG, "Invalid interval value", e);
        }
        
    }
    
    private String getAttributeStringValue(AttributeSet attrs, String namespace, String name, String defaultValue) {
        String value = attrs.getAttributeValue(namespace, name);
        if(value == null)
            value = defaultValue;
        
        return value;
    }

    @Override
    public void onBindViewHolder(PreferenceViewHolder holder) {
        super.onBindViewHolder(holder);

        try
        {
            // move our seekbar to the new view we've been given
            ViewParent oldContainer = mSeekBar.getParent();
            ViewGroup newContainer = (ViewGroup)holder.findViewById(R.id.seekBarPrefBarContainer);
            
            if (oldContainer != newContainer) {
                // remove the seekbar from the old view
                if (oldContainer != null) {
                    ((ViewGroup) oldContainer).removeView(mSeekBar);
                }
                // remove the existing seekbar (there may not be one) and add ours
                newContainer.removeAllViews();
                newContainer.addView(mSeekBar, ViewGroup.LayoutParams.FILL_PARENT,
                        ViewGroup.LayoutParams.WRAP_CONTENT);
            }
        }
        catch(Exception ex) {
            Log.e(TAG, "Error binding view: " + ex.toString());
        }

	mStatusText = (TextView)holder.findViewById(R.id.seekBarPrefValue);
	mUnitsLeftText = (TextView)holder.findViewById(R.id.seekBarPrefUnitsLeft);
	mUnitsRightText = (TextView)holder.findViewById(R.id.seekBarPrefUnitsRight);

	holder.itemView.setOnFocusChangeListener(this);

	updateView();
    }
    
    protected void updateView() {
        try {
            mStatusText.setText(String.valueOf(mCurrentValue));
            mStatusText.setMinimumWidth(30);
            mSeekBar.setProgress(mCurrentValue - mMinValue);
            mUnitsRightText.setText(mUnitsRight);
            mUnitsLeftText.setText(mUnitsLeft);
        }
        catch(Exception e) {
            Log.e(TAG, "Error updating seek bar preference", e);
        }
    }

    public void progressStep(int step) {
	mCurrentValue += step;
	onProgressChanged(mSeekBar, mCurrentValue - mMinValue, true);
    }

    public boolean hasFocus() {
	return mHasFocus;
    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        int newValue = progress + mMinValue;
        
        if(newValue > mMaxValue)
            newValue = mMaxValue;
        else if(newValue < mMinValue)
            newValue = mMinValue;
        else if(mInterval != 1 && newValue % mInterval != 0)
            newValue = Math.round(((float)newValue)/mInterval)*mInterval;  
        
        // change rejected, revert to the previous value
        if(!callChangeListener(newValue)){
            seekBar.setProgress(mCurrentValue - mMinValue); 
            return; 
        }

        // change accepted, store it
        mCurrentValue = newValue;
        mStatusText.setText(String.valueOf(newValue));
        persistInt(newValue);

	if(fromUser)
	    seekBar.setProgress(mCurrentValue - mMinValue);
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {}

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {
        notifyChanged();
    }


    @Override 
    protected Object onGetDefaultValue(TypedArray ta, int index){
        
        int defaultValue = ta.getInt(index, DEFAULT_VALUE);
        return defaultValue;
        
    }

    @Override
    protected void onSetInitialValue(boolean restoreValue, Object defaultValue) {

        if(restoreValue) {
            mCurrentValue = getPersistedInt(mCurrentValue);
        }
        else {
            int temp = 0;
            try {
                temp = (Integer)defaultValue;
            }
            catch(Exception ex) {
                Log.e(TAG, "Invalid default value: " + defaultValue.toString());
            }
            
            persistInt(temp);
            mCurrentValue = temp;
        }
        
    }

    public void onFocusChange(View v, boolean hasFocus) {
	mHasFocus = hasFocus;
    }
}
