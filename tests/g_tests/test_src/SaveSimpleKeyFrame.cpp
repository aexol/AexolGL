/**************************************************************
 *                                                            * 
 *      Filename:   SaveSimpleKeyFrame.cpp                    * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 paź 06 11:16:29                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/

#include "SimpleKeyFrameSample.hpp"
#include <aex/KeyFrameData.hpp>
#include "gmock/gmock.h"

TEST(SaveSimpleKeyFrame, CanSaveToFile)
{
    aex::SaveSimpleKFDataToAexFile("sample1", sampleData1);
}
