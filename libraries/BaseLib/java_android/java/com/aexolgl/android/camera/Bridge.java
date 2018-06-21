package com.aexolgl.android.camera;


import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.hardware.Camera;
import android.media.MediaScannerConnection;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.view.WindowManager;

import com.aexolgl.android.app.AexActivity;

import java.io.File;
import java.util.ArrayList;
import java.util.Locale;

/**
 * Created by darek on 18/02/15.
 */

public class Bridge {
    public static int CONST_CAMERA_SUCCESS() {return 1;} // means TRUE too
    public static int CONST_CAMERA_FAILURE() {return 0;} // means FALSE too
    public static int CONST_CAMERA_IS_OPENED() {return 2;}
    public static int CONST_CAMERA_IS_NOT_OPENED() {return 3;}
    public static int CONST_CAMERA_OPENING_ERROR() {return 4;}
    public static int CONST_CAMERA_NO_CAMERA_WITH_PROVIDED_ID() {return 5;}
    public static int CONST_CAMERA_NO_PERMISSION(){return 6;}
    public static int CONST_PARAMETER_SUPPORTED() {return 7;}
    public static int CONST_PARAMETER_NOT_SUPPORTED() {return 8;}

    public static final String LOG_TAGG = "aexolgl";
    private static ArrayList<CameraDevice> cameras = new ArrayList<CameraDevice>();

    // always release camera if pausing
    public static void onPause() {
        for(int i = 0 ; i < cameras.size() ; i++) {
            getCam(i).onPause();
        }
    }

    public static void onResume() {
        for(int i = 0 ; i < cameras.size() ; i++) {
            getCam(i).onResume();
        }
    }

    private static CameraDevice getCam(int camID){
        return cameras.get(camID);
    }

    static String errorMessage = new String();
    public static void setErrorMessage(String msg){
            errorMessage = msg;
    }

    public static String getErrorMessage(int camID){
        return errorMessage;
    }

    public static void setIsJavaTextureOwner(boolean isOwner, int camID){
        getCam(camID).setIsJavaTextureOwner(isOwner);
    }
    public static int getNumberOfCameras() {
        return Camera.getNumberOfCameras();
    }

    public static int open(int cameraId) {
        while(cameraId >= cameras.size())
            cameras.add(new CameraDevice());

        return getCam(cameraId).open(cameraId);
    }

    public static int release(int camID) {
       return getCam(camID).release();
    }

    public static int updateTextureIfNeeded(int camID){
        return getCam(camID).updateTextureIfNeeded();
    }

    public static int getTextureID(int camID){
        return getCam(camID).getTextureID();
    }

    public static int autoFocus(int camID){
        return getCam(camID).autoFocus();
    }

    public static int startPreview(int camID) {
       return getCam(camID).startPreview();
    }

    public static int stopPreview(int camID) {
        return getCam(camID).stopPreview();
    }

    /* return in int arr format, "<WIDTH>:<HEIGHT> ", divided by spaces. 2 possible sizes eg. "400:300 800:600"  */
    public static int[] getSupportedPreviewSizes(int camID){
        return getCam(camID).getSupportedPreviewSizes();
    }

    public static int setPreviewSize(int width, int height, int camID){
        return getCam(camID).setPreviewSize(width, height);
    }

    public static int setDisplayOrientation(int degrees, int camID){
        return getCam(camID).setDisplayOrientation(degrees);
    }

    public static int setParameter(int param, int camID){
        return getCam(camID).setParameter(param);
    }

    public static int isSupported(int param, int camID){
        return getCam(camID).isSupported(param);
    }

    public static int[] getZoomRatios(int camID){
        return getCam(camID).getZoomRatios();
    }

    public static int getZoomAndroid(int camID){
        return getCam(camID).getZoom();
    }

    public static float getZoomAexolGL(int camID){
        return getCam(camID).getZoomAexolGL();
    }

    public static float getZoomMaxAexolGL(int camID){
        return getCam(camID).getZoomMaxAexolGL();
    }

    public static int setZoomAexolGL(float zoom, int camID){
        return getCam(camID).setZoomAexolGL(zoom);
    }

    public static int setZoomSmoothAexolGL(float zoom, int camID){
        return getCam(camID).setZoomSmoothAexolGL(zoom);
    }

    public static int isSmoothlyZoomSupported(int camID){
        return getCam(camID).isSmoothlyZoomSupported(camID);
    }

    public static float getFocalLength(int camID){
        return getCam(camID).getFocalLength();
    }

    public static float getExposureEVAexolGL(int camID){
        return getCam(camID).getExposureEVAexolGL();
    }

    public static float getMinExposureEVAexolGL(int camID){
        return getCam(camID).getMinExposureEVAexolGL();
    }

    public static float getMaxExposureEVAexolGL(int camID){
        return getCam(camID).getMaxExposureEVAexolGL();
    }

    public static int setExposureEVAexolGL(float ev, int camID){
        return getCam(camID).setExposureEVAexolGL(ev);
    }

    public static float getExposureCompensationStep(int camID){
        return getCam(camID).getExposureCompensationStep();
    }

