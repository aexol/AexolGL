#include "aex/tween/TweenPlayer.hpp"

namespace aex{
    namespace tween{
        TweenPlayer::TweenPlayer() {
        }

        TweenPlayer::~TweenPlayer() {
            clear();
        }

        void TweenPlayer::add(Tweenable* tween) {
            if (std::find(m_tweenables.begin(), m_tweenables.end(), tween) == m_tweenables.end())
                m_tweenables.push_back(tween);
        }

        void TweenPlayer::step(seconds delta) {
            for (size_t i = 0; i < m_tweenables.size(); i++) {
                m_tweenables[i]->step(delta);
                if (m_tweenables[i]->isDone()) {
                    delete m_tweenables[i];
                    m_tweenables.erase(m_tweenables.begin() + i);
                    i--;
                    // continue
                }
            }
        }

        void TweenPlayer::clear() {
            for (size_t i = 0; i < m_tweenables.size(); i++) {
                delete m_tweenables[i];
            }
            m_tweenables.clear();
        }

        void TweenPlayer::kill(Tweenable* tweenable)
        {
          auto elem = std::find(m_tweenables.begin(), m_tweenables.end(), tweenable );
          if(elem != m_tweenables.end())
          {
            delete *elem;
            m_tweenables.erase(elem);
          }
        }
    }
}
