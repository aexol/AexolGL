/*
 * File:   CameraMediator.cpp
 * Author: darek
 *
 * Created on February 25, 2015, 3:57 PM
 */

#include "CameraMediator.hpp"
#include "CameraBridge.hpp"
#include <memory>
#include <sstream>
#include <aex/Logger.hpp>

namespace aex
{
    int SUCCESS_JAVA;
    int FAILURE_JAVA;
    int IS_OPENED_JAVA;
    int IS_NOT_OPENED_JAVA;
    int OPENING_ERROR_JAVA;
    int NO_CAMERA_JAVA;
    int NO_PERMISSION_JAVA;
    int IS_SUPPORTED_JAVA;
    int IS_NOT_SUPPORTED_JAVA;

// singleton state
    struct CameraState
    {
        bool jniInited;
        bool opened;
        float m_transformMatrix[16];
        int textureID;
        bool bCameraFrameIsWaiting;
        bool m_bIsNewTransformMatrix;
        std::function<void()> cbOnNewFrame;
        std::function<void()> cbExpComp;
        std::function<void()> cbLensPos;
        std::function<void()> cbSmoothZoom;
        std::function<void(bool)> cbAutoFocus;
        std::function<void(int)> cbTakePicture;
        std::function<void(int)> cbStopVideo;
    };
    
    std::vector<std::unique_ptr<CameraState>> st;
    std::unique_ptr<com_aexolgl_camera_Bridge> bridge{nullptr};

