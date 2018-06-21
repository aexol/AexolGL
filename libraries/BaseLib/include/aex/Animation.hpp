/**************************************************************
 *                                                            * 
 *      Filename:   Animation.hpp                             * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 wrz 24 12:40:50                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/

#ifndef _AEX_ANIMATION_HPP
#define _AEX_ANIMATION_HPP

#include <aex/Common.hpp>
#include <aex/BasicAnimation.hpp>
#include <aex/AnimationPlayers.hpp>
#include <aex/Simulation.hpp>

namespace aex
{
    using ATSimulationDataTraits = SimulationDataTraits<aexTimeUnit>;
    using ATStaticFrameLayout = StaticFrameLayout<ATSimulationDataTraits>;
    template<class T>
    using ATAnimationPlayer = AnimationPlayer<T, aexTimeUnit>;
    template<template<class> class Player>
    using StaticAnimation = BasicAnimation<DataDelegate, ATStaticFrameLayout, PlayerDelegate<Player>>;
    using FFStaticAnimation = StaticAnimation<ATAnimationPlayer>;
    using FFPlayer = typename FFStaticAnimation::player_type;
    using ATSimulation = typename FFPlayer::simulation_type;
}

#endif // _AEX_ANIMATION_HPP
