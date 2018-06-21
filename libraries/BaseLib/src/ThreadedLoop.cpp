#include <aex/ThreadedLoop.hpp>
#include "framework/include/internal_sdl.hpp"
namespace aex
{

    ThreadedLoop::ThreadedLoop(aex::GlobalGameEventsPtr evHandler) :
        m_started(false),
        m_workers_done(0)
    {
        if(evHandler)
        {
            evHandler->registerEventFunc(this, &ThreadedLoop::onQuit);
        }
    }

    ThreadedLoop::~ThreadedLoop()
    {
        finish();

        for(auto& thread : m_threads)
        {
            thread.join();
        }

        m_suspended_loops.clear();
    }

    void ThreadedLoop::onQuit(const EXIT_EVENT* e)
    {
        UNUSED(e);
        finish();
    }

    void ThreadedLoop::addLoop(ALoop&& loop)
    {
        m_running_loops.push_back(std::move(loop));
    }

    void ThreadedLoop::run()
    {
        auto end = m_running_loops.end();

        //if (m_running_loops.size() > 1)
        for(auto iter = ++m_running_loops.begin(); iter != end; ++iter)
        {
            m_threads.push_back(std::thread(&ThreadedLoop::executeLoop, this, iter));
        }

        while(!m_running_loops.empty())
        {
            {
                std::unique_lock<std::mutex> lck(m_frame_start_mutex);
                m_started = true;
                m_frame_start.notify_all();
            }
            executeMainLoop(m_running_loops.begin());

            std::unique_lock<std::mutex> lck(m_frame_end_mutex);

            if(m_running_loops.size() > 1)
            {
                m_frame_end.wait(lck, [this]()
                {
                    return m_workers_done != m_threads.size();
                });
            }

            m_started = false;

            while(!m_threadsQueuedForRemoval.empty())
            {
                m_threads[*m_threadsQueuedForRemoval.begin()].join();
                m_threads.erase(m_threads.begin() + *m_threadsQueuedForRemoval.begin());
                m_threadsQueuedForRemoval.pop_front();
            }
        }

        for(auto& thread : m_threads)
        {
            thread.join();
        }
    }

    void ThreadedLoop::executeLoop(const std::list<aex::ALoop>::iterator& loop)
    {
        while(true)
        {
            std::unique_lock<std::mutex> lck(m_frame_start_mutex);
            m_frame_start.wait(lck, [this]()
            {
                return m_started;
            });

            if(loop->once())
            {
                std::lock_guard<std::mutex> lock(m_erase_lock);
                auto erase_idx = std::distance(m_running_loops.begin(), loop);
                m_threadsQueuedForRemoval.push_back(erase_idx - 1);
                m_suspended_loops.push_back(std::move(*loop));
                m_running_loops.erase(loop);
                ++m_workers_done;
                m_frame_end.notify_one();
                break;
            }

            ++m_workers_done;
        }
    }




    void ThreadedLoop::executeMainLoop(const std::list<aex::ALoop>::iterator& loop)
    {
        if(loop->once() || SDL_QuitRequested())
        {
            finish();

            if(m_threads.empty())
            {
                m_suspended_loops.push_back(std::move(m_running_loops.front()));
                m_running_loops.clear();
            }
        }
    }

    aex::ALoop* ThreadedLoop::getLoop(int num)
    {
        int i = 0;
        aex::ALoop* ret = nullptr;

        for(auto iter = m_running_loops.begin(); iter != m_running_loops.end(); iter++)
        {

            if(i == num)
            {
                ret = &(*iter);
                return ret;
            }

            i++;
        }

        return ret;
    }

    aex::ALoop* ThreadedLoop::getMainThreadLoop()
    {
        if(m_running_loops.size() >= 1)
        {
            return &(*m_running_loops.begin());
        }
        else
        {
            return nullptr;
        }


    }

    void ThreadedLoop::finish()
    {
        for(auto& a : m_running_loops)
        {
            a.finish();
        }
    }


}
