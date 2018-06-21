package com.aexolgl.android.camera;

import android.hardware.Camera;
import android.util.Log;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by darek on 05.03.15.
 */
public class ParametersMapping {

    public static final String DEFAULT = "default";

    public static final int RANGE = 100;
    // Values for antibanding settings.
    public static final int ANTIBANDING_DEFAULT = (RANGE * 1);
    public static final int ANTIBANDING_AUTO = (RANGE * 1) + 1;
    public static final int ANTIBANDING_50HZ = (RANGE * 1) + 2;
    public static final int ANTIBANDING_60HZ = (RANGE * 1) + 3;
    public static final int ANTIBANDING_OFF = (RANGE * 1) + 4;

    // Values for white balance settings.
    public static final int WHITE_BALANCE_DEFAULT = (RANGE * 2);
    public static final int WHITE_BALANCE_AUTO = (RANGE * 2) + 1;
    public static final int WHITE_BALANCE_INCANDESCENT = (RANGE * 2) + 2;
    public static final int WHITE_BALANCE_FLUORESCENT = (RANGE * 2) + 3;
    public static final int WHITE_BALANCE_WARM_FLUORESCENT = (RANGE * 2) + 4;
    public static final int WHITE_BALANCE_DAYLIGHT = (RANGE * 2) + 5;
    public static final int WHITE_BALANCE_CLOUDY_DAYLIGHT = (RANGE * 2) + 6;
    public static final int WHITE_BALANCE_TWILIGHT = (RANGE * 2) + 7;
    public static final int WHITE_BALANCE_SHADE = (RANGE * 2) + 8;
    public static final int WHITE_BALANCE_LOCKED = (RANGE * 2) + 9;
    public static final int WHITE_BALANCE_CONTINUOUS_AUTO = (RANGE * 2) + 10;

    // Values for color effect settings.
    public static final int COLOR_EFFECT_DEFAULT = (RANGE * 3);
    public static final int COLOR_EFFECT_NONE = (RANGE * 3) + 1;
    public static final int COLOR_EFFECT_MONO = (RANGE * 3) + 2;
    public static final int COLOR_EFFECT_NEGATIVE = (RANGE * 3) + 3;
    public static final int COLOR_EFFECT_SOLARIZE = (RANGE * 3) + 4;
    public static final int COLOR_EFFECT_SEPIA = (RANGE * 3) + 5;
    public static final int COLOR_EFFECT_POSTERIZE = (RANGE * 3) + 6;
    public static final int COLOR_EFFECT_WHITEBOARD = (RANGE * 3) + 7;
    public static final int COLOR_EFFECT_BLACKBOARD = (RANGE * 3) + 8;
    public static final int COLOR_EFFECT_AQUA = (RANGE * 3) + 9;

    // Values flash mode
    public static final int FLASH_MODE_DEFAULT = (RANGE * 4);
    public static final int FLASH_MODE_OFF = (RANGE * 4) + 1;
    public static final int FLASH_MODE_ON = (RANGE * 4) + 2;
    public static final int FLASH_MODE_AUTO = (RANGE * 4) + 3;
    public static final int FLASH_MODE_RED_EYE = (RANGE * 4) + 4;

    // Keys scene mode
    public static final int SCENE_MODE_DEFAULT = (RANGE * 5);
    public static final int SCENE_MODE_AUTO = (RANGE * 5) + 1;
    public static final int SCENE_MODE_ACTION = (RANGE * 5) + 2;
    public static final int SCENE_MODE_PORTRAIT = (RANGE * 5) + 3;
    public static final int SCENE_MODE_LANDSCAPE = (RANGE * 5) + 4;
    public static final int SCENE_MODE_NIGHT = (RANGE * 5) + 5;
    public static final int SCENE_MODE_NIGHT_PORTRAIT = (RANGE * 5) + 6;
    public static final int SCENE_MODE_THEATRE = (RANGE * 5) + 7;
    public static final int SCENE_MODE_BEACH = (RANGE * 5) + 8;
    public static final int SCENE_MODE_SNOW = (RANGE * 5) + 9;
    public static final int SCENE_MODE_SUNSET = (RANGE * 5) + 10;
    public static final int SCENE_MODE_STEADYPHOTO = (RANGE * 5) + 11;
    public static final int SCENE_MODE_FIREWORKS = (RANGE * 5) + 12;
    public static final int SCENE_MODE_SPORTS = (RANGE * 5) + 13;
    public static final int SCENE_MODE_PARTY = (RANGE * 5) + 14;
    public static final int SCENE_MODE_CANDLELIGHT = (RANGE * 5) + 15;
    public static final int SCENE_MODE_BARCODE = (RANGE * 5) + 16;
    public static final int SCENE_MODE_HDR = (RANGE * 5) + 17;

