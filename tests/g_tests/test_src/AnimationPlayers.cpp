#include "MockSimulationDataTraits.hpp"
#include "aex/AnimationPlayers.hpp"
#include "aex/Simulation.hpp"
#include "aex/Traits.hpp"
#include "gmock/gmock.h"
#include <limits>
#include <random>
#include <iostream>

using ::testing::Return;
using ::testing::ReturnArg;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::AtLeast;
using ::testing::Between;

class MockRunnerNoPauseResumeInit;
class MockRunnerNoPauseResume;
class MockRunner;
class MockFrame
{
    public:
        MOCK_METHOD0(begin, float());
        MOCK_METHOD0(end, float());
};

class AnimationMockType
{
    public:
        typedef aex::SimulationDataTraits<int>::simulation_data_type simulation_data_type;
        MOCK_METHOD1(startAnimation, bool(aex::BasicAnimationPlayer<AnimationMockType, MockRunnerNoPauseResumeInit>*));
        MOCK_METHOD1(stopAnimation, bool(aex::BasicAnimationPlayer<AnimationMockType, MockRunnerNoPauseResumeInit>*));
        MOCK_METHOD1(startAnimation, bool(aex::BasicAnimationPlayer<AnimationMockType, MockRunnerNoPauseResume>*));
        MOCK_METHOD1(stopAnimation, bool(aex::BasicAnimationPlayer<AnimationMockType, MockRunnerNoPauseResume>*));
        MOCK_METHOD1(startAnimation, bool(aex::BasicAnimationPlayer<AnimationMockType, MockRunner>*));
        MOCK_METHOD1(stopAnimation, bool(aex::BasicAnimationPlayer<AnimationMockType, MockRunner>*));
        MOCK_METHOD0(init, bool());
        MOCK_METHOD1(run, void(const simulation_data_type& data));
        MOCK_METHOD0(pause, void());
        MOCK_METHOD0(resume, void());
        MOCK_CONST_METHOD0(getDuration, simulation_data_type&());
        MOCK_CONST_METHOD0(getSimulationState, simulation_data_type&());
        void SetUpDefaults()
        {
            ON_CALL(*this, getDuration())
                .WillByDefault(::testing::ReturnRef(d))
                ;
            ON_CALL(*this, getSimulationState())
                .WillByDefault(::testing::ReturnRef(d))
                ;
        }
    private:
        simulation_data_type d;
};

class MockRunnerNoPauseResumeInit
{
    public:
        MOCK_CONST_METHOD1(run, void(AnimationMockType&));
};

class MockRunnerNoPauseResume
{
    public:
        MOCK_CONST_METHOD1(run, void(AnimationMockType&));
        MOCK_CONST_METHOD1(init, bool(AnimationMockType&));
};

class MockRunner
{
    public:
        MOCK_CONST_METHOD1(run, void(AnimationMockType&));
        MOCK_CONST_METHOD1(init, bool(AnimationMockType&));
        MOCK_CONST_METHOD1(pause, void(AnimationMockType&));
        MOCK_CONST_METHOD1(resume, void(AnimationMockType&));
};

TEST(BasicAnimationPlayer, CanStartAnimation)
{
    AnimationMockType animation;
    aex::BasicAnimationPlayer<AnimationMockType, MockRunnerNoPauseResumeInit> player;

    EXPECT_CALL(animation, startAnimation(&player))
        .Times(5)
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true))
        ;

    // Was cleaned up after added animations?
    EXPECT_CALL(animation, stopAnimation(&player))
        .Times(4)
        .WillRepeatedly(Return(true));
        ;

    for(int i = 0; i < 5; i++)
        player.addAnimation(&animation);
}

TEST(BasicAnimationPlayer, CanStopAnimation)
{
    AnimationMockType animation;
    aex::BasicAnimationPlayer<AnimationMockType, MockRunnerNoPauseResumeInit> player;

    EXPECT_CALL(animation, stopAnimation(&player))
        .Times(2)
        .WillOnce(Return(true))
        .WillRepeatedly(Return(false));

    player.removeAnimation(&animation);
    player.removeAnimation(&animation);
}

TEST(BasicAnimationPlayer, CanRunAnimations)
{
    static CONSTEXPR const std::size_t ANIMATIONS_SIZE = 10;
    AnimationMockType animation[ANIMATIONS_SIZE];
    MockRunnerNoPauseResumeInit runner;
    aex::BasicAnimationPlayer<AnimationMockType, MockRunnerNoPauseResumeInit> player;

    for(auto& anim : animation)
    {
        EXPECT_CALL(anim, startAnimation(&player))
            .Times(1)
            .WillRepeatedly(Return(true))
            ;
        EXPECT_CALL(anim, stopAnimation(&player))
            .Times(1)
            .WillRepeatedly(Return(true))
            ;
    }
    EXPECT_CALL(runner, run(_))
        .Times(10);

    for(auto& anim: animation)
        player.addAnimation(&anim);
    player.run(runner);
}

