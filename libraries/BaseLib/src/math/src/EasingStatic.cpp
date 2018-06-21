#include <aex/Easing.hpp>

namespace aex {
    namespace math {
        namespace Easings {
            /////////////////////////////////////////////////////////////////////Back

            float Back::easeIn(float time, float a, float b, float duration)
            {
                float s = 1.70158f;
                time /= duration;
                return b * time* time * ((s + 1.0f) * time - s) + a;
            }

            float Back::easeOut(float time, float a, float b, float duration)
            {
                float s = 1.70158f;
                time = time / duration - 1.0f;
                return b * (time * time * ((s + 1.0f) * time + s) + 1.0f) + a;
            }

            float Back::easeInOut(float time, float a, float b, float duration)
            {
                float s = 1.70158f;

                if((time /= duration / 2.0f) < 1.0f)
                {
                    s *= 1.525f;
                    return b / 2.0f * (time * time * (s + 1.0f) * time - s) + a;
                }

                time -= 2.0f;
                s *= 1.525f;
                return b / 2.0f * (time * time * ((s + 1.0f) * time + s) + 2.0f) + a;
            }

            /////////////////////////////////////////////////////////////////////Bounce

            float Bounce::easeIn(float time, float a, float b, float duration)
            {
                return b - easeOut(duration - time, 0.0f, b, duration) + a;
            }

            float Bounce::easeOut(float time, float a, float b, float duration)
            {
                if((time /= duration) < (1.0f / 2.75f))
                {
                    return b * (7.5625f * time * time) + a;
                }
                else if(time < (2.0f / 2.75f))
                {
                    time -= (1.5f / 2.75f);
                    return b * (7.5625f * time * time + 0.75f) + a;
                }
                else if(time < (2.5f / 2.75f))
                {
                    time -= (2.25f / 2.75f);
                    return b * (7.5625f * time * time + 0.9375f) + a;
                }
                else
                {
                    time -= (2.625f / 2.75f);
                    return b * (7.5625f * time * time + 0.984375f) + a;
                }
            }

            float Bounce::easeInOut(float time, float a, float b, float duration)
            {
                if(time < duration / 2.0f)
                {
                    return easeIn(time * 2.0f, 0.0f, b, duration) * 0.5f + a;
                }
                else
                {
                    return easeOut(time * 2.0f - duration, 0.0f, b, duration) * 0.5f + b * 0.5f + a;
                }
            }

            /////////////////////////////////////////////////////////////////////Circ

            float Circ::easeIn(float time, float a, float b, float duration)
            {
                time /= duration;
                return -b * (sqrt(1.0f - time * time) - 1.0f) + a;
            }

            float Circ::easeOut(float time, float a, float b, float duration)
            {
                time = time / duration - 1.0f;
                return b * sqrt(1.0f - (time  * time)) + a;
            }

            float Circ::easeInOut(float time, float a, float b, float duration)
            {
                if((time /= duration / 2.0f) < 1)
                {
                    return -b / 2.0f * (sqrt(1.0f - time * time) - 1.0f) + a;
                }

                return b / 2.0f * (sqrt(1.0f - time * (time-2.0f) ) + 1.0f) + a;
            }

            /////////////////////////////////////////////////////////////////////Cubic

            float Cubic::easeIn(float time, float a, float b, float duration)
            {
                time /= duration;
                return b * time * time * time + a;
            }

            float Cubic::easeOut(float time, float a, float b, float duration)
            {
                time = time / duration - 1.0f;
                return b *(time * time * time + 1.0f) + a;
            }

            float Cubic::easeInOut(float time, float a, float b, float duration)
            {
                if((time /= duration / 2.0f) < 1.0f)
                {
                    return b / 2.0f * pow(time, 3.0f) + a;
                }
                time -= 2.0f;
                return b / 2.0f * (time * time * time + 2.0f) + a;
            }

            /////////////////////////////////////////////////////////////////////Linear

            float Linear::easeIn(float time, float a, float b, float duration)
            {
                return (b * (time / duration)) + a;
            }

            float Linear::easeOut(float time, float a, float b, float duration)
            {
                return (b * (time / duration)) + a;
            }

            float Linear::easeInOut(float time, float a, float b, float duration)
            {
                return (b * (time / duration)) + a;
            }

            /////////////////////////////////////////////////////////////////////Quint

            float Quint::easeIn(float time, float a, float b, float duration)
            {
                time /= duration;
                return b * pow(time, 5.0f) + a;
            }

