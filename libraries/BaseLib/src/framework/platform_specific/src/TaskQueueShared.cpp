/**************************************************************
 *                                                            * 
 *      Filename:   TaskQueueShared.cpp                       * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2017 maj 15 15:16:35                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/

#include <aex/TaskQueue.hpp>
#include <aex/Thread.hpp>
#include <aex/Semaphore.hpp>
#include <cds/container/mspriority_queue.h>
#include <vector>
#include <cstdint>

namespace aex{
    using task_type = std::unique_ptr<__detail::Task>;
    bool prio(TASK_PRIORITY lhs, TASK_PRIORITY rhs) {
        return static_cast<uint8_t>(lhs) < static_cast<uint8_t>(rhs);
    }

    class ThreadBasedTQ : public ITaskQueue {
        using queue_type = std::deque<task_type>;
        queue_type m_queue;
        QueueHints m_hints;
        std::vector<Thread> m_workers;
        std::atomic<std::uint_fast8_t> m_finish;
        Semaphore m_taskCount;
        void pushTask(task_type&& task) {
            m_queue.push_with([&task](task_type& res)(res = std::move(task)));
        }
        ThreadBasedTQ(QueueHints hints) : m_queue(), m_workers(4), m_finish(0) {
        }
        void run() {
            char buffer[33];
            for(size_t i = 0; i < m_hints.Workers; i++) {
                m_workers.append(Thread([this](){
                }, m_hints.Name+"_"+to_string(i)));
            }
        }

        ~ThreadBasedTQ() {
        }
    };
}
