/*
 * File:   CameraMediator.hpp
 * Author: darek
 *
 * Created on February 25, 2015, 3:13 PM
 */

#ifndef __CAMERA_MEDIATOR_HPP
#define	__CAMERA_MEDIATOR_HPP

#include <aex/Common.hpp>
#include <android/log.h>
#include <jni.h>
#include <string>
#include <vector>
#include <functional>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeInit
(JNIEnv*, jclass, jint, jint, jint);

JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeSetTransformMatrix
(JNIEnv*, jclass, jfloatArray, jint);

void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeDraw
(JNIEnv*, jclass);

JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeOnNewCameraFrame
(JNIEnv*, jclass, jint);

JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeCallbackSmoothZoomEnded
(JNIEnv*, jclass, jint);

JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeCallbackAutoFocus
(JNIEnv*, jclass, jboolean, jint);

JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeCallbackTakePicture
(JNIEnv*, jclass, jint, jint);

//JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeCallbackStopVideo
//(JNIEnv*, jclass, jint, jint);

#ifdef __cplusplus
}
#endif

#define  LOG_TAG    "camtest"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

namespace aex
{

// Singleton
    class CameraMediator
    {
    public:
        CameraMediator();
        virtual ~CameraMediator();
        void setTransformMatrix(float* transMatrix, int camID);
        const float* getTransformMatrix(int camID) ;
        int open(JNIEnv* env, int camID);
        int start(JNIEnv* env, int camID);
        int stop(JNIEnv* env, int camID);
        int release(JNIEnv* env, int camID);
        void onNewCameraFrame(JNIEnv* env, int camID);
        int getTextureID(JNIEnv* env, int camID) ;
        bool isCameraFrameWaiting(int camID) ;
        bool isNewTransformMatrix(int camID) ;
        void transformMatrixLoaded(int camID);
        void updateTexture(JNIEnv* env, int camID);
        int getNumberOfCameras(JNIEnv* env) ;
        bool isOpened(int camID);
        
        int setCallbackOnNewFrame(const std::function<void()>& cb, int camID);       
        
        // parameters
        int setDisplayOrientation(JNIEnv* env, int degrees, int camID);
        int setPreviewSize(JNIEnv* env, int width, int height, int camID);
        int getSupportedPreviewSizes(JNIEnv* env, std::vector<int>* retSizes, int camID);
        int setParameter(JNIEnv* env, int param, int camID);
        int isSupported(JNIEnv* env, int param, bool* retVal, int camID);

        int getZoom(JNIEnv* env, float* retZoom, int camID);
        int getZoomMax(JNIEnv* env, float* retZoomMax, int camID);
        int setZoom(JNIEnv* env, float zoomFactor, int camID);
        int setZoomSmooth(JNIEnv* env, float zoomFactor, int camID);
        int isSmoothZoomSupported(JNIEnv* env, bool* retIsSupp, int camID);
        int getFocalLength(JNIEnv* env, float* retFocalLength, int camID);

        int getExposureCompensationStep(JNIEnv* env, float* retStep, int camID);
        int getExposureCompensation(JNIEnv* env, float* retEv, int camID);
        int getMinExposureCompensation(JNIEnv* env, float* retEv, int camID);
        int getMaxExposureCompensation(JNIEnv* env, float* retEv, int camID);
        int setExposureCompensation(JNIEnv* env, float ev, int camID);

        int getMaxExposureAreas(JNIEnv* env, int* retAreas, int camID);
        int getExposureAreas(JNIEnv* env, std::vector<float>* areas, int camID);
        int setExposureAreas(JNIEnv* env, const std::vector<float>& areas, int camID);

        int getFocusDistances(JNIEnv* env, float* retNear, float* retOptimal, float* retFar, int camID);
        int getMaxFocusAreas(JNIEnv* env, int* retMax, int camID);
        int getFocusAreas(JNIEnv* env, std::vector<float>* retAreas, int camID);
        int setFocusAreas(JNIEnv* env, const std::vector<float>& areas, int camID);

        int getPrevievFPSRange(JNIEnv* env, float* retMinFPS, float* retMaxFPS, int camID);
        int getSupportedPreviewFPSRanges(JNIEnv* env, std::vector<float>* retRanges, int camID);
        int setPreviewFPSRange(JNIEnv* env, float minFPS, float maxFPS, int camID);

        int getHorizontalViewAngle(JNIEnv* env, float* retHVA, int camID);
        int getVerticalViewAngle(JNIEnv* env, float* retVVA, int camID);

        int getLensAperture(JNIEnv* env, float* retLensAperture, int camID);
        int getLensPosition(JNIEnv* env, float* retLensPosition, int camID);
        int getFOV(JNIEnv* env, float* retFov, int camID);

        int setLensPositionWithFixedFocusMode(JNIEnv* env, float lensPos, int camID);

        int autoFocus(JNIEnv* env, int camID);
        int takePicture(JNIEnv* env, aex::string path, int width, int height, int format, int JPGQuality, int filter, int camID);
        int startVideo(JNIEnv* env, aex::string path, int width, int height, int camID);
        int stopVideo(JNIEnv* env, int camID);
        int getSupportedPictureSizes(JNIEnv* env, std::vector<int>* retSizes, int camID);
        int isSupportedPictureFormat(JNIEnv* env, int format, bool* result, int camID);
        int setRotation(JNIEnv* env, int degrees, int camID);
        int getSupportedVideoSizes(JNIEnv* env, std::vector<int>* retSizes, int camID);
        
        int getFlatten(JNIEnv* env, aex::string* result, int camID);
        
        int setCallbackExposureCompensation(const std::function<void()> cb, int camID);
        int setCallbackLensPosition(const std::function<void()> cb, int camID);
        int setCallbackSmoothZoom(const std::function<void()> cb, int camID);
        int setCallbackAutoFocus(const std::function<void(bool)> cb, int camID);
        int setCallbackTakePicture(const std::function<void(int)> cb, int camID);
        //int setCallbackStopVideo(const std::function<void(int)> cb, int camID);
        
        void invokeCallbackSmoothZoom(int camID);
        void invokeCallbackAutoFocus(bool success, int camID);
        void invokeCallbackTakePicture(int result, int camID);
        //void invokeCallbackStopVideo(int result, int camID);
        
        int getUserDataDirectory(JNIEnv* env, aex::string* result);
        int getDownloadCacheDirectory(JNIEnv* env, aex::string* result);
        int getExternalStorageDirectory(JNIEnv* env, aex::string* result);
        int getExternalStoragePublicDirectory(JNIEnv* env, int type, aex::string* result);
        int mkdir(JNIEnv* env, const aex::string& path);
        
        int openGallery(JNIEnv* env, const aex::string& path);
        
        // CM - CameraMediator
        int javaCodeToCMCode(int javaCode) const;

        //int getSupportedAntibanding(JNIEnv* env, std::vector<int>* suppParams, int camID);
        static int SUCCESS_CODE()
        {
            return 1;
        }
        static int FAILURE_CODE()
        {
            return 0;
        }
        static int IS_OPENED_CODE()
        {
            return 2;
        }
        static int IS_NOT_OPENED_CODE()
        {
            return 3;
        }
        static int OPENING_ERROR_CODE()
        {
            return 4;
        }
        static int NO_CAMERA_CODE()
        {
            return 5;
        }
        static int NO_PERMISSION_CODE()
        {
            return 6;
        }
        static int IS_SUPPORTED_CODE()
        {
            return 7;
        }
        static int IS_NOT_SUPPORTED_CODE()
        {
            return 8;
        }
    private:
    };
}
#endif	/* CameraMediator_HPP */

