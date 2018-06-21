//
//  Matrix.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 02.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#ifndef __aexolGL__Matrix__
#define __aexolGL__Matrix__
#include <aex/Common.hpp>
#include <aex/Matrix3.hpp>

#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <array>
#include <memory>
#include <vector>
#include <type_traits>
#include <mutex>

namespace aex {
    namespace math {
        class Vector3;
        class Matrix3;
        class Matrix;
        class Quaternion;

        /**
         *	@brief	Returns perspective matrix
         *
         *	@param 	fov
         *	@param 	aspect
         *	@param 	near
         *	@param 	far
         *
         *	@return	Matrix
         */
        Matrix AEX_PUBLIC_API perspective(const float fov, const float aspect, const float near, const float far);
        //Returns Matrix perspective
        /**
         *	@brief	Returns idenity matrix
         *
         *	@return	Matrix
         */
        Matrix AEX_PUBLIC_API identity();

        /**
         *	@brief	Returns Matrix filled with zeros
         *
         *	@return	Matrix
         */
        Matrix AEX_PUBLIC_API zero();

        /**
         *	@brief	Returns Flustrum matrix
         *
         *	@param 	l 	left
         *	@param 	r   right
         *	@param 	b 	bottom
         *	@param 	t 	top
         *	@param 	n 	zNear
         *	@param 	f 	zFar
         *
         *	@return	Matrix
         */
        Matrix AEX_PUBLIC_API frustum(float l, float r, float b, float t, float n, float f); //Frustum Matrix

        /**
         *	@brief	Returns Ortho matrix
         *
         *	@param 	l 	left
         *	@param 	r   right
         *	@param 	b 	bottom
         *	@param 	t 	top
         *	@param 	n 	zNear
         *	@param 	f 	zFar
         *
         *	@return	Matrix
         */
        Matrix AEX_PUBLIC_API ortho(float l, float r, float b, float t, float n, float f);
        //Well .... ortho matrix!

        Matrix AEX_PUBLIC_API scale(Vector3 vec);
        /**
         *	@brief	Returns scale matrix
         *
         *	@param 	x 	x dim scale
         *	@param 	y 	y dim scale
         *	@param 	z 	z dim scale
         *
         *	@return	Matrix
         */
        Matrix AEX_PUBLIC_API scale(float x, float y, float z);

        Matrix AEX_PUBLIC_API translate(Vector3 vec);
        //Scale matrix
        /**
         *	@brief	Returns translate matrix
         *
         *	@param 	x 	x direction
         *	@param 	y 	y direction
         *	@param 	z 	z direction
         *
         *	@return	<#return value description#>
         */
        Matrix AEX_PUBLIC_API translate(float x, float y, float z); //Translation matrix

        Matrix AEX_PUBLIC_API rotate(float a, Vector3 vec);
        /**
         *	@brief	Returns rotation matrix. Angle around xyz axis.
         *
         *	@param 	a 	Angle
         *	@param 	x 	x axis
         *	@param 	y 	y axis
         *	@param 	z   z axis
         *
         *	@return	<#return value description#>
         */
        Matrix AEX_PUBLIC_API rotate(float a, float x, float y, float z);

        Matrix rotateAngles(Vector3 vec);

        /**
         *	@brief	Returns euler rotation matrix. 
         *
         *	@param 	x 	x axis
         *	@param 	y 	y axis
         *	@param 	z       z axis
         *
         *	@return	<#return value description#>
         */
        Matrix AEX_PUBLIC_API rotateAngles(float x, float y, float z);


        /**
         *	@brief	Returns euler rotation matrix. 
         *
         *  @param  Vector3 vec,
         *	@param 	x 	x angle
         *	@param 	y 	y angle
         *	@param 	z       z angle
         *
         *	@return	<#return value description#>
         */
        Vector3 AEX_PUBLIC_API rotateVector(Vector3 vec, float x, float y, float z);

        /**
         *	@brief	Returns euler rotation matrix. 
         *
         *  @param  Vector3 vec,
         *	@param 	Vector3 angles,
         *
         *	@return	<#return value description#>
         */
        Vector3 AEX_PUBLIC_API rotateVector(Vector3 vec, Vector3 angles);

