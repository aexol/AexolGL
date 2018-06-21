/*
 * File:   Easing.hpp
 * Author: mritke
 *
 * Created on September 11, 2014, 4:55 PM
 */

#ifndef EASING_HPP
#define	EASING_HPP

#include "Common.hpp"
#include <aex/Math.hpp>

namespace aex
{
    namespace math
    {
        namespace Easings
        {

            enum EasingType
            {
                EASE_IN, EASE_OUT, EASE_IN_OUT, EASE_OUT_IN
            };

            enum EasingFuncType 
            {
                EASEING_BACK, EASEING_BOUNCE, EASEING_CIRCULAR, EASEING_CUBIC, EASEING_LINEAR, EASEING_QUINT, EASEING_QUADRIC, EASEING_EXPONENTIAL, EASEING_ELASTIC
            };

            class Easing
            {
                private:

                public:
                    virtual ~Easing(){}
                    virtual float eIn(float time, float a, float b, float duration = 1.0f) = 0;
                    virtual float eOut(float time, float a, float b, float duration = 1.0f) = 0;
                    virtual float eInOut(float time, float a, float b, float duration = 1.0f) = 0;
            };

            class Back : public Easing
            {
                public:
                    //float step(float step);

                    float eIn(float time, float a, float b, float duration = 1.0f);
                    float eOut(float time, float a, float b, float duration = 1.0f);
                    float eInOut(float time, float a, float b, float duration = 1.0f);

                    static float easeIn(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeInOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);

            };

            class Bounce : public Easing
            {
                public:

                    float eIn(float time, float a, float b, float duration = 1.0f);
                    float eOut(float time, float a, float b, float duration = 1.0f);
                    float eInOut(float time, float a, float b, float duration = 1.0f);

                    static float easeIn(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeInOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);

            };

            class Circ : public Easing
            {
                public:

                    float eIn(float time, float a, float b, float duration = 1.0f);
                    float eOut(float time, float a, float b, float duration = 1.0f);
                    float eInOut(float time, float a, float b, float duration = 1.0f);

                    static float easeIn(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeInOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);

            };

            class Cubic : public Easing
            {
                public:

                    float eIn(float time, float a, float b, float duration = 1.0f);
                    float eOut(float time, float a, float b, float duration = 1.0f);
                    float eInOut(float time, float a, float b, float duration = 1.0f);

                    static float easeIn(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeInOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);

            };

            class Linear : public Easing
            {
                public:

                    float eIn(float time, float a, float b, float duration = 1.0f);
                    float eOut(float time, float a, float b, float duration = 1.0f);
                    float eInOut(float time, float a, float b, float duration = 1.0f);

                    static float easeIn(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeInOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);

            };

            class Quint : public Easing
            {
                public:

                    float eIn(float time, float a, float b, float duration = 1.0f);
                    float eOut(float time, float a, float b, float duration = 1.0f);
                    float eInOut(float time, float a, float b, float duration = 1.0f);

                    static float easeIn(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeInOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);

            };

            class Sine : public Easing
            {
                public:

                    float eIn(float time, float a, float b, float duration = 1.0f);
                    float eOut(float time, float a, float b, float duration = 1.0f);
                    float eInOut(float time, float a, float b, float duration = 1.0f);

                    static float easeIn(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeInOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);

            };

            class Quadric : public Easing
            {
                public:

                    float eIn(float time, float a, float b, float duration = 1.0f);
                    float eOut(float time, float a, float b, float duration = 1.0f);
                    float eInOut(float time, float a, float b, float duration = 1.0f);

                    static float easeIn(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeInOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);

            };

            class Exponential : public Easing
            {
                public:

                    float eIn(float time, float a, float b, float duration = 1.0f);
                    float eOut(float time, float a, float b, float duration = 1.0f);
                    float eInOut(float time, float a, float b, float duration = 1.0f);

                    static float easeIn(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeInOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);

            };

            class Elastic : public Easing
            {
                public:

                    float eIn(float time, float a, float b, float duration = 1.0f);
                    float eOut(float time, float a, float b, float duration = 1.0f);
                    float eInOut(float time, float a, float b, float duration = 1.0f);

