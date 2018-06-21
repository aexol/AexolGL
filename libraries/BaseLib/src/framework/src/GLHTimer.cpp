#include <aex/GLHTimer.hpp>

#include <chrono>
#include <aex/Logger.hpp>

inline double twSecondsNow()
{

    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return millis / 1000.0;
}

GLhTimer::GLhTimer() : m_bIsWorking(false)
{
    m_dStartTime = 0;
    m_dStopTime = 0;
    m_dBuffer = 0;
    m_dLastTickTime = 0;
    this->restart();
    this->start();
}

GLhTimer::~GLhTimer() { }

void GLhTimer::start()
{
    if(m_bIsWorking)
    {
        return;
    }

    m_dBuffer = twSecondsNow();
    m_dStartTime += (m_dBuffer - m_dStopTime);
    m_dLastTickTime = m_dBuffer;
    m_bIsWorking = true;
}
void GLhTimer::stop()
{
    if(m_bIsWorking == false)
    {
        return;
    }

    m_dStopTime = twSecondsNow();
    m_bIsWorking = false;
}

bool GLhTimer::isWorking()
{
    return m_bIsWorking;
}

void GLhTimer::restart()
{
    m_dStartTime = twSecondsNow();
    m_dStopTime = m_dStartTime;
    m_dLastTickTime = m_dStartTime;
}

double GLhTimer::tick()
{
    if(m_bIsWorking == false)
    {
        return 0;
    }

    m_dBuffer = m_dLastTickTime;
    m_dLastTickTime = twSecondsNow();

    return m_dLastTickTime - m_dBuffer;
}

double GLhTimer::allTime()
{
    if(m_bIsWorking)
    {
        return twSecondsNow() - m_dStartTime;
    }
    else
    {
        return m_dStopTime - m_dStartTime;
    }
}


GLhFPSCounter::GLhFPSCounter(bool countWhileFPS, bool countSampledFPS,
                             bool countGlobalFPS, float samplingTime) :
    m_bTmpFPS(countWhileFPS), m_bSampledFPS(countSampledFPS), m_bGlobalFPS(countGlobalFPS),
    m_fSamplingTime(samplingTime), m_bNextSample(true)
{

    m_dGlobalStartTime = twSecondsNow();
    m_dSampleStartTime = 0;
    m_dTemporaryStartTime = 0;

    m_dBuffer = 0;
    m_fTemporaryFPS = 0.0f;
    m_fGlobalFPS = 0.0f;
    m_fSampledFPS = 0.0f;
    m_uSampledFrames = 0;
    m_uGlobalFrames = 0;

    m_bNewSampledFPS = true;
}

GLhFPSCounter::~GLhFPSCounter() { }

void GLhFPSCounter::startOfFrame()
{
    if(m_bTmpFPS)
    {
        m_dTemporaryStartTime = twSecondsNow();
    }

    if(m_bSampledFPS)
    {
        if(m_bNextSample)
        {
            m_bNextSample = false;
            m_dSampleStartTime = twSecondsNow();
        }
    }
}

void GLhFPSCounter::endOfFrame()
{
    if(m_bTmpFPS)
    {
        m_dBuffer = twSecondsNow();
        m_fTemporaryFPS = 1.0f / (m_dBuffer - m_dTemporaryStartTime);

    }

    if(m_bSampledFPS)
    {
        m_uSampledFrames++;


        m_dBuffer = twSecondsNow();

        if((float)(m_dBuffer - m_dSampleStartTime) >= m_fSamplingTime)
        {
            m_fSampledFPS = (float)m_uSampledFrames / (float)(m_dBuffer - m_dSampleStartTime);
            m_bNextSample = true;
            m_uSampledFrames = 0;
            m_bNewSampledFPS = true;
        }
    }

    if(m_bGlobalFPS)
    {
        m_uGlobalFrames++;
        m_dBuffer = twSecondsNow();
        m_fGlobalFPS = (double) m_uGlobalFrames / (double)(m_dBuffer - m_dGlobalStartTime);
    }
}

float GLhFPSCounter::fps(enum GLH_FPS_COUNTER  fpsType)
{

    switch(fpsType)
    {

    case GLH_FPS_COUNTER_TEMPORARY:
        if(m_bTmpFPS)
        {
            return m_fTemporaryFPS;
        }
        else
        {
            return -1.0f;
        }

        break;

    case GLH_FPS_COUNTER_SAMPLED:
        if(m_bSampledFPS)
        {
            m_bNewSampledFPS = false;
            return m_fSampledFPS;
        }
        else
        {
            return -1.0f;
        }

        break;

    case GLH_FPS_COUNTER_GLOBAL:
        if(m_bGlobalFPS)
        {
            return m_fGlobalFPS;
        }
        else
        {
            return -1.0f;
        }

        break;

    default:
        return -1.0f;
    }
}

std::ostream& operator << (std::ostream& out, GLhFPSCounter& counter)
{
    out << "# temporary: " << counter.fps(GLH_FPS_COUNTER_TEMPORARY) << std::endl;
    out << "#   sampled: " << counter.fps(GLH_FPS_COUNTER_SAMPLED) << std::endl;
    out << "#    global: " << counter.fps(GLH_FPS_COUNTER_GLOBAL);
    return out;
}
