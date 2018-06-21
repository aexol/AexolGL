#include "com_aexol_camera_jni_NativeCameraLib.h"

#include "NativeCameraTextureRenderer.h"

#ifndef LOG_TAG
#define  LOG_TAG    "camtest"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif

NativeCameraTextureRenderer renderer;

JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeInit
(JNIEnv *env, jclass clazz, jint width, jint height, jint camID) {

    renderer.init(env, width, height);
}

JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeSetTransformMatrix
(JNIEnv *env, jclass clazz, jfloatArray textureTransform, jint camID) {
    float *texTrans;
    texTrans = env->GetFloatArrayElements(textureTransform, 0);

    // checking provided values
    for(int i = 0 ; i < 4 ; i++) {
        LOGI("%f %f %f %f", texTrans[i*4 +0],texTrans[i*4 +1],texTrans[i*4 +2],texTrans[i*4 +3]);
    }

    renderer.setTransformMatrix(texTrans);
    env->ReleaseFloatArrayElements(textureTransform, texTrans, JNI_ABORT); // po prostu zwolnienie pamieci po uzyciu, bez updatow czegokolwiek
}

JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeDraw
(JNIEnv *env, jclass clazz) {
    renderer.onDraw(env);
}

JNIEXPORT void JNICALL Java_com_aexolgl_android_camera_NativeCameraLib_nativeOnNewCameraFrame
(JNIEnv *env, jclass clazz, jint camID) {
    renderer.onNewCameraFrame(env);
}

