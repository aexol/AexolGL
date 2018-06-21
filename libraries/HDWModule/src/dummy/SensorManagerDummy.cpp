#include "../../include/aex/hdw/SensorManager.hpp"

namespace aex
{
  namespace hdw
  {
    std::vector<SensorInfo> SensorManager::nativeGetAvailableSensors()
    {
      return std::vector<SensorInfo>();
    }

    std::unique_ptr<Sensor> SensorManager::nativeGetDefaultSensor(SENSOR_TYPE type)
    {
      return std::unique_ptr<Sensor>();
    }

    std::unique_ptr<Sensor> SensorManager::nativeGetSensor(const SensorInfo& sensorInfo)
    {
      return std::unique_ptr<Sensor>();
    }

    bool SensorManager::nativeDefaultSensorAvailable(SENSOR_TYPE type)
    {
      return false;
    }
  }
}
