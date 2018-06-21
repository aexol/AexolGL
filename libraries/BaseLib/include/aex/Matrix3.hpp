/*
 * Matrix3.h
 *
 *  Created on: Jul 1, 2013
 *      Author: aexol-1
 */

#ifndef MATRIX3_H_
#define MATRIX3_H_
#include <aex/Math.hpp>
#include <aex/Vector3.hpp>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
#include <initializer_list>

namespace aex
{ 
    namespace math{

        class Vector3;
        class AEX_PUBLIC_API Matrix3
        {
            public:
                /**
                 *	@brief	Default ctor
                 *
                 *	@return	makes idenity 3x3 matrix
                 */
                Matrix3();

                /**
                 *	@brief	Matrx from 3 Vector3. Vector3 as column or row(true:column,false:rows).
                 *
                 *	@param 	 u vector3
                 *	@param 	 v vector3
                 *	@param 	 w vector3
                 *	@param 	columns (default true)
                 *	@return	<#return value description#>
                 */
                Matrix3(Vector3 u, Vector3 v, Vector3 w, bool columns = true);

                /**
                 *	@brief	Ctor matrix from vector<float>
                 */
                Matrix3(const std::vector<float> mat);

                /**
                 *	@brief	Returns transposed matrix
                 */
                inline Matrix3 transpose() const;
                //JS engine has row-major structure, this one column-major, so don't transpose unless you want to use row-major
                inline Matrix3 operator-() const;
                inline Matrix3 operator*(const float a) const;                    //Matrix times scalar
                Vector3 operator*(const Vector3& vec) const;
                inline Matrix3 operator*(const Matrix3& mat) const;
                inline Matrix3 operator+(const Matrix3& mat) const;
                inline Matrix3 operator-(const Matrix3& mat) const;
                inline float operator[](const size_t index) const;
                inline float& operator[](const size_t index);
                /**
                 *	@brief	Check is matrix zero
                 *
                 *	@return	bool
                 */
                inline bool isZero(float epsilon = 0.00000001f) const;

                /**
                 *	@brief	Returns size of matrix
                 *
                 */
                inline int size(void) const;

                inline Matrix3 inverse(float epsilon = 0.00000001f);
                /**
                 *	@brief	Returns matrix determinant
                 */
                inline float determinant();

                /**
                 *	@brief	Returns matrix as vector<float>
                 */
                inline std::vector<float> getM()
                {
                    return m_mat;
                }

