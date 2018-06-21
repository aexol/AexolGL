//
//  GameObjectNode.h
//  AexolGL
//
//  Created by Kamil Matysiewicz on 24.10.2013.
//
//

#ifndef AexolGL_GameObjectNode_h
#define AexolGL_GameObjectNode_h

#include <aex/Common.hpp>
#include <aex/GameObject.hpp>

namespace aex
{

    /**
     *	@brief	 Main idea is to make a game object ( Monster, Player, Tree, whatever)
     *  to be made from lil parts, each responsible for their own stuff
     *  (Rendering, Logic, AI, Sound, Physics, etc.) and keeping them separate as much as possible.
     *  Communication between those items will happen based on local events
     *  defined by on-need basis ( for ex. EVENT_MOVE ), each object will be allowed to
     *  interpret event and data it carries(if any) in it's own way.
     *  his class is a base class for all those parts that will make up
     *  a game object.
     */
    class AEX_PUBLIC_API GameObjectNode
    {
    public:
        typedef shared_ptr<GameObject> GameObjectPtr;

        /**
         *	@brief Constructor, takes pointer to GameObject wrapping up all parts of an object in game and allowing communication between each part.
         *
         *	@param 	parent 	pointer to parent GameObject
         */
        GameObjectNode(GameObject* parent) : m_parent(parent) {}


        template <class T, class EventT>
        void registerEventFunc(T* obj, void (T::*memFn)(EventT*));

        void Post(const GameEvent* e)
        {
            m_parent->handleEvent(e);
        }

        virtual ~GameObjectNode() {}
    protected:
        GameObject* m_parent;
    };

    template <class T, class EventT> void
    GameObjectNode::registerEventFunc(T* obj, void (T::*memFn)(EventT*))
    {
        m_parent->registerEventFunc(obj, memFn);
    }

    typedef shared_ptr<GameObjectNode> GameObjectNodePtr;

}

#endif
