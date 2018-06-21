#include <aex/Easing.hpp>

namespace aex {
    namespace math {
        namespace Easings {
            ///////////////////////////////////////////////////////////Back

            float Back::eIn(float time, float a, float b, float duration)
            {
                return Back::easeIn(time, a, b, duration);
            }

            float Back::eOut(float time, float a, float b, float duration)
            {
                return Back::easeOut(time, a, b, duration);
            }

            float Back::eInOut(float time, float a, float b, float duration)
            {
                return Back::easeInOut(time, a, b, duration);
            }


            /////////////////////////////////////////////////////////////////////Bounce

            float Bounce::eIn(float time, float a, float b, float duration)
            {
                return Bounce::easeIn(time, a, b, duration);
            }

            float Bounce::eOut(float time, float a, float b, float duration)
            {
                return Bounce::easeOut(time, a, b, duration);
            }

            float Bounce::eInOut(float time, float a, float b, float duration)
            {
                return Bounce::easeInOut(time, a, b, duration);
            }


            /////////////////////////////////////////////////////////////////////Circ

            float Circ::eIn(float time, float a, float b, float duration)
            {
                return Circ::easeIn(time, a, b, duration);
            }

            float Circ::eOut(float time, float a, float b, float duration)
            {
                return Circ::easeOut(time, a, b, duration);
            }

            float Circ::eInOut(float time, float a, float b, float duration)
            {
                return Circ::easeInOut(time, a, b, duration);
            }


            /////////////////////////////////////////////////////////////////////Cubic

            float Cubic::eIn(float time, float a, float b, float duration)
            {
                return Cubic::easeIn(time, a, b, duration);
            }

            float Cubic::eOut(float time, float a, float b, float duration)
            {
                return Cubic::easeOut(time, a, b, duration);
            }

            float Cubic::eInOut(float time, float a, float b, float duration)
            {
                return Cubic::easeInOut(time, a, b, duration);
            }


            /////////////////////////////////////////////////////////////////////Linear

            float Linear::eIn(float time, float a, float b, float duration)
            {
                return Linear::easeIn(time, a, b, duration);
            }

            float Linear::eOut(float time, float a, float b, float duration)
            {
                return Linear::easeOut(time, a, b, duration);
            }

            float Linear::eInOut(float time, float a, float b, float duration)
            {
                return Linear::easeInOut(time, a, b, duration);
            }


            /////////////////////////////////////////////////////////////////////Quint

            float Quint::eIn(float time, float a, float b, float duration)
            {
                return Quint::easeIn(time, a, b, duration);
            }

            float Quint::eOut(float time, float a, float b, float duration)
            {
                return Quint::easeOut(time, a, b, duration);
            }

            float Quint::eInOut(float time, float a, float b, float duration)
            {
                return Quint::easeInOut(time, a, b, duration);
            }


            /////////////////////////////////////////////////////////////////////Sine

            float Sine::eIn(float time, float a, float b, float duration)
            {
                return Sine::easeIn(time, a, b, duration);
            }

            float Sine::eOut(float time, float a, float b, float duration)
            {
                return Sine::easeOut(time, a, b, duration);
            }

            float Sine::eInOut(float time, float a, float b, float duration)
            {
                return Sine::easeInOut(time, a, b, duration);
            }


            /////////////////////////////////////////////////////////////////////Quadric

            float Quadric::eIn(float time, float a, float b, float duration)
            {
                return Quadric::easeIn(time, a, b, duration);
            }

            float Quadric::eOut(float time, float a, float b, float duration)
            {
                return Quadric::easeOut(time, a, b, duration);
            }

            float Quadric::eInOut(float time, float a, float b, float duration)
            {
                return Quadric::easeInOut(time, a, b, duration);
            }


            /////////////////////////////////////////////////////////////////////Exponential

            float Exponential::eIn(float time, float a, float b, float duration)
            {
                return Exponential::easeIn(time, a, b, duration);
            }

            float Exponential::eOut(float time, float a, float b, float duration)
            {
                return Exponential::easeOut(time, a, b, duration);
            }

            float Exponential::eInOut(float time, float a, float b, float duration)
            {
                return Exponential::easeInOut(time, a, b, duration);
            }


            /////////////////////////////////////////////////////////////////////Elastic

            float Elastic::eIn(float time, float a, float b, float duration)
            {
                return Elastic::easeIn(time, a, b, duration);
            }

            float Elastic::eOut(float time, float a, float b, float duration)
            {
                return Elastic::easeOut(time, a, b, duration);
            }

            float Elastic::eInOut(float time, float a, float b, float duration)
            {
                return Elastic::easeInOut(time, a, b, duration);
            }

            float Sigmoid::eIn(float time, float a, float b, float duration)
            {
                return Elastic::easeIn(time, a, b, duration);
            }

            float Sigmoid::eOut(float time, float a, float b, float duration)
            {
                return Elastic::easeOut(time, a, b, duration);
            }

            float Sigmoid::eInOut(float time, float a, float b, float duration)
            {
                return Elastic::easeInOut(time, a, b, duration);
            }


            float Sigmoid::getLambda()
            {
                return m_lambda;
            }

            float Sigmoid::getSigma()
            {
                return m_sigma;
            }

            void Sigmoid::setLambda(float lambda)
            {
                m_lambda=lambda;
            }

            void Sigmoid::setSigma(float sigma)
            {
                m_sigma=sigma;
            }
        } /* Easings */
    } /* math */
} /* aex */
