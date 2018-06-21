#include <aex/SubsystemGuard.hpp>
#include "gmock/gmock.h"

using ::testing::Return;
using ::testing::SaveArg;
using ::testing::_;
template<class T>
using NiceMock = ::testing::NiceMock<T>;

static CONSTEXPR std::array<aex::SubsystemIDType, 8> SubsystemFlags
{{
    aex::VideoSubsystem,
    aex::AudioSubsystem,
    aex::EventsSubsystem,
    aex::TimerSubsystem,
    aex::JoystickSubsystem,
    aex::HapticSubsystem,
    aex::ControllerSubsystem,
    aex::FontSubsystem
}};

TEST(SubsystemState, ValidInitState)
{
    aex::SubsystemState state;
    for(auto& flag:SubsystemFlags)
        EXPECT_FALSE(state.wasInit(flag));
}

TEST(SubsystemState, NoImplInit)
{
    aex::SubsystemState state;
    for(auto& flag:SubsystemFlags)
        EXPECT_FALSE(state.init(flag));
}

TEST(SubsystemState, NoImplQuit)
{
    aex::SubsystemState state;
    for(auto& flag:SubsystemFlags)
        state.quit(flag);
}

TEST(SubsystemState, NoImplGetError)
{
    aex::SubsystemState state;
    EXPECT_EQ(state.getError(), "No implementation set.");
}

TEST(SubsystemState, NoImplClearError)
{
    aex::SubsystemState state;
    state.clearError();
}

TEST(SubsystemState, NoImplNoError)
{
    aex::SubsystemState state;
    EXPECT_FALSE(state.noError());
}

TEST(SubsystemState, NoImplBoolConversion)
{
    aex::SubsystemState state;
    EXPECT_FALSE(bool(state));
}

class MockSubsystemInitManager : public aex::ISubsystemInitManager
{
    public:
        enum class CtorID
        {
            DEFAULT,
            MOVE,
            COPY,
            BAD
        };
        MOCK_METHOD1(init, bool(aex::SubsystemIDType subsystems));
        MOCK_METHOD1(quit, void(aex::SubsystemIDType subsystems));
        MOCK_CONST_METHOD0(getError, const aex::string&());
        MOCK_METHOD0(clearError, void());
        MOCK_CONST_METHOD0(noError, bool());
        CtorID *var;
        MockSubsystemInitManager() : var(nullptr){}
        explicit MockSubsystemInitManager(CtorID* _ref) : var(_ref)
        {
            *var = CtorID::DEFAULT;
        }
        MockSubsystemInitManager(MockSubsystemInitManager&& mgr) : var(mgr.var)
        {
            *var = CtorID::MOVE;
        }
        MockSubsystemInitManager(const MockSubsystemInitManager& mgr) : var(mgr.var)
        {
            *var = CtorID::COPY;
        }
};

ACTION_P(ReturnQuittingPointee, q)
{

}

TEST(SubsystemState, CanInitAndDestroy)
{
    int enabledSubsystems = 0;
    int cleanedSubsystems;
    {
        aex::SubsystemState state;
        MockSubsystemInitManager* mockManager = new MockSubsystemInitManager();
        for(std::size_t i = 0; i < SubsystemFlags.size() / 2; i++)
        {
            EXPECT_CALL(*mockManager, init(SubsystemFlags[i]))
                .Times(1)
                .WillRepeatedly(Return(true))
                ;
            enabledSubsystems |= SubsystemFlags[i];
        }
        for(std::size_t i = SubsystemFlags.size() / 2; i < SubsystemFlags.size(); i++)
            EXPECT_CALL(*mockManager, init(SubsystemFlags[i]))
                .Times(1)
                .WillRepeatedly(Return(false))
                ;
        EXPECT_CALL(*mockManager, quit(_))
            .Times(1)
            .WillRepeatedly(SaveArg<0>(&cleanedSubsystems))
            ;
        state.trySet([mockManager](){return std::unique_ptr<MockSubsystemInitManager>(mockManager);});
        mockManager = nullptr;

        for(std::size_t i = 0; i < SubsystemFlags.size() / 2; i++)
            EXPECT_TRUE(state.init(SubsystemFlags[i]));
        for(std::size_t i = SubsystemFlags.size() / 2; i < SubsystemFlags.size(); i++)
            EXPECT_FALSE(state.init(SubsystemFlags[i]));
    }
    EXPECT_NE(enabledSubsystems, 0);
    EXPECT_EQ(enabledSubsystems, cleanedSubsystems);
}

