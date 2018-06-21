//
//  GameObjectInternalEvent.h
//  AexolGL
//
//  Created by Kamil Matysiewicz on 24.10.2013.
//
//

#ifndef AexolGL_GameObjectInternalEvent_h
#define AexolGL_GameObjectInternalEvent_h

#include <aex/GameObjectNode.hpp>
#include <aex/Common.hpp>

namespace aex
{
    class AEX_PUBLIC_API GameObjectInternalEvent
    {
    public:
        virtual aex::string getEventType() = 0;
    };
    typedef shared_ptr<GameObjectInternalEvent> InternalEventPtr;
}


#endif
