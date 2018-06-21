//
//  Matrix.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 02.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//
#include <aex/Quaternion.hpp>
#include <aex/Matrix.hpp>
#include <algorithm>

namespace aex {
    namespace math {
        Matrix::Matrix()
        {
            m_mat = Math::identity4();
        }

        template<class... Args, typename>
            Matrix::Matrix(Args&& ... args) : m_mat
        {
            {
                aex::forward<Args>(args)...
            }
        }
        {
        }

        Matrix::Matrix(const std::array<float, 16>& mat)
        {
            m_mat = mat;
        }

        Matrix::Matrix(std::array<float, 16>&&  mat)
        {
            m_mat = std::move(mat);
        }

        Vector3 Matrix::transformPoint(const Vector3& vec, float w) const
        {
            return Vector3(
                    m_mat[0] * vec.x + m_mat[1] * vec.y + m_mat[2]  * vec.z + w * m_mat[3],
                    m_mat[4] * vec.x + m_mat[5] * vec.y + m_mat[6]  * vec.z + w * m_mat[7],
                    m_mat[8] * vec.x + m_mat[9] * vec.y + m_mat[10] * vec.z + w * m_mat[11]
                    ) / (m_mat[12] * vec.x + m_mat[13] * vec.y + m_mat[14] * vec.z + w * m_mat[15]);
        }

        Vector3 Matrix::transformVector(const Vector3& vec, float w) const
        {
            return Vector3(
                    m_mat[0] * vec.x + m_mat[1] * vec.y + m_mat[2]  * vec.z + w * m_mat[3] ,
                    m_mat[4] * vec.x + m_mat[5] * vec.y + m_mat[6]  * vec.z + w * m_mat[7],
                    m_mat[8] * vec.x + m_mat[9] * vec.y + m_mat[10] * vec.z + w * m_mat[11]);
        }

        Matrix perspective(const float fov, const float aspect, const float near, const float far)
        {
            //float y = (float) (Math.tan(fov * Math.PI / 360) * near);
            //float x = y * aspect;
            float f = 1.0f / tan(fov * Math::V_DEG2RADDIV2);
            float nearLessFar = (near - far);
            std::array<float, 16> perspective_mat
            {
                {
                    f / aspect, 0.0f, 0.0f, 0.0f,
                      0.0f, f, 0.0f, 0.0f,
                      0.0f, 0.0f, (near + far) / nearLessFar, (2.0f * near * far) / nearLessFar,
                      0.0f, 0.0f, -1.0f, 0.0f
                }
            };
            //        perspective_mat[0]          = f/aspect;
            //        perspective_mat[5]          = f;
            //        perspective_mat[10]         = (near+far)/nearLessFar;
            //        perspective_mat[11]         = (2*near*far)/nearLessFar;
            return Matrix(std::move(perspective_mat));
        }

        Matrix identity()
        {
            return Matrix(Math::identity4());
        }

        Matrix zero()
        {
            std::array<float, 16> zero //Completly unnecessary but wth.
            {
                {
                    0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f
                }
            };
            return Matrix(std::move(zero));
        }

        Matrix frustum(float l, float r, float b, float t, float n, float f)
        {
            std::array<float, 16> frustum
            {
                {
                    2.0f * n / (r - l), 0.0f, (r + l) / (r - l), 0.0f,
                        0.0f, 2.0f * n / (t - b), (t + b) / (t - b), 0.0f,
                        0.0f, 0.0f, -(f + n) / (f - n), -2.0f * f* n / (f - n),
                        0.0f, 0.0f, -1.0f, 0.0f
                }
            };
            //        frustum[0]  = 2.0f * n / ( r - l );
            //        frustum[2]  = ( r + l )/( r - l );
            //        frustum[5]  = 2.0f * n / ( t - b );
            //        frustum[6]  = (t + b) / ( t - b );
            //        frustum[10] = -( f + n ) / ( f - n );
            //        frustum[11] = -2.0f * f * n / ( f - n );
            //        frustum[14] = -1.0f;
            return Matrix(std::move(frustum));
        }