TEST(SubsystemState, CanQuitSelective)
{
    aex::SubsystemState state;
    MockSubsystemInitManager* mockManager = new MockSubsystemInitManager();
    int enabledState = 0;
    EXPECT_CALL(*mockManager, init(_))
        .WillRepeatedly(Return(true))
        ;
    for(auto& flag : SubsystemFlags)
    {
        EXPECT_CALL(*mockManager, quit(flag))
            .Times(1)
            ;
    }
    state.trySet([mockManager](){return std::unique_ptr<MockSubsystemInitManager>(mockManager);});
    mockManager = nullptr;
    for(auto& flag : SubsystemFlags)
    {
        state.init(flag);
        enabledState |= flag;
    }
    for(auto& flag : SubsystemFlags)
    {
        state.quit(flag);
        enabledState &= ~flag;
        EXPECT_FALSE(state.wasInit(flag));
        EXPECT_TRUE(state.wasInit(enabledState));
    }
}

TEST(SubsystemState, CanChangeImpl)
{
    aex::SubsystemState state;
    MockSubsystemInitManager* mockManager = new MockSubsystemInitManager();
    int enabledFlags = 0;
    for(auto& flag : SubsystemFlags)
    {
        EXPECT_CALL(*mockManager, init(flag))
            .Times(1)
            .WillRepeatedly(Return(true))
            ;
        enabledFlags |= flag;
    }
    EXPECT_CALL(*mockManager, quit(enabledFlags))
        .Times(1)
        ;
    state.trySet([mockManager](){return std::unique_ptr<MockSubsystemInitManager>(mockManager);});
    mockManager = new MockSubsystemInitManager();
    for(auto& flag : SubsystemFlags)
        state.init(flag);

    state.trySet([mockManager](){return std::unique_ptr<MockSubsystemInitManager>(mockManager);});
    for(auto& flag : SubsystemFlags)
        EXPECT_FALSE(state.wasInit(flag));
}

class TestSubsystemGuardBase : public aex::SubsystemGuard
{
    public:
        TestSubsystemGuardBase() : var(MockSubsystemInitManager::CtorID::BAD){}
        MockSubsystemInitManager::CtorID var;
};

class TestSubsystemGuardFactory : public TestSubsystemGuardBase 
{
    public:
        struct callable 
        {
            std::unique_ptr<aex::ISubsystemInitManager> operator()()
            {
                return std::unique_ptr<aex::ISubsystemInitManager>{new MockSubsystemInitManager{var}};
            }
            MockSubsystemInitManager::CtorID *var;
        };
        bool init();
};

class TestSubsystemGuardLambda : public TestSubsystemGuardBase 
{
    public:
        bool init();
};

class TestSubsystemGuardCopy : public TestSubsystemGuardBase
{
    public:
        bool init();
};

class TestSubsystemGuardMove : public TestSubsystemGuardBase
{
    public:
        bool init();
};

class TestSubsystemGuardConversion : public TestSubsystemGuardBase
{
    public:
        bool init();
};


bool
TestSubsystemGuardFactory::init()
{
    return setSubsystemInitManager(callable{&var});
}

bool
TestSubsystemGuardLambda::init()
{
    auto lambda = [this](){return std::unique_ptr<aex::ISubsystemInitManager>{new MockSubsystemInitManager{&var}};};
    return setSubsystemInitManager(lambda);
}

bool
TestSubsystemGuardCopy::init()
{
    MockSubsystemInitManager mgr(&var);
    return setSubsystemInitManager(mgr);
}

bool
TestSubsystemGuardMove::init()
{
    return setSubsystemInitManager(MockSubsystemInitManager{&var});
}

bool
TestSubsystemGuardConversion::init()
{
    auto mgr = std::unique_ptr<MockSubsystemInitManager>{new NiceMock<MockSubsystemInitManager>{&var}};
    return setSubsystemInitManager(std::move(mgr));
}

