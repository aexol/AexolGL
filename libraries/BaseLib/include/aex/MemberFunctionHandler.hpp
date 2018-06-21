#ifndef MEMBERFUNCTIONHANDLER_H
#define MEMBERFUNCTIONHANDLER_H

#include <aex/HandlerFunctionBase.hpp>
#include <functional>

namespace aex
{

//!
//! Template class used to wrap a call to a specific
//! method to an Event. Should not be accessed
//! directly, but by inheriting EventHandler/GameObjectNode
//!

    template <typename T, class EventT>
    class MemberFunctionHandler : public HandlerFunctionBase
    {
    public:


        //typedef void (T::*MemberFunc)(EventT*);
        typedef std::function<void(EventT*)> MemberFunc;

        MemberFunc GetFunc()
        {
            return m_function;
        }


        MemberFunctionHandler(MemberFunc memFn , T* obj) : m_function(memFn) , m_objectPointer(obj) {}
        
        void call(const GameEvent* e)
        {
            return m_function(static_cast<EventT*>(e));
        }

        T* GetObjectPointer() const
        {
            return m_objectPointer;
        }

        bool isSameObject(void* pointerObject)
        {
            return static_cast<void*>(m_objectPointer) == pointerObject;
        }

    private:
        MemberFunc m_function;
        T* m_objectPointer;
    };
}

#endif
