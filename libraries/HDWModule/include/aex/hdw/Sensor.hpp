#ifndef SENSOR_HDW_HEADER_FILE
#define SENSOR_HDW_HEADER_FILE

#include <iostream>
#include <string>
#include <vector>
#include <memory>

namespace aex
{
  namespace hdw
  {
    class Sensor;
    class SensorManager;
    enum SENSOR_TYPE // check with iOS sensors
    {
      NONE = 0,
      TYPE_ACCELEROMETER = 1,
      TYPE_GYROSCOPE = 2,
      TYPE_MAGNETIC_FIELD = 3,
      TYPE_MOTION_DETECT = 4,
      TYPE_LIGHT = 5
    };

    struct SensorInfo
    {
      friend class Sensor;
      friend class SensorManager;

      std::string name;
      SENSOR_TYPE type;

    private:
      int nativeType; // android, ios not default type code in sensor management
    public:
      friend std::ostream& operator << (std::ostream& out, const SensorInfo& si);
    };
    std::ostream& operator << (std::ostream& out, const SensorInfo& si);

    struct SensorEvent
    {
      std::vector<float> values;
      friend std::ostream& operator << (std::ostream& out, const SensorEvent& se);
    };
    std::ostream& operator << (std::ostream& out, const SensorEvent& se);

    class Sensor{
    public:
      friend class SensorManager;
    protected:
      Sensor(SensorInfo sensorInfo, int sensorID);
    public:
      SensorInfo getSensorInfo() const;
      void start();
      void stop();
      void addListener(const std::function<void(const SensorEvent& event)>& listener);
      virtual ~Sensor();
    public:
      
      Sensor() = delete;
      Sensor(const Sensor&) = delete;
      Sensor& operator=(const Sensor&) = delete;
      Sensor(Sensor&&) = delete;
      Sensor& operator=(Sensor&&) = delete;

    protected:
      void nativeDestructor(int sensorID);
      void nativeStart(int sensorID);
      void nativeStop(int sensorID);
      void nativeAddListener(int sensorID, const std::function<void(const SensorEvent& event)>& listener);
    protected:
      SensorInfo m_sensorInfo;
      int m_sensorID;
      std::vector<std::unique_ptr<std::function<void(SensorEvent&)>>> m_listeners;
    };

    std::string sensorTypeToString(SENSOR_TYPE type);
  }
}

#endif
