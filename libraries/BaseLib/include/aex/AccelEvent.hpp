/** @file AccelEvent.hpp
 *  @brief Definition of AccelEvent.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __AexolGL__AccelEvent__
#define __AexolGL__AccelEvent__

#include <aex/Common.hpp>
#include <aex/Event.hpp>

namespace aex
{
    class AEX_PUBLIC_API AccelEvent : public Event
    {
    public:
        /**
         *	@brief	Called on Accel Event
         *
         *	@param 	e 	Event data
         */
        void handle(void* e);

        /**
         *	@brief	Called n mouse down event.
         *
         *	@param 	e 	Event data
         */
        virtual void on_accel_state_change(const AccelerometerEventData& e) { UNUSED(e); }
    };
}

#endif /* defined(__AexolGL__TouchEvent__) */
