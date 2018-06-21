/*
 * Math.h
 *
 *  Created on: Jun 27, 2013
 *      Author: aexol-1
 */

#ifndef MATH_H_
#define MATH_H_
#include <aex/Common.hpp>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <array>
#include <vector>

namespace aex
{
    namespace math{

        class Vector2;
        class Vector3;

        class AEX_PUBLIC_API Math
        {
            public:
                static const float PI;
                static const float TWO_PI;
                static const float HALF_PI;
                static const float QUARTER_PI;
                static const float V_DEG2RAD;
                static const float V_DEG2RADDIV2;
                static const float V_RAD2DEG;
                static const float EPSILON;
                static const float PHI;
                static const float GOLDEN_ANGLE_DEG;
                static const float GOLDEN_ANGLE_RAD;

                template<typename NumberType> static inline typename std::enable_if<std::is_integral<NumberType>::value, NumberType>::type max(NumberType a, NumberType b);
                template<typename NumberType> static inline typename std::enable_if<std::is_floating_point<NumberType>::value, NumberType>::type max(NumberType a, NumberType b);
                template<typename NumberType> static inline typename std::enable_if<std::is_integral<NumberType>::value, NumberType>::type min(NumberType a, NumberType b);
                template<typename NumberType> static inline typename std::enable_if<std::is_floating_point<NumberType>::value, NumberType>::type min(NumberType a, NumberType b);

                static inline float DEG2RAD(float a);
                static inline float RAD2DEG(float a);
                static inline bool closeEnough(float a, float b, float epsilon = 0.000001f);
                static inline bool closeEnough(double a, double b, double epsilon = 0.000001f);
                static bool closeEnough(Vector3 a, Vector3 b, float epsilon = 0.000001f);
                static int sgnInt(int val);
                static int sgnFloat(float val);

                static inline std::array<float, 16> identity4();
                static inline std::array<float, 9> identity3();
                static inline std::array<float, 4> identity2();

                static inline int randomInt(int min, int max);
                static inline float randomFloat(float min, float max);
                static Vector3 randomVec3varyingLenght(float  minlenght, float maxlenght);
                static Vector3 randomVec3(float lenght);
                static bool  isPointLiesInVector(Vector3 begin, Vector3 end, Vector3 point);
                static float lerp(float a, float b, float t);

                static float fast_sine(float rad);
                static float fast_sine_precise(float rad);
                static float fast_sine_poly( float _fX );
                static float fast_cosine(float rad);
                static float fast_cosine_precise(float rad);
                static float fast_cosine_poly( float _fX );
                static float clamp_value_float(float min, float max, float value);
                static int clamp_value_int(int min, int max, int value);
                static float isBetween(float min, float max, float value);
                static float isBetweenFlaot(float min, float max, float value);

                static Vector3 axis_angle_to_euler(float angleDeg, float x,float y,float z);
                static Vector3 axis_angle_to_euler(float angleDeg, Vector3 axis );
                static float angleBeetwenVectors(const Vector3& v1, const Vector3& v2, const Vector3& center);
                static float angle_from_point_deg(float x, float y);
                static Vector2 point_on_circle(float angleDeg, float radius);
                static Vector3 point_on_sphere(float angleDegX, float angleDegY, float radius);
                static Vector3 hsv_to_rgb(float hue, float sat, float val);
                static Vector3 rgb_to_hsv(float red, float grn, float blu);
                static Vector3 vector_cross_plane(Vector3 position, Vector3 vector, Vector3 normal, float d);

                Math();
                virtual ~Math();
            private:
        };

        template<typename NumberType>
            typename std::enable_if<std::is_integral<NumberType>::value, NumberType>::type
            Math::max(NumberType a, NumberType b)
            {
                return (a > b) ? a : b;
            }

        template<typename NumberType>
            typename std::enable_if<std::is_floating_point<NumberType>::value, NumberType>::type
            Math::max(NumberType a, NumberType b)
            {
                return (a > b) ? a : b;
            }

        template<typename NumberType>
            typename std::enable_if<std::is_integral<NumberType>::value, NumberType>::type
            Math::min(NumberType a, NumberType b)
            {
                return (a < b) ? a : b;
            }