                virtual ~Matrix3();
            private:
                std::vector<float> m_mat;
        };
        Matrix3
            Matrix3::operator*(const float a) const
            {
                std::vector<float> mat;
                mat.reserve(9);
                mat[0] = m_mat[0] * a;
                mat[3] = m_mat[3] * a;
                mat[6] = m_mat[6] * a;
                mat[1] = m_mat[1] * a;
                mat[4] = m_mat[4] * a;
                mat[7] = m_mat[7] * a;
                mat[2] = m_mat[2] * a;
                mat[5] = m_mat[5] * a;
                mat[8] = m_mat[8] * a;
                return Matrix3(mat);
            }
        Matrix3
            Matrix3::operator*(const Matrix3& mat) const
            {
                std::vector<float> mat_ret;
                mat_ret.reserve(9);
                mat_ret[0] = m_mat[0] * mat.m_mat[0] + m_mat[1] * mat.m_mat[3] + m_mat[2] * m_mat[6];
                mat_ret[1] = m_mat[0] * mat.m_mat[1] + m_mat[1] * mat.m_mat[4] + m_mat[2] * m_mat[7];
                mat_ret[2] = m_mat[0] * mat.m_mat[2] + m_mat[1] * mat.m_mat[5] + m_mat[2] * m_mat[8];
                mat_ret[3] = m_mat[3] * mat.m_mat[0] + m_mat[4] * mat.m_mat[3] + m_mat[5] * m_mat[6];
                mat_ret[4] = m_mat[3] * mat.m_mat[1] + m_mat[4] * mat.m_mat[4] + m_mat[5] * m_mat[7];
                mat_ret[5] = m_mat[3] * mat.m_mat[2] + m_mat[4] * mat.m_mat[5] + m_mat[5] * m_mat[8];
                mat_ret[6] = m_mat[6] * mat.m_mat[0] + m_mat[7] * mat.m_mat[3] + m_mat[8] * m_mat[6];
                mat_ret[7] = m_mat[6] * mat.m_mat[1] + m_mat[7] * mat.m_mat[4] + m_mat[8] * m_mat[7];
                mat_ret[8] = m_mat[6] * mat.m_mat[2] + m_mat[7] * mat.m_mat[5] + m_mat[8] * m_mat[8];
                return Matrix3(mat_ret);
            }
        Matrix3
            Matrix3::operator+(const Matrix3& mat) const
            {
                std::vector<float> mat_ret;
                mat_ret.reserve(9);
                mat_ret[0] = m_mat[0] + mat.m_mat[0];
                mat_ret[3] = m_mat[3] + mat.m_mat[3];
                mat_ret[6] = m_mat[6] + mat.m_mat[6];
                mat_ret[1] = m_mat[1] + mat.m_mat[1];
                mat_ret[4] = m_mat[4] + mat.m_mat[4];
                mat_ret[7] = m_mat[7] + mat.m_mat[7];
                mat_ret[2] = m_mat[2] + mat.m_mat[2];
                mat_ret[5] = m_mat[5] + mat.m_mat[5];
                mat_ret[8] = m_mat[8] + mat.m_mat[8];
                return Matrix3(mat_ret);
            }
        Matrix3
            Matrix3::operator-(const Matrix3& mat) const
            {
                std::vector<float> mat_ret;
                mat_ret.reserve(9);
                mat_ret[0] = m_mat[0] - mat.m_mat[0];
                mat_ret[3] = m_mat[3] - mat.m_mat[3];
                mat_ret[6] = m_mat[6] - mat.m_mat[6];
                mat_ret[1] = m_mat[1] - mat.m_mat[1];
                mat_ret[4] = m_mat[4] - mat.m_mat[4];
                mat_ret[7] = m_mat[7] - mat.m_mat[7];
                mat_ret[2] = m_mat[2] - mat.m_mat[2];
                mat_ret[5] = m_mat[5] - mat.m_mat[5];
                mat_ret[8] = m_mat[8] - mat.m_mat[8];
                return Matrix3(mat_ret);
            }
        float&
            Matrix3::operator[](const size_t index)
            {
                return m_mat[index];
            }
        float
            Matrix3::operator[](const size_t index) const
            {
                return m_mat[index];
            }
        bool
            Matrix3::isZero(float epsilon) const
            {
                for(int i = 0; i < 9; i++)
                {
                    if(!Math::closeEnough(m_mat[i], 0.0f, epsilon))
                    {
                        return false;
                    }
                }

                return true;
            }
        int
            Matrix3::size(void) const
            {
                return 9;
            }
        Matrix3
            Matrix3::transpose() const
            {
                std::vector<float> ret = { m_mat[0], m_mat[3], m_mat[6], m_mat[1], m_mat[4], m_mat[7], m_mat[2], m_mat[5], m_mat[8] };
                return Matrix3(ret);
            }
        Matrix3
            Matrix3::inverse(float epsilon)
            {
                // Invert a 3x3 using cofactors.  This is faster than using a generic
                // Gaussian elimination because of the loop overhead of such a method.

                Matrix3 inverse;

                // Compute the adjoint.
                inverse.m_mat[0] = m_mat[4] * m_mat[8] - m_mat[5] * m_mat[7];
                inverse.m_mat[1] = m_mat[2] * m_mat[7] - m_mat[1] * m_mat[8];
                inverse.m_mat[2] = m_mat[1] * m_mat[5] - m_mat[2] * m_mat[4];
                inverse.m_mat[3] = m_mat[5] * m_mat[6] - m_mat[3] * m_mat[8];
                inverse.m_mat[4] = m_mat[0] * m_mat[8] - m_mat[2] * m_mat[6];
                inverse.m_mat[5] = m_mat[2] * m_mat[3] - m_mat[0] * m_mat[5];
                inverse.m_mat[6] = m_mat[3] * m_mat[7] - m_mat[4] * m_mat[6];
                inverse.m_mat[7] = m_mat[1] * m_mat[6] - m_mat[0] * m_mat[7];
                inverse.m_mat[8] = m_mat[0] * m_mat[4] - m_mat[1] * m_mat[3];

                float det = m_mat[0] * inverse.m_mat[0] + m_mat[1] * inverse.m_mat[3] + m_mat[2] * inverse.m_mat[6];

                if(fabs(det) > epsilon)
                {
                    float invDet = 1.0f / det;
                    inverse.m_mat[0] *= invDet;
                    inverse.m_mat[1] *= invDet;
                    inverse.m_mat[2] *= invDet;
                    inverse.m_mat[3] *= invDet;
                    inverse.m_mat[4] *= invDet;
                    inverse.m_mat[5] *= invDet;
                    inverse.m_mat[6] *= invDet;
                    inverse.m_mat[7] *= invDet;
                    inverse.m_mat[8] *= invDet;
                    return inverse;
                }

                return Matrix3();
            }
        float
            Matrix3::determinant()
            {
                Matrix3 inverse;

                inverse.m_mat[0] = m_mat[4] * m_mat[8] - m_mat[5] * m_mat[7];
                inverse.m_mat[1] = m_mat[2] * m_mat[7] - m_mat[1] * m_mat[8];
                inverse.m_mat[2] = m_mat[1] * m_mat[5] - m_mat[2] * m_mat[4];
                inverse.m_mat[3] = m_mat[5] * m_mat[6] - m_mat[3] * m_mat[8];
                inverse.m_mat[4] = m_mat[0] * m_mat[8] - m_mat[2] * m_mat[6];
                inverse.m_mat[5] = m_mat[2] * m_mat[3] - m_mat[0] * m_mat[5];
                inverse.m_mat[6] = m_mat[3] * m_mat[7] - m_mat[4] * m_mat[6];
                inverse.m_mat[7] = m_mat[1] * m_mat[6] - m_mat[0] * m_mat[7];
                inverse.m_mat[8] = m_mat[0] * m_mat[4] - m_mat[1] * m_mat[3];

                float det = m_mat[0] * inverse.m_mat[0] + m_mat[1] * inverse.m_mat[3] + m_mat[2] * inverse.m_mat[6];

                return det;
            }
    }
}
#endif /* MATRIX3_H_ */
