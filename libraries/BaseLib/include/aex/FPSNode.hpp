#include <aex/AexolGL.hpp>
#include <aex/GLHTimer.hpp>
#include <chrono>
#include <thread>

namespace aex {
    class FPSNode : public aex::ALoopNode {
    public:

        void operator()() {
            counter.endOfFrame();
            if (counter.isNewSampledFPS()) {
                LOG("FPS:[", counter.fps(GLH_FPS_COUNTER::GLH_FPS_COUNTER_SAMPLED), "]");
            }
            counter.startOfFrame();

            std::this_thread::sleep_for(std::chrono::milliseconds(15));

        }

        virtual bool init() {
            return true;
        }

        virtual void pause() {
        }

        virtual void resume() {
        }

        virtual void update() {
            FPSNode& thisRef = *this;
            thisRef();
        }

        virtual void finish() {
        }
    private:
        GLhFPSCounter counter;
    };

}
