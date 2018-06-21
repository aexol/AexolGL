//
//  Quaternion.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 03.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include <aex/Math.hpp>
#include <aex/Quaternion.hpp>

namespace aex {
    namespace math {
        Quaternion getQuaternionFromEulerAnglesRad(double x, double y, double z) { //heading, attitude, bank
            double w;
            /*double	ex, ey, ez;		// temp half euler angles
              double	cr, cp, cy, sr, sp, sy, cpcy, spsy;		// temp vars in roll,pitch yaw


              ex = x / 2.0d;	//half them
              ey = y / 2.0d;
              ez = z / 2.0d;

              cr = Math.cos(ex);
              cp = Math.cos(ey);
              cy = Math.cos(ez);

              sr = Math.sin(ex);
              sp = Math.sin(ey);
              sy = Math.sin(ez);

              cpcy = cp * cy;
              spsy = sp * sy;

              w = (float)(cr * cpcy + sr * spsy);

              x = sr * cpcy - cr * spsy;
              y = cr * sp * cy + sr * cp * sy;
              z = cr * cp * sy - sr * sp * cy;*/

            double c1 = cos(x / 2.0f);//heading heading angle (rotation about y)
            double s1 = sin(x / 2.0f);
            double c2 = cos(y / 2.0f);//attitude attitude angle (rotation about z)
            double s2 = sin(y / 2.0f);
            double c3 = cos(z / 2.0f);//bank bank angle (rotation about x)
            double s3 = sin(z / 2.0f);
            double c1c2 = c1 * c2;
            double s1s2 = s1 * s2;
            w = c1c2 * c3 + s1s2 * s3;
            x = s1 * c2 * c3 - c1 * s2 * s3;
            y = c1 * s2 * c3 + s1 * c2 * s3;
            z = c1c2 * s3 - s1s2 * c3;
            /*w =c1c2*c3 - s1s2*s3;
              x =c1c2*s3 + s1s2*c3;
              y =s1*c2*c3 + c1*s2*s3;
              z =c1*s2*c3 - s1*c2*s3;*/


            return Quaternion((float) x, (float) y, (float) z, (float) w);

        }

        Quaternion getQuaternionFromEulerDeg(double x, double y, double z) // Return Quaternion representation of Euler angles in degrees
        {
            return aex::math::getQuaternionFromEulerAnglesRad(x * Math::V_DEG2RAD, y * Math::V_DEG2RAD, z * Math::V_DEG2RAD);
        }

        Quaternion getQuaternionFromAxisAngle(const Vector3& v, double fi) // Return Quaternion representation of AxisAngle, angle is in radians
        {
            double half_fi, scale;
            Vector3 vec = v.unit();

            if (v.isZero()) // if axis is zero, then return quaternion (1,0,0,0)
            {
                return getIdentityQuaternion();
            }

            half_fi = fi / 2.0f;
            scale = sin(half_fi);
            return Quaternion(vec.x * scale, vec.y * scale, vec.z * scale, cos(half_fi));
        }

        Quaternion getQuaternionFromAxisAngleDeg(const Vector3& v, double fi) //Return Quaternion representation from AxisAngle, angle in degrees
        {
            return getQuaternionFromAxisAngle(v, fi * Math::V_DEG2RAD);
        }

        Quaternion getQuaternionFromMatrix(const Matrix& matrix) {
            float qw, qx, qy, qz;
            float trace = matrix(0, 0) + matrix(1, 1) + matrix(2, 2);

            if (trace > 0) {
                float s = sqrtf(trace + 1.0f) * 2.0f; // s=4*qw

                qx = (matrix(2, 1) - matrix(1, 2)) / s;
                qy = (matrix(0, 2) - matrix(2, 0)) / s;
                qz = (matrix(1, 0) - matrix(0, 1)) / s;
                qw = 0.25f * s;
            } else {
                if ((matrix(0, 0) > matrix(1, 1)) && (matrix(0, 0) > matrix(2, 2))) {
                    float s = sqrtf(1.0 + matrix(0, 0) - matrix(1, 1) - matrix(2, 2)) * 2.0f; // s=4*qx

                    qx = 0.25f * s;
                    qy = (matrix(0, 1) + matrix(1, 0)) / s;
                    qz = (matrix(0, 2) + matrix(2, 0)) / s;
                    qw = (matrix(2, 1) - matrix(1, 2)) / s;
                } else if (matrix(1, 1) > matrix(2, 2)) {
                    float s = sqrtf(1.0 + matrix(1, 1) - matrix(0, 0) - matrix(2, 2)) * 2.0f; // s=4*qy

                    qx = (matrix(0, 1) + matrix(1, 0)) / s;
                    qy = 0.25f * s;
                    qz = (matrix(1, 2) + matrix(2, 1)) / s;
                    qw = (matrix(0, 2) - matrix(2, 0)) / s;
                } else {
                    float s = sqrtf(1.0 + matrix(2, 2) - matrix(0, 0) - matrix(1, 1)) * 2.0f; // s=4*qz

                    qx = (matrix(0, 2) + matrix(2, 0)) / s;
                    qy = (matrix(1, 2) + matrix(2, 1)) / s;
                    qz = 0.25f * s;
                    qw = (matrix(1, 0) - matrix(0, 1)) / s;
                }
            }

            return Quaternion(qx, qy, qz, qw);
        }
        //return Quaternion(qx, qy, qz, qw);
        //From Geometric tools, Rotation.h