TEST(BasicAnimationPlayer, PauseAnimations)
{
    static CONSTEXPR const std::size_t ANIMATIONS_SIZE = 10;
    AnimationMockType animation[ANIMATIONS_SIZE];
    MockRunnerNoPauseResumeInit runner;
    aex::BasicAnimationPlayer<AnimationMockType, MockRunnerNoPauseResumeInit> player;

    for(auto& anim : animation)
    {
        EXPECT_CALL(anim, startAnimation(&player))
            .Times(1)
            .WillRepeatedly(Return(true))
            ;
        EXPECT_CALL(anim, stopAnimation(&player))
            .Times(1)
            .WillRepeatedly(Return(true))
            ;
    }
    EXPECT_CALL(runner, run(_))
        .Times(0);

    for(auto& anim: animation)
        player.addAnimation(&anim);
    player.pause(runner);
    player.run(runner);
}


TEST(BasicAnimationPlayer, CanResumeAnimations)
{
    static CONSTEXPR const std::size_t ANIMATIONS_SIZE = 10;
    AnimationMockType animation[ANIMATIONS_SIZE];
    MockRunnerNoPauseResumeInit runner;
    aex::BasicAnimationPlayer<AnimationMockType, MockRunnerNoPauseResumeInit> player;

    for(auto& anim : animation)
    {
        EXPECT_CALL(anim, startAnimation(&player))
            .Times(1)
            .WillRepeatedly(Return(true))
            ;
        EXPECT_CALL(anim, stopAnimation(&player))
            .Times(1)
            .WillRepeatedly(Return(true))
            ;
    }
    EXPECT_CALL(runner, run(_))
        .Times(10);

    for(auto& anim: animation)
        player.addAnimation(&anim);
    player.pause(runner);
    player.run(runner);
    player.resume(runner);
    player.run(runner);
}

TEST(BasicAnimationPlayer, CanInit)
{
    static CONSTEXPR const std::size_t ANIMATIONS_SIZE = 10;
    AnimationMockType animation[ANIMATIONS_SIZE];
    MockRunnerNoPauseResume runner;
    aex::BasicAnimationPlayer<AnimationMockType, MockRunnerNoPauseResume> player;

    for(auto& anim : animation)
    {
        EXPECT_CALL(anim, startAnimation(&player))
            .Times(1)
            .WillRepeatedly(Return(true))
            ;
        EXPECT_CALL(anim, stopAnimation(&player))
            .Times(1)
            .WillRepeatedly(Return(true))
            ;
    }
    EXPECT_CALL(runner, init(_))
        .Times(10)
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true))
        ;
    EXPECT_CALL(runner, run(_))
        .Times(9);

    for(auto& anim: animation)
        player.addAnimation(&anim);
    player.run(runner);
}

TEST(BasicAnimationPlayer, CanPause)
{
    static CONSTEXPR const std::size_t ANIMATIONS_SIZE = 10;
    AnimationMockType animation[ANIMATIONS_SIZE];
    MockRunner runner;
    aex::BasicAnimationPlayer<AnimationMockType, MockRunner> player;

    for(auto& anim : animation)
    {
        EXPECT_CALL(anim, startAnimation(&player))
            .Times(1)
            .WillRepeatedly(Return(true))
            ;
        EXPECT_CALL(anim, stopAnimation(&player))
            .Times(1)
            .WillRepeatedly(Return(true))
            ;
    }
    EXPECT_CALL(runner, init(_))
        .Times(10)
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true))
        ;
    EXPECT_CALL(runner, pause(_))
        .Times(9);

    for(auto& anim: animation)
        player.addAnimation(&anim);
    player.pause(runner);
}

TEST(BasicAnimationPlayer, CanResume)
{
    static CONSTEXPR const std::size_t ANIMATIONS_SIZE = 10;
    AnimationMockType animation[ANIMATIONS_SIZE];
    MockRunner runner;
    aex::BasicAnimationPlayer<AnimationMockType, MockRunner> player;

    for(auto& anim : animation)
    {
        EXPECT_CALL(anim, startAnimation(&player))
            .Times(1)
            .WillRepeatedly(Return(true))
            ;
        EXPECT_CALL(anim, stopAnimation(&player))
            .Times(1)
            .WillRepeatedly(Return(true))
            ;
    }
    EXPECT_CALL(runner, init(_))
        .Times(10)
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true))
        ;
    EXPECT_CALL(runner, pause(_))
        .Times(9)
        ;
    EXPECT_CALL(runner, resume(_))
        .Times(9)
        ;

    for(auto& anim: animation)
        player.addAnimation(&anim);
    player.pause(runner);
    player.resume(runner);
}

