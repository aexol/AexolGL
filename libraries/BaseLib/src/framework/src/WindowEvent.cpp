//
//  WindowEvent.cpp
//  AexolGL
//
//  Created by Kamil Matysiewicz on 29.08.2013.
//
//

#include <aex/WindowEvent.hpp>
#include "../include/internal_sdl.hpp"

namespace aex
{
void
WindowEvent::handle(void* e)
{
    SDL_Event* ev = static_cast<SDL_Event*>(e);

    switch(ev->window.event)
    {
    case SDL_WINDOWEVENT_RESIZED:
    {
        WindowResizeData new_size {ev->window.data1, ev->window.data2};
        resize_event(new_size);
        break;
    }

    default:
        break;
    }
}
}
