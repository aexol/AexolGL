#include <aex/InputDispatchLoopNode.hpp>
#include "../include/internal_sdl.hpp"
#include <aex/Accelerometer.hpp>
namespace aex
{
    InputDispatchLoopNode::InputDispatchLoopNode():
        ALoopNode(),
        m_event_dispatcher(new EventDispatcher)
    {

    }
    
    bool
    InputDispatchLoopNode::init()
    {
        aex::Accelerometer accel;
        accel.init();
        accel.startSampling();
        accel.setSamplingRate(1000);
        return true;
    }
    
    void
    InputDispatchLoopNode::pause()
    {

    }
    
    void
    InputDispatchLoopNode::resume()
    {

    }
    
    void
    InputDispatchLoopNode::update()
    {

        SDL_Event event;
        aex::Accelerometer accel;
        accel.update();

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                if(SDL_GetEventState(SDL_QUIT) == SDL_ENABLE)
                {
                    SDL_Event event;
                    event.type = SDL_QUIT;
                    SDL_PushEvent(&event);
                    break;
                }
            }
            else
            {
                if(m_event_dispatcher)
                {
                    m_event_dispatcher->dispatch(&event);
                }
            }
          }
    }
    void
    InputDispatchLoopNode::finish()
    {

    }
    void
    InputDispatchLoopNode::setMouseHandler(aex::MouseEvent* m)
    {
        m_mouseEvents = m;
        m_event_dispatcher->add_event_handler(m, EventType::MOUSE);
    }

    void
    InputDispatchLoopNode::setKeyboardHandler(aex::KeyboardEvent* m)
    {
        m_keyboardEvents = m;
        m_event_dispatcher->add_event_handler(m, EventType::KEYBOARD);
    }

    void
    InputDispatchLoopNode::setWindowHandler(aex::WindowEvent* m)
    {
        m_windowEvents = m;
        m_event_dispatcher->add_event_handler(m, EventType::WINDOW);
    }

    void
    InputDispatchLoopNode::setTouchHandler(aex::TouchEvent* m)
    {
        m_touchEvents = m;
        m_event_dispatcher->add_event_handler(m, EventType::TOUCH);
    }
}
