/*
 * File:   Timer.hpp
 * Author: mritke
 *
 * Created on November 15, 2013, 12:07 PM
 */

#ifndef TIMER_HPP
#define	TIMER_HPP
#include <aex/Common.hpp>
#include <aex/ChronoClock.hpp>
#include <aex/Singleton.hpp>
#include <chrono>
#include <mutex>

namespace aex
{

    class ChronoClock;


    class AEX_PUBLIC_API AexTimer
    {
    private:
        static aex::unique_ptr<AexTimer> instance_;
        static std::once_flag only_one;
        Clock& m_clock;


        AexTimer(Clock& clock);
    public:
        using time_point = std::chrono::steady_clock::time_point;
        using ms = std::chrono::milliseconds;
        using time_type = aexTimeUnit;
        ~AexTimer();
        void             setStartPointNow();
        void             setFrameTimeNow();
        void             updateElapsed();
        aexTimeUnit      getElapsedTime() const;
        aexTimeUnit      getElapsedTimePauseInsensitive();
        void             pause();
        void             unPause();


        //singleton stuff
        template<typename CLOCK = ChronoClock>
        static AexTimer & getInstance();

    };


    template<typename CLOCK>
    AexTimer&
    AexTimer::getInstance()
    {
        std::call_once(AexTimer::only_one,
                       []()
        {
            Clock& clock = static_cast<Clock&>(CLOCK::getInstance());

            if(!instance_)
            {
                AexTimer::instance_.reset(new AexTimer(clock));
            }
        }
                      );

        return *AexTimer::instance_;
    }
}


#endif	/* TIMER_HPP */

