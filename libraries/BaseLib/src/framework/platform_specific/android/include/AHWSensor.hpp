#ifndef __AEX_AHWSENSOR_HPP
#define __AEX_AHWSENSOR_HPP
#include "AConfiguration.hpp"
#include "AQueue.hpp"
#include <aex/Event.hpp>
#include <aex/AndroidAPI.hpp>
#include <aex/JniObject.hpp>
#include <aex/ThreadLocalSingleton.hpp>
#include <SDL.h>
#include <android/looper.h>
#include <android/sensor.h>
#include <thread>
#include <mutex>

namespace aex
{
static CONSTEXPR float G_FORCE = 9.81f;
static CONSTEXPR int AEX_LOOPER_ID_USER = 4;



template<class DeviceClass>
class AHWSensor : public ThreadLocalSingleton<AHWSensor<DeviceClass>>, public AQueue
{
protected:

public:
    AHWSensor();
    void enable();
    void disable();
    void poll();
    void setPollingRate(int rate);

private:
    ALooper* m_looper;
    ASensorEventQueue* m_eventQueue;
    ASensorManager* m_sensorManager;
    const ASensor* m_sensor;
    int m_minDelay;

    void init();
    static int notify(int fd, int events, void* data);
    void pushEvent();
    friend class ThreadLocalSingleton<AHWSensor<DeviceClass>>;
};

template<class DeviceClass>
AHWSensor<DeviceClass>::AHWSensor() :
    m_looper(nullptr),
    m_eventQueue(nullptr),
    m_sensorManager(nullptr)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    // Disable depracated warning for ASensorManager as it's only available
    // since API Level 26 (Android 8.0)
    m_sensorManager = ASensorManager_getInstance();
#pragma clang diagnostic pop
    m_sensor = ASensorManager_getDefaultSensor(m_sensorManager, DeviceClass {}
            .getDeviceType());
}

template<class DeviceClass>
void
AHWSensor<DeviceClass>::init()
{
    if(!m_looper)
    {
        m_looper = ALooper_forThread();

        if(!m_looper)
        {
            m_looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
        }
    }

    if(!m_eventQueue)
    {
        m_eventQueue = ASensorManager_createEventQueue(m_sensorManager, m_looper, AEX_LOOPER_ID_USER, notify, this);
    }
}

template<class DeviceClass>
void
AHWSensor<DeviceClass>::enable()
{
    init();

    if(m_eventQueue)
    {
        ASensorEventQueue_enableSensor(m_eventQueue, m_sensor);
        m_minDelay = ASensor_getMinDelay(m_sensor);
    }
}

template<class DeviceClass>
void
AHWSensor<DeviceClass>::disable()
{
    if(m_eventQueue)
    {
        ASensorEventQueue_disableSensor(m_eventQueue, m_sensor);
    }
}

template<class DeviceClass>
void
AHWSensor<DeviceClass>::poll()
{
    pushEvent();
}

template<class DeviceClass>
void
AHWSensor<DeviceClass>::setPollingRate(int rate)
{
    if(m_eventQueue)
    {
        if(rate < m_minDelay)
        {
            rate = m_minDelay;
        }

        ASensorEventQueue_setEventRate(m_eventQueue, m_sensor, rate);
    }
}

template<class DeviceClass>
int
AHWSensor<DeviceClass>::notify(int fd, int events, void* data)
{
    ((AHWSensor<DeviceClass>*)data)->pushEvent();
    return 1;
}

template<class DeviceClass>
void
AHWSensor<DeviceClass>::pushEvent()
{
    if(!m_eventQueue)
    {
        return;
    }

    ASensorEvent event;

    while(ASensorEventQueue_getEvents(m_eventQueue, &event, 1) > 0)
    {
        DeviceClass device;
        device.handle(event);
    }
}

template<class EventGenerator>
struct AndroidAccel
{

    CONSTEXPR int getDeviceType() const
    {
        return ASENSOR_TYPE_ACCELEROMETER;
    }

    void handle(const ASensorEvent& event)
    {
        EventGenerator eg;
        eg.generateEvent(event.acceleration.x, event.acceleration.y, event.acceleration.z);
    }
};

template<class EventGenerator>
struct AndroidGyro
{

    CONSTEXPR int getDeviceType() const
    {
        return ASENSOR_TYPE_GYROSCOPE;
    }

    void handle(const ASensorEvent& event)
    {
        EventGenerator eg;
        eg.generateEvent(event.vector.x / G_FORCE, event.vector.y / G_FORCE, event.vector.z / G_FORCE);
    }
};
}
#endif
