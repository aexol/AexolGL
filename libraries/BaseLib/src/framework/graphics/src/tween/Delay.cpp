#include "aex/tween/Delay.hpp"

namespace aex {
    namespace tween {
        Delay::Delay(seconds duration) :
        m_duration(duration),
        m_timeSum(0) {
        }

        void Delay::step(seconds delta) {
            m_timeSum += delta;
            if (m_timeSum >= m_duration)
                setDone(true);
        }

        Delay* Delay::delay(seconds duration) {
            return new Delay(duration);
        }
    }
}