TEST(SubsystemGuard, CanInit)
{
    TestSubsystemGuardFactory sgf;
    TestSubsystemGuardLambda sgl;
    TestSubsystemGuardCopy sgc;
    TestSubsystemGuardMove sgm;
    TestSubsystemGuardConversion sgcv;
    EXPECT_TRUE(sgf.init());
    EXPECT_EQ(MockSubsystemInitManager::CtorID::DEFAULT, sgf.var);
    EXPECT_TRUE(sgl.init());
    EXPECT_EQ(MockSubsystemInitManager::CtorID::DEFAULT, sgl.var);
    EXPECT_TRUE(sgc.init());
    EXPECT_EQ(MockSubsystemInitManager::CtorID::COPY, sgc.var);
    EXPECT_TRUE(sgm.init());
    EXPECT_EQ(MockSubsystemInitManager::CtorID::MOVE, sgm.var);
    EXPECT_TRUE(sgcv.init());
    EXPECT_EQ(MockSubsystemInitManager::CtorID::DEFAULT, sgcv.var);
}

class TestSubsystemGuard : public aex::SubsystemGuard
{
    public:
        TestSubsystemGuard() : TestSubsystemGuard(aex::SubsystemIDType{}){}
        explicit TestSubsystemGuard(aex::SubsystemIDType s) :
            aex::SubsystemGuard(s),
            manager{new NiceMock<MockSubsystemInitManager>{}},
            mgrRef{manager.get()}
            {}
        explicit TestSubsystemGuard(const TestSubsystemGuard& rhs) :
            aex::SubsystemGuard(rhs),
            manager{nullptr},
            mgrRef(rhs.mgrRef)
            {}
        TestSubsystemGuard& operator=(const TestSubsystemGuard& rhs) 
        {
            aex::SubsystemGuard::operator=(rhs);
            manager.reset();
            mgrRef = rhs.mgrRef;
            return *this;
        }
        bool init();
        std::unique_ptr<MockSubsystemInitManager> manager;
        MockSubsystemInitManager* mgrRef;
};

bool
TestSubsystemGuard::init()
{
    return manager && setSubsystemInitManager(std::move(manager));
}

TEST(SubsystemGuard, CanStartAndStopVideo)
{
    TestSubsystemGuard sg;
    sg.init();
    EXPECT_CALL(*sg.mgrRef, init(aex::VideoSubsystem))
        .Times(3)
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true))
        ;
    EXPECT_CALL(*sg.mgrRef, quit(aex::VideoSubsystem))
        .Times(2)
        ;
    EXPECT_THROW(sg.start_video(), aex::FailedToInitSubsystem);
    sg.start_video();
    sg.start_video();
    sg.stop_video();
    sg.start_video();
}

TEST(SubsystemGuard, CanStartAndStopAudio)
{
    TestSubsystemGuard sg;
    sg.init();
    EXPECT_CALL(*sg.mgrRef, init(aex::AudioSubsystem))
        .Times(3)
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true))
        ;
    EXPECT_CALL(*sg.mgrRef, quit(aex::AudioSubsystem))
        .Times(2)
        ;
    EXPECT_THROW(sg.start_audio(), aex::FailedToInitSubsystem);
    sg.start_audio();
    sg.start_audio();
    sg.stop_audio();
    sg.start_audio();
}

TEST(SubsystemGuard, CanStartAndStopEvents)
{
    TestSubsystemGuard sg;
    sg.init();
    EXPECT_CALL(*sg.mgrRef, init(aex::EventsSubsystem))
        .Times(3)
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true))
        ;
    EXPECT_CALL(*sg.mgrRef, quit(aex::EventsSubsystem))
        .Times(2)
        ;
    EXPECT_THROW(sg.start_events(), aex::FailedToInitSubsystem);
    sg.start_events();
    sg.start_events();
    sg.stop_events();
    sg.start_events();
}

TEST(SubsystemGuard, CanStartAndStopTimer)
{
    TestSubsystemGuard sg;
    sg.init();
    EXPECT_CALL(*sg.mgrRef, init(aex::TimerSubsystem))
        .Times(3)
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true))
        ;
    EXPECT_CALL(*sg.mgrRef, quit(aex::TimerSubsystem))
        .Times(2)
        ;
    EXPECT_THROW(sg.start_timer(), aex::FailedToInitSubsystem);
    sg.start_timer();
    sg.start_timer();
    sg.stop_timer();
    sg.start_timer();
}

