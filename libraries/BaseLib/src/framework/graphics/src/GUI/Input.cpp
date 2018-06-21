#include "aex/gui/Input.hpp"
#include <aex/AexolGL.hpp>
#include <cmath>

namespace aex
{
    namespace gui
    {

        void LayeredTouchEvent::finger_down_event(aex::FingerEventData data)
        {
          for(auto& handler : m_eventLayers){
            if(handler->finger_down_event(data)) // if true, break
              break;
          }
        }

        void LayeredTouchEvent::finger_up_event(aex::FingerEventData data)
        {
          for(auto& handler : m_eventLayers)
            if(handler->finger_up_event(data))
              break;
        }

        void LayeredTouchEvent::finger_motion_event(aex::FingerEventData data)
        {
          for(auto& handler : m_eventLayers)
            if(handler->finger_motion_event(data))
              break;
        }

        void LayeredTouchEvent::addEventLayer(std::unique_ptr<TouchEventLayer> layer)
        {
          m_eventLayers.push_back(std::move(layer));
        }


        void LayeredKeyboardEvent::key_down_event(KeyboardEventData keyEvent)
        {
          for(auto& handler : m_eventLayers)
            if(handler->key_down_event(keyEvent))
              break;
        }

        void LayeredKeyboardEvent::key_up_event(KeyboardEventData keyEvent)
        {
          for(auto& handler : m_eventLayers)
            if(handler->key_up_event(keyEvent))
              break;
        }

        void LayeredKeyboardEvent::key_textedit_event(TextEditEventData textEditEvent)
        {
          for(auto& handler : m_eventLayers)
            if(handler->key_textedit_event(textEditEvent))
              break;
        }

        void LayeredKeyboardEvent::key_textinput_event(TextInputEventData textInputEvent)
        {
          for(auto& handler : m_eventLayers)
            if(handler->key_textinput_event(textInputEvent))
              break;
        }

        void LayeredKeyboardEvent::addEventLayer(std::unique_ptr<KeyboardEventLayer> layer)
        {
          m_eventLayers.push_back(std::move(layer));
        }


        void LayeredMouseEvent::mouse_down_event(MouseButtonEventData e)
        {
          for(auto& handler : m_eventLayers)
            if(handler->mouse_down_event(e))
              break;
        }

        void LayeredMouseEvent::mouse_up_event(MouseButtonEventData e)
        {
          for(auto& handler : m_eventLayers)
            if(handler->mouse_up_event(e))
              break;
        }

        void LayeredMouseEvent::mouse_motion_event(MouseMotionEventData e)
        {
          for(auto& handler : m_eventLayers)
            if(handler->mouse_motion_event(e))
              break;
        }

        void LayeredMouseEvent::mouse_wheel_event(MouseWheelEventData e)
        {
          for(auto& handler : m_eventLayers)
            if(handler->mouse_wheel_event(e))
              break;
        }

        void LayeredMouseEvent::addEventLayer(std::unique_ptr<MouseEventLayer> layer)
        {
          m_eventLayers.push_back(std::move(layer));
        }

        GUITouchEvent::GUITouchEvent(TouchInterface* touchInterface) : m_touchInterface(touchInterface)
        {
        }

        bool GUITouchEvent::finger_down_event(aex::FingerEventData e)
        {
            this->touchDownEvent(e.x, e.y, e.fingerId);
            return true;
        }

        bool GUITouchEvent::finger_up_event(aex::FingerEventData e)
        {
            this->touchUpEvent(e.x, e.y, e.fingerId);
            return true;
        }

        bool GUITouchEvent::finger_motion_event(aex::FingerEventData e)
        {
            this->touchMotionEvent(e.x, e.y, e.dx, e.dy, e.fingerId);
            return true;
        }

        // GestureHandler Callbacks
        void GUITouchEvent::touchDownCallback(float x, float y, int fingerId)
        {
            m_touchInterface->touchDown(x, y, fingerId);
        }

        void GUITouchEvent::touchMotionCallback(float x, float y, float dx, float dy, int fingerId)
        {
            m_touchInterface->touchMotion(x, y, dx, dy, fingerId);
        }

        void GUITouchEvent::touchUpCallback(float x, float y, int fingerId)
        {
            m_touchInterface->touchUp(x, y, fingerId);
        }

