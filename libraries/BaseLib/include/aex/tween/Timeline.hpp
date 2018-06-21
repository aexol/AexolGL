#ifndef TIMELINE_AEX_TWEEN_HEADER
#define TIMELINE_AEX_TWEEN_HEADER

#include "Tweenable.hpp"
#include "TweenPlayer.hpp"

namespace aex {
    namespace tween {

        class Timeline : public Tweenable {
        protected:
            enum TYPE {
                SEQUENCE,
                PARALLEL
            };
            Timeline(TYPE type);
        public:
            virtual ~Timeline();
            Timeline* push(Tweenable* tweenable);
            void start(TweenPlayer& player);

        protected:
            virtual void step(seconds delta);
        protected:
            std::vector<Tweenable*> m_tweens;
            TYPE m_type;
        public: 
            static Timeline* sequence();
            static Timeline* parallel();
        protected:
        };
    }
}

#endif