            float Quint::easeOut(float time, float a, float b, float duration)
            {
                time = time / duration - 1.0f;
                return b * (pow(time, 5.0f) + 1.0f) + a;
            }

            float Quint::easeInOut(float time, float a, float b, float duration)
            {
                if((time /= duration / 2.0f) < 1.0f)
                {
                    return b / 2.0f * pow(time, 5.0f) + a;
                }
                time -= 2.0f;
                return b / 2.0f * (pow(time, 5.0f) + 2.0f) + a;
            }

            /////////////////////////////////////////////////////////////////////Sine

            float Sine::easeIn(float time, float a, float b, float duration)
            {
                return -b * cos(time / duration * (Math::PI / 2.0f)) + b + a;
            }

            float Sine::easeOut(float time, float a, float b, float duration)
            {
                return b * sin(time / duration * (Math::PI / 2.0f)) + a;
            }

            float Sine::easeInOut(float time, float a, float b, float duration)
            {
                return -b / 2.0f * (cos(Math::PI * time / duration) - 1.0f) + a;
            }

            /////////////////////////////////////////////////////////////////////Quadric

            float Quadric::easeIn(float time, float a, float b, float duration)
            {
                time /= duration;
                return b * time * time + a;
            }

            float Quadric::easeOut(float time, float a, float b, float duration)
            {
                time /= duration;
                return -b * time * (time - 2.0f) + a;
            }

            float Quadric::easeInOut(float time, float a, float b, float duration)
            {
                time /= duration / 2.0f;
                if(time < 1.0f)
                {
                    return ((b / 2.0f) * (time * time)) + a;
                }

                return -b / 2.0f * (((time - 2.0f) * (time-1.0f)) - 1.0f) + a;
            }

            /////////////////////////////////////////////////////////////////////Exponential

            float Exponential::easeIn(float time, float a, float b, float duration)
            {
                return (time == 0.0f) ? a : b * pow(2.0f, 10.0f * (time / duration - 1.0f)) + a;
            }

            float Exponential::easeOut(float time, float a, float b, float duration)
            {
                return (time == duration) ? a + b : b * (-pow(2.0f, -10.0f * time / duration) + 1.0f) + a;
            }

            float Exponential::easeInOut(float time, float a, float b, float duration)
            {
                if(time == 0)
                {
                    return a;
                }

                if(time == duration)
                {
                    return a + b;
                }

                if((time /= duration / 2.0f) < 1.0f)
                {
                    return b / 2.0f * pow(2.0f, 10.0f * (time - 1.0f)) + a;
                }

                return b / 2.0f * (-pow(2.0f, -10.0f * --time) + 2.0f) + a;
            }

            /////////////////////////////////////////////////////////////////////Elastic

            float Elastic::easeIn(float time, float a, float b, float duration)
            {
                if(time == 0)
                {
                    return a;
                }

                if((time /= duration) == 1.0f)
                {
                    return a + b;
                }

                float p = duration * 0.3f;
                float temp = b;
                float s = p / 4.0f;
                time -= 1.0f;
                float postFix = temp * pow(2.0f, 10.0f * time);
                return -(postFix * sin((time * duration - s) * (2.0f * Math::PI) / p)) + a;
            }

            float Elastic::easeOut(float time, float a, float b, float duration)
            {
                if(time == 0.0f)
                {
                    return a;
                }

                if((time /= duration) == 1.0f)
                {
                    return a + b;
                }

                float p = duration * 0.3f;
                float temp = b;
                float s = p / 4.0f;
                return (temp * pow(2.0f, -10.0f * time) * sin((time * duration - s) * (2.0f * Math::PI) / p) + b + a);
            }

            float Elastic::easeInOut(float time, float a, float b, float duration)
            {
                if(time == 0)
                {
                    return a;
                }

                if((time /= duration / 2.0f) == 2.0f)
                {
                    return a + b;
                }

                float p = duration * (0.3f * 1.5f);
                float temp = b;
                float s = p / 4.0f;

                if(time < 1.0f)
                {
                    time -= 1.0f;
                    float postFix = temp * pow(2.0f, 10.0f * time);
                    return -0.5f * (postFix * sin((time * duration - s) * (2.0f * Math::PI) / p)) + a;
                }
                time -= 1.0f;
                float postFix = temp * pow(2.0f, -10.0f * time);
                return postFix * sin((time * duration - s) * (2.0f * Math::PI) / p) * 0.5f + b + a;
            }
        } /* Easings */
    } /* math */
} /* aex */

