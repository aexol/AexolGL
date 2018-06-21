/** @file MouseEvent.hpp
 *  @brief Definition of MouseEvent.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __AexolGL__MouseEvent__
#define __AexolGL__MouseEvent__

#include <aex/Event.hpp>

namespace aex
{
    struct AEX_PUBLIC_API MouseEventData
    {
        uint32_t timestamp;
        uint32_t windowId;
        uint32_t which;
        MouseEventData(uint32_t t, uint32_t wId, uint32_t wh) :
            timestamp(t),
            windowId(wId),
            which(wh) {}
    };

    struct AEX_PUBLIC_API MouseWheelEventData : MouseEventData
    {
        int32_t x;
        int32_t y;
        MouseWheelEventData(uint32_t t, uint32_t wId, uint32_t wh , int32_t tx, int32_t ty) :
            MouseEventData(t, wId, wh),
            x(tx),
            y(ty)
        {}
    };

    struct AEX_PUBLIC_API MouseMotionEventData : MouseEventData
    {
        uint32_t state;
        int32_t x;
        int32_t y;
        int32_t xrel;
        int32_t yrel;
        MouseMotionEventData(uint32_t t, uint32_t wId, uint32_t wh, uint32_t st, int32_t xx, int32_t yy, int32_t xr, int32_t yr):
            MouseEventData(t, wId, wh),
            state(st),
            x(xx),
            y(yy),
            xrel(xr),
            yrel(yr)
        {}
    };
    struct AEX_PUBLIC_API MouseButtonEventData : MouseEventData
    {
        uint8_t button;
        uint8_t state;
        uint8_t clicks;
        int32_t x;
        int32_t y;
        MouseButtonEventData(uint32_t t, uint32_t wId, uint32_t wh, uint8_t b, uint8_t st, uint8_t c, int32_t xx, int32_t yy):
            MouseEventData(t, wId, wh),
            button(b),
            state(st),
            clicks(c),
            x(xx),
            y(yy)
        {}
    };



    class AEX_PUBLIC_API MouseEvent : public Event
    {
    public:
        /**
         *	@brief	Called n mouse down event.
         *
         *	@param 	e 	Event data
         */
        virtual void mouse_down_event(MouseButtonEventData e) { UNUSED(e); }

        /**
         *	@brief	Called on mouse up event.
         *
         *	@param 	e 	Event data.
         */
        virtual void mouse_up_event(MouseButtonEventData e) { UNUSED(e); }

        /**
         *	@brief	Called on mouse motion event.
         *
         *	@param 	e 	Event data.
         */
        virtual void mouse_motion_event(MouseMotionEventData e) { UNUSED(e); }

        /**
        *	@brief	Called on mouse wheel event.
        *
        *	@param 	e 	Event data.
        */
        virtual void mouse_wheel_event(MouseWheelEventData e) { UNUSED(e); }
    protected:
        /**
         *	@brief	Called on Mouse Event
         *
         *	@param 	e 	Event data
         */
        void handle(void* e);

    };

    bool AEX_PUBLIC_API isLeftButton(MouseButtonEventData* e);
    bool AEX_PUBLIC_API isRightButton(MouseButtonEventData* e);
    bool AEX_PUBLIC_API isMiddleButton(MouseButtonEventData* e);
    bool AEX_PUBLIC_API is4thButton(MouseButtonEventData* e);
    bool AEX_PUBLIC_API is5thButton(MouseButtonEventData* e);
}

#endif /* defined(__AexolGL__TouchEvent__) */
