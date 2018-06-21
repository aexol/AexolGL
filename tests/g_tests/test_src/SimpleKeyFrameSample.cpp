/**************************************************************
 *                                                            * 
 *      Filename:   SimpleKeyFrameSample.cpp                  * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 paź 06 11:18:25                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/

#include "SimpleKeyFrameSample.hpp"

aex::mesh_indices_type indicesSample1[] = {
    0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1
};

aMath::Vector2 uvsSample1[] = {
    aMath::Vector2(0.0f, 1.0f),
    aMath::Vector2(2.0f, 3.0f),
    aMath::Vector2(4.0f, 5.0f),
    aMath::Vector2(6.0f, 7.0f),
    aMath::Vector2(8.0f, 9.0f),
    aMath::Vector2(10.0f, 11.0f),
    aMath::Vector2(12.0f, 13.0f),
    aMath::Vector2(14.0f, 15.0f)
};
aex::PositionNormal frameSample1FrameNr1[]
{
    aex::PositionNormal{aMath::Vector3(0.0f, 0.0f, 0.0f), aMath::Vector3(0.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(0.0f, 0.0f, 1.0f), aMath::Vector3(0.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(0.0f, 0.0f, 2.0f), aMath::Vector3(0.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(0.0f, 0.0f, 3.0f), aMath::Vector3(0.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(0.0f, 0.0f, 4.0f), aMath::Vector3(0.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(0.0f, 0.0f, 5.0f), aMath::Vector3(0.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(0.0f, 0.0f, 6.0f), aMath::Vector3(0.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(0.0f, 0.0f, 7.0f), aMath::Vector3(0.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr2[]
{
    aex::PositionNormal{aMath::Vector3(1.0f, 0.0f, 0.0f), aMath::Vector3(1.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(1.0f, 0.0f, 1.0f), aMath::Vector3(1.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(1.0f, 0.0f, 2.0f), aMath::Vector3(1.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(1.0f, 0.0f, 3.0f), aMath::Vector3(1.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(1.0f, 0.0f, 4.0f), aMath::Vector3(1.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(1.0f, 0.0f, 5.0f), aMath::Vector3(1.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(1.0f, 0.0f, 6.0f), aMath::Vector3(1.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(1.0f, 0.0f, 7.0f), aMath::Vector3(1.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr3[]
{
    aex::PositionNormal{aMath::Vector3(2.0f, 0.0f, 0.0f), aMath::Vector3(2.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(2.0f, 0.0f, 1.0f), aMath::Vector3(2.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(2.0f, 0.0f, 2.0f), aMath::Vector3(2.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(2.0f, 0.0f, 3.0f), aMath::Vector3(2.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(2.0f, 0.0f, 4.0f), aMath::Vector3(2.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(2.0f, 0.0f, 5.0f), aMath::Vector3(2.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(2.0f, 0.0f, 6.0f), aMath::Vector3(2.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(2.0f, 0.0f, 7.0f), aMath::Vector3(2.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr4[]
{
    aex::PositionNormal{aMath::Vector3(3.0f, 0.0f, 0.0f), aMath::Vector3(3.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(3.0f, 0.0f, 1.0f), aMath::Vector3(3.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(3.0f, 0.0f, 2.0f), aMath::Vector3(3.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(3.0f, 0.0f, 3.0f), aMath::Vector3(3.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(3.0f, 0.0f, 4.0f), aMath::Vector3(3.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(3.0f, 0.0f, 5.0f), aMath::Vector3(3.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(3.0f, 0.0f, 6.0f), aMath::Vector3(3.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(3.0f, 0.0f, 7.0f), aMath::Vector3(3.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr5[]
{
    aex::PositionNormal{aMath::Vector3(4.0f, 0.0f, 0.0f), aMath::Vector3(4.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(4.0f, 0.0f, 1.0f), aMath::Vector3(4.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(4.0f, 0.0f, 2.0f), aMath::Vector3(4.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(4.0f, 0.0f, 3.0f), aMath::Vector3(4.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(4.0f, 0.0f, 4.0f), aMath::Vector3(4.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(4.0f, 0.0f, 5.0f), aMath::Vector3(4.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(4.0f, 0.0f, 6.0f), aMath::Vector3(4.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(4.0f, 0.0f, 7.0f), aMath::Vector3(4.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr6[]
{
    aex::PositionNormal{aMath::Vector3(5.0f, 0.0f, 0.0f), aMath::Vector3(5.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(5.0f, 0.0f, 1.0f), aMath::Vector3(5.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(5.0f, 0.0f, 2.0f), aMath::Vector3(5.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(5.0f, 0.0f, 3.0f), aMath::Vector3(5.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(5.0f, 0.0f, 4.0f), aMath::Vector3(5.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(5.0f, 0.0f, 5.0f), aMath::Vector3(5.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(5.0f, 0.0f, 6.0f), aMath::Vector3(5.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(5.0f, 0.0f, 7.0f), aMath::Vector3(5.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr7[]
{
    aex::PositionNormal{aMath::Vector3(6.0f, 0.0f, 0.0f), aMath::Vector3(6.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(6.0f, 0.0f, 1.0f), aMath::Vector3(6.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(6.0f, 0.0f, 2.0f), aMath::Vector3(6.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(6.0f, 0.0f, 3.0f), aMath::Vector3(6.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(6.0f, 0.0f, 4.0f), aMath::Vector3(6.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(6.0f, 0.0f, 5.0f), aMath::Vector3(6.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(6.0f, 0.0f, 6.0f), aMath::Vector3(6.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(6.0f, 0.0f, 7.0f), aMath::Vector3(6.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr8[]
{
    aex::PositionNormal{aMath::Vector3(7.0f, 0.0f, 0.0f), aMath::Vector3(7.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(7.0f, 0.0f, 1.0f), aMath::Vector3(7.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(7.0f, 0.0f, 2.0f), aMath::Vector3(7.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(7.0f, 0.0f, 3.0f), aMath::Vector3(7.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(7.0f, 0.0f, 4.0f), aMath::Vector3(7.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(7.0f, 0.0f, 5.0f), aMath::Vector3(7.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(7.0f, 0.0f, 6.0f), aMath::Vector3(7.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(7.0f, 0.0f, 7.0f), aMath::Vector3(7.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr9[]
{
    aex::PositionNormal{aMath::Vector3(8.0f, 0.0f, 0.0f), aMath::Vector3(8.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(8.0f, 0.0f, 1.0f), aMath::Vector3(8.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(8.0f, 0.0f, 2.0f), aMath::Vector3(8.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(8.0f, 0.0f, 3.0f), aMath::Vector3(8.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(8.0f, 0.0f, 4.0f), aMath::Vector3(8.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(8.0f, 0.0f, 5.0f), aMath::Vector3(8.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(8.0f, 0.0f, 6.0f), aMath::Vector3(8.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(8.0f, 0.0f, 7.0f), aMath::Vector3(8.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr10[]
{
    aex::PositionNormal{aMath::Vector3(9.0f, 0.0f, 0.0f), aMath::Vector3(9.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(9.0f, 0.0f, 1.0f), aMath::Vector3(9.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(9.0f, 0.0f, 2.0f), aMath::Vector3(9.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(9.0f, 0.0f, 3.0f), aMath::Vector3(9.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(9.0f, 0.0f, 4.0f), aMath::Vector3(9.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(9.0f, 0.0f, 5.0f), aMath::Vector3(9.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(9.0f, 0.0f, 6.0f), aMath::Vector3(9.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(9.0f, 0.0f, 7.0f), aMath::Vector3(9.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr11[]
{
    aex::PositionNormal{aMath::Vector3(10.0f, 0.0f, 0.0f), aMath::Vector3(10.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(10.0f, 0.0f, 1.0f), aMath::Vector3(10.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(10.0f, 0.0f, 2.0f), aMath::Vector3(10.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(10.0f, 0.0f, 3.0f), aMath::Vector3(10.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(10.0f, 0.0f, 4.0f), aMath::Vector3(10.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(10.0f, 0.0f, 5.0f), aMath::Vector3(10.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(10.0f, 0.0f, 6.0f), aMath::Vector3(10.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(10.0f, 0.0f, 7.0f), aMath::Vector3(10.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr12[]
{
    aex::PositionNormal{aMath::Vector3(11.0f, 0.0f, 0.0f), aMath::Vector3(11.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(11.0f, 0.0f, 1.0f), aMath::Vector3(11.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(11.0f, 0.0f, 2.0f), aMath::Vector3(11.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(11.0f, 0.0f, 3.0f), aMath::Vector3(11.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(11.0f, 0.0f, 4.0f), aMath::Vector3(11.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(11.0f, 0.0f, 5.0f), aMath::Vector3(11.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(11.0f, 0.0f, 6.0f), aMath::Vector3(11.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(11.0f, 0.0f, 7.0f), aMath::Vector3(11.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr13[]
{
    aex::PositionNormal{aMath::Vector3(12.0f, 0.0f, 0.0f), aMath::Vector3(12.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(12.0f, 0.0f, 1.0f), aMath::Vector3(12.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(12.0f, 0.0f, 2.0f), aMath::Vector3(12.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(12.0f, 0.0f, 3.0f), aMath::Vector3(12.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(12.0f, 0.0f, 4.0f), aMath::Vector3(12.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(12.0f, 0.0f, 5.0f), aMath::Vector3(12.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(12.0f, 0.0f, 6.0f), aMath::Vector3(12.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(12.0f, 0.0f, 7.0f), aMath::Vector3(12.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr14[]
{
    aex::PositionNormal{aMath::Vector3(13.0f, 0.0f, 0.0f), aMath::Vector3(13.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(13.0f, 0.0f, 1.0f), aMath::Vector3(13.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(13.0f, 0.0f, 2.0f), aMath::Vector3(13.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(13.0f, 0.0f, 3.0f), aMath::Vector3(13.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(13.0f, 0.0f, 4.0f), aMath::Vector3(13.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(13.0f, 0.0f, 5.0f), aMath::Vector3(13.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(13.0f, 0.0f, 6.0f), aMath::Vector3(13.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(13.0f, 0.0f, 7.0f), aMath::Vector3(13.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr15[]
{
    aex::PositionNormal{aMath::Vector3(14.0f, 0.0f, 0.0f), aMath::Vector3(14.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(14.0f, 0.0f, 1.0f), aMath::Vector3(14.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(14.0f, 0.0f, 2.0f), aMath::Vector3(14.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(14.0f, 0.0f, 3.0f), aMath::Vector3(14.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(14.0f, 0.0f, 4.0f), aMath::Vector3(14.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(14.0f, 0.0f, 5.0f), aMath::Vector3(14.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(14.0f, 0.0f, 6.0f), aMath::Vector3(14.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(14.0f, 0.0f, 7.0f), aMath::Vector3(14.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr16[]
{
    aex::PositionNormal{aMath::Vector3(15.0f, 0.0f, 0.0f), aMath::Vector3(15.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(15.0f, 0.0f, 1.0f), aMath::Vector3(15.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(15.0f, 0.0f, 2.0f), aMath::Vector3(15.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(15.0f, 0.0f, 3.0f), aMath::Vector3(15.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(15.0f, 0.0f, 4.0f), aMath::Vector3(15.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(15.0f, 0.0f, 5.0f), aMath::Vector3(15.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(15.0f, 0.0f, 6.0f), aMath::Vector3(15.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(15.0f, 0.0f, 7.0f), aMath::Vector3(15.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr17[]
{
    aex::PositionNormal{aMath::Vector3(16.0f, 0.0f, 0.0f), aMath::Vector3(16.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(16.0f, 0.0f, 1.0f), aMath::Vector3(16.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(16.0f, 0.0f, 2.0f), aMath::Vector3(16.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(16.0f, 0.0f, 3.0f), aMath::Vector3(16.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(16.0f, 0.0f, 4.0f), aMath::Vector3(16.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(16.0f, 0.0f, 5.0f), aMath::Vector3(16.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(16.0f, 0.0f, 6.0f), aMath::Vector3(16.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(16.0f, 0.0f, 7.0f), aMath::Vector3(16.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr18[]
{
    aex::PositionNormal{aMath::Vector3(17.0f, 0.0f, 0.0f), aMath::Vector3(17.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(17.0f, 0.0f, 1.0f), aMath::Vector3(17.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(17.0f, 0.0f, 2.0f), aMath::Vector3(17.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(17.0f, 0.0f, 3.0f), aMath::Vector3(17.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(17.0f, 0.0f, 4.0f), aMath::Vector3(17.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(17.0f, 0.0f, 5.0f), aMath::Vector3(17.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(17.0f, 0.0f, 6.0f), aMath::Vector3(17.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(17.0f, 0.0f, 7.0f), aMath::Vector3(17.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr19[]
{
    aex::PositionNormal{aMath::Vector3(18.0f, 0.0f, 0.0f), aMath::Vector3(18.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(18.0f, 0.0f, 1.0f), aMath::Vector3(18.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(18.0f, 0.0f, 2.0f), aMath::Vector3(18.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(18.0f, 0.0f, 3.0f), aMath::Vector3(18.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(18.0f, 0.0f, 4.0f), aMath::Vector3(18.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(18.0f, 0.0f, 5.0f), aMath::Vector3(18.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(18.0f, 0.0f, 6.0f), aMath::Vector3(18.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(18.0f, 0.0f, 7.0f), aMath::Vector3(18.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr20[]
{
    aex::PositionNormal{aMath::Vector3(19.0f, 0.0f, 0.0f), aMath::Vector3(19.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(19.0f, 0.0f, 1.0f), aMath::Vector3(19.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(19.0f, 0.0f, 2.0f), aMath::Vector3(19.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(19.0f, 0.0f, 3.0f), aMath::Vector3(19.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(19.0f, 0.0f, 4.0f), aMath::Vector3(19.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(19.0f, 0.0f, 5.0f), aMath::Vector3(19.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(19.0f, 0.0f, 6.0f), aMath::Vector3(19.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(19.0f, 0.0f, 7.0f), aMath::Vector3(19.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr21[]
{
    aex::PositionNormal{aMath::Vector3(20.0f, 0.0f, 0.0f), aMath::Vector3(20.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(20.0f, 0.0f, 1.0f), aMath::Vector3(20.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(20.0f, 0.0f, 2.0f), aMath::Vector3(20.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(20.0f, 0.0f, 3.0f), aMath::Vector3(20.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(20.0f, 0.0f, 4.0f), aMath::Vector3(20.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(20.0f, 0.0f, 5.0f), aMath::Vector3(20.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(20.0f, 0.0f, 6.0f), aMath::Vector3(20.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(20.0f, 0.0f, 7.0f), aMath::Vector3(20.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr22[]
{
    aex::PositionNormal{aMath::Vector3(21.0f, 0.0f, 0.0f), aMath::Vector3(21.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(21.0f, 0.0f, 1.0f), aMath::Vector3(21.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(21.0f, 0.0f, 2.0f), aMath::Vector3(21.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(21.0f, 0.0f, 3.0f), aMath::Vector3(21.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(21.0f, 0.0f, 4.0f), aMath::Vector3(21.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(21.0f, 0.0f, 5.0f), aMath::Vector3(21.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(21.0f, 0.0f, 6.0f), aMath::Vector3(21.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(21.0f, 0.0f, 7.0f), aMath::Vector3(21.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr23[]
{
    aex::PositionNormal{aMath::Vector3(22.0f, 0.0f, 0.0f), aMath::Vector3(22.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(22.0f, 0.0f, 1.0f), aMath::Vector3(22.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(22.0f, 0.0f, 2.0f), aMath::Vector3(22.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(22.0f, 0.0f, 3.0f), aMath::Vector3(22.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(22.0f, 0.0f, 4.0f), aMath::Vector3(22.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(22.0f, 0.0f, 5.0f), aMath::Vector3(22.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(22.0f, 0.0f, 6.0f), aMath::Vector3(22.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(22.0f, 0.0f, 7.0f), aMath::Vector3(22.0f, 0.0f, 7.0f)},
};
aex::PositionNormal frameSample1FrameNr24[]
{
    aex::PositionNormal{aMath::Vector3(23.0f, 0.0f, 0.0f), aMath::Vector3(23.0f, 0.0f, 0.0f)},
    aex::PositionNormal{aMath::Vector3(23.0f, 0.0f, 1.0f), aMath::Vector3(23.0f, 0.0f, 1.0f)},
    aex::PositionNormal{aMath::Vector3(23.0f, 0.0f, 2.0f), aMath::Vector3(23.0f, 0.0f, 2.0f)},
    aex::PositionNormal{aMath::Vector3(23.0f, 0.0f, 3.0f), aMath::Vector3(23.0f, 0.0f, 3.0f)},
    aex::PositionNormal{aMath::Vector3(23.0f, 0.0f, 4.0f), aMath::Vector3(23.0f, 0.0f, 4.0f)},
    aex::PositionNormal{aMath::Vector3(23.0f, 0.0f, 5.0f), aMath::Vector3(23.0f, 0.0f, 5.0f)},
    aex::PositionNormal{aMath::Vector3(23.0f, 0.0f, 6.0f), aMath::Vector3(23.0f, 0.0f, 6.0f)},
    aex::PositionNormal{aMath::Vector3(23.0f, 0.0f, 7.0f), aMath::Vector3(23.0f, 0.0f, 7.0f)},
};

using frame_array = aex::SimpleKeyFrameData::unique_vertex_data_type;
frame_array framesSample1[] = 
{
    std::vector<aex::PositionNormal>(frameSample1FrameNr1, frameSample1FrameNr1+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr2, frameSample1FrameNr2+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr3, frameSample1FrameNr3+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr4, frameSample1FrameNr4+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr5, frameSample1FrameNr5+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr6, frameSample1FrameNr6+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr7, frameSample1FrameNr7+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr8, frameSample1FrameNr8+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr9, frameSample1FrameNr9+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr10, frameSample1FrameNr10+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr11, frameSample1FrameNr11+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr12, frameSample1FrameNr12+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr13, frameSample1FrameNr13+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr14, frameSample1FrameNr14+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr15, frameSample1FrameNr15+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr16, frameSample1FrameNr16+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr17, frameSample1FrameNr17+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr18, frameSample1FrameNr18+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr19, frameSample1FrameNr19+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr20, frameSample1FrameNr20+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr21, frameSample1FrameNr21+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr22, frameSample1FrameNr22+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr23, frameSample1FrameNr23+8),
    std::vector<aex::PositionNormal>(frameSample1FrameNr24, frameSample1FrameNr24+8)
};

aex::SimpleKeyFrameData sampleData1 = aex::SimpleKeyFrameData{
    std::vector<aex::mesh_indices_type>(indicesSample1, indicesSample1+14),
    std::vector<aMath::Vector2>(uvsSample1, uvsSample1+8),
    std::vector<frame_array>(framesSample1, framesSample1+24)
};
