#include "../../include/aex/hdw/Sensor.hpp"
#include <sstream>
#include <iostream>
#include <aex/Logger.hpp>
#include "JNIUtils.hpp"
#include "com_aexolgl_android_hdw_HDWBridge.h"

JNIEXPORT void JNICALL Java_com_aexolgl_android_hdw_HDWBridge_nativeOnSensorChanged(
JNIEnv *env, jclass clazz, jfloatArray valuesArray, jlong cbAddress)
{
  using namespace aex::hdw;
  SensorEvent sensorEvent;

  jsize len = env->GetArrayLength(valuesArray);
  jfloat *body = env->GetFloatArrayElements(valuesArray, 0);
  sensorEvent.values = std::vector<float>(body, body + len);
  env->ReleaseFloatArrayElements(valuesArray, body, JNI_ABORT);

  std::function<void(const SensorEvent& event)>* cbPtr = (std::function<void(const SensorEvent& event)>*)cbAddress;
  (*cbPtr)(sensorEvent);
}

namespace aex
{
  namespace hdw
  {
    void Sensor::nativeDestructor(int sensorID)
    {
      auto activity = GetAndroidActivityAsActivity();
      getJniClass("com/aexolgl/android/hdw/HDWBridge").staticCallVoid("removeSensor", sensorID, activity);
    }

    void Sensor::nativeStart(int sensorID)
    {
      auto activity = GetAndroidActivityAsActivity();
      getJniClass("com/aexolgl/android/hdw/HDWBridge").staticCallVoid("sensorStart", sensorID, activity);
    }

    void Sensor::nativeStop(int sensorID)
    {
      auto activity = GetAndroidActivityAsActivity();
      getJniClass("com/aexolgl/android/hdw/HDWBridge").staticCallVoid("sensorStop", sensorID, activity);
    }

    void Sensor::nativeAddListener(int sensorID, const std::function<void(const SensorEvent& event)>& listener)
    {
      auto activity = GetAndroidActivityAsActivity();
      int sensorType = m_sensorInfo.type;
      std::function<void(SensorEvent& event)> listenerWrapper = [listener, sensorType](SensorEvent& event){
        // here can be handled some parameters changes for specific sensors to keep compatibility between platforms
        switch(sensorType)
        {
          // axis to -axis (similar to iOS)
          case SENSOR_TYPE::TYPE_ACCELEROMETER:
            event.values[0] = -event.values[0] / 9.80665f; // in 1G scale
            event.values[1] = -event.values[1] / 9.80665f;
            event.values[2] = -event.values[2] / 9.80665f;
          break;
          default: break;
        }
        listener(event);
      };
      std::unique_ptr<std::function<void(SensorEvent& event)>> listenerUPtr(new std::function<void(SensorEvent& event)>(listenerWrapper));
      std::function<void(SensorEvent& event)>* rawPtr = listenerUPtr.get();
      m_listeners.push_back(std::move(listenerUPtr));
      getJniClass("com/aexolgl/android/hdw/HDWBridge").staticCallVoid("addListener", sensorID, (long long int)rawPtr, activity);
    }
  }
}
