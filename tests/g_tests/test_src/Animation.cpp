/**************************************************************
 *                                                            * 
 *      Filename:   Animation.cpp                             * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 wrz 24 12:40:29                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/

#include "gmock/gmock.h"
#include <aex/Animation.hpp>
#include <aex/Math.hpp>

using FrameStateType = typename std::decay<decltype(std::declval<aex::FFStaticAnimation>().getDataDelegate().getState())>::type;
using ::testing::_;

static CONSTEXPR unsigned SAMPLE_SIZE = 10;
static CONSTEXPR unsigned UPDATE_PRECISION = 100;
static CONSTEXPR double DURATION = 1.0;
static CONSTEXPR unsigned FRAMES = 24;

namespace aex{
    bool operator==(const ::FrameStateType& lhs, const ::FrameStateType& rhs)
    {
        return aMath::Math::closeEnough(lhs.factor, rhs.factor) && lhs.startingFrame == rhs.startingFrame && lhs.endingFrame == rhs.endingFrame;
    }
}

class MockData
{
    public:
        MOCK_METHOD1(setFrameState, void(const FrameStateType&));
};

class FFStaticAnimationTest : public ::testing::Test
{
    public:
        FFStaticAnimationTest() :
            ::testing::Test(),
            animation(),
            player(),
            simulation(),
            updater(animation.getDataDelegate())
    {
    }
    protected:
        virtual void SetUp()
        {
            animation.setDurataion(DURATION);
            animation.setFrameCount(FRAMES);
            animation.setPlayer(&player);
        }

        aex::FFStaticAnimation animation;
        aex::FFPlayer player;
        aex::ATSimulation simulation;
        aex::BasicAnimationDataUpdater<MockData> updater;
};

TEST_F(FFStaticAnimationTest, CanInitialize)
{
    EXPECT_EQ(animation.getDataDelegate().getState(), FrameStateType{});
}

TEST_F(FFStaticAnimationTest, CanSetDurationLength)
{
    const auto& animationDuration = animation.getDuration();
    const auto& animationLength = animation.getLayout().size();
    const auto& firstFrame = animation.getLayout().begin();
    const auto& frameDuration = animation.getLayout().frameDifference(firstFrame, (firstFrame+1));
    EXPECT_EQ(animationDuration, DURATION);
    EXPECT_EQ(animationLength, FRAMES);
    EXPECT_FLOAT_EQ(frameDuration, animationDuration / static_cast<aex::aexTimeUnit>(animationLength));
}

TEST_F(FFStaticAnimationTest, CanSetData)
{
    MockData animationDataSamples[SAMPLE_SIZE];
    FrameStateType callParams[SAMPLE_SIZE];
    const auto& animationDuration = animation.getDuration();
    const auto& animationLength = animation.getLayout().size();
    const auto& frameDuration = animationDuration / static_cast<
            typename std::decay<decltype(animationDuration)>::type
        >(animationLength);

    for(unsigned i = 0; i < SAMPLE_SIZE; i++)
    {
        EXPECT_CALL(animationDataSamples[i], setFrameState(_))
            .WillRepeatedly(::testing::SaveArg<0>(&(callParams[i])));
        updater.addAnimationData(&animationDataSamples[i]);
    }

    for(double sum = 0.0; sum < DURATION; sum += DURATION/static_cast<double>(UPDATE_PRECISION))
    {
        simulation.setData(sum);
        player.visit(simulation);
        player.run();
        animation.updateData();
        updater.update();
        auto frameNumber = aex::FrameIndex{static_cast<typename aex::FrameIndex::value_type>(sum / frameDuration)};
        auto nextFrame = aex::FrameIndex((frameNumber+static_cast<typename aex::FrameIndex::frame_distance_type>(1)) % FRAMES);
        auto factor = (sum - frameDuration * frameNumber)*animationDuration / frameDuration;
        FrameStateType expectedState(factor, frameNumber, nextFrame);
        for(int i = 0; i < SAMPLE_SIZE; i++)
            EXPECT_EQ(expectedState, callParams[i]);
    }
}
