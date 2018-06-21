/*
 * Vector3.h
 *
 *  Created on: Jul 1, 2013
 *      Author: aexol-1
 */

#ifndef VECTOR3_H_
#define VECTOR3_H_
#include <aex/Math.hpp>
#include <aex/Vector2.hpp>
#include <aex/Matrix3.hpp>
#include <aex/Common.hpp>
#include <string>
#include <vector>
#include <ostream>
#include <iostream>

namespace aex
{
    namespace math {
        class Matrix3;
        class Quaternion;

        class AEX_PUBLIC_API Vector3
        {
            public:
                float x;
                float y;
                float z;
            private:
                //#ifndef AEX_PACK_TIGHT
                //        aex::__padding<aex::eval_padding(sizeof(float)*3)> _padding;
                //#endif
            public:
                /**
                 *	@brief	Zero vector ctor
                 *
                 */
                Vector3();



                //Zero vec
                /**
                 *	@brief	Vector3 ctor from 1 float: x=y=z=f
                 *
                 *	@param 	f 	float
                 *
                 */
                explicit Vector3(float f);
                //Vector from 1 val
                /**
                 *	@brief	Vector3 ctor from 3 floats: x,y,z
                 *
                 *	@param 	x
                 *	@param 	y
                 *	@param 	z
                 */
                Vector3(const float tx, const float ty, const float tz = 0.0f);
                //Vector from 3 vals
                /**
                 *	@brief	Vector3 copy ctor
                 *
                 *	@param 	vec 	Vector to copy
                 *
                 */
                Vector3(const Vector3& vec);


                //Copy of vec
                /**
                 *	@brief	C
                 *
                 *	@param 	arr 	<#arr description#>
                 *	@param 	0 	<#0 description#>
                 *	@param 	3 	<#3 description#>
                 *
                 *	@return	<#return value description#>
                 */
                Vector3(const float* const arr, size_t offset = 0, size_t size = 3);

                /**
                 *	@brief	Vector3 from Vector2, z=0
                 *
                 *	@param 	vec 	Vector2 to copy
                 *
                 *	@return	<#return value description#>
                 */
                explicit Vector3(const Vector2& vec);
                //3D vec from 2D, no depth
                ~Vector3();
                /**
                 *	@brief	Exact compare of 2 vectors ignoring GLfloating point error
                 *
                 *	@param 	vec 	Vector2 to compare
                 *
                 */
                inline bool exactCompare(const Vector3& vec) const;
                //Exact compare of 2 vectors ignoring GLfloating point error

                /**
                 *	@brief	Compare vectors using epsilon (default Math::EPSILON)
                 *
                 *	@param 	vec 	Vector2 to compare
                 *	@param 	aex::math::Math::EPSILON 	float epsilon
                 *
                 *	@return	bool
                 */
                inline bool compare(const Vector3& vec, const float epsilon = 0.0000001f) const;
                //Compare with epsilon


                inline float* data();

                inline Vector3 abs();

                inline Vector3 operator-() const; //Negate vector
                inline Vector3 operator+(const Vector3& vec) const; //Add 2 vectors, value returned in new Vector
                inline Vector3 operator-(const Vector3& vec) const; //Subtract 2 vectors, value returned in new Vector
                inline Vector3 operator*(const float s) const; //Multiply by scalar, return in new Vector
                /**
                 *	@brief	Returns cross-product of vectors
                 *
                 *
                 *	@return	New Vector3
                 */
                inline Vector3 operator*(const Vector3& vec) const;
                //Unlike Vector2 this one is a cross-product, yeah, I'm evil
                /**
                 *	@brief	Pre-multiplication, watch it, it's column major
                 *
                 *	@param 	mat 	Matrix3
                 *
                 *	@return     Vector3
                 */
                Vector3 operator*(const Matrix3& mat) const;
                //Pre-multiplication, watch it, it's column major
                inline Vector3 operator/(const float s) const; //Divide by scalar, or if you're annyoin multiply be reverse :P
                inline Vector3 operator/(const Vector3& vec) const; //Divide scalars of vectors by each other, maybe in distant future, in galaxy far far away it will be useful
                /**
                 *	@brief	Dot product of vectors
                 *
                 *	@return	New Vector3
                 */
                inline float operator%(const Vector3& vec) const;
                //Dot product
                inline void operator+=(const Vector3& vec); //Add 2 vectors, keep result in caller
                inline void operator-=(const Vector3& vec); //Sub 2 Vectors, result kept in caller
                inline void operator*=(const float s); //Multiply by scalar, result stored in caller
                inline void operator*=(const Vector3& vec); //Cross, stored in caller
                void operator*=(const Matrix3& mat); //Pre-multiplication stored in caller
                inline void operator/=(const float s); //Ahhh, whatever ...
                inline void operator/=(const Vector3& vec);
                inline Vector3& operator=(const Vector3& vec);
                inline Vector3& operator=(const Vector2& vec);
                inline bool operator==(const Vector3& vec) const;
                inline bool operator!=(const Vector3& vec) const;
                inline float& operator[](const size_t index); //Convenience function
                inline float* operator()(); //Convenience function
                inline float operator[](const size_t index) const;

