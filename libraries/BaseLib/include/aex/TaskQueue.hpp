/**************************************************************
 *                                                            * 
 *      Filename:   TaskQueue.hpp                             * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2017 maj 15 12:50:58                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#ifndef TASKQUEUE_HPP
#define TASKQUEUE_HPP 
#include <aex/Common.hpp>
#include <aex/Task.hpp>
#include <aex/Semaphore.hpp>
#include <memory>
#include <thread>
#include <chrono>
#include <atomic>
#include <deque>
#include <array>
#include <mutex>
namespace aex {
    enum class TASK_PRIORITY : uint8_t {
        LOW,
        DEFAULT,
        HIGH
    };

    class AEX_PUBLIC_API ITaskQueue {
        private:
            using task_type = std::unique_ptr<_detail::Task>;
            void pushTask(task_type&& task, TASK_PRIORITY priority) = 0;
        public:
            template<class T>
            void pushTask(Task<T>&& task, TASK_PRIORITY priority) {
                pushTask(std::move(task.m_task), priority);
            }
            void run() = 0;
            void finish() = 0;
    };

    namespace tq {
        namespace Implementation {
            struct Native{};
            struct Threaded{};
        }

        template<class W>
        struct Workers{
            using WorkersType = W;
        };
        
        template<class Lock>
        struct LockType {
            using LockType = Lock;
        }

        template<class Rep, Period>
        struct QueueOpts{
            const std::string name;
            const duration_type timeout;
            const bool useThisThread;
            const bool global;
            template<class Rep2, class Period2>
            QueueOpts(const std::string& name,
                    const std::chrono::duration<Rep2, Period2>& d,
                    bool g,
                    bool utt
                    ) :
                name(name),
                timeout(std::duration_cast<duration_type>(d))
                global(g),
                useThisThread(utt)
            {
            }
            QueueOpts(const std::string& name) :
                QueueOpts(name, std::chrono::seconds(1)){
            }
            template<class Rep2, class Period2>
            QueueOpts(const std::duration<Rep2, Period2>& d) :
                QueueOpts("Queue", d){
            }
            QueueOpts() : QueueOpts(std::chrono::seconds(1)){}
        }

        template<size_t count>
        struct DefaultWorkerPool : public std::array<Thread, count> {
            using std::array<Thread, count>::array;
            template<class Rep, class Period>
            void run(const std::function<void()>& f, const QueueOpts<Rep, Period>& opts) {
                for(auto& t : (*this)) {
                    t = Thread(Task(f), opts.name);
                }
            }
        };
    }

    namespace _tq_detail {
        template<class... Args>
        struct find_implementation_type;

        template<class... Args> 
        struct find_implementation_type<::aex::tq::Implementation::Native, Args...>
        {
            using type = ::aex::tq::Implementation::Native;
        };

        template<class... Args> 
        struct find_implementation_type<::aex::tq::Implementation::Threaded, Args...>
        {
            using type = ::aex::tq::Implementation::Threaded;
        };

        template<class Arg, class... Args> 
        struct find_implementation_type<Arg, Args...> 
        {
            using type = typename find_implementation_type<Args...>::type;
        };

        template<>
        struct find_implementation_type<>
        {
            using type = ::aex::tq::Implementation::Native;
        };

        template<class... Args>
        struct find_workers_type;

        template<class W, class... Args> 
        struct find_workers_type<::aex::tq::Workers<W>, Args...>
        {
            using type = W;
        };

        template<class Arg, class... Args>
        struct find_workers_type<Arg, Args...>
        {
            using type = typename find_workers_type<Args...>::type;
        };

        template<> 
        struct find_workers_type<>
        {
            using type = DefaultWorkerPool<4>;
        };

        template<class... Args>
        struct find_lock_type;

        template<class Lock, class... Args>
        struct find_lock_type<::aex::tq::LockType<Lock>, Args...> {
            using type = Lock;
        }

        template<class Arg, class... Args>
        struct find_lock_type<Arg, Args...> {
            using type = typename find_lock_type<Args...>::type;
        }

        template<>
        struct find_lock_type<> {
            using type = std::mutex;
        }

        template<class... Args>
        struct queue_traits{
            using Implementation = typename find_implementation_type<Args...>::type;
            using Workers  = typename find_worker_count<Args...>::type;
            using LockType = typename find_lock_type<Args...>::type;
        };

        template<class Implementation,
            class Rep,
            class Period,
            class Traits
        >
        class TaskQueueBase {
            private:
                using task_type = std::unique_ptr<::aex::__detail::Task>;
                using queue_type = std::deque<task_type>;
                using workers_type = Traits::Workers;
                using lock_type = Traits::LockType;
                using options_type = ::aex::tq::QueueOpts<Rep, Period>;
                queue_type m_queue;
                workers_type m_workers;
                std::atomic<std::uint_fast32_t> m_done;
                Semaphore<lock_type> m_sem;
                options_type m_options;
                lock_type m_lock;
            public:
                TaskQueueBase(const options_type& opts) :
                    m_done(0),
                    m_options(opts){}
                void pushTask(task_type&& task) {
                    m_queue.push(std::move(task));
                }
                void run() {
                    m_workers.run([this](){
                        std::uint_fast32_t done = this->m_done.load();
                        while(!done) {
                            task_type task;
                            if(m_sem.wait_for(m_options.timeout)) {
                                task.reset();
                                {
                                    std::lock_guard<lock_type> lock(this->m_lock);
                                    task = std::move(m_queue.front());
                                    m_queue.pop_front();
                                }
                                (*task)()
                            }
                            done = this->m_done.load();
                        }
                    });
                }
                void stop() {
                    m_done.store(1);
                }
                ~TaskQueueBase() {
                    stop();
                }
            private:

        }

#ifdef HAS_NATIVE_TQ
        class NativeTQ {
            public:
                using duration_type = std::chrono::milliseconds;
                NativeTQ(const std::string&,
                        const duration_type&,
                        bool,
                        );

                template<class Rep, class Period>
                NativeTQ(const Options<Rep, Period>& opts): 
                    NativeTQ(opts.name,
                            std::duration_cast<duration_type>(opts.timeout),
                            opts.global){}
        };

        template<size_t Workers, bool UseThisThread>
        class TaskQueueBase<Implementation::Native, Workers, UseThisThread> {
            public:
            private:
                NativeTQ m_queue;
        };
#endif
        template<class... Args> 
        struct find_task_queue_type;

        template<class Rep,
            class Period,
            class Implementation,
            class Traits,
            size_t W,
            bool U>
        struct find_task_queue_type<Rep,
            Period,
            Implementation,
            Traits,
            ::aex::tq::WorkerCount<W>,
            ::aex::tq::UseThisThread<U>> 
        {
            using type = TaskQueueBase<Rep, Period, Implementation, Traits, W, U>;
        };

        template<class Rep, class Period, class... Args>
        struct find_task_queue_type<Rep, Period, queue_traits<Args...>> {
            using queue_traits = queue_traits<Args...>;
            using type = typename find_task_queue_type<
                  Rep,
                  Period,
                  typename queue_traits::Implementation,
                  queue_traits,
                  typename queue_traits::Workers,
                  typename queue_traits::UseThisThread>::type;
        };

    }

    template<class... Args>
    _tq_detail::queue_traits<Args...> make_traits() {
        return queue_traits<Args...>;
    }

    template<class Rep, class Period, class Traits = _tq_detail::queue_traits<>>
    class TasksQueue : public typename _tq_detail::find_task_queue_type<
        Rep,
        Period,
        Traits>::type {
    };


    void AEX_PUBLIC_API InitGlobalTaskQueue(QueueHints hints);
    template<class T>
    void AEX_PUBLIC_API PushGlobalTask(Task<T>&& task, TASK_PRIORITY priority);
    void AEX_PUBLIC_API FinalizeGlobalTaskQueue();
    class AEX_PUBLIC_API GlobalTaskQueueGuard {
        public:
            GlobalTaskQueueGuard();
            ~GlobalTaskQueueGuard();
    }
}

#endif /* TASKQUEUE_HPP */
