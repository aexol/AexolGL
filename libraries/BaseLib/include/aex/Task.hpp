/**************************************************************
 *                                                            *
 *      Filename:   Task.hpp                                  *
 *                                                            *
 *   Description:                                             *
 *                                                            *
 *       Version:   1.0                                       *
 *       Created:   2017 maj 15 15:19:56                      *
 *                                                            *
 *        Author:   Kamil Matysiewicz                         *
 *          Mail:   kamil@aexol.com                           *
 *  Organization:   Aexol                                     *
 *                                                            *
 **************************************************************/
#ifndef TASK_HPP
#define TASK_HPP
#include <aex/Common.hpp>
#include <future>

namespace aex {
    namespace __detail {
        class AEX_PUBLIC_API Task {
            protected:
                using task_ref_t = void*;
                using task_cast_t = void(task_ref_t);
                using task_t = std::unique_ptr<void, task_cast_t*>;;

                template<class R, class... Args>
                struct _dispatch_table {
                    using task_type = std::packaged_task<R()>;
                    using future_type = decltype(std::declval<task_type>().get_future());

                    static void execute_task(task_ref_t task)  {
                        task_type *ctask = static_cast<task_type*>(task);
                        (*ctask)();
                    }

                    static void delete_task(task_ref_t task) {
                        task_type *ctask = static_cast<task_type*>(task);
                        delete ctask;
                    }

                    static future_type get_future(task_ref_t task) {
                        if(task == nullptr) {
                            return future_type{};
                        }
                        task_type *ctask = static_cast<task_type*>(task);
                        return ctask->get_future();
                    }

                    template<class F, class... CArgs>
                    static task_t make_task(F&& f, CArgs&&... args) {
                        return task_t{new task_type(std::bind(aex::forward<F>(f), aex::forward<CArgs>(args)...)), &delete_task};
                    }

                    static task_cast_t* make_task_exec() {
                        return &execute_task;
                    }
                };

                task_t m_task;
                task_cast_t* m_exec;
                template<class T>
                struct get_ret_type
                    : public get_ret_type<decltype(&T::operator())> {};

                template <typename ClassType, typename ReturnType, typename... Args>
                struct get_ret_type<ReturnType(ClassType::*)(Args...) const>
                // we specialize for pointers to member function
                {
                    enum { arity = sizeof...(Args) };
                // arity is the number of arguments.

                    typedef ReturnType type;

                    template <size_t i>
                    struct arg
                    {
                        typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
                        // the i-th argument is equivalent to the i-th tuple element of a tuple
                        // composed of those arguments.
                    };
                };
            public:
                template<class R, class... Args>
                using task_type = typename _dispatch_table<R, Args...>::task_type;
                template<class R, class... Args>
                using future_type = typename _dispatch_table<R, Args...>::future_type;
                Task(Task&&) = default;
                Task(const Task&) = delete;
                Task& operator=(Task&&) = default;
                Task& operator=(const Task&) = delete;

                template<class F, class... Args>
                Task(F&& task, Args&&... args):
                m_task(_dispatch_table<typename get_ret_type<F>::type, Args...>::make_task(aex::forward<F>(task), aex::forward<Args>(args)...)),
                m_exec{_dispatch_table<typename get_ret_type<F>::type, Args...>::execute_task}
                {
                }

                template<class R, class... Args>
                typename _dispatch_table<R, Args...>::future_type get_future() {
                    return _dispatch_table<R, Args...>::get_future(m_task.get());
                }
                void operator()();
        };
    }
    template<class T> class Task;
    template<class R, class... Args>
    class Task<R(Args...)> {
        public:
            using future_type = __detail::Task::future_type<R, Args...>;
            using task_type = std::unique_ptr<__detail::Task>;
        private:
            task_type m_task;
            future_type m_future;
            friend class TaskQueue;
            friend class Thread;
        public:
            Task(){}
            Task(Task&&) = default;
            template<class F, class... CArgs>
            Task(F&& task, CArgs&&... args) :
                m_task(new __detail::Task(aex::forward<F>(task), aex::forward<CArgs>(args)...)),
                m_future(m_task->get_future<R, Args...>())
            {
            }
            Task& operator=(Task&&) = default;
            future_type get_future() {
                return future_type(std::move(m_future));
            }
            void operator()() {
                if(m_task)
                    m_task();
            }
    };
}

#endif /* TASK_HPP */