    void loadBridgeIfNeeded(JNIEnv* env)
    {
        
        bridge.reset(new com_aexolgl_camera_Bridge(env));
//        if(!bridge)
//        {
//            bridge.reset(new com_aexolgl_camera_Bridge(env));
//        }else{
//            // return ?? / TODO
//        }

        // ========= LOADING JNI METHODS AREA ===========
        bridge->LOAD_STATIC_CONST_CAMERA_SUCCESS_(env);
        bridge->LOAD_STATIC_CONST_CAMERA_FAILURE_(env);
        bridge->LOAD_STATIC_CONST_CAMERA_IS_OPENED_(env);
        bridge->LOAD_STATIC_CONST_CAMERA_IS_NOT_OPENED_(env);
        bridge->LOAD_STATIC_CONST_CAMERA_OPENING_ERROR_(env);
        bridge->LOAD_STATIC_CONST_CAMERA_NO_CAMERA_WITH_PROVIDED_ID_(env);
        bridge->LOAD_STATIC_CONST_CAMERA_NO_PERMISSION_(env);
        bridge->LOAD_STATIC_CONST_PARAMETER_SUPPORTED_(env);
        bridge->LOAD_STATIC_CONST_PARAMETER_NOT_SUPPORTED_(env);

        bridge->LOAD_STATIC_getNumberOfCameras_(env);
        bridge->LOAD_STATIC_open__I(env);
        bridge->LOAD_STATIC_getTextureID__I(env);
        bridge->LOAD_STATIC_startPreview__I(env);
        bridge->LOAD_STATIC_stopPreview__I(env);
        bridge->LOAD_STATIC_release__I(env);
        bridge->LOAD_STATIC_updateTextureIfNeeded__I(env);
        bridge->LOAD_STATIC_setIsJavaTextureOwner__ZI(env);
        bridge->LOAD_STATIC_getErrorMessage__I(env);
        bridge->LOAD_STATIC_getSupportedPreviewSizes__I(env);
        bridge->LOAD_STATIC_setPreviewSize__III(env);
        bridge->LOAD_STATIC_setParameter__II(env);
        bridge->LOAD_STATIC_isSupported__II(env);
        bridge->LOAD_STATIC_setDisplayOrientation__II(env);

        bridge->LOAD_STATIC_getZoomAexolGL__I(env);
        bridge->LOAD_STATIC_getZoomMaxAexolGL__I(env);
        bridge->LOAD_STATIC_setZoomAexolGL__FI(env);
        bridge->LOAD_STATIC_setZoomSmoothAexolGL__FI(env);
        bridge->LOAD_STATIC_isSmoothlyZoomSupported__I(env);
        bridge->LOAD_STATIC_getFocalLength__I(env);

        bridge->LOAD_STATIC_getExposureEVAexolGL__I(env);
        bridge->LOAD_STATIC_getMinExposureEVAexolGL__I(env);
        bridge->LOAD_STATIC_getMaxExposureEVAexolGL__I(env);
        bridge->LOAD_STATIC_setExposureEVAexolGL__FI(env);
        bridge->LOAD_STATIC_getExposureCompensationStep__I(env);
        bridge->LOAD_STATIC_getExposureCompensationAndroid__I(env);
        bridge->LOAD_STATIC_getExposureCompensationMinAndroid__I(env);
        bridge->LOAD_STATIC_getExposureCompensationMaxAndroid__I(env);

        bridge->LOAD_STATIC_getMaxNumExposureAreas__I(env);
        bridge->LOAD_STATIC_getExposureAreasAexolGL__I(env);
        bridge->LOAD_STATIC_setExposureAreasAexolGL__ArrayarrayFI(env);

        bridge->LOAD_STATIC_getFocusDistances__I(env);
        bridge->LOAD_STATIC_getMaxNumFocusAreas__I(env);
        bridge->LOAD_STATIC_getFocusAreasAexolGL__I(env);
        bridge->LOAD_STATIC_setFocusAreasAexolGL__ArrayarrayFI(env);

        bridge->LOAD_STATIC_getPreviewFPSRangeAexolGL__I(env);
        bridge->LOAD_STATIC_getSupportedPreviewFPSRangesAexolGL__I(env);
        bridge->LOAD_STATIC_setPreviewFPSRangeAexolGL__FFI(env);

        bridge->LOAD_STATIC_getHorizontalViewAngle__I(env);
        bridge->LOAD_STATIC_getVerticalViewAngle__I(env);

        bridge->LOAD_STATIC_autoFocus__I(env);
        bridge->LOAD_STATIC_takePicture__String_IIIIII(env);
        bridge->LOAD_STATIC_getSupportedPictureSizes__I(env);
        bridge->LOAD_STATIC_isSupportedPictureFormat__II(env);
        bridge->LOAD_STATIC_setRotation__II(env);
        bridge->LOAD_STATIC_getFlatten__I(env);
        
        bridge->LOAD_STATIC_getUserDataDirectory_(env);
        bridge->LOAD_STATIC_getDownloadCacheDirectory_(env);
        bridge->LOAD_STATIC_getExternalStorageDirectory_(env);
        bridge->LOAD_STATIC_getExternalStoragePublicDirectory__I(env);
        bridge->LOAD_STATIC_mkdir__String(env);
        
        bridge->LOAD_STATIC_openGallery__String(env);
        bridge->LOAD_STATIC_startVideo__String_III(env);
        bridge->LOAD_STATIC_stopVideo__I(env);
        bridge->LOAD_STATIC_getSupportedVideoSizes__I(env);
        // To remove after debuging: TODO
//        bridge->LOAD_STATIC_getZoomAndroid__I(env);
//        bridge->LOAD_STATIC_getZoomRatios__I(env);
        // +++++++ STOP LOADING JNI METHODS AREA +++++++++++

        SUCCESS_JAVA = bridge->STATIC_CONST_CAMERA_SUCCESS_(env);
        FAILURE_JAVA = bridge->STATIC_CONST_CAMERA_FAILURE_(env);
        IS_OPENED_JAVA = bridge->STATIC_CONST_CAMERA_IS_OPENED_(env);
        IS_NOT_OPENED_JAVA = bridge->STATIC_CONST_CAMERA_IS_NOT_OPENED_(env);
        OPENING_ERROR_JAVA = bridge->STATIC_CONST_CAMERA_OPENING_ERROR_(env);
        NO_CAMERA_JAVA = bridge->STATIC_CONST_CAMERA_NO_CAMERA_WITH_PROVIDED_ID_(env);
        NO_PERMISSION_JAVA =  bridge->STATIC_CONST_CAMERA_NO_PERMISSION_(env);
        IS_SUPPORTED_JAVA = bridge->STATIC_CONST_PARAMETER_SUPPORTED_(env);
        IS_NOT_SUPPORTED_JAVA = bridge->STATIC_CONST_PARAMETER_NOT_SUPPORTED_(env);
    }

    std::string getCPPString(JNIEnv* env, jstring js)
    {
        const char* bytes = env->GetStringUTFChars(js, 0);
        std::string msg = std::string(bytes);
        env->ReleaseStringUTFChars(js, bytes);
        return msg;
    }

    std::string readErrorMessage(JNIEnv* env, int camID)
    {
        jstring javaString = (jstring)bridge->STATIC_getErrorMessage__I(env, camID);
        std::string msg = getCPPString(env, javaString);
        env->DeleteLocalRef(javaString);
        return msg;
    }
    
    
    
