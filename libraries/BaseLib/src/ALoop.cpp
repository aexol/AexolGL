//
//  ALoop.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 01.08.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include <aex/ALoop.hpp>
#include <aex/ALoopNode.hpp>
#include <aex/EventDispatcher.hpp>
#include <aex/Logger.hpp>
#include "framework/include/internal_sdl.hpp"
namespace aex
{

    ALoop::ALoop() : m_done(false), m_pause(false), m_resume(false), m_event_dispatcher()
    {
    }

    ALoop::ALoop(const ALoop& al):
        m_pauseLock()
    {
        *this = al;
    }

    ALoop::ALoop(ALoop&& al):
        m_pauseLock()
    {
        *this = al;
    }

    ALoop&
    ALoop::operator=(const ALoop& al)
    {
        if(this != &al)
        {
            std::lock_guard<std::mutex> lock(al.m_pauseLock);
            
            m_done = al.m_done;
            m_pause = al.m_pause;
            m_resume = al.m_resume;
            m_running_nodes = al.m_running_nodes;
            m_suspended_nodes = al.m_suspended_nodes;
            m_newNodes = al.m_newNodes;
            m_event_dispatcher = al.m_event_dispatcher;
        }
        return *this;
    }

    ALoop&
    ALoop::operator=(ALoop&& al)
    {
        if(this != &al)
        {
            std::lock_guard<std::mutex> lock(al.m_pauseLock);
            
            std::swap(m_done, al.m_done);
            std::swap(m_pause, al.m_pause);
            std::swap(m_resume, al.m_resume);
            std::swap(m_running_nodes, al.m_running_nodes);
            std::swap(m_suspended_nodes, al.m_suspended_nodes);
            std::swap(m_newNodes, al.m_newNodes);
            std::swap(m_event_dispatcher, al.m_event_dispatcher);
        }
        return *this;
    }

    void
    ALoop::run_node(Node_ptr node)
    {
        //std::lock_guard<std::mutex> lock{m_pauseLock};
        m_newNodes.push_back(node);
    }

    void
    ALoop::_runNode(Node_ptr node)
    {
        if(!m_running_nodes.empty())
        {
            NodeList::iterator iter;
            for(iter = m_running_nodes.begin(); iter != m_running_nodes.end(); ++iter)
            {
                if((*iter)->m_priority < node->m_priority)
                {
                    break;
                }
            }
            m_running_nodes.insert(iter, node);
        }
        else
        {
            m_running_nodes.push_back(node);
        }
    }

    bool
    ALoop::add_node(Node_ptr node)
    {
        if(!node->init())
        {
            return false;
        }

        run_node(node);
        return true;
    }

    void ALoop::remove_node(Node_ptr node)
    {
        node->m_done = true;  
    }
    
    void
    ALoop::remove_node(Node_ptr node,const std::function<void()>& onDeleteCallback)
    {
        node->m_done = true;
        m_removeCallbackNodes.emplace(node, onDeleteCallback);
        
    }

    void
    ALoop::pause()
    {
        std::lock_guard<std::mutex> lock{m_pauseLock};
        m_pause = true;
        m_resume = false;
    }

    void
    ALoop::resume()
    {
        std::lock_guard<std::mutex> lock{m_pauseLock};
        m_resume = true;
        m_pause = false;
    }

    void
    ALoop::suspend_node(Node_ptr node)
    {
        std::lock_guard<std::mutex> lock{m_pauseLock};
        _suspendNode(node);
    }

    void
    ALoop::_suspendNode(Node_ptr node)
    {
        node->pause();
        m_running_nodes.remove(node);
        m_suspended_nodes.push_back(node);
    }

    void
    ALoop::resume_node(Node_ptr node)
    {
        std::lock_guard<std::mutex> lock{m_pauseLock};
        _resumeNode(node);
    }

    void
    ALoop::_resumeNode(Node_ptr node)
    {
        node->resume();
        m_suspended_nodes.remove(node);
        _runNode(node);
    }

    void
    ALoop::run()
    {
        while(!m_done)
        {
            step();
        }
    }

    void
    ALoop::step()
    {
        if(SDL_QuitRequested())
        {
            finish();
        }

        once();
    }

    bool
    ALoop::once()
    {
        // add new nodes
        while(!m_newNodes.empty())
        {
            _runNode(m_newNodes.front());
            m_newNodes.pop_front();
        }
              
        if(!m_running_nodes.empty())
        {
            NodeList::iterator iter = m_running_nodes.begin();

            while(iter != m_running_nodes.end())
            {
                if((*iter)->m_done)
                {
                    iter = m_running_nodes.erase(iter);
                    if(m_removeCallbackNodes.find((*iter))!=m_removeCallbackNodes.end())
                    {
                        m_removeCallbackNodes[(*iter)]();
                    }
                }
                else
                {
                    (*iter)->update();
                    ++iter;
                }
            }
        }
        
        std::lock_guard<std::mutex> lock{m_pauseLock};
        
        if(m_pause)
        {
            while(!m_running_nodes.empty())
            {
                _suspendNode(m_running_nodes.front());
            }
            m_pause = false;
        
        } else if(m_resume)
        {
            while(!m_suspended_nodes.empty())
            {
                _resumeNode(m_suspended_nodes.front());
            }
            m_resume = false;
            
        }               

        return m_running_nodes.empty();
    }

    void ALoop::handleEvent(void* e)
    {
        m_event_dispatcher.dispatch((SDL_Event*)e);
    }

    void
    ALoop::finish()
    {
        NodeList::iterator iter = m_running_nodes.begin();

        while(iter != m_running_nodes.end())
        {
            (*iter)->m_done = true;
            ++iter;
        }

        m_done = true;
    }

    void
    ALoop::add_event_handler(Event* e, EventType::Event e_type)
    {
        m_event_dispatcher.add_event_handler(e, e_type);
    }
}
