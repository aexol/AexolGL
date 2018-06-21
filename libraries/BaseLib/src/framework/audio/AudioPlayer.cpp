#include <aex/audio/AudioPlayer.hpp>
#include <aex/audio/SoundSource.hpp>
#include <aex/audio/SoundSourceHandler.hpp>
#include <aex/FileManager.hpp>

#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>

namespace aex {

#define BUFFER_SIZE   32768     // 32 KB buffers
#define CAP_SIZE 2048

#define LOG_AUDIO false

    SoundBuffer::SoundBuffer(std::string name) : m_name(name)
    {

    }

    bool
    SoundBuffer::load()
    {
        std::string ext = m_name.substr(m_name.size() - 3, m_name.size());
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (ext == "wav")
        {
            m_fileFormat = WAV_FILE;
            LOG_ERR("WAV Not supported... yet");
        } else if (ext == "ogg")
        {
            m_fileFormat = OGG_FILE;
            return loadOgg(m_name);
        } else if (ext == "mp3")
        {
            m_fileFormat = MP3_FILE;
            LOG_ERR("MP3 Not supported... yet)");
        } else if (ext == "aac")
        {

            m_fileFormat = AAC_FILE;
            loadAac(m_name);
            LOG_ERR("AAC Not supported... yet");
        } else
        {
            LOG_ERR("File ext not supported! ", ext);
        }
        return false;

    }

    bool SoundBuffer::loadOgg(const std::string& filename)
    {
        vorbis_info* pInfo;
        OggVorbis_File oggFile;

        FileManager fm;
        aex::ifstream ReadFile = fm.open_read_file(filename);

        int endian = 0; // 0 for Little-Endian, 1 for Big-Endian

        if (ReadFile.is_open())
        {
            ov_callbacks ov_callback;

            ov_callback.close_func = closeOgg;
            ov_callback.read_func = readOgg;
            ov_callback.seek_func = seekOgg;
            ov_callback.tell_func = tellOgg;

            if (LOG_AUDIO)
            {
                LOG_DEBUG("Opening ", filename, " for decoding...");
            }

            if (int err = ov_open_callbacks(&ReadFile, &oggFile, NULL, 0, ov_callback) != 0)
            {

                if (err == OV_EREAD)
                {
                    LOG_ERR("Error opening ", filename, " ", err);
                } else if (err == OV_ENOTVORBIS)
                {
                    LOG_ERR("Error opening ", filename, ". Bitstream does not contain any Vorbis data.", err);
                } else if (err == OV_EVERSION)
                {
                    LOG_ERR("Error opening ", filename, ". Vorbis version mismatch.", err);
                } else if (err == OV_EBADHEADER)
                {
                    LOG_DEBUG("Error opening ", filename, ". Invalid Vorbis bitstream header.", err);
                } else if (err == OV_EFAULT)
                {
                    LOG_ERR("Error opening ", filename, ". Internal logic fault; indicates a bug or heap/stack corruption.", err);
                } else
                {
                    LOG_ERR("Error opening ", filename, ". Something weird happened.", err);
                }

                return false;
            }

            pInfo = ov_info(&oggFile, -1);

            m_frequnecy = pInfo->rate;
            char array[BUFFER_SIZE]; // Local fixed size array
            std::vector <char> buffer;

            int bitStream = 0;
            long bytes = 0;

            do
            {

                bytes = ov_read(&oggFile, array, 4096, endian, 2, 1, &bitStream);

                buffer.insert(buffer.end(), array, array + bytes);

            }
            while (bytes > 0);

            if (pInfo->channels == 1)
            {
                m_format = AL_FORMAT_MONO16;
            } else
            {
                m_format = AL_FORMAT_STEREO16;
            }

            m_frequnecy = pInfo->rate;
            alGenBuffers(1, &m_buffer);

            //LOG_DEBUG("OGG in memory size: ", (buffer.size() * sizeof (char)) / 1024.0f, "kb");
            alBufferData(m_buffer, m_format, buffer.data(), buffer.size() * sizeof (char), m_frequnecy);
            // alutCreateBufferFromFileImage()

        } else
        {
            LOG_ERR("could not open ", filename);
            return false;
        }

        ov_clear(&oggFile);

        return true;
    }

