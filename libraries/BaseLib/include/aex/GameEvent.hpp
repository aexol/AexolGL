#ifndef GAMEEVENT_H
#define GAMEEVENT_H
#include <aex/Common.hpp>
#include <map>

namespace aex
{
//!
//! Default in-game event without any data.
//! Any new new game events added to the game
//! must inherit this class.
//!
    class AEX_PUBLIC_API GameEvent
    {
    protected:
        virtual ~GameEvent() = 0;
    };
    inline GameEvent::~GameEvent() {};

//typedef std::map<aex::string, GameEvent*> GameEventsMap;

// typedef  aex::shared_ptr<GameEvent> GameEvent_ptr;


}

#endif