        /**
         *	@brief	Translateion matrix to translateo ne plate to other. 
         *
         *  @param  Vector3 current plane,
         *	@param 	Vector3 target plane,
         *
         *	@return	<#return value description#>
         */
        Matrix AEX_PUBLIC_API translateToOtherPlane(Vector3 current, Vector3 target);

        Matrix AEX_PUBLIC_API lookAt(const Vector3& e, const Vector3& c, const Vector3& u);

        //rotate by a around xyz axis
        /**
         *	@brief	Returns lookAt matrix ()
         *
         *	@param 	ex  eye x
         *	@param 	ey  eye y
         *	@param 	ez 	eye z
         *	@param 	cx 	target x
         *	@param 	cy 	target y
         *	@param 	cz 	target z
         *	@param 	ux 	up x
         *	@param 	uy 	up y
         *	@param 	uz 	up z
         *
         *	@return	<#return value description#>
         */
        Matrix AEX_PUBLIC_API lookAt(const float ex, const float ey, const float ez, const float tx, const float ty, const float tz, const float ux, const float uy, const float uz);
        //lookAt matrix

        Matrix AEX_PUBLIC_API normalMatrix( const Matrix& model, Matrix view);

        class AEX_PUBLIC_API Matrix
        {

            public:
                typedef std::array<float, 16> value_type;
                /**
                 *	@brief	Default ctor, sets matrix to idenity
                 *
                 *	@return
                 */
                Matrix();
                //Creates new Matrix and sets it to identity
                /**
                 *	@brief	Copy ctor
                 *
                 *	@param 	mat 	matrix to copy from
                 *
                 */
                Matrix(const Matrix& mat) = default;
                /**
                 *	@brief	Move ctor
                 *
                 *	@param 	mat 	matrix to copy from
                 *
                 */
                Matrix(Matrix&& mat) = default;

                /**
                 *	@brief	Creates identity matrix and fills corresponding values with values from mat
                 *
                 */
                template < class... Args, typename = typename aex::disable_if<sizeof...(Args) != 16>::type>
                    Matrix(Args && ... args);
                //ć
                /**
                 *	@brief	Make matrix from vector<float>
                 *
                 *	@param 	 	if vector size=16 copy it, if vector size<16 fill witch 0.0f
                 */
                Matrix(const std::array<float, 16>&);

                /**
                 *	@brief	Make matrix from vector<float>
                 *
                 *	@param 	 	if vector size=16 copy it, if vector size<16 fill witch 0.0f
                 */
                Matrix(std::array<float, 16>&&); //Creates identity matrix and fills corresponding values with values from mat

                inline float operator[](const size_t index) const; //Access operator returning copy of value in index
                inline float& operator[](const size_t index); //Returns reference to value at index
                Matrix operator*(const Matrix& mat) const;
                inline Matrix& operator=(const Matrix& mat) = default;
                inline Matrix& operator=(Matrix&& mat) = default;
                inline Matrix& operator=(const std::array<float, 16>& mat);
                inline Matrix& operator=(std::array<float, 16>&& mat) NOEXCEPT;
                Matrix& operator*=(const Matrix& mat);

                /**
                 *  @brief Element access.
                 *
                 *  Access matrix element at M_ii_jj.
                 *
                 *  @param size_t Row
                 *  @param size_t Column
                 */
                inline float& operator()(std::size_t ii, std::size_t jj);

                /**
                 *  @brief Element access.
                 *
                 *  Access matrix element at M_ii_jj.
                 *
                 *  @param size_t Row
                 *  @param size_t Column
                 */
                inline  const float& operator()(std::size_t ii, std::size_t jj) const;

                void translateRotateScale(float tx, float ty, float tz, const Quaternion& q, float sx, float sy, float sz);

                void translate(float x, float y, float z) ;

                void rotate(const Quaternion& q) ;

                void scale(float x, float y, float z) ;

                /**
                 *	@brief	Returns inverse matrix
                 *
                 *	@return	Matrix
                 */
                Matrix inverse() const;

                Matrix removeTranslation() const;
                //Inverse matrix
                /**
                 *	@brief	Returns inverse 3x3 matrix
                 *
                 *	@return	Matrix3
                 */
                Matrix3 toInverseMat3() const;
                //Inverse 3x3 matrix
                /**
                 *	@brief	Returns transposed Matrix
                 *
                 *	@return	Matrix
                 */
                inline Matrix transpose() const;
                //row-major memory layout
                /**
                 *	@brief	Returns Determinant of matrix
                 *
                 *	@return	<#return value description#>
                 */
                float det() const;

