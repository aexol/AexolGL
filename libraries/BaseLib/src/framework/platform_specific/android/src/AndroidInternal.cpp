//#include "../../src/DisplayState.hpp"
//#include <aex/Vector3.hpp>
//#include <jni.h>
//#include <android/sensor.h>

//extern "C" JNIEXPORT void JNICALL Java_com_aexolgl_android_app_AexActivity_changeOrientation(jint orientation)
//{
//aex::set_display_orientation(orientation);
//}
//namespace aex
//{
//static ASensorManager* sensorManager = nullptr;
//static const ASensor* accelerometerSensor = nullptr;
//static ASensorEventQueue* sensorEventQueue = nullptr;
//static ALooper* getLooper()
//{
//ALooper* looper = ALooper_forThread();
//if(!looper)
//{
//looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
//}
//return looper;
//}
//static bool initSensorManager()
//{
//sensorManager = ASensorManager_getInstance();
//accelerometerSensor = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_ACCELEROMETER);
//sensorEventQueue = ASensorManager_createEventQueue(sensorManager, looper, LOOPER_ID_USER, NULL, )
//}
//}
