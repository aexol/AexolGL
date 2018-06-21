
//
//  AccelEvent.cpp
//  AexolGL
//
//  Created by Kamil Matysiewicz on 27.08.2013.
//
//

#include <aex/AccelEvent.hpp>
#include "../include/internal_sdl.hpp"
namespace aex
{
    void
    AccelEvent::handle(void* e)
    {
        SDL_Event* ev = static_cast<SDL_Event*>(e);

        switch(ev->user.code)
        {
        case AEX_ACCELEROMETER_STATE_CHANGE:
            on_accel_state_change(*(AccelerometerEventData*)ev->user.data1);
            break;

        default:
            break;
        }
    }
}
