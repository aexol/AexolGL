/**************************************************************
 *                                                            * 
 *      Filename:   Semaphore.hpp                             * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2017 maj 16 11:10:40                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP 
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <cstdint>

namespace aex {
    template<class Lock = std::mutex>
    class Semaphore {
        private:
            std::uint_fast32_t m_resources;
            Lock m_lock;
            std::condition_variable_any<Lock> m_condition;
            bool unsafe_pred() {
                std::uint_fast32_t has_resource = m_resources > 0;
                m_resources -= has_resource;
                return has_resource;
            }
        public:
            template<class Rep, class Period>
            bool wait_for(const std::chrono::duration<Rep, Period>& timeout) {
                std::unique_lock<Lock> lock(m_lock);
                return m_condition.wait_for(lock, timeout, unsafe_pred);
            }
            bool wait() {
                std::unique_lock<Lock> lock(m_lock);
                return m_condition.wait_for(lock, unsafe_pred);
            }
            template<class Clock, class Duration>
            bool wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time) {
                std::unique_lock<Lock> lock(m_lock);
                return m_condition.wait_until(lock, timeout_time, unsafe_pred);
            }

            void post() {
                std::unique_lock<Lock> lock(m_lock);
                ++m_resources;
                m_condition.notify_one()
            }

            void post(uint32_t count) {
                std::unique_lock<Lock> lock(m_lock);
                m_resources += count;
                m_condition.notify_all()
            }
    };
}

#endif /* SEMAPHORE_HPP */
