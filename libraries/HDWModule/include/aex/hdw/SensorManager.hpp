#ifndef SENSOR_MANAGER_AEXOL_HDW_HEADER
#define SENSOR_MANAGER_AEXOL_HDW_HEADER

#include <iostream>
#include <string>
#include <vector>

#include "Sensor.hpp"

namespace aex
{
  namespace hdw
  {
    class SensorManager
    {
    public:
      SensorManager();
      virtual ~SensorManager();
      std::vector<SensorInfo> getAvailableSensors();
      std::unique_ptr<Sensor> getDefaultSensor(SENSOR_TYPE type);
      std::unique_ptr<Sensor> getSensor(const SensorInfo& sensorInfo);
      bool defaultSensorAvailable(SENSOR_TYPE type);

    protected:
      std::vector<SensorInfo> nativeGetAvailableSensors();
      std::unique_ptr<Sensor> nativeGetDefaultSensor(SENSOR_TYPE type);
      std::unique_ptr<Sensor> nativeGetSensor(const SensorInfo& sensorInfo);
      bool nativeDefaultSensorAvailable(SENSOR_TYPE type);
    };
  }
}

#endif
