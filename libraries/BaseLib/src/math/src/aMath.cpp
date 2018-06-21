/*
 * Math.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: aexol-1
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <aex/Math.hpp>
#include <aex/Vector2.hpp>
#include <aex/Vector3.hpp>
#include <aex/Logger.hpp>
namespace aex {
    namespace math {
        const float Math::PI = 3.14159265358979323846f;
        const float Math::TWO_PI = 2.0f * PI;
        const float Math::HALF_PI = 0.5f * PI;
        const float Math::QUARTER_PI = 0.25f * PI;

        const float Math::V_DEG2RAD = PI / 180.0f;
        const float Math::V_DEG2RADDIV2 = PI / 360.0f;
        const float Math::V_RAD2DEG = 180.0f / PI;
        const float Math::EPSILON = 0.00001f;
        const float Math::PHI = 1.6180339887498948482;
        const float Math::GOLDEN_ANGLE_DEG = 137.5077640500378546463487;
        const float Math::GOLDEN_ANGLE_RAD = 2.39996322972865332;
        Math::Math() {
            // TODO Auto-generated constructor stub
        }

        Math::~Math() {
            // TODO Auto-generated destructor stub
        }

        static CONSTEXPR const float pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348;
        static CONSTEXPR const float _TWO_PI = 6.2831853071795864769252867665590057683943387987502116419498891846156328125724179972560696;

        template <typename T, typename = typename std::enable_if <
            std::is_arithmetic<T>::value
            >::type
            > CONSTEXPR T const_square(T& a)
            {
                return a * a;
            }

        template <typename T, typename = typename std::enable_if <
            std::is_arithmetic<T>::value
            >::type
            > CONSTEXPR T const_div(T a, T b)
            {
                return a / b;
            }

        float Math::lerp(float a, float b, float t)
        {
            return (((1.0f - t) * a) + (b * t));
        }

        float Math::fast_sine(float x)
        {
            x += pi;
            int32_t floor = (int32_t) (x / _TWO_PI);
            floor -= (x < 0) & 1;
            x = x - _TWO_PI * floor;
            x -= pi;

            const float B = const_div(4.0f, pi);
            const float C = const_div(-4.0f, (const_square(pi)));

            float y = B * x + C * x * fabs(x);

            return y;
        }

        float Math::fast_sine_precise(float x)
        {
            x += pi;
            int32_t floor = (int32_t) (x / _TWO_PI);
            floor -= (x < 0) & 1;
            x = x - _TWO_PI * floor;
            x -= pi;

            const float B = const_div(4.0f, pi);
            const float C = const_div(-4.0f, (const_square(pi)));

            float y = B * x + C * x * fabs(x);

            const float P = 0.225;
            y = P * (y * fabs(y) - y) + y;
            return y;
        }

        float Math::fast_sine_poly(float _fX)
        {
            int32_t i32I = int32_t( _fX * 0.31830988618379067153776752674503f );	// 1 / PI.
            _fX = (_fX - float( i32I ) * 3.1415926535897932384626433832795f);

            float fX2 = _fX * _fX;

            return (i32I & 1) ?
                -_fX * (float( 1.0 ) +
                        fX2 * (float( -1.66666671633720398e-01 ) +
                            fX2 * (float( 8.33333376795053482e-03 ) +
                                fX2 * (float( -1.98412497411482036e-04 ) +
                                    fX2 * (float( 2.75565571428160183e-06 ) +
                                        fX2 * (float( -2.50368437093584362e-08 ) +
                                            fX2 * (float( 1.58846852338356825e-10 ) +
                                                fX2 * float( -6.57978446033657960e-13 )))))))) :
                _fX * (float( 1.0 ) +
                        fX2 * (float( -1.66666671633720398e-01 ) +
                            fX2 * (float( 8.33333376795053482e-03 ) +
                                fX2 * (float( -1.98412497411482036e-04 ) +
                                    fX2 * (float( 2.75565571428160183e-06 ) +
                                        fX2 * (float( -2.50368437093584362e-08 ) +
                                            fX2 * (float( 1.58846852338356825e-10 ) +
                                                fX2 * float( -6.57978446033657960e-13 ))))))));
        }

        float Math::fast_cosine(float x)
        {
            x += pi / 2;
            float pix2 = 2 * pi;
            uint32_t* pi_bits = (uint32_t*) (&pix2);
            *pi_bits &= (~(x > pi) + 1);
            x -= pix2;
            return fast_sine(x);
        }

        float Math::fast_cosine_precise(float x)
        {
            x += pi / 2;
            float pix2 = 2 * pi;
            uint32_t* pi_bits = (uint32_t*) (&pix2);
            *pi_bits &= (~(x > pi) + 1);
            x -= pix2;
            return fast_sine_precise(x);
        }

        float Math::fast_cosine_poly( float _fX ) 
        {
            int32_t i32I = int32_t( _fX * 0.31830988618379067153776752674503f );	// 1 / PI.
            _fX = (_fX - float( i32I ) * 3.1415926535897932384626433832795f);

            float fX2 = _fX * _fX;

            return (i32I & 1) ?
                -float( 1.0 ) -
                fX2 * (float( -5.00000000000000000e-01 ) +
                        fX2 * (float( 4.16666641831398010e-02 ) +
                            fX2 * (float( -1.38888671062886715e-03 ) +
                                fX2 * (float( 2.48006836045533419e-05 ) +
                                    fX2 * (float( -2.75369188784679864e-07 ) +
                                        fX2 * (float( 2.06202765973273472e-09 ) +
                                            fX2 * float( -9.77589970779790818e-12 ))))))) :
                float( 1.0 ) +
                fX2 * (float( -5.00000000000000000e-01 ) +
                        fX2 * (float( 4.16666641831398010e-02 ) +
                            fX2 * (float( -1.38888671062886715e-03 ) +
                                fX2 * (float( 2.48006836045533419e-05 ) +
                                    fX2 * (float( -2.75369188784679864e-07 ) +
                                        fX2 * (float( 2.06202765973273472e-09 ) +
                                            fX2 * float( -9.77589970779790818e-12 )))))));
        }

        float Math::clamp_value_float(float min, float max, float value)
        {

            float difference = max - min;

            if (value > max)
            {
                value -= difference;
            } else if (value < min)
            {
                value += difference;
            }

            return value;

        }

        int Math::clamp_value_int(int min, int max, int value)
        {

            int difference = max - min;

            if (value > max)
            {
                value -= difference;
            } else if (value < min)
            {
                value += difference;
            }

            return value;

        }

        float Math::isBetween(float min, float max, float value)
        {
            if (value > min && value < max)
            {
                return true;
            } else
            {
                return false;
            }

        }

        float Math::isBetweenFlaot(float min, float max, float value)
        {

            if (max < min)
            {
                float temp = max;
                max = min;
                min = temp;
            }

            if (value > min && value < max)
            {
                return true;
            } else
            {
                return false;
            }
        }

        Vector3 Math::axis_angle_to_euler(float angle, float x, float y, float z)
        {
            Vector3 ret;

            float s = sin(angle);
            float c = cos(angle);
            float t = 1.0f - c;
            //  if axis is not already normalised then uncomment this
            // double magnitude = Math.sqrt(x*x + y*y + z*z);
            // if (magnitude==0) throw error;
            // x /= magnitude;
            // y /= magnitude;
            // z /= magnitude;
            if ((x * y * t + z * s) > 0.998f)
            { // north pole singularity detected
                ret.x = 2.0f * atan2(x * sin(angle / 2.0f), cos(angle / 2.0f));
                ret.y = PI / 2.0f;
                ret.z = 0.0f;
                return ret;
            } else if ((x * y * t + z * s) < -0.998f)
            { // south pole singularity detected
                ret.x = -2.0f * atan2(x * sin(angle / 2.0f), cos(angle / 2.0f));
                ret.y = -PI / 2.0f;
                ret.z = 0.0f;
                return ret;
            }

            ret.x = atan2(y * s - x * z * t, 1.0f - (y * y + z * z) * t);
            ret.y = asin(x * y * t + z * s);
            ret.z = atan2(x * s - y * z * t, 1.0f - (x * x + z * z) * t);

            return ret;
        }

        Vector3 Math::axis_angle_to_euler(float angleDeg, Vector3 axis)
        {
            return axis_angle_to_euler(angleDeg, axis.x, axis.y, axis.z);
        }

        float Math::angleBeetwenVectors(const Vector3& v1, const Vector3& v2, const Vector3& center)
        {
            Vector3 c1 = v1 - center;
            Vector3 c2 = v2 - center;

            //LOG_DEBUG("c1.cross(c2).length(): ",c1.cross(c2).length()," c1.dot(c2): ", c1.dot(c2));
            //LOG_DEBUG("c1.cross(c2).length(): ",c1.cross(c2).length()," c1.dot(c2): ", c1.dot(c2));
            //float angleInRadians = acosf((c1.dot(c2)) / (c1.length() * c2.length()));


            float sina = cross(c1, c2).length() / (c1.length() * c2.length());
            float cosa = dot(c1, c2) / (c1.length() * c2.length());

            float angle = atan2(sina, cosa);

            float sign = dot(c1, c2);
            if (sign < 0.0) {
                angle = -angle;
            }

            /*float angle = acos(dot(c1.unit(), c2.unit()));
              Vector3 crosp = cross(c1, c2);

              Vector3 Vn = v1.cross(v2);

              if (dot(Vn, crosp) < 0) { // Or > 0
              angle = -angle;
              }*/

            float angleInDegrees = angle * V_RAD2DEG;

            return angleInDegrees;

        }


        float Math::angle_from_point_deg(float x, float y)
        {
            float angleInRadians = std::atan2(x,y);
            float angleInDegrees = angleInRadians * V_RAD2DEG;

            if (angleInDegrees < 0.0f)
            {
                angleInDegrees = 360.0f + angleInDegrees;
            }
            return angleInDegrees;
        }

        Vector2 Math::point_on_circle(float angleDeg, float radius)
        {
            Vector2 ret;
            ret.x = Math::fast_sine_precise(angleDeg * V_DEG2RAD) * radius;
            ret.y = Math::fast_cosine_precise(angleDeg * V_DEG2RAD) * radius;

            return ret;
        }

        Vector3 Math::point_on_sphere(float angleDegX, float angleDegY, float radius)
        {
            Vector3 ret;

            ret.x = Math::fast_sine_precise(angleDegX * V_DEG2RAD) * Math::fast_cosine_precise(angleDegY * V_DEG2RAD) * radius;
            ret.y = Math::fast_sine_precise(angleDegY * V_DEG2RAD) * radius;
            ret.z = Math::fast_cosine_precise(angleDegX * V_DEG2RAD) * Math::fast_cosine_precise(angleDegY * V_DEG2RAD) * radius;
            return ret;
        }

        Vector3 Math::hsv_to_rgb(float hue, float sat, float val)
        {
            float red = 0.0f, grn = 0.0f, blu = 0.0f;
            float i, f, p, q, t;

            if (closeEnough(val, 0.0f))
            {
                red = 0.0f;
                grn = 0.0f;
                blu = 0.0f;
            } else
            {
                hue /= 60.0f;
                i = floor(hue);
                f = hue - i;
                p = val * (1 - sat);
                q = val * (1 - (sat * f));
                t = val * (1 - (sat * (1 - f)));

                if (closeEnough(i, 0.0f))
                {
                    red = val;
                    grn = t;
                    blu = p;
                } else if (closeEnough(i, 1.0f))
                {
                    red = q;
                    grn = val;
                    blu = p;
                } else if (closeEnough(i, 2.0f))
                {
                    red = p;
                    grn = val;
                    blu = t;
                } else if (closeEnough(i, 3.0f))
                {
                    red = p;
                    grn = q;
                    blu = val;
                } else if (closeEnough(i, 4.0f))
                {
                    red = t;
                    grn = p;
                    blu = val;
                } else if (closeEnough(i, 5.0f))
                {
                    red = val;
                    grn = p;
                    blu = q;
                }
            }

            Vector3 ret;
            ret.x = red;
            ret.y = grn;
            ret.z = blu;
            return ret;
        }

        bool
            Math::closeEnough(Vector3 a, Vector3 b, float epsilon)
            {
                return closeEnough(a.x, b.x, epsilon) && closeEnough(a.y, b.y, epsilon) && closeEnough(a.z, b.z, epsilon);
            }

        int Math::sgnInt(int val)
        {
            return (0 < val) - (val < 0);
        }

        int Math::sgnFloat(float val)
        {
            return (0.0f < val) - (val < 0.0f);
        }

        Vector3 Math::vector_cross_plane(Vector3 position, Vector3 vector, Vector3 normal, float d)
        {
            Vector3 ret;

            Vector3 a= position;
            Vector3 b= position+vector;



            return ret;
        }

        Vector3 Math::rgb_to_hsv(float red, float grn, float blu)
        {
            float hue, sat, val;
            float x, f, i;

            x = min(min(red, grn), blu);
            val = max(max(red, grn), blu);

            if (closeEnough(x, val))
            {
                hue = 0.0f;
                sat = 0.0f;
            } else
            {
                f = (closeEnough(red, x)) ? grn - blu : ((closeEnough(grn, x)) ? blu - red : red - grn);
                i = (closeEnough(red, x)) ? 3.0f : ((closeEnough(grn, x)) ? 5.0f : 1.0f);
                hue = fmod((i - f / (val - x)) * 60.0f, 360.0f);
                sat = ((val - x) / val);
            }

            Vector3 ret;
            ret.x = hue;
            ret.y = sat;
            ret.z = val;
            return ret;
        }

        bool Math::isPointLiesInVector(Vector3 begin, Vector3 end, Vector3 point)
        {
            if (Math::closeEnough((end - begin).cross(point - begin).length(), 0.0f, 0.001f))
            {
                if ((point - begin).dot(end - point) >= 0.0f)
                {
                    return true;
                } else
                {
                    return false;
                }
            } else
            {
                return false;
            }
        }

        Vector3 Math::randomVec3(float lenght)
        {
            Vector3 ret(randomFloat(-10.0, 10.0), randomFloat(-10.0, 10.0), randomFloat(-10.0, 10.0));
            ret.normalizeThis();
            ret *= lenght;
            return ret;
        }

        Vector3 Math::randomVec3varyingLenght(float minlenght, float maxlenght)
        {
            return randomVec3(randomFloat(minlenght, maxlenght));
        }

    } /* math */
} /* aex */
