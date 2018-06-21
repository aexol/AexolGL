package com.aexolgl.android.camera;

import android.hardware.Camera;

/**
 * Created by darek on 16/04/15.
 */
public class ShutterHandler implements Camera.ShutterCallback {

    private int camID;

    public ShutterHandler(int camID){
        this.camID = camID;
    }

    @Override
    public void onShutter() {

    }
}
