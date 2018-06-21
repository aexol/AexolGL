package com.aexolgl.android.camera;

/**
 * Created by darek on 20/02/15.
 */
public class NativeCameraLib {
    static {
        System.loadLibrary("main");
    }

    public static native void nativeInit(int width, int height, int camID);
    public static native void nativeDraw();

    public static native void nativeSetTransformMatrix(float textureTransform[], int camID);
    public static native void nativeOnNewCameraFrame(int camID);

    public static native void nativeCallbackSmoothZoomEnded(int camID);
    public static native void nativeCallbackAutoFocus(boolean success, int camID);
    public static native void nativeCallbackTakePicture(int result, int camID);
    public static native void nativeCallbackStopVideo(int result, int camID);
}