    // Focus values
    public static final int FOCUS_MODE_DEFAULT = (RANGE * 6);
    public static final int FOCUS_MODE_AUTO = (RANGE * 6) + 1;
    public static final int FOCUS_MODE_INFINITY = (RANGE * 6) + 2;
    public static final int FOCUS_MODE_MACRO = (RANGE * 6) + 3;
    public static final int FOCUS_MODE_FIXED = (RANGE * 6) + 4;
    public static final int FOCUS_MODE_EDOF = (RANGE * 6) + 5;
    public static final int FOCUS_MODE_CONTINUOUS_VIDEO = (RANGE * 6) + 6;
    public static final int FOCUS_MODE_CONTINUOUS_PICTURE = (RANGE * 6) + 7;

    // Torch values
    public static final int TORCH_MODE_DEFAULT = (RANGE * 7);
    public static final int TORCH_MODE_OFF = (RANGE * 7) + 1;
    public static final int TORCH_MODE_ON = (RANGE * 7) + 2;
    public static final int TORCH_MODE_AUTO = (RANGE * 7) + 3;

    // Exposure values
    public static final int EXPOSURE_MODE_DEFAULT = (RANGE * 8);
    public static final int EXPOSURE_MODE_AUTO = (RANGE * 8) + 1;
    public static final int EXPOSURE_MODE_FIXED = (RANGE * 8) + 2;
    public static final int EXPOSURE_MODE_CONTINUOUS_AUTO_EXPOSURE = (RANGE * 8) + 3;
    public static final int EXPOSURE_MODE_CUSTOM = (RANGE * 8) + 4;

    private static HashMap<Integer, String> aexToCameraParamMap = new HashMap<Integer, String>();

