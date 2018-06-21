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
    
    struct AEX_PUBLIC_API GestureEventData
    {
        
    };
    
    class AEX_PUBLIC_API GestureEvent : public Event
    {
    public:

        void handle(void* e);


        virtual void gesture(GestureEventData* e) { }


    };
}

#endif /* defined(__AexolGL__TouchEvent__) */
