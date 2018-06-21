/*
 * File:   AudioPlayer.hpp
 * Author: mritke
 *
 * Created on June 10, 2014, 2:17 PM
 */

#ifndef AUDIOPLAYER_HPP
#define AUDIOPLAYER_HPP
#include <aex/Common.hpp>
#include <aex/AexolGL.hpp>
#include <aex/ChronoClock.hpp>
#include <aex/SubsystemGuard.hpp>
#include <aex/audio/SoundSource.hpp>
#include <aex/audio/SoundSourceHandler.hpp>
//#include <AL/al.h>
#if __TARGET_PLATFORM__ == __APPLE_IOS__ || __TARGET_PLATFORM__ == __APPLE_OSX__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

class vorbis_info;
class OggVorbis_File;

namespace aex {

    class ResetAllAudio : public aex::GameEvent {
    };

    class StopAllAudio : public aex::GameEvent {
    };

    class ResumeAllAudio : public aex::GameEvent {
    };

    enum AudioFileFormat {
        NONE_FORMAT_FILE, WAV_FILE, OGG_FILE, MP3_FILE /*NO NO NO*/, FLAC_FILE, AAC_FILE
    };

    class AudioPlayer;

    class SoundBuffer {

    public:
        uint32_t m_buffer;
        SoundBuffer(std::string name);

        virtual void update() {
        }
        virtual bool load();
        virtual bool loadOgg(const std::string& filename);
        virtual bool loadAac(const std::string& filename);
        virtual uint32_t* getBufferPointer(bool Queue);
        virtual uint32_t getBuffer();
        void setLoop(ALboolean loop);
        ALboolean getLoop() const;
        
        
    protected:
        AudioPlayer* m_parentPlayer;
        aex::aexTimeUnit m_lastUseTime;
        std::string m_name;
        int m_format;
        int m_size;
        void* m_data;
        int m_frequnecy;
        ALboolean m_loop = true;

        AudioFileFormat m_fileFormat = NONE_FORMAT_FILE;

    };



    class SoundStreamOggBuffer : public SoundBuffer {

    public:
        uint32_t m_buffers[2];
        SoundStreamOggBuffer(std::string name);
        ~SoundStreamOggBuffer();
        void update();
        bool load();
        bool loadOgg(const std::string& filename);
        uint32_t* getBufferPointer(bool Queue);
        uint32_t getBuffer();
        
    protected:

        vorbis_info* m_pInfo;
        OggVorbis_File* m_oggFile;
        aex::ifstream* m_ReadFile;
        int m_endian = 0;
        bool m_playingBuffer = false;
        bool m_bufferToLoad = false;
        char m_array[4096]; // Local fixed size array
        std::vector<char> m_buffertemp;
        int m_bitStream;
        uint32_t m_bufferToQueue;
        uint32_t m_bufferToUnQueue;
    };

    typedef aex::unique_ptr<SoundBuffer> SoundBuffer_ptr;


     namespace __detail_tsoundSource
    {
        uint32_t nextId();
    }


    typedef aex::shared_ptr<SoundSource> SoundSource_ptr;



    class SoundLisener {

    public:
        SoundLisener(AudioPlayer* m_parentPlayer, aex::QuaternionCamera* camera) : m_camera(camera) {
        }
        void update();
        void listenerVolume(float volume);
        void listenerOnOff(bool on);
        void setupLisener();
        
    private:
        aex::QuaternionCamera* m_camera;
        float m_orientation[6];
        float m_volume = 0.99f;

    };
    typedef aex::shared_ptr<SoundLisener> SoundLisener_ptr;

    class AudioCapture {
    
        //bool m_microphone = true;
    public:
        AudioCapture(AudioPlayer* m_parentPlayer);
        ~AudioCapture();
        void startRecording();
        void stopRecording();
        void update();
        
    private:
        ALCdevice* m_inDevice = nullptr;

    };
    typedef aex::shared_ptr<AudioCapture> AudioCapture_ptr;


        class StartSourceEvent : public aex::GameEvent {

    public:
        StartSourceEvent(const std::string& soundName ,SoundSource** sourcePointer);
        StartSourceEvent(const std::string& soundName ,SoundSource** sourcePointer, aex::Aex* aex, bool looped);

        SoundSource**  getSourcePointer() const;
        bool isLooped() const;
        aex::Aex* getAex() const;
        std::string getSoundName() const;
        
    private:
        std::string m_soundName;
        SoundSource** m_sourcePointer;
        aex::Aex* m_aex = nullptr;
        bool m_looped = false;
        
    };


    class StopSourceEvent : public aex::GameEvent {

    public:
        StopSourceEvent(SoundSource* sourcePointer);
        SoundSource*  getSourcePointer() const;
    private:
        SoundSource* m_sourcePointer;
    };

    class RemoveSourceEvent : public aex::GameEvent {

    public:
        RemoveSourceEvent(SoundSource* sourcePointer);
        SoundSource*  getSourcePointer() const;
    
    private:
        SoundSource* m_sourcePointer;
        
    };


    class AudioPlayer : public aex::ALoopNode, public aex::GameObject {

    public:
        AudioPlayer(aex::GlobalGameEventsPtr globalEventhandler, aex::QuaternionCamera* camera);
        ~AudioPlayer();

        bool init();
        void update();
        void pause();
        void resume();
        void finish();

        void resetSoundScene();
        SoundBuffer* getSoundBuffer(SoundSource* source);
        SoundSource* startPlayingSource(const std::string& name, aex::Aex* aex = nullptr, bool isLooped = false); //deprecated
        SoundSourceHandler startPlayingHandler(const std::string& name, aex::Aex* aex, bool isLooped);
        void stopPlayingSource(SoundSource* source);
        void removeSource(SoundSource* source);

    private:
        void querySpecyfication();

    protected:
        aex::GlobalGameEventsPtr m_globalEventhandler;
        std::vector<SoundSource_ptr> m_soundSources;
        AudioCapture_ptr m_audioCapture;

        std::map<std::string, SoundBuffer_ptr> m_soundBuffers;
        //aex::SubsystemGuard* m_system; //(aex::SubsystemGuard(aex::AudioSubsystem));

        uint32_t m_maxBuffers = 32;
        bool m_soundON = true;
        bool m_soundsStop = false;

        SoundLisener_ptr m_lisener;
        mutable std::mutex m_addMutex;

        ALCdevice* m_outDevice = nullptr;
        ALCcontext* m_context = nullptr;
        std::deque<size_t> m_freeIndexList;
    };

    typedef aex::shared_ptr<AudioPlayer> AudioPlayer_ptr;

    size_t readOgg(void* ptr, size_t size, size_t nmemb, void* datasource);
    int seekOgg(void* datasource, int64_t offset, int whence);
    long tellOgg(void* datasource);
    int closeOgg(void* datasource);
}

#endif /* AUDIOPLAYER_HPP */
