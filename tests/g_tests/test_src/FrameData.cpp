/**************************************************************
 *                                                            * 
 *      Filename:   FrameData.cpp                             * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 paź 01 09:29:37                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/

#include <aex/FrameData.hpp>
#include <aex/AexRenderer.hpp>
#include "gmock/gmock.h"

using ::testing::NotNull;
using ::testing::Ref;
using MockFrameData = aex::FrameData<int>;
using MockAnimatedFrameData = aex::AnimatedFrameData<MockFrameData>;
using aex::AexRenderer;
using state_type = aex::FrameState;
using factor_type = typename state_type::factor_type;

TEST(FrameData, CanConstruct)
{
    MockFrameData frames;
    MockFrameData frames2(60u);
    EXPECT_EQ(frames.size(), 24u);
    EXPECT_EQ(frames2.size(), 60u);
}

TEST(FrameData, CopyMoveSemantics)
{
    MockFrameData frames1;
    MockFrameData frames2(60u);
    MockFrameData frames3(frames1);
    MockFrameData frames4(std::move(frames2));
    EXPECT_EQ(frames1.size(), 24u);
    EXPECT_EQ(frames1.size(), frames3.size());
    EXPECT_EQ(frames2.size(), 0u);
    EXPECT_EQ(frames4.size(), 60u);
}

TEST(FrameData, CanIterate)
{
    MockFrameData frames;
    const MockFrameData& cframes = frames;
    auto it = frames.begin();
    auto it_end = frames.end();
    EXPECT_NE(it, it_end);

    auto cframes_it = cframes.begin();
    auto cframes_it_end = cframes.end();
    EXPECT_NE(cframes_it, cframes_it_end);

    auto cit = frames.cbegin();
    auto cit_end = frames.cend();
    EXPECT_NE(cit, cit_end);

    auto rit = frames.rbegin();
    auto rit_end = frames.rend();
    EXPECT_NE(rit, rit_end);

    auto cframes_rit = cframes.rbegin();
    auto cframes_rit_end = cframes.rend();
    EXPECT_NE(cframes_rit, cframes_rit_end);

    auto crit = frames.crbegin();
    auto crit_end = frames.crend();
    EXPECT_NE(crit, crit_end);

    for(auto& frame : frames);
}

TEST(FrameData, CopyMoveAssignment)
{
    MockFrameData frames1;
    MockFrameData frames2(60);
    MockFrameData frames3;
    MockFrameData frames4(60);
    EXPECT_THROW(frames1 = frames2, aex::FrameDataError);
    EXPECT_THROW(frames2 = frames1, aex::FrameDataError);
}

TEST(FrameData, CanAccessElement)
{
    MockFrameData frames1;
    const MockFrameData& frames2 = frames1;
    auto size  = frames1.size();
    for(int ii = 0; ii < size; ii++)
    {
        EXPECT_EQ(&frames1[ii], &frames1.at(ii));
        EXPECT_EQ(&frames2[ii], &frames2.at(ii));
    }
    EXPECT_THROW(frames1.at(size), aex::FrameDataError);
    EXPECT_THROW(frames2.at(size), aex::FrameDataError);
}

TEST(AnimatedFrameData, CanConstructAssign)
{
    MockFrameData frames1;
    MockFrameData frames2(60);
    MockFrameData frames3(60);
    EXPECT_EQ(frames1.size(), 24);
    EXPECT_EQ(frames2.size(), 60);
    EXPECT_EQ(frames3.size(), 60);
    MockAnimatedFrameData animatedFrames1(frames1);
    MockAnimatedFrameData animatedFrames2(std::move(frames2));
    EXPECT_EQ(frames1.size(), 24);
    EXPECT_EQ(frames2.size(), 0);
    EXPECT_EQ(frames3.size(), 60);
    animatedFrames1 = frames1;
    animatedFrames2 = std::move(frames3);
    EXPECT_EQ(frames1.size(), 24);
    EXPECT_EQ(frames2.size(), 0);
    EXPECT_EQ(frames3.size(), 0);
}

TEST(AnimatedFrameData, setFrameState)
{
    MockFrameData frames;
    int i = 0;
    for(auto& f : frames)
    {
        f = i;
        i++;
    }
    MockAnimatedFrameData animatedFrames(frames);
    using frame_index_type = typename state_type::frame_index_type;
    state_type state(0.3, frame_index_type(5), frame_index_type(6));
    animatedFrames.setFrameState(state);
    EXPECT_EQ(animatedFrames.getFirstFrame(), 5);
    EXPECT_EQ(animatedFrames.getNextFrame(), 6);
    EXPECT_EQ(animatedFrames.getFactor(), 0.3);
}

class MockFrameDataDraw;
using FrameDrawWrapper = aex::FrameDrawWrapper<MockAnimatedFrameData, MockFrameDataDraw>;

class MockFrameDrawImpl
{
    public:
        MOCK_METHOD0(defaultCtor, void());
        MOCK_METHOD1(copyCtor, void(const MockFrameDataDraw&));
        MOCK_METHOD1(moveCtor, void(const MockFrameDataDraw&));
        MOCK_METHOD1(copy, void(const MockFrameDataDraw&));
        MOCK_METHOD1(move, void(const MockFrameDataDraw&));
        MOCK_METHOD4(draw, void(AexRenderer*, int, int, float));
        MOCK_METHOD4(finishdraw, void(AexRenderer*, int, int, float));
        MOCK_METHOD0(needsUpdate, void());

    private:
        /* data */
};