        Matrix ortho(float l, float r, float b, float t, float n, float f)
        {
            std::array<float, 16> ortho
            {
                {
                    2.0f / (r - l), 0.0f, 0.0f, (r + l) / (r - l),
                        0.0f, 2.0f / (t - b), 0.0f, (t + b) / (t - b),
                        0.0f, 0.0f, -2.0f / (f - n), (f + n) / (f - n),
                        0.0f, 0.0f, 0.0f, 1.0f
                }
            };
            //        ortho[0]  = 2.0f / ( r - l );
            //        ortho[3] = ( r + l ) / ( r - l );
            //        ortho[5]  = 2.0f / ( t - b );
            //        ortho[7] = ( t + b ) / ( t - b );
            //        ortho[10] = -2.0f / ( f - n );
            //        ortho[11] = ( f + n ) / ( f - n );
            //        ortho[12] = 0.0f;
            //        ortho[13] = 0.0f;
            //        ortho[14] = 0.0f;
            //        ortho[15] = 1.0f;
            return Matrix(std::move(ortho));
        }

        Matrix scale(Vector3 vec)
        {
            return scale(vec.x, vec.y, vec.z);
        }

        Matrix scale(float x, float y, float z)
        {
            std::array<float, 16> scale_matrix
            {
                {
                    x, 0.0f, 0.0f, 0.0f,
                        0.0f, y, 0.0f, 0.0f,
                        0.0f, 0.0f, z, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f
                }
            };
            //
            //        scale_matrix[0]  = x;                   //Scale on X Axis
            //        scale_matrix[5]  = y;                   //Scale on Y Axis
            //        scale_matrix[10] = z;                   //Scale on Z Axis
            //        scale_matrix[15] = 1.0f;
            //
            return Matrix(std::move(scale_matrix));
        }

        Matrix translate(Vector3 vec)
        {
            return translate(vec.x, vec.y, vec.z);
        }

        Matrix translate(float x, float y, float z)
        {
            std::array<float, 16> translate =
            {
                {
                    1.0f, 0.0f, 0.0f, x,
                    0.0f, 1.0f, 0.0f, y,
                    0.0f, 0.0f, 1.0f, z,
                    0.0f, 0.0f, 0.0f, 1.0f
                }
            };
            //
            //        translate[3] = x;                  //Translate on X Axis
            //        translate[7] = y;                  //Translate on Y Axis
            //        translate[11] = z;                  //Translate on Z Axis
            //
            return Matrix(std::move(translate));
        }

        Matrix rotate(float a, Vector3 vec)
        {
            return rotate(a, vec.x, vec.y, vec.z);
        }

        Matrix rotate(float a, float x, float y, float z)
        {
            if(a != 0.0f && (x != 0.0f || y != 0.0f || z != 0.0f))
            {
                float d = sqrt(x * x + y * y + z * z);
                a *= Math::V_DEG2RAD;
                x /= d;
                y /= d;
                z /= d;
                float c = Math::fast_cosine_precise(a); // Changed from cosine
                float s = Math::fast_sine_precise(a); //Changed from sine
                //float c = Math::fast_cosine(a); // Changed from cosine
                //float s = Math::fast_sine(a);  //Changed from sine
                float t = 1 - c;
                std::array<float, 16> rotate =
                {
                    {
                        x* x* t + c, x* y* t - z * s, x* z* t + y * s, 0.0f,
                        y* x* t + z * s, y* y* t + c, y* z* t - x * s, 0.0f,
                        z* x* t - y * s, z* y* t + x * s, z* z* t + c, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f
                    }
                };

                //            rotate[0]  = x * x * t + c;
                //            rotate[1]  = x * y * t - z * s;
                //            rotate[2]  = x * z * t + y * s;
                //
                //            rotate[4]  = y * x * t + z * s;
                //            rotate[5]  = y * y * t + c;
                //            rotate[6]  = y * z * t - x * s;
                //
                //            rotate[8]  = z * x * t - y * s;
                //            rotate[9]  = z * y * t + x * s;
                //            rotate[10] = z * z * t + c;

                Matrix mat(std::move(rotate));

                return mat;
            }
            else
            {
                return Matrix();
            }
        }

        Matrix rotateAngles(Vector3 vec)
        {
            return rotateAngles(vec.x, vec.y, vec.z);
        }

