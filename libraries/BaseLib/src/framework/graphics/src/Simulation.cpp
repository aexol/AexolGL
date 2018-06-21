#include <aex/Simulation.hpp>

namespace aex
{
    /*************** SimulationClock implementation ****************/

    SimulationClock::SimulationClock():
    m_clock(AexTimer::getInstance())
    {
    }

    bool
    SimulationClock::init()
    {
        return true;
    }

    void
    SimulationClock::pause(){}

    void
    SimulationClock::resume(){}

    void
    SimulationClock::update()
    {
        m_clock.setFrameTimeNow();
    }

    void
    SimulationClock::finish()
    {
    }
}
