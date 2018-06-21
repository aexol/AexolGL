/**************************************************************
 *                                                            * 
 *      Filename:   Traits.hpp                                * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 paź 01 10:23:55                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/

#ifndef _AEX_TRAITS_HPP
#define _AEX_TRAITS_HPP

#include <aex/Exceptions.hpp>
#include <type_traits>
#include <chrono>
#include <cmath>

namespace aex
{
    namespace _traits_detail
    {
        template<class T, bool Primitive = std::is_arithmetic<T>::value>
        struct TimeTraitsDetail
        {
            using time_type = T;

            static CONSTEXPR time_type max()
            {
                return T::max();
            }

            static CONSTEXPR time_type min()
            {
                return T::min();
            }

            static CONSTEXPR time_type zero()
            {
                return T::zero();
            }
        };

        template<class T>
        struct TimeTraitsDetail<T, true>
        {
            using time_type = T;

            static CONSTEXPR time_type max()
            {
                return std::numeric_limits<T>::max();
            }

            static CONSTEXPR time_type min()
            {
                return std::numeric_limits<T>::lowest();
            }

            static CONSTEXPR time_type zero()
            {
                return T{};
            }
        };
    }
    template<class T>
    struct TimeTraits
    {
        using traits_detail_type = _traits_detail::TimeTraitsDetail<T>;
        using time_type = typename traits_detail_type::time_type;

        static CONSTEXPR time_type max()
        {
            return traits_detail_type::max();
        }

        static CONSTEXPR time_type min()
        {
            return traits_detail_type::min();
        }

        static CONSTEXPR time_type zero()
        {
            return traits_detail_type::zero();
        }

        static CONSTEXPR time_type unit()
        {
            return (zero())++;
        }

        static time_type& assign(time_type& lhs, const time_type& rhs)
        {
            lhs = rhs;
            return lhs;
        }

        static CONSTEXPR bool lt(const time_type& lhs, const time_type& rhs)
        {
            return lhs < rhs;
        }

        static CONSTEXPR bool eq(const time_type& lhs, const time_type& rhs)
        {
            return lhs == rhs;
        }

        static CONSTEXPR bool gt(const time_type& lhs, const time_type& rhs)
        {
            return lhs > rhs;
        }

        static CONSTEXPR time_type add(const time_type& lhs, const time_type& rhs)
        {
            return lhs + rhs;
        }

        static time_type& iadd(time_type& lhs, const time_type& rhs)
        {
            return assign(lhs, add(lhs, rhs));
        }

        static CONSTEXPR time_type sub(const time_type& lhs, const time_type& rhs)
        {
            return lhs - rhs;
        }

        static time_type& isub(time_type& lhs, const time_type& rhs)
        {
            return assign(lhs, sub(lhs, rhs));
        }

        static CONSTEXPR time_type mul(const time_type& lhs, const time_type& rhs)
        {
            return lhs * rhs;
        }

        static time_type& imul(time_type& lhs, const time_type& rhs)
        {
            return assign(lhs, mul(lhs, rhs));
        }

        static CONSTEXPR time_type div(const time_type& lhs, const time_type& rhs)
        {
            return !eq(rhs, zero()) ? lhs / rhs : throw MathError("Division by zero");
        }

        template<class U = T, typename std::enable_if<!std::is_floating_point<U>::value>::type* = nullptr>
        static CONSTEXPR time_type mod(const time_type& lhs, const time_type& rhs)
        {
            return !eq(rhs, zero()) ? lhs % rhs : throw MathError("Division by zero.");
        }

        template<class U = T, typename std::enable_if<std::is_floating_point<U>::value>::type* = nullptr>
        static CONSTEXPR time_type mod(const time_type& lhs, const time_type& rhs)
        {
            //Round up lhs in modulo as it's used most commonly in
            //range checking.
            return !eq(rhs, zero()) ? std::fmod(lhs, rhs) : throw MathError("Division by zero.");
        }

        static time_type& imod(time_type& lhs, const time_type& rhs)
        {
            return assign(lhs, mod(lhs, rhs));
        }

        static time_type& inc(time_type& lhs)
        {
            return iadd(lhs, unit());
        }
    };

    namespace _traits_detail
    {
        template<class T>
        struct is_chrono_duration
        {
            static CONSTEXPR bool value = false;
        };

        template<class T, class U>
        struct is_chrono_duration<std::chrono::duration<T, U>>
        {
            static CONSTEXPR bool value = true;
        };

        template<class T>
        struct is_user_simulation_type
        {
            static CONSTEXPR bool value = !(std::is_arithmetic<T>::value || is_chrono_duration<T>::value);
        };

        template<
            class SimulationDataType,
            bool Primitive = std::is_arithmetic<SimulationDataType>::value
        >
        struct get_time_type
        {
            using time_type = typename SimulationDataType::time_type;
        };

        template<class T>
        struct get_time_type<T, true>
        {
            using time_type = T;
        };

        template<class T, class U>
        struct get_time_type<std::chrono::duration<T, U>, false>
        {
            using time_type = typename std::chrono::duration<T, U>::rep;
        };
    }

    /*
     * Forward declaration for SimulationDataTraits.
     */

    namespace _traits_detail
    {
        template<class SimulationDataType, typename = void>
        struct SimulationDataTraitsBase;
        
        /*
         * @brief Primitive simulation data traits are assumed to be simple time value.
         */

        template<class SimulationDataType>
        struct SimulationDataTraitsBase<
            SimulationDataType, 
            typename std::enable_if<
                std::is_arithmetic<SimulationDataType>::value ||
                _traits_detail::is_chrono_duration<SimulationDataType>::value
            >::type
        > : public TimeTraits<SimulationDataType>
        {
            using time_traits_type = TimeTraits<SimulationDataType>;
            using simulation_data_type = typename time_traits_type::time_type;
            static CONSTEXPR simulation_data_type max()
            {
                return time_traits_type::max();
            }
            static CONSTEXPR simulation_data_type min()
            {
                return time_traits_type::min();
            }
            static CONSTEXPR simulation_data_type zero()
            {
                return time_traits_type::zero();
            }
            static simulation_data_type unit()
            {
                auto z = zero();
                return z++;
            }
        };

        template<class SimulationDataType>
        struct SimulationDataTraitsBase<
            SimulationDataType, 
            typename std::enable_if<
                !(std::is_arithmetic<SimulationDataType>::value ||
                _traits_detail::is_chrono_duration<SimulationDataType>::value)
            >::type
        >
        {
            using simulation_data_type = SimulationDataType;
            static CONSTEXPR simulation_data_type max()
            {
                return simulation_data_type::max();
            }

            static CONSTEXPR simulation_data_type min()
            {
                return simulation_data_type::min();
            }

            static CONSTEXPR simulation_data_type zero()
            {
                return simulation_data_type::zero();
            }

            static simulation_data_type unit()
            {
                auto _zero = zero();
                return _zero++;
            }

            template<class U>
            static simulation_data_type& assign(simulation_data_type& lhs, const U& rhs)
            {
                lhs = rhs;
                return lhs;
            }

            template<class U>
            static CONSTEXPR  simulation_data_type 
            add(const simulation_data_type& lhs, const U& rhs)
            {
                return lhs + rhs;
            }

            template<class U>
            static simulation_data_type&
            iadd(simulation_data_type& lhs, const U& rhs)
            {
                return assign(lhs, add(lhs, rhs));
            }

            template<class U>
            static CONSTEXPR simulation_data_type 
            sub(const simulation_data_type& lhs, const U& rhs)
            {
                return lhs - rhs;
            }

            template<class U>
            static simulation_data_type&
            isub(simulation_data_type& lhs, const U& rhs)
            {
                return assign(lhs, sub(lhs, rhs));
            }

            template<class U>
            static CONSTEXPR simulation_data_type 
            mul(const simulation_data_type& lhs, const U& rhs)
            {
                return lhs * rhs;
            }

            template<class U>
            static simulation_data_type&
            imul(simulation_data_type& lhs, const U& rhs)
            {
                return assign(lhs, mul(lhs, rhs));
            }

            template<class U>
            static CONSTEXPR simulation_data_type 
            div(const simulation_data_type& lhs, const U& rhs)
            {
                return !eq(rhs, zero()) ? lhs / rhs : throw MathError("Division by zero.");
            }

            template<class U>
            static simulation_data_type&
            idiv(simulation_data_type& lhs, const U& rhs)
            {
                return assign(lhs, div(lhs, rhs));
            }

            template<class U>
            static CONSTEXPR simulation_data_type 
            mod(const simulation_data_type& lhs, const U& rhs)
            {
                return !eq(rhs, zero()) ? lhs % rhs : throw MathError("Division by zero.");
            }

            template<class U>
            static simulation_data_type&
            imod(simulation_data_type& lhs, const U& rhs)
            {
                return assign(lhs, mod(lhs, rhs));
            }

            template<class U>
            static simulation_data_type&
            inc(simulation_data_type& lhs)
            {
                return iadd(lhs, unit());
            }

            template<class U>
            static CONSTEXPR bool 
            lt(const simulation_data_type& lhs, const U& rhs)
            {
                return lhs < rhs;
            }

            template<class U>
            static CONSTEXPR bool 
            eq(const simulation_data_type& lhs, const U& rhs)
            {
                return lhs == rhs;
            }

            template<class U>
            static CONSTEXPR bool 
            gt(const simulation_data_type& lhs, const U& rhs)
            {
                return lhs > rhs;
            }
        };
    }

    namespace _traits_detail
    {
        template <typename T>
        class has_forward {
            private:
                template<typename C> static auto Test(void*)
                -> decltype(C{std::declval<C>().forward(std::declval<C const>(),
                            std::declval<C const>())}, std::true_type{});

                template<typename> static std::false_type Test(...);

            public:
            static bool const value = decltype(Test<T>(0))::value;
        };

        template <typename T>
        class has_rewind {
            private:
                template<typename C> static auto Test(void*)
                -> decltype(C{std::declval<C>().rewind(std::declval<C const>(),
                            std::declval<C const>())}, std::true_type{});

                template<typename> static std::false_type Test(...);

            public:
            static bool const value = decltype(Test<T>(0))::value;
        };
    }

    template<class SimulationDataType>
    struct SimulationDataTraits : public _traits_detail::SimulationDataTraitsBase<SimulationDataType>
    {
        using Base = _traits_detail::SimulationDataTraitsBase<SimulationDataType>;
        using simulation_data_type = SimulationDataType;

        static simulation_data_type neg(const simulation_data_type& rhs)
        {
            return Base::sub(Base::zero(), rhs);
        }

        template<class U = SimulationDataType,
            typename std::enable_if<
                !_traits_detail::has_forward<U>::value
            >::type* = nullptr
        >
        static simulation_data_type& forward(simulation_data_type& lhs, const simulation_data_type& rhs, const simulation_data_type& range)
        {
            Base::assign(lhs, Base::mod(Base::add(lhs, rhs), range));
            if(lhs < Base::zero())
                Base::iadd(lhs, range);
            return lhs;
        }

        template<class U = SimulationDataType,
            typename std::enable_if<
                _traits_detail::has_forward<U>::value
            >::type* = nullptr
        >
        static simulation_data_type& forward(simulation_data_type& lhs, const simulation_data_type& rhs, const simulation_data_type& range)
        {
            return lhs.forward(rhs, range);
        }

        template<class U = SimulationDataType,
            typename std::enable_if<
                !_traits_detail::has_rewind<U>::value
            >::type* = nullptr
        >
        static simulation_data_type& rewind(simulation_data_type& lhs, const simulation_data_type& rhs, const simulation_data_type& range)
        {
            Base::assign(lhs, Base::mod(Base::sub(lhs, rhs), range));
            return lhs;
        }

        template<class U = SimulationDataType,
            typename std::enable_if<
                _traits_detail::has_rewind<U>::value
            >::type* = nullptr
        >
        static simulation_data_type& rewind(simulation_data_type& lhs, const simulation_data_type& rhs, const simulation_data_type& range)
        {
            return lhs.rewind(rhs, range);
        }

        static simulation_data_type forward(const simulation_data_type& lhs, const simulation_data_type& rhs, const simulation_data_type& range)
        {
            simulation_data_type t_lhs;
            Base::assign(t_lhs, lhs);
            forward(t_lhs, rhs, range);
            return t_lhs;
        }

        static simulation_data_type rewind(const simulation_data_type& lhs, const simulation_data_type& rhs, const simulation_data_type& range)
        {
            simulation_data_type t_lhs;
            Base::assign(t_lhs, lhs);
            rewind(t_lhs, rhs, range);
            return t_lhs;
        }
    };

}
#endif // _AEX_TRAITS_HPP
