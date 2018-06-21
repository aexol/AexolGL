/*
 * File:   SpriteAnimated.hpp
 * Author: mritke
 *
 * Created on November 14, 2014, 4:08 PM
 */

#ifndef SPRITEANIMATED_HPP
#define	SPRITEANIMATED_HPP
#include <aex/AexolGL.hpp>
namespace aex
{

    struct AEX_PUBLIC_API SpriteFrame
    {
        int frameNumber;
        int pageNumber;
        aex::math::Vector2 UvOffset;
        aex::math::Vector2 UvSize;

        SpriteFrame()
        {
        }
        SpriteFrame(int frameNumber, int pageNumber, float sizeX, float sizeY, float offsetX, float offsetY);
    };

    struct AEX_PUBLIC_API AnimationTypeInfo
    {
        aex::string animationName = "";
        int startFrame = 0;
        int endFrame = 0;
        int animationSize = 0;
        float m_duration;

        AnimationTypeInfo()
        {
        }
        AnimationTypeInfo(aex::string animationName, int startFrame, int endFrame);
    };

    struct AEX_PUBLIC_API AnimationSequence
    {
        bool looped = true;
        int sequenceProgress = 0;
        std::vector<aex::string> sequence;
        std::vector<int> repeats;
    };

    /*class AnimationInfo
    {
    protected:

        int m_frames = 0;
        int m_currenFrame = 0;
        int m_currentPage = 0;
        AnimationTypeInfo m_currentAnimationType;
        bool m_mulitiTextureAtlas = false;
        bool m_multiAnimationTypes = false;
        std::vector<SpriteFrame> m_framesInfo;
        std::map<aex::string, AnimationTypeInfo> m_animationsInfo;
        std::vector<Texture*> m_textures;
    public:
        AnimationInfo();
        void update(aexTimeUnit elpsedtime);
    };*/

    class AEX_PUBLIC_API SpriteAnimated : public Sprite
    {
    public:

        SpriteAnimated();
        ~SpriteAnimated();
        virtual void draw(AexRenderer* gl);

        virtual void update(aexTimeUnit elpsedtime);
        virtual void finishdraw(AexRenderer* gl);
        void reset();
        void calculateFrame();
        void LoadAnimationsFromFile(const aex::string& filename);
        void LoadSeqenceFromFile(const aex::string& filename);
        void generateJson(const aex::string& filename, int pixelsX, int pixelsY, int framesX, int framesY);
        bool setAnimationType(aex::string animationTypeName);
        void setFramesInfo(std::vector<SpriteFrame> framesInfo);
        std::vector<SpriteFrame> getFramesInfo() const;
        void setFrameUniformSize(bool frameUniformSize);
        bool isFrameUniformSize() const;
        void setTime(float time);
        float getTime() const;
        void setFrameduration(float frameduration);
        float getFrameduration() const;
        void setDuration(float duration);
        float getDuration() const;
        void setCurrenFrame(int currenFrame);
        int getCurrenFrame() const;
        void setFrames(int frames);
        int getFrames() const;

        aex::string getAnimationType() const;
        void setCurrentAnimationType(AnimationTypeInfo currentAnimationType);
        AnimationTypeInfo getCurrentAnimationType() const;
        void setSpeed(float speed);
        float getSpeed() const;
        
    private:
        int m_fps = 24;
        int m_frames = 0;
        int m_currenFrame = 0;
        int m_currentPage = 0;
        AnimationTypeInfo m_currentAnimationType;
        float m_duration = 0.0f;
        float m_frameduration;
        float m_speed = 1.0f;
        bool m_rewind = false;
        bool m_paused = false;
        bool m_mulitiTextureAtlas = false;
        aexTimeUnit m_time = 0.0;
        UniformFloat2 m_UvOffsetUniform;
        UniformFloat2 m_UvSizeUniform;
        bool m_frameUniformSize = false;
        bool m_isUniformSizeSet = false;
        std::vector<SpriteFrame> m_framesInfo;
        std::map<aex::string, AnimationTypeInfo> m_animationsInfo;

        std::vector<Texture*> m_textures;
        bool m_multiAnimationTypes = false;
        aex::string m_animationType;
        bool m_sequence;


    };

    typedef aex::shared_ptr<SpriteAnimated> SpriteAnimated_ptr;

}


#endif	/* SPRITEANIMATED_HPP */

