#ifndef DELAY_AEX_TWEEN_HEADER
#define DELAY_AEX_TWEEN_HEADER

#include "Tweenable.hpp"

namespace aex
{
    namespace tween
    {
        class Delay : public Tweenable
        {
        protected:
            Delay(seconds duration);
        public:
            virtual void step(seconds delta);
        protected:
            seconds m_duration;
            seconds m_timeSum;
        public:
            static Delay* delay(seconds time);
        };
    }
}


#endif