        Matrix rotateAngles(float x, float y, float z)
        {
            Matrix ret = rotate( x,  1.0f,  0.0f, 0.0f);
            ret *= rotate( y,  0.0f,  1.0f, 0.0f);
            ret *= rotate( x,  0.0f,  0.0f, 1.0f);
            return ret;
        }

        Vector3 rotateVector(Vector3 vec, float x, float y, float z)
        {
            return rotateAngles( x,  y , z).transformPoint(vec);
        }

        Vector3 rotateVector(Vector3 vec, Vector3 angle)
        {
            return rotateVector( vec, angle.x, angle.y, angle.z);
        }

        Matrix translateToOtherPlane(Vector3 current, Vector3 target)
        {
            Vector3 crossPlane = current.cross(target);
            float rotationAngle = acos(dot(current, target));
            return  rotate(rotationAngle * Math::V_RAD2DEG, crossPlane.x, crossPlane.y, crossPlane.z);
        }

        Matrix lookAt(const Vector3& e, const Vector3& c, const Vector3& u)
        {
            const Vector3 f = (c - e).unit();
            const Vector3 s = f * u.unit();
            const Vector3 t = s.unit() * f;
            const Vector3 neg_e = -e;
            std::array<float, 16> lookAt
            {
                {
                    s.x,    s.y,    s.z,    s % neg_e,
                        t.x,    t.y,    t.z,    t % neg_e,
                        -f.x,   -f.y,   -f.z,   f % e,
                        0.0f,   0.0f,   0.0f,   1.0f
                }
            };

            return Matrix(std::move(lookAt));;
        }

        void Matrix::translateRotateScale(float tx, float ty, float tz, const Quaternion& q, float sx, float sy, float sz)
        {
            translate(tx, ty, tz);
            rotate(q);
            scale(sx, sy, sz);
        }

        void Matrix::translate(float x, float y, float z)
        {
            m_mat[3] += x * m_mat[0] + y * m_mat[1] + z * m_mat[2];
            m_mat[7] += x * m_mat[4] + y * m_mat[5] + z * m_mat[6];
            m_mat[11] += x * m_mat[8] + y * m_mat[9] + z * m_mat[10];
            m_mat[15] += x * m_mat[12] + y * m_mat[13] + z * m_mat[14];
        }

        void Matrix::rotate(const Quaternion& q)
        {
            (*this) *= q.getMatrix();
        }

        void Matrix::scale(float x, float y, float z)
        {
            m_mat[0] *= x;
            m_mat[1] *= y;
            m_mat[2] *= z;
            m_mat[4] *= x;
            m_mat[5] *= y;
            m_mat[6] *= z;
            m_mat[8] *= x;
            m_mat[9] *= y;
            m_mat[10] *= z;
            m_mat[12] *= x;
            m_mat[13] *= y;
            m_mat[14] *= z;
        }

        Matrix lookAt(const float ex, const float ey, const float ez, const float tx, const float ty, const float tz, const float ux, const float uy, const float uz)
        {
            Vector3 e(ex, ey, ez);
            Vector3 c(tx, ty, tz);
            Vector3 u(ux, uy, uz);
            Vector3 f = (e - c).unit();
            Vector3 s = (u * f).unit();
            Vector3 t = (f * s).unit();
            std::array<float, 16> lookAt //Completly unnecessary but wth.
            {
                {
                    s.x, s.y, s.z, (s % e),
                        t.x, t.y, t.z, -(t % e),
                        f.x, f.y, f.z, -(f % e),
                        0.0f, 0.0f, 0.0f, 1.0f
                }
            };

            Matrix mat(std::move(lookAt));

            return mat;
        }


        Matrix normalMatrix( const Matrix& model, Matrix view)
        {
            view[3]=view[7]=view[11]=0.0f;
            view[15]=1.0f;
            Matrix ret = view * model;

            return ret.inverse().transpose();
        }

