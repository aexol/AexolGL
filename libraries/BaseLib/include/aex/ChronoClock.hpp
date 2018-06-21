#ifndef _CHRONOCLOCK_HPP
#define _CHRONOCLOCK_HPP
#include <aex/Common.hpp>
#include <aex/Clock.hpp>
#include <chrono>

namespace aex
{
    typedef std::chrono::steady_clock::time_point time_point;
    typedef std::chrono::milliseconds ms;

    class AEX_PUBLIC_API ChronoClock : public Clock
    {
        ChronoClock();
        ~ChronoClock();

    public:

        void             SetStartPointNow();
        void             SetFrameTimeNow();
        void             UpdateElapsed();
        aexTimeUnit      GetElapsedTime() const;
        aexTimeUnit      GetElapsedTimePauseInsensitive();
        void             Pause();
        void             UnPause();

        //singleton stuff
        static  ChronoClock&         getInstance();
        
        
    private:
        time_point      m_startPoint;
        time_point      m_now;
        aexTimeUnit     m_elapsedTime;
        time_point      m_elapsedFrameTime;
        time_point      m_pausePoint;
        aexTimeUnit     m_pauseElapsedTime;
        bool            m_pause;
        aexTimeUnit     m_frameElapsedTime;

    };
}

#endif
