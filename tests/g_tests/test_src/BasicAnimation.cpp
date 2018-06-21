/**************************************************************
 *                                                            * 
 *      Filename:   BasicAnimation.cpp                        * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 wrz 18 10:22:04                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#include "MockSimulationDataTraits.hpp"
#include "gmock/gmock.h"
#include <aex/BasicAnimation.hpp>

using FrameIndex = aex::BasicFrameIndex<int>;
using FrameStateTypeInfo = aex::BasicFrameStateTypeInfo<FrameIndex, MockSimDataTraits>;
using ReverseReplay = aex::ReverseReplayDistanceEvaluator<FrameIndex>;
using FrameRange = aex::BasicFrameRange<FrameIndex>;
using ReverseFrameRange = aex::BasicFrameRange<FrameIndex>;
using StaticFrameLayout = aex::StaticFrameLayout<MockSimDataTraits, FrameRange>;
using FStaticFrameLayout = aex::StaticFrameLayout<FMockSimDataTraits, FrameRange>;
using FrameState = aex::BasicFrameState<FrameStateTypeInfo>;

template<class T>
class TMockPlayer
{
    public:
        using simulation_data_traits_type = FMockSimDataTraits;
        using animation_list_type = TMockPlayer;
        using simulation_data_type = int;
        MOCK_METHOD1_T(addAnimation, void(T*));
        MOCK_METHOD1_T(removeAnimation, void(T*));
        MOCK_METHOD1_T(runAnimationImpl, void(T*));
        MOCK_METHOD1_T(stopAnimationImpl, void(T*));
        MOCK_METHOD1_T(pauseAnimationImpl, void(T*));
        MOCK_METHOD1_T(resumeAnimationImpl, void(T*));
        MOCK_METHOD0_T(getAnimationsList, animation_list_type*());
    protected:
        void runAnimation(T* p)
        {
            runAnimationImpl(p);
        }
        void stopAnimation(T* p)
        {
            stopAnimationImpl(p);
        }
        void pauseAnimation(T* p)
        {
            pauseAnimationImpl(p);
        }
        void resumeAnimation(T* p)
        {
            resumeAnimationImpl(p);
        }
};

class MockData
{
    public:
        MOCK_METHOD1(setFrameState, void(const aex::FrameState&));
    private:
        /* data */
};

using MockDataUpdater = aex::BasicAnimationDataUpdater<MockData>;
using MockPlayerDelegate = aex::PlayerDelegate<TMockPlayer>;
using DataDelegate = aex::BasicDataDelegate<FrameState>;
using Animation = aex::BasicAnimation<DataDelegate, FStaticFrameLayout, MockPlayerDelegate>;

template<class T>
struct TestGuard
{
    TestGuard()
    {
        TMockSimDataTraits<T>::_instance = &instance;
        instance.SetUpDefaults();
    }
    ~TestGuard()
    {
        TMockSimDataTraits<T>::_instance = nullptr;
    }

    TMockSimDataTraitsInstance<T> instance;
};

TEST(StaticFrameLayout, CanDiv)
{
    MockSimDataTraitsInstance instance;
    MockSimDataTraits::_instance = &instance;
    instance.SetUpDefaults();
    EXPECT_CALL(instance, div(_, _))
        .Times(1)
        ;
    StaticFrameLayout layout;
    MockSimDataTraits::_instance = nullptr;
}

TEST(StaticFrameLayout, CanStartFrameLayout)
{
    MockSimDataTraitsInstance::simulation_data_type frame = 1;
    MockSimDataTraitsInstance instance;
    MockSimDataTraits::_instance = &instance;
    instance.SetUpDefaults();
    EXPECT_CALL(instance, div(_, _))
        .Times(1)
        ;
    EXPECT_CALL(instance, mul(_, frame))
        .Times(1)
        ;
    StaticFrameLayout layout;
    layout.getFrameEnd(layout.begin());
    MockSimDataTraits::_instance = nullptr;
}

TEST(StaticFrameLayout, CanDefaultConstruct)
{
    MockSimDataTraitsInstance instance;
    MockSimDataTraits::_instance = &instance;
    instance.SetUpDefaults();
    StaticFrameLayout layout;
    EXPECT_EQ(24, layout.size());
    MockSimDataTraits::_instance = nullptr;
}

TEST(StaticFrameLayout, CanIterate)
{
    MockSimDataTraitsInstance instance;
    MockSimDataTraits::_instance = &instance;
    instance.SetUpDefaults();
    StaticFrameLayout layout;
    auto it = layout.begin();
    while(it != layout.end())
        it++;
    EXPECT_EQ(layout.end(), it);
    MockSimDataTraits::_instance = nullptr;
}

TEST(StaticFrameLayout, CanGetTime)
{
    MockSimDataTraitsInstance instance;
    MockSimDataTraits::_instance = &instance;
    instance.SetUpDefaults();
    EXPECT_CALL(instance, unit())
        .Times(1)
        .WillRepeatedly(Return(24));
    StaticFrameLayout layout;
    int i = 0;
    for(auto it = layout.begin(); it != layout.end(); it++, i++ )
    {
        EXPECT_EQ(i, layout.getFrameStart(it));
        EXPECT_EQ((i+1)%layout.size(), layout.getFrameEnd(it));
    }
    MockSimDataTraits::_instance = nullptr;
}

TEST(StaticFrameLayout, CanReturnFrameForSimData)
{
    FMockSimDataTraitsInstance instance;
    FMockSimDataTraits::_instance = &instance;
    instance.SetUpDefaults();
    FStaticFrameLayout layout;
    auto steps = layout.size() * 2;
    auto duration = instance.unit();
    auto durStep = duration/steps;
    for(int i = 0; i < steps; i++)
    {
        auto frame = layout.getFrameAt(durStep * i);
        auto idx = FrameIndex{i/2};
        EXPECT_EQ(idx, *frame);
    }
    FMockSimDataTraits::_instance = nullptr;
}

TEST(BasicAnimation, CanSetPlayer)
{
    TestGuard<float> testGuard;
    TMockPlayer<MockPlayerDelegate> player;
    Animation anim;
    EXPECT_CALL(player, addAnimation(_))
        .Times(1);
    EXPECT_CALL(player, getAnimationsList())
        .Times(1)
        .WillRepeatedly(Return(&player));
    anim.setPlayer(&player);
}

TEST(BasicAnimation, CanRemove)
{
    TestGuard<float> testGuard;
    TMockPlayer<MockPlayerDelegate> player;
    Animation anim;
    EXPECT_CALL(player, getAnimationsList())
        .Times(2)
        .WillRepeatedly(Return(&player));
    EXPECT_CALL(player, addAnimation(_))
        .Times(1);
    EXPECT_CALL(player, removeAnimation(_))
        .Times(1);
    anim.setPlayer(&player);
    anim.setPlayer(&player);
    anim.setPlayer();
}

