#ifndef TWEEN_PLAYERr_AEX_TWEEN_HEADER
#define TWEEN_PLAYERr_AEX_TWEEN_HEADER

#include <vector>
#include <memory>
#include <algorithm>
#include "Tweenable.hpp"

namespace aex {
    namespace tween {

        class TweenPlayer {
        public:
            TweenPlayer();
            virtual ~TweenPlayer();
            void add(Tweenable* tween);
            void step(seconds delta);
            void clear();
            void kill(Tweenable* tweenable);
        protected:
            std::vector<Tweenable*> m_tweenables;
        };
    }
}

#endif
