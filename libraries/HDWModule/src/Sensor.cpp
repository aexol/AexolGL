#include "../include/aex/hdw/Sensor.hpp"
namespace aex
{
  namespace hdw
  {
    std::ostream& operator << (std::ostream& out, const SensorInfo& si)
    {
      out << "Sensor:" << std::endl;
      out << "name: " << si.name << std::endl;
      out << "typename: " << sensorTypeToString(si.type);
      return out;
    }

    std::ostream& operator << (std::ostream& out, const SensorEvent& se)
    {
      out << "[";
      for(int i = 0 ; i < se.values.size() ; i++)
      {
        out << se.values[i];
        if(i+1 < se.values.size())
          out << ",";
      }
      out << "]";
      return out;
    }

    Sensor::Sensor(SensorInfo sensorInfo, int sensorID) :
    m_sensorInfo(sensorInfo),
    m_sensorID(sensorID)
    {
    }

    Sensor::~Sensor()
    {
      nativeDestructor(m_sensorID);
    }

    SensorInfo Sensor::getSensorInfo() const
    {
      return m_sensorInfo;
    }

    void Sensor::start()
    {
      nativeStart(m_sensorID);
    }

    void Sensor::stop()
    {
      nativeStop(m_sensorID);
    }

    void Sensor::addListener(const std::function<void(const SensorEvent& event)>& listener)
    {
       nativeAddListener(m_sensorID, listener);
    }

    std::string sensorTypeToString(SENSOR_TYPE type)
    {
      switch(type)
      {
        case SENSOR_TYPE::NONE:
          return "none";
        case SENSOR_TYPE::TYPE_ACCELEROMETER:
          return "accelerometer";
        case SENSOR_TYPE::TYPE_GYROSCOPE:
          return "gyroscope";
        case SENSOR_TYPE::TYPE_MAGNETIC_FIELD:
          return "magnetic_field";
        case SENSOR_TYPE::TYPE_MOTION_DETECT:
          return "motion_detect";
        case SENSOR_TYPE::TYPE_LIGHT:
          return "light";
        default:
          return "none";
      }
    }
  }
}
