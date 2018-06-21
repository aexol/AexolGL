package com.aexolgl.android.camera;

import android.annotation.TargetApi;
import android.content.Context;
import android.graphics.Rect;
import android.hardware.Camera;
import android.graphics.ImageFormat;
import android.media.CamcorderProfile;
import android.media.MediaRecorder;
import android.os.Build;
import android.util.Log;
import android.view.Surface;
import android.view.WindowManager;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

/**
 * Created by darek on 24/03/15.
 */
class CameraDevice implements Camera.OnZoomChangeListener{
    @Override
    public void onZoomChange(int zoomValue, boolean stopped, Camera camera) {
        isSmoothZooming = stopped;

        if(stopped){
            NativeCameraLib.nativeCallbackSmoothZoomEnded(camID);
        }
    }

    private int camID;
    private  Camera camera = null;
    private  Camera.Parameters cameraParam = null;
    private  int lastOpenedCamID = -1;
    private  NativeCameraTextureProvider textureProvider = null;
    private  String errorMessage = new String("no error");
    private boolean isSmoothZooming = false;
    private int displayOrientation = 0;
    private boolean isRecording = false;
    private MediaRecorder mMediaRecorder = null;

    public static final int PICTURE_TYPE_JPG = 0x01;
    public static final int PICTURE_TYPE_PNG = 0x02;

    public static final int SUCCESS = 0x01;
    public static final int FAILURE = 0x00;
    public static final int NOT_ENOUGHT_MEMORY = 0x02;
    public static final int FILE_NOT_FOUND = 0x03;
    public static final int IO_EXCEPTION = 0x04;

    private HashMap<Integer, String> defaultParams = new HashMap<Integer, String>();

    public void onPause() {

        if(isRecording){
            stopVideo();
        }

        if (camera != null) {
                camera.stopPreview();

            camera.release();
            camera = null;
        }

        if (textureProvider != null) {
            textureProvider.onPause();
        }
    }


    public void onResume() {

    }

    public  void setErrorMesssage(String msg) {
        Bridge.setErrorMessage(msg);
    }

    public  String getErrorMessage(){
        return errorMessage;
    }

    public  void setIsJavaTextureOwner(boolean isOwner){
        textureProvider.setIsTextureOwner(isOwner);
    }

    public  int open(int cameraId) {
        if (camera != null) {
            return Bridge.CONST_CAMERA_IS_OPENED();
        }

        if ((cameraId >= 0) && (cameraId < Bridge.getNumberOfCameras())) {
            try{
                camera = Camera.open(cameraId);
            }
            catch(Exception e){
                errorMessage = new String(e.getMessage());
                Log.d(Bridge.LOG_TAGG, e.getMessage());
                return Bridge.CONST_CAMERA_NO_PERMISSION();
            }

            if (camera == null) {
                setErrorMesssage("Cant open camera now");
                return Bridge.CONST_CAMERA_OPENING_ERROR();
            }
            lastOpenedCamID = cameraId;
        } else {
            setErrorMesssage("No camera device with id: "+cameraId);
            return Bridge.CONST_CAMERA_NO_CAMERA_WITH_PROVIDED_ID();
        }

        camID = cameraId;
        cameraParam = camera.getParameters();
        cameraParam.setPreviewFormat(ImageFormat.NV21);

        // remember first default params for camera
        defaultParams.put(ParametersMapping.ANTIBANDING_DEFAULT, cameraParam.getAntibanding());
        defaultParams.put(ParametersMapping.WHITE_BALANCE_DEFAULT, cameraParam.getWhiteBalance());
        defaultParams.put(ParametersMapping.COLOR_EFFECT_DEFAULT,  cameraParam.getColorEffect());
        defaultParams.put(ParametersMapping.FLASH_MODE_DEFAULT,  cameraParam.getFlashMode());
        defaultParams.put(ParametersMapping.SCENE_MODE_DEFAULT,  cameraParam.getSceneMode());
        defaultParams.put(ParametersMapping.FOCUS_MODE_DEFAULT,  cameraParam.getFocusMode());
        defaultParams.put(ParametersMapping.TORCH_MODE_DEFAULT, cameraParam.getFlashMode());

        camera.setParameters(cameraParam);
        textureProvider = new NativeCameraTextureProvider(camera, this, cameraId); // it is invoking method nativeOnFrame... from NativeCameraLib
        return Bridge.CONST_CAMERA_SUCCESS();
    }

    public  int release() {
        if (camera != null) {
            textureProvider.onPause(); // deinit OpenGL components (dont delete texture, aexolGL will delete texture on hardware)
            camera.release();
            camera = null;
        }

        return Bridge.CONST_CAMERA_SUCCESS();
    }

    public  int updateTextureIfNeeded(){
        if(textureProvider.updateTextureIfNeeded()) // return true if texture changes
        {
            return Bridge.CONST_CAMERA_SUCCESS();
        }else{
            return Bridge.CONST_CAMERA_FAILURE();
        }
    }

