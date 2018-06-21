package com.aexolgl.android.camera;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class NativeRenderer implements GLSurfaceView.Renderer {
    public static final String LOG_TAG = "libgl2jni";

    public NativeRenderer() {
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        //  GL2JNILib.init(width, height);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        NativeCameraLib.nativeInit(width, height, 0);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        synchronized (this) {
            NativeCameraLib.nativeDraw(); // Send drawing command to native code with texture id
        }
    }
}
