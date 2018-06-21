#ifndef __AEX_iOSHWSensor_HH
#define __AEX_iOSHWSensor_HH

#include <aex/ThreadLocalSingleton.hpp>
#include <aex/Singleton.hpp>
#include <aex/Event.hpp>
#include <boost/lockfree/queue.hpp>
#include <array>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <CoreMotion/CoreMotion.h>

namespace aex
{
    class MotionManager : public Singleton<MotionManager>
    {
    protected:
        MotionManager();
    public:
        CMMotionManager* sharedManager();
    private:
        CMMotionManager* m_manager;
    };
    template<class DeviceClass>
    class iOSHWSensor : public ThreadLocalSingleton<iOSHWSensor<DeviceClass>>
    {
    protected:
        iOSHWSensor();
    public:
        void enable();
        void disable();
        void poll();
        void setPollingRate(int rate);
    private:
        CMMotionManager* m_manager;
        static const NSTimeInterval secToMicrosec;
        DeviceClass m_device;
        friend class ThreadLocalStorage<iOSHWSensor<DeviceClass>>; //FIXME: Do something about protected ctor
    };
    template<class DeviceClass>
    const NSTimeInterval iOSHWSensor<DeviceClass>::secToMicrosec = 0.000001;
    template<class DeviceClass>
    iOSHWSensor<DeviceClass>::iOSHWSensor()
    {
        MotionManager& mm = MotionManager::GetInstance();
        m_manager = mm.sharedManager();
    }
    template<class DeviceClass>
    void iOSHWSensor<DeviceClass>::enable()
    {
        if(m_device.is_good(m_manager))
        {
            m_device.startUpdates(m_manager);
        }
    }
    template<class DeviceClass>
    void iOSHWSensor<DeviceClass>::disable()
    {
        if(m_device.is_active(m_manager))
        {
            m_device.stopUpdates(m_manager);
        }
    }
    template<class DeviceClass>
    void iOSHWSensor<DeviceClass>::poll()
    {
        m_device.push();
    }
    template<class DeviceClass>
    void iOSHWSensor<DeviceClass>::setPollingRate(int rate)
    {
        NSTimeInterval tRate = secToMicrosec * rate;
        m_device.setDeviceEventRate(m_manager, tRate);
    }


    template<class EventGenerator>
    struct iOSAccelDevice
    {
        using value_type = std::array<float, 3>;
        iOSAccelDevice() : m_queuedInput(0) {}
        bool is_good(CMMotionManager* manager)
        {
            return [manager isAccelerometerAvailable] == YES;
        }
        bool is_active(CMMotionManager* manager)
        {
            return [manager isAccelerometerActive] == YES;
        }
        void startUpdates(CMMotionManager* manager)
        {
            [manager startAccelerometerUpdatesToQueue:[NSOperationQueue mainQueue] withHandler: ^ (CMAccelerometerData * accelerometerData, NSError * error)
            {
                m_queuedInput.push(value_type
                {
                    {
                        static_cast<float>(accelerometerData.acceleration.x),
                        static_cast<float>(accelerometerData.acceleration.y),
                        static_cast<float>(accelerometerData.acceleration.z)
                    }
                });
            }];
        }
        void stopUpdates(CMMotionManager* manager)
        {
            [manager stopAccelerometerUpdates];
        }
        void setDeviceEventRate(CMMotionManager* manager, NSTimeInterval t)
        {
            static const NSTimeInterval accelMin = 0.01;

            if(t < accelMin)
            {
                t = accelMin;
            }

            [manager setAccelerometerUpdateInterval:t];
        }
        void push()
        {
            EventGenerator eg;
            value_type accelData;

            while(m_queuedInput.pop(accelData))
            {
                eg.generateEvent(accelData[0], accelData[1], accelData[2]);
            }
        }
    private:
        using queue_type = boost::lockfree::queue<value_type, boost::lockfree::fixed_sized<false> >;
        queue_type m_queuedInput;

    };
}

#endif