                inline float chebLength() const;

                /**
                 *	@brief	Sum of squared components
                 *
                 *	@return	float
                 */
                inline float squaredLength() const;
                //Sum of squared commponents
                /**
                 *	@brief	Length of Vector
                 *
                 *	@return	float
                 */
                inline float length() const;
                //Square root of sum of squared components (of squared components .... of squared components ... of squared components)
                /**
                 *	@brief	Normalize this vector and keep the result in caller (you're gonna lose your current vector, just so you know)
                 */
                inline void normalizeThis();
                //Normalize this vector and keep the result in caller (you're gonna lose your current vector, just so you know)
                /**
                 *	@brief	Normalize this vector and return the result in new Vector
                 *
                 *	@return	Vector3
                 */
                inline Vector3 unit() const;
                //Normalize this vector and return the result in new Vector
                /**
                 *	@brief	Minimal value in vector
                 *
                 *	@return	float
                 */
                inline float min() const;

                /**
                 *	@brief	Maximal value in vector
                 *
                 *	@return	<#return value description#>
                 */
                inline float max() const;

                inline Vector3 positive() const;

                /**
                 *	@brief	Returns cross-product of vectors
                 *
                 *	@param 	vec 	Vector3
                 *
                 *	@return	Vector3
                 */

                inline Vector3 cross(const Vector3& vec) const;

                //Same as *
                /**
                 *	@brief	Returns dot-product of vectors
                 *
                 *	@param 	vec 	Vector3
                 *
                 *	@return	Vector3
                 */
                static inline float dot(const Vector3& a, const Vector3& b);
                inline float dot(const Vector3& vec) const;
                friend float dot(const Vector3& a, const Vector3& b);

                //Same as %
                inline void toArray(float* const arr, const size_t offset = 0, const size_t size = 3) const; //Puts vector in array arr, I added offset and size though I don't know why

                /**
                 *	@brief	Check is vector zero
                 *
                 *	@return	bool
                 */
                inline bool isZero(float epsilon = 0.0000001f) const;

                inline float* toArray()
                {
                    return &x;
                }
                /**
                 *	@brief	Multiply vector by vector
                 *
                 *	@return	Vector3
                 */
                inline Vector3 multiply(const Vector3&) const;

                inline aex::string toString() const;

                /**
                 *	@brief	Rotation beetwen two vectors
                 *
                 *	@param 	v 	Vector to compare
                 *
                 *	@return	Quaternion
                 */
                Quaternion getRotationTo(Vector3 v);


                inline Vector2 xy() const;
                inline Vector2 yz() const;
                inline Vector2 xz() const;

                static inline Vector3 intIDtoVectorID(uint32_t id);
                static inline std::array<uint8_t,4> intIDtoVectorRGBA(uint32_t id);
                static inline uint32_t vectorIDtoIntID(Vector3 colorId);


                static inline Vector3 Red();
                static inline Vector3 Green();
                static inline Vector3 Blue();

                static inline Vector3 OneOneOne();
                static inline Vector3 Zero();
                static inline Vector3 Up();
                static inline Vector3 Down();
                static inline Vector3 Right();
                static inline Vector3 Left();
                static inline Vector3 Front();
                static inline Vector3 Back();


                friend std::ostream& operator<<(std::ostream& os, const Vector3& vec);


        };

        float dot(const Vector3& a , const Vector3& b);

