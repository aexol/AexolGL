//
//  MouseEvent.cpp
//  AexolGL
//
//  Created by Kamil Matysiewicz on 27.08.2013.
//
//

#include <aex/MouseEvent.hpp>
#include "../include/internal_sdl.hpp"

namespace aex
{
    void
    MouseEvent::handle(void* e)
    {
        SDL_Event* sdlEvent = static_cast<SDL_Event*>(e);

        switch(sdlEvent->type)
        {
        case SDL_MOUSEBUTTONDOWN:
        {
            SDL_MouseButtonEvent& motionEvent = sdlEvent->button;
            MouseButtonEventData md
            {
                motionEvent.timestamp,
                motionEvent.windowID,
                motionEvent.which,
                motionEvent.button,
                motionEvent.state,
                motionEvent.clicks,
                motionEvent.x,
                motionEvent.y
            };
            mouse_down_event(md);
            break;
        }

        case SDL_MOUSEBUTTONUP:
        {
            SDL_MouseButtonEvent& motionEvent = sdlEvent->button;
            MouseButtonEventData md
            {
                motionEvent.timestamp,
                motionEvent.windowID,
                motionEvent.which,
                motionEvent.button,
                motionEvent.state,
                motionEvent.clicks,
                motionEvent.x,
                motionEvent.y
            };
            mouse_up_event(md);
            break;
        }

        case SDL_MOUSEMOTION:
        {
            SDL_MouseMotionEvent& motionEvent = sdlEvent->motion;
            MouseMotionEventData md
            {
                motionEvent.timestamp,
                motionEvent.windowID,
                motionEvent.which,
                motionEvent.state,
                motionEvent.x,
                motionEvent.y,
                motionEvent.xrel,
                motionEvent.yrel,
            };
            mouse_motion_event(md);
            break;
        }

        case SDL_MOUSEWHEEL:
        {
            SDL_MouseWheelEvent& wheelEvent = sdlEvent->wheel;
            MouseWheelEventData md
            {
                wheelEvent.timestamp,
                wheelEvent.windowID,
                wheelEvent.which,
                wheelEvent.x,
                wheelEvent.y,

            };
            mouse_wheel_event(md);
            break;
        }

        default:
            break;
        }
    }


    bool isLeftButton(MouseButtonEventData* e)
    {
        if(e->button == SDL_BUTTON_LEFT)
        {
            return true;
        }

        return false;
    }
    bool isRightButton(MouseButtonEventData* e)
    {
        if(e->button == SDL_BUTTON_RIGHT)
        {
            return true;
        }

        return false;
    }

    bool isMiddleButton(MouseButtonEventData* e)
    {
        if(e->button == SDL_BUTTON_MIDDLE)
        {
            return true;
        }

        return false;
    }
    bool is4thButton(MouseButtonEventData* e)
    {
        if(e->button == SDL_BUTTON_X1)
        {
            return true;
        }

        return false;
    }

    bool is5thButton(MouseButtonEventData* e)
    {
        if(e->button == SDL_BUTTON_X2)
        {
            return true;
        }

        return false;
    }

}
