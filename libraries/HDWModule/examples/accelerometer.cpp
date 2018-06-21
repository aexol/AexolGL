#include <aex/AexolGL.hpp>
#include <aex/hdw/SensorManager.hpp>

using namespace aex::hdw;

int main()
{
  std::unique_ptr<Sensor> sensor = sensorManager.getDefaultSensor(SENSOR_TYPE::TYPE_ACCELEROMETER);
  if(sensor)
  {
    LOG("THERE IS SENSOR:", sensor->getSensorInfo().name);
    sensor->addListener([](const SensorEvent& ev){
      std::stringstream ss;
      ss << "[";
      for(int i = 0 ; i < ev.values.size() ; i++)
      {
        ss << ev.values[i];
        if(i+1 < ev.values.size())
          ss << ", ";
      }
      ss << "]";
      LOG(ss.str());
    });
    sensor->start(); // on app pause stop() method is needed to avoid battery drain
  }else
  {
    // NO SENSOR:
    LOG("!!! NO SENSOR:", sensorTypeToString(SENSOR_TYPE::TYPE_ACCELEROMETER));
  }
  return 0;
}
