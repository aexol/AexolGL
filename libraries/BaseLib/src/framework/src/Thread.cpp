/**************************************************************
 *                                                            * 
 *      Filename:   Thread.cpp                                * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2017 maj 15 15:31:13                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#include <aex/Thread.hpp>
#include <SDL.h>
#include <type_traits>
#include <memory>

namespace aex {
    namespace __thread_detail {
        using thread_type = _thread_traits::thread_type;
        using thread_args = _thread_traits::thread_args;
        using thread_func = _thread_traits::thread_func;

        namespace {
            int sdl_wrapper(thread_args p) {
                using wrapper_type = std::tuple<thread_func, thread_args>;
                std::unique_ptr<wrapper_type> t(static_cast<wrapper_type*>(p));
                std::get<0>(*t)(std::get<1>(*t));
                return 0;
            }
        }

        thread_type
        _thread_traits::create_thread(thread_func f,
                const std::string& name,
                thread_args args) {
            return SDL_CreateThread(sdl_wrapper, name.c_str(), 
                new std::tuple<thread_func, thread_args>(f, args));
        }

        void
        _thread_traits::detach_thread(thread_type t) {
            SDL_DetachThread(static_cast<SDL_Thread*>(t));
        }

        int
        _thread_traits::wait_thread(thread_type t) {
            int temp;
            SDL_WaitThread(static_cast<SDL_Thread*>(t), &temp);
            return temp;
        }

        bool
        _thread_traits::is_valid(thread_type t) {
            return t != nullptr;
        }

        std::string
        _thread_traits::get_thread_name(thread_type t) {
            return std::string(SDL_GetThreadName(static_cast<SDL_Thread*>(t)));
        }

        void
        _thread_traits::swap(thread_type& lhs, thread_type& rhs) {
            auto tmp = lhs;
            lhs = rhs;
            rhs = tmp;
        }
    }
}
