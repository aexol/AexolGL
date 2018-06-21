#include "../include/aex/hdw/SensorManager.hpp"

namespace aex
{
  namespace hdw
  {
    SensorManager::SensorManager()
    {
    }

    SensorManager::~SensorManager()
    {
    }

    std::vector<SensorInfo> SensorManager::getAvailableSensors()
    {
      return nativeGetAvailableSensors();
    }

    std::unique_ptr<Sensor> SensorManager::getDefaultSensor(SENSOR_TYPE type)
    {
      return nativeGetDefaultSensor(type);
    }

    std::unique_ptr<Sensor> SensorManager::getSensor(const SensorInfo& sensorInfo)
    {
      return nativeGetSensor(sensorInfo);
    }

    bool SensorManager::defaultSensorAvailable(SENSOR_TYPE type)
    {
      return nativeDefaultSensorAvailable(type);
    }
  }
}