        template<typename NumberType>
            typename std::enable_if<std::is_floating_point<NumberType>::value, NumberType>::type
            Math::min(NumberType a, NumberType b)
            {
                return (a < b) ? a : b;
            }
        /*
         * Returns angle a given in degree in radians
         */
        float
            Math::DEG2RAD(float a)
            {
                return a * V_DEG2RAD;
            }

        /*
         * Returns angle a given in radians in degree
         */
        float
            Math::RAD2DEG(float a)
            {
                return a * V_RAD2DEG;
            }

        /*
         * Compares 2 floats with epsilon acceptable error
         */
        bool
            Math::closeEnough(float a, float b, float epsilon)
            {
                return std::fabs(a - b) < epsilon;
            }

        bool
            Math::closeEnough(double a, double b, double epsilon)
            {
                return std::fabs(a - b) < epsilon;
            }


        std::array<float, 16> Math::identity4()
        {
            return (std::array<float, 16>
                    {
                    {1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f}
                    });
        }
        std::array<float, 9> Math::identity3()
        {
            return (std::array<float, 9>
                    {
                    {1.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 1.0f}
                    });
        }
        std::array<float, 4> Math::identity2()
        {
            return (std::array<float, 4>
                    {
                    {1.0f, 0.0f,
                    0.0f, 1.0f}
                    });
        }

        /*
           Produces a random int x, min <= x <= max
           following a uniform distribution
           */
        int
            Math::randomInt(int min, int max)
            {
                return min + rand() % (max - min + 1);
            }

        /*
           Produces a random float x, min <= x <= max
           following a uniform distribution
           */

        float
            Math::randomFloat(float min, float max)
            {
                return ((float)rand()) / (float) RAND_MAX * (max - min) + min;
            }

#define MID ((lo + hi + 1) / 2)
        CONSTEXPR uint64_t sqrt_helper(uint64_t x, uint64_t lo, uint64_t hi)
        {
            return lo == hi ? lo : ((x / MID < MID)
                    ? sqrt_helper(x, lo, MID - 1) : sqrt_helper(x, MID, hi));
        }

        CONSTEXPR uint64_t ct_sqrt(uint64_t x)
        {
            return sqrt_helper(x, 0, x / 2 + 1);
        }
#undef MID
        template<class T, std::size_t SIZE>
            std::array<T, SIZE> transpose(const std::array<T, SIZE>& arr)
            {
                CONSTEXPR std::size_t size_root = ct_sqrt(SIZE);
                std::array<T, SIZE> ret;

                for(std::size_t ii = 0; ii < size_root; ii++)
                    for(std::size_t jj = 0; jj < size_root; jj++)
                    {
                        ret[ii + jj * size_root] = arr[ii * size_root + jj];
                    }

                return ret;
            }
        template<class T, std::size_t SIZE>
            void vectorToArray(const std::vector<T>& vecIn, std::array<T, SIZE>& arrOut)
            {
                if(vecIn.size() == SIZE)
                {
                    std::copy(vecIn.begin(), vecIn.end(), arrOut.begin());
                }
            }
        template<class T, std::size_t SIZE>
            void arrayToVector(const std::array<T, SIZE>& arrIn, std::vector<T>& vecOut)
            {
                std::copy(arrIn.begin(), arrIn.end(), vecOut.begin());
            }

        template<class T, std::size_t SIZE>
            void fillArrayIdentity(std::array<T, SIZE>& arr)
            {
                CONSTEXPR std::size_t size_root = ct_sqrt(SIZE);
                arr = std::array<T, SIZE> {};

                for(std::size_t ii = 0; ii < SIZE; ii += size_root + 1)
                {
                    arr[ii] = (T)1;
                }
            }
        template<class T, std::size_t SIZE>
            std::array<T, SIZE> makeArrayIdentity()
            {
                std::array<T, SIZE> ret;
                fillArrayIdentity(ret);
                return ret;
            }
#if __TARGET_PLATFORM__ == __ANDROID_OS__
#else
#endif
    }
}

/**
 * DEPRACATED.
 */
namespace aMath = aex::math;
#endif /* MATH_H_ */