    bool SoundStreamOggBuffer::loadOgg(const std::string& filename)
    {

        FileManager fm;
        m_ReadFile = new aex::ifstream(fm.open_read_file(filename));

        if (m_ReadFile->is_open())
        {

            // m_ReadFile = ReadFile;
            ov_callbacks ov_callback;

            ov_callback.close_func = closeOgg;
            ov_callback.read_func = readOgg;
            ov_callback.seek_func = seekOgg;
            ov_callback.tell_func = tellOgg;

            if (LOG_AUDIO)
            {
                LOG_DEBUG("Opening Streambuffer ", filename, " for decoding...");
            }
            if (int err = ov_open_callbacks(m_ReadFile, m_oggFile, NULL, 0, ov_callback) != 0)
            {

                if (err == OV_EREAD)
                {
                    LOG_ERR("Error opening ", filename, " ", err);
                } else if (err == OV_ENOTVORBIS)
                {
                    LOG_ERR("Error opening ", filename, ". Bitstream does not contain any Vorbis data.", err);
                } else if (err == OV_EVERSION)
                {
                    LOG_ERR("Error opening ", filename, ". Vorbis version mismatch.", err);
                } else if (err == OV_EBADHEADER)
                {
                    LOG_ERR("Error opening ", filename, ". Invalid Vorbis bitstream header.", err);
                } else if (err == OV_EFAULT)
                {
                    LOG_ERR("Error opening ", filename, ". Internal logic fault; indicates a bug or heap/stack corruption.", err);
                } else
                {
                    LOG_ERR("Error opening ", filename, ". Something weird happened.", err);
                }

                return false;
            }

            m_pInfo = ov_info(m_oggFile, -1);

            m_frequnecy = m_pInfo->rate;


            if (m_pInfo->channels == 1)
            {
                m_format = AL_FORMAT_MONO16;
            } else
            {
                m_format = AL_FORMAT_STEREO16;
            }

            return true;
        } else
        {
            LOG_ERR("Error opening ", filename, ".");
            return false;
        }

    }

    void
    SoundStreamOggBuffer::update()
    {

        //m_bufferToLoad = !m_playingBuffer;
        if (!m_playingBuffer)
        {
            int bytes = 1;
            int total = 0;
            m_buffertemp.clear();

            while (total < BUFFER_SIZE && bytes != 0)
            {
                bytes = ov_read(m_oggFile, m_array, 4096, m_endian, 2, 1, &m_bitStream);
                m_buffertemp.insert(m_buffertemp.begin() + total, m_array, m_array + bytes);
                total += bytes;
            }

            if (total)
            {
                //LOG_DEBUG("updating buffer: ", m_buffers[(int) m_bufferToLoad]);
                alBufferData(m_buffers[(int) m_bufferToLoad], m_format, m_buffertemp.data(), m_buffertemp.size() * sizeof (char), m_frequnecy);
                m_bufferToQueue = m_buffers[(int) m_bufferToLoad];
                m_bufferToUnQueue = m_buffers[(int) !m_bufferToLoad];
                m_bufferToLoad = !m_bufferToLoad;
            }

            if (!bytes)
            {
                ov_raw_seek(m_oggFile, 0);
            }

            m_playingBuffer = true;
        }
    }

    bool SoundBuffer::loadAac(const std::string& filename)
    {
        return false;
    }

    SoundStreamOggBuffer::SoundStreamOggBuffer(std::string name) : SoundBuffer(name)
    {
        alGenBuffers(2, m_buffers);
        m_oggFile = new OggVorbis_File();

    }

    bool SoundStreamOggBuffer::load()
    {

        // LOG_DEBUG("loading from stream");

        if (m_name.substr(m_name.size() - 3, m_name.size()) == "ogg")
        {
            if (LOG_AUDIO)
            {
                LOG_DEBUG("loading ogg to stream");
            }
            m_fileFormat = OGG_FILE;
            return loadOgg(m_name);
        } else
        {

            LOG_DEBUG("Only ogg files are streamable.");
        }

        return false;

    }

    uint32_t* SoundBuffer::getBufferPointer(bool Queue)
    {
        return &m_buffer;
    }

    uint32_t SoundBuffer::getBuffer()
    {
        return m_buffer;
    }

    void SoundBuffer::setLoop(ALboolean loop)
    {
        m_loop = loop;
    }

