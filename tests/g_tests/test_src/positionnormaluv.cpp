#include "MockWindow.hpp"
#include <aex/VertexData.hpp>
#include <aex/Exceptions.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>

typedef aex::VertexData::AttribList VAttribList;
typedef aex::VertexData::AttribType VAttribType;

class PositionNormalUVTesting : public ::testing::Test
{
    protected:
        typedef aex::PositionNormalUV VertexType;
        typedef std::vector<VertexType> VertexArray;

        virtual void SetUp()
        {
            SampleData.resize(10);
            vData = aex::PositionNormalUV::createVertexDataFromPositionNormalUV(SampleData);
        }
        const VAttribList& getAttribs()
        {
            return vData.getAttribs();
        }
        VertexArray SampleData; 
        aex::VertexData vData;
        aex::VertexData vEmptyData;
};

TEST_F(PositionNormalUVTesting, Validity)
{
    // Hide VertexData so that they get destroyed before window.
    MockWindow window;
    aex::VertexData vData;
    aex::VertexData vEmptyData;
    EXPECT_EQ((bool)vEmptyData, false);
    EXPECT_EQ((bool)vData, false);
    aex::AWindow::initWindow(window, 0, 0);
    vData = aex::PositionNormalUV::createVertexDataFromPositionNormalUV(SampleData);
    EXPECT_EQ((bool)vData, true);
    vEmptyData = std::move(vData);
    EXPECT_EQ((bool)vEmptyData, true);
    EXPECT_EQ((bool)vData, false);
    vData = vEmptyData;
    EXPECT_EQ((bool)vEmptyData, true);
    EXPECT_EQ((bool)vData, true);
}

TEST_F(PositionNormalUVTesting, ThreeAttribsSize)
{
    auto& attribs = getAttribs();
    ASSERT_EQ(3, attribs.size());
}

TEST_F(PositionNormalUVTesting, CorrectDefaultNames)
{
    auto& attribs = getAttribs();
    int i = 0;
    for(auto it = attribs.begin(); it != attribs.end(); it++)
    {
        i++;
        if(i == 1)
            ASSERT_EQ("gl_Vertex", it->first);
        if(i == 2)
            ASSERT_EQ("gl_Normal", it->first);
        if(i == 3)
            ASSERT_EQ("gl_TexCoord", it->first);
    }
}
TEST_F(PositionNormalUVTesting, CorrectStride)
{
    auto& attribs = getAttribs();
    std::size_t sizeOfVertex = sizeof(VertexType);
    for(const VAttribType& att : attribs)
    {
        ASSERT_EQ(sizeOfVertex, att.second.stride);
    }
}

TEST_F(PositionNormalUVTesting, CheckElementCount)
{
    auto& attribs = getAttribs();
    int i = 0;
    for(auto it = attribs.begin(); it != attribs.end(); it++)
    {
        i++;
        if(i == 1)
            ASSERT_EQ(3, it->second.count);
        if(i == 2)
            ASSERT_EQ(3, it->second.count);
        if(i == 3)
            ASSERT_EQ(2, it->second.count);
    }
}

TEST_F(PositionNormalUVTesting, SizeOfAttribute)
{
    auto& attribs = getAttribs();
    int i = 0;
    for(auto it = attribs.begin(); it != attribs.end(); it++)
    {
        i++;
        if(i == 1)
            ASSERT_EQ(sizeof(aMath::Vector3), it->second.size);
        if(i == 2)
            ASSERT_EQ(sizeof(aMath::Vector3), it->second.size);
        if(i == 3)
            ASSERT_EQ(sizeof(aMath::Vector2), it->second.size);
    }
}

TEST_F(PositionNormalUVTesting, TypeOfAttribute)
{
    auto& attribs = getAttribs();
    for(auto it = attribs.begin(); it != attribs.end(); it++)
    {
        ASSERT_EQ(aex::api_detail::PrimitiveType::FLOAT, it->second.type);
    }
}