    public  int getTextureID(){
        if(textureProvider != null)
            return textureProvider.getVideoTextureID();
        else
            return -1;
    }

    public int autoFocus(){
        camera.autoFocus(new Camera.AutoFocusCallback() {
            @Override
            public void onAutoFocus(boolean success, Camera camera) {
                NativeCameraLib.nativeCallbackAutoFocus(success, camID); // c++ callback
            }
        });

        return Bridge.CONST_CAMERA_SUCCESS();
    }

    public  int startPreview() {
        if (camera == null) {
            return Bridge.CONST_CAMERA_IS_NOT_OPENED();
        }

        camera.startPreview();

        return Bridge.CONST_CAMERA_SUCCESS();
    }

    public  int stopPreview() {
        if (camera != null) {
            camera.stopPreview();
            return Bridge.CONST_CAMERA_SUCCESS();
        }

        return Bridge.CONST_CAMERA_SUCCESS();
    }

    public int getAntibanding(){
        return strParamToAexolGLIntParam(camera.getParameters().getAntibanding());
    }

    public int getWhiteBalance(){
        return strParamToAexolGLIntParam(camera.getParameters().getWhiteBalance());
    }

    public int getColorEffect(){
        return strParamToAexolGLIntParam(camera.getParameters().getColorEffect());
    }

    public int getFlashMode(){
        int flashMode = strParamToAexolGLIntParam(camera.getParameters().getFlashMode());
        if(flashMode == ParametersMapping.TORCH_MODE_DEFAULT) flashMode = ParametersMapping.FLASH_MODE_DEFAULT;
        if(flashMode == ParametersMapping.TORCH_MODE_OFF) flashMode = ParametersMapping.FLASH_MODE_OFF;
        if(flashMode == ParametersMapping.TORCH_MODE_ON) flashMode = ParametersMapping.FLASH_MODE_ON;
        if(flashMode == ParametersMapping.TORCH_MODE_AUTO) flashMode = ParametersMapping.FLASH_MODE_AUTO;
        return flashMode;
    }

    public int getSceneMode(){
        return strParamToAexolGLIntParam(camera.getParameters().getSceneMode());
    }

    public int getFocusMode(){
        return strParamToAexolGLIntParam(camera.getParameters().getFocusMode());
    }

    public int[] getSupportedAntibanding(){
        return suppParamsToAexolGLSuppParams(cameraParam.getSupportedAntibanding());
    }

    public int[] getSupportedWhiteBalance(){
        return suppParamsToAexolGLSuppParams(cameraParam.getSupportedWhiteBalance());
    }

    public int[] getSupportedColorEffects(){
        return suppParamsToAexolGLSuppParams(cameraParam.getSupportedColorEffects());
    }

    public int[] getSupportedFlashModes(){
        return suppParamsToAexolGLSuppParams(cameraParam.getSupportedFlashModes());
    }

    // new, "PARAM1 PARAM2 PARAM3" eg. "101 104 106" - consts from camera parameters class
    public int[] getSupportedSceneModes(){
        return suppParamsToAexolGLSuppParams(cameraParam.getSupportedSceneModes());
    }

    public int[] getSupportedFocusModes(){
        return suppParamsToAexolGLSuppParams(cameraParam.getSupportedFocusModes());
    }



    public int getTorchMode(){
        int torchMode = strParamToAexolGLIntParam(camera.getParameters().getFlashMode());
        if(torchMode == ParametersMapping.FLASH_MODE_DEFAULT) torchMode = ParametersMapping.TORCH_MODE_DEFAULT;
        if(torchMode == ParametersMapping.FLASH_MODE_OFF) torchMode = ParametersMapping.TORCH_MODE_OFF;
        if(torchMode == ParametersMapping.FLASH_MODE_ON) torchMode = ParametersMapping.TORCH_MODE_ON;
        if(torchMode == ParametersMapping.FLASH_MODE_AUTO) torchMode = ParametersMapping.TORCH_MODE_AUTO;
        return torchMode;
    }

    public float getZoomAexolGL(){
        return andrZoomToAexolGLZoom(getZoom(), cameraParam.getZoomRatios());
    }

    public float getZoomMaxAexolGL(){
        return andrZoomToAexolGLZoom(getMaxZoom(), cameraParam.getZoomRatios());
    }

    public int setZoomAexolGL(float zoomFactor){
        if(isSmoothZooming)
            return Bridge.CONST_CAMERA_SUCCESS();

        int andrZoomId = aexolGLZoomToAndroidZoom(zoomFactor, cameraParam.getZoomRatios());
        return setZoom(andrZoomId);
    }

    public int setZoomSmoothAexolGL(float zoom){
        if(isSmoothZooming)
            return Bridge.CONST_CAMERA_SUCCESS();

        if(cameraParam.isSmoothZoomSupported()){
            int andrZoomId = aexolGLZoomToAndroidZoom(zoom, cameraParam.getZoomRatios());
            isSmoothZooming = true;
            camera.setZoomChangeListener(this);
            camera.startSmoothZoom(andrZoomId);
            return Bridge.CONST_CAMERA_SUCCESS();
        }else{
            return Bridge.CONST_CAMERA_FAILURE();
        }
    }

