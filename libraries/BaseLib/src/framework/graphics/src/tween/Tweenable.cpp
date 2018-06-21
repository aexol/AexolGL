#include "aex/tween/Tweenable.hpp"

namespace aex {
    namespace tween {
        Tweenable::Tweenable() :
        m_done(false),
        m_paused(false) {
        }

        Tweenable::~Tweenable() {
        }

        bool Tweenable::isDone() const {
            return m_done;
        }

        void Tweenable::setDone(bool done) {
            m_done = done;
        }

        bool Tweenable::isPaused() const {
            return m_paused;
        }

        void Tweenable::pause() {
            m_paused = true;
        }

        void Tweenable::resume() {
            m_paused = false;
        }

        void Tweenable::end() {
            setDone(true);
        }
    }
}