    ALboolean SoundBuffer::getLoop() const
    {
        return m_loop;
    }

    SoundStreamOggBuffer::~SoundStreamOggBuffer()
    {
        alDeleteBuffers(2, m_buffers);
        delete m_oggFile;
    }

    uint32_t* SoundStreamOggBuffer::getBufferPointer(bool Queue)
    {
        if (Queue)
        {
            m_playingBuffer = false;
            //LOG_DEBUG("Queue buffor: ", m_bufferToQueue);
            return &m_bufferToQueue;
        } else
        {
            //LOG_DEBUG("UNQueue buffor: ", m_bufferToUnQueue);
            return &m_bufferToUnQueue;
        }
    }

    uint32_t SoundStreamOggBuffer::getBuffer()
    {

        return m_buffers[(int) m_bufferToLoad];
    }


    namespace __detail_tsoundSource {
        std::atomic<uint32_t> _nextId(1);

        uint32_t nextId()
        {
            return _nextId.fetch_add(1);
        }
    }

    void SoundLisener::update()
    {
        if (m_camera->isCameraMoved())
        {
            setupLisener();
        }
    }

    void SoundLisener::listenerVolume(float volume)
    {
        m_volume = volume;
        alListenerf(AL_GAIN, volume);
    }

    void SoundLisener::listenerOnOff(bool on)
    {
        if (on)
        {
            alListenerf(AL_GAIN, m_volume);
        } else
        {
            alListenerf(AL_GAIN, 0.0f);
        }
    }

    void SoundLisener::setupLisener()
    {
        //LOG_DEBUG("SoundLisener::setupLisener()");
        alListenerfv(AL_POSITION, m_camera->GetPosition().data());

        m_orientation[0] = m_camera->getTarget().x;
        m_orientation[1] = m_camera->getTarget().y;
        m_orientation[2] = m_camera->getTarget().z;
        m_orientation[3] = m_camera->getTargetYAxis().x;
        m_orientation[4] = m_camera->getTargetYAxis().y;
        m_orientation[5] = m_camera->getTargetYAxis().z;

        alListenerfv(AL_VELOCITY, m_camera->getCurrentVelocity().data());
        alListenerfv(AL_ORIENTATION, m_orientation);
    }

    AudioCapture::AudioCapture(AudioPlayer* parentPlayer)
    {
        m_inDevice = alcCaptureOpenDevice(NULL, 44100, AL_FORMAT_STEREO16, 1024);

        if (ALenum temp = alcGetError(m_inDevice) != AL_NO_ERROR)
        {
            LOG_ERR("Could not open audio capture device. Error: ", temp);
            // return 0;
        }

        const ALCchar* devices;
        LOG_DEBUG("Opening capture device: ");
        devices = alcGetString(m_inDevice, ALC_CAPTURE_DEVICE_SPECIFIER);
        {
            string dev(devices);
            LOG_DEBUG(dev);
        }

        if (!m_inDevice)
        {
            LOG_ERR("Could not open audio capture device.");
        }

        delete devices;
    }

    AudioCapture::~AudioCapture()
    {
        alcCaptureStop(m_inDevice);
        alcCaptureCloseDevice(m_inDevice);
    }

    void AudioCapture::startRecording()
    {
        alcCaptureStart(m_inDevice);
    }

    void AudioCapture::stopRecording()
    {
        alcCaptureStop(m_inDevice);

    }

    void AudioCapture::update()
    {
        ALint samplesAvailable = 0;
        alcGetIntegerv(m_inDevice, ALC_CAPTURE_SAMPLES, 1, &samplesAvailable);
        if (LOG_AUDIO)
        {
            LOG_DEBUG("samplesAvailable ", samplesAvailable);
        }

        if (samplesAvailable > CAP_SIZE)
        {


        }

    }

    AudioPlayer::AudioPlayer(aex::GlobalGameEventsPtr globalEventhandler, aex::QuaternionCamera* camera) : aex::GameObject("AudioPlayer"),
    m_globalEventhandler(globalEventhandler)
    {
        m_lisener = aex::make_shared<SoundLisener>(this, camera);
        //m_globalEventhandler->registerEventFunc(this, &AudioPlayer::StartAudioEventHandler);
        //m_globalEventhandler->registerEventFunc(this, &AudioPlayer::StopAudioEventHandler);
        //m_globalEventhandler->registerEventFunc(this, &AudioPlayer::StopAllAudioEventHandler);
        //m_globalEventhandler->registerEventFunc(this, &AudioPlayer::ResumeAllAudioEventHandler);
    }

