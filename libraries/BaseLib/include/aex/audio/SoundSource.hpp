/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SoundSource.hpp
 * Author: rafa
 *
 * Created on May 16, 2017, 9:34 AM
 */

#ifndef SOUNDSOURCE_HPP
#define SOUNDSOURCE_HPP
#include <aex/Common.hpp>
#include <aex/Math.hpp>
#include <mutex>

namespace aex {
    class AudioPlayer;
    class SoundBuffer;
    class Aex;

    class SoundSource {
    public:
        SoundSource();
        SoundSource(AudioPlayer* parent);
        SoundSource(AudioPlayer* parent, const std::string& name);
        SoundSource(AudioPlayer* parent, const std::string& name, uint32_t buffer);
        SoundSource(AudioPlayer* parent, const std::string& name, aex::Aex* aex, bool isLooped = false, bool isStreamed = false);

        void load();
        void play();
        void stop();
        void pause();
        void start();
        void update();
        void rewind();
        void endLooping();
        bool isPlaying();
        void clear();
        void setupSource();
        void setPosition(aex::math::Vector3 pos);
        void setVolume(float volume);
        float getVolume();

        ~SoundSource();
        void setStreamed(bool Streamed);
        bool isStreamed() const;
        void setName(const std::string& name);
        std::string getName() const;
        void setLoaded(bool Loaded);
        bool isLoaded() const;

        uint32_t getId() const;
        bool isPaused() const;


    private:
        AudioPlayer* m_parentPlayer;
        std::string m_name;
        bool m_isLoaded = false;
        bool m_isStreamed = false;

        bool m_isPlaying = false;
        bool m_isPaused = false;
        bool m_looped;
        SoundBuffer* m_soundBuffer;
        uint32_t m_source;
        aex::Aex* m_aex;

        float m_pitch = 1.0f;
        float m_gain = 1.0f;
        uint32_t loops = 1;
        bool m_sourceEnded;
        bool m_sourceToDelete;

        uint32_t m_id;

        std::mutex m_mutex;

    };

}


#endif /* SOUNDSOURCE_HPP */

