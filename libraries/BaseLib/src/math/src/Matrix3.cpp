/*
 * Matrix3.cpp
 *
 *  Created on: Jul 1, 2013
 *      Author: aexol-1
 */

#include <aex/Matrix3.hpp>
#include <aex/Vector3.hpp>
namespace aex {
    namespace math {
        Vector3
            Matrix3::operator*(const Vector3& vec) const
            {
                float x = m_mat[0] * vec.x + m_mat[1] * vec.y + m_mat[2] * vec.z;
                float y = m_mat[3] * vec.x + m_mat[4] * vec.y + m_mat[5] * vec.z;
                float z = m_mat[6] * vec.x + m_mat[7] * vec.y + m_mat[8] * vec.z;
                return Vector3(x, y, z);
            }

        Matrix3::Matrix3() : m_mat(9)
        {
            m_mat[0] = 1.0f;
            m_mat[3] = 0.0f;
            m_mat[6] = 0.0f;
            m_mat[1] = 0.0f;
            m_mat[4] = 1.0f;
            m_mat[7] = 0.0f;
            m_mat[2] = 0.0f;
            m_mat[5] = 0.0f;
            m_mat[8] = 1.0f;
        }
        Matrix3::Matrix3(Vector3 u, Vector3 v, Vector3 w, bool columns) : m_mat(9)
        {
            if(columns)
            {
                m_mat[0] = u.x;
                m_mat[1] = v.x;
                m_mat[2] = w.x;
                m_mat[3] = u.y;
                m_mat[4] = v.y;
                m_mat[5] = w.y;
                m_mat[6] = u.z;
                m_mat[7] = v.z;
                m_mat[8] = w.z;
            }
            else
            {
                m_mat[0] = u.x;
                m_mat[1] = u.y;
                m_mat[2] = u.z;
                m_mat[3] = v.x;
                m_mat[4] = v.y;
                m_mat[5] = v.z;
                m_mat[6] = w.x;
                m_mat[7] = w.y;
                m_mat[8] = w.z;
            }
        }
        Matrix3::Matrix3(const std::vector<float> mat)
        {
            if(m_mat.size() != 9)
            {
                m_mat.reserve(9);
            }

            if(mat.size() < 9)
            {
                std::copy(mat.begin(), mat.end(), m_mat.begin());
            }
            else
            {
                std::copy(mat.begin(), mat.begin() + 9 , m_mat.begin());
            }
        }
        Matrix3::~Matrix3()
        {
            // TODO Auto-generated destructor stub
        }
    } /* math */
} /* aex */
