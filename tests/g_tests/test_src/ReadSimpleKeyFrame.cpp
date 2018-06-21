/**************************************************************
 *                                                            * 
 *      Filename:   ReadSimpleKeyFrameData.cpp                * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 paź 06 12:41:57                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/

#include "SimpleKeyFrameSample.hpp"
#include <aex/KeyFrameData.hpp>
#include "gmock/gmock.h"

TEST(ReadSimpleKeyFrameData, ReadFromAexFile)
{
    auto simpleKeyFrameData = aex::LoadSimpleKFDataFromAexFile("sample1");
    unsigned idx, jdx;

    idx = 0;
    EXPECT_EQ(simpleKeyFrameData.indices.size(), sampleData1.indices.size());
    for(auto& id : simpleKeyFrameData.indices)
    {
        EXPECT_EQ(id, sampleData1.indices[idx]);
        idx++;
    }

    idx = 0;
    EXPECT_EQ(simpleKeyFrameData.uvs.size(), sampleData1.uvs.size());
    for(auto& uv : simpleKeyFrameData.uvs)
    {
        EXPECT_EQ(uv, sampleData1.uvs[idx]);
        idx++;
    }

    idx = 0;
    jdx = 0;
    EXPECT_EQ(simpleKeyFrameData.frames.size(), sampleData1.frames.size());
    for(auto& frame : simpleKeyFrameData.frames)
    {
        EXPECT_EQ(frame.size(), sampleData1.frames[jdx].size());
        idx = 0;
        for(auto& frameVal : frame)
        {
            EXPECT_EQ(frameVal.position, sampleData1.frames[jdx][idx].position);
            EXPECT_EQ(frameVal.normal, sampleData1.frames[jdx][idx].normal);
            idx++;
        }
        jdx++;
    }
}
