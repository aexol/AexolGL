//
//  GlobalEvent.h
//  AexolGL
//
//  Created by Kamil Matysiewicz on 24.10.2013.
//
//

#ifndef AexolGL_GlobalEvent_h
#define AexolGL_GlobalEvent_h

#include <aex/Common.hpp>
#include <aex/GlobalGameEvents.hpp>

namespace aex
{

    /**
     *	@brief GlobalEvent class is a Base class for objects capable
     * of generating/handling global game events. It can be useful
     * for example in conjunction with some input library, like SDL
     * or GLUT.
     */
    class AEX_PUBLIC_API GlobalEvent

    {
    public:
        typedef shared_ptr<GlobalGameEvents> GlobalGameEventsPtr;

        /**
         *	@brief	Constructor, takes pointer to GlobalGameEvents which handles communication between objects in game.
         *
         *	@param 	parent 	pointer to global game events object
         *
         */
        GlobalEvent(GlobalGameEventsPtr parent) : m_parent(parent) {}


        /**
         *	@brief	Register an and it's event handling method.
         *
         */
        template <class T, class EventT>
        void registerEventFunc(T* obj, void (T::*memFn)(EventT*));


        /**
         *	@brief	Post event for registered listeners.
         */
        void Post(const GameEvent* e)
        {
            m_parent->handleEvent(e);
        }


        virtual ~GlobalEvent() {}
    private:
        GlobalGameEventsPtr m_parent;
    };

    template <class T, class EventT> void
    GlobalEvent::registerEventFunc(T* obj, void (T::*memFn)(EventT*))
    {
        m_parent->registerEventFunc(obj, memFn);
    }

    typedef shared_ptr<GlobalEvent> GlobalEventPtr;

}

#endif