    void jintArrayToVec(JNIEnv* env, jintArray jArr, std::vector<int>* retVec)
    {
        if(jArr == NULL)
        {
            return;
        }

        jsize len = env->GetArrayLength(jArr);

        if(len > 0)
        {
            retVec->clear();
            int* array = env->GetIntArrayElements(jArr, 0);

            for(int i = 0 ; i < len ; i++)
            {
                retVec->push_back(array[i]);
            }

            env->ReleaseIntArrayElements(jArr, array, JNI_ABORT); // JNI_ABORT - just release native memory
        }
    }

    void jfloatArrayToVec(JNIEnv* env, jfloatArray jArr, std::vector<float>* retVec)
    {
        if(jArr == NULL)
        {
            return;
        }

        jsize len = env->GetArrayLength(jArr);

        //LOG("len: ", len);
        if(len > 0)
        {
            retVec->clear();
            float* array = env->GetFloatArrayElements(jArr, 0);

            for(int i = 0 ; i < len ; i++)
            {
                retVec->push_back(array[i]);
            }

            env->ReleaseFloatArrayElements(jArr, array, JNI_ABORT); // JNI_ABORT - just release native memory
        }
    }

    CameraMediator::CameraMediator()
    {
    }

    CameraMediator::~CameraMediator()
    {
        // TODO think about releasing camera state () ??)
    }

    void CameraMediator::setTransformMatrix(float* transMatrix, int camID)
    {
        memcpy(st[camID]->m_transformMatrix, transMatrix, 16 * sizeof(float));
        st[camID]->m_bIsNewTransformMatrix = true;
    }

    const float* CameraMediator::getTransformMatrix(int camID)
    {
        return st[camID]->m_transformMatrix;
    }

    void CameraMediator::transformMatrixLoaded(int camID)
    {
        st[camID]->m_bIsNewTransformMatrix = false;
    }

    int CameraMediator::open(JNIEnv* env, int camID)
    {
        loadBridgeIfNeeded(env);

        while(camID >= st.size())
            st.push_back(std::unique_ptr<CameraState> {nullptr});

        if(! st[camID])
        {
            st[camID].reset(new CameraState());
            st[camID]->bCameraFrameIsWaiting = false;
            st[camID]->m_bIsNewTransformMatrix = false;
            st[camID]->jniInited = false;
            st[camID]->opened = false;
            st[camID]->m_bIsNewTransformMatrix = false;
            st[camID]->jniInited = true;
        }

        if(st[camID]->opened)
        {
            return IS_OPENED_CODE();
        }

        int code = bridge->STATIC_open__I(env, camID);
        if(code == SUCCESS_JAVA)
        {
            bridge->STATIC_setIsJavaTextureOwner__ZI(env, false, camID);
            st[camID]->textureID = bridge->STATIC_getTextureID__I(env, camID);
            st[camID]->opened = true;
            return SUCCESS_CODE();
        }
        return javaCodeToCMCode(code);
    }

    int CameraMediator::start(JNIEnv* env, int camID)
    {
        if(! st[camID]->opened)
        {
            return IS_NOT_OPENED_CODE();
        }

        int code = bridge->STATIC_startPreview__I(env, camID);
        return javaCodeToCMCode(code);
    }

    int CameraMediator::stop(JNIEnv* env, int camID)
    {
        loadBridgeIfNeeded(env);
        
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }
        
        
        