TEST(SubsystemGuard, CanStartAndStopJoystick)
{
    TestSubsystemGuard sg;
    sg.init();
    EXPECT_CALL(*sg.mgrRef, init(aex::JoystickSubsystem))
        .Times(3)
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true))
        ;
    EXPECT_CALL(*sg.mgrRef, quit(aex::JoystickSubsystem))
        .Times(2)
        ;
    EXPECT_THROW(sg.start_joystick(), aex::FailedToInitSubsystem);
    sg.start_joystick();
    sg.start_joystick();
    sg.stop_joystick();
    sg.start_joystick();
}

TEST(SubsystemGuard, CanStartAndStopHaptic)
{
    TestSubsystemGuard sg;
    sg.init();
    EXPECT_CALL(*sg.mgrRef, init(aex::HapticSubsystem))
        .Times(3)
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true))
        ;
    EXPECT_CALL(*sg.mgrRef, quit(aex::HapticSubsystem))
        .Times(2)
        ;
    EXPECT_THROW(sg.start_haptic(), aex::FailedToInitSubsystem);
    sg.start_haptic();
    sg.start_haptic();
    sg.stop_haptic();
    sg.start_haptic();
}

TEST(SubsystemGuard, CanStartAndStopController)
{
    TestSubsystemGuard sg;
    sg.init();
    EXPECT_CALL(*sg.mgrRef, init(aex::ControllerSubsystem))
        .Times(3)
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true))
        ;
    EXPECT_CALL(*sg.mgrRef, quit(aex::ControllerSubsystem))
        .Times(2)
        ;
    EXPECT_THROW(sg.start_controller(), aex::FailedToInitSubsystem);
    sg.start_controller();
    sg.start_controller();
    sg.stop_controller();
    sg.start_controller();
}

TEST(SubsystemGuard, CanStartAndStopFont)
{
    TestSubsystemGuard sg;
    sg.init();
    EXPECT_CALL(*sg.mgrRef, init(aex::FontSubsystem))
        .Times(3)
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true))
        ;
    EXPECT_CALL(*sg.mgrRef, quit(aex::FontSubsystem))
        .Times(2)
        ;
    EXPECT_THROW(sg.start_fonts(), aex::FailedToInitSubsystem);
    sg.start_fonts();
    sg.start_fonts();
    sg.stop_fonts();
    sg.start_fonts();
}

TEST(SubsystemGuard, CanStartAndStopAll)
{
    auto allFlag = 0;
    for(auto& flag : SubsystemFlags)
        allFlag |= flag;
    TestSubsystemGuard sg;
    sg.init();
    EXPECT_CALL(*sg.mgrRef, init(allFlag))
        .Times(3)
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true))
        ;
    EXPECT_CALL(*sg.mgrRef, quit(allFlag))
        .Times(2)
        ;
    EXPECT_THROW(sg.start_all(), aex::FailedToInitSubsystem);
    sg.start_all();
    sg.start_all();
    sg.stop_all();
    sg.start_all();
}

TEST(SubsystemGuard, CanLazyInit)
{
    aex::SubsystemIDType allFlag = 0;
    for(auto& flag : SubsystemFlags)
        allFlag |= flag;
    TestSubsystemGuard sg(allFlag);
    EXPECT_CALL(*sg.mgrRef, init(allFlag))
        .Times(1)
        .WillRepeatedly(Return(true))
        ;
    EXPECT_CALL(*sg.mgrRef, quit(allFlag))
        .Times(1)
        ;
    sg.init();
}

TEST(SubsystemState, CanBeShared)
{
    aex::SubsystemIDType allFlag = 0;
    TestSubsystemGuard sg;
    for(auto& flag : SubsystemFlags)
        allFlag |= flag;
    EXPECT_FALSE(sg.isEnabled(allFlag));
    {
        TestSubsystemGuard t_sg;
        t_sg.init();
        EXPECT_CALL(*t_sg.mgrRef, init(allFlag))
            .Times(1)
            .WillRepeatedly(Return(true))
            ;
        EXPECT_CALL(*t_sg.mgrRef, quit(allFlag))
            .Times(1)
            ;
        t_sg.start_all();
        sg = t_sg;
    }
    EXPECT_TRUE(sg.isEnabled(allFlag));
}