    public static int getExposureCompensationAndroid(int camID){
        return getCam(camID).getExposureCompensation();
    }

    public static int getExposureCompensationMinAndroid(int camID){
        return getCam(camID).getMinExposureCompensation();
    }

    public static int getExposureCompensationMaxAndroid(int camID){
        return getCam(camID).getMaxExposureCompensation();
    }

    public static int getMaxNumExposureAreas(int camID){
        return getCam(camID).getMaxNumExposureAreas();
    }

    public static float[] getExposureAreasAexolGL(int camID){
        return getCam(camID).getExposureAreasAexolGL();
        //return new float[0];
    }

    public static int setExposureAreasAexolGL(float[] normAreas, int camID){
        return getCam(camID).setExposureAreasAexolGL(normAreas);
    }


    // new
    // 3 elements tab [min, optimal, far] distances in meters (depends on algorithm device etc, the error in values can be large, use only as refference (andr doc.)
    public static float[] getFocusDistances(int camID){
        return getCam(camID).getFocusDistances();
    }

    public static int getMaxNumFocusAreas(int camID){
        return getCam(camID).getMaxNumFocusAreas();
    }

    public static float[] getFocusAreasAexolGL(int camID){
        return getCam(camID).getFocusAreasAexolGL();
    }

    public static int setFocusAreasAexolGL(float[] normAreas, int camID){
        return getCam(camID).setFocusAreasAexolGL(normAreas);
    }

    // [minFPS, maxFPS]
    public static float[] getPreviewFPSRangeAexolGL(int camID){
        return getCam(camID).getPreviewFpsRangeAexolGL();
    }

    public static float[] getSupportedPreviewFPSRangesAexolGL(int camID){
        return getCam(camID).getSupportedPreviewFPSRangesAexolGL();
    }

    public static int setPreviewFPSRangeAexolGL(float minFPS, float maxFPS, int camID){
        return getCam(camID).setPreviewFpsRangeAexolGL(minFPS, maxFPS);
    }

    public static float getHorizontalViewAngle(int camID){
        return getCam(camID).getHorizontalViewAngle();
    }

    public static float getVerticalViewAngle(int camID){
        return getCam(camID).getVerticalViewAngle();
    }

    public static int takePicture(String fullPath, int width, int height, int format, int JPGQuality, int filter, int camID){
        return getCam(camID).takePicture(fullPath,  width,  height, format, JPGQuality, filter);
    }

    // TODO add more params for eg. format, quality etc.
    public static int startVideo(String fullPath, int width, int height, int camID){
        return getCam(camID).startVideo(fullPath, width, height);
    }

    public static int stopVideo(int camID){
        return getCam(camID).stopVideo();
    }

    public static int[] getSupportedVideoSizes(int camID) {
        return getCam(camID).getSupportedVideoSizes();
    }

     /* return in int arr format, "<WIDTH>:<HEIGHT> ", divided by spaces. 2 possible sizes eg. "400:300 800:600"  */
    public static int[] getSupportedPictureSizes(int camID){
        return getCam(camID).getSupportedPictureSizes();
    }

    public static int isSupportedPictureFormat(int format, int camID){
        return getCam(camID).isSupportedPictureFormat(format);
    }

    public static int setRotation(int rotation, int camID){
        return getCam(camID).setRotation(rotation);
    }

    public static String getFlatten(int camID){
        return getCam(camID).getFlatten();
    }

    public static String getUserDataDirectory(){
        return Directory.getUserDataDirectory();
    }

    public static String getDownloadCacheDirectory(){
        return Directory.getDownloadCacheDirectory();
    }

    public static String getExternalStorageDirectory(){
        return Directory.getExternalStorageDirectory();
    }

    public static String getExternalStoragePublicDirectory(int type){
        return Directory.getExternalStoragePublicDirectory(type);
    }

    public static int mkdir(String dirPath){
        return Directory.mkdir(dirPath);
    }

    private static Gallery gallery;

    public static int openGallery(String path){
//        File root = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM).getPath());
//        Uri uri = Uri.fromFile(root);
//
//        Intent intent = new Intent();
//        intent.setAction(android.content.Intent.ACTION_VIEW);
//        intent.setData(uri);
//        AexActivity.getAppContext().startActivity(intent);

        // WORKS:
        //String url = "http://www.google.com";
        //Intent i = new Intent(Intent.ACTION_VIEW);
        //i.setData(Uri.parse(url));
        //((Activity) AexActivity.getAppContext()).startActivity(i);

        // Invoke this on main UI thread
        ((Activity) AexActivity.getAppContext()).runOnUiThread(new Runnable() {
            @Override
            public void run() {
                // ACTION_PICK, ACTION_VIEW ... etc
                Intent i = new Intent(Intent.ACTION_VIEW, MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
                ((Activity) AexActivity.getAppContext()).startActivity(i);
//                Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(
//                        "content://media/internal/images/media"));
//                ((Activity) AexActivity.getAppContext()).startActivity(intent);
            }
        });
//        gallery = new Gallery(path);
//        int code = gallery.open();
//        return code;

        return Bridge.CONST_CAMERA_SUCCESS();
    }
}
