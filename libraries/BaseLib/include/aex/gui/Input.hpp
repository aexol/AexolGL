#ifndef INPUT_AEX_GUI_HEADER
#define INPUT_AEX_GUI_HEADER

#include "GUIMath.hpp"

#include <aex/KeyboardEvent.hpp>
#include <aex/TouchEvent.hpp>
#include <aex/MouseEvent.hpp>
#include <aex/GestureHandler.hpp>
#include <aex/InputDispatchLoopNode.hpp>

namespace aex {
    namespace gui {

        // Interface defined as input for Frame and inner components - similar
        class AEX_PUBLIC_API TouchInterface {
        public:
            virtual void touchDown(float x, float y, int fingerId) = 0;
            virtual void touchMotion(float x, float y, float dx, float dy, int fingerId) = 0;
            virtual void touchUp(float x, float y, int fingerId) = 0;
            virtual void touchLine(float x1, float y1, float x2, float y2, int fingerId) = 0;
            virtual void touchClick(float x, float y, int fingerId) = 0;
            virtual void touchScale(float scale) = 0;
        };

        class AEX_PUBLIC_API InputInterface {
        public:
            virtual void key_down_event(KeyboardEventData keyEvent) = 0;
            virtual void key_up_event(KeyboardEventData keyEvent) = 0;
            virtual void key_textedit_event(TextEditEventData textEditEvent) = 0;
            virtual void key_textinput_event(TextInputEventData textInputEvent) = 0;
        };

        class AEX_PUBLIC_API MouseInterface {
        public:
          virtual void mouse_down_event(MouseButtonEventData e) = 0;
          virtual void mouse_up_event(MouseButtonEventData e) = 0;
          virtual void mouse_motion_event(MouseMotionEventData e) = 0;
          virtual void mouse_wheel_event(MouseWheelEventData e) = 0;
        };


        // ----------------- classes for handling input registered in dispatcher ---------------------
        class AEX_PUBLIC_API TouchEventLayer;
        class AEX_PUBLIC_API KeyboardEventLayer;
        class AEX_PUBLIC_API MouseEventLayer;
        // class AEX_PUBLIC_API WindowEventLayer; // TODO in future if needed

        class AEX_PUBLIC_API LayeredTouchEvent : public aex::TouchEvent
        {
        public:
          void finger_down_event(aex::FingerEventData data);
          void finger_up_event(aex::FingerEventData data);
          void finger_motion_event(aex::FingerEventData data);

          void addEventLayer(std::unique_ptr<TouchEventLayer> layer);
        protected:
          std::vector<std::unique_ptr<TouchEventLayer>> m_eventLayers;
        };

        class AEX_PUBLIC_API LayeredKeyboardEvent : public aex::KeyboardEvent
        {
        public:
          void key_down_event(KeyboardEventData keyEvent);
          void key_up_event(KeyboardEventData keyEvent);
          void key_textedit_event(TextEditEventData textEditEvent);
          void key_textinput_event(TextInputEventData textInputEvent);

          void addEventLayer(std::unique_ptr<KeyboardEventLayer> layer);
        protected:
          std::vector<std::unique_ptr<KeyboardEventLayer>> m_eventLayers;
        };

        class AEX_PUBLIC_API LayeredMouseEvent : public aex::MouseEvent
        {
        public:
          void mouse_down_event(MouseButtonEventData e);
          void mouse_up_event(MouseButtonEventData e);
          void mouse_motion_event(MouseMotionEventData e);
          void mouse_wheel_event(MouseWheelEventData e);

          void addEventLayer(std::unique_ptr<MouseEventLayer> layer);

        protected:
          std::vector<std::unique_ptr<MouseEventLayer> > m_eventLayers;
        };

        class AEX_PUBLIC_API TouchEventLayer
        {
        public:
          virtual bool finger_down_event(aex::FingerEventData data) = 0;
          virtual bool finger_up_event(aex::FingerEventData data) = 0;
          virtual bool finger_motion_event(aex::FingerEventData data) = 0;
        };