TEST(BasicAnimationPlayer, ResumeOnlyPaused)
{
    static CONSTEXPR const std::size_t ANIMATIONS_SIZE = 10;
    AnimationMockType animation[ANIMATIONS_SIZE];
    MockRunner runner;
    aex::BasicAnimationPlayer<AnimationMockType, MockRunner> player;

    for(auto& anim : animation)
    {
        EXPECT_CALL(anim, startAnimation(&player))
            .Times(1)
            .WillRepeatedly(Return(true))
            ;
        EXPECT_CALL(anim, stopAnimation(&player))
            .Times(1)
            .WillRepeatedly(Return(true))
            ;
    }
    EXPECT_CALL(runner, resume(_))
        .Times(0);

    for(auto& anim: animation)
        player.addAnimation(&anim);
    player.resume(runner);
}

TEST(SimDataDiff, ZeroInitalized)
{
    MockSimDataTraitsInstance mockSimData;
    MockSimDataTraits::_instance = &mockSimData;
    EXPECT_CALL(mockSimData, max())
        .Times(1)
        ;
    EXPECT_CALL(mockSimData, min())
        .Times(1)
        ;
    aex::SimDataDiff<MockSimDataTraits> simDataDiff;
    MockSimDataTraits::_instance = nullptr;
}

TEST(SimDataDiff, CanUpdate)
{
    static CONSTEXPR std::size_t UPDATES_COUNT = 100;
    static CONSTEXPR std::size_t PAUSES_COUNT = 10;
    NiceMock<MockSimDataTraitsInstance> mockSimData;
    MockSimDataTraits::_instance = &mockSimData;
    mockSimData.SetUpDefaults();
    EXPECT_CALL(mockSimData, max())
        .Times(UPDATES_COUNT+PAUSES_COUNT+1)
        ;
    EXPECT_CALL(mockSimData, min())
        .Times(UPDATES_COUNT+PAUSES_COUNT+1)
        ;
    EXPECT_CALL(mockSimData, assign(_, _))
        .Times(3*UPDATES_COUNT+PAUSES_COUNT)
        ;
    EXPECT_CALL(mockSimData, eq(_, _))
        .Times(Between(UPDATES_COUNT, 2*UPDATES_COUNT))
        .WillRepeatedly(ArgsEq())
        ;
    aex::SimDataDiff<MockSimDataTraits> simDataDiff;
    for(auto i = 0; i < UPDATES_COUNT; i++)
    {
        if(i%PAUSES_COUNT == 0)
            simDataDiff.pause();
        simDataDiff.update(i);
        if(i%PAUSES_COUNT == 0)
            EXPECT_EQ(simDataDiff.getDiff(), 0);
        else
            EXPECT_EQ(simDataDiff.getDiff(), 1);
    }
    MockSimDataTraits::_instance = nullptr;
}

TEST(SimDataDiff, CanHandleOverflow)
{
    static CONSTEXPR std::size_t UPDATES_COUNT = 1000;
    const int _max = 39;
    NiceMock<MockSimDataTraitsInstance> mockSimData;
    MockSimDataTraits::_instance = &mockSimData;
    mockSimData.SetUpDefaults();
    ON_CALL(mockSimData, max())
        .WillByDefault(Return(_max))
        ;
    ON_CALL(mockSimData, min())
        .WillByDefault(Return(int{}))
        ;
    aex::SimDataDiff<MockSimDataTraits> simDataDiff;
    for(auto i = 0; i < UPDATES_COUNT; i++)
    {
        simDataDiff.update(i);
        if(!i)
            EXPECT_EQ(simDataDiff.getDiff(), 0);
        else if(i && i % _max == 0) 
            EXPECT_EQ(simDataDiff.getDiff(), -(_max-1));
        else
            EXPECT_EQ(simDataDiff.getDiff(), 1);
    }
    MockSimDataTraits::_instance = nullptr;
}

