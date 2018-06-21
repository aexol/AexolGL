/** @file TouchEvent.hpp
 *  @brief Definition of TouchEvent.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __AexolGL__TouchEvent__
#define __AexolGL__TouchEvent__

#include <aex/Event.hpp>

namespace aex
{
    struct AEX_PUBLIC_API FingerEventData
    {
        int64_t touchId;
        int64_t fingerId;
        float x; 
        float y;
        float dx;
        float dy;
        float pressure;
    };

    class AEX_PUBLIC_API TouchEvent : public Event
    {
    public:
        /**
         *	@brief	Called on Touch Event
         *
         *	@param 	e 	Event data
         */
        void handle(void* e);

        /**
         *	@brief	Called n finger down event.
         *
         *	@param 	e 	Event data
         */
        virtual void finger_down_event(FingerEventData data) { UNUSED(data); }

        /**
         *	@brief	Called on finger up event.
         *
         *	@param 	e 	Event data.
         */
        virtual void finger_up_event(FingerEventData data) { UNUSED(data); }

        /**
         *	@brief	Called on finger motion event.
         *
         *	@param 	e 	Event data.
         */
        virtual void finger_motion_event(FingerEventData data) { UNUSED(data); }

    };
}

#endif /* defined(__AexolGL__TouchEvent__) */
