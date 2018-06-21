#include <aex/ChronoClock.hpp>

namespace aex
{

    ChronoClock::ChronoClock() :
        m_pause(false)
    {
        m_startPoint = std::chrono::steady_clock::now();

    }

    void
    ChronoClock::SetStartPointNow()
    {

        m_startPoint = std::chrono::steady_clock::now();

    }

    aexTimeUnit
    ChronoClock::GetElapsedTime() const
    {
        return m_frameElapsedTime;
    }

    void
    ChronoClock::SetFrameTimeNow()
    {

        m_elapsedFrameTime = std::chrono::steady_clock::now();
        m_now = m_elapsedFrameTime;

        if(!m_pause)
        {
            ms time = std::chrono::duration_cast<ms>(m_now - m_startPoint);
            aexTimeUnit ret = time.count() - m_pauseElapsedTime;
            m_frameElapsedTime =  ret / 1000.0f;
        }
        else
        {
            m_frameElapsedTime =  0;
        }

    }

    aexTimeUnit
    ChronoClock::GetElapsedTimePauseInsensitive()
    {
        ms time = std::chrono::duration_cast<ms>(std::chrono::steady_clock::now() - m_startPoint);
        return time.count() / 1000.0f;
    }

    void ChronoClock::UpdateElapsed()
    {
        ms time = std::chrono::duration_cast<ms>(std::chrono::steady_clock::now() - m_startPoint);
        m_elapsedTime = time.count();

    }

    void ChronoClock::Pause()
    {
        if(!m_pause)
        {
            m_pause = true;
            m_pausePoint = std::chrono::steady_clock::now();
        }
    }

    void ChronoClock::UnPause()
    {
        if(m_pause)
        {
            m_pause = false;
            ms time = std::chrono::duration_cast<ms>(std::chrono::steady_clock::now() - m_pausePoint);
            m_pauseElapsedTime += time.count();
        }

    }

    ChronoClock::~ChronoClock()
    {

    }

    ChronoClock&
    ChronoClock::getInstance()
    {
        static ChronoClock instance;
        return instance;
    }
}
