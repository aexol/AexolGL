#include "../../include/aex/hdw/SensorManager.hpp"
#include "MotionManager.hh"
#include <iostream>
#include <sstream>

namespace aex
{
  namespace hdw
  {

    std::vector<SensorInfo> SensorManager::nativeGetAvailableSensors()
    {
      std::vector<SensorInfo> sensorsInfos;

      if(iosGetMotionManager().accelerometerAvailable)
      {
        SensorInfo si;
        si.name = sensorTypeToString(SENSOR_TYPE::TYPE_ACCELEROMETER);
        si.type = SENSOR_TYPE::TYPE_ACCELEROMETER;
        sensorsInfos.push_back(si);
      }
      if(iosGetMotionManager().gyroAvailable)
      {
        SensorInfo si;
        si.name = sensorTypeToString(SENSOR_TYPE::TYPE_GYROSCOPE);
        si.type = SENSOR_TYPE::TYPE_GYROSCOPE;
        sensorsInfos.push_back(si);
      }
      if(iosGetMotionManager().magnetometerAvailable)
      {
        SensorInfo si;
        si.name = sensorTypeToString(SENSOR_TYPE::TYPE_MAGNETIC_FIELD);
        si.type = SENSOR_TYPE::TYPE_MAGNETIC_FIELD;
        sensorsInfos.push_back(si);
      }
      if(iosGetMotionManager().deviceMotionAvailable)
      {
        SensorInfo si;
        si.name = sensorTypeToString(SENSOR_TYPE::TYPE_MOTION_DETECT);
        si.type = SENSOR_TYPE::TYPE_MOTION_DETECT;
        sensorsInfos.push_back(si);
      }

      return sensorsInfos;
    }

    std::unique_ptr<Sensor> SensorManager::nativeGetDefaultSensor(SENSOR_TYPE type)
    {
      if(nativeDefaultSensorAvailable(type))
      {
        if(!iosIsSensorCreatedWith(type))
        {
          SensorInfo si;
          si.name = sensorTypeToString(type);
          si.type = type;
          si.nativeType = 0; // no native types, api defines all

          int sensorID = iosCreateSensor(type);
          return std::unique_ptr<Sensor>(new Sensor(si, sensorID));
        }
      }

      return std::unique_ptr<Sensor>(); // nullptr
    }

    std::unique_ptr<Sensor> SensorManager::nativeGetSensor(const SensorInfo& sensorInfo)
    {
      return nativeGetDefaultSensor(sensorInfo.type); // same, no vendor sensors like in android -> one vendor == one api
    }

    bool SensorManager::nativeDefaultSensorAvailable(SENSOR_TYPE type)
    {
      switch(type)
      {
        case SENSOR_TYPE::TYPE_ACCELEROMETER: return iosGetMotionManager().accelerometerAvailable;
        case SENSOR_TYPE::TYPE_GYROSCOPE: return iosGetMotionManager().gyroAvailable;
        case SENSOR_TYPE::TYPE_MAGNETIC_FIELD: return iosGetMotionManager().magnetometerAvailable;
        case SENSOR_TYPE::TYPE_MOTION_DETECT: return iosGetMotionManager().deviceMotionAvailable;
        case SENSOR_TYPE::TYPE_LIGHT: return false;
        default: return false;
      }
    }
  }
}
