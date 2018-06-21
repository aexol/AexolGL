/**************************************************************
 *                                                            * 
 *      Filename:   KeyFrameData.cpp                          * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 wrz 30 15:28:03                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/

#include <aex/KeyFrameData.hpp>
#include <aex/ShaderProgram.hpp>
#include "gmock/gmock.h"

using ::testing::Return;
using ::testing::Ref;
using ::testing::Eq;
using ShaderProgram = aex::ShaderProgram;
uintptr_t arbitrary_ptr_value = 0x123456;
void* arbitrary_ptr = reinterpret_cast<void*>(arbitrary_ptr_value);
const ShaderProgram& program = *static_cast<ShaderProgram*>(arbitrary_ptr);

class MockUniqueStateImpl
{
    public:
        using VertexAttribList = double;
        using AttribList = int;
        MOCK_CONST_METHOD1(enable, void(const ShaderProgram&));
        MOCK_CONST_METHOD1(enableAsNext, void(const ShaderProgram&));
        MOCK_CONST_METHOD1(disable, void(const ShaderProgram&));
        MOCK_CONST_METHOD1(disableAsNext, void(const ShaderProgram&));
        MOCK_METHOD1(setNextIdentity, void(const VertexAttribList&));
        MOCK_METHOD1(makeIdentityList, VertexAttribList(const AttribList&));
};

class MockUniqueState
{
    public:
        using  VertexAttribList = MockUniqueStateImpl::VertexAttribList;
        using  AttribList = MockUniqueStateImpl::AttribList;
        void enable(const ShaderProgram& program) const
        {
            MockUniqueState::_instance->enable(program);
        }
        void enableAsNext(const ShaderProgram& program) const
        {
            MockUniqueState::_instance->enableAsNext(program);
        }
        void disable(const ShaderProgram& program) const
        {
            MockUniqueState::_instance->disable(program);
        }
        void disableAsNext(const ShaderProgram& program) const
        {
            MockUniqueState::_instance->disableAsNext(program);
        }
        void setNextIdentity(const MockUniqueStateImpl::VertexAttribList& val)
        {
            MockUniqueState::_instance->setNextIdentity(val);
        }
        VertexAttribList makeIdentityList(const MockUniqueStateImpl::AttribList& al)
        {
            return MockUniqueState::_instance->makeIdentityList(al);
        }
        static MockUniqueStateImpl* _instance;
};
MockUniqueStateImpl*
MockUniqueState::_instance = nullptr;

struct UniqueStateGuard
{
    UniqueStateGuard()
    {
        if(!MockUniqueState::_instance)
            MockUniqueState::_instance = &m_impl;
    }
    ~UniqueStateGuard()
    {
        if(&m_impl == MockUniqueState::_instance)
            MockUniqueState::_instance = nullptr;
    }
    MockUniqueStateImpl m_impl;
};

class MockSharedState
{
    public:
        MOCK_METHOD1(enable, void(const ShaderProgram&));
        MOCK_METHOD1(disable, void(const ShaderProgram&));
};

using KeyFrame = aex::KeyFrame<MockUniqueState, MockSharedState>;
TEST(KeyFrame, CanSetSharedState)
{
    MockSharedState state;
    KeyFrame frame;
    frame.setSharedState(&state);
}

TEST(KeyFrame, CanMakeIdentityList)
{
    UniqueStateGuard guard;
    MockUniqueStateImpl& impl = guard.m_impl;
    KeyFrame frame;
    int name = 7;
    double mapping = 0.123456;
    EXPECT_CALL(impl, makeIdentityList(name))
        .Times(1)
        .WillRepeatedly(Return(mapping));
    EXPECT_FLOAT_EQ(frame.makeIdentityList(name), mapping);
}

TEST(KeyFrame, CanSetNextMap)
{
    UniqueStateGuard guard;
    MockUniqueStateImpl& impl = guard.m_impl;
    KeyFrame frame;
    double mapping = 0.123456;
    EXPECT_CALL(impl, setNextIdentity(Ref(mapping)))
        .Times(1);
    frame.setNextMap(mapping);
}

TEST(KeyFrame, CanEnableState)
{
    UniqueStateGuard guard;
    MockUniqueStateImpl& impl = guard.m_impl;
    KeyFrame frame;
    EXPECT_CALL(impl, enable(Ref(program)))
        .Times(1);
    frame.enable(program);
}

TEST(KeyFrame, CanEnableNextState)
{
    UniqueStateGuard guard;
    MockUniqueStateImpl& impl = guard.m_impl;
    KeyFrame frame;
    EXPECT_CALL(impl, enableAsNext(Ref(program)))
        .Times(1);
    frame.enableAsNext(program);
}

TEST(KeyFrame, CanEnableSharedState)
{
    MockSharedState sharedState;
    KeyFrame frame;
    EXPECT_CALL(sharedState, enable(Ref(program)))
        .Times(1);
    frame.enableSharedState(program);
    frame.setSharedState(&sharedState);
    frame.enableSharedState(program);
}

TEST(KeyFrame, CanDisableState)
{
    UniqueStateGuard guard;
    MockUniqueStateImpl& impl = guard.m_impl;
    KeyFrame frame;
    EXPECT_CALL(impl, disable(Ref(program)))
        .Times(1);
    frame.disable(program);
}

TEST(KeyFrame, CanDisableNextState)
{
    UniqueStateGuard guard;
    MockUniqueStateImpl& impl = guard.m_impl;
    KeyFrame frame;
    EXPECT_CALL(impl, disableAsNext(Ref(program)))
        .Times(1);
    frame.disableAsNext(program);
}

TEST(KeyFrame, CanDisableSharedState)
{
    MockSharedState sharedState;
    KeyFrame frame;
    EXPECT_CALL(sharedState, disable(Ref(program)))
        .Times(1);
    frame.disableSharedState(program);
    frame.setSharedState(&sharedState);
    frame.disableSharedState(program);
}

using KeyFrameArray = aex::KeyFrameArray<MockUniqueState, MockSharedState>;
using KeyFrameAnimationData = aex::KeyFrameAnimationData<KeyFrameArray>;

TEST(KeyFrameAnimation, CanMakeFrameMap)
{
    UniqueStateGuard guard;
    MockUniqueStateImpl& impl = guard.m_impl;
    KeyFrameAnimationData animation;
    int name = 7;
    double mapping = 0.123456;
    EXPECT_CALL(impl, makeIdentityList(Ref(name)))
        .Times(1)
        .WillRepeatedly(Return(mapping));
    EXPECT_CALL(impl, setNextIdentity(Eq(mapping)))
        .Times(animation.animationSize());
    animation.setNextFrameMap(name);
}
