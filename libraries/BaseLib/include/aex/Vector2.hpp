/*
 * Vector2.h
 *
 *  Created on: Jun 28, 2013
 *      Author: aexol-1
 */

#ifndef VECTOR2_H_
#define VECTOR2_H_
#include <aex/Math.hpp>
namespace aex {
    namespace math{
        class AEX_PUBLIC_API Vector2 {
            public:
                float x;
                float y;
                /**
                 *	@brief	Default ctor
                 *
                 */
                Vector2();

                /**
                 *	@brief	Ctor from 2 floats
                 *
                 *	@param 	x 	x dim
                 *	@param 	y 	y dim
                 *
                 */
                Vector2(float x, float y);
                /**
                 *	@brief	Copy ctor
                 *
                 *	@param 	vec 	vecto2 to copy
                 */
                Vector2(const Vector2& vec);

                ~Vector2();
                /**
                 *	@brief	Exact compare vectors
                 *
                 *	@param 	vec 	Vector2 to compare
                 *
                 *	@return	bool
                 */
                inline bool exactCompare(const Vector2& vec) const;
                //Exact compare of 2 vectors ignoring GLfloating point error
                /**
                 *	@brief	Compare vectors using epsilon (default Math::EPSILON)
                 *
                 *	@param vec 	Vector2 to compare
                 *	@param epsilon 	float epsilon
                 *
                 *	@return	bool
                 */
                inline bool compare(const Vector2& vec, const float epsilon = 0.0000001f) const;
                //Compare with epsilon

                Vector2 rotate(float angle);
                inline Vector2 operator-() const; //Negate vector
                inline Vector2 operator+(const Vector2& vec) const; //Add 2 vectors, value returned in new Vector
                inline Vector2 operator-(const Vector2& vec) const; //Subtract 2 vectors, value returned in new Vector
                inline Vector2 operator*(const float s) const; //Multiply by scalar, return in new Vector
                inline Vector2 operator/(const float s) const; //Divide by scalar, return in new Vector
                inline Vector2 operator*(const Vector2& vec) const; //Multiply values of scalar, pretty usless but meh
                inline float operator%(const Vector2& vec) const; //Dot product
                inline void operator+=(const Vector2& vec); //Add 2 vectors, keep result in caller
                inline void operator-=(const Vector2& vec); //Sub 2 Vectors, result kept in caller
                inline void operator*=(const float s); //Multiply by scalar, result stored in caller
                inline void operator*=(const Vector2& vec); //Multiply 2 vectors, result in caller
                inline void operator/=(const float s);
                inline bool operator==(const Vector2& vec) const; //Compare
                inline bool operator!=(const Vector2& vec) const; //Compare
                inline float operator[](const int index) const; //Convenience function

                inline bool TestAB(const Vector2& A, const Vector2& B) const;


                inline float chebLength() const;
                //Sum of squared commponents
                /**
                 *	@brief	Sum of squared commponents
                 *
                 *	@return	float
                 */
                inline float squaredLength() const;
                //Sum of squared commponents
                /**
                 *	@brief	Lenght of vector
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
                 *	@return	Vector2
                 */
                inline Vector2 unit() const;
                inline Vector2 abs() const;
                inline float angleDeg() const;
                inline float dot(const Vector2& v2) const;
                inline float cross(const Vector2& v2) const;
                inline float* data();
                //Normalize this vector and return the result in new Vector
                friend std::ostream& operator<<(std::ostream& os, const Vector2& vec);
        };
        Vector2 lerpf(const Vector2& a, const Vector2& b, const float& fraction);
        Vector2 unit(const Vector2& v1);

        float dot(const Vector2& v1, const Vector2& v2);
        float cross(const Vector2& v1, const Vector2& v2);
        float length(const Vector2& v1);


        inline std::ostream& operator<<(std::ostream& os, const Vector2& vec) {
            os << "[" << vec.x << ", " << vec.y << "]";
            return os;
        }

        bool Vector2::TestAB(const Vector2& A, const Vector2& B) const {

            if (A.x < B.x && A.y < B.y) {
                return (x >= A.x && y >= A.y && x <= B.x && y <= B.y);
            }

            if (A.x > B.x && A.y > B.y) {
                return (x <= A.x && y <= A.y && x >= B.x && y >= B.y);
            }

            return false;
        }

        bool
            Vector2::exactCompare(const Vector2& vec) const {
                return ((x == vec.x) && (y == vec.y));
            }

        bool
            Vector2::compare(const Vector2& vec, const float epsilon) const {
                return ((fabs(x - vec.x) < epsilon) && (fabs(y - vec.y) < epsilon));
            }

        Vector2
            Vector2::operator-() const {
                return Vector2(-x, -y);
            }

        Vector2
            Vector2::operator+(const Vector2& vec) const {
                return Vector2(x + vec.x, y + vec.y);
            }

        Vector2
            Vector2::operator-(const Vector2& vec) const {
                return Vector2(x - vec.x, y - vec.y);
            }

        Vector2
            Vector2::operator*(const float s) const {
                return Vector2(x * s, y * s);
            }

        Vector2
            Vector2::operator*(const Vector2& vec) const {
                return Vector2(x * vec.x, y * vec.y);
            }

        Vector2
            Vector2::operator/(const float s) const {
                return Vector2(x / s, y / s);
            }

        float
            Vector2::operator%(const Vector2& vec) const {
                return x * vec.x + y * y;
            }

        void
            Vector2::operator+=(const Vector2& vec) {
                x += vec.x;
                y += vec.y;
            }

        void
            Vector2::operator-=(const Vector2& vec) {
                x -= vec.x;
                y -= vec.y;
            }

        void
            Vector2::operator*=(const float s) {
                x *= s;
                y *= s;
            }

        void
            Vector2::operator/=(const float s) {
                x /= s;
                y /= s;
            }

        void
            Vector2::operator*=(const Vector2& vec) {
                x *= vec.x;
                y *= vec.y;
            }

        float
            Vector2::operator[](const int index) const {
                return (&x)[index];
            }

        bool
            Vector2::operator==(const Vector2& vec) const {
                return compare(vec);
            }

        bool
            Vector2::operator!=(const Vector2& vec) const {
                return !compare(vec);
            }

        float
            Vector2::chebLength() const {
                return Math::max(fabs(x), fabs(y));
            }

        float
            Vector2::squaredLength() const {
                return x * x + y * y;
            }

        float
            Vector2::length() const {
                return sqrt(x * x + y * y);
            }

        void
            Vector2::normalizeThis() {
                float len = length();
                x /= len;
                y /= len;
            }

        Vector2
            Vector2::unit() const {
                float len = length();
                return Vector2(x / len, y / len);
            }

        Vector2
            Vector2::abs() const {
                return Vector2(fabs(x), fabs(y));
            }

        float Vector2::angleDeg() const
        {
            return Math::angle_from_point_deg(x, y);
        }

        float Vector2::dot(const Vector2& v2) const {
            return x * v2.x + y * v2.y;
        }

        float Vector2::cross(const Vector2& v2) const {
            return x * v2.y - y * v2.x;
        }

        float* Vector2::data() {
            return &x;
        }
    } /* math */
} /* aex */
#endif /* VECTOR2_H_ */
