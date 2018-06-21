#include "aex/tween/Timeline.hpp"

namespace aex {
    namespace tween {

        Timeline::Timeline(TYPE type) :
        m_type(type) {
        }

        Timeline::~Timeline() {
            for (size_t i = 0; i < m_tweens.size(); i++) {
                delete m_tweens[i];
            }
        }

        void Timeline::step(seconds delta) {
            if (m_tweens.empty()) {
                setDone(true);
                return;
            }

            if (m_type == TYPE::SEQUENCE) {
                m_tweens[0]->step(delta);
                if (m_tweens[0]->isDone()) {
                    delete m_tweens[0];
                    m_tweens.erase(m_tweens.begin());
                    if (m_tweens.empty())
                        setDone(true);
                }
            }

            if (m_type == TYPE::PARALLEL) {
                for (size_t i = 0; i < m_tweens.size(); i++) {
                    m_tweens[i]->step(delta);
                    if (m_tweens[i]->isDone()) {
                        delete m_tweens[i];
                        m_tweens.erase(m_tweens.begin() + i);
                        i--;
                    }
                }
                if (m_tweens.empty())
                    setDone(true);
            }
        }

        Timeline* Timeline::push(Tweenable* tweenable) {
            setDone(false);
            m_tweens.push_back(tweenable);
            return this;
        }

        void Timeline::start(TweenPlayer& player) {
            player.add(this);
        }

        Timeline* Timeline::sequence() {
            return new Timeline(TYPE::SEQUENCE);
        }

        Timeline* Timeline::parallel() {
            return new Timeline(TYPE::PARALLEL);
        }
    }
}