/** @file WindowEvent.hpp
 *  @brief Definition of WindowEvent.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef AexolGL_WindowEvent_h
#define AexolGL_WindowEvent_h

#include <aex/Event.hpp>

namespace aex
{
    struct AEX_PUBLIC_API WindowResizeData
    {
        int32_t w;
        int32_t h;
    };
    class AEX_PUBLIC_API WindowEvent : public Event
    {
    public:
        /**
         *	@brief	Called when window event happens.
         *
         *	@param 	e 	Event data.
         */
        void handle(void* e);

        /**
         *	@brief	Called on window resize event.
         *
         *	@param 	e 	Event data.
         */
        virtual void resize_event(WindowResizeData new_size) { UNUSED(new_size); }

    };
}



#endif
