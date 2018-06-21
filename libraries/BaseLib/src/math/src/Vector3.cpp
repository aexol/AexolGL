/*
 * Vector3.cpp
 *
 *  Created on: Jul 1, 2013
 *      Author: aexol-1
 */

#include <aex/Vector3.hpp>
#include <aex/Matrix3.hpp>
#include <aex/Quaternion.hpp>
#include <cmath>

namespace aex {
    namespace math {
        Vector3::Vector3() :
            x(0.0f),
            y(0.0f),
            z(0.0f)
        {

        }

        Vector3::Vector3(float f) :
            x(f),
            y(f),
            z(f)
        {

        }

        Vector3::Vector3(const float tx, const float ty, const float tz) :
            x(tx),
            y(ty),
            z(tz)
        {

        }

        Vector3::Vector3(const Vector3& vec) :
            x(vec.x),
            y(vec.y),
            z(vec.z)
        {

        }

        Vector3::Vector3(const Vector2& vec) :
            x(vec.x),
            y(0.0f),
            z(vec.y)
        {

        }

        Vector3::Vector3(const float* const arr, size_t offset, size_t size)
        {
            if(size > 3)
            {
                size = 3;
            }

            x = 0.0f;
            y = 0.0f;
            z = 0.0f;

            if(size == 0)
                if(arr != NULL)
                {
                    size_t maxSize = sizeof(arr) / sizeof(float);

                    if(offset + size <= maxSize)
                    {
                        for(size_t i = offset; i < offset + size; i++)
                        {
                            (&x)[i] = arr[i];
                        }
                    }
                }
        }

        Quaternion
            Vector3::getRotationTo(Vector3 v)
            {
                Vector3& v_this = (*this);
                Vector3 v0 = v_this / v_this.length();
                Vector3 v1 = v / v.length();
                Vector3 cVec = v0 * v1;
                float d = v0 % v1;
                Quaternion quaternion;
                float divisor = 1 / sqrt(2 * (1 + d));

                if(Math::closeEnough(d, 0.0f))
                {
                    quaternion = getIdentityQuaternion(); //TO-DO action to take when 180 deg turn
                }
                else if(fabs(d) < 1.0f - Math::EPSILON)
                {
                    quaternion = Quaternion((float)(cVec.x * divisor), (float)(cVec.y * divisor), (float)(cVec.z * divisor), (float)(sqrt((1 + d) / 2)));
                }
                else
                {
                    quaternion = getIdentityQuaternion();
                }

                return quaternion;
            }

        Vector3::~Vector3()
        {
            // TODO Auto-generated destructor stub
        }

        Vector3
            Vector3::operator*(const Matrix3& mat) const   //            -         -
            {
                float t_x = x * mat[0] + y * mat[3] + z * mat[6]; //           | a   d   g |
                float t_y = x * mat[1] + y * mat[4] + z * mat[7]; //[ x y z] * | b   e   h | = [ x*a+y*b+z*c x*d+y*e+z*f g*x+h*y+*i*z ]
                float t_z = x * mat[2] + y * mat[5] + z * mat[8]; //           | c   f   i |
                return Vector3(t_x, t_y, t_z); //            -         -         -         -
            }

        void
            Vector3::operator*=(const Matrix3& mat)
            {
                float t_x = x * mat[0] + y * mat[3] + z * mat[6];
                float t_y = x * mat[0] + y * mat[3] + z * mat[6];
                float t_z = x * mat[2] + y * mat[5] + z * mat[8];
                x = t_x;
                y = t_y;
                z = t_z;
            }

        /*float dot(const Vector3& a , const Vector3& b)
          {
          return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
          }*/

        /**
         * @param a
         * @param b
         * @return
         */
        Vector3 min(const Vector3& a, const Vector3& b)
        {
            return Vector3(Math::min(a.x, b.x), Math::min(a.y, b.y), Math::min(a.z, b.z));
        }

        /**
         * @param a
         * @param b
         * @return
         */
        Vector3 max(Vector3 a, Vector3 b)
        {
            return Vector3(Math::max(a.x, b.x), Math::max(a.y, b.y), Math::max(a.z, b.z));
        }

        /**
         * @param a
         * @param b
         * @param fraction
         * @return
         */
        Vector3 lerpV3(const Vector3& a, const Vector3& b, const Vector3& fraction)
        {
            return a + (b - a) * fraction;
        }

        Vector3 lerpf(const Vector3& a, const Vector3& b, const float& fraction)
        {
            return (a * (1.0f - fraction)) + (b * fraction);
        }

        Vector3 cross(const Vector3& a, const Vector3& b)
        {
            return Vector3((a.y * b.z) - (a.z * b.y), (a.z * b.x) -( a.x * b.z), (a.x * b.y) -( a.y * b.x));
        }

        float dot(const Vector3& a, const Vector3& b)
        {
            return a.dot(b);
        }

        float length(const Vector3& a)
        {
            return a.length();
        }

        Vector3 negate(const Vector3& a)
        {
            return a*(-1.0);

        }
        Vector3 unit(const Vector3& a)
        {
            return a.unit();
        }

        Vector3 fromArray(const std::vector<float>& vec, size_t begin, size_t end)
        {
            return Vector3(vec[begin], vec[begin + 1], vec[begin + 2]);
        }

        Vector3 angleCentroidWeight(const Vector3& sharedVec, const Vector3& v1, const Vector3& v2, const Vector3& faceNorm)
        {
            float centerX = (v1.x + v2.x + sharedVec.x) / 3.0f;
            float centerY = (v1.y + v2.y + sharedVec.y) / 3.0f;
            float centerZ = (v1.z + v2.z + sharedVec.z) / 3.0f;

            Vector3 centroid(centerX, centerY, centerZ);

            Vector3 c1 = v1 - sharedVec;
            Vector3 c2 = v2 - sharedVec;

            float angle = acosf((c1.dot(c2)) / (c1.length() * c2.length()));

            float w = angle / ((centroid - sharedVec).length());

            return faceNorm * w;
        }

    } /* math */
} /* aex */