        typedef aex::shared_ptr<Vector3> Vector3_ptr; ///highly unkillable, spreads like turtles(fast and in every direction), use witch caution;
        typedef std::vector<Vector3> Vector3Vec; ///highly unkillable, spreads like turtles(fast and in every direction), use witch caution;
        typedef Vector3 vec3;

        inline std::ostream& operator<<(std::ostream& os, const Vector3& vec)
        {
            os << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]";
            return os;
        }

        bool
            Vector3::exactCompare(const Vector3& vec) const
            {
                return ((x == vec.x) && (y == vec.y) && (z == vec.z));
            }

        inline Vector3 Vector3::abs()
        {
            return Vector3(fabs(x),fabs(y),fabs(z));
        }

        bool
            Vector3::compare(const Vector3& vec, float epsilon) const
            {
                return ((fabs(x - vec.x) < epsilon) && (fabs(y - vec.y) < epsilon) && (fabs(z - vec.z) < epsilon));
            }

        float* Vector3::data()
        {
            return &x;
        }

        Vector3
            Vector3::operator-() const
            {
                return Vector3(-x, -y, -z);
            }

        Vector3
            Vector3::operator+(const Vector3& vec) const
            {
                return Vector3(x + vec.x, y + vec.y, z + vec.z);
            }

        Vector3
            Vector3::operator-(const Vector3& vec) const
            {
                return Vector3(x - vec.x, y - vec.y, z - vec.z);
            }

        Vector3
            Vector3::operator*(const float s) const
            {
                return Vector3(x * s, y * s, z * s);
            }

        Vector3
            Vector3::operator*(const Vector3& vec) const
            {

                return Vector3((y * vec.z) - (z * vec.y),
                        (z * vec.x) - (x * vec.z),
                        (x * vec.y) - (y * vec.x));
            }

        Vector3
            Vector3::operator/(const float s) const
            {
                return Vector3(x / s, y / s, z / s);
            }

        Vector3
            Vector3::operator/(const Vector3& vec) const
            {
                return Vector3(x / vec.x, y / vec.y, z / vec.z);
            }

        float
            Vector3::operator%(const Vector3& vec) const
            {
                return x * vec.x + y * vec.y + z * vec.z;
            }

        void
            Vector3::operator+=(const Vector3& vec)
            {
                x += vec.x;
                y += vec.y;
                z += vec.z;
            }

        void
            Vector3::operator-=(const Vector3& vec)
            {
                x -= vec.x;
                y -= vec.y;
                z -= vec.z;
            }

        void
            Vector3::operator*=(const float s)
            {
                x *= s;
                y *= s;
                z *= s;
            }

        void
            Vector3::operator*=(const Vector3& vec)
            {
                float t_x = (y * vec.z) - (z * vec.y);
                float t_y = (z * vec.x) - (x * vec.z);
                float t_z = (x * vec.y) - (y * vec.x);
                x = t_x;
                y = t_y;
                z = t_z;
            }

        void
            Vector3::operator/=(const float s)
            {
                x /= s;
                y /= s;
                z /= s;
            }

        void
            Vector3::operator/=(const Vector3& vec)
            {
                x /= vec.x;
                y /= vec.y;
                z /= vec.z;
            }

        Vector3&
            Vector3::operator=(const Vector3& vec)
            {
                x = vec.x;
                y = vec.y;
                z = vec.z;
                return *this;
            }

        Vector3&
            Vector3::operator=(const Vector2& vec)
            {
                x = vec.x;
                y = vec.y;
                z = 0.0f;
                return *this;
            }

        bool
            Vector3::operator==(const Vector3& vec) const
            {
                return ((x == vec.x) && (y == vec.y) && (z == vec.z)) ? true : false;
            }

        bool
            Vector3::operator!=(const Vector3& vec) const
            {
                return !((x == vec.x) && (y == vec.y) && (z == vec.z)) ? true : false;
            }

        float*
            Vector3::operator()()
            {
                return &x;
            }

        float&
            Vector3::operator[](const size_t index)
            {
                return (&x)[index];
            }

        float
            Vector3::operator[](const size_t index) const
            {
                return (&x)[index];
            }


        float 
            Vector3::chebLength() const
            {
                return Math::max(Math::max(fabs(x),fabs(y)),fabs(z));
            }

        float
            Vector3::squaredLength() const
            {
                return x * x + y * y + z * z;
            }

        float
            Vector3::length() const
            {
                float sqrLen = squaredLength();
                if(sqrLen != 0)
                    return sqrt(x * x + y * y + z * z);
                return 0;
            }