        Matrix Matrix::operator*(const Matrix& mat) const
        {
            const Matrix& A = *this;
            const Matrix& B = mat;
            Matrix AB;
            AB(0, 0) = A(0, 0)* B(0, 0) + A(0, 1)* B(1, 0) + A(0, 2) * B(2, 0) + A(0, 3) * B(3, 0);
            AB(0, 1) = A(0, 0)* B(0, 1) + A(0, 1)* B(1, 1) + A(0, 2) * B(2, 1) + A(0, 3) * B(3, 1);
            AB(0, 2) = A(0, 0)* B(0, 2) + A(0, 1)* B(1, 2) + A(0, 2) * B(2, 2) + A(0, 3) * B(3, 2);
            AB(0, 3) = A(0, 0)* B(0, 3) + A(0, 1)* B(1, 3) + A(0, 2) * B(2, 3) + A(0, 3) * B(3, 3);

            AB(1, 0) = A(1, 0)* B(0, 0) + A(1, 1)* B(1, 0) + A(1, 2) * B(2, 0) + A(1, 3) * B(3, 0);
            AB(1, 1) = A(1, 0)* B(0, 1) + A(1, 1)* B(1, 1) + A(1, 2) * B(2, 1) + A(1, 3) * B(3, 1);
            AB(1, 2) = A(1, 0)* B(0, 2) + A(1, 1)* B(1, 2) + A(1, 2) * B(2, 2) + A(1, 3) * B(3, 2);
            AB(1, 3) = A(1, 0)* B(0, 3) + A(1, 1)* B(1, 3) + A(1, 2) * B(2, 3) + A(1, 3) * B(3, 3);

            AB(2, 0) = A(2, 0)* B(0, 0) + A(2, 1)* B(1, 0) + A(2, 2) * B(2, 0) + A(2, 3) * B(3, 0);
            AB(2, 1) = A(2, 0)* B(0, 1) + A(2, 1)* B(1, 1) + A(2, 2) * B(2, 1) + A(2, 3) * B(3, 1);
            AB(2, 2) = A(2, 0)* B(0, 2) + A(2, 1)* B(1, 2) + A(2, 2) * B(2, 2) + A(2, 3) * B(3, 2);
            AB(2, 3) = A(2, 0)* B(0, 3) + A(2, 1)* B(1, 3) + A(2, 2) * B(2, 3) + A(2, 3) * B(3, 3);

            AB(3, 0) = A(3, 0)* B(0, 0) + A(3, 1)* B(1, 0) + A(3, 2) * B(2, 0) + A(3, 3) * B(3, 0);
            AB(3, 1) = A(3, 0)* B(0, 1) + A(3, 1)* B(1, 1) + A(3, 2) * B(2, 1) + A(3, 3) * B(3, 1);
            AB(3, 2) = A(3, 0)* B(0, 2) + A(3, 1)* B(1, 2) + A(3, 2) * B(2, 2) + A(3, 3) * B(3, 2);
            AB(3, 3) = A(3, 0)* B(0, 3) + A(3, 1)* B(1, 3) + A(3, 2) * B(2, 3) + A(3, 3) * B(3, 3);
            return Matrix(AB);
        }