                    static float easeIn(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                    static float easeInOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);

            };


            class Sigmoid : public Easing
            {

                float m_lambda = 2.0;
                float m_sigma = 0.2;

                public:

                float getLambda();
                float getSigma();

                void setLambda(float lambda);
                void setSigma(float sigma);

                float eIn(float time, float a, float b, float duration = 1.0f);
                float eOut(float time, float a, float b, float duration = 1.0f);
                float eInOut(float time, float a, float b, float duration = 1.0f);

                static float easeIn(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                static float easeOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);
                static float easeInOut(float time, float a = 0.0f, float b = 1.0f, float duration = 1.0f);

            };

            template<class T>
            class EasingWrap
            {
                private:
                    T m_a;
                    T m_b;
                    T m_value;
                    Easing* m_easing;
                    EasingType m_type;
                    aex::aexTimeUnit m_time = 0.0;
                    aex::aexTimeUnit m_duration;

                public:

                    EasingWrap(T a, T b, Easing* easing, EasingType type = EASE_IN, float duration = 1.0f) : m_a(a), m_b(b), m_easing(easing), m_type(type), m_duration(duration)
                {
                }

                    EasingWrap(T a, T b, EasingFuncType ftype, EasingType etype = EASE_IN, float duration = 1.0f) : m_a(a), m_b(b), m_type(etype), m_duration(duration)
                {
                    switch(ftype)
                    {

                        case EasingFuncType::EASEING_BACK:
                            m_easing = new Back();
                            break;

                        case EasingFuncType::EASEING_BOUNCE:
                            m_easing = new Bounce();
                            break;

                        case EasingFuncType::EASEING_CIRCULAR:
                            m_easing = new Circ();
                            break;

                        case EasingFuncType::EASEING_CUBIC:
                            m_easing = new Cubic();
                            break;

                        case EasingFuncType::EASEING_LINEAR:
                            m_easing = new Linear();
                            break;

                        case EasingFuncType::EASEING_QUINT:
                            m_easing = new Quint();
                            break;

                        case EasingFuncType::EASEING_QUADRIC:
                            m_easing = new Quadric();
                            break;

                        case EasingFuncType::EASEING_EXPONENTIAL:
                            m_easing = new Exponential();
                            break;

                        case EasingFuncType::EASEING_ELASTIC:
                            m_easing = new Elastic();
                            break;

                        default:
                            m_easing = new Linear();
                            break;

                    }

                }

                    ~EasingWrap()
                    {
                        delete m_easing;
                    }

                    T step(float elapsedtime)
                    {
                        m_time = aex::math::Math::clamp_value_float(0.0, m_duration, (float)m_time + elapsedtime);
                        return compute(m_time);
                    }

                    T compute(float time)
                    {
                        m_time = time;
                        switch(m_type)
                        {
                            case EASE_IN:
                                return m_value = m_a + (m_b - m_a) * m_easing->eIn(m_time, 0.0f, 1.0f, m_duration);
                                break;

                            case EASE_OUT:
                                return m_value = m_a + (m_b - m_a) * m_easing->eOut(m_time, 0.0f, 1.0f, m_duration);
                                break;

                            case EASE_IN_OUT:
                                return m_value = m_a + (m_b - m_a) * m_easing->eInOut(m_time, 0.0f, 1.0f, m_duration);
                                break;

                            default:
                                return m_value = m_a + (m_b - m_a) * m_easing->eIn(m_time, 0.0f, 1.0f, m_duration);
                                break;
                        }
                    }

                    T getValue()
                    {
                        return m_value;
                    }

                    void setA(T a)
                    {
                        m_a = a;
                    }

                    T getA() const
                    {
                        return m_a;
                    }

                    void setB(T b)
                    {
                        m_b = b;
                    }

                    T getB() const
                    {
                        return m_b;
                    }

                    void setType(EasingType type)
                    {
                        m_type = type;
                    }

                    EasingType getType() const
                    {
                        return m_type;
                    }

                    void setEasing(Easing* easing)
                    {
                        m_easing = easing;
                    }

                    Easing* getEasing() const
                    {
                        return m_easing;
                    }
            };
        }
    }
}

#endif	/* EASING_HPP */