        void
            Vector3::normalizeThis()
            {
                float len = length();
                if(len != 0)
                {
                    x /= len;
                    y /= len;
                    z /= len;
                }
            }

        Vector3
            Vector3::unit() const
            {
                float len = length();
                if(len != 0)
                    return Vector3(x / len, y / len, z / len);
                return Vector3(0.0f, 0.0f, 0.0f);
            }

        float
            Vector3::min() const
            {

                return Math::min(Math::min(x, y), z);
            }

        float
            Vector3::max() const
            {
                return Math::max(Math::max(x, y), z);
            }


        Vector3 
            Vector3::positive() const
            {
                return Vector3(fabs(x),fabs(y),fabs(z));
            }

        Vector3
            Vector3::cross(const Vector3& vec) const
            {
                return Vector3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
            }

        float
            Vector3::dot(const Vector3& vec) const
            {
                return x * vec.x + y * vec.y + z * vec.z;
            }

        float 
            Vector3::dot(const Vector3& a, const Vector3& b)
            {
                return a.dot(b);
            }

        void
            Vector3::toArray(float* const arr, const size_t offset, const size_t size) const
            {
                if(arr != NULL)
                {
                    size_t maxSize = sizeof(arr) / sizeof(float);

                    if(offset + size <= maxSize)
                    {
                        for(size_t i = offset; i < offset + size; i++)
                        {
                            arr[i] = (&x)[i % size];
                        }
                    }
                }
            }

        bool
            Vector3::isZero(float epsilon) const
            {
                if(!Math::closeEnough(x, 0.0f, epsilon))
                {
                    return false;
                }

                if(!Math::closeEnough(y, 0.0f, epsilon))
                {
                    return false;
                }

                if(!Math::closeEnough(z, 0.0f, epsilon))
                {
                    return false;
                }

                return true;
            }

        Vector3
            Vector3::multiply(const Vector3& vec) const
            {
                return Vector3(x * vec.x, y * vec.y, z * vec.z);
            }

        aex::string
            Vector3::toString() const
            {
                return aex::string("[ " + aex::to_string(x) + ", " + aex::to_string(y) + ", " + aex::to_string(z) + " ]");
            }

        Vector2 
            Vector3::xy() const
            {
                return Vector2(x,y);
            }
        Vector2 
            Vector3::yz() const
            {
                return Vector2(y,z);
            }

        Vector2 
            Vector3::xz() const
            {
                return Vector2(x,z);
            }

        inline float
            distance(const Vector3& vec1, const Vector3& vec2)
            {
                return (vec2 - vec1).length();
            }

        inline float
            distance(const float x1, const float y1, const float z1, const float x2, const float y2, const float z2)
            {
                float xx = x2 - x1;
                float yy = y2 - y1;
                float zz = z2 - z1;
                return sqrt(xx * xx + yy * yy + zz * zz);
            }

        inline float
            distance(const float* const arr1, const float* const arr2)
            {
                size_t arr1_size = sizeof(arr1) / sizeof(float);
                size_t arr2_size = sizeof(arr2) / sizeof(float);

                if(arr2_size >= 3 && arr1_size >= 3)
                {
                    return distance(arr1[0], arr1[1], arr1[2], arr2[0], arr2[1], arr2[2]);
                }

                return 0.0f;
            }

        inline float distanceXZ(Vector3 v, Vector3 v2)
        {
            return (v2 - v).length();
        }

        inline float distanceXZ(float* vArray, float* vArray2)
        {
            Vector3 v(vArray[0], 0.0f, vArray[2]);
            Vector3 v2(vArray2[0], 0.0f, vArray2[2]);
            return (v2 - v).length();
        }

        /**
         * @param v
         * @param v2
         * @return
         */
        inline float distanceXY(Vector3 v, Vector3 v2)
        {
            return (v2 - v).length();
        }

        /**
         * @param vArray
         * @param vArray2
         * @return
         */
        inline float distanceXY(float* vArray, float* vArray2)
        {
            Vector3 v(vArray[0], vArray[1], 0.0f);
            Vector3 v2(vArray2[0], vArray2[1], 0.0f);
            return (v2 - v).length();
        }

        /**
         * @param v
         * @param v2
         * @return
         */
        inline float distanceYZ(Vector3 v, Vector3 v2)
        {
            return (v2 - v).length();
        }