    AudioPlayer::~AudioPlayer()
    {
        // delete m_system;
        LOG_DEBUG("Audio Player Destructor.");
        m_soundSources.erase(m_soundSources.begin(), m_soundSources.end());
        alcMakeContextCurrent(NULL);
        alcDestroyContext(m_context);
        alcCloseDevice(m_outDevice);
    }

    void
    AudioPlayer::querySpecyfication()
    {
        //SDL_mixer stuff
        /*if (Mix_QuerySpec(&m_device.audio_rate, &m_device.audio_format, &m_device.audio_channels)) {
            //LOG_DEBUG("Audio has been opened");
        }*/
    }

    StartSourceEvent::StartSourceEvent(const std::string& soundName, SoundSource** sourcePointer) :
    m_soundName(soundName),
    m_sourcePointer(sourcePointer)
    {

    }

    StartSourceEvent::StartSourceEvent(const std::string& soundName, SoundSource** sourcePointer, aex::Aex* aex, bool looped) :
    m_soundName(soundName),
    m_sourcePointer(sourcePointer),
    m_aex(aex),
    m_looped(looped)
    {

    }

    SoundSource** StartSourceEvent::getSourcePointer() const
    {
        return m_sourcePointer;
    }

    bool StartSourceEvent::isLooped() const
    {
        return m_looped;
    }

    aex::Aex* StartSourceEvent::getAex() const
    {
        return m_aex;
    }

    std::string StartSourceEvent::getSoundName() const
    {
        return m_soundName;
    }

    StopSourceEvent::StopSourceEvent(SoundSource* sourcePointer) :
    m_sourcePointer(sourcePointer)
    {

    }

    SoundSource* StopSourceEvent::getSourcePointer() const
    {
        return m_sourcePointer;
    }

    RemoveSourceEvent::RemoveSourceEvent(SoundSource* sourcePointer) :
    m_sourcePointer(sourcePointer)
    {

    }

    SoundSource* RemoveSourceEvent::getSourcePointer() const
    {
        return m_sourcePointer;
    }

    SoundSource* AudioPlayer::startPlayingSource(const std::string& name, aex::Aex* aex, bool isLooped)
    {
        SoundSource* newsound = nullptr;
        if (m_soundON)
        {
            std::lock_guard<std::mutex> lock(m_addMutex);

            newsound = new SoundSource(this, name, aex, isLooped);

            if (!m_freeIndexList.empty())
            {
                size_t index = m_freeIndexList.front();
                m_freeIndexList.pop_front();
                m_soundSources[index] = SoundSource_ptr{newsound};
            } else
            {
                m_soundSources.push_back(SoundSource_ptr{newsound});
            }

            newsound->play();

        }
        return newsound;
    }

    SoundSourceHandler AudioPlayer::startPlayingHandler(const std::string& name, aex::Aex* aex, bool isLooped)
    {
        SoundSourceHandler newsoundhandler;
        if (m_soundON)
        {
            std::lock_guard<std::mutex> lock(m_addMutex);

            SoundSource* newsoundsource = new SoundSource(this, name, aex, isLooped);
            SoundSource_ptr shared = SoundSource_ptr{newsoundsource};
            size_t index = 0;
            if (!m_freeIndexList.empty())
            {
                index = m_freeIndexList.front();
                m_freeIndexList.pop_front();
                m_soundSources[index] = shared;
            } else
            {
                m_soundSources.push_back(shared);
                index = m_soundSources.size() - 1;
            }

            newsoundhandler = SoundSourceHandler(shared, newsoundsource->getId(), index);

            newsoundhandler.play();

        }
        return newsoundhandler;
    }

    void AudioPlayer::stopPlayingSource(SoundSource* source)
    {
        for (auto iter = m_soundSources.begin(); iter != m_soundSources.end(); iter++)
        {
            if ((*iter).get() == source)
            {
                if ((*iter)->isPlaying())
                {
                    (*iter)->stop();
                }
            }

        }
    }