        int code = bridge->STATIC_stopPreview__I(env, camID);
        return javaCodeToCMCode(code);
    }

    int CameraMediator::release(JNIEnv* env, int camID)
    {
        if(!st[camID])
        {
            return IS_NOT_OPENED_CODE();
        }

        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        int code = bridge->STATIC_release__I(env, camID);

        if(code == SUCCESS_JAVA)
        {
            st[camID].reset(nullptr);

            bool anyOpened = false;

            for(int i = 0 ; i < st.size() ; i++)
            {
                if(st[i])
                {
                    if(st[i]->opened)
                    {
                        anyOpened = true;
                    }
                }
            }

            if(! anyOpened)
            {
                st.clear();
                bridge.reset(nullptr);
            }

            // check if this is the last, if yes, release static vars
            return SUCCESS_CODE();
        }

        return FAILURE_CODE();
    }

    void CameraMediator::onNewCameraFrame(JNIEnv* env, int camID)
    {
        if(st.size() > camID)
        {
            st[camID]->bCameraFrameIsWaiting = true;
            st[camID]->cbOnNewFrame(); // callback sending task to main camera thread
        }
    }

    int CameraMediator::getTextureID(JNIEnv* env, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }
        st[camID]->textureID = bridge->STATIC_getTextureID__I(env, camID);
        return st[camID]->textureID;
    }

    bool CameraMediator::isCameraFrameWaiting(int camID)
    {
        return st[camID]->bCameraFrameIsWaiting;
    }

    bool CameraMediator::isNewTransformMatrix(int camID)
    {
        return  st[camID]->m_bIsNewTransformMatrix;
    }

    void CameraMediator::updateTexture(JNIEnv* env, int camID)
    {
        if(st[camID]->bCameraFrameIsWaiting)
        {
            st[camID]->bCameraFrameIsWaiting = false;
            bridge->STATIC_updateTextureIfNeeded__I(env, camID);
        }
    }

    int CameraMediator::getNumberOfCameras(JNIEnv* env)
    {
        loadBridgeIfNeeded(env);
        return bridge->STATIC_getNumberOfCameras_(env);
    }

    bool CameraMediator::isOpened(int camID)
    {
        if(st.size() <= camID)
        {
            return false;
        }

        return st[camID]->opened;
    }
    
    int CameraMediator::setCallbackOnNewFrame(const std::function<void()>& cb, int camID){
        if(isOpened(camID)){
            st[camID]->cbOnNewFrame = cb;
        }
        return 0;
    }

    int CameraMediator::setDisplayOrientation(JNIEnv* env, int degrees, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        int code = bridge->STATIC_setDisplayOrientation__II(env, degrees, camID);
        return javaCodeToCMCode(code);
    }

    int CameraMediator::setPreviewSize(JNIEnv* env, int width, int height, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        int code = bridge->STATIC_setPreviewSize__III(env, width, height, camID);

        if(code != SUCCESS_JAVA)
        {
            return FAILURE_CODE();
        }

        return SUCCESS_CODE();
    }

    int CameraMediator::getSupportedPreviewSizes(JNIEnv* env, std::vector<int>* retSizes, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        jintArray jArr = bridge->STATIC_getSupportedPreviewSizes__I(env, camID);
        jintArrayToVec(env, jArr, retSizes);
        env->DeleteLocalRef(jArr);

        return SUCCESS_CODE();
    }

    int CameraMediator::setParameter(JNIEnv* env, int param, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        int code = bridge->STATIC_setParameter__II(env, param, camID);

        if(code == IS_NOT_OPENED_JAVA)
        {
            return IS_NOT_OPENED_CODE();
        }

        if(code == IS_SUPPORTED_JAVA)
        {
            return IS_SUPPORTED_CODE();
        }
        else
        {
            return IS_NOT_SUPPORTED_CODE();
        }

    }

    int CameraMediator::isSupported(JNIEnv* env, int param, bool* retVal, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        int code = bridge->STATIC_isSupported__II(env, param, camID);

        if(code == IS_SUPPORTED_JAVA)
        {
            *retVal = true;
            return SUCCESS_CODE();
        }
        else
        {
            *retVal = false;
            return SUCCESS_CODE();
        }
    }

    int CameraMediator::getZoom(JNIEnv* env, float* retZoom, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        float zoomFactor = bridge->STATIC_getZoomAexolGL__I(env, camID);
        *retZoom = zoomFactor;

        return SUCCESS_CODE();
    }

    int CameraMediator::getZoomMax(JNIEnv* env, float* retZoomMax, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        float zoomFactorMax = bridge->STATIC_getZoomMaxAexolGL__I(env, camID);
        *retZoomMax = zoomFactorMax;

        return SUCCESS_CODE();
    }

