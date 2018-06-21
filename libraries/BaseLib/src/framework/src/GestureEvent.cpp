#include <aex/GestureEvent.hpp>
#include <SDL.h>

namespace aex
{

    GestureEvent::GestureEvent()
    {

    }

    GestureEvent::handle(SDL_Event* e)
    {

        //LOG("event type: ", e->type);

        switch(e->type)
        {
            /*    case SDL_FINGERDOWN:
                    finger_down_event( e );
                    break;
                case SDL_FINGERUP:
                    finger_up_event( e );
                    break;
                case SDL_FINGERMOTION:
                    finger_motion_event( e );
                    break;
                default:
                    break;
            }*/
        }

    }