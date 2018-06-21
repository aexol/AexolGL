/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SoundSourceHandler.hpp
 * Author: rafa
 *
 * Created on May 12, 2017, 2:58 PM
 */

#ifndef SOUNDSOURCEHANDLER_HPP
#define SOUNDSOURCEHANDLER_HPP

#include<aex/audio/SoundSource.hpp>
#include <aex/Vector3.hpp>

namespace aex {

    class SoundSourceHandler {
    public:
        SoundSourceHandler();
        SoundSourceHandler(aex::shared_ptr<SoundSource>sourceHandle, size_t m_id, size_t m_vectorIndex);
        ~SoundSourceHandler();

        void setSourceHandle(aex::shared_ptr<SoundSource> sourceHandle);
        size_t getVectorIndex() const;
        size_t getSourceId() const;
        void resetSource();

        bool isHandleValid();

        bool play();
        bool stop();
        bool pause();
        bool start();
        bool rewind();
        bool endLooping();
        bool isPlaying();
        bool setPosition(aex::math::Vector3 pos);
        bool setVolume(float gain);
        bool getVolume(float* gain);

    private:
        aex::weak_ptr<SoundSource> m_sourceHandle;
        size_t m_sourceId = 0;
        size_t m_vectorIndex;

    };

}


#endif /* SOUNDSOURCEHANDLER_HPP */