        Matrix& Matrix::operator*=(const Matrix& mat)
        {
            const Matrix& A = *this;
            const Matrix& B = mat;
            Matrix AB;
            AB(0, 0) = A(0, 0)* B(0, 0) + A(0, 1)* B(1, 0) + A(0, 2) * B(2, 0) + A(0, 3) * B(3, 0);
            AB(0, 1) = A(0, 0)* B(0, 1) + A(0, 1)* B(1, 1) + A(0, 2) * B(2, 1) + A(0, 3) * B(3, 1);
            AB(0, 2) = A(0, 0)* B(0, 2) + A(0, 1)* B(1, 2) + A(0, 2) * B(2, 2) + A(0, 3) * B(3, 2);
            AB(0, 3) = A(0, 0)* B(0, 3) + A(0, 1)* B(1, 3) + A(0, 2) * B(2, 3) + A(0, 3) * B(3, 3);

            AB(1, 0) = A(1, 0)* B(0, 0) + A(1, 1)* B(1, 0) + A(1, 2) * B(2, 0) + A(1, 3) * B(3, 0);
            AB(1, 1) = A(1, 0)* B(0, 1) + A(1, 1)* B(1, 1) + A(1, 2) * B(2, 1) + A(1, 3) * B(3, 1);
            AB(1, 2) = A(1, 0)* B(0, 2) + A(1, 1)* B(1, 2) + A(1, 2) * B(2, 2) + A(1, 3) * B(3, 2);
            AB(1, 3) = A(1, 0)* B(0, 3) + A(1, 1)* B(1, 3) + A(1, 2) * B(2, 3) + A(1, 3) * B(3, 3);

            AB(2, 0) = A(2, 0)* B(0, 0) + A(2, 1)* B(1, 0) + A(2, 2) * B(2, 0) + A(2, 3) * B(3, 0);
            AB(2, 1) = A(2, 0)* B(0, 1) + A(2, 1)* B(1, 1) + A(2, 2) * B(2, 1) + A(2, 3) * B(3, 1);
            AB(2, 2) = A(2, 0)* B(0, 2) + A(2, 1)* B(1, 2) + A(2, 2) * B(2, 2) + A(2, 3) * B(3, 2);
            AB(2, 3) = A(2, 0)* B(0, 3) + A(2, 1)* B(1, 3) + A(2, 2) * B(2, 3) + A(2, 3) * B(3, 3);

            AB(3, 0) = A(3, 0)* B(0, 0) + A(3, 1)* B(1, 0) + A(3, 2) * B(2, 0) + A(3, 3) * B(3, 0);
            AB(3, 1) = A(3, 0)* B(0, 1) + A(3, 1)* B(1, 1) + A(3, 2) * B(2, 1) + A(3, 3) * B(3, 1);
            AB(3, 2) = A(3, 0)* B(0, 2) + A(3, 1)* B(1, 2) + A(3, 2) * B(2, 2) + A(3, 3) * B(3, 2);
            AB(3, 3) = A(3, 0)* B(0, 3) + A(3, 1)* B(1, 3) + A(3, 2) * B(2, 3) + A(3, 3) * B(3, 3);

            *this = std::move(AB);

            return (*this);
        }
        Matrix Matrix::inverse() const
        {
            std::array<float, 16> mat;

            mat[0] = m_mat[5] * m_mat[10] * m_mat[15] -
                m_mat[5] * m_mat[14] * m_mat[11] -
                m_mat[6] * m_mat[9] * m_mat[15] +
                m_mat[6] * m_mat[13] * m_mat[11] +
                m_mat[7] * m_mat[9] * m_mat[14] -
                m_mat[7] * m_mat[13] * m_mat[10];

            mat[1] = -m_mat[1] * m_mat[10] * m_mat[15] +
                m_mat[1] * m_mat[14] * m_mat[11] +
                m_mat[2] * m_mat[9] * m_mat[15] -
                m_mat[2] * m_mat[13] * m_mat[11] -
                m_mat[3] * m_mat[9] * m_mat[14] +
                m_mat[3] * m_mat[13] * m_mat[10];

            mat[2] = m_mat[1] * m_mat[6] * m_mat[15] -
                m_mat[1] * m_mat[14] * m_mat[7] -
                m_mat[2] * m_mat[5] * m_mat[15] +
                m_mat[2] * m_mat[13] * m_mat[7] +
                m_mat[3] * m_mat[5] * m_mat[14] -
                m_mat[3] * m_mat[13] * m_mat[6];

            mat[3] = -m_mat[1] * m_mat[6] * m_mat[11] +
                m_mat[1] * m_mat[10] * m_mat[7] +
                m_mat[2] * m_mat[5] * m_mat[11] -
                m_mat[2] * m_mat[9] * m_mat[7] -
                m_mat[3] * m_mat[5] * m_mat[10] +
                m_mat[3] * m_mat[9] * m_mat[6];

            mat[4] = -m_mat[4] * m_mat[10] * m_mat[15] +
                m_mat[4] * m_mat[14] * m_mat[11] +
                m_mat[6] * m_mat[8] * m_mat[15] -
                m_mat[6] * m_mat[12] * m_mat[11] -
                m_mat[7] * m_mat[8] * m_mat[14] +
                m_mat[7] * m_mat[12] * m_mat[10];

            mat[5] = m_mat[0] * m_mat[10] * m_mat[15] -
                m_mat[0] * m_mat[14] * m_mat[11] -
                m_mat[2] * m_mat[8] * m_mat[15] +
                m_mat[2] * m_mat[12] * m_mat[11] +
                m_mat[3] * m_mat[8] * m_mat[14] -
                m_mat[3] * m_mat[12] * m_mat[10];

            mat[6] = -m_mat[0] * m_mat[6] * m_mat[15] +
                m_mat[0] * m_mat[14] * m_mat[7] +
                m_mat[2] * m_mat[4] * m_mat[15] -
                m_mat[2] * m_mat[12] * m_mat[7] -
                m_mat[3] * m_mat[4] * m_mat[14] +
                m_mat[3] * m_mat[12] * m_mat[6];

            mat[7] = m_mat[0] * m_mat[6] * m_mat[11] -
                m_mat[0] * m_mat[10] * m_mat[7] -
                m_mat[2] * m_mat[4] * m_mat[11] +
                m_mat[2] * m_mat[8] * m_mat[7] +
                m_mat[3] * m_mat[4] * m_mat[10] -
                m_mat[3] * m_mat[8] * m_mat[6];

            mat[8] = m_mat[4] * m_mat[9] * m_mat[15] -
                m_mat[4] * m_mat[13] * m_mat[11] -
                m_mat[5] * m_mat[8] * m_mat[15] +
                m_mat[5] * m_mat[12] * m_mat[11] +
                m_mat[7] * m_mat[8] * m_mat[13] -
                m_mat[7] * m_mat[12] * m_mat[9];

            mat[9] = -m_mat[0] * m_mat[9] * m_mat[15] +
                m_mat[0] * m_mat[13] * m_mat[11] +
                m_mat[1] * m_mat[8] * m_mat[15] -
                m_mat[1] * m_mat[12] * m_mat[11] -
                m_mat[3] * m_mat[8] * m_mat[13] +
                m_mat[3] * m_mat[12] * m_mat[9];

            mat[10] = m_mat[0] * m_mat[5] * m_mat[15] -
                m_mat[0] * m_mat[13] * m_mat[7] -
                m_mat[1] * m_mat[4] * m_mat[15] +
                m_mat[1] * m_mat[12] * m_mat[7] +
                m_mat[3] * m_mat[4] * m_mat[13] -
                m_mat[3] * m_mat[12] * m_mat[5];

            mat[11] = -m_mat[0] * m_mat[5] * m_mat[11] +
                m_mat[0] * m_mat[9] * m_mat[7] +
                m_mat[1] * m_mat[4] * m_mat[11] -
                m_mat[1] * m_mat[8] * m_mat[7] -
                m_mat[3] * m_mat[4] * m_mat[9] +
                m_mat[3] * m_mat[8] * m_mat[5];

            mat[12] = -m_mat[4] * m_mat[9] * m_mat[14] +
                m_mat[4] * m_mat[13] * m_mat[10] +
                m_mat[5] * m_mat[8] * m_mat[14] -
                m_mat[5] * m_mat[12] * m_mat[10] -
                m_mat[6] * m_mat[8] * m_mat[13] +
                m_mat[6] * m_mat[12] * m_mat[9];

            mat[13] = m_mat[0] * m_mat[9] * m_mat[14] -
                m_mat[0] * m_mat[13] * m_mat[10] -
                m_mat[1] * m_mat[8] * m_mat[14] +
                m_mat[1] * m_mat[12] * m_mat[10] +
                m_mat[2] * m_mat[8] * m_mat[13] -
                m_mat[2] * m_mat[12] * m_mat[9];

            mat[14] = -m_mat[0] * m_mat[5] * m_mat[14] +
                m_mat[0] * m_mat[13] * m_mat[6] +
                m_mat[1] * m_mat[4] * m_mat[14] -
                m_mat[1] * m_mat[12] * m_mat[6] -
                m_mat[2] * m_mat[4] * m_mat[13] +
                m_mat[2] * m_mat[12] * m_mat[5];

            mat[15] = m_mat[0] * m_mat[5] * m_mat[10] -
                m_mat[0] * m_mat[9] * m_mat[6] -
                m_mat[1] * m_mat[4] * m_mat[10] +
                m_mat[1] * m_mat[8] * m_mat[6] +
                m_mat[2] * m_mat[4] * m_mat[9] -
                m_mat[2] * m_mat[8] * m_mat[5];

            Matrix inv(std::move(mat));

            float det = m_mat[0] * inv.m_mat[0] + m_mat[1] * inv.m_mat[4] + m_mat[2] * inv.m_mat[8] + m_mat[3] * inv.m_mat[12];

            if(det == 0)
            {
                return Matrix();
            }

            for(int i = 0; i < 16; i++)
            {
                inv.m_mat[i] /= det;
            }

            return inv;
        }

