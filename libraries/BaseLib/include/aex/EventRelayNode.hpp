#ifndef AEXOLGL_EVENTREALYNODE
#define AEXOLGL_EVENTREALYNODE

#include <aex/Common.hpp>
#include <aex/GameEvent.hpp>
#include <aex/GlobalGameEvents.hpp>
#include <aex/GameObject.hpp>
#include <aex/GameObjectNode.hpp>

namespace aex
{

    class AEX_PUBLIC_API EventRelayNode;

    template<class EventT>
    struct AEX_PUBLIC_API RelayEventT : public GameEvent
    {
        typedef EventT event_type;

        RelayEventT(const EventT* e, const EventRelayNode* p) : m_event(e), m_ptr(p)
        {
        }

        bool looped(const EventRelayNode* p) const
        {
            return m_ptr == p;
        }
        static_assert(std::is_base_of<GameEvent, EventT>::value, "EventT must be derived from GameEvent");
        const EventT* m_event;
    private:
        const EventRelayNode* m_ptr;
    };

    /**
     *	@brief EventRelayNode is a GameObjectNode responsible for connecting GlobalEventSystem with local object events. What it does is pretty much just listening on both local and global wrappers for specific events and depending on what developer wants to acomplish posting it either in local scope or global scope for  other game objects to interpret it in a way they wish.
     */
    class AEX_PUBLIC_API EventRelayNode : public GameObjectNode
    {

        template<typename T>
        struct AEX_PUBLIC_API is_packed
        {

            enum
            {
                value = false
            };
        };
        template<typename T>
        struct AEX_PUBLIC_API is_packed<RelayEventT<T>>
        {
            enum
            {
                value = true
            };
        };


    public:

        /**
         *	@brief	Constructor takes pointers to object owning the relay node and pointer to global event system.
         *
         *	@param 	parent 	pointer to parent object
         *	@param 	globalLink 	pointer to Global event system
         *
         *	@return	<#return value description#>
         */
        EventRelayNode(GameObject* parent, GlobalGameEventsPtr globalLink) : GameObjectNode(parent), m_globalLink(
                globalLink)
        {
        }

        EventRelayNode();

        /**
         *	@brief	addPassInsideEvent method takes no parameters except for template argument
         * defining handler method. After a call to this method relay node will
         * listen for events on global event system for EvenT type of event and pass
         * it around inside the game object.
         *
         * For ex. eventRelayNodeObject.addPassInsideEvent<SuperBigExplosionEventType>();
         *
         * makes eventrelaynode listen for SuperBigExplosionEventType events
         *
         *
         */
        template<class EventT>
        void addPassInsideEvent();


        /**
         *	@brief addPassOutsideEvent method, like it's counterpart addPassInsideEvent, takes no parameters
         * except for template argument. After a call to this, relay node will listen for local events
         * of EventT type and will broadcast it, when it occurs to the world.
         *
         * for ex. eventRelayNodeObject.addPassOutsideEvent<SuperBigExplosionEventType>();
         *
         * Will make relay node listen for SuperBigExplosionEventType event inside of an object (generated for ex. by logic module)
         *
         *
         */
        template<class EventT>
        void addPassOutsideEvent();

        ~EventRelayNode();

    protected:
        GlobalGameEventsPtr m_globalLink;

        //std::list<void*> m_revRelayEvents;

        /*
         *	@brief	realyIn( const EventT* e ) is a default behaviour fr relay node when a global event of type EventT occurs. Default behaviour is basicaly nothing more than just forwarding the event without any modifications inside an object.
         *
         *	@param 	e 	event to pass
         */
        //template< class EventT , typename>
        //void relayIn(const EventT* e);



        /**
         *	@brief	relayOut( const EventT* e ) is a default behaviour for relay node when a local game event of type EventT occurs. Default behaviour is nothing more than just forwarding an event to world without any modifications.
         *
         *	@param 	e 	event to pass
         */
        template<class EventT>
        void relayOut(const EventT* e);

        template<class EventT, typename std::enable_if<is_packed<EventT>::value, int>::type = 0>
        void
        relayIn(const EventT* e)
        {
            if(!e->looped(this))
            {
                Post(e->m_event);
            }
        }

        template < class EventT, typename std::enable_if < !is_packed<EventT>::value, int >::type = 0 >
        void
        relayIn(const EventT* e)
        {
            Post(e);
        }


    };

    typedef shared_ptr<EventRelayNode> EventRelayNode_ptr;


    template<class EventT> void
    EventRelayNode::addPassInsideEvent()
    {
        typedef void (EventRelayNode::*EventFunctionType)(const EventT*);
        typedef void (EventRelayNode::*REventFunctionType)(const RelayEventT<EventT>*);
        EventFunctionType evFunc = &EventRelayNode::relayIn<EventT>;
        REventFunctionType revFunc = &EventRelayNode::relayIn<RelayEventT<EventT>>;

        m_globalLink->registerEventFunc(this, evFunc);
        m_globalLink->registerEventFunc(this, revFunc);
    }

    template<class EventT> void
    EventRelayNode::addPassOutsideEvent()
    {

        typedef void (EventRelayNode::*EventFunctionType)(const EventT*);
        EventFunctionType evFunc = &EventRelayNode::relayOut<EventT>;
        registerEventFunc(this, evFunc);
    }

    template<class EventT> void
    EventRelayNode::relayOut(const EventT* e)
    {
        RelayEventT<EventT>* re = new RelayEventT<EventT>(e, this);
        m_globalLink->handleEvent(re);
        delete re;
    }

}
#endif
