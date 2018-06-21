//
//  TouchEvent.cpp
//  AexolGL
//
//  Created by Kamil Matysiewicz on 27.08.2013.
//
//

#include <aex/TouchEvent.hpp>
#include <aex/Logger.hpp>
#include "../include/internal_sdl.hpp"

namespace aex
{
    void
    TouchEvent::handle(void* e)
    {

        SDL_TouchFingerEvent& ev = static_cast<SDL_Event*>(e)->tfinger;
        FingerEventData data
        {
            ev.touchId,
            ev.fingerId,
            ev.x,
            ev.y,
            ev.dx,
            ev.dy,
            ev.pressure
        };

        switch(ev.type)
        {
        case SDL_FINGERDOWN:
            finger_down_event(data);
            break;

        case SDL_FINGERUP:
            finger_up_event(data);
            break;

        case SDL_FINGERMOTION:
            finger_motion_event(data);
            break;

        default:
            break;
        }
    }
}