//    void debugRatios(JNIEnv* env, float zoomFactor, int camID)
//    {
//        std::vector<int> ratios;
//        jintArray jArr = bridge->STATIC_getZoomRatios__I(env, camID);
//        jintArrayToVec(env, jArr, &ratios);
//        env->DeleteLocalRef(jArr);
//
//        int zoomAndroid = bridge->STATIC_getZoomAndroid__I(env, camID);
//
//        std::stringstream ss;
//
//        for(int i = 0 ; i < ratios.size() ; i++)
//        {
//            ss << i << ":[" << ratios[i] << "] ";
//        }
//
//        LOG("#### Zoom ratios: ", ss.str());
//        LOG("#### Chosen zoom android id from android ratios: ", zoomAndroid);
//    }

    int CameraMediator::setZoom(JNIEnv* env, float zoomFactor, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        int code = bridge->STATIC_setZoomAexolGL__FI(env, zoomFactor, camID);

        //debugRatios(env, zoomFactor, camID);
        return javaCodeToCMCode(code);
    }

    int CameraMediator::setZoomSmooth(JNIEnv* env, float zoomFactor, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        int code = bridge->STATIC_setZoomSmoothAexolGL__FI(env, zoomFactor, camID);

        //debugRatios(env, zoomFactor, camID);
        return javaCodeToCMCode(code);
    }

    int CameraMediator::isSmoothZoomSupported(JNIEnv* env, bool* retIsSupp, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        int code = bridge->STATIC_isSmoothlyZoomSupported__I(env, camID);

        if(code == SUCCESS_JAVA)
        {
            *retIsSupp = true;
            return SUCCESS_CODE();
        }
        else
        {
            *retIsSupp = false;
            return SUCCESS_CODE();
        }
    }

    int CameraMediator::getFocalLength(JNIEnv* env, float* retFocalLength, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        *retFocalLength = bridge->STATIC_getFocalLength__I(env, camID);

        return SUCCESS_CODE();
    }

    int CameraMediator::getExposureCompensationStep(JNIEnv* env, float* retStep, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        *retStep = bridge->STATIC_getExposureCompensationStep__I(env, camID);

        return SUCCESS_CODE();
    }

    int CameraMediator::getExposureCompensation(JNIEnv* env, float* retEv, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        *retEv = bridge->STATIC_getExposureEVAexolGL__I(env, camID);

        return SUCCESS_CODE();
    }

    int CameraMediator::getMinExposureCompensation(JNIEnv* env, float* retEv, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        *retEv = bridge->STATIC_getMinExposureEVAexolGL__I(env, camID);

        return SUCCESS_CODE();
    }

    int CameraMediator::getMaxExposureCompensation(JNIEnv* env, float* retEv, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        *retEv = bridge->STATIC_getMaxExposureEVAexolGL__I(env, camID);

        return SUCCESS_CODE();
    }

    int CameraMediator::setExposureCompensation(JNIEnv* env, float ev, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        int code = bridge->STATIC_setExposureEVAexolGL__FI(env, ev, camID);

//        int evAndr = bridge->STATIC_getExposureCompensationAndroid__I(env, camID);
//        int minEV = bridge->STATIC_getExposureCompensationMinAndroid__I(env, camID);
//        int maxEV = bridge->STATIC_getExposureCompensationMaxAndroid__I(env, camID);
//        LOG("ANDROID EXPOSURE PARAMS AFTER SET:\n", evAndr, " ", minEV, " ", maxEV, " ");

        if(code == IS_SUPPORTED_JAVA)
        {
            if(st[camID]->cbExpComp)
            {
                st[camID]->cbExpComp();    // callback invokation (android api dont have callbacks for this method)
            }

            return IS_SUPPORTED_CODE();
        }
        else
        {
            return IS_NOT_SUPPORTED_CODE();
        }
    }

    int CameraMediator::getMaxExposureAreas(JNIEnv* env, int* retAreas, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        *retAreas = bridge->STATIC_getMaxNumExposureAreas__I(env, camID);

        return SUCCESS_CODE();
    }

    int CameraMediator::getExposureAreas(JNIEnv* env, std::vector<float>* retAreas, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        jfloatArray jArr = bridge->STATIC_getExposureAreasAexolGL__I(env, camID);
        jfloatArrayToVec(env, jArr, retAreas);
        env->DeleteLocalRef(jArr);
        return SUCCESS_CODE();
    }

    int CameraMediator::setExposureAreas(JNIEnv* env, const std::vector<float>& areas, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        jfloatArray jArr = env->NewFloatArray(areas.size());
        jfloat* fill = new jfloat[areas.size()];

        for(int i = 0 ; i < areas.size() ; i++)
        {
            fill[i] = areas[i];
        }

        env->SetFloatArrayRegion(jArr, 0, areas.size(), fill); // send data to jvm array
        delete[] fill;

        int code = bridge->STATIC_setExposureAreasAexolGL__ArrayarrayFI(env, jArr, camID);
        env->DeleteLocalRef(jArr);
        return javaCodeToCMCode(code);
    }

    int CameraMediator::getFocusDistances(JNIEnv* env, float* retNear, float* retOptimal, float* retFar, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        std::vector<float> distances;
        jfloatArray jArr = bridge->STATIC_getFocusDistances__I(env, camID);
        jfloatArrayToVec(env, jArr, &distances);
        env->DeleteLocalRef(jArr);

        *retNear = distances[0];
        *retOptimal = distances[1];
        *retFar = distances[2];

        return SUCCESS_CODE();
    }

    int CameraMediator::getMaxFocusAreas(JNIEnv* env, int* retMax, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        *retMax = bridge->STATIC_getMaxNumFocusAreas__I(env, camID);

        return SUCCESS_CODE();
    }
    int CameraMediator::getFocusAreas(JNIEnv* env, std::vector<float>* retAreas, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        jfloatArray jArr = bridge->STATIC_getFocusAreasAexolGL__I(env, camID);
        jfloatArrayToVec(env, jArr, retAreas);
        env->DeleteLocalRef(jArr);

        return SUCCESS_CODE();
    }
    int CameraMediator::setFocusAreas(JNIEnv* env, const std::vector<float>& areas, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        jfloatArray jArr = env->NewFloatArray(areas.size());
        jfloat* fill = new jfloat[areas.size()];

        for(int i = 0 ; i < areas.size() ; i++)
        {
            fill[i] = areas[i];
        }

        env->SetFloatArrayRegion(jArr, 0, areas.size(), fill); // send data to jvm array
        delete[] fill;

        int code = bridge->STATIC_setFocusAreasAexolGL__ArrayarrayFI(env, jArr, camID);
        env->DeleteLocalRef(jArr);
        return javaCodeToCMCode(code);
    }

    int CameraMediator::getPrevievFPSRange(JNIEnv* env, float* retMinFPS, float* retMaxFPS, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        std::vector<float> range;
        jfloatArray jArr = bridge->STATIC_getPreviewFPSRangeAexolGL__I(env, camID);
        jfloatArrayToVec(env, jArr, &range);
        env->DeleteLocalRef(jArr);

        *retMinFPS = range[0];
        *retMaxFPS = range[1];

        return SUCCESS_CODE();
    }
    int CameraMediator::getSupportedPreviewFPSRanges(JNIEnv* env, std::vector<float>* retRanges, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        jfloatArray jArr = bridge->STATIC_getSupportedPreviewFPSRangesAexolGL__I(env, camID);
        jfloatArrayToVec(env, jArr, retRanges);
        env->DeleteLocalRef(jArr);

        return SUCCESS_CODE();
    }
    int CameraMediator::setPreviewFPSRange(JNIEnv* env, float minFPS, float maxFPS, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        int code = bridge->STATIC_setPreviewFPSRangeAexolGL__FFI(env, minFPS, maxFPS, camID);

        //std::string str = readErrorMessage(env, camID);
        return javaCodeToCMCode(code);
    }

    int CameraMediator::getHorizontalViewAngle(JNIEnv* env, float* retHVA, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        *retHVA = bridge->STATIC_getHorizontalViewAngle__I(env, camID);

        return SUCCESS_CODE();
    }

    int CameraMediator::getVerticalViewAngle(JNIEnv* env, float* retVVA, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        *retVVA = bridge->STATIC_getVerticalViewAngle__I(env, camID);

        return SUCCESS_CODE();
    }

    int CameraMediator::getLensAperture(JNIEnv* env, float* retLensAperture, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        *retLensAperture = -1.0f;    // TODO

        return SUCCESS_CODE();
    }

    int CameraMediator::getLensPosition(JNIEnv* env, float* retLensPosition, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        *retLensPosition = -1.0f;    //TODO

        return SUCCESS_CODE();
    }

    int CameraMediator::getFOV(JNIEnv* env, float* retFov, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        *retFov = -1.0f; // TODO

        return SUCCESS_CODE();
    }

    int CameraMediator::setLensPositionWithFixedFocusMode(JNIEnv* env, float lensPos, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        int code = FAILURE_JAVA;
        // TODO set lensPos somewhere (in bridge ?, check if FIXED FOC MODE IS SUPP)

        if(code == SUCCESS_JAVA)
        {
            if(st[camID]->cbLensPos)
            {
                st[camID]->cbLensPos();    // android doesnt have this in api, invoke here
            }
        }
        
        return javaCodeToCMCode(code);
    }

    int CameraMediator::autoFocus(JNIEnv* env, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        int code = bridge->STATIC_autoFocus__I(env, camID);
        return javaCodeToCMCode(code);
    }

    int CameraMediator::takePicture(JNIEnv* env, std::string path, int width, int height, int format, int JPGQuality, int filter,  int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }
        
        jstring js = env->NewStringUTF(path.c_str());
        int code = bridge->STATIC_takePicture__String_IIIIII(env, js, width, height, format, JPGQuality, filter, camID);
        
        env->DeleteLocalRef(js);
        