    // value from 1 to 100
    public int getZoom(){
        return camera.getParameters().getZoom();
    }

    // zoomRatios ???
    public int[] getZoomRatios(){
        return integersListToArray(camera.getParameters().getZoomRatios());
    }

    public int getMaxZoom(){
        return cameraParam.getMaxZoom();
    }

    // new, from 0 to getMaxZoom()
    public int setZoom(int zoom){
        if(zoom < 0) zoom = 0;
        if(zoom > cameraParam.getMaxZoom()) zoom = cameraParam.getMaxZoom();

        try{
            cameraParam.setZoom(zoom);
            camera.setParameters(cameraParam);
            return Bridge.CONST_PARAMETER_SUPPORTED();
        }catch(Exception e){
            errorMessage = new String("#--(!): \n"+e.getMessage());
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }
    }

    public int isSmoothlyZoomSupported(int camID){
        if(cameraParam.isSmoothZoomSupported()){
            return Bridge.CONST_CAMERA_SUCCESS();
        }else{
            return Bridge.CONST_CAMERA_FAILURE();
        }
    }

    // new
    public int getAutoWhiteBalanceLock(){
        if(cameraParam.getAutoWhiteBalanceLock())
            return Bridge.CONST_CAMERA_SUCCESS();
        else
            return Bridge.CONST_CAMERA_FAILURE();
    }

    // new
    public int setAutoWhiteBalanceLock(boolean param){
        try{
            cameraParam.setAutoWhiteBalanceLock(param);
            camera.setParameters(cameraParam);
            return Bridge.CONST_PARAMETER_SUPPORTED();
        }catch(Exception e){
            errorMessage = new String("#--(!): \n"+e.getMessage());
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }
    }

    // new
    public int getAutogetExposureAreasAexolGLLock(){
        if(cameraParam.getAutoExposureLock())
            return Bridge.CONST_CAMERA_SUCCESS();
        else
            return Bridge.CONST_CAMERA_FAILURE();
    }

    // new
    public int setAutoExposureLock(boolean param){
        try{
            cameraParam.setAutoExposureLock(param);
            camera.setParameters(cameraParam);
            return Bridge.CONST_PARAMETER_SUPPORTED();
        }catch(Exception e){
            errorMessage = new String("#--(!): "+e.getMessage());
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }
    }

    public int getMaxNumExposureAreas(){
        return camera.getParameters().getMaxNumMeteringAreas();
    }

    // float arr of areas: left,top,right,bottom,weight left2,top2,right2,bottom2,weight2
    public float[] getExposureAreasAexolGL(){
        try {
            List<Camera.Area> areas = camera.getParameters().getMeteringAreas(); // no nie, no po prostu kurwa no nie (throws exception bad int " 0" ... wtf? )
            return areasToNormalizeFloatArray(areas);
        }catch(Exception e){
            setErrorMesssage(e.getMessage());
            return new float[0];
        }
    }

    public int setExposureAreasAexolGL(float[] normAreas){
        try {
            List<Camera.Area> areas = normalizeFloatArrayToAreas(normAreas);
            return setExposureAreas(areas);
        }catch(Exception e){
            setErrorMesssage(e.getMessage());
            return Bridge.CONST_CAMERA_FAILURE();
        }
    }

    // new. areas in rect -1000, -1000 to 1000, 1000
    public int setExposureAreas(List<Camera.Area> areas){
        try{
            if(areas.size() > 0)
                cameraParam.setMeteringAreas(areas);
            else
                cameraParam.setMeteringAreas(null); // auto

            camera.setParameters(cameraParam);
            return Bridge.CONST_CAMERA_SUCCESS();
        }catch(Exception e){
            errorMessage = new String("#--(!): "+e.getMessage());
            return Bridge.CONST_CAMERA_FAILURE();
        }
    }

    // return value <= 0, if 0 - exposure compensation not supported
    public int getMinExposureCompensation(){
        return cameraParam.getMinExposureCompensation();
    }

    // return value >= 0, if 0 - exposure compensation not supported
    public int getMaxExposureCompensation(){
        return cameraParam.getMaxExposureCompensation();
    }

    public int getExposureCompensation(){
        return camera.getParameters().getExposureCompensation();
    }

    public float getExposureCompensationStep(){
        return cameraParam.getExposureCompensationStep();
    }

    // 0 - no exposure, range from getMinExposureCompensation() to getMaxExposureCompensation()
    public int setExposureCompensation(int compensation){
        try{
            if(cameraParam.getExposureCompensation() != compensation) {
                cameraParam.setExposureCompensation(compensation);
                camera.setParameters(cameraParam);
            }
                return Bridge.CONST_PARAMETER_SUPPORTED();
        }catch(Exception e){
            errorMessage = new String("#--(!): \n"+e.getMessage());
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }
    }

