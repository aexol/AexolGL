#include "../include/SDLAccelEventGenerator.hpp"
#include <aex/Math.hpp>
#include <aex/Event.hpp>
#include "../../include/internal_sdl.hpp"
namespace aex

{
void
SDLAccelEventGenerator::generateEvent(float x, float y, float z)
{
    AccelerometerEventData* accelerometerEventData =
        new AccelerometerEventData {aex::math::Vector3{x, y, z}};
    auto sdl_accel_type = Event::getAccelerometerEventType();
    SDL_Event sdlEvent;
    SDL_zero(sdlEvent);
    sdlEvent.type = sdl_accel_type;
    sdlEvent.user.code = AEX_ACCELEROMETER_STATE_CHANGE;
    sdlEvent.user.data1 = accelerometerEventData;
    SDL_PushEvent(&sdlEvent);
}
}