        void GUITouchEvent::lineCallback(float x1, float y1, float x2, float y2, int fingerId)
        {
            m_touchInterface->touchLine(x1, y1, x2, y2, fingerId);
        }

        void GUITouchEvent::clickCallback(float x, float y, int fingerId)
        {
            m_touchInterface->touchClick(x, y, fingerId);
        }

        void GUITouchEvent::scaleCallback(float scale)
        {
            m_touchInterface->touchScale(scale);
        }

        GUIKeyboardEvent::GUIKeyboardEvent(InputInterface* inputInterface) : m_inputInterface(inputInterface)
        {
        }

        bool GUIKeyboardEvent::key_down_event(KeyboardEventData e)
        {
            m_inputInterface->key_down_event(e);
            return true;
        }

        bool GUIKeyboardEvent::key_up_event(KeyboardEventData e)
        {
            m_inputInterface->key_up_event(e);
            return true;
        }

        bool GUIKeyboardEvent::key_textedit_event(TextEditEventData e)
        {
            m_inputInterface->key_textedit_event(e);
            return true;
        }

        bool GUIKeyboardEvent::key_textinput_event(TextInputEventData e)
        {
            m_inputInterface->key_textinput_event(e);
            return true;
        }


        GUIMouseEvent::GUIMouseEvent(MouseInterface* mouseInterface) :
        m_mouseInterface(mouseInterface)
        {
        }

        bool GUIMouseEvent::mouse_down_event(MouseButtonEventData e)
        {
          m_mouseInterface->mouse_down_event(e);
          return true;
        }

        bool GUIMouseEvent::mouse_up_event(MouseButtonEventData e)
        {
          m_mouseInterface->mouse_up_event(e);
          return true;
        }

        bool GUIMouseEvent::mouse_motion_event(MouseMotionEventData e)
        {
          m_mouseInterface->mouse_motion_event(e);
          return true;
        }

        bool GUIMouseEvent::mouse_wheel_event(MouseWheelEventData e)
        {
          m_mouseInterface->mouse_wheel_event(e);
          return true;
        }

        GUIMouseAsTouchEvent::GUIMouseAsTouchEvent(TouchInterface* touchInterface,  int wXPix, int wYPix) :
        m_touchInterface(touchInterface),
        isDown(false),
        windowXPix(wXPix),
        windowYPix(wYPix)
        {
        }

        bool GUIMouseAsTouchEvent::mouse_down_event(MouseButtonEventData e)
        {
          if(!isDown)
          {
            isDown = true;
            downButton = e.button;
            downX = (float)e.x / windowXPix;
            downY = (float)e.y / windowYPix;
            m_touchInterface->touchDown(downX, downY, e.button); //to fix here remeber just first click event data
          }

          return true;
        }
        bool GUIMouseAsTouchEvent::mouse_up_event(MouseButtonEventData e)
        {
          float upX = (float)e.x / windowXPix;
          float upY = (float)e.y / windowYPix;
          m_touchInterface->touchUp(upX, upY, e.button);
          if(e.button == downButton)
          {
            float distance = sqrt( pow(upX-downX, 2) + pow(upY-downY, 2));

            if(distance < 0.06f)
            {
                m_touchInterface->touchClick(downX, downY, e.button);
            }else
            {
                m_touchInterface->touchLine(downX, downY, upX, upY, e.button); //
            }

            isDown = false;
          }

          return true;
        }

        bool GUIMouseAsTouchEvent::mouse_motion_event(MouseMotionEventData e)
        {
          float tmpX = (float)e.x / windowXPix;
          float tmpY = (float)e.y / windowYPix;
          float xrel = (float)e.x / windowXPix;
          float yrel = (float)e.y / windowYPix;
          m_touchInterface->touchMotion(tmpX, tmpY, xrel, yrel, downButton);

          return true;
        }

        bool GUIMouseAsTouchEvent::mouse_wheel_event(MouseWheelEventData e)
        {
          return true;
        }

        TouchEvent::TouchEvent(long long componentID, Position screenPosition, Position innerPosition, Position movement, int fingerID) :
        componentID(componentID),
        screenPosition(screenPosition),
        innerPosition(innerPosition),
        movement(movement),
        fingerID(fingerID)
        {
        }
    }
}