        /**
         * @param vArray
         * @param vArray2
         * @return
         */
        inline float distanceYZ(float* vArray, float* vArray2)
        {
            Vector3 v(0.0f, vArray[1], vArray[2]);
            Vector3 v2(0.0f, vArray2[1], vArray2[2]);
            return (v2 - v).length();
        }

        /**
         * @param theta
         * @param phi
         * @return
         */
        inline Vector3 random(double theta, double phi)
        {
            float x = (float)(cos(theta) * cos(phi));
            float y = (float) sin(phi);
            float z = (float)(sin(theta) * cos(phi));
            return Vector3(x, y, z);
        }


        /**
         * @param a
         * @param b
         * @return
         */
        Vector3 min(const Vector3& a, const Vector3& b);
        /**
         * @param a
         * @param b
         * @return
         */
        Vector3 max(Vector3 a, Vector3 b);
        /**
         * @param a
         * @param b
         * @param fraction
         * @return
         */
        Vector3 lerpV3(const Vector3& a, const Vector3& b, const Vector3& fraction);
        Vector3 lerpf(const Vector3& a, const Vector3& b, const float& fraction);

        Vector3 cross(const Vector3& a, const Vector3& b);
        Vector3 negate(const Vector3& a);
        Vector3 unit(const Vector3& a);
        float length(const Vector3& a);
        float dot(const Vector3& a, const Vector3& b);
        Vector3 fromArray(const std::vector<float>&, size_t, size_t);

        Vector3 angleCentroidWeight(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& faceNorm);



        Vector3
            Vector3::intIDtoVectorID(uint32_t id)
            {
                Vector3 ret;
                int r = id / 65536;
                r = std::max(r, 0);
                int g = (id - (r * 65536)) / 256;
                g = std::max(g, 0);
                int b = ((id - (r * 65536)) - (g * 256));
                b = std::max(b, 0);

                // convert to floats. only divide by 255, because range is 0-255
                ret.x = (float) r / 255.0f;
                ret.y = (float) g / 255.0f;
                ret.z = (float) b / 255.0f;

                return ret;
            }

        std::array<uint8_t,4>  Vector3::intIDtoVectorRGBA(uint32_t id)
        {

            std::array<uint8_t,4>  ret;
            ret[0] = id & 0xFF; id >>= 8;
            ret[1] = id & 0xFF; id >>= 8;
            ret[2] = id & 0xFF; id >>= 8;
            ret[3] = id & 0xFF; /* dwColor >>= 8; */


            return ret;
        }

        uint32_t
            Vector3::vectorIDtoIntID(Vector3 colorId)
            {
                uint32_t ret = 0;
                ret += static_cast<uint32_t>(colorId.z * 255.0f) * 65536;
                ret += static_cast<uint32_t>(colorId.y * 255.0f) * 256;
                ret += static_cast<uint32_t>(colorId.x * 255.0f);
                return ret;
            }

        Vector3 Vector3::Red()
        {
            return Vector3(1.0f, 0.0f, 0.0f);
        }

        Vector3 Vector3::Green()
        {
            return Vector3(0.0f, 1.0f, 0.0f);
        }

        Vector3 Vector3::Blue()
        {
            return Vector3(0.0f, 0.0f, 1.0f);
        }

        Vector3 Vector3::OneOneOne()
        {
            return Vector3(1.0f, 1.0f, 1.0f);
        }

        Vector3 Vector3::Zero()
        {
            return Vector3(0.0f, 0.0f, 0.0f);
        }

        Vector3 Vector3::Up()
        {
            return Vector3(0.0f, 1.0f, 0.0f);
        }

        Vector3 Vector3::Down()
        {
            return Vector3(0.0f, -1.0f, 0.0f);
        }

        Vector3 Vector3::Right()
        {
            return Vector3(1.0f, 0.0f, 0.0f);
        }

        Vector3 Vector3::Left()
        {
            return Vector3(-1.0f, 0.0f, 0.0f);
        }

        Vector3 Vector3::Front()
        {
            return Vector3(0.0f, 0.0f, 1.0f);
        }

        Vector3 Vector3::Back()
        {
            return Vector3(0.0f, 0.0f, -1.0f);
        }

    } /* math */
} /* aex */

#endif /* VECTOR3_H_ */