//        std::string str = readErrorMessage(env, camID);
//        LOG("Info ? :", str);

        return javaCodeToCMCode(code);
    }
    
    int CameraMediator::startVideo(JNIEnv* env, std::string path, int width, int height, int camID){
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }
        
        jstring js = env->NewStringUTF(path.c_str());
        int code = bridge->STATIC_startVideo__String_III(env, js, width, height, camID);
        
        env->DeleteLocalRef(js);
        
        return javaCodeToCMCode(code);
    }
        
    int CameraMediator::stopVideo(JNIEnv* env, int camID){
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }
        
        int code = bridge->STATIC_stopVideo__I(env, camID);
        
        return javaCodeToCMCode(code);
    }
    
    int CameraMediator::getSupportedPictureSizes(JNIEnv* env, std::vector<int>* retSizes, int camID){
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        jintArray jArr = bridge->STATIC_getSupportedPictureSizes__I(env, camID); // TODO change
        jintArrayToVec(env, jArr, retSizes);
        env->DeleteLocalRef(jArr);

        return SUCCESS_CODE();
    }
    
    int CameraMediator::isSupportedPictureFormat(JNIEnv* env, int format, bool* result, int camID){
        int code = bridge->STATIC_isSupportedPictureFormat__II(env);
        if(code == SUCCESS_JAVA){
            *result = true;
        }else{
            *result = false;
        }
        return javaCodeToCMCode(code);
    }
    
    int CameraMediator::setRotation(JNIEnv* env, int degrees, int camID){
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }
        
        return javaCodeToCMCode(bridge->STATIC_setRotation__II(env, degrees, camID));
    }
    
    int CameraMediator::getSupportedVideoSizes(JNIEnv* env, std::vector<int>* retSizes, int camID){
        if(! isOpened(camID)){
            return IS_NOT_OPENED_CODE();
        }
        
        jintArray jArr = bridge->STATIC_getSupportedVideoSizes__I(env, camID); // TODO change
        jintArrayToVec(env, jArr, retSizes);
        env->DeleteLocalRef(jArr);
        
        return SUCCESS_CODE();
    }
    
    int CameraMediator::getFlatten(JNIEnv* env, std::string* result, int camID){
        jstring jstr = (jstring)bridge->STATIC_getFlatten__I(env, camID);
        *result = getCPPString(env, jstr);
        env->DeleteLocalRef(jstr);
        return SUCCESS_CODE();
    }
  
    int CameraMediator::setCallbackExposureCompensation(const std::function<void()> cb, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        st[camID]->cbExpComp = cb;
        return SUCCESS_CODE();
    }

    int CameraMediator::setCallbackLensPosition(const std::function<void()> cb, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        st[camID]->cbLensPos = cb;
        return SUCCESS_CODE();
    }

    int CameraMediator::setCallbackSmoothZoom(const std::function<void()> cb, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        st[camID]->cbSmoothZoom = cb;
        return SUCCESS_CODE();
    }

    int CameraMediator::setCallbackAutoFocus(const std::function<void(bool)> cb, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        st[camID]->cbAutoFocus = cb;
        return SUCCESS_CODE();
    }

    int CameraMediator::setCallbackTakePicture(const std::function<void(int)> cb, int camID)
    {
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }

        st[camID]->cbTakePicture = cb;
        return SUCCESS_CODE();
    }
    
    int CameraMediator::setCallbackStopVideo(const std::function<void(int)> cb, int camID){
        if(! isOpened(camID))
        {
            return IS_NOT_OPENED_CODE();
        }
        
        st[camID]->cbStopVideo = cb;
        return SUCCESS_CODE();
    }

    void CameraMediator::invokeCallbackSmoothZoom(int camID)
    {
        if(st[camID]->cbSmoothZoom)
        {
            st[camID]->cbSmoothZoom();
        }
    }

    void CameraMediator::invokeCallbackAutoFocus(bool success, int camID)
    {
        if(st[camID]->cbAutoFocus)
        {
            st[camID]->cbAutoFocus(success);
        }
    }

    void CameraMediator::invokeCallbackTakePicture(int result, int camID)
    {
        if(st[camID]->cbTakePicture)
        {
            st[camID]->cbTakePicture(result);
        }
    }
    
    void CameraMediator::invokeCallbackStopVideo(int result, int camID){
        if(st[camID]->cbStopVideo)
        {
            st[camID]->cbStopVideo(result);
        }
    }
    
    int CameraMediator::getUserDataDirectory(JNIEnv* env, std::string* result) {
        loadBridgeIfNeeded(env);
        jstring jstr = (jstring)bridge->STATIC_getUserDataDirectory_(env);
        *result = getCPPString(env, jstr);
        env->DeleteLocalRef(jstr);
        return SUCCESS_CODE();
    }

    int CameraMediator::getDownloadCacheDirectory(JNIEnv* env, std::string* result) {
        loadBridgeIfNeeded(env);
        jstring jstr = (jstring)bridge->STATIC_getDownloadCacheDirectory_(env);
        *result = getCPPString(env, jstr);
        env->DeleteLocalRef(jstr);
        return SUCCESS_CODE();
    }

    int CameraMediator::getExternalStorageDirectory(JNIEnv* env, std::string* result) {
        loadBridgeIfNeeded(env);
        jstring jstr = (jstring)bridge->STATIC_getExternalStorageDirectory_(env);
        *result = getCPPString(env, jstr);
        env->DeleteLocalRef(jstr);
        return SUCCESS_CODE();
    }

    int CameraMediator::getExternalStoragePublicDirectory(JNIEnv* env, int type, std::string* result) {
        loadBridgeIfNeeded(env);
        jstring jstr = (jstring)bridge->STATIC_getExternalStoragePublicDirectory__I(env, type);
        *result = getCPPString(env, jstr);
        env->DeleteLocalRef(jstr);
        return SUCCESS_CODE();
    }
    
    int CameraMediator::mkdir(JNIEnv* env, const std::string& path){
        loadBridgeIfNeeded(env);
        jstring jstr = env->NewStringUTF(path.c_str());
        int code = bridge->STATIC_mkdir__String(env, jstr);
        env->DeleteLocalRef(jstr);
        return javaCodeToCMCode(code);
    }
    
    int CameraMediator::openGallery(JNIEnv* env, const std::string& path){
        loadBridgeIfNeeded(env);
        jstring jstr = env->NewStringUTF(path.c_str());
        int code = bridge->STATIC_openGallery__String(env, jstr);
        env->DeleteLocalRef(jstr);
        return javaCodeToCMCode(code);
    }   
    
    int CameraMediator::javaCodeToCMCode(int javaCode) const{
        if (javaCode == SUCCESS_JAVA) {
            return SUCCESS_CODE();
        }
        if (javaCode == FAILURE_JAVA) {
            return FAILURE_CODE();
        }
        if (javaCode == IS_OPENED_JAVA) {
            return IS_OPENED_CODE();
        }
        if (javaCode == IS_NOT_OPENED_JAVA) {
            return IS_NOT_OPENED_CODE();
        }
        if (javaCode == OPENING_ERROR_JAVA) {
            return OPENING_ERROR_CODE();
        }
        if (javaCode == NO_CAMERA_JAVA) {
            return NO_CAMERA_CODE();
        }
        if (javaCode == NO_PERMISSION_JAVA) {
            return NO_PERMISSION_CODE();
        }
        if (javaCode == IS_SUPPORTED_JAVA) {
            return IS_SUPPORTED_CODE();
        }
        if (javaCode == IS_NOT_SUPPORTED_JAVA) {
            return IS_NOT_SUPPORTED_CODE();
        }
        return FAILURE_CODE();
    }
} // end of namespace aex


