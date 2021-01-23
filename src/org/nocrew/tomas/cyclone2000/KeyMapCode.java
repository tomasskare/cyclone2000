package org.nocrew.tomas.cyclone2000;

import android.os.Build;
import android.util.SparseArray;
import android.view.KeyEvent;

public class KeyMapCode {
    public static String keyMapString(int keyCode) {

	String name;

	name = KeyEvent.keyCodeToString(keyCode);
	if(name.substring(0, 8).equals("KEYCODE_"))
	    name = name.substring(8);
	return name;
    }
}

