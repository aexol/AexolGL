#include <aex/MockClock.hpp>

namespace aex
{


    MockClock::MockClock():
        m_pause(false)
    {
    }

    void
    MockClock::SetStartPointNow()
    {

        m_startPoint = 0.0f;

    }

    aexTimeUnit
    MockClock::GetElapsedTime() const
    {
            aexTimeUnit ret = now - m_startPoint;
            return ret;
    }

    aexTimeUnit
    MockClock::GetElapsedTime()
    {
        if(!m_pause)
        {

            now += 1.0f / 48.0f;
            aexTimeUnit ret = now - m_startPoint;
            return ret;
        }
        else
        {
            return 0.0f;
        }

        // return m_startPoint - chrono::steady_clock::now();

    }


    aexTimeUnit
    MockClock::GetElapsedTimePauseInsensitive()
    {

        aexTimeUnit time = now - m_startPoint;
        return  time;

        // return m_startPoint - chrono::steady_clock::now();

    }


    void MockClock::UpdateElapsed()
    {
        aexTimeUnit time = now - m_startPoint;
        m_elapsedTime = time;

    }

    void MockClock::SetFrameTimeNow()
    {

    }



    void MockClock::Pause()
    {
        if(!m_pause)
        {
            m_pause = true;
            m_pausePoint = now;
        }
    }

    void MockClock::UnPause()
    {
        if(m_pause)
        {
            m_pause = false;
            aexTimeUnit time = now - m_pausePoint;
            m_pauseElapsedTime += time;
        }

    }

    MockClock::~MockClock()
    {

    }


    MockClock&
    MockClock::getInstance()
    {
        static MockClock instance;
        return instance;
    }


}
