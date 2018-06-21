//
//  EventDispatcher.cpp
//  AexolGL
//
//  Created by Kamil Matysiewicz on 22.08.2013.
//
//

#include <aex/EventDispatcher.hpp>
#include "../include/internal_sdl.hpp"
#include "aex/Exceptions.hpp"
namespace aex
{
    const Sint32 EventDispatcher::EVENT_COUNT = 10;

    EventDispatcher::EventDispatcher() :
        m_events
    {
        EVENT_COUNT
    }
    {
        std::fill(m_events.begin(), m_events.end(), nullptr);
    }

    EventDispatcher::~EventDispatcher()
    {

    }

    void
    EventDispatcher::dispatch(void* e, Sint32 event_id)
    {
        /* Check if sane ID */
        if(event_id >= 0 && event_id < EVENT_COUNT)
        {
            /* Check if handler defined and call it */
            if(m_events[ event_id ])
            {
                m_events[ event_id ]->handle(e);
            }

            return;
        }

        /* FIXME: Try guessing handler based on type, only predefined events */

        Uint32 type = static_cast<SDL_Event*>(e)->type & 0xFFF0;

        //Handle native SDL events
        switch(type)
        {
        case 0x0100:
        {
            if(m_events[ 0 ])
            {
                m_events[ 0 ]->handle(e);
            }
        }

        return;

        case 0x0200:
        {
            if(m_events[ 1 ])
            {
                m_events[ 1 ]->handle(e);
            }
        }

        return;

        case 0x0300:
        {
            if(m_events[ 2 ])
            {
                m_events[ 2 ]->handle(e);
            }
        }

        return;

        case 0x0400:
        {
            if(m_events[ 3 ])
            {
                m_events[ 3 ]->handle(e);
            }
        }

        return;

            /*
            case 0x0500:
            {
                if( m_events[ 4 ] )
                    m_events[ 4 ]->handle(e);
            }
            //*/
        return;

        case 0x0600:
        {
            if(m_events[ 4 ])
            {
                m_events[ 4 ]->handle(e);
            }
        }

        return;

        case 0x0650:
        {
            if(m_events[ 5 ])
            {
                m_events[ 5 ]->handle(e);
            }
        }

        return;

        case 0x0700:
        {
            if(m_events[ 6 ])
            {
                m_events[ 6 ]->handle(e);
            }
        }

        return;

        default:
            break;
        }

        if(static_cast<SDL_Event*>(e)->type == Event::getAccelerometerEventType())
        {
            AccelerometerEventData* data = (AccelerometerEventData*)static_cast<SDL_Event*>(e)->user.data1;

            if(m_events[aex::EventType::ACCELEROMETER])
            {
                m_events[aex::EventType::ACCELEROMETER]->handle(e);
            }

            delete data;
        }
        else
        {
            if(m_events[aex::EventType::USER_DEFINED])
            {
                m_events[aex::EventType::USER_DEFINED]->handle(e);
            }
        }
    }

    void
    EventDispatcher::add_event_handler(Event* e, EventType::Event e_type)
    {
        /* Check if sane ID */
        if(e_type < 0 || e_type > EventType::UNSUPPORTED)
        {
            throw UnsupportedEnumError("add_event_handler  EventType::UNSUPPORTED");
        }
        
        if(e_type >= 0 && e_type < EVENT_COUNT)
        {
            /* Check if handler defined and call it */
            if(m_events.size() < (size_t)e_type + 1)
            {
                m_events.resize(e_type + 1);
            }

            m_events[ e_type ] = e;
        }
    }
}
