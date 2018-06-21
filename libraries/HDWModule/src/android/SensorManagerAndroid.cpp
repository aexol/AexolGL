#include "../../include/aex/hdw/SensorManager.hpp"
#include "JNIUtils.hpp"
#include <aex/Logger.hpp>
#include <json/json.h>
#include <sstream>
#include <iostream>

namespace aex
{
  namespace hdw
  {
    std::vector<SensorInfo> SensorManager::nativeGetAvailableSensors()
    {
      std::string resultJson;
      auto activity = GetAndroidActivityAsActivity();
      resultJson = getJniClass("com/aexolgl/android/hdw/HDWBridge").staticCall("getAvailableSensors", resultJson, activity);

      LOG(resultJson);

      std::vector<SensorInfo> resultInfos;

      Json::Value root;
      Json::Reader reader;
      if (!reader.parse(resultJson, root)) {
          LOG_ERR("--(!): nativeGetAvailableSensors(): ", reader.getFormattedErrorMessages());
          return resultInfos;
      }

      for (const Json::Value& next : root) {
        int sensorCode = next["sensorCode"].asInt();
        int nativeType = next["sensorType"].asInt();
        std::string sensorName = next["sensorName"].asString();
        SensorInfo si;
        si.type = (SENSOR_TYPE)sensorCode;
        si.name = sensorName;
        si.nativeType = nativeType;
        resultInfos.push_back(si);
      }

      return resultInfos;
    }

    std::unique_ptr<Sensor> SensorManager::nativeGetDefaultSensor(SENSOR_TYPE type)
    {
      int sensorID = 0;
      auto activity = GetAndroidActivityAsActivity();
      sensorID = getJniClass("com/aexolgl/android/hdw/HDWBridge").staticCall("getDefaultSensor",  sensorID, (int)type, activity);
      if(sensorID != 0)
      {
        SensorInfo si;
        si.name = sensorTypeToString(type);
        si.type = type;
        return std::unique_ptr<Sensor>(new Sensor(si, sensorID));
      }

      return std::unique_ptr<Sensor>(); // nullptr
    }

    std::unique_ptr<Sensor> SensorManager::nativeGetSensor(const SensorInfo& sensorInfo)
    {
      int sensorID = 0;
      auto activity = GetAndroidActivityAsActivity();
      sensorID = getJniClass("com/aexolgl/android/hdw/HDWBridge").staticCall("getSensor",  sensorID, sensorInfo.nativeType, activity);
      if(sensorID != 0)
      {
        return std::unique_ptr<Sensor>(new Sensor(sensorInfo, sensorID));
      }

      return std::unique_ptr<Sensor>(); // nullptr
    }

    bool SensorManager::nativeDefaultSensorAvailable(SENSOR_TYPE type)
    {
      int exists = false;
      auto activity = GetAndroidActivityAsActivity();
      exists = getJniClass("com/aexolgl/android/hdw/HDWBridge").staticCall("defaultSensorExists", exists, (int)type, activity);
      return (bool)exists;
    }
  }
}
