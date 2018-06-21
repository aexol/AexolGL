/**************************************************************
 *                                                            * 
 *      Filename:   SimpleKeyFrameAnimation.cpp               * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 paź 06 14:06:45                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/

#include "MockWindow.hpp"
#include "MockObjectGuard.hpp"
#include "SimpleKeyFrameSample.hpp"
#include <aex/Common.hpp>
#include <aex/KeyFrameData.hpp>
#include <aex/Animation.hpp>
#include <aex/DrawableMediator.hpp>
#include <gmock/gmock.h>
#include <cmath>
#include <ctgmath>

namespace aex
{
    class AexRenderer;
}
namespace _math_sup
{
#if __TARGET_PLATFORM__ == __ANDROID_OS__
    using ::round;
#else
    using std::round;
#endif // __TARGET_PLATFORM__ == __ANDROID_OS__
}

using AexRenderer = aex::AexRenderer;

uintptr_t arbitrary_ptr_value = 0x123456;
void* arbitrary_ptr = reinterpret_cast<void*>(arbitrary_ptr_value);
AexRenderer* gl = static_cast<AexRenderer*>(arbitrary_ptr);

using SimpleKeyFrameAnimationData = aex::SimpleKeyFrameAnimationData;
using FFStaticAnimation = aex::FFStaticAnimation; 
using ATSimulation = aex::ATSimulation;
using FFPlayer = aex::FFPlayer;
using SimpleKeyFrameDataUpdate = aex::BasicAnimationDataUpdater<SimpleKeyFrameAnimationData>;
using factor_type = typename FFStaticAnimation::frame_state_type::factor_type;
using ::testing::_;

class KeyFrameAnimationDrawObjectMockImpl
{
    public:
        using frame_type = typename SimpleKeyFrameAnimationData::frame_type;
        MOCK_METHOD4(draw, void(AexRenderer*, const frame_type&, const frame_type&, const factor_type&));
        MOCK_METHOD4(finishdraw, void(AexRenderer*, const frame_type&, const frame_type&, const factor_type&));
        MOCK_METHOD0(needsUpdate, void());
};

class KeyFrameAnimationDrawObjectMock
{
    public:
        using frame_type = KeyFrameAnimationDrawObjectMockImpl::frame_type;
        void draw(AexRenderer* gl, const frame_type& f1, const frame_type& f2, const factor_type& f)
        {
            KeyFrameAnimationDrawObjectMock::_instance->draw(gl, f1, f2, f);
        }
        void finishdraw(AexRenderer* gl, const frame_type& f1, const frame_type& f2, const factor_type& f)
        {
            KeyFrameAnimationDrawObjectMock::_instance->finishdraw(gl, f1, f2, f);
        }
        void needsUpdate()
        {
            KeyFrameAnimationDrawObjectMock::_instance->needsUpdate();
        }
        static KeyFrameAnimationDrawObjectMockImpl* _instance;
};

KeyFrameAnimationDrawObjectMockImpl* KeyFrameAnimationDrawObjectMock::_instance = nullptr;

using SimpleKeyFrameDrawable = aex::FrameDrawWrapper<SimpleKeyFrameAnimationData, KeyFrameAnimationDrawObjectMock>;
using SimpleKeyFrameDrawObject = aex::DrawableMediator<SimpleKeyFrameDrawable>;

TEST(SimpleKeyFrameAnimation, CanDraw)
{
    using frame_type = KeyFrameAnimationDrawObjectMock::frame_type;
    using state_type = typename FFStaticAnimation::frame_state_type;
    const auto frameCount = sampleData1.frames.size();
    using size_type = decltype(frameCount);
    const double durations[] = {1.0, 2.0, 3.0, 4.0, 3.0, 2.0, 1.0};
    const double playTime = 10.0;
    const double updateInterval = 0.1;
    MockWindow window;
    MockObjectGuard<KeyFrameAnimationDrawObjectMock, KeyFrameAnimationDrawObjectMockImpl> g;
    FFStaticAnimation animation;
    FFPlayer player;
    ATSimulation simulation;
    SimpleKeyFrameDataUpdate updater(animation.getDataDelegate());
    SimpleKeyFrameDrawObject simpleKFDrawObject(SimpleKeyFrameDrawable(makeAnimationData(sampleData1)));
    aex::DrawObject& doRef = simpleKFDrawObject.getDrawable();
    updater.addAnimationData(&simpleKFDrawObject.getDrawableImplementation().getAnimationData());
    std::vector<const frame_type*> frames;
    frames.resize(frameCount);
    for(int i = 0; i < frameCount; i++)
    {
        state_type state;
        state.startingFrame = aex::FrameIndex(i);
        state.endingFrame = aex::FrameIndex((i+1)%frameCount);
        auto& animationData = simpleKFDrawObject.getDrawableImplementation().getAnimationData();
        animationData.setFrameState(state);
        frames[i] = &animationData.getFirstFrame();
    }
    double time = 0.0;
    double animationTime = -updateInterval;
    double lastDuration = durations[0];
    for(const auto& duration : durations)
    {
        const double frameDuration = duration / static_cast<double>(frameCount);
        animation.setDurataion(duration);
        animation.setFrameCount(frameCount);
        animation.setPlayer(&player);
        animationTime *= duration / lastDuration;
        for(double begin = time, sum = begin; sum < playTime+begin; sum += updateInterval, time = sum)
        {
            animationTime += updateInterval;
            animationTime = _math_sup::round(animationTime * 10000) / 10000;
            animationTime = std::fmod(animationTime, duration);
            simulation.setData(sum);
            player.visit(simulation);
            player.run();
            animation.updateData();
            updater.update();
            auto frameNumber = static_cast<size_type>(animationTime / frameDuration);
            auto factor = (animationTime - frameDuration * frameNumber) / frameDuration;
            auto borderCaseFactor = factor;
            auto alternateFrameNumber = frameNumber;
            if(aMath::Math::closeEnough(factor, 1.0, 0.001))
            {
                borderCaseFactor = 0.0;
                alternateFrameNumber = (frameNumber + 1) % frameCount;
            }
            if(aMath::Math::closeEnough(factor, 0.0, 0.001))
            {
                borderCaseFactor = 1.0;
                alternateFrameNumber = (frameNumber + frameCount - 1) % frameCount;
            }
            EXPECT_CALL(*KeyFrameAnimationDrawObjectMock::_instance,
                    draw(gl,
                        ::testing::AnyOf(
                            ::testing::Ref(*frames[frameNumber]),
                            ::testing::Ref(*frames[alternateFrameNumber])
                            ),
                        ::testing::AnyOf(
                            ::testing::Ref(*frames[(frameNumber+1)%frameCount]),
                            ::testing::Ref(*frames[(alternateFrameNumber+1)%frameCount])
                            ),
                        ::testing::AnyOf(
                            ::testing::DoubleNear(factor, 0.001),
                            ::testing::DoubleNear(borderCaseFactor, 0.001)
                            )
                        ))
                .Times(1)
                .RetiresOnSaturation();
            EXPECT_LE(frameNumber, frameCount);
            EXPECT_LE(factor, 1.0);
            doRef.draw(gl);
        }
        lastDuration = duration;
    }
}

TEST(SimpleKeyFrameAnimation, CanFinishDraw)
{
    using frame_type = KeyFrameAnimationDrawObjectMock::frame_type;
    using state_type = typename FFStaticAnimation::frame_state_type;
    const auto frameCount = sampleData1.frames.size();
    using size_type = decltype(frameCount);
    const double durations[] = {1.0, 2.0, 3.0, 4.0, 3.0, 2.0, 1.0};
    const double playTime = 10.0;
    const double updateInterval = 0.1;
    MockWindow window;
    MockObjectGuard<KeyFrameAnimationDrawObjectMock, KeyFrameAnimationDrawObjectMockImpl> g;
    FFStaticAnimation animation;
    FFPlayer player;
    ATSimulation simulation;
    SimpleKeyFrameDataUpdate updater(animation.getDataDelegate());
    SimpleKeyFrameDrawObject simpleKFDrawObject(SimpleKeyFrameDrawable(makeAnimationData(sampleData1)));
    aex::DrawObject& doRef = simpleKFDrawObject.getDrawable();
    updater.addAnimationData(&simpleKFDrawObject.getDrawableImplementation().getAnimationData());
    std::vector<const frame_type*> frames;
    frames.resize(frameCount);
    for(int i = 0; i < frameCount; i++)
    {
        state_type state;
        state.startingFrame = aex::FrameIndex(i);
        state.endingFrame = aex::FrameIndex((i+1)%frameCount);
        auto& animationData = simpleKFDrawObject.getDrawableImplementation().getAnimationData();
        animationData.setFrameState(state);
        frames[i] = &animationData.getFirstFrame();
    }
    double time = 0.0;
    double animationTime = -updateInterval;
    double lastDuration = durations[0];
    for(const auto& duration : durations)
    {
        const double frameDuration = duration / static_cast<double>(frameCount);
        animation.setDurataion(duration);
        animation.setFrameCount(frameCount);
        animation.setPlayer(&player);
        animationTime *= duration / lastDuration;
        for(double begin = time, sum = begin; sum < playTime+begin; sum += updateInterval, time = sum)
        {
            animationTime += updateInterval;
            animationTime = _math_sup::round(animationTime * 10000) / 10000;
            animationTime = std::fmod(animationTime, duration);
            simulation.setData(sum);
            player.visit(simulation);
            player.run();
            animation.updateData();
            updater.update();
            auto frameNumber = static_cast<size_type>(animationTime / frameDuration);
            auto factor = (animationTime - frameDuration * frameNumber) / frameDuration;
            auto borderCaseFactor = factor;
            auto alternateFrameNumber = frameNumber;
            if(aMath::Math::closeEnough(factor, 1.0, 0.001))
            {
                borderCaseFactor = 0.0;
                alternateFrameNumber = (frameNumber + 1) % frameCount;
            }
            if(aMath::Math::closeEnough(factor, 0.0, 0.001))
            {
                borderCaseFactor = 1.0;
                alternateFrameNumber = (frameNumber + frameCount - 1) % frameCount;
            }
            EXPECT_CALL(*KeyFrameAnimationDrawObjectMock::_instance,
                    finishdraw(gl,
                        ::testing::AnyOf(
                            ::testing::Ref(*frames[frameNumber]),
                            ::testing::Ref(*frames[alternateFrameNumber])
                            ),
                        ::testing::AnyOf(
                            ::testing::Ref(*frames[(frameNumber+1)%frameCount]),
                            ::testing::Ref(*frames[(alternateFrameNumber+1)%frameCount])
                            ),
                        ::testing::AnyOf(
                            ::testing::DoubleNear(factor, 0.001),
                            ::testing::DoubleNear(borderCaseFactor, 0.001)
                            )
                        ))
                .Times(1)
                .RetiresOnSaturation();
            EXPECT_LE(frameNumber, frameCount);
            EXPECT_LE(factor, 1.0);
            doRef.finishdraw(gl);
        }
        lastDuration = duration;
    }
}

TEST(SimpleKeyFrameAnimation, CanHandleJerkySim)
{
    using frame_type = KeyFrameAnimationDrawObjectMock::frame_type;
    using state_type = typename FFStaticAnimation::frame_state_type;
    const auto frameCount = sampleData1.frames.size();
    using size_type = decltype(frameCount);
    const double durations[] = {1.0, 2.0, 3.0, 4.0, 3.0, 2.0, 1.0};
    const double playTime = 10.0;
    const double updateInterval = 0.1;
    MockWindow window;
    MockObjectGuard<KeyFrameAnimationDrawObjectMock, KeyFrameAnimationDrawObjectMockImpl> g;
    FFStaticAnimation animation;
    FFPlayer player;
    ATSimulation simulation;
    SimpleKeyFrameDataUpdate updater(animation.getDataDelegate());
    SimpleKeyFrameDrawObject simpleKFDrawObject(SimpleKeyFrameDrawable(makeAnimationData(sampleData1)));
    aex::DrawObject& doRef = simpleKFDrawObject.getDrawable();
    updater.addAnimationData(&simpleKFDrawObject.getDrawableImplementation().getAnimationData());
    std::vector<const frame_type*> frames;
    frames.resize(frameCount);
    for(int i = 0; i < frameCount; i++)
    {
        state_type state;
        state.startingFrame = aex::FrameIndex(i);
        state.endingFrame = aex::FrameIndex((i+1)%frameCount);
        auto& animationData = simpleKFDrawObject.getDrawableImplementation().getAnimationData();
        animationData.setFrameState(state);
        frames[i] = &animationData.getFirstFrame();
    }
    double animationTime = -updateInterval;
    double lastDuration = durations[0];
    double time = 0.0;
    for(const auto& duration : durations)
    {
        const double frameDuration = duration / static_cast<double>(frameCount);
        animation.setDurataion(duration);
        animation.setFrameCount(frameCount);
        animation.setPlayer(&player);
        animationTime *= duration / lastDuration;
        if(&duration != &durations[0])
        {
            animationTime += std::fmod(-time, duration);
            if(animationTime < 0.0)
                animationTime += duration;
            animationTime -= updateInterval;
            if(animationTime < 0.0)
                animationTime += duration;
        }
        for(double sum = 0.0f; sum < playTime; time = sum, sum += updateInterval)
        {
            animationTime += updateInterval;
            animationTime = _math_sup::round(animationTime * 10000.0) / 10000;
            animationTime = std::fmod(animationTime, duration);
            simulation.setData(sum);
            player.visit(simulation);
            player.run();
            animation.updateData();
            updater.update();
            auto frameNumber = static_cast<size_type>(animationTime / frameDuration);
            auto factor = (animationTime - frameDuration * frameNumber) / frameDuration;
            auto borderCaseFactor = factor;
            auto alternateFrameNumber = frameNumber;
            if(aMath::Math::closeEnough(factor, 1.0, 0.001))
            {
                borderCaseFactor = 0.0;
                alternateFrameNumber = (frameNumber + 1) % frameCount;
            }
            if(aMath::Math::closeEnough(factor, 0.0, 0.001))
            {
                borderCaseFactor = 1.0;
                alternateFrameNumber = (frameNumber + frameCount - 1) % frameCount;
            }
            EXPECT_CALL(*KeyFrameAnimationDrawObjectMock::_instance,
                    finishdraw(gl,
                        ::testing::AnyOf(
                            ::testing::Ref(*frames[frameNumber]),
                            ::testing::Ref(*frames[alternateFrameNumber])
                            ),
                        ::testing::AnyOf(
                            ::testing::Ref(*frames[(frameNumber+1)%frameCount]),
                            ::testing::Ref(*frames[(alternateFrameNumber+1)%frameCount])
                            ),
                        ::testing::AnyOf(
                            ::testing::DoubleNear(factor, 0.001),
                            ::testing::DoubleNear(borderCaseFactor, 0.001)
                            )
                        )
                    )
                .Times(1)
                .RetiresOnSaturation();
            EXPECT_LE(frameNumber, frameCount);
            EXPECT_LE(factor, 1.0);
            doRef.finishdraw(gl);
        }
        lastDuration = duration;
    }
}
