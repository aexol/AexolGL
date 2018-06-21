#include "../../include/aex/hdw/Sensor.hpp"
#include "MotionManager.hh"

namespace aex
{
  namespace hdw
  {
    void Sensor::nativeDestructor(int sensorID)
    {
      iosRemoveSensor(sensorID);
    }

    void Sensor::nativeStart(int sensorID)
    {
      iosStartSensor(sensorID);
    }

    void Sensor::nativeStop(int sensorID)
    {
      iosStopSensor(sensorID);
    }

    void Sensor::nativeAddListener(int sensorID, const std::function<void(const SensorEvent& event)>& listener)
    {
      int sensorType = m_sensorInfo.type;
      std::function<void(SensorEvent& event)> listenerWrapper = [listener, sensorType](SensorEvent& event){
        // here can be handled some parameters changes for specific sensors to keep compatibility between platforms
        // switch(sensorType)
        // {
        //   default: break;
        // }
        listener(event);
      };
      std::unique_ptr<std::function<void(SensorEvent& event)>> listenerUPtr(new std::function<void(SensorEvent& event)>(listenerWrapper));

      std::function<void(SensorEvent& event)>* rawPtr = listenerUPtr.get();
      m_listeners.push_back(std::move(listenerUPtr));
      iosAddSensorListener(sensorID, *rawPtr);
    }
  }
}
