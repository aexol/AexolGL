/* 
 * File:   InputEventLoopNode.hpp
 * Author: mritke
 *
 * Created on 10 lutego 2016, 16:11
 */

#ifndef INPUTEVENTDISPATCHLOOPNODE_HPP
#define INPUTEVENTDISPATCHLOOPNODE_HPP

#include <aex/AexolGL.hpp>
#include <aex/GlobalGameEvents.hpp>
#include <aex/BuildInEvents.hpp>

#include <aex/MouseEvent.hpp>
#include <aex/KeyboardEvent.hpp>
#include <aex/WindowEvent.hpp>
#include <aex/TouchEvent.hpp>

namespace aex {

    class EventDispatcher;

    class AEX_PUBLIC_API InputDispatchLoopNode : public ALoopNode {
    public:
        InputDispatchLoopNode();
        void setMouseHandler(MouseEvent* m);
        void setKeyboardHandler(KeyboardEvent* m);
        void setWindowHandler(WindowEvent* m);
        void setTouchHandler(TouchEvent* m);

    private:
        bool init();
        void pause();
        void resume();
        void update();
        void finish();

        MouseEvent* m_mouseEvents = nullptr;
        KeyboardEvent* m_keyboardEvents = nullptr;
        WindowEvent* m_windowEvents = nullptr;
        TouchEvent* m_touchEvents = nullptr;

        aex::unique_ptr<EventDispatcher> m_event_dispatcher;
    };

}

#endif /* INPUTEVENTDISPATCHLOOPNODE_HPP */