                //Determinant of matrix
                /**
                 *	@brief  Transforms point to screen space
                 *
                 *	@param 	vec point to transform float w = 1.0f;
                 *
                 *	@return	Vector3
                 */
                Vector3 transformPoint(const Vector3& vec, float w = 1.0f) const;

                //Transform point to screen space
                /**
                 *	@brief	Transforms vector with matrix
                 *
                 *	@param 	vec 	vector to transform
                 *
                 *	@return	Vector 3
                 */
                Vector3 transformVector(const Vector3& vec, float w = 1.0f) const;
                //Transform vector with matrix
                /**
                 *	@brief	Resets matrix to idenity
                 */
                inline void identity();
                //Resets this matrix to identity
                /**
                 *	@brief	Returns matrix as vector
                 *
                 */
                inline std::array<float, 16>& getM();

                /**
                 *	@brief	Returns matrix as vector
                 *
                 */
                inline const std::array<float, 16>& getM() const;

                friend std::ostream& operator<<(std::ostream& os, const Matrix& mat);
                friend bool operator==( const Matrix& lhs, const Matrix& rhs);
                friend bool operator!=( const Matrix& lhs, const Matrix& rhs);
            private:
                std::array<float, 16> m_mat;
        };



        float&
            Matrix::operator()(std::size_t ii, std::size_t jj)
            {
                return m_mat[ii*4 + jj];
            }

        const float&
            Matrix::operator()(std::size_t ii, std::size_t jj) const
            {
                return m_mat[ii*4 + jj];
            }


        Vector3 AEX_PUBLIC_API extract_translation_from_matrix(const Matrix& mat);
        Quaternion AEX_PUBLIC_API extract_rotation_from_matrix(const Matrix& mat);
        Vector3 AEX_PUBLIC_API extract_scale_from_matrix(const Matrix& mat);

        inline std::ostream& operator<<(std::ostream& os, const Matrix& mat)
        {
            os << "["   << mat[0] << ", " << mat[1] << ", " << mat[2] << ", " << mat[3] << ", \n"
                << mat[4] << ", " << mat[5] << ", " << mat[6] << ", " << mat[7] << ", \n"
                << mat[8] << ", " << mat[9] << ", " << mat[10] << ", " << mat[11] << ", \n"
                << mat[12] << ", " << mat[13] << ", " << mat[14] << ", " << mat[15] << "]";
            return os;
        }


        float Matrix::operator[](const size_t index) const
        {
            if(index >= 16)
            {
                throw std::out_of_range("Matrix is 4x4 size, index out of bounds");
            }

            return m_mat[index];
        }
        float& Matrix::operator[](const size_t index)
        {
            if(index >= 16)
            {
                throw std::out_of_range("Matrix is 4x4 size, index out of bounds");
            }

            return m_mat[index];
        }


        Matrix& Matrix::operator=(const std::array<float, 16>& mat)
        {
            m_mat = mat;
            return (*this);
        }

        Matrix& Matrix::operator=(std::array<float, 16>&& mat) NOEXCEPT
        {
            m_mat = std::move(mat);
            return (*this);
        }

        Matrix Matrix::transpose() const
        {
            std::array<float, 16> mat
            {{
                 m_mat[0], m_mat[4], m_mat[8], m_mat[12],
                 m_mat[1], m_mat[5], m_mat[9], m_mat[13],
                 m_mat[2], m_mat[6], m_mat[10], m_mat[14],
                 m_mat[3], m_mat[7], m_mat[11], m_mat[15]
             }};
            return Matrix(mat);
        }

        void Matrix::identity()
        {
            m_mat =
            {
                {
                    1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
                }
            };
        }

        float det(const std::array<float, 16>& _m, std::size_t n);

        std::array<float, 16>& Matrix::getM()
        {
            return m_mat;
        }

        const std::array<float, 16>& Matrix::getM() const
        {
            return m_mat;
        }
        bool operator==(const Matrix& lhs, const Matrix& rhs);
        bool operator!=(const Matrix& lhs, const Matrix& rhs);
    } /* math */
} /* aex */
#endif /* defined(__aexolGL__Matrix__) */
