#include <aex/Aex.hpp>
#include <aex/audio/AudioPlayer.hpp>


namespace aex {

    SoundSource::SoundSource() : SoundSource(nullptr)
    {
    }

    SoundSource::SoundSource(AudioPlayer* parent) : SoundSource(parent, "")
    {
    }

    SoundSource::SoundSource(AudioPlayer* parent, const std::string& name) :
    m_parentPlayer(parent),
    m_name(name),
    m_aex(nullptr),
    m_id(__detail_tsoundSource::nextId())
    {

    }

    SoundSource::SoundSource(AudioPlayer* parent, const std::string& name, aex::Aex* aex, bool looped, bool isStreamed) :
    m_parentPlayer(parent),
    m_name(name),
    m_isStreamed(isStreamed),
    m_looped(looped),
    m_aex(aex),
    m_id(__detail_tsoundSource::nextId())
    {

    }

    SoundSource::~SoundSource()
    {
        clear();
    }

    void SoundSource::setStreamed(bool Streamed)
    {
        m_isStreamed = Streamed;
    }

    bool SoundSource::isStreamed() const
    {
        return m_isStreamed;
    }

    void SoundSource::setName(const std::string& name)
    {
        m_name = name;
    }

    std::string SoundSource::getName() const
    {
        return m_name;
    }

    void SoundSource::setLoaded(bool Loaded)
    {
        m_isLoaded = Loaded;
    }

    bool SoundSource::isLoaded() const
    {
        return m_isLoaded;
    }

    uint32_t SoundSource::getId() const
    {
        return m_id;
    }

    bool SoundSource::isPaused() const
    {
        return m_isPaused;
    }

    void SoundSource::setupSource()
    {
        if (!m_isStreamed)
        {
            alSourcei(m_source, AL_BUFFER, m_soundBuffer->m_buffer);
        } else
        {
            m_soundBuffer->update();
            alSourceQueueBuffers(m_source, 1, m_soundBuffer->getBufferPointer(true));

            m_soundBuffer->update();
            alSourceQueueBuffers(m_source, 1, m_soundBuffer->getBufferPointer(true));
        }

        alSourcef(m_source, AL_PITCH, m_pitch);
        alSourcef(m_source, AL_GAIN, m_gain);

        if (m_aex)
        {
            alSourcefv(m_source, AL_POSITION, m_aex->getPosition().data());
        } else
        {
            alSourcei(m_source, AL_SOURCE_RELATIVE, AL_TRUE);
        }

        //alSourcefv(m_source, AL_VELOCITY, SourceVel);
        if (!m_isStreamed)
        {
            alSourcei(m_source, AL_LOOPING, m_looped);
        }

    }

    void SoundSource::endLooping()
    {
        m_looped = false;
        alSourcei(m_source, AL_LOOPING, m_looped);
    }
    
    void
    SoundSource::setPosition(aex::math::Vector3 pos)
    {
        if (m_isLoaded)
        {
            alSourcefv(m_source, AL_POSITION, pos.data());
        }
    }

    void
    SoundSource::setVolume(float volume)
    {
        if (m_isLoaded)
        {
            m_gain = volume;
            alSourcef(m_source, AL_GAIN, m_gain);
        }
    }
    
    float 
    SoundSource::getVolume()
    {
        return m_gain;
    }

    void
    SoundSource::update()
    {

        if (m_isStreamed && m_isLoaded && m_isPlaying && m_isPaused)
        {

//            LOG_DEBUG("Queued :", queued);

            int processed;
            alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processed);

            //LOG_DEBUG("processsed :", processed);
            if (processed > 0)
            {
                //if (!isPlaying()) {

                alSourceUnqueueBuffers(m_source, 1, m_soundBuffer->getBufferPointer(false));
                m_soundBuffer->update();
                alSourceQueueBuffers(m_source, 1, m_soundBuffer->getBufferPointer(true));

                if (!isPlaying())
                {
                    alSourcePlay(m_source);
                }

            } else
            {
                //LOG_DEBUG("stream is playing");
            }
        }

        if (m_aex && m_isLoaded)
        {
            alSourcefv(m_source, AL_POSITION, m_aex->getPosition().data());
        }
    }

    void
    SoundSource::load()
    {
        alGenSources(1, &m_source);

        if (m_parentPlayer && (m_soundBuffer = m_parentPlayer->getSoundBuffer(this)) != nullptr)
        {
            setupSource();
            m_isLoaded = true;
        } else
        {
            m_isLoaded = false;
        }
    }

    void SoundSource::play()
    {
        if (!m_isLoaded)
        {
            load();
        }
        
        if(m_isPaused)
        {
            m_isPaused = false;
        }

        m_isPlaying = true;
        alSourcePlay(m_source);
    }

    void SoundSource::stop()
    {
        m_isPlaying = false;
        alSourcePause(m_source);
    }
    
    void SoundSource::pause()
    {
        m_isPaused = true;
        alSourcePause(m_source);
    }

    void SoundSource::start()
    {
        if (!m_isLoaded)
        {
            load();
        }

        m_isPlaying = true;
        alSourcePlay(m_source);
    }

    void SoundSource::rewind()
    {
        alSourceRewind(m_source);
    }

    bool SoundSource::isPlaying()
    {
        ALenum state;

        alGetSourcei(m_source, AL_SOURCE_STATE, &state);
        return (state == AL_PLAYING);
    }

    void
    SoundSource::clear()
    {
        m_isLoaded = false;
        alDeleteSources(1, &m_source);
    }
}