class MockFrameDataDraw
{
    public:
        explicit MockFrameDataDraw()
        {
            _instance->defaultCtor();
        }
        MockFrameDataDraw(const MockFrameDataDraw& rhs)
        {
            _instance->copyCtor(rhs);
        }
        MockFrameDataDraw(MockFrameDataDraw&& rhs)
        {
            _instance->moveCtor(rhs);
        }
        MockFrameDataDraw& operator=(const MockFrameDataDraw& rhs)
        {
            _instance->copy(rhs);
            return *this;
        }
        MockFrameDataDraw& operator=(MockFrameDataDraw&& rhs)
        {
            _instance->move(rhs);
            return *this;
        }
        void draw(AexRenderer* a, int b, int c, float d)
        {
            _instance->draw(a, b, c, d);
        }
        void finishdraw(AexRenderer* a, int b, int c, float d)
        {
            _instance->finishdraw(a, b, c, d);
        }
        void needsUpdate()
        {
            _instance->needsUpdate();
        }
        static MockFrameDrawImpl* _instance;
};

MockFrameDrawImpl*
MockFrameDataDraw::_instance = nullptr;

struct MockDrawGuard
{
    MockDrawGuard()
    {
        if(MockFrameDataDraw::_instance == nullptr)
            MockFrameDataDraw::_instance = &m_impl;
    }
    ~MockDrawGuard()
    {
        if(MockFrameDataDraw::_instance == &m_impl)
            MockFrameDataDraw::_instance = nullptr;
    }
    MockFrameDrawImpl m_impl;
};


TEST(FrameDrawWrapper, CanConstruct)
{
    MockDrawGuard drawGuard;
    MockFrameDrawImpl& drawImpl = drawGuard.m_impl;

    //Default ctor.
    EXPECT_CALL(drawImpl, defaultCtor())
        .Times(3);
    FrameDrawWrapper wrapper1;

    //Copy ctor.
    EXPECT_CALL(drawImpl, copy(Ref(wrapper1.getDrawable())))
        .Times(1);
    FrameDrawWrapper wrapper2(wrapper1);

    //Move ctor.
    EXPECT_CALL(drawImpl, move(Ref(wrapper2.getDrawable())))
        .Times(1);
    FrameDrawWrapper wrapper3(std::move(wrapper2));
}

TEST(FrameDrawWrapper, CanDraw)
{
    MockDrawGuard drawGuard;
    MockFrameDrawImpl& drawImpl = drawGuard.m_impl;
    uintptr_t arbitrary_ptr_value = 0x123456;
    void* arbitrary_ptr = reinterpret_cast<void*>(arbitrary_ptr_value);
    AexRenderer* gl = static_cast<AexRenderer*>(arbitrary_ptr);
    MockFrameData frames;
    int i = 0;
    for(auto& f : frames)
    {
        f = i;
        i++;
    }
    MockAnimatedFrameData animatedFrames(frames);
    using frame_index_type = typename state_type::frame_index_type;
    state_type state(0.3, frame_index_type(5), frame_index_type(6));

    animatedFrames.setFrameState(state);

    EXPECT_CALL(drawImpl, draw(gl, 5, 6, 0.3))
        .Times(1);

    FrameDrawWrapper wrapper;
    wrapper = animatedFrames;
    wrapper.draw(gl);
}

TEST(FrameDrawWrapper, CanFinishDraw)
{
    MockDrawGuard drawGuard;
    MockFrameDrawImpl& drawImpl = drawGuard.m_impl;
    uintptr_t arbitrary_ptr_value = 0x123456;
    void* arbitrary_ptr = reinterpret_cast<void*>(arbitrary_ptr_value);
    AexRenderer* gl = static_cast<AexRenderer*>(arbitrary_ptr);
    MockFrameData frames;
    int i = 0;
    for(auto& f : frames)
    {
        f = i;
        i++;
    }
    MockAnimatedFrameData animatedFrames(frames);
    using frame_index_type = typename state_type::frame_index_type;
    state_type state(0.3, frame_index_type(5), frame_index_type(6));

    animatedFrames.setFrameState(state);

    EXPECT_CALL(drawImpl, finishdraw(gl, 5, 6, 0.3))
        .Times(1);

    FrameDrawWrapper wrapper;
    wrapper = animatedFrames;
    wrapper.finishdraw(gl);
}

TEST(FrameDrawWrapper, CanNeedsUpdate)
{
    MockDrawGuard drawGuard;
    MockFrameDrawImpl& drawImpl = drawGuard.m_impl;
    EXPECT_CALL(drawImpl, needsUpdate())
        .Times(1);

    FrameDrawWrapper wrapper;
    wrapper.needsUpdate();
}
