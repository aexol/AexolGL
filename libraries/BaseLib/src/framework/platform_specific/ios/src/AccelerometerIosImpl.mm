#import "../include/iOSHWSensor.hh"
#include <aex/Accelerometer.hpp>
#include "../../include/SDLAccelEventGenerator.hpp"

namespace aex
{
    typedef iOSHWSensor<iOSAccelDevice<SDLAccelEventGenerator>> AAccel;

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
        AAccel& accel = AAccel::GetInstanceForThread();
        accel.enable();
    }
    void
    Accelerometer::stopSampling()
    {

        AAccel& accel = AAccel::GetInstanceForThread();
        accel.disable();
    }
    void
    Accelerometer::update()
    {
        AAccel& accel = AAccel::GetInstanceForThread();
        accel.poll();
    }
    void
    Accelerometer::setSamplingRate(int rate)
    {
        AAccel& accel = AAccel::GetInstanceForThread();
        accel.setPollingRate(rate);
    }
}