        class AEX_PUBLIC_API KeyboardEventLayer
        {
        public:
          virtual bool key_down_event(KeyboardEventData keyEvent) = 0;
          virtual bool key_up_event(KeyboardEventData keyEvent) = 0;
          virtual bool key_textedit_event(TextEditEventData textEditEvent) = 0;
          virtual bool key_textinput_event(TextInputEventData textInputEvent) = 0;
        };

        class AEX_PUBLIC_API MouseEventLayer
        {
        public:
          virtual bool mouse_down_event(MouseButtonEventData e) = 0;
          virtual bool mouse_up_event(MouseButtonEventData e) = 0;
          virtual bool mouse_motion_event(MouseMotionEventData e) = 0;
          virtual bool mouse_wheel_event(MouseWheelEventData e) = 0;
        };

        class AEX_PUBLIC_API GUITouchEvent : public TouchEventLayer, public GestureHandler {
        public:
            GUITouchEvent(TouchInterface* touchInterface);
            bool finger_down_event(aex::FingerEventData data);
            bool finger_up_event(aex::FingerEventData data);
            bool finger_motion_event(aex::FingerEventData data);

            // GestureHandler Callbacks
            virtual void touchDownCallback(float x, float y, int fingerId);
            virtual void touchMotionCallback(float x, float y, float dx, float dy, int fingerId);
            virtual void touchUpCallback(float x, float y, int fingerId);
            virtual void lineCallback(float x1, float y1, float x2, float y2, int fingerId);
            virtual void clickCallback(float x, float y, int fingerId);
            virtual void scaleCallback(float scale);
        private:
            TouchInterface* m_touchInterface;
        };

        class AEX_PUBLIC_API GUIKeyboardEvent : public KeyboardEventLayer {
        public:
            GUIKeyboardEvent(InputInterface* inputInterface);
            bool key_down_event(KeyboardEventData keyEvent);
            bool key_up_event(KeyboardEventData keyEvent);
            bool key_textedit_event(TextEditEventData textEditEvent);
            bool key_textinput_event(TextInputEventData textInputEvent);
        private:
            InputInterface* m_inputInterface;
        };

        class AEX_PUBLIC_API GUIMouseEvent : public MouseEventLayer {
        public:
          GUIMouseEvent(MouseInterface* mouseInterface);
          bool mouse_down_event(MouseButtonEventData e);
          bool mouse_up_event(MouseButtonEventData e);
          bool mouse_motion_event(MouseMotionEventData e);
          bool mouse_wheel_event(MouseWheelEventData e);
        private:
          MouseInterface* m_mouseInterface;
        };

        class AEX_PUBLIC_API GUIMouseAsTouchEvent : public MouseEventLayer
        {
        public:
          GUIMouseAsTouchEvent(TouchInterface* touchInterface,  int wXPix, int wYPix);
          bool mouse_down_event(MouseButtonEventData e);
          bool mouse_up_event(MouseButtonEventData e);
          bool mouse_motion_event(MouseMotionEventData e);
          bool mouse_wheel_event(MouseWheelEventData e);
        private:
          TouchInterface* m_touchInterface;
          float downX, downY;
          uint8_t downButton;
          bool isDown;
          int windowXPix, windowYPix;
        };
        // ----------------------------------------------------------------------------------------


        // ============ Listeners api for every single components input (mouse,touch) events handling =============
        class AEX_PUBLIC_API TouchEvent {
        public:
            TouchEvent(long long componentID, Position screenPosition, Position innerPosition, Position movement, int fingerID);
            long long componentID;
            Position screenPosition;
            Position innerPosition;
            Position movement;
            int fingerID;
        };

        class AEX_PUBLIC_API Listener {
        public:
        };

        class AEX_PUBLIC_API TouchActionListener : public Listener {
        public:
            virtual void actionTouchDown(const TouchEvent& e) = 0;
            virtual void actionTouchMotion(const TouchEvent& e) = 0;
            virtual void actionTouchUp(const TouchEvent& e) = 0;
            virtual void actionTouchClick(const TouchEvent& e) = 0;
        };
        // ============================================================================
    }
}

#endif
