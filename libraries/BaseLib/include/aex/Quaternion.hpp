//
//  Quaternion.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 03.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#ifndef __aexolGL__Quaternion__
#define __aexolGL__Quaternion__

#include <aex/Math.hpp>
#include <aex/Vector3.hpp>
#include <aex/Matrix.hpp>
#include <iostream>
namespace aex
{
    namespace math{

        class Quaternion;
        class Vector3;
        class Matrix;

        /**
         *	@brief	Makes quaternion from 3 angles in radian
         *
         *	@param 	x 	x angle
         *	@param 	y 	y angle
         *	@param 	z 	z angle
         *
         *	@return	Quaternion
         */
        Quaternion AEX_PUBLIC_API getQuaternionFromEulerAnglesRad(double x, double y, double z);

        /**
         *	@brief	Makes quaternion from 3 angles in degrees
         *
         *	@param 	x 	x angle
         *	@param 	y 	y angle
         *	@param 	z 	z angle
         *
         *	@return	Quaternion
         */
        Quaternion AEX_PUBLIC_API getQuaternionFromEulerDeg(double x, double y, double z);

        /**
         *	@brief	Makes Quaternion from angle around axis in radians
         *
         *	@param 	v 	Vector3 ange
         *	@param 	fi 	angle
         *
         *	@return	Quaternion
         */
        Quaternion AEX_PUBLIC_API getQuaternionFromAxisAngle(const Vector3& v, double fi);

        /**
         *	@brief	Makes Quaternion from angle around axis in degrees
         *
         *	@param 	v 	Vector3 ange
         *	@param 	fi 	angle
         *
         *	@return	Quaternion
         */
        Quaternion AEX_PUBLIC_API getQuaternionFromAxisAngleDeg(const Vector3& v, double fi);

        /**
         *	@brief	Makes qaternion from 4x4 matrix
         *
         *	@param 	matrix 	Matrix 4x4 (example:lookAt)
         */
        Quaternion AEX_PUBLIC_API getQuaternionFromMatrix(const Matrix& matrix);

        /**
         *	@brief	Makes quaternion from spherical coordinates
         *
         *	@param 	latitude 	latitude coordinate
         *	@param 	longitude 	longitude coordinate
         *	@param 	angle 	angle aroud axis
         *
         *	@return	Quaternion
         */
        Quaternion AEX_PUBLIC_API getQuaternionFromSpherical(float latitude, float longitude, float angle);

        /**
         *	@brief	Makes idenity qaternion
         *
         *	@return	<#return value description#>
         */
        Quaternion AEX_PUBLIC_API getIdentityQuaternion();



        class AEX_PUBLIC_API Quaternion
        {
            static CONSTEXPR float NORMALIZE_EPSILON = 0.000001f;
            public:
            Quaternion(float x, float y, float z, float w, float epsilon = NORMALIZE_EPSILON);
            Quaternion();
            inline  Quaternion      operator-() const;
            inline  Quaternion      operator+(const Quaternion& quat) const;
            inline  Quaternion      operator*(const Quaternion& quat) const;
            inline  Quaternion      operator*(float q) const;
            inline  Quaternion      operator/(const Quaternion& quat) const;
            inline  Quaternion      operator/(float divisor) const;
            inline  Quaternion&     operator+=(const Quaternion& quat);
            inline  Quaternion&     operator*=(const Quaternion& quat);
            inline  Quaternion&     operator*=(float q);
            inline  Quaternion&     operator/=(const Quaternion& quat);
            inline  Quaternion&     operator/=(float divisor);
            inline  float&          operator[](const size_t index);
            inline  float           operator[](const size_t index) const;

            /**
             *	@brief	Rotate vector using quaterinon
             *
             *	@param 	vec 	vector to rotate
             *
             *	@return	rotated Vector3
             */
            inline  Vector3         rotateVector(const Vector3& vec) const;

            //inline  Vector3         rotatet(const Vector3 &vec) const;

            /**
             *	@brief	Returns squared magnitude of quaternion
             *
             *	@return	float
             */
            inline  float           magnitudeSquared() const;
            /**
             *	@brief	Returns magnitude of quaternion
             *
             *	@return	float
             */
            inline  float           magnitude() const;

            /**
             *	@brief	Returns inverse quaternion
             *
             *	@return	Quaternion
             */
            inline  Quaternion      inverse() const;

            /**
             *	@brief	Returns conjugate quaternion
             *
             *	@return	Quaternion
             */
            inline  Quaternion      conjugate() const;

            /**
             *	@brief	Returns negative quaternion
             *
             *	@return	Quaternion
             */
            inline  Quaternion      negative() const;

            /**
             *	@brief	Returns rotation matrix 4x4 from this quaternion
             *
             *	@return	Matrix
             */
            Matrix          getMatrix()	const;

            /**
             *	@brief	Get x
             *
             *	@return	float
             */
            float         getX()
            {
                return m_x;
            }

            /**
             *	@brief	Get y
             *
             *	@return	float
             */
            float         getY()
            {
                return m_y;
            }

            /**
             *	@brief	Get z
             *
             *	@return	float
             */
            float         getZ()
            {
                return m_z;
            }

            /**
             *	@brief	Get w
             *
             *	@return	float
             */
            float         getW()
            {
                return m_w;
            }

            /**
             * @brief Normalize quaternion.
             *
             * Results in this = (this.x/this.magnitude(), this.y/this.magnitude(), this.z/this.magnitude(), this.w/this.magnitude())
             */
            void normalize(float epsilon = NORMALIZE_EPSILON);
            Quaternion unit();

