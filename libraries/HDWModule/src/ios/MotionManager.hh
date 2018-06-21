#ifndef MOTION_MANAGER_AEX_HDW_HEADER
#define MOTION_MANAGER_AEX_HDW_HEADER

#include <map>
#include <exception>
#include <aex/Logger.hpp>
#import <CoreMotion/CMMotionManager.h>

namespace aex
{
  namespace hdw
  {

    struct SensorIOS
    {
      SENSOR_TYPE type;
      std::vector<std::function<void(SensorEvent& event)>> listeners;
      bool working;
    };

    int g_sensorsCounter = 0;
    std::map<int, SensorIOS> g_createdSensors;
    CMMotionManager* g_motionManager = 0; // global

    int iosGetNextSensorID()
    {
      return g_sensorsCounter++;
    }

    CMMotionManager* iosGetMotionManager()
    {
      if(g_motionManager == 0)
      {
         g_motionManager = [[CMMotionManager alloc] init];
      }
      return g_motionManager;
    }

    SensorIOS* iosGetSensorIOSPtr(int sensorID)
    {
      if(g_createdSensors.find(sensorID) != g_createdSensors.end())
      {
        return &g_createdSensors[sensorID];
      }
      return nullptr;
    }

    int iosCreateSensor(SENSOR_TYPE type)
    {
      SensorIOS sensorIOS;
      sensorIOS.type = type;
      sensorIOS.working = false;
      int sensorID = iosGetNextSensorID();
      g_createdSensors[sensorID] = sensorIOS;
      return sensorID;
    }

    bool iosIsSensorCreatedWith(SENSOR_TYPE type)
    {
      for(auto& elem : g_createdSensors)
      {
        if(elem.second.type == type)
        {
          return true;
        }
      }
      return false;
    }

    void iosAddSensorListener(int sensorID,const std::function<void(SensorEvent& event)>& listener)
    {
      SensorIOS* sensorIOS = iosGetSensorIOSPtr(sensorID);
      if(sensorIOS)
      {
        if(sensorIOS->working)
          throw new std::runtime_error("--(!): can't add listeners to working sensors, stop it first");

        sensorIOS->listeners.push_back(listener);
      }
    }

    void iosStartSensor(int sensorID)
    {
      SensorIOS* sensorIOS = iosGetSensorIOSPtr(sensorID);
      if(sensorIOS)
      {
        if( ! sensorIOS->working)
        {
          switch(sensorIOS->type)
          {
            case SENSOR_TYPE::TYPE_ACCELEROMETER:
            [iosGetMotionManager() startAccelerometerUpdatesToQueue:NSOperationQueue.mainQueue
                withHandler:^(CMAccelerometerData *accelerometerData,NSError *error)
                {
                  float x_1G = accelerometerData.acceleration.x; // http://www.endmemo.com/sconvert/m_s2g.php
                  float y_1G = accelerometerData.acceleration.y;
                  float z_1G = accelerometerData.acceleration.z;
                  SensorEvent event;
                  event.values = {x_1G, y_1G, z_1G};
                  for(auto& listener: sensorIOS->listeners)
                  {
                    listener(event);
                  }
                }
            ];
            break;
            case SENSOR_TYPE::TYPE_GYROSCOPE:
            [iosGetMotionManager() startGyroUpdatesToQueue:NSOperationQueue.mainQueue
                withHandler:^(CMGyroData *gyroData, NSError *error)
                {
                  float xRotRate = gyroData.rotationRate.x;
                  float yRotRate = gyroData.rotationRate.y;
                  float zRotRate = gyroData.rotationRate.z;

                  SensorEvent event;
                  event.values = {xRotRate, yRotRate, zRotRate};
                  for(auto& listener: sensorIOS->listeners)
                  {
                    listener(event);
                  }
                }
            ];
            break;
            case SENSOR_TYPE::TYPE_MAGNETIC_FIELD:
            [iosGetMotionManager() startMagnetometerUpdatesToQueue:NSOperationQueue.mainQueue
                withHandler:^(CMMagnetometerData *magnetometerData, NSError *error)
                {
                  float xMicroTeslas = magnetometerData.magneticField.x;
                  float yMicroTeslas = magnetometerData.magneticField.y;
                  float zMicroTeslas = magnetometerData.magneticField.z;

                  SensorEvent event;
                  event.values = {xMicroTeslas, yMicroTeslas, zMicroTeslas};
                  for(auto& listener: sensorIOS->listeners)
                  {
                    listener(event);
                  }
                }
            ];
            break;
            case SENSOR_TYPE::TYPE_MOTION_DETECT:
            [iosGetMotionManager() startDeviceMotionUpdatesToQueue:NSOperationQueue.mainQueue
                withHandler:^(CMDeviceMotion *motion, NSError *error)
                {
                  // attitude
                  float rollRadians = motion.attitude.roll;
                  float pitchRadians = motion.attitude.pitch;
                  float yawRadians = motion.attitude.yaw;

                  // rotationRate
                  float xRotRate = motion.rotationRate.x;
                  float yRotRate = motion.rotationRate.y;
                  float zRotRate = motion.rotationRate.z;

                  // gravity
                  float xGravity_1G = motion.gravity.x;
                  float yGravity_1G = motion.gravity.y;
                  float zGravity_1G = motion.gravity.z;

                  // userAcceleration
                  float xUserAcc_1G = motion.userAcceleration.x;
                  float yUserAcc_1G = motion.userAcceleration.y;
                  float zUserAcc_1G = motion.userAcceleration.z;

                  // magneticField
                  float xMagneticField = motion.magneticField.field.x;
                  float yMagneticField = motion.magneticField.field.y;
                  float zMagneticField = motion.magneticField.field.z;

                  // heading
                  //float heading0_360_deg = motion.heading;

                  SensorEvent event;
                  event.values =
                  { rollRadians, pitchRadians, yawRadians,
                    xRotRate, yRotRate, zRotRate,
                    xGravity_1G, yGravity_1G, zGravity_1G,
                    xUserAcc_1G, yUserAcc_1G, zUserAcc_1G,
                    xMagneticField, yMagneticField, zMagneticField
                  };

                  for(auto& listener: sensorIOS->listeners)
                  {
                    listener(event);
                  }
                }
            ];
            break;
            default: break;
          }
          sensorIOS->working = true;
        }
      }
    }

    void iosStopSensor(int sensorID)
    {
      SensorIOS* sensorIOS = iosGetSensorIOSPtr(sensorID);
      if(sensorIOS)
      {
        if(sensorIOS->working)
        {
          switch(sensorIOS->type)
          {
            case SENSOR_TYPE::TYPE_ACCELEROMETER: [iosGetMotionManager() stopAccelerometerUpdates]; break;
            case SENSOR_TYPE::TYPE_GYROSCOPE: [iosGetMotionManager() stopMagnetometerUpdates]; break;
            case SENSOR_TYPE::TYPE_MAGNETIC_FIELD: [iosGetMotionManager() stopMagnetometerUpdates]; break;
            case SENSOR_TYPE::TYPE_MOTION_DETECT: [iosGetMotionManager() stopDeviceMotionUpdates]; break;
            break;
            default: break;
          }
          sensorIOS->working = false;
        }
      }
    }

    void iosRemoveSensor(int sensorID)
    {
      iosStopSensor(sensorID);
      g_createdSensors.erase(sensorID);
    }
  }
}

#endif
