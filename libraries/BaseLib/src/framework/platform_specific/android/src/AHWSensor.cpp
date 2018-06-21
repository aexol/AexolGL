#include "AHWSensor.hpp"
#include "AConfiguration.hpp"
#include <aex/Event.hpp>
#include <aex/AndroidAPI.hpp>
#include <aex/JniObject.hpp>
#include <SDL.h>
#include <mutex>


namespace aex
{
AHWSensor::AHWSensor(int deviceType):
    m_deviceType(deviceType),
    m_looper(nullptr),
    m_sensorManager(nullptr),
    m_eventQueue(nullptr)
{
    m_looper = ALooper_forThread();

    if(!m_looper)
    {
        m_looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    }

    m_sensorManager = ASensorManager_getInstance();

    if(m_sensorManager && m_deviceType == ACCEL_DEVICE)
    {
        m_sensor = ASensorManager_getDefaultSensor(m_sensorManager, ASENSOR_TYPE_ACCELEROMETER);
    }

    if(m_sensorManager)
    {
        m_eventQueue = ASensorManager_createEventQueue(m_sensorManager, m_looper, LOOPER_ID_USER, notify, this);
    }
}

void
AHWSensor::enable()
{
    if(m_eventQueue)
    {
        ASensorEventQueue_enableSensor(m_eventQueue, m_sensor);
    }
}

void
AHWSensor::disable()
{
    if(m_eventQueue)
    {
        ASensorEventQueue_disableSensor(m_eventQueue, m_sensor);
    }
}

void
AHWSensor::poll()
{
    pushEvent();
}

void
AHWSensor::setPollingRate(int rate)
{
    if(m_eventQueue)
    {
        ASensorEventQueue_setEventRate(m_eventQueue, m_sensor, rate);
    }
}

int
AHWSensor::notify(int fd, int events, void* data)
{
    ((AHWSensor*)data)->pushEvent();
}

void
AHWSensor::pushEvent()
{
    ASensorEvent event;

    while(!(ASensorEventQueue_getEvents(m_eventQueue, &event, 1)))
    {
        if(m_deviceType == ASENSOR_TYPE_ACCELEROMETER)
        {
            AccelerometerEventData* accelerometerEventData =
                new AccelerometerEventData {aex::math::Vector3{event.acceleration.x, event.acceleration.y, event.acceleration.z}};
            auto sdl_accel_type = Event::getAccelerometerEventType();
            SDL_Event sdlEvent;
            SDL_zero(sdlEvent);
            sdlEvent.type = sdl_accel_type;
            sdlEvent.user.code = AEX_ACCELEROMETER_STATE_CHANGE;
            sdlEvent.user.data1 = accelerometerEventData;
            SDL_PushEvent(&event);
        }
    }
}
}