    public float getExposureEVAexolGL(){
        return getExposureCompensation() * getExposureCompensationStep();
    }

    public float getMinExposureEVAexolGL(){
        return getMinExposureCompensation()*getExposureCompensationStep();
    }

    public float getMaxExposureEVAexolGL(){
        return getMaxExposureCompensation()*getExposureCompensationStep();
    }

    public int setExposureEVAexolGL(float ev){
        int minID = getMinExposureCompensation();
        int maxID = getMaxExposureCompensation();
        float step = getExposureCompensationStep();

        int closestID = minID;
        float minDiff = Math.abs(ev - (minID*step));

        for(int i = minID ; i <= maxID ; i++){
            float tmpDiff = Math.abs(ev - (i*step));
            if(minDiff > tmpDiff){
                minDiff = tmpDiff;
                closestID = i;
            }
        }

        return setExposureCompensation(closestID);
    }

    public float getFocalLength(){
        return camera.getParameters().getFocalLength();
    }

    public int getMaxNumFocusAreas(){
        return camera.getParameters().getMaxNumFocusAreas();
    }

    // 3 float elems, nearDistance, optimalDistance, farDistance
    public float[] getFocusDistances(){
        float[] distances = new float[3];
        camera.getParameters().getFocusDistances(distances);

        float[] retDistances = new float[3];
        retDistances[0] = distances[Camera.Parameters.FOCUS_DISTANCE_NEAR_INDEX];
        retDistances[1] = distances[Camera.Parameters.FOCUS_DISTANCE_OPTIMAL_INDEX];
        retDistances[2] = distances[Camera.Parameters.FOCUS_DISTANCE_FAR_INDEX];

        return retDistances;
    }

    // new, areas  reprezentation: "left:top:right:bottom:weight left2:top2:right2:bottom2:weight2" etc... eg. "0.2:0.2:0.3:0.3:0.5"
    public float[] getFocusAreasAexolGL(){
        try {
            List<Camera.Area> areas = camera.getParameters().getFocusAreas();
            return areasToNormalizeFloatArray(areas);
        }catch(Exception e){
            setErrorMesssage("--(!): "+e.getMessage());
            return new float[0];
        }
    }

    public int setFocusAreasAexolGL(float[] areasNorm){
        try{
            return setFocusAreas(normalizeFloatArrayToAreas(areasNorm));
        }catch(Exception e){
            setErrorMesssage("--(!): "+e.getMessage());
            return Bridge.CONST_CAMERA_FAILURE();
        }
    }

    // new
    public int setFocusAreas(List<Camera.Area> areas){
        try{
            if(areas.size() > 0)
               cameraParam.setFocusAreas(areas);
            else
                cameraParam.setFocusAreas(null); // auto focus areas

            camera.setParameters(cameraParam);
            return Bridge.CONST_CAMERA_SUCCESS();
        }catch(Exception e){
            errorMessage = new String("#--(!): \n"+e.getMessage());
            return Bridge.CONST_CAMERA_FAILURE();
        }
    }

    public int isSupported(int param){
        return ParametersMapping.isSupported(param, camera, cameraParam);
    }

    public int setParameter(int param){
        return ParametersMapping.setParameter(param, camera, cameraParam, defaultParams);
    }

    public float[] getPreviewFpsRangeAexolGL(){
        int[] range = getPreviewFpsRange();
        float[] rangeFloat = new float[range.length];
        for(int i = 0 ; i < range.length ; i++){
            rangeFloat[i] = (float)range[i] / 1000.0f;
        }
        return rangeFloat;
    }

    // ret 2 values int array: [minFPS, maxFPS]
    public int[] getPreviewFpsRange(){
        int[] range = new int[2];
        camera.getParameters().getPreviewFpsRange(range);
        int[] retRange = new int[2];
        retRange[0] = range[Camera.Parameters.PREVIEW_FPS_MIN_INDEX];
        retRange[1] = range[Camera.Parameters.PREVIEW_FPS_MAX_INDEX];

        return retRange;
    }

    public float[] getSupportedPreviewFPSRangesAexolGL(){
        int[] range = getSupportedPreviewFPSRanges();
        float[] rangeFloat = new float[range.length];
        for(int i = 0 ; i < range.length ; i++){
            rangeFloat[i] = (float)range[i] / 1000.0f;
        }
        return rangeFloat;
    }

    // new "min:max min2:max2" etc..., eg. "10:30 20:40"
    public int[] getSupportedPreviewFPSRanges(){
        List<int[]> suppFPSRanges = camera.getParameters().getSupportedPreviewFpsRange();

        List<Integer> ranges = new ArrayList<Integer>();

        for(int[] range : suppFPSRanges){
            ranges.add(range[Camera.Parameters.PREVIEW_FPS_MIN_INDEX]);
            ranges.add(range[Camera.Parameters.PREVIEW_FPS_MAX_INDEX]);
        }

        int[] arr = new int[ranges.size()];
        for(int i = 0 ; i < arr.length ; i++){
            arr[i] = ranges.get(i);
        }

        return arr;
    }

