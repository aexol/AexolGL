/**************************************************************
 *                                                            * 
 *      Filename:   MockSimulationDataTraits.hpp              * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 wrz 18 10:58:21                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#ifndef MOCKSIMULATIONDATATRAITS_HPP
#define MOCKSIMULATIONDATATRAITS_HPP 
#include <aex/Common.hpp>
#include "gmock/gmock.h"
#include <cmath>

using ::testing::Return;
using ::testing::_;

template<class T>
T mod(const T& lhs, const T& rhs)
{
    return lhs % rhs;
}

template<class T>
T imod(T& lhs, const T& rhs)
{
    lhs %= rhs;
    return lhs;
}

float mod(float lhs, float rhs)
{
    return std::fmod(lhs, rhs);
}

float imod(float& lhs, float rhs)
{
    return std::fmod(lhs, rhs);
}

ACTION(Assign) { arg0 = arg1; return arg0; }
ACTION(Sum) { return arg0 + arg1; }
ACTION(ISum) { arg0 += arg1; return arg0; }
ACTION(Sub) { return arg0 - arg1; }
ACTION(ISub) { arg0 -= arg1; return arg0; }
ACTION(Mul) { return arg0 * arg1; }
ACTION(IMul) { arg0 *= arg1; return arg0; }
ACTION(Div) { return arg0 / arg1; }
ACTION(IDiv) { arg0 /= arg1; return arg0; }
ACTION(Mod) { return mod(arg0, arg1); }
ACTION(IMod) { imod(arg0, arg1); return arg0; }
ACTION(ArgsLt) { return arg0 < arg1; }
ACTION(ArgsEq) { return arg0 == arg1; }
ACTION(ArgsGt) { return arg0 > arg1; }
ACTION(Inc) { arg0++; return  arg0;}
ACTION(Neg) {return -arg0;}

template<class T>
struct TMockSimDataTraitsInstance
{
    typedef T simulation_data_type;
    MOCK_CONST_METHOD0_T(max, simulation_data_type());
    MOCK_CONST_METHOD0_T(min, simulation_data_type());
    MOCK_CONST_METHOD0_T(zero, simulation_data_type());
    MOCK_CONST_METHOD0_T(unit, simulation_data_type());
    MOCK_CONST_METHOD2_T(assign, simulation_data_type&(simulation_data_type& lhs, const simulation_data_type& rhs));

    MOCK_CONST_METHOD2_T(add, simulation_data_type(const simulation_data_type& lhs, const simulation_data_type& rhs));
    MOCK_CONST_METHOD2_T(iadd, simulation_data_type&(simulation_data_type& lhs, const simulation_data_type& rhs));

    MOCK_CONST_METHOD2_T(sub, simulation_data_type(const simulation_data_type& lhs, const simulation_data_type& rhs));
    MOCK_CONST_METHOD2_T(isub, simulation_data_type&(simulation_data_type& lhs, const simulation_data_type& rhs));

    MOCK_CONST_METHOD2_T(mul, simulation_data_type(const simulation_data_type&, const simulation_data_type&));
    MOCK_CONST_METHOD2_T(imul, simulation_data_type&(simulation_data_type&, const simulation_data_type&));

    MOCK_CONST_METHOD2_T(div, simulation_data_type(const simulation_data_type&, const simulation_data_type&));
    MOCK_CONST_METHOD2_T(idiv, simulation_data_type&(simulation_data_type&, const simulation_data_type&));

    MOCK_CONST_METHOD2_T(mod, simulation_data_type(const simulation_data_type& lhs, const simulation_data_type& rhs));
    MOCK_CONST_METHOD2_T(imod, simulation_data_type&(simulation_data_type& lhs, const simulation_data_type& rhs));

    MOCK_CONST_METHOD2_T(lt, bool(const simulation_data_type& lhs, const simulation_data_type& rhs));
    MOCK_CONST_METHOD2_T(eq, bool(const simulation_data_type& lhs, const simulation_data_type& rhs));
    MOCK_CONST_METHOD2_T(gt, bool(const simulation_data_type& lhs, const simulation_data_type& rhs));

    MOCK_CONST_METHOD1_T(inc, simulation_data_type&(simulation_data_type&));
    MOCK_CONST_METHOD1_T(neg, simulation_data_type(const simulation_data_type&));

    void SetUpDefaults()
    {
        ON_CALL(*this, max())
            .WillByDefault(Return(std::numeric_limits<T>::max()))
            ;
        ON_CALL(*this, min())
            .WillByDefault(Return(T{}))
            ;
        ON_CALL(*this, zero())
            .WillByDefault(Return(T{}))
            ;
        ON_CALL(*this, unit())
            .WillByDefault(Return(T{1}))
            ;
        ON_CALL(*this, assign(_, _))
            .WillByDefault(Assign())
            ;
        ON_CALL(*this, add(_, _))
            .WillByDefault(Sum())
            ;
        ON_CALL(*this, iadd(_, _))
            .WillByDefault(ISum())
            ;
        ON_CALL(*this, sub(_, _))
            .WillByDefault(Sub())
            ;
        ON_CALL(*this, isub(_, _))
            .WillByDefault(ISub())
            ;
        ON_CALL(*this, mul(_, _))
            .WillByDefault(Mul())
            ;
        ON_CALL(*this, imul(_, _))
            .WillByDefault(IMul())
            ;
        ON_CALL(*this, div(_, _))
            .WillByDefault(Div())
            ;
        ON_CALL(*this, idiv(_, _))
            .WillByDefault(IDiv())
            ;
        ON_CALL(*this, mod(_, _))
            .WillByDefault(Mod())
            ;
        ON_CALL(*this, imod(_, _))
            .WillByDefault(IMod())
            ;
        ON_CALL(*this, lt(_, _))
            .WillByDefault(ArgsLt())
            ;
        ON_CALL(*this, eq(_, _))
            .WillByDefault(ArgsEq())
            ;
        ON_CALL(*this, gt(_, _))
            .WillByDefault(ArgsGt())
            ;
        ON_CALL(*this, inc(_))
            .WillByDefault(Inc())
            ;
        ON_CALL(*this, neg(_))
            .WillByDefault(Neg())
            ;
    }
};

template<class T>
struct TMockSimDataTraits
{
    using instance_type = TMockSimDataTraitsInstance<T>;
    typedef typename instance_type::simulation_data_type simulation_data_type;
    static instance_type* _instance;
    static simulation_data_type max(){return _instance->max();}
    static simulation_data_type min(){return _instance->min();}
    static simulation_data_type zero(){return _instance->zero();}
    static simulation_data_type unit(){return _instance->unit();}
    static simulation_data_type& assign(simulation_data_type& lhs, const simulation_data_type& rhs){return _instance->assign(lhs, rhs);}
    static simulation_data_type add(const simulation_data_type& lhs, const simulation_data_type& rhs){return _instance->add(lhs, rhs);}
    static simulation_data_type& iadd(simulation_data_type& lhs, const simulation_data_type& rhs){return _instance->iadd(lhs, rhs);}
    static simulation_data_type sub(const simulation_data_type& lhs, const simulation_data_type& rhs){return _instance->sub(lhs, rhs);}
    static simulation_data_type& isub(simulation_data_type& lhs, const simulation_data_type& rhs){return _instance->isub(lhs, rhs);}
    static simulation_data_type mul(const simulation_data_type& lhs, const simulation_data_type& rhs){return _instance->mul(lhs, rhs);}
    static simulation_data_type& imul(simulation_data_type& lhs, const simulation_data_type& rhs){return _instance->imul(lhs, rhs);}
    static simulation_data_type div(const simulation_data_type& lhs, const simulation_data_type& rhs){return _instance->div(lhs, rhs);}
    static simulation_data_type& idiv(simulation_data_type& lhs, const simulation_data_type& rhs){return _instance->idiv(lhs, rhs);}
    static simulation_data_type mod(const simulation_data_type& lhs, const simulation_data_type& rhs){return _instance->mod(lhs, rhs);}
    static simulation_data_type& imod(simulation_data_type& lhs, const simulation_data_type& rhs){return _instance->imod(lhs, rhs);}
    static bool lt(const simulation_data_type& lhs, const simulation_data_type& rhs){return _instance->lt(lhs, rhs);}
    static bool eq(const simulation_data_type& lhs, const simulation_data_type& rhs){return _instance->eq(lhs, rhs);}
    static bool gt(const simulation_data_type& lhs, const simulation_data_type& rhs){return _instance->gt(lhs, rhs);}
    static simulation_data_type& inc(simulation_data_type& lhs){return _instance->inc(lhs);}
    static simulation_data_type neg(const simulation_data_type& lhs){return _instance->neg(lhs);}
};

template<class T> TMockSimDataTraitsInstance<T>* TMockSimDataTraits<T>::_instance = nullptr;

using MockSimDataTraits = TMockSimDataTraits<int>;
using MockSimDataTraitsInstance = typename MockSimDataTraits::instance_type;
using FMockSimDataTraits = TMockSimDataTraits<float>;
using FMockSimDataTraitsInstance = typename FMockSimDataTraits ::instance_type;
using ATMockSimDataTraits = TMockSimDataTraits<aex::aexTimeUnit>;
using ATMockSimDataTraitsInstance = typename ATMockSimDataTraits::instance_type;

#endif /* MOCKSIMULATIONDATATRAITS_HPP */
