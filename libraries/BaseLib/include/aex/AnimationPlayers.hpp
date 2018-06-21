/**************************************************************
 *                                                            * 
 *      Filename:   AnimationPlayers.hpp                      * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 paź 01 10:23:18                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/

#ifndef _AEX_ANIMATIONSTRACKERS_HPP
#define _AEX_ANIMATIONSTRACKERS_HPP

#include <aex/ISimulation.hpp>
#include <aex/Common.hpp>
#include <aex/Traits.hpp>
#include <aex/Exceptions.hpp>
#include <list>
#include <chrono>
#include <algorithm>
#include <cassert>

namespace aex
{
    class AEX_PUBLIC_API AnimationPlayerError : public BaseExcept
    {
            static CONSTEXPR const char* ERROR_TAG = "AnimationPlayerError";
        public:
            explicit AnimationPlayerError(const aex::string& errMsg = "");
    };

    template<class AnimationType, class AnimationRunner>
    class BasicAnimationPlayer
    {
        public:
            using animation_type = AnimationType;
            using runner_type = AnimationRunner;
            using animation_list_type = std::list<animation_type*>;
            /*
             * @brief Add animation to list.
             */
            void addAnimation(animation_type* anim);

            /*
             * @brief Remove animation.
             */
            void removeAnimation(animation_type* anim);

            /*
             * @brief Pause all played animations.
             */
            void pause(const runner_type& runner);


            /*
             * @brief Resume all paused animations.
             */
            void resume(const runner_type& runner);

            /*
             * @brief Execute all animations.
             */
            void run(const runner_type& runner);


            virtual ~BasicAnimationPlayer();
        protected:
            void runAnimation(animation_type* anim);
            void stopAnimation(animation_type* anim);
            void pauseAnimation(animation_type* anim);
            void resumeAnimation(animation_type* anim);
            void initNew(const runner_type& runner);

            animation_list_type m_playedAnimations;
            animation_list_type m_newAnimations;
            animation_list_type m_pausedAnimations;
            friend AnimationType;
    };

    template<class SimulationDataTraitsType>
    class SimDataDiff
    {
        public:
            using simulation_data_traits_type = SimulationDataTraitsType;
            using simulation_data_type = typename simulation_data_traits_type::simulation_data_type;
            SimDataDiff();
            void update(const simulation_data_type& newEnd);
            void pause();
            void resume();
            simulation_data_type& getDiff();
            const simulation_data_type& getDiff() const;

        private:
            simulation_data_type m_diff;
            simulation_data_type m_end;
    };

    enum class AnimationPlayerStateENUM {
        PLAYING,
        PAUSED
    };

    template<class SimulationDataTraitsType>
    class AnimationPlayerState
    {
        public:
            using simulation_data_traits_type = SimulationDataTraitsType;
            using simulation_data_type = typename simulation_data_traits_type::simulation_data_type;
            using simulation_type = BaseSimpleSimulation<simulation_data_traits_type>;
            AnimationPlayerState();
            void pause();
            void resume();
            simulation_data_type getDiff();
            void visit(simulation_type& sim);
        protected:
            void update(const simulation_data_type& t);
        private:
            SimDataDiff<simulation_data_traits_type> m_simDiff;
            AnimationPlayerStateENUM m_state;
    };

    template<class AnimationType, class SimulationDataTraitsType>
    struct AnimationTraits;

    template<class AnimationType,
        class SimulationDataTraitsType,
        class AnimationTraitsT = AnimationTraits<AnimationType, SimulationDataTraitsType>
    >
    struct RunnerBase
    {
        using simulation_data_traits_type = SimulationDataTraitsType;
        using simulation_data_type = typename simulation_data_traits_type::simulation_data_type;
        using animation_traits_type = AnimationTraitsT;
        using animation_type = typename animation_traits_type::animation_type;
        bool init(animation_type& animation) const;
        void pause(animation_type& animation) const;
        void resume(animation_type& animation) const;
        simulation_data_type m_diff;
    };

    template<class AnimationType,
        class SimulationDataTraitsType,
        class AnimationTraitsT = AnimationTraits<AnimationType, SimulationDataTraitsType>
    >
    struct FFRunner : public RunnerBase<AnimationType, SimulationDataTraitsType, AnimationTraitsT>
    {
        using Base = RunnerBase<AnimationType, SimulationDataTraitsType, AnimationTraitsT>;
        using simulation_data_traits_type = typename Base::simulation_data_traits_type;
        using simulation_data_type = typename simulation_data_traits_type::simulation_data_type;
        using animation_traits_type = typename Base::animation_traits_type;
        using animation_type = typename animation_traits_type::animation_type;
        void run(animation_type& animation) const;
    };

    /**
     * @brief Animation player is used to play to update animations based on simulation data.
     *
     * @tparam AnimationType Animation type.
     * @tparam SimulationDataType Simulation data type.
     * @tparam SimulationDataTraitsType
     */
    template<class AnimationType,
        class SimulationDataType,
        class SimulationDataTraitsType = SimulationDataTraits<SimulationDataType>,
        class AnimationRunner = FFRunner<AnimationType, SimulationDataTraitsType>
    >
    class AnimationPlayer final : public ISimulatorVisitor<SimulationDataTraitsType>
    {
        public:
            using animation_type = AnimationType;
            using simulation_data_traits_type = SimulationDataTraitsType;
            using runner_type = AnimationRunner;
            using animation_list_type =  BasicAnimationPlayer<animation_type, runner_type>;
            using player_state_type = AnimationPlayerState<simulation_data_traits_type>;
            using simulation_type = BaseSimpleSimulation<simulation_data_traits_type>;

            /*
             * @brief Update with new time from simulation.
             */
            void visit(simulation_type& sim);

            /*
             * @brief Progress animations forward.
             */
            void run();
            
            /*
             * @brief Return played animations list.
             */
            animation_list_type* getAnimationsList();
            const animation_list_type* getAnimationsList() const;

        private:
            animation_list_type m_animationPlayer;
            player_state_type m_state;
            runner_type m_runner;
    };
}

#include "details/AnimationPlayers.ipp"
#endif // _AEX_ANIMATIONSTRACKERS_HPP
