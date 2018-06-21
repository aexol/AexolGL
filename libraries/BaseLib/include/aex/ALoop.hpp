//
//  ALoop.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 01.08.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#ifndef __aexolGL__ALoop__
#define __aexolGL__ALoop__
#include <aex/Common.hpp>
#include <aex/Event.hpp>
#include <aex/EventDispatcher.hpp>
#include <memory>
#include <list>
#include <map>
#include <mutex>

namespace aex
{
    class ALoopNode;
    class EventDispatcher;

    class AEX_PUBLIC_API ALoop
    {
        typedef ALoopNode* Node_ptr;
        typedef std::list<Node_ptr> NodeList;
        typedef std::map <Node_ptr, const std::function<void()> > CallbackList;
    public:
        ALoop();
        ALoop(ALoop&& al);
        ALoop(const ALoop& al);
        ALoop& operator=(ALoop&& al);
        ALoop& operator=(const ALoop& al);
        /**
         *	@brief	Adds new node to game loop
         *
         *	@param 	Node_ptr 	new node shared pointer
         *
         *	@return	bool
         */
        bool add_node(Node_ptr node);

        void remove_node(Node_ptr node , const std::function<void()> & );
        void remove_node(Node_ptr node);
        /**
         *	@brief	run game loop
         *
         */
        void run();
        /**
         * @brief
         */
        void step();
        /**
         * @brief
         *
         * @return
         */
        bool once();

        /**
         * @brief Pause loop.
         */
        void pause();


        /**
         * @brief Resume loop.
         */
        void resume();

        /**
         *	@brief	Removing node from runing list and adds it to suspended list
         *
         */
        void suspend_node(Node_ptr);

        /**
         *	@brief	Removing node from suspended list and adds it in to run list
         *
         *	@param 	Node_ptr 	<#Node_ptr description#>
         */
        void resume_node(Node_ptr);

        /**
         *	@brief	Set all nade as done in this frame.
         */
        void finish();

        /**
         *  @depracated
         *	@brief	Adds new event handler
         *
         *	@param 	std::unique_ptr<Event>& 	unique pointer to event
         *	@param 	EventType::Event 	Set event type
         */
        void add_event_handler(Event* e, EventType::Event);

    protected:
        void                                run_node(Node_ptr);
    private:
        void   _suspendNode(Node_ptr node);
        void   _resumeNode(Node_ptr node);
        void   _runNode(Node_ptr node);
        void   handleEvent(void* e);
        
    protected:
        bool                                m_done;
        bool                                m_pause;
        bool                                m_resume;
        mutable std::mutex                  m_pauseLock;
        NodeList                            m_running_nodes;
        NodeList                            m_suspended_nodes;
        EventDispatcher                     m_event_dispatcher;
        NodeList                            m_newNodes;
        CallbackList                        m_removeCallbackNodes;
    };
}

#endif /* defined(__aexolGL__ALoop__) */
