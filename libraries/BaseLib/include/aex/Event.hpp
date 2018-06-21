//
//  Event.h
//  AexolGL
//
//  Created by Kamil Matysiewicz on 22.08.2013.
//
//

#ifndef __AexolGL__Event__
#define __AexolGL__Event__
#include <aex/Vector3.hpp>
namespace aex
{
    /**
     *	@brief	Struct containing some generalised system event types. ( Input, Window, App state, etc.)
     */
    struct AEX_PUBLIC_API EventType
    {

        enum Event
        {
            STATE_CHANGE = 0,        /**< APP state (background, foreground, etc  */

            /* Window events */
            WINDOW = 1, /**< Window state change */

            /* Keyboard events */
            KEYBOARD = 2,           /**<KEYBOARD type EVENT*/

            /* Mouse events */
            MOUSE = 3,              /**<MOUSE type EVENT*/

            /* Joystick events */
            JOYSTICK = 4,            /**JOYSTICK type EVENT*/

            /* Game controller events */
            CONTROLLER = 5, /**CONTROLLER type EVENT*/

            /* Touch events */
            TOUCH = 6,       /**TOUCH type EVENT*/

            /* Gesture events */
            GESTURE = 7,     /**GESTURE type EVENT*/

            ACCELEROMETER = 8,

            USER_DEFINED = 9,    /**USER defined events */

            UNSUPPORTED = 10    /**UNSUPPORTED events */
        };
    };
//Additional event types
    static CONSTEXPR size_t AEX_DEFINED_EVENT_COUNT = 0x01;
    static CONSTEXPR size_t AEX_ACCELEROMETER_EVENT = 0x01;
//Acceleremoter event codes
    static CONSTEXPR size_t AEX_ACCELEROMETER_STATE_CHANGE = 0x00;
    struct AEX_PUBLIC_API AccelerometerEventData
    {
        aex::math::Vector3 axes;
    };
    class AEX_PUBLIC_API Event
    {
    public:
        static uint32_t getAccelerometerEventType();
    protected:
        friend class EventDispatcher;
        static bool isAccelerometerEvent(void* e);
        //Extract data from event, on success event is invalidated.
        static AccelerometerEventData getAccelDataFromEvent(void* e);
//        static Uint32 getAccelerometerEventType();
        virtual void handle(void* e) = 0;
    };
}

#endif /* defined(__AexolGL__Event__) */
