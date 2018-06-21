#include <aex/AexolGL.hpp>
#include <aex/hdw/SensorManager.hpp>

using namespace aex::hdw;

int main()
{
//  turn on ALL available sensors - a little bit crazy
  SensorManager sensorManager;
  std::vector<std::unique_ptr<Sensor>> allSensors;
  for(int i = 0 ; i < availableSensors.size() ; i++)
  {
    std::unique_ptr<Sensor> sensor = sensorManager.getSensor(availableSensors[i]);
    if(!sensor)
      continue;
    Sensor* sensorPtr = sensor.get();
    sensor->addListener([sensorPtr](const SensorEvent& ev){
      std::stringstream ss;
      ss << "[";
      for(int j = 0 ; j < ev.values.size() ; j++)
      {
         ss << ev.values[j];
         if(j+1 < ev.values.size() )
          ss << ", ";
      }
      ss << "]";
      LOG(sensorPtr->getSensorInfo().name,": ", ss.str());
    });
    allSensors.push_back(std::move(sensor));
  }
  for(auto& sensor : allSensors)
  {
    sensor->start();
  }
  return 0;
}
