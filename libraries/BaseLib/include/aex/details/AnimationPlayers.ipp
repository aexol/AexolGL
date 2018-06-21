/**************************************************************
 *                                                            * 
 *      Filename:   AnimationPlayers.ipp                      * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 paź 01 10:23:33                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/

#ifndef _AEX_ANIMATIONPLAYERS_IPP
#define _AEX_ANIMATIONPLAYERS_IPP

#include <aex/Common.hpp>
#include <type_traits>

namespace aex
{
    template<class T, class U>
    bool StartAnimation(T player, U animation)
    {
        return animation->startAnimation(player);
    }

    template<class T, class U>
    bool StopAnimation(T player, U animation)
    {
        return animation->stopAnimation(player);
    }

    template<class T, class U>
    void
    BasicAnimationPlayer<T, U>::addAnimation(animation_type* anim)
    {
        if(StartAnimation(this, anim))
        {
            runAnimation(anim);
        }
    }

    template<class T, class U>
    void
    BasicAnimationPlayer<T, U>::removeAnimation(animation_type* anim)
    {
        if(StopAnimation(this, anim))
        {
            stopAnimation(anim);
        }
    }

    namespace _animationplayers_details
    {
        template <typename T, typename U>
        class has_pause {
            private:
                template<class R, void (R::*)(U&) const> struct _has_valid_pause{};
                template<typename C> static std::true_type Test(_has_valid_pause<C, &C::pause>*);
                template<typename> static std::false_type Test(...);

            public:
            static bool const value = decltype(Test<T>(0))::value;
        };

        template <typename T, typename U>
        class has_resume {
            private:
                template<class R, void (R::*)(U&) const> struct _has_valid_resume{};
                template<typename C> static std::true_type Test(_has_valid_resume<C, &C::resume>*);
                template<typename> static std::false_type Test(...);

            public:
                static bool const value = decltype(Test<T>(0))::value;
        };

        template <typename T, typename U>
        class has_init {
            private:
                template<typename C> static std::true_type Test(decltype(&C::init)*);
                template<typename> static std::false_type Test(...);

            public:
                static bool const value = decltype(Test<T>(0))::value;
        };

        template <typename T, typename U>
        class has_valid_init {
            private:
                template<class R, class RR> static void _has_valid_init(bool (RR::*)(U&) const);
                template<typename C> static std::true_type Test(decltype(_has_valid_init<C>(&C::init))*);
                template<typename> static std::false_type Test(...);

            public:
                static bool const value = decltype(Test<T>(0))::value;
        };

        template<class RunnerType, class AnimationType, bool HasPause = has_pause<RunnerType, AnimationType>::value>
        struct exec_runner_pause
        {
            void operator()(RunnerType& runner, AnimationType* animation)
            {
                runner.pause(*animation);
            }
        };

        template<class RunnerType, class AnimationType>
        struct exec_runner_pause<RunnerType, AnimationType, false>
        {
            void operator()(RunnerType& runner, AnimationType* animation)
            {
            }
        };

        template<class RunnerType, class AnimationType, bool HasResume = has_resume<RunnerType, AnimationType>::value>
        struct exec_runner_resume
        {
            void operator()(RunnerType& runner, AnimationType* animation)
            {
                runner.resume(*animation);
            }
        };

        template<class RunnerType, class AnimationType>
        struct exec_runner_resume<RunnerType, AnimationType, false>
        {
            void operator()(RunnerType& runner, AnimationType* animation)
            {
            }
        };

        template<class RunnerType, class AnimationType, bool HasInit = has_init<RunnerType, AnimationType>::value>
        struct exec_runner_init
        {
            bool operator()(RunnerType& runner, AnimationType* animation)
            {
                static_assert(has_valid_init<RunnerType, AnimationType>::value, "Invalid member method signature. Expected bool init(animation_type&) const.");
                return runner.init(*animation);
            }
        };

        template<class RunnerType, class AnimationType>
        struct exec_runner_init<RunnerType, AnimationType, false>
        {
            bool operator()(RunnerType& runner, AnimationType* animation)
            {
                return true;
            }
        };
    }

    template<class T, class U>
    void
    BasicAnimationPlayer<T, U>::pause(const runner_type& runner)
    {
        initNew(runner);
        std::for_each(m_playedAnimations.begin(), m_playedAnimations.end(), [&runner](animation_type* anim)
                {
                    _animationplayers_details::exec_runner_pause<const runner_type, animation_type>{}(runner, anim);
                }
            );
        m_pausedAnimations.insert(m_pausedAnimations.end(), m_playedAnimations.begin(), m_playedAnimations.end());
        m_playedAnimations.clear();
    }

    template<class T, class U>
    void
    BasicAnimationPlayer<T, U>::resume(const runner_type& runner)
    {
        std::for_each(m_pausedAnimations.begin(), m_pausedAnimations.end(), [&runner](animation_type* anim)
                {
                    _animationplayers_details::exec_runner_resume<const runner_type, animation_type>{}(runner, anim);
                }
            );
        m_playedAnimations.insert(m_playedAnimations.end(), m_pausedAnimations.begin(), m_pausedAnimations.end());
        m_pausedAnimations.clear();
    }

    template<class T, class U>
    void
    BasicAnimationPlayer<T, U>::run(const runner_type& runner)
    {
        initNew(runner);
        std::for_each(m_playedAnimations.begin(), m_playedAnimations.end(), [&runner](animation_type* anim){runner.run(*anim);});
    }

    template<class T, class U>
    BasicAnimationPlayer<T, U>::~BasicAnimationPlayer()
    {
        auto stopLambda = [this](animation_type* anim){removeAnimation(anim);};
        animation_list_type t;
        t.swap(m_newAnimations);
        std::for_each(std::begin(t), std::end(t), stopLambda);
        t.swap(m_playedAnimations);
        std::for_each(std::begin(t), std::end(t), stopLambda);
        t.swap(m_pausedAnimations);
        std::for_each(std::begin(t), std::end(t), stopLambda);
    }

    template<class T, class U>
    void
    BasicAnimationPlayer<T, U>::runAnimation(animation_type* anim)
    {
        m_newAnimations.push_back(anim);
    }

    template<class T, class U>
    void
    BasicAnimationPlayer<T, U>::stopAnimation(animation_type* anim)
    {
        m_playedAnimations.remove(anim);
        m_pausedAnimations.remove(anim);
        m_newAnimations.remove(anim);
    }

    template<class T, class U>
    void
    BasicAnimationPlayer<T, U>::pauseAnimation(animation_type* anim)
    {
        m_playedAnimations.remove(anim);
        m_newAnimations.remove(anim);
        m_pausedAnimations.push_back(anim);
    }

    template<class T, class U>
    void
    BasicAnimationPlayer<T, U>::resumeAnimation(animation_type* anim)
    {
        m_pausedAnimations.remove(anim);
        runAnimation(anim);
    }

    template<class T, class U>
    void
    BasicAnimationPlayer<T, U>::initNew(const runner_type& runner)
    {
        while(!m_newAnimations.empty())
        {
            auto anim = m_newAnimations.front();
            if(_animationplayers_details::exec_runner_init<const runner_type, animation_type>{}(runner, anim))
            {
                m_newAnimations.pop_front();
                m_playedAnimations.push_back(anim);
            } else removeAnimation(anim);
        }
    }

    template<class SimulationDataTraitsType>
    SimDataDiff<SimulationDataTraitsType>::SimDataDiff() :
    m_diff(simulation_data_traits_type::min()),
    m_end(simulation_data_traits_type::max())
    {
    }

    template<class SimulationDataTraitsType>
    void
    SimDataDiff<SimulationDataTraitsType>::update(const simulation_data_type& newEnd)
    {
        auto max = simulation_data_traits_type::max();
        auto min = simulation_data_traits_type::min();
        simulation_data_type clampNewEnd;
        if(newEnd >= max || newEnd < min)
        {
            simulation_data_traits_type::assign(clampNewEnd,
                simulation_data_traits_type::add(
                    simulation_data_traits_type::mod(
                        simulation_data_traits_type::sub(newEnd, min),
                        simulation_data_traits_type::sub(max, min)
                    ),
                    min
                )
            );
        } else {
            simulation_data_traits_type::assign(clampNewEnd, newEnd);
        }
        if(!simulation_data_traits_type::eq(m_end, max))
        {
            if(simulation_data_traits_type::gt(m_end, clampNewEnd))
            {
                simulation_data_traits_type::assign(m_diff, 
                        simulation_data_traits_type::neg(
                                simulation_data_traits_type::sub(m_end, clampNewEnd)
                            )
                        );
            }
            else
            {
                simulation_data_traits_type::assign(m_diff, simulation_data_traits_type::sub(clampNewEnd, m_end));
            }
        }
        simulation_data_traits_type::assign(m_end, clampNewEnd);
    }

    template<class SimulationDataTraitsType>
    void
    SimDataDiff<SimulationDataTraitsType>::pause()
    {
        simulation_data_traits_type::assign(m_end, simulation_data_traits_type::max());
        simulation_data_traits_type::assign(m_diff, simulation_data_traits_type::min());
    }

    template<class SimulationDataTraitsType>
    void
    SimDataDiff<SimulationDataTraitsType>::resume()
    {
    }

    template<class SimulationDataTraitsType>
    typename SimDataDiff<SimulationDataTraitsType>::simulation_data_type&
    SimDataDiff<SimulationDataTraitsType>:: getDiff()
    {
        return m_diff;
    }

    template<class SimulationDataTraitsType>
    const typename SimDataDiff<SimulationDataTraitsType>::simulation_data_type&
    SimDataDiff<SimulationDataTraitsType>:: getDiff() const
    {
        return m_diff;
    }

    template<class SimulationDataTraitsType>
    AnimationPlayerState<SimulationDataTraitsType>::AnimationPlayerState() :
        m_state(AnimationPlayerStateENUM::PLAYING){}

    template<class SimulationDataTraitsType>
    void
    AnimationPlayerState<SimulationDataTraitsType>::pause()
    {
        m_state = AnimationPlayerStateENUM::PAUSED;
        m_simDiff.pause();
    }

    template<class SimulationDataTraitsType>
    void
    AnimationPlayerState<SimulationDataTraitsType>::resume()
    {
        m_state = AnimationPlayerStateENUM::PLAYING;
        m_simDiff.resume();
    }

    template<class SimulationDataTraitsType>
    void
    AnimationPlayerState<SimulationDataTraitsType>::visit(simulation_type& sim)
    {
        update(sim.getData());
    }

    template<class SimulationDataTraitsType>
    void
    AnimationPlayerState<SimulationDataTraitsType>::update(const simulation_data_type& simData)
    {
        if(m_state == AnimationPlayerStateENUM::PLAYING)
            m_simDiff.update(simData);
    }

    template<class SimulationDataTraitsType>
    typename AnimationPlayerState<SimulationDataTraitsType>::simulation_data_type
    AnimationPlayerState<SimulationDataTraitsType>::getDiff()
    {
        return m_simDiff.getDiff();
    }

    namespace _animationplayers_details {
        template <typename T>
        class animation_has_pause {
            private:
                template<class R, void (R::*)() const> struct _has_valid_pause{};
                template<typename C> static std::true_type Test(_has_valid_pause<C, &C::pause>*);
                template<typename> static std::false_type Test(...);

            public:
            static bool const value = decltype(Test<T>(0))::value;
        };

        template <typename T>
        class animation_has_resume {
            private:
                template<class R, void (R::*)() const> struct _has_valid_resume{};
                template<typename C> static std::true_type Test(_has_valid_resume<C, &C::resume>*);
                template<typename> static std::false_type Test(...);

            public:
                static bool const value = decltype(Test<T>(0))::value;
        };

        template <typename T>
        class animation_has_init {
            private:
                template<typename C> static std::true_type Test(decltype(&C::init)*);
                template<typename> static std::false_type Test(...);

            public:
                static bool const value = decltype(Test<T>(0))::value;
        };

        template <typename T>
        class animation_has_valid_init {
            private:
                template<class R, bool (R::*)()> struct _has_valid_init{};
                template<typename C> static std::true_type Test(_has_valid_init<C, &C::init>*);
                template<typename> static std::false_type Test(...);

            public:
                static bool const value = decltype(Test<T>(0))::value;
        };

        template<class AnimationType, bool HasPause = animation_has_pause<AnimationType>::value>
        struct exec_animation_pause
        {
            void operator()(AnimationType& animation)
            {
                animation.pause();
            }
        };

        template<class AnimationType>
        struct exec_animation_pause<AnimationType, false>
        {
            void operator()(AnimationType& animation)
            {
            }
        };

        template<class AnimationType, bool HasResume = animation_has_resume<AnimationType>::value>
        struct exec_animation_resume
        {
            void operator()(AnimationType& animation)
            {
                animation.resume();
            }
        };

        template<class AnimationType>
        struct exec_animation_resume<AnimationType, false>
        {
            void operator()(AnimationType& animation)
            {
            }
        };

        template<class AnimationType, bool HasInit = animation_has_init<AnimationType>::value>
        struct exec_animation_init
        {
            bool operator()(AnimationType& animation)
            {
                static_assert(animation_has_valid_init<AnimationType>::value, "Invalid member method signature. Expected bool init().");
                return animation.init();
            }
        };

        template<class AnimationType>
        struct exec_animation_init<AnimationType, false>
        {
            bool operator()(AnimationType& animation)
            {
                return true;
            }
        };

        template<class T, bool check> struct RetByRef
        {
            typedef T type;
        };

        template<class T> struct RetByRef<T, true>
        {
            typedef T& type;
        };
    }
   template<class AnimationType, class SimulationDataTraitsType>
   struct AnimationTraits
   {
        typedef typename SimulationDataTraitsType::simulation_data_type simulation_data_type;
        typedef AnimationType animation_type;

        static simulation_data_type GetDuration(const animation_type& animation)
        {
            return animation.getDuration();
        }

        static simulation_data_type GetSimulationState(const animation_type& animation)
        {
            return animation.getSimulationState();
        }

        static void Run(animation_type& animation, const simulation_data_type& data)
        {
            animation.run(data);
        }
   };

    template<class AnimationType,
        class SimulationDataTraitsType,
        class AnimationTraitsT
    >
    bool
    RunnerBase<AnimationType, SimulationDataTraitsType, AnimationTraitsT>::init(animation_type& animation) const
    {
        return _animationplayers_details::exec_animation_init<AnimationType>{}(animation);
    }

    template<class AnimationType,
        class SimulationDataTraitsType,
        class AnimationTraitsT
    >
    void
    RunnerBase<AnimationType, SimulationDataTraitsType, AnimationTraitsT>::pause(animation_type& animation) const
    {
        _animationplayers_details::exec_animation_pause<AnimationType>{}(animation);
    }

    template<class AnimationType,
        class SimulationDataTraitsType,
        class AnimationTraitsT
    >
    void
    RunnerBase<AnimationType, SimulationDataTraitsType, AnimationTraitsT>::resume(animation_type& animation) const
    {
        _animationplayers_details::exec_animation_resume<AnimationType>{}(animation);
    }

    template<class AnimationType,
        class SimulationDataTraitsType,
        class AnimationTraitsT
    >
    void
    FFRunner<AnimationType, SimulationDataTraitsType, AnimationTraitsT>::run(animation_type& animation) const
    {
        const simulation_data_type& newState = simulation_data_traits_type::forward(
                animation_traits_type::GetSimulationState(animation),
                this->m_diff,
                animation_traits_type::GetDuration(animation)
                );
        animation_traits_type::Run(animation, newState);
    }

    template<class AnimationType, class SimulationDataType, class SimulationDataTraits, class AnimationRunner>
    void
    AnimationPlayer<AnimationType, SimulationDataType, SimulationDataTraits, AnimationRunner>::visit(simulation_type& sim)
    {
        m_state.visit(sim);
    }

    template<class AnimationType, class SimulationDataType, class SimulationDataTraits, class AnimationRunner>
    void
    AnimationPlayer<AnimationType, SimulationDataType, SimulationDataTraits, AnimationRunner>::run()
    {
        m_runner.m_diff = m_state.getDiff();
        m_animationPlayer.run(m_runner);
    }

    template<class AnimationType, class SimulationDataType, class SimulationDataTraits, class AnimationRunner>
    typename AnimationPlayer<AnimationType, SimulationDataType, SimulationDataTraits, AnimationRunner>::animation_list_type*
    AnimationPlayer<AnimationType, SimulationDataType, SimulationDataTraits, AnimationRunner>::getAnimationsList()
    {
        return &m_animationPlayer;
    }

    template<class AnimationType, class SimulationDataType, class SimulationDataTraits, class AnimationRunner>
    const typename AnimationPlayer<AnimationType, SimulationDataType, SimulationDataTraits, AnimationRunner>::animation_list_type*
    AnimationPlayer<AnimationType, SimulationDataType, SimulationDataTraits, AnimationRunner>::getAnimationsList() const
    {
        return &m_animationPlayer;
    }
}

#endif // _AEX_ANIMATIONPLAYERS_IPP
