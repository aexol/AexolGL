/**************************************************************
 *                                                            *
 *      Filename:   Thread.hpp                                *
 *                                                            *
 *   Description:                                             *
 *                                                            *
 *       Version:   1.0                                       *
 *       Created:   2017 maj 15 15:17:48                      *
 *                                                            *
 *        Author:   Kamil Matysiewicz                         *
 *          Mail:   kamil@aexol.com                           *
 *  Organization:   Aexol                                     *
 *                                                            *
 **************************************************************/
#ifndef THREAD_HPP
#define THREAD_HPP
#include <aex/Common.hpp>
#include <aex/functional.hpp>
#include <aex/cds_support.hpp>
#include <memory>
#include <tuple>
namespace aex {
    namespace __thread_detail{
        struct _thread_traits {
            using thread_type = void*;
            using thread_args = void*;
            using thread_func = void (*)(thread_args);

            static thread_type create_thread(thread_func,
                    const std::string&,
                    thread_args);
            static void detach_thread(thread_type);
            static int wait_thread(thread_type);
            static std::string get_thread_name(thread_type);
            static bool is_valid(thread_type);
            static void swap(thread_type&, thread_type&);
        };
    }
    template<class Traits = __thread_detail::_thread_traits>
    class TThread {
        private:
            using thread_type = typename Traits::thread_type;
            using thread_args = typename Traits::thread_args;
            using thread_func = typename Traits::thread_func;

            thread_type m_thread;
            void terminate() {
                if(joinable()) {
                    join();
                }
            }

            thread_type release() {
                thread_type _t = thread_type();
                Traits::swap(m_thread, _t);
                return _t;
            }

            template<class Fp>
            static void execute(void* p) {
                cds_thread_guard cds;
                std::unique_ptr<Fp> fp(static_cast<Fp*>(p));
                _invoke(std::move(*fp));
            }

            template<class F, class... Args>
            static inline void _invoke(std::tuple<F, Args...>&& t) {
                using tuple_size = std::tuple_size<std::tuple<F, Args...>>;
                _delayed(aex::forward<std::tuple<F, Args...>>(t),
                    typename __functional_detail::gen_idx_seq<tuple_size::value - 1>::type()
                );
            }

            template<class F, class... Args, size_t... Seq>
            static inline void _delayed(std::tuple<F, Args...>&& t, const __functional_detail::sequence<Seq...>& s) {
                invoke(std::move(std::get<0>(t)),
                    std::move(std::get<Seq+1>(t))...
                );
            }

            template<class F, class... Args>
            static inline thread_type create_thread(F&& f,
                    const std::string& name,
                    Args... args) {
                using tuple_type = std::tuple<
                    typename std::decay<F>::type,
                    typename std::decay<Args>::type...
                >;
                auto exec_func = &TThread::template execute<tuple_type>;
                std::unique_ptr<tuple_type> t_args(new tuple_type(
                            aex::forward<F>(f),
                            aex::forward<Args>(args)...
                ));
                thread_type t = Traits::create_thread(exec_func, name, t_args.get());
                if(t) {
                    t_args.release();
                }
                return t;
            }

        public:

            TThread() : m_thread() {}
            TThread(const TThread&) = delete;
            TThread(TThread&& other) : TThread() {
                *this = std::move(other);
            }

            TThread& operator=(TThread&& other) {
                if(this != &other) {
                    Traits::swap(m_thread, other.m_thread);
                }
                return *this;
            }

            ~TThread() {
                terminate();
            }

            template<class F, class... Args>
            TThread(F&& f, const std::string& name, Args&&... args) :
                m_thread(create_thread(aex::forward<F>(f), name, aex::forward<Args>(args)...))
            {
            }
            void detach() {
                auto _t = release();
                Traits::detach_thread(_t);
            }

            void join() {
                auto _t = release();
                Traits::wait_thread(_t);
            }

            bool joinable() {
                return Traits::is_valid(m_thread);
            }
    };
    using Thread = TThread<>;
}

#endif /* THREAD_HPP */