    public int setPreviewFpsRangeAexolGL(float min, float max){
        return setPreviewFpsRange((int)(min*1000), (int)(max*1000));
    }

    // new
    public int setPreviewFpsRange(int min, int max){
        if(camera == null)
            return Bridge.CONST_CAMERA_IS_NOT_OPENED();
        try {
            cameraParam.setPreviewFpsRange(min, max);
            camera.setParameters(cameraParam);
            return Bridge.CONST_CAMERA_SUCCESS();
        }catch (Exception e){
            errorMessage = new String("#--(!): "+e.getMessage());
            return Bridge.CONST_CAMERA_FAILURE();
        }
    }

    // USING JUST A FORMAT FOR GL_TEXTURE_EXTERNAL_OS for handling in AexolGL
    public int[] getSupportedPreviewFormats(){
        return integersListToArray(cameraParam.getSupportedPreviewFormats());
    }

    // ret ImageType.XXX const value
    public int getPreviewFormat(){
        return camera.getParameters().getPreviewFormat();
    }

    public int[] getPreferredPreviewSizeForVideo(){
        List<Camera.Size> sizes = new ArrayList<Camera.Size>();
        sizes.add(camera.getParameters().getPreferredPreviewSizeForVideo());
        return sizesToIntArray(sizes);
    }

    /* return in string format, "<WIDTH>:<HEIGHT> ", divided by spaces. 2 possible sizes eg. "400:300 800:600"  */
    public  int[] getSupportedPreviewSizes(){
        return sizesToIntArray(camera.getParameters().getSupportedPreviewSizes());
    }

    public  int setPreviewSize(int width, int height){
        try {
            cameraParam.setPreviewSize(width, height);
            camera.setParameters(cameraParam);
            return Bridge.CONST_CAMERA_SUCCESS();
        }
        catch(Exception e){
            //Log.d(Bridge.LOG_TAGG, e.getMessage());
            errorMessage = new String("#--(!): \n"+e.getMessage());
            return Bridge.CONST_CAMERA_FAILURE();
        }
    }

    public float getHorizontalViewAngle(){
        float angle = camera.getParameters().getHorizontalViewAngle();
        return angle;
    }

    public float getVerticalViewAngle(){
        float angle = camera.getParameters().getVerticalViewAngle();
        return angle;
    }

    public  int setDisplayOrientation(int degrees){
        try {
            camera.setDisplayOrientation(degrees % 360);
            textureProvider.displayOrientationChanged();
            displayOrientation = degrees % 360;
            return Bridge.CONST_CAMERA_SUCCESS();
        }catch(Exception e){
            //Log.d(LOG_TAGG, e.getMessage());
            errorMessage = new String("#--(!): \n"+e.getMessage());
            return Bridge.CONST_CAMERA_FAILURE();
        }
    }

    // new
    public int setRotation(int degrees){
        if(camera == null)
            return Bridge.CONST_CAMERA_IS_NOT_OPENED();
        try{
            cameraParam.setRotation(degrees);
            camera.setParameters(cameraParam);
            return Bridge.CONST_CAMERA_SUCCESS();
        }catch(Exception e){
            errorMessage = new String("#--(!): \n"+e.getMessage());
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }
    }


    // return ImageType.XXX consts list which describes picture formats
    public int[] getSupportedPictureFormats(){
        return integersListToArray(cameraParam.getSupportedPictureFormats());
    }

    public int[] getSupportedPictureSizes(){
        return sizesToIntArray(cameraParam.getSupportedPictureSizes());
    }

    public int isSupportedPictureFormat(int format){
        if(PhotoHandler.isSupportedPictureFormat(format)){
            return Bridge.CONST_CAMERA_SUCCESS();
        }

        return Bridge.CONST_CAMERA_FAILURE();
    }

    // value from 1 (worst) to 100 (best)
    public int getJpegQuality(){
        return camera.getParameters().getJpegQuality();
    }

    // new, quality is number between 1 - 100 (lowest - best)
    public int setJpegQuality(int quality){
        if(quality < 1) quality = 1;
        if(quality > 100) quality = 100;

        try{
            cameraParam.setJpegQuality(quality);
            camera.setParameters(cameraParam);
            return Bridge.CONST_PARAMETER_SUPPORTED();
        }catch(Exception e){
            errorMessage = new String("#--(!): \n"+e.getMessage());
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }
    }

    // value from 1 (worst) to 100 (best)
    public int getJpegThumbnailQuality(){
        return camera.getParameters().getJpegThumbnailQuality();
    }

    // new, quality like above
    public int setJpegThumbnailQuality(int quality){
        if(quality < 1) quality = 1;
        if(quality > 100) quality = 100;

        try{
            cameraParam.setJpegThumbnailQuality(quality);
            camera.setParameters(cameraParam);
            return Bridge.CONST_PARAMETER_SUPPORTED();
        }catch(Exception e){
            errorMessage = new String("#--(!): \n"+e.getMessage());
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }
    }

