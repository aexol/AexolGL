#include "../../include/aex/hdw/Sensor.hpp"

namespace aex
{
  namespace hdw
  {
    void Sensor::nativeDestructor(int sensorID)
    {
    }

    void Sensor::nativeStart(int sensorID)
    {
    }

    void Sensor::nativeStop(int sensorID)
    {
    }

    void Sensor::nativeAddListener(int sensorID, const std::function<void(const SensorEvent& event)>& listener)
    {
    }
  }
}
