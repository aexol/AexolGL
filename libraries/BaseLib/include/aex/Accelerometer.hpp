#ifndef __AEX_ACCELEROMETER_HPP
#define __AEX_ACCELEROMETER_HPP
#include <aex/Common.hpp>
namespace aex
{
    class AEX_PUBLIC_API Accelerometer
    {
    public:
        Accelerometer();
        void init();
        void update();

        void setSamplingRate(int rate);
        void startSampling();
        void stopSampling();
        ~Accelerometer();
        static bool HasAccelerometerSupport();
    };
}

#endif