        /*float r22 = matrix(2, 2);
          float x, y, z, w;
          if(r22 <= 0.0f)
          {
          float dif10 = matrix(1, 1) - matrix(0, 0);
          float omr22 = 1.0f - r22;
          if(dif10 <= 0.0f)
          {
          float fxsqr = omr22 - dif10;
          float inv4x = 0.5f / std::sqrt(fxsqr);
          x = fxsqr * inv4x;
          y = (matrix(0, 1) + matrix(1, 0))*inv4x;
          z = (matrix(0, 2) + matrix(2, 0))*inv4x;
          w = (matrix(1, 2) - matrix(2, 1))*inv4x;
          } else {
          float fysqr = omr22 + dif10;
          float inv4y = 0.5f / std::sqrt(fysqr);
          x = (matrix(0, 1) + matrix(1, 0))*inv4y;
          y = fysqr * inv4y;
          z = (matrix(1, 2) + matrix(2, 1))*inv4y;
          w = (matrix(2, 0) - matrix(0, 2))*inv4y;
          }
          } else {
          float sum10 = matrix(1, 1) + matrix(0, 0);
          float opr22 = 1.0f + r22;
          if(sum10 <= 0.0f)
          {
          float fourZSqr = opr22 - sum10;
          float inv4z = 0.5f / std::sqrt(fourZSqr);
          x = (matrix(0, 2) + matrix(2, 0))*inv4z;
          y = (matrix(1, 2) + matrix(2, 1))*inv4z;
          z = fourZSqr*inv4z;
          w = (matrix(0, 1) - matrix(1, 0))*inv4z;
          } else {
          float fourWSqr = opr22 + sum10;
          float inv4w = 0.5f / std::sqrt(fourWSqr);
          x = (matrix(1, 2) - matrix(2, 1))*inv4w;
          y = (matrix(2, 0) - matrix(0, 2))*inv4w;
          z = (matrix(0, 1) - matrix(1, 0))*inv4w;
          w = fourWSqr*inv4w;
          }
          }
          return Quaternion(x, y, z, w);
          }*/

        Quaternion getQuaternionFromSpherical(float latitude, float longitude, float angle) {
            float sin_a = sin(angle / 2.0f);
            float cos_a = cos(angle / 2.0f);
            float sin_lat = sin(latitude);
            float cos_lat = cos(latitude);

            float sin_long = sin(longitude);
            float cos_long = cos(longitude);

            float qx = sin_a * cos_lat * sin_long;
            float qy = sin_a * sin_lat;
            float qz = sin_a * sin_lat * cos_long;
            float qw = cos_a;
            return Quaternion(qx, qy, qz, qw);
        }

        Quaternion getIdentityQuaternion() {
            return Quaternion();
        }


        float dot(Quaternion left, Quaternion right)
        {
            return left.m_x * right.m_x + left.m_y * right.m_y + left.m_z * right.m_z + left.m_w * right.m_w;
        }

        Quaternion::Quaternion(float x, float y, float z, float w, float epsilon) :
            m_x(x),
            m_y(y),
            m_z(z),
            m_w(w) {
                //normalize(epsilon);
            }

        Quaternion::Quaternion() :
            m_x(0.0f),
            m_y(0.0f),
            m_z(0.0f),
            m_w(1.0f) {
            }

