/** @file EventHandler.hpp
 *  @brief Definition of EventHandler.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef AEXOLGL_EVENTHANDLER_H
#define AEXOLGL_EVENTHANDLER_H

#include <aex/MemberFunctionHandler.hpp>
#include <aex/TypeInfo.hpp>
#include <aex/Common.hpp>
#include <aex/Logger.hpp>
#include <functional>
#include <map>
#include <list>
#include <cstddef>

typedef decltype(nullptr) nullptr_t;

typedef decltype(nullptr) nullptr_t;
namespace aex
{

    /**
     *	@brief	EventHandler handles communication between objects.
     *  @details EventHandler object keeps track of objects and methods that are supposed to be called when an specific event takes place.
     */
    class AEX_PUBLIC_API EventHandler
    {
    protected:
        typedef shared_ptr<HandlerFunctionBase> HandlerFunctionPtr;

        struct AEX_PUBLIC_API SafeHandleErase
        {

            SafeHandleErase(HandlerFunctionPtr&& h) : handle(h), Active(true)
            {
            }
            HandlerFunctionPtr handle;
        private:
            bool Active;
            friend class EventHandler;
        };
        typedef std::list<SafeHandleErase> ListOfHandlers;
        typedef std::map<TypeInfo, ListOfHandlers> Handlers;

        /**
         *	@brief	List of handlers.
         */
        Handlers m_handlers;
    public:
        /**
         *	@brief	Destuctor
         */
        ~EventHandler();

        /**
         *	@brief	Dispatches events to listeners
         *
         *	@param 	event 	Event to be dispatched.
         */
        void handleEvent(const GameEvent* event);

        /**
         *	@brief	Template method used for registering event handler methods.
         *
         *  @details Method template creating function object and adding it to list of handlers, which are called when specific type of event is detected.
         *
         *	@param 	obj 	Pointer to a listener.
         *	@param 	T::*memFn 	Pointer to a method that handles particular event
         */
        template<class T, class EventT> typename ListOfHandlers::reverse_iterator
        registerEventFunc(T* obj, void (T::*memFn)(EventT*));

        template<class EventT> typename ListOfHandlers::reverse_iterator
        registerEventFunc( std::function<void(EventT*)> func);

        template<class T, class EventT> typename ListOfHandlers::reverse_iterator
        unregisterEventFunc(T* obj, void (T::*memFn)(EventT*));

        template<class T> void
        unregisterObject(T* obj);

    public:
        typedef ListOfHandlers::reverse_iterator entry_reverse_iterator;
    };

    template<class T, class EventT> typename EventHandler::entry_reverse_iterator
    EventHandler::registerEventFunc(T* obj, void (T::*memFn)(EventT*))
    {
        HandlerFunctionBase* handle = new MemberFunctionHandler<T, EventT>(std::bind(memFn, obj, std::placeholders::_1), obj);
        ListOfHandlers& listOfHandlers = m_handlers[TypeInfo(typeid(EventT))];

        listOfHandlers.push_back(SafeHandleErase(HandlerFunctionPtr(handle)));
        return listOfHandlers.rbegin();
    }

    template< class EventT> typename EventHandler::entry_reverse_iterator
    EventHandler::registerEventFunc( std::function<void(EventT*)> func)
    {
        HandlerFunctionBase* handle = new MemberFunctionHandler<nullptr_t, EventT>(func);
        ListOfHandlers& listOfHandlers = m_handlers[TypeInfo(typeid(EventT))];

        listOfHandlers.push_back(SafeHandleErase(HandlerFunctionPtr(handle)));
        return listOfHandlers.rbegin();
    }

    template<class T, class EventT> typename EventHandler::entry_reverse_iterator
    EventHandler::unregisterEventFunc(T* obj, void (T::*memFn)(EventT*))
    {

        ListOfHandlers& listOfHandlers = m_handlers[TypeInfo(typeid(EventT))];

        for(ListOfHandlers::iterator iter = listOfHandlers.begin(); iter != listOfHandlers.end(); iter++)
        {

            if(iter->handle->isSameObject(obj))
            {
                iter->Active = false;
            }

        }

        return listOfHandlers.rbegin();
    }

    template<class T> void
    EventHandler::unregisterObject(T* obj)
    {

        for(auto& handlers : m_handlers)
        {

            ListOfHandlers& listOfHandlers = handlers.second;

            for(ListOfHandlers::iterator iter = listOfHandlers.begin(); iter != listOfHandlers.end(); iter++)
            {

                if(iter->handle->isSameObject(obj))
                {
                    iter->Active = false;
                    //LOG_DEBUG("object unregister");
                }
            }
        }
    }
}

#endif