    static{
        aexToCameraParamMap.put(ANTIBANDING_DEFAULT, DEFAULT);
        aexToCameraParamMap.put(ANTIBANDING_AUTO, Camera.Parameters.ANTIBANDING_AUTO);
        aexToCameraParamMap.put(ANTIBANDING_50HZ, Camera.Parameters.ANTIBANDING_50HZ);
        aexToCameraParamMap.put(ANTIBANDING_60HZ, Camera.Parameters.ANTIBANDING_60HZ);
        aexToCameraParamMap.put(ANTIBANDING_OFF, Camera.Parameters.ANTIBANDING_OFF);

        aexToCameraParamMap.put(WHITE_BALANCE_DEFAULT, DEFAULT);
        aexToCameraParamMap.put(WHITE_BALANCE_AUTO, Camera.Parameters.WHITE_BALANCE_AUTO);
        aexToCameraParamMap.put(WHITE_BALANCE_INCANDESCENT, Camera.Parameters.WHITE_BALANCE_INCANDESCENT);
        aexToCameraParamMap.put(WHITE_BALANCE_FLUORESCENT, Camera.Parameters.WHITE_BALANCE_FLUORESCENT);
        aexToCameraParamMap.put(WHITE_BALANCE_WARM_FLUORESCENT, Camera.Parameters.WHITE_BALANCE_WARM_FLUORESCENT);
        aexToCameraParamMap.put(WHITE_BALANCE_DAYLIGHT, Camera.Parameters.WHITE_BALANCE_DAYLIGHT);
        aexToCameraParamMap.put(WHITE_BALANCE_CLOUDY_DAYLIGHT, Camera.Parameters.WHITE_BALANCE_CLOUDY_DAYLIGHT);
        aexToCameraParamMap.put(WHITE_BALANCE_TWILIGHT, Camera.Parameters.WHITE_BALANCE_TWILIGHT);
        aexToCameraParamMap.put(WHITE_BALANCE_SHADE, Camera.Parameters.WHITE_BALANCE_SHADE);
        aexToCameraParamMap.put(WHITE_BALANCE_LOCKED, "method-setted");
        aexToCameraParamMap.put(WHITE_BALANCE_CONTINUOUS_AUTO, Camera.Parameters.WHITE_BALANCE_AUTO);

        aexToCameraParamMap.put(COLOR_EFFECT_DEFAULT, DEFAULT);
        aexToCameraParamMap.put(COLOR_EFFECT_NONE, Camera.Parameters.EFFECT_NONE);
        aexToCameraParamMap.put(COLOR_EFFECT_MONO, Camera.Parameters.EFFECT_MONO);
        aexToCameraParamMap.put(COLOR_EFFECT_NEGATIVE, Camera.Parameters.EFFECT_NEGATIVE);
        aexToCameraParamMap.put(COLOR_EFFECT_SOLARIZE, Camera.Parameters.EFFECT_SOLARIZE);
        aexToCameraParamMap.put(COLOR_EFFECT_SEPIA, Camera.Parameters.EFFECT_SEPIA);
        aexToCameraParamMap.put(COLOR_EFFECT_POSTERIZE, Camera.Parameters.EFFECT_POSTERIZE);
        aexToCameraParamMap.put(COLOR_EFFECT_WHITEBOARD, Camera.Parameters.EFFECT_WHITEBOARD);
        aexToCameraParamMap.put(COLOR_EFFECT_BLACKBOARD, Camera.Parameters.EFFECT_BLACKBOARD);
        aexToCameraParamMap.put(COLOR_EFFECT_AQUA, Camera.Parameters.EFFECT_AQUA);

        aexToCameraParamMap.put(FLASH_MODE_DEFAULT, DEFAULT);
        aexToCameraParamMap.put(FLASH_MODE_OFF, Camera.Parameters.FLASH_MODE_OFF);
        aexToCameraParamMap.put(FLASH_MODE_ON, Camera.Parameters.FLASH_MODE_ON);
        aexToCameraParamMap.put(FLASH_MODE_AUTO, Camera.Parameters.FLASH_MODE_AUTO);
        aexToCameraParamMap.put(FLASH_MODE_RED_EYE, Camera.Parameters.FLASH_MODE_RED_EYE);
       // aexToCameraParamMap.put(FLASH_MODE_TORCH, Camera.Parameters.FLASH_MODE_TORCH);

        aexToCameraParamMap.put(SCENE_MODE_DEFAULT, DEFAULT);
        aexToCameraParamMap.put(SCENE_MODE_AUTO, Camera.Parameters.SCENE_MODE_AUTO);
        aexToCameraParamMap.put(SCENE_MODE_ACTION, Camera.Parameters.SCENE_MODE_ACTION);
        aexToCameraParamMap.put(SCENE_MODE_PORTRAIT, Camera.Parameters.SCENE_MODE_PORTRAIT);
        aexToCameraParamMap.put(SCENE_MODE_LANDSCAPE, Camera.Parameters.SCENE_MODE_LANDSCAPE);
        aexToCameraParamMap.put(SCENE_MODE_NIGHT, Camera.Parameters.SCENE_MODE_NIGHT);
        aexToCameraParamMap.put(SCENE_MODE_NIGHT_PORTRAIT, Camera.Parameters.SCENE_MODE_NIGHT_PORTRAIT);
        aexToCameraParamMap.put(SCENE_MODE_THEATRE, Camera.Parameters.SCENE_MODE_THEATRE);
        aexToCameraParamMap.put(SCENE_MODE_BEACH, Camera.Parameters.SCENE_MODE_BEACH);
        aexToCameraParamMap.put(SCENE_MODE_SNOW, Camera.Parameters.SCENE_MODE_SNOW);
        aexToCameraParamMap.put(SCENE_MODE_SUNSET, Camera.Parameters.SCENE_MODE_SUNSET);
        aexToCameraParamMap.put(SCENE_MODE_STEADYPHOTO, Camera.Parameters.SCENE_MODE_STEADYPHOTO);
        aexToCameraParamMap.put(SCENE_MODE_FIREWORKS, Camera.Parameters.SCENE_MODE_FIREWORKS);
        aexToCameraParamMap.put(SCENE_MODE_SPORTS, Camera.Parameters.SCENE_MODE_SPORTS);
        aexToCameraParamMap.put(SCENE_MODE_PARTY, Camera.Parameters.SCENE_MODE_PARTY);
        aexToCameraParamMap.put(SCENE_MODE_CANDLELIGHT, Camera.Parameters.SCENE_MODE_CANDLELIGHT);
        aexToCameraParamMap.put(SCENE_MODE_BARCODE, Camera.Parameters.SCENE_MODE_BARCODE);
        //aexToCameraParamMap.put(SCENE_MODE_HDR, Camera.Parameters.SCENE_MODE_HDR);
        aexToCameraParamMap.put(SCENE_MODE_HDR, "hdr"); // TODO - dodac sprytniejsze rozkminianie, czy parametr istnieje

        aexToCameraParamMap.put(FOCUS_MODE_DEFAULT, DEFAULT);
        aexToCameraParamMap.put(FOCUS_MODE_AUTO, Camera.Parameters.FOCUS_MODE_AUTO);
        aexToCameraParamMap.put(FOCUS_MODE_INFINITY, Camera.Parameters.FOCUS_MODE_INFINITY);
        aexToCameraParamMap.put(FOCUS_MODE_MACRO, Camera.Parameters.FOCUS_MODE_MACRO);
        aexToCameraParamMap.put(FOCUS_MODE_FIXED, Camera.Parameters.FOCUS_MODE_FIXED);
        aexToCameraParamMap.put(FOCUS_MODE_EDOF, Camera.Parameters.FOCUS_MODE_EDOF);
        aexToCameraParamMap.put(FOCUS_MODE_CONTINUOUS_VIDEO, Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
        aexToCameraParamMap.put(FOCUS_MODE_CONTINUOUS_PICTURE, Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);

        aexToCameraParamMap.put(TORCH_MODE_DEFAULT, DEFAULT);
        aexToCameraParamMap.put(TORCH_MODE_OFF, Camera.Parameters.FLASH_MODE_OFF);
        aexToCameraParamMap.put(TORCH_MODE_ON, Camera.Parameters.FLASH_MODE_TORCH); // torch mode is in iOS
        aexToCameraParamMap.put(TORCH_MODE_AUTO, Camera.Parameters.FLASH_MODE_TORCH); // torch mode is in iOS

        aexToCameraParamMap.put(EXPOSURE_MODE_DEFAULT, DEFAULT);
        aexToCameraParamMap.put(EXPOSURE_MODE_AUTO, "method-setted");
        aexToCameraParamMap.put(EXPOSURE_MODE_FIXED, "method-setted");
        aexToCameraParamMap.put(EXPOSURE_MODE_CONTINUOUS_AUTO_EXPOSURE, "method-setted");
        aexToCameraParamMap.put(EXPOSURE_MODE_CUSTOM, "method-setted");
    }

    private static String aexolKeyToCameraKey(int param){
        String retVal = aexToCameraParamMap.get(param);
        if(retVal == null)
            retVal = "";
        return retVal;
    }

    private static <T, E> T getKeyByValue(Map<T, E> map, E value) {
        for (Map.Entry<T, E> entry : map.entrySet()) {
            if (value.equals(entry.getValue())) {
                return entry.getKey();
            }
        }
        return null;
    }

    public static Integer getIntParamKeyOf(String value){
        return getKeyByValue(aexToCameraParamMap, value);
    }

    private static boolean isType(int param, int first){
        first = ((int)first/(int)RANGE)*RANGE;
        return ( (param >= first) && (param < (first+RANGE)) ) ? true : false;
    }

    public static int setParameter(int param, Camera camera, Camera.Parameters cameraParam, HashMap<Integer, String> defaultParams){
        if(camera == null)
            return Bridge.CONST_CAMERA_IS_NOT_OPENED();
        try{
        if(isSupported(param, camera, cameraParam) == Bridge.CONST_PARAMETER_NOT_SUPPORTED()){
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }

        String key = aexolKeyToCameraKey(param);

        if(key.equals(DEFAULT)){
            key = defaultParams.get(param); // remembered param when camera were opening
        }

            Camera.Parameters params = cameraParam;

            if(isType(param, ANTIBANDING_DEFAULT)){
                params.setAntibanding(key);
            }

            if(isType(param, WHITE_BALANCE_DEFAULT)){
                if(param != WHITE_BALANCE_LOCKED)
                    params.setWhiteBalance(key); //Changing the white balance mode with setWhiteBalance(String) will release the auto-white balance lock if it is set.
                else{
                    if(params.isAutoWhiteBalanceLockSupported()){
                        params.setAutoWhiteBalanceLock(true);
                    }else{
                        return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
                    }
                }
            }

            if(isType(param, COLOR_EFFECT_DEFAULT)){
                params.setColorEffect(key);
            }

            if(isType(param, FLASH_MODE_DEFAULT)){
                if(cameraParam.getFlashMode() != null)
                    params.setFlashMode(key);
            }

            if(isType(param, SCENE_MODE_DEFAULT)){
                params.setSceneMode(key);
            }

            if(isType(param, FOCUS_MODE_DEFAULT)){
                params.setFocusMode(key);
            }

            if(isType(param, TORCH_MODE_DEFAULT)){
                if(cameraParam.getFlashMode() != null)
                    params.setFlashMode(key); // torch mode is in flash mode on android
            }

            if(isType(param, EXPOSURE_MODE_DEFAULT)){
                if(param == EXPOSURE_MODE_FIXED){
                    if( ! params.isAutoExposureLockSupported())
                        return Bridge.CONST_PARAMETER_NOT_SUPPORTED();

                    params.setAutoExposureLock(true);
                }
                if(param == EXPOSURE_MODE_AUTO){
                    params.setAutoExposureLock(false);
                    // params.setMeteringAreas(null); // auto areas
                }
                if(param == EXPOSURE_MODE_CONTINUOUS_AUTO_EXPOSURE){
                    params.setAutoExposureLock(false);
                    // params.setMeteringAreas(null); // auto areas
                }
                if(param == EXPOSURE_MODE_CUSTOM){
                    //return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
                    if( ! params.isAutoExposureLockSupported()) {
                        Log.v(Bridge.LOG_TAGG, "exposure mode custom is NOT supported");
                        return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
                    }

                    Log.v(Bridge.LOG_TAGG, "exposure mode custom supported");
                    params.setAutoExposureLock(true);
                }
            }

            camera.setParameters(params); // can throw exception

        }catch(Exception e){
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }

        return Bridge.CONST_PARAMETER_SUPPORTED();
    }

    public static int isSupported(int param, Camera camera, Camera.Parameters cameraParam){
        if(camera == null)
            return Bridge.CONST_CAMERA_IS_NOT_OPENED();

        String key = aexolKeyToCameraKey(param);

        if(key.equals(DEFAULT))
            return Bridge.CONST_PARAMETER_SUPPORTED();

        Camera.Parameters params = cameraParam;
        List<String> supportedParams = null;

        if(isType(param, ANTIBANDING_DEFAULT)){
            supportedParams = params.getSupportedAntibanding();
        }

        if(isType(param, WHITE_BALANCE_DEFAULT)){
            if(param != WHITE_BALANCE_LOCKED)
                supportedParams = params.getSupportedWhiteBalance();
            else{
                if(params.isAutoWhiteBalanceLockSupported()){
                    return Bridge.CONST_PARAMETER_SUPPORTED();
                }else{
                    return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
                }
            }
        }

        if(isType(param, COLOR_EFFECT_DEFAULT)){
            supportedParams = params.getSupportedColorEffects();
        }

        if(isType(param, FLASH_MODE_DEFAULT)){
            supportedParams = params.getSupportedFlashModes();
        }

        if(isType(param, SCENE_MODE_DEFAULT)){
            supportedParams = params.getSupportedSceneModes();
        }

        if(isType(param, FOCUS_MODE_DEFAULT)){
            supportedParams = params.getSupportedFocusModes();
        }

        if(isType(param, TORCH_MODE_DEFAULT)){
            supportedParams = params.getSupportedFlashModes(); // torch mode is in focus params on android
        }

        if(isType(param, EXPOSURE_MODE_DEFAULT)){
            if(param == EXPOSURE_MODE_FIXED){
                if(params.isAutoExposureLockSupported()){
                    return Bridge.CONST_PARAMETER_SUPPORTED();
                }else{
                    return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
                }
            }
            if(param == EXPOSURE_MODE_AUTO){
                return Bridge.CONST_PARAMETER_SUPPORTED();
            }
            if(param == EXPOSURE_MODE_CONTINUOUS_AUTO_EXPOSURE){
                return Bridge.CONST_PARAMETER_SUPPORTED();
            }
            if(param == EXPOSURE_MODE_CUSTOM){
                return Bridge.CONST_PARAMETER_NOT_SUPPORTED(); // TODO public Android Camera Api dont support setting iso, shutter speed and f/X values;
                // TODO in future maeybe use workaround with flatten method and key params setting in Parameters class
            }

            return Bridge.CONST_PARAMETER_SUPPORTED(); // default is supported
        }

        if(supportedParams == null)
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        if(supportedParams.size() == 0){
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }

        if(supportedParams.contains(key)){
            return Bridge.CONST_PARAMETER_SUPPORTED();
        }else{
            return Bridge.CONST_PARAMETER_NOT_SUPPORTED();
        }
    }
}