        Matrix Quaternion::getMatrix() const //Return rotation matrix from this quternion
        {
            double xx, yy, zz, ww, xy, zw, xz, yw, yz, xw, invs;
            xx = m_x * m_x;
            yy = m_y * m_y;
            zz = m_z * m_z;
            ww = m_w * m_w;
            xy = m_x * m_y;
            zw = m_z * m_w;
            xz = m_x * m_z;
            yw = m_y * m_w;
            yz = m_y * m_z;
            xw = m_x * m_w;
            invs = 1 / (xx + yy + zz + ww);
            Matrix m;
            m(0, 0) = (xx - yy - zz + ww) * invs;
            m(0, 1) = 2.0 * (xy - zw) * invs;
            m(0, 2) = 2.0 * (xz + yw) * invs;
            m(0, 3) = 0;
            m(1, 0) = 2.0 * (xy + zw) * invs;
            m(1, 1) = (-xx + yy - zz + ww) * invs;
            m(1, 2) = 2.0 * (yz - xw) * invs;
            m(1, 3) = 0;
            m(2, 0) = 2.0 * (xz - yw) * invs;
            m(2, 1) = 2.0 * (yz + xw) * invs;
            m(2, 2) = (-xx - yy + zz + ww) * invs;
            m(2, 3) = 0;
            m(3, 0) = 0;
            m(3, 1) = 0;
            m(3, 2) = 0;
            m(3, 3) = 1;
            return m;

            //        Matrix r(aex::math::Math::identity4());
            //
            //        float twoX = 2.0f*m_x;
            //        float twoY = 2.0f*m_y;
            //        float twoZ = 2.0f*m_z;
            //        float twoXX = twoX*m_x;
            //        float twoXY = twoX*m_y;
            //        float twoXZ = twoX*m_z;
            //        float twoXW = twoX*m_w;
            //        float twoYY = twoY*m_y;
            //        float twoYZ = twoY*m_z;
            //        float twoYW = twoY*m_w;
            //        float twoZZ = twoZ*m_z;
            //        float twoZW = twoZ*m_w;
            //
            //        r(0, 0) = 1.0f - twoYY - twoZZ;
            //        r(1, 0) = twoXY - twoZW;
            //        r(2, 0) = twoXZ + twoYW;
            //        r(0, 1) = twoXY + twoZW;
            //        r(1, 1) = 1.0f - twoXX - twoZZ;
            //        r(2, 1) = twoYZ - twoXW;
            //        r(0, 2) = twoXZ - twoYW;
            //        r(1, 2) = twoYZ + twoXW;
            //        r(2, 2) = 1.0f - twoXX - twoYY;
            //
            //        return r;
        }

        Vector3 Quaternion::converseToEuler() {

            Quaternion q1 = *this;

            double sqw = q1.m_w * q1.m_w;
            double sqx = q1.m_x * q1.m_x;
            double sqy = q1.m_y * q1.m_y;
            double sqz = q1.m_z * q1.m_z;
            double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
            double test = q1.m_x * q1.m_y + q1.m_z * q1.m_w;

            double heading = 0.0;
            double attitude = 0.0;
            double bank = 0.0;

            if (test > 0.499 * unit) { // singularity at north pole
                heading = 2.0 * atan2(q1.m_x, q1.m_w) * aex::math::Math::V_RAD2DEG;
                attitude = aex::math::Math::PI / 2.0;
                bank = 0.0;
                return Vector3(bank, heading, attitude);
            }
            if (test < -0.499 * unit) { // singularity at south pole
                heading = -2.0 * atan2(q1.m_x, q1.m_w) * aex::math::Math::V_RAD2DEG;
                attitude = -aex::math::Math::PI / 2.0;
                bank = 0.0;
                return Vector3(bank, heading, attitude);
            }

            heading = atan2(2 * q1.m_y * q1.m_w - 2 * q1.m_x * q1.m_z, sqx - sqy - sqz + sqw);
            attitude = asin(2 * test / unit);
            bank = atan2(2 * q1.m_x * q1.m_w - 2 * q1.m_y * q1.m_z, -sqx + sqy - sqz + sqw);

            return Vector3(bank * aex::math::Math::V_RAD2DEG, heading * aex::math::Math::V_RAD2DEG, attitude * aex::math::Math::V_RAD2DEG);
        }

        void Quaternion::normalize(float epsilon) {
            float mag = magnitudeSquared();
            if (std::abs(1.0f - mag) > epsilon) {
                *this /= std::sqrt(mag);
            }
        }

        Quaternion Quaternion::unit()
        {
            Quaternion ret = *this;
            ret.normalize();
            return ret;
        }


        Quaternion getRotationTo(Vector3 from, Vector3 to) {

            from.normalizeThis();
            to.normalizeThis();
            Vector3 c = from * to;

            float d = from % to;
            Quaternion ret = Quaternion(c.x, c.y, c.z, d + sqrt((from % from) * (to % to)));
            return ret;
        }
} /* math */
} /* aex */
