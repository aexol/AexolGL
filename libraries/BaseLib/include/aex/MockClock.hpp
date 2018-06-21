#ifndef _MOCKCLOCK_HPP
#define _MOCKCLOCK_HPP
#include <aex/Common.hpp>
#include <aex/Clock.hpp>

namespace aex
{

    class AEX_PUBLIC_API MockClock : public Clock
    {

    public:

        void             SetStartPointNow();
        void             UpdateElapsed();
        void             SetFrameTimeNow();
        aexTimeUnit      GetElapsedTime();
        aexTimeUnit      GetElapsedTime() const;
        aexTimeUnit      GetElapsedTimePauseInsensitive();
        void             Pause();
        void             UnPause();

        //singleton stuff
        static  MockClock&         getInstance();
        
    private:
        aexTimeUnit     now;
        aexTimeUnit     m_startPoint;
        aexTimeUnit     m_elapsedTime;
        aexTimeUnit     m_pauseElapsedTime;
        aexTimeUnit     m_pausePoint;
        bool            m_pause;


        MockClock();
        ~MockClock();

        
    };
}

#endif