// singleton
aex::CameraMediator cm;

JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeInit
(JNIEnv* env, jclass clazz, jint width, jint height, jint camID)
{

}

JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeSetTransformMatrix
(JNIEnv* env, jclass clazz, jfloatArray textureTransform, jint camID)
{

    float* texTrans = env->GetFloatArrayElements(textureTransform, 0);
    cm.setTransformMatrix(texTrans, camID);
    env->ReleaseFloatArrayElements(textureTransform, texTrans, JNI_ABORT); // JNI_ABORT - just release native memory
}

JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeDraw
(JNIEnv* env, jclass clazz)
{

}

JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeOnNewCameraFrame
(JNIEnv* env, jclass clazz, jint camID)
{
    cm.onNewCameraFrame(env, camID);
}


JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeCallbackSmoothZoomEnded
(JNIEnv* env, jclass clazz, jint camID)
{
    cm.invokeCallbackSmoothZoom(camID);
}

JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeCallbackAutoFocus
(JNIEnv* env, jclass clazz, jboolean success, jint camID)
{
    cm.invokeCallbackAutoFocus(success, camID);
}

JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeCallbackTakePicture
(JNIEnv* env, jclass clazz, jint result, jint camID)
{
    cm.invokeCallbackTakePicture(result, camID);
}

JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeCallbackStopVideo
(JNIEnv* env, jclass clazz, jint result, jint camID)
{
    cm.invokeCallbackStopVideo(result, camID);
}