        Matrix Matrix::removeTranslation() const
        {
            std::array<float, 16> mat;
            std::copy(m_mat.begin(), m_mat.begin() + 16, mat.begin());
            mat[3] = 0.0f;
            mat[7] = 0.0f;
            mat[11] = 0.0f;
            return Matrix(std::move(mat));
        }


        Matrix3 Matrix::toInverseMat3() const
        {
            float a00 = m_mat[0], a01 = m_mat[1], a02 = m_mat[2],
            a10 = m_mat[4], a11 = m_mat[5], a12 = m_mat[6],
            a20 = m_mat[8], a21 = m_mat[9], a22 = m_mat[10],

            b01 = a22 * a11 - a12 * a21,
            b11 = -a22 * a10 + a12 * a20,
            b21 = a21 * a10 - a11 * a20,

            d = a00 * b01 + a01 * b11 + a02 * b21,
            id;

            if(d == 0)
            {
                return Matrix3();
            }

            id = 1 / d;

            Matrix3 dst;
            dst[0] = b01 * id;
            dst[1] = (-a22 * a01 + a02 * a21) * id;
            dst[2] = (a12 * a01 - a02 * a11) * id;
            dst[3] = b11 * id;
            dst[4] = (a22 * a00 - a02 * a20) * id;
            dst[5] = (-a12 * a00 + a02 * a10) * id;
            dst[6] = b21 * id;
            dst[7] = (-a21 * a00 + a01 * a20) * id;
            dst[8] = (a11 * a00 - a01 * a10) * id;
            return dst;
        }

