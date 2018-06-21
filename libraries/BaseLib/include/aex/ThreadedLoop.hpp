#ifndef __AEX_THREADED_LOOP_HPP
#define __AEX_THREADED_LOOP_HPP
#include<aex/ALoop.hpp>
#include<aex/GlobalGameEvents.hpp>
#include<aex/BuildInEvents.hpp>
#include <thread>
#include <mutex>
#include  <condition_variable>
namespace aex
{
    class AEX_PUBLIC_API ThreadedLoop
    {
    public:
        ThreadedLoop(aex::GlobalGameEventsPtr evHandler = nullptr);
        void onQuit(const EXIT_EVENT* e);
        void addLoop(ALoop&& loop);
        void run();
        void finish();
        aex::ALoop* getLoop(int i);
        aex::ALoop* getMainThreadLoop();
        ~ThreadedLoop();
    private:
        void executeLoop(const std::list<aex::ALoop>::iterator& loop);
        void executeMainLoop(const std::list<aex::ALoop>::iterator& loop);

        std::list<aex::ALoop> m_running_loops;
        std::list<aex::ALoop> m_suspended_loops;
        std::vector<std::thread> m_threads;
        std::list<size_t> m_threadsQueuedForRemoval;
        std::condition_variable m_frame_end;
        std::condition_variable m_frame_start;
        std::mutex m_erase_lock;
        std::mutex m_frame_start_mutex;
        std::mutex m_frame_end_mutex;
        bool m_started;
        size_t m_workers_done;
    };

    typedef aex::shared_ptr<ThreadedLoop> ThreadedLoop_ptr;

}
#endif