            Vector3 converseToEuler();

            friend float dot(Quaternion left, Quaternion right);
            private:
            float m_x;
            float m_y;
            float m_z;
            float m_w;
        };
        Quaternion
            Quaternion::operator-() const
            {
                return Quaternion(-m_x, -m_y, -m_z, -m_w);
            }
        Quaternion
            Quaternion::operator+(const Quaternion& quat) const
            {
                return Quaternion(m_x + quat.m_x, m_y + quat.m_y, m_z + quat.m_z, m_w + quat.m_w);
            }
        Quaternion
            Quaternion::operator*(const Quaternion& quat) const  //Return Quaternion*Quaternion
            {
                /*
                 * w_1*w_2 - x_1*x_2 - y_1*y_2 - z_1*z_2
                 * + (w_1*x_2 + x_1*w_2 + y_1*z_2 - z_1*y_2)i
                 * + (w_1*y_2 - x_1*z_2 + y_1*w_2 + z_1*x_2)j
                 * + (w_1*z_2 + x_1*y_2 - y_1*x_2 + z_1*w_2)k 
                 */
                double rx, ry, rz, rw;		// temp result

                rw	= quat.m_w * m_w - quat.m_x * m_x - quat.m_y * m_y - quat.m_z * m_z;

                rx	= quat.m_w * m_x + quat.m_x * m_w - quat.m_y * m_z + quat.m_z * m_y;
                ry	= quat.m_w * m_y + quat.m_y * m_w - quat.m_z * m_x + quat.m_x * m_z;
                rz	= quat.m_w * m_z + quat.m_z * m_w - quat.m_x * m_y + quat.m_y * m_x;

                return Quaternion((float)rx, (float)ry, (float)rz, (float)rw);
            }
        Quaternion
            Quaternion::operator*(float q) const
            {
                return Quaternion(m_x * q, m_y * q, m_z * q, m_w * q);
            }
        Quaternion
            Quaternion::operator/(const aex::math::Quaternion& quat) const
            {
                Quaternion ret;
                ret.m_w = (m_w * quat.m_w +
                        m_x * quat.m_x +
                        m_y * quat.m_y +
                        m_z * quat.m_z) / quat.magnitudeSquared();
                ret.m_x = (m_x * quat.m_w -
                        m_w * quat.m_x +
                        m_y * quat.m_z -
                        m_z * quat.m_y) / quat.magnitudeSquared();
                ret.m_y = (m_y * quat.m_w -
                        m_w * quat.m_y +
                        m_z * quat.m_x -
                        m_x * quat.m_z) / quat.magnitudeSquared();
                ret.m_z = (m_z * quat.m_w -
                        m_w * quat.m_z +
                        m_x * quat.m_y -
                        m_y * quat.m_x) / quat.magnitudeSquared();
                return ret;
            }
        Quaternion
            Quaternion::operator/(float divisor) const
            {
                return Quaternion(m_x / divisor, m_y / divisor, m_z / divisor, m_w / divisor);
            }
        Quaternion&
            Quaternion::operator+=(const Quaternion& quat)
            {
                Quaternion q = (*this) + quat;
                (*this) = q;
                return (*this);
            }
        Quaternion&
            Quaternion::operator*=(const Quaternion& quat)  //Return Quaternion*Quaternion
            {
                Quaternion q = (*this) * quat;
                (*this) = q;
                return (*this);
            }
        Quaternion&
            Quaternion::operator*=(float s)
            {
                Quaternion q = (*this) * s;
                (*this) = q;
                return (*this);
            }
        Quaternion&
            Quaternion::operator/=(const aex::math::Quaternion& quat)
            {
                Quaternion q = (*this) / quat;
                (*this) = q;
                return (*this);
            }
        Quaternion&
            Quaternion::operator/=(float divisor)
            {
                Quaternion q = (*this) / divisor;
                (*this) = q;
                return (*this);
            }

        Vector3
            Quaternion::rotateVector(const Vector3& vec) const
            {
                Quaternion qVector(vec.x, vec.y, vec.z, 0.0f);
                Quaternion mult = (*this) * qVector * conjugate();
                return Vector3(mult.m_x, mult.m_y, mult.m_z);
            }
        float
            Quaternion::magnitudeSquared() const
            {
                return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
            }
        float
            Quaternion::magnitude() const
            {
                return std::sqrt(magnitudeSquared());
            }
        Quaternion
            Quaternion::inverse() const
            {
                float magSquared = magnitudeSquared();
                return Quaternion((-m_x) / magSquared, (-m_y) / magSquared, (-m_z) / magSquared, m_w / magSquared);
            }
        Quaternion
            Quaternion::conjugate() const
            {
                return Quaternion(-m_x, -m_y, -m_z, m_w);
            }
        Quaternion
            Quaternion::negative() const
            {
                return Quaternion(-m_x, -m_y, -m_z, -m_w);
            }

        float&
            Quaternion::operator[](const size_t index)
            {
                return (&m_x)[index];
            }

        float
            Quaternion::operator[](const size_t index) const
            {
                return (&m_x)[index];
            }

        AEX_PUBLIC_API Quaternion getRotationTo(Vector3 from, Vector3 to);
        AEX_PUBLIC_API float dot(Quaternion left, Quaternion right);


    }
}

#endif /* defined(__aexolGL__Quaternion__) */
