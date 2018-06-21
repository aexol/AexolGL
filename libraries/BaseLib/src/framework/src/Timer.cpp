
#include <aex/Timer.hpp>
#include <aex/ChronoClock.hpp>

namespace aex
{

    std::once_flag AexTimer::only_one;
    aex::unique_ptr<AexTimer> AexTimer::instance_ = nullptr;

    AexTimer::AexTimer(Clock& clock) :
        m_clock(clock)
    {
        m_clock.SetFrameTimeNow();
    }

    void
    AexTimer::setStartPointNow()
    {
        m_clock.SetStartPointNow();
    }

    void
    AexTimer::setFrameTimeNow()
    {
        m_clock.SetFrameTimeNow();
    }

    aexTimeUnit
    AexTimer::getElapsedTime() const
    {
        return m_clock.GetElapsedTime();

    }

    aexTimeUnit
    AexTimer::getElapsedTimePauseInsensitive()
    {
        return m_clock.GetElapsedTimePauseInsensitive();
    }

    void AexTimer::updateElapsed()
    {
        m_clock.UpdateElapsed();
    }

    void AexTimer::pause()
    {
        m_clock.Pause();
    }

    void AexTimer::unPause()
    {
        m_clock.UnPause();

    }

    AexTimer::~AexTimer()
    {

    }
}