    public int[] getSupportedJpegThumbnailSizes(){
        return sizesToIntArray(cameraParam.getSupportedJpegThumbnailSizes());
    }

    public int[] getJpegThumbnailSize(){
        List<Camera.Size> sizes = new ArrayList<Camera.Size>();
        sizes.add(camera.getParameters().getJpegThumbnailSize());
        return sizesToIntArray(sizes);
    }

    // new,
    public int setJpegThumbnailSize(int width, int height){
        try{
            cameraParam.setJpegThumbnailSize(width, height);
            camera.setParameters(cameraParam);
            return Bridge.CONST_PARAMETER_SUPPORTED();
        }catch(Exception e){
            errorMessage = new String("#--(!): \n"+e.getMessage());
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }
    }

    // return ImageType.XXX const
    public int getPictureFormat(){
        return camera.getParameters().getPictureFormat();
    }

    // new, Image.XXX constant as parameter
    public int setPictureFormat(int Image_format){
        try{
            cameraParam.setPictureFormat(Image_format);
            camera.setParameters(cameraParam);
            return Bridge.CONST_PARAMETER_SUPPORTED();
        }catch(Exception e){
            errorMessage = new String("#--(!): \n"+e.getMessage());
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }
    }

    public int[] getPictureSize(){
        List<Camera.Size> sizes = new ArrayList<Camera.Size>();
        sizes.add(camera.getParameters().getPictureSize());
        return sizesToIntArray(sizes);
    }

    // new
    public int setPictureSize(int width, int height){
        try{
            cameraParam.setPictureSize(width, height);
            camera.setParameters(cameraParam);
            return Bridge.CONST_PARAMETER_SUPPORTED();
        }catch(Exception e){
            errorMessage = new String("#--(!): \n"+e.getMessage());
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }
    }

    // new -
    /*his tells the camera that the intent of the application is to record videos start(), not to take still pictures takePicture(Camera.ShutterCallback, Camera.PictureCallback, Camera.PictureCallback, Camera.PictureCallback). Using this hint can allow MediaRecorder.start() to start faster or with fewer glitches on output. This should be called before starting preview for the best result, but can be changed while the preview is active. The default value is false. The app can still call takePicture() when the hint is true or call MediaRecorder.start() when the hint is false. But the performance may be worse.*/
    public int setRecordingHint(boolean param){
        try{
            cameraParam.setRecordingHint(param);
            camera.setParameters(cameraParam);
            return Bridge.CONST_PARAMETER_SUPPORTED();
        }catch(Exception e){
            errorMessage = new String("#--(!): \n"+e.getMessage());
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }
    }

