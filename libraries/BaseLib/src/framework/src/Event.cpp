#include <aex/Event.hpp>
#include <mutex>
#include <algorithm>
#include <limits>
#include "../include/internal_sdl.hpp"

namespace aex
{
Uint32 userDefinedEvents[AEX_DEFINED_EVENT_COUNT];
std::once_flag eventRegisterFlag;
void registerAexEvents()
{
    userDefinedEvents[0] = SDL_RegisterEvents(AEX_DEFINED_EVENT_COUNT);

    if(userDefinedEvents[0] != std::numeric_limits<Uint32>::max())
    {
        for(uint32_t ii = 1; ii < AEX_DEFINED_EVENT_COUNT; ++ii)
        {
            userDefinedEvents[ii] = userDefinedEvents[ii - 1] + 1;
        }
    }
    else
    {
        std::fill(userDefinedEvents, userDefinedEvents + sizeof(userDefinedEvents), -1);
    }
}
bool
Event::isAccelerometerEvent(void* e)
{
    std::call_once(eventRegisterFlag, registerAexEvents);

    return e ? userDefinedEvents[0] != std::numeric_limits<Uint32>::max() && static_cast<SDL_Event*>(e)->type == userDefinedEvents[0] : false;
}
uint32_t
Event::getAccelerometerEventType()
{
    std::call_once(eventRegisterFlag, registerAexEvents);
    return userDefinedEvents[0];
}
AccelerometerEventData
Event::getAccelDataFromEvent(void* e)
{
    SDL_Event* sdlEvent = static_cast<SDL_Event*>(e);

    if(sdlEvent->type != getAccelerometerEventType())
        return AccelerometerEventData {};

    AccelerometerEventData* d = static_cast<AccelerometerEventData*>(sdlEvent->user.data1);

    AccelerometerEventData ret = *d;

    delete d;

    sdlEvent->user.data1 = nullptr;

    return ret;
}
}