        float Matrix::det() const
        {
            return aex::math::det(m_mat, 4);
        }
        float det(const std::array<float, 16>& _m, std::size_t n)
        {
            std::array<float, 16> m = _m;
            float ratio, det;
            std::size_t i, j, k;

            for(i = 0; i < n; i++)
            {
                for(j = 0; j < n; j++)
                {
                    if(j > i)
                    {
                        ratio = m[j * n + i] / m[i * n + i];

                        for(k = 0; k < n; k++)
                        {
                            m[j * n + k] -= ratio * m[i * n + k];
                        }
                    }
                }
            }

            det = 1; //storage for determinant

            for(i = 0; i < n; i++)
            {
                det *= m[i * n + i];
            }

            return det;
        }

        bool operator==(const Matrix& lhs, const Matrix& rhs)
        {
            if(&lhs == &rhs)
                return true;
            /*
               for(std::size_t ii = 0; ii < 4; ++ii)
               for(std::size_t jj = 0; jj < 4; ++jj)
               if(lhs(ii, jj) != rhs(ii, jj))
               return false;*/

            return lhs.m_mat==rhs.m_mat;

        }

        bool operator!=(const Matrix& lhs, const Matrix& rhs)
        {
            return !(lhs == rhs);
        }

        Vector3 extract_translation_from_matrix(const Matrix& mat)
        {
            return Vector3(mat[3],mat[7],mat[11]);
        }

        Quaternion extract_rotation_from_matrix(const Matrix& mat)
        {
            Matrix rot =mat;
            rot[3]=rot[7]=rot[11]=0.0f;
            Vector3 scl = extract_scale_from_matrix(mat);
            Matrix scale= aex::math::scale(scl.x,scl.y,scl.z);
            rot=rot*scale.inverse();
            return getQuaternionFromMatrix(rot).unit();
        }

        Vector3 extract_scale_from_matrix(const Matrix& mat)
        {

            Vector3 row0=Vector3(mat[0], mat[4], mat[8]);
            Vector3 row1=Vector3(mat[1], mat[5], mat[9]);
            Vector3 row2=Vector3(mat[2], mat[6], mat[10]);

            return Vector3(row0.length(),row1.length(),row2.length());
        }

    } /* math */
} /* aex */
