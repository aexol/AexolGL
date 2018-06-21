#ifndef TWEENABLE_AEX_TWEEN_HEADER
#define TWEENABLE_AEX_TWEEN_HEADER

#include <memory>

namespace aex {
    namespace tween {
        class TweenPlayer;
        class Timeline;
        typedef float seconds;

        class Tweenable {
        public:
            friend class TweenPlayer;
            friend class Timeline;
        public:
            Tweenable();
            virtual ~Tweenable();
            bool isDone() const;
            bool isPaused() const;
            void pause();
            void resume();
            void end();
        protected:
            virtual void step(seconds delta) = 0;
            void setDone(bool done);
        protected:
            bool m_done;
            bool m_paused;
        };
    }
}

#endif
