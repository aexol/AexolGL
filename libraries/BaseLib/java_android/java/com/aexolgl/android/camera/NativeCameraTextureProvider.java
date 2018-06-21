package com.aexolgl.android.camera;

import android.graphics.Matrix;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.util.Log;

import java.io.IOException;

/**
 * Created by darek on 23/02/15.
 */
public class NativeCameraTextureProvider implements SurfaceTexture.OnFrameAvailableListener {
    public static final String LOG_TAG = "camtest";
    private int[] textures = new int[1];

    private SurfaceTexture videoTexture;
    private float[] videoTextureTransform;
    private boolean bVideoTransformSeted = false;
    private boolean frameAvailable = false;
    private boolean openglInited = false;
    private boolean isTextureOwner = true;

    private int camID;
    private Camera camera;
    CameraDevice camDev;

    public NativeCameraTextureProvider(Camera camera, CameraDevice camDevice, int camID) {
        videoTextureTransform = new float[16];
        this.camID = camID;
        this.camera = camera;
        this.camDev = camDevice;
        initGLComponents();
    }

    public void onResume() {

    }

    public void displayOrientationChanged(){
        bVideoTransformSeted = false;
    }

    public void setIsTextureOwner(boolean val){
        isTextureOwner = val;
    }

    public void onPause() {
        deinitGLComponents(); // TODO - WYWOLYWANA ZE ZLEGO WATKU WYKRZACZA BLEDA !!!!!!
        //Log.d(LOG_TAG, "textures released");
    }

    protected void initGLComponents() {
        if (!openglInited) {
            setupTexture();
            openglInited = true;
        }
    }

    private int[] picTextures = new int[1];
    private SurfaceTexture picVideoTexture;

    private void setupTexture() {
        // Generate the actual texture
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES20.glGenTextures(1, textures, 0);
        checkGlError("Texture generate");

        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, textures[0]);
        checkGlError("Texture bind");

        videoTexture = new SurfaceTexture(textures[0]);
        videoTexture.setOnFrameAvailableListener(this);
        try {
            camera.setPreviewTexture(getVideoTexture());
        } catch (IOException e) {
            Log.d(LOG_TAG, e.getMessage());
        }

       //Log.d(LOG_TAG, "Texture setted on id " + textures[0]);
    }

    protected void deinitGLComponents() {
        if (openglInited) {
            openglInited = false;

            if(isTextureOwner)
                GLES20.glDeleteTextures(1, textures, 0);

            videoTexture.release();
            videoTexture.setOnFrameAvailableListener(null);
        }
    }

    public void checkGlError(String op) {
        int error;
        while ((error = GLES20.glGetError()) != GLES20.GL_NO_ERROR) {
            Log.e("SurfaceTest", op + ": glError " + GLUtils.getEGLErrorString(error));
        }
    }

    public SurfaceTexture getVideoTexture() {
        return videoTexture;
    }

    public int getVideoTextureID() {return textures[0]; }

    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        synchronized (this) {
            frameAvailable = true;
            NativeCameraLib.nativeOnNewCameraFrame(camID); // tell to native, that new frame is waiting
        }
    }

    // do wywolania w momencie pojawienia sie nowej tekstury aby odebran najswiezsze dane
    public boolean updateTextureIfNeeded(){
        synchronized (this) {
            if (frameAvailable) {
                videoTexture.updateTexImage();
                frameAvailable = false;

                // BEST PRACTICE IS TO INVOKE JUST ONLY IF REALLY TEXTURE TRANSFORM CHANGED
                if (!bVideoTransformSeted) {
                    videoTexture.getTransformMatrix(videoTextureTransform);
                    NativeCameraLib.nativeSetTransformMatrix(videoTextureTransform, camID);
                    bVideoTransformSeted = true;
                }


                return true;
            }else{
                return false;
            }
        }
    }
}
