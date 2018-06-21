#ifndef _CLOCK_HPP
#define _CLOCK_HPP
#include <aex/Common.hpp>

namespace aex
{

    class AEX_PUBLIC_API Clock
    {

    public:
        virtual void             SetStartPointNow() = 0;
        virtual void             SetFrameTimeNow() = 0;
        virtual void             UpdateElapsed() = 0;
        virtual aexTimeUnit      GetElapsedTime() const = 0;
        virtual aexTimeUnit      GetElapsedTimePauseInsensitive() = 0;
        virtual void             Pause() = 0;
        virtual void             UnPause() = 0;

        virtual ~Clock() = 0;
    };
    inline Clock::~Clock() {}
}

#endif