    void AudioPlayer::removeSource(SoundSource* source)
    {
        std::lock_guard<std::mutex> lock(m_addMutex);
        for (size_t i = 0; i < m_soundSources.size(); i++)
        {
            if (m_soundSources[i].get() == source)
            {
                m_soundSources[i]->stop();
                m_soundSources[i].reset();
                m_freeIndexList.push_back(i);
            }

        }
    }

    bool AudioPlayer::init()
    {
        const ALCchar* devices;
        devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
        LOG_DEBUG("Available playback devices: \n", devices);

        m_outDevice = alcOpenDevice(NULL);
        ALenum status = alcGetError(m_outDevice);
        if (status != AL_NO_ERROR)
        {
            LOG_ERR("Could not open audio device: ", status);
        }

        if (!m_outDevice)
        {
            LOG_ERR("Could not open audio device.");
            return false;
        }

        m_context = alcCreateContext(m_outDevice, NULL);
        alcMakeContextCurrent(m_context);

        if (!m_context)
        {
            LOG_ERR("Could not open audio device.");
            return false;
        }

        LOG("Audio device open success!");
        return true;
    }

    void AudioPlayer::update()
    {

        if (m_soundON)
        {

            if (!m_soundsStop)
            {
                m_lisener->update();

                std::lock_guard<std::mutex> lock(m_addMutex);
                for (size_t i = 0; i < m_soundSources.size(); i++)
                {
                    if (m_soundSources[i])
                    {
                        m_soundSources[i]->update();

                        if (!m_soundSources[i]->isPlaying())
                        {
                            m_soundSources[i].reset();
                            m_freeIndexList.push_back(i);
                        }
                    }
                }
            }
        }
    }

    void AudioPlayer::resetSoundScene()
    {
        std::lock_guard<std::mutex> lock(m_addMutex);
        m_soundSources.erase(m_soundSources.begin(), m_soundSources.end());
    }

    SoundBuffer* AudioPlayer::getSoundBuffer(SoundSource* source)
    {
        SoundBuffer* buffer = nullptr;
        if (m_soundON)
        {
            const std::string& source_name = source->getName();
            if (m_soundBuffers.find(source_name) != m_soundBuffers.end())
            {
                buffer = m_soundBuffers[source_name].get();
            } else
            {
                if (!source->getName().empty())
                {
                    SoundBuffer_ptr& buff = m_soundBuffers[source_name];
                    if (!source->isStreamed())
                    {
                        buff.reset(new SoundBuffer{source->getName()});
                        //LOG_DEBUG("SoundBuffer created");
                    } else
                    {
                        buff.reset(new SoundStreamOggBuffer{source_name});
                        //LOG_DEBUG("SoundStreamOggBuffer created");
                    }
                    if (!buff->load())
                    {
                        buff.reset();
                    }
                    buffer = buff.get();
                }
            }
        }
        return buffer;
    }

    void AudioPlayer::pause()
    {

        m_lisener->listenerOnOff(false);
    }

    void AudioPlayer::resume()
    {
        m_lisener->listenerOnOff(true);
    }

    void AudioPlayer::finish()
    {
        m_done = true;
    }

    size_t readOgg(void* ptr, size_t size, size_t nmemb, void* datasource)
    {
        aex::fstream* File = (aex::fstream*)(datasource);
        File->read((char*) ptr, size * nmemb);
        return File->gcount();
    }

    int seekOgg(void* datasource, int64_t offset, int whence)
    {
        aex::fstream* File = reinterpret_cast<aex::fstream*> (datasource);
        std::ios::seekdir Dir;

        File->clear();

        switch (whence) {
            case SEEK_SET:
                Dir = std::ios::beg;
                break;

            case SEEK_CUR:
                Dir = std::ios::cur;
                break;

            case SEEK_END:
                Dir = std::ios::end;
                break;

            default:
                return -1;
        }

        File->seekg((std::streamoff)offset, Dir);

        return (File->fail() ? -1 : 0);
    }

    long tellOgg(void* datasource)
    {
        aex::fstream* File = reinterpret_cast<aex::fstream*> (datasource);
        return File->tellg();
    }

    int closeOgg(void* datasource)
    {
        return 0;
    }

}
////////////////////////////////
