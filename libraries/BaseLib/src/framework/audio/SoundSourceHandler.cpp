
#include <aex/audio/SoundSourceHandler.hpp>
#include <aex/audio/SoundSource.hpp>
#include <aex/Logger.hpp>
namespace aex {

    SoundSourceHandler::SoundSourceHandler()
    {

    }

    SoundSourceHandler::SoundSourceHandler(aex::shared_ptr<SoundSource> sourceHandle, size_t id, size_t vectorIndex) :
    m_sourceId(id),
    m_vectorIndex(vectorIndex)
    {
        m_sourceHandle = sourceHandle;
    }

    SoundSourceHandler::~SoundSourceHandler()
    {

    }

    void SoundSourceHandler::setSourceHandle(aex::shared_ptr<SoundSource> sourceHandle)
    {
        m_sourceHandle = sourceHandle;
    }

    bool SoundSourceHandler::isHandleValid()
    {
        if (auto spw = m_sourceHandle.lock())
        {
            return true;
        } else
        {
            return false;
        }
    }

    size_t SoundSourceHandler::getVectorIndex() const
    {
        return m_vectorIndex;
    }

    size_t SoundSourceHandler::getSourceId() const
    {
        return m_sourceId;
    }

    bool SoundSourceHandler::play()
    {
        if (auto spw = m_sourceHandle.lock())
        {
            spw->play();
            return true;
        } else
        {
            LOG_DEBUG("Source(id: ", aex::to_string(m_sourceId), ") is not active!");
            return false;
        }
        return false;
    }

    bool SoundSourceHandler::stop()
    {
        if (auto spw = m_sourceHandle.lock())
        {
            spw->stop();
            return true;
        } else
        {
            LOG_DEBUG("Source(id: ", aex::to_string(m_sourceId), ") is not active!");
            return false;
        }

    }

    bool SoundSourceHandler::pause()
    {
        if (auto spw = m_sourceHandle.lock())
        {
            spw->pause();
            return true;
        } else
        {
            LOG_DEBUG("Source(id: ", aex::to_string(m_sourceId), ") is not active!");
            return false;
        }
        return false;
    }

    bool SoundSourceHandler::start()
    {
        if (auto spw = m_sourceHandle.lock())
        {
            spw->start();
            return true;
        } else
        {
            LOG_DEBUG("Source(id: ", aex::to_string(m_sourceId), ") is not active!");
            return false;
        }
    }

    bool SoundSourceHandler::endLooping()
    {
        if (auto spw = m_sourceHandle.lock())
        {
            spw->endLooping();
            return true;
        } else
        {
            LOG_DEBUG("Source(id: ", aex::to_string(m_sourceId), ") is not active!");
            return false;
        }
    }

    bool SoundSourceHandler::rewind()
    {
        if (auto spw = m_sourceHandle.lock())
        {
            spw->rewind();
            return true;
        } else
        {
            LOG_DEBUG("Source(id: ", aex::to_string(m_sourceId), ") is not active!");
            return false;
        }
    }

    bool SoundSourceHandler::isPlaying()
    {
        if (auto spw = m_sourceHandle.lock())
        {
            return spw->isPlaying();
        } else
        {
            LOG_DEBUG("Source(id: ", aex::to_string(m_sourceId), ") is not active!");
            return false;
        }

        return false;
    }

    bool SoundSourceHandler::setPosition(aex::math::Vector3 pos)
    {
        if (auto spw = m_sourceHandle.lock())
        {
            spw->setPosition(pos);
            return true;
        } else
        {
            LOG_DEBUG("Source(id: ", aex::to_string(m_sourceId), ") is not active!");
            return false;
        }
    }

    bool SoundSourceHandler::setVolume(float gain)
    {
        if (auto spw = m_sourceHandle.lock())
        {
            spw->setVolume(gain);
            return true;
        } else
        {

            LOG_DEBUG("Source(id: ", aex::to_string(m_sourceId), ") is not active!");
            return false;
        }
    }

    bool SoundSourceHandler::getVolume(float* gain)
    {
        if (auto spw = m_sourceHandle.lock())
        {
            *gain = spw->getVolume();
            return true;
        } else
        {
            LOG_DEBUG("Source(id: ", aex::to_string(m_sourceId), ") is not active!");
            return false;
        }
    }


}