    @TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1)
    public int getVideoStabilization(){
        if(android.os.Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1)
            if(cameraParam.getVideoStabilization())
                return Bridge.CONST_CAMERA_SUCCESS();
            else
                return Bridge.CONST_CAMERA_FAILURE();

        return Bridge.CONST_CAMERA_FAILURE();
    }

    // new
    @TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1)
    public int setVideoStabilization(boolean stabilize){
        try{
            // if android api >= 15
            if(android.os.Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1) {
                cameraParam.setVideoStabilization(stabilize);
                camera.setParameters(cameraParam);
            }
            return Bridge.CONST_PARAMETER_SUPPORTED();
        }catch(Exception e){
            errorMessage = new String("#--(!): \n"+e.getMessage());
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }
    }

    // new, "width:height"
    public int[] getPreviewSize(){
        List<Camera.Size> sizes = new ArrayList<Camera.Size>();
        sizes.add(camera.getParameters().getPreviewSize());
        return sizesToIntArray(sizes);
    }

    public int takePicture(String fullPath, int width, int height, int format, int JPGQuality, int filter){
        List<Camera.Size> sizes = cameraParam.getSupportedPictureSizes();
        boolean isSuppSize = false;
        for(Camera.Size s : sizes){
            if(s.width == width)
                if(s.height == height){
                    isSuppSize = true;
                    break;
                }
        }

        if( ! isSuppSize){
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }

        cameraParam.setPictureFormat(ImageFormat.JPEG);
        cameraParam.setPictureSize(width, height);


        if(format == PhotoHandler.FORMAT_JPG) {
            cameraParam.setJpegQuality(JPGQuality);
        }
        else{
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }

        setErrorMesssage("width: " + width + ", height: " + height);

        try {
            camera.setParameters(cameraParam);
        }
        catch(Exception e){
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }

        PhotoHandler photoHandler = null;
        try {
            photoHandler = new PhotoHandler(fullPath, format, filter, camID);
        } catch (Exception e) {
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }

        camera.takePicture(new ShutterHandler(camID), null, photoHandler);

        return Bridge.CONST_CAMERA_SUCCESS();
    }


    private int prepareMediaRecorder(String fullPath, int width, int height){
        //mCamera = getCameraInstance();
        mMediaRecorder = new MediaRecorder();

        // Krok 1. Odblokuj kamere i ustaw ja dla media recordera
        camera.unlock();
        mMediaRecorder.setCamera(camera);

        // Krok 2. Ustaw zrodla dzwieku i obrazu
        mMediaRecorder.setAudioSource(MediaRecorder.AudioSource.CAMCORDER);
        mMediaRecorder.setVideoSource(MediaRecorder.VideoSource.CAMERA);

        //Krok 3. Powyzej API 8 ustaw CamProfile (jakosc ogolna ?)
        int quality = sizeToCamcorderProfileQuality(width, height);
        mMediaRecorder.setProfile(CamcorderProfile.get(camID, quality));

        // Krok 4 Ustawienie pliku wyjsciowego dla obrazu z kamery
        mMediaRecorder.setOutputFile(fullPath);

        // Krok 5. Ustawienie widoku preview (wyjscia dla obrazu aktualnego camery)
        //mMediaRecorder.setPreviewDisplay(new Surface(textureProvider.getVideoTexture())); // not null - change camera surface to new - this in param will be to display
        mMediaRecorder.setPreviewDisplay(null); // null - not change camera preview surface

        mMediaRecorder.setVideoFrameRate(30);

        // Krok 6. Przygotowanie skonfigurowanego media recordera
        try{
            mMediaRecorder.prepare();
            Thread.sleep(500);
            //camera.lock();
            //camera.startPreview();
        }catch (IllegalStateException exc){
            setErrorMesssage(exc.getMessage());
            return Bridge.CONST_CAMERA_FAILURE();
        }catch(IOException exc){
            setErrorMesssage(exc.getMessage());
            return Bridge.CONST_CAMERA_FAILURE();
        }catch(Exception exc){
            setErrorMesssage(exc.getMessage());
            return Bridge.CONST_CAMERA_FAILURE();
        }

        return Bridge.CONST_CAMERA_SUCCESS();
    }

    private void releaseMediaRecorder(){
        if (mMediaRecorder != null) {
            mMediaRecorder.reset();   // clear recorder configuration
            mMediaRecorder.release(); // release the recorder object
            mMediaRecorder = null;
            camera.lock();           // lock camera for later use
        }
    }

    public int startVideo(String fullPath, int width, int height){
        if(isRecording){
            return Bridge.CONST_CAMERA_FAILURE();
        }
        int code = Bridge.CONST_CAMERA_FAILURE();

        try {
             code = prepareMediaRecorder(fullPath, width, height);
        }catch(Exception e){
            setErrorMesssage(e.getMessage());
        }

        if(code == Bridge.CONST_CAMERA_SUCCESS()){
            try {
                mMediaRecorder.start();
            }catch(Exception e){
                setErrorMesssage(e.getMessage());
            }
            isRecording = true;
            return code;
        }else{
            releaseMediaRecorder();
            return code;
        }
    }

    public int stopVideo(){
        if(isRecording){
            try {
                if (mMediaRecorder != null)
                    mMediaRecorder.stop();

                releaseMediaRecorder();
            }catch(Exception e){
                setErrorMesssage(e.getMessage());
            }
            isRecording = false;
            NativeCameraLib.nativeCallbackStopVideo(CameraDevice.SUCCESS, camID); // manually in stopVideo cpp method
            return Bridge.CONST_CAMERA_SUCCESS();
        }

        return Bridge.CONST_CAMERA_SUCCESS();
    }

    // [width1,height1,width2,height2...]
    public int[] getSupportedVideoSizes(){
        int i = 0;
        int[] sizes = new int [6];

        int[] tab;
        tab = camcorderProfileQualityToSize(CamcorderProfile.QUALITY_HIGH);
        sizes[i++] = tab[0];
        sizes[i++] = tab[1];
        tab = camcorderProfileQualityToSize(CamcorderProfile.QUALITY_QVGA);
        sizes[i++] = tab[0];
        sizes[i++] = tab[1];
        tab = camcorderProfileQualityToSize(CamcorderProfile.QUALITY_LOW);
        sizes[i++] = tab[0];
        sizes[i++] = tab[1];

        return sizes;
    }

    public int sizeToCamcorderProfileQuality(int width, int height){

        if(width == 1920 && height == 1080){
            return CamcorderProfile.QUALITY_HIGH;
        }
        if(width == 1280 && height == 720){
            return CamcorderProfile.QUALITY_QVGA;
        }
        if(width == 720 && height == 480){
            return CamcorderProfile.QUALITY_LOW;
        }

        // default
        return CamcorderProfile.QUALITY_HIGH;
    }

    public int[] camcorderProfileQualityToSize(int code){
        int[] tab = new int[2];
        switch(code){
            case CamcorderProfile.QUALITY_HIGH:
                tab[0] = 1920;
                tab[1] = 1080;
                break;
            case CamcorderProfile.QUALITY_QVGA:
                tab[0] = 1280;
                tab[1] = 720;
                break;
            case CamcorderProfile.QUALITY_LOW:
                tab[0] = 720;
                tab[1] = 480;
                break;
            default:
                tab[0] = 0; // error
                tab[1] = 0;
        }

        return tab;
    }

    public String getFlatten(){
        return camera.getParameters().flatten();
    }

    /* helping functions */
    private int[] integersListToArray(List<Integer> intsList){
        int[] arr = new int[intsList.size()];
        for(int i = 0 ; i < arr.length ; i++) {
            arr[i] = intsList.get(i);
        }
        return arr;
    }

    private int[] sizesToIntArray(List<Camera.Size> sizes){
        List<Integer> list = new ArrayList<Integer>();

        for(Camera.Size s : sizes){
            list.add(s.width);
            list.add(s.height);
        }

        int[] sizesArr = new int[list.size()];
        for(int i = 0 ; i < sizesArr.length ; i++){
            sizesArr[i] = list.get(i);
        }

        return sizesArr;
    }

    private int[] suppParamsToAexolGLSuppParams(List<String> params){
        List<Integer> intParams = new ArrayList<Integer>();

        for(String s : params){
            Integer key = ParametersMapping.getIntParamKeyOf(s);
            if(key != null)
                intParams.add(key);
        }

        int[] intParamsArr = new int[intParams.size()];
        for(int i = 0 ; i < intParamsArr.length ; i++) intParamsArr[i] = intParams.get(i);

        return intParamsArr;
    }

    private List<Camera.Area> normalizeFloatArrayToAreas(float[] normAreas) throws Exception {
        List<Camera.Area> areas = new ArrayList<Camera.Area>();
        if(normAreas != null) {
            if(normAreas.length % 5 != 0)
                throw new Exception("normAreas length must be a multiplication of 5 (5, 10, 15 etc...)");

            for (int i = 0; i < normAreas.length; i += 5) {
                Rect r = new Rect();

                float normLeft = normAreas[i];
                float normTop = normAreas[i + 1];
                float normRight = normAreas[i + 2];
                float normBottom = normAreas[i + 3];
                float normWeight = normAreas[i + 4];

                r.left = (int) (normLeft * 2000.0f - 1000);
                r.top = (int) (normTop * 2000.0f - 1000);

                r.right = (int) (normRight * 2000.0f - 1000);
                r.bottom = (int) (normBottom * 2000.0f - 1000);
                int weight = (int) (normWeight * 1000.0f);

                areas.add(new Camera.Area(r, weight));
            }
        }

        return areas;
    }

    private float[] areasToNormalizeFloatArray(List<Camera.Area> areas){
            List<Float> normAreas = new ArrayList<Float>();

            if (areas != null) {
                    for (Camera.Area area : areas) {
                        int left = area.rect.left;  // max left top -1000, -1000, max right bottom 1000, 1000 (from android documentation)
                        int top = area.rect.top;

                        int right = area.rect.right;
                        int bottom = area.rect.bottom;
                        int weight = area.weight; // from 1 to 1000 (android documentation)

                        // mapping to normalize area 0,0 to 1,1 (floats)
                        float normLeft = ((float) left + 1000) / 2000.0f;
                        float normTop = ((float) top + 1000) / 2000.0f;
                        float normRight = ((float) right + 1000) / 2000.0f;
                        float normBottom = ((float) bottom + 1000) / 2000.0f;
                        float normWeight = (float) weight / 1000.0f;

                        normAreas.add(normLeft);
                        normAreas.add(normTop);
                        normAreas.add(normRight);
                        normAreas.add(normBottom);
                        normAreas.add(normWeight);
                    }
                    float[] areasFloat = new float[normAreas.size()];
                    for (int i = 0; i < areasFloat.length; i++) {
                        areasFloat[i] = normAreas.get(i);
                    }
                    return areasFloat;
            }

        return new float[0];
    }

    private int strParamToAexolGLIntParam(String param){
        Integer key = ParametersMapping.getIntParamKeyOf(param);

        if(key != null)
            return key.intValue();
        else
            return -1;
    }

    private float andrZoomToAexolGLZoom(int andrZoom, List<Integer> zoomRatios ){
        if(andrZoom >= 0 && andrZoom < zoomRatios.size()){
            return zoomRatios.get(andrZoom) / 100.f;
        }else{
            return 1.0f;
        }
    }

    private int aexolGLZoomToAndroidZoom(float aexolZoom, List<Integer> zoomRatios){
        int closestID = 0;
        float minDifference;
        minDifference = Math.abs((zoomRatios.get(0) / 100.0f) - aexolZoom);

        for(int i = 0 ; i < zoomRatios.size() ; i++){
            float tmpDiff = Math.abs((zoomRatios.get(i) / 100.0f) - aexolZoom);
            if(tmpDiff < minDifference){
                minDifference = tmpDiff;
                closestID = i;
            }
        }

        return closestID;
    }
}

