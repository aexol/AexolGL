/*
 * Vector2.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: aexol-1
 */

#include <aex/Vector2.hpp>
namespace aex {
    namespace math {
        Vector2::Vector2() : x(0.0f), y(0.0f)
        {
        }

        Vector2::Vector2(float tx, float ty) : x(tx), y(ty)
        {

        }

        Vector2::Vector2(const Vector2& vec) : x(vec.x), y(vec.y)
        {

        }

        Vector2::~Vector2()
        {
            // TODO Auto-generated destructor stub
        }

        Vector2 Vector2::rotate(float angle)
        {

            float radAngle = aex::math::Math::DEG2RAD(angle);
            float cs = cos(radAngle);
            float sn = sin(radAngle);

            return Vector2(x * cs - y * sn, x * sn + y * cs);

        }

        Vector2 lerpf(const Vector2& a, const Vector2& b, const float& fraction)
        {

            return (a * (1.0f - fraction)) + (b * fraction);
        }

        Vector2 abs(const Vector2& v1)
        {
            return v1.abs();
        }

        float dot(const Vector2& v1, const Vector2& v2)
        {
            return v1.dot(v2);
        }

        float cross(const Vector2& v1, const Vector2& v2)
        {
            return v1.cross(v2);
        }

        float length(const Vector2& v1)
        {
            return v1.length();
        }

        Vector2 unit(const Vector2& v1)
        {
            return v1.unit();
        }
    } /* math */
} /* aex */