TEST(AnimationPlayerState, CanZeroInit)
{
    NiceMock<MockSimDataTraitsInstance> mockSimData;
    MockSimDataTraits::_instance = &mockSimData;
    mockSimData.SetUpDefaults();
    static CONSTEXPR std::size_t PLAYERS = 256;
    aex::BaseSimpleSimulation<MockSimDataTraits> simulation;
    simulation.setData(1);
    aex::AnimationPlayerState<MockSimDataTraits> players[PLAYERS];
    for(auto& player : players)
    {
        player.visit(simulation);
        EXPECT_EQ(player.getDiff(), 0);
    }
    
    MockSimDataTraits::_instance = nullptr;
}

TEST(AnimationPlayerState, CanUpdate)
{
    NiceMock<MockSimDataTraitsInstance> mockSimData;
    MockSimDataTraits::_instance = &mockSimData;
    mockSimData.SetUpDefaults();
    static CONSTEXPR std::size_t PLAYERS = 1;
    static CONSTEXPR std::size_t SAMPLES = 256;
    aex::BaseSimpleSimulation<MockSimDataTraits> simulation;
    unsigned tick = 1;
    simulation.setData(tick);
    aex::AnimationPlayerState<MockSimDataTraits> players[PLAYERS];
    for(auto& player : players)
    {
        player.visit(simulation);
        EXPECT_EQ(player.getDiff(), 0);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);

    for(int i = 0; i < SAMPLES; i++)
    {
        unsigned n;
        tick += (n = generator() % 15);
        for(auto& player : players)
        {
            simulation.setData(tick);
            player.visit(simulation);
            EXPECT_EQ(player.getDiff(), n);
        }
    }

    MockSimDataTraits::_instance = nullptr;
}

TEST(AnimationPlayerState, CanPauseResume)
{
    NiceMock<MockSimDataTraitsInstance> mockSimData;
    MockSimDataTraits::_instance = &mockSimData;
    mockSimData.SetUpDefaults();
    static CONSTEXPR std::size_t PLAYERS = 1;
    static CONSTEXPR std::size_t SAMPLES = 256;
    aex::BaseSimpleSimulation<MockSimDataTraits> simulation;
    unsigned tick = 1;
    simulation.setData(tick);
    aex::AnimationPlayerState<MockSimDataTraits> players[PLAYERS];
    for(auto& player : players)
    {
        player.visit(simulation);
        EXPECT_EQ(player.getDiff(), 0);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    bool wasPaused = false;

    for(int i = 0; i < SAMPLES; i++)
    {
        unsigned n;
        tick += (n = generator() % 15);
        for(auto& player : players)
        {
            simulation.setData(tick);
            if(n%2 == 1)
            {
                player.pause();
                player.visit(simulation);
                EXPECT_EQ(player.getDiff(), 0);
                wasPaused = true;
            } else {
                if(wasPaused)
                {
                    player.resume();
                }
                player.visit(simulation);
                if(wasPaused)
                {
                    EXPECT_EQ(player.getDiff(), 0);
                    wasPaused = false;
                }
                else {
                    EXPECT_EQ(player.getDiff(), n);
                }
            }
        }
    }

    MockSimDataTraits::_instance = nullptr;
}

TEST(FFRunner, CanInit)
{
    aex::FFRunner<AnimationMockType, aex::SimulationDataTraits<int>> runner;
    AnimationMockType animation;
    EXPECT_CALL(animation, init())
        .Times(1)
        ;
    runner.init(animation);
}

TEST(FFRunner, CanRun)
{
    aex::FFRunner<AnimationMockType, aex::SimulationDataTraits<int>> runner;
    AnimationMockType animation;
    animation.SetUpDefaults();
    EXPECT_CALL(animation, run(_))
        .Times(1)
        ;
    runner.run(animation);
}

TEST(FFRunner, IsForwarding)
{
    static CONSTEXPR std::size_t SAMPLES = 100;
    static CONSTEXPR std::size_t DIFF = 1;
    aex::FFRunner<AnimationMockType, aex::SimulationDataTraits<int>> runner;
    NiceMock<AnimationMockType> animation;
    AnimationMockType::simulation_data_type mockSimData = 0;
    AnimationMockType::simulation_data_type mockSimDuration = 24;
     
    EXPECT_CALL(animation, run(_))
        .WillRepeatedly(::testing::SaveArg<0>(&mockSimData))
        ;

    EXPECT_CALL(animation, getDuration())
        .WillRepeatedly(::testing::ReturnRef(mockSimDuration))
        ;

    EXPECT_CALL(animation, getSimulationState())
        .WillRepeatedly(::testing::ReturnRef(mockSimData))
        ;

    for(std::size_t i = 1; i < SAMPLES+1; i+=DIFF)
    {
        runner.m_diff = DIFF;
        runner.run(animation);
        EXPECT_EQ(mockSimData, i % mockSimDuration);
    }
}
