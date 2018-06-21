#include "../include/ALooperWrapper.hpp"
#include "../include/AHWSensor.hpp"
#include <aex/Accelerometer.hpp>
#include "../../include/SDLAccelEventGenerator.hpp"

namespace aex
{
    typedef AHWSensor<AndroidAccel<SDLAccelEventGenerator>> AAccel;

    Accelerometer::Accelerometer()
    {
    }
    Accelerometer::~Accelerometer() {}
    void
    Accelerometer::init()
    {

    }
    void
    Accelerometer::startSampling()
    {
        ALooperWrapper& looper = ALooperWrapper::GetInstanceForThread();
        AAccel& accel = AAccel::GetInstanceForThread();
        accel.enable();
        looper.add(&accel);
    }
    void
    Accelerometer::stopSampling()
    {

        ALooperWrapper& looper = ALooperWrapper::GetInstanceForThread();
        AAccel& accel = AAccel::GetInstanceForThread();
        looper.remove(&accel);
        accel.disable();
    }
    void
    Accelerometer::update()
    {
        ALooperWrapper& looper = ALooperWrapper::GetInstanceForThread();
        looper.exec();
    }
    void
    Accelerometer::setSamplingRate(int rate)
    {
        AAccel& accel = AAccel::GetInstanceForThread();
        accel.setPollingRate(rate);
    }
}

