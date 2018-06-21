/** @file EventDispatcher.hpp
 *  @brief Definition of EventDispatcher.hpp.
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __AexolGL__EventDispatcher__
#define __AexolGL__EventDispatcher__

#include <aex/Event.hpp>
#include <aex/Common.hpp>
#include <vector>

namespace aex
{
    /**
     *	@brief	Responsible for dispatching system events.
     *  @warning You shouldn't really ever need to create this one as long as you use our build in game loop.
     */
    class AEX_PUBLIC_API EventDispatcher
    {
    public:
        /**
         *	@brief	Constructor
         */
        EventDispatcher();
        ~EventDispatcher();
        void    dispatch(void* e, int32_t event_id = -1);
        /**
         *	@brief	Add handler to EventType::Event.
         *
         *	@param 	event 	Pointer to an object handling eventType event.
         *	@param 	eventType 	Type of event that is handled.
         */
        void    add_event_handler(Event* event, EventType::Event eventType);

    private:
        static const int32_t EVENT_COUNT;
        /**
         *	@brief	Events that we liten to.
         */
        std::vector<Event*>  m_events;
        /**
         *	@brief	Called in loop when system for all queued system events.
         *
         *	@param 	e 	Pointer to recived event.
         *	@param 	event_id    Type of event.
         */


        friend class ALoop;
    };
}

#endif /* defined(__AexolGL__EventDispatcher__) */
