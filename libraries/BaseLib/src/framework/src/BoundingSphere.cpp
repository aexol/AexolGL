//
//  boundingSphere.cpp
//  AexolGL
//
//  Created by Kamil Matysiewicz on 05.11.2013.
//
//

#include <aex/BoundingSphere.hpp>
#include <array>
#include <cfloat>

namespace aex
{

    BoundingSphere::BoundingSphere() :
        m_center(),
        m_radius(0.0f),
        m_Epsilon(0.001f)
    {

    }

    BoundingSphere::BoundingSphere(Vector c) :
        m_center(c),
        m_radius(0.0f),
        m_Epsilon(0.001f)
    {

    }

    BoundingSphere::BoundingSphere(Vector p1, Vector p2) :
        m_center((p1.x + p2.x) * 0.5f, (p1.y + p2.y) * 0.5f, (p1.z + p2.z) * 0.5f),
        m_Epsilon(0.001f)
    {
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float dz = p2.z - p1.z;
        m_radius = 0.25f * (dx * dx + dy * dy + dz * dz);
    }

    BoundingSphere::BoundingSphere(Vector p0, Vector p1, Vector p2) :
        m_Epsilon(0.001f)
    {
        Vector  A   = p0 - p2;
        Vector  B   = p1 - p2;
        float AdA = A % A;
        float AdB = A % B;
        float BdB = B % B;
        float det = AdA * BdB - AdB * AdB;

        if(fabs(det) > 0.0f)
        {
            float m00, m01, m10, m11, d0, d1;

            if(AdA >= BdB)
            {
                m00 = 1.0f;
                m01 = AdB / AdA;
                m10 = m01;
                m11 = BdB / AdA;
                d0 = 0.5f;
                d1 = 0.5f * m11;
            }
            else
            {
                m00 = AdA / BdB;
                m01 = AdB / BdB;
                m10 = m01;
                m11 = 1.0f;
                d0 = 0.5f * m00;
                d1 = 0.5f;
            }

            float invDet = 1.0f / (m00 * m11 - m01 * m10);
            float u0 = invDet * (m11 * d0 - m01 * d1);
            float u1 = invDet * (m00 * d1 - m10 * d0);
            float u2 = 1.0f - u0 - u1;
            m_center = (p0 * u0) + (p1 * u1) + (p2 * u2);
            Vector tmp = (A * u0) + (B * u1);
            m_radius = tmp % tmp;
        }
        else
        {
            m_center = Vector();
            m_radius = 0.0f;
        }
    }

    BoundingSphere::BoundingSphere(Vector p0, Vector p1, Vector p2, Vector p3) :
        m_Epsilon(0.001f)
    {
        // Compute the sphere containing p0, p1, p2, and p3.  The Center in
        // barycentric coordinates is K = u0*p0+u1*p1+u2*p2+u3*p3 where
        // u0+u1+u2+u3=1.  The Center is equidistant from the three points, so
        // |K-p0| = |K-p1| = |K-p2| = |K-p3| = R where R is the radius of the
        // sphere.
        //
        // From these conditions,
        //   K-p0 = u0*A + u1*B + u2*C - A
        //   K-p1 = u0*A + u1*B + u2*C - B
        //   K-p2 = u0*A + u1*B + u2*C - C
        //   K-p3 = u0*A + u1*B + u2*C
        // where A = p0-p3, B = p1-p3, and C = p2-p3 which leads to
        //   r^2 = |u0*A+u1*B+u2*C|^2 - 2*Dot(A,u0*A+u1*B+u2*C) + |A|^2
        //   r^2 = |u0*A+u1*B+u2*C|^2 - 2*Dot(B,u0*A+u1*B+u2*C) + |B|^2
        //   r^2 = |u0*A+u1*B+u2*C|^2 - 2*Dot(C,u0*A+u1*B+u2*C) + |C|^2
        //   r^2 = |u0*A+u1*B+u2*C|^2
        // Subtracting the last equation from the first three and writing
        // the equations as a linear system,
        //
        // +-                          -++   -+       +-        -+
        // | Dot(A,A) Dot(A,B) Dot(A,C) || u0 | = 0.5 | Dot(A,A) |
        // | Dot(B,A) Dot(B,B) Dot(B,C) || u1 |       | Dot(B,B) |
        // | Dot(C,A) Dot(C,B) Dot(C,C) || u2 |       | Dot(C,C) |
        // +-                          -++   -+       +-        -+
        //
        // If M = [A B C] is the matrix whose columns are the vectors A, B, and C;
        // if D is the 3x1 column 0.5*(Dot(A,A),Dot(B,B),Dot(C,C)); and if U is
        // the 3x1 column (u0,u1,u2), then the system is M^T*M*U = D.  The system
        // is solved in two steps:  V = M*U = M^{-T}*D, U = M^{-1}*V.  After
        // solving the system, r^2 is computed from the fourth equation listed
        // previously.

        Vector A = p0 - p3;
        Vector B = p1 - p3;
        Vector C = p2 - p3;

        aex::math::Matrix3 M(A, B, C);

        Vector D((A % A), B % B, C % C);

        D = D * 0.5f;

        // TODO:  With mEpsilon == 0.0, there are data sets for which this
        // algorithm fails.  A small positive mEpsilon appears to help, but
        // this is a classic problem of computational geometry--determining
        // the correct sign of a determinant when using floating-point
        // arithmetic.  One of the goals of the Malleable Mathematics
        // Library is to eliminate such problems (using arbitrary precision
        // arithmetic or a filtered predicate).
        aex::math::Matrix3 invM = M.inverse(m_Epsilon);

        if(!invM.isZero())
        {
            Vector V = D * invM;        //This two lines are not equal! One is a pre-multiplication (which treats vector as row)
            Vector U = invM * V;        //and next is post-multiplication (which treats vector as column).
            float U3 = 1.0f - U.x - U.y - U.z;
            m_center = (p0 * U.x) + (p1 * U.y) + (p2 * U.z) + (p3 * U3);
            Vector tmp((A * U.x) + (B * U.y) + (C * U.z));
            m_radius = tmp % tmp;
        }
        else
        {
            m_center = Vector();
            m_radius = 0.0f;
        }
    }

    BoundingSphere&
    BoundingSphere::operator=(const BoundingSphere& bSphere)
    {
        m_center      = bSphere.m_center;
        m_radius      = bSphere.m_radius;
        m_Epsilon     = bSphere.m_Epsilon;
        return (*this);
    }

    void
    BoundingSphere::welzlCompute(const std::vector<float>& vertices)
    {
        size_t pcount = vertices.size();
        pcount /= 3;

        std::vector<Vector> vertCopy;
        vertCopy.reserve(pcount);

        for(size_t i = 0; i < pcount; i++)
        {
            vertCopy.push_back(Vector(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]));
        }

        welzlCalc(vertCopy);
    }
    void
    BoundingSphere::welzlCalc(std::vector<Vector>& vertices)
    {
        std::vector<Vector> supportSet(0);
        supportSet.reserve(4);

        /*
         std::random_device rd;                                          //Shuffle to ensure random processing
         std::mt19937 g( rd() );

         std::random_shuffle( vertices.begin(),vertices.end(), g );
         //*/

        size_t size = vertices.size();

        if(size > 0)
        {
            Vector p = vertices.at(0);

            setSphere(p);

            size_t index = 1;

            supportSet.push_back(p);

            while(index < size)
            {
                Vector pi = vertices.at(index);

                if(std::find(supportSet.cbegin(), supportSet.cend(), pi) == supportSet.cend())
                {
                    if(!contains(pi))
                    {
                        if(update(supportSet, pi))
                        {
                            index = 0;
                            continue;
                        }
                    }
                }

                index++;
            }
        }

        /*
         int i = 1;
         for(Vector v : supportSet){
         System.out.println("Punkt "+i+": "+v.x+", "+v.y+", "+v.z);
         i++;
         }
         //*/
        m_radius = sqrt(m_radius);
    }
    bool
    BoundingSphere::update(std::vector<Vector>& supportSet, Vector p)
    {
        size_t supportSize = supportSet.size();

        if(supportSize == 1)
        {
            return updateOne(supportSet, p);
        }
        else if(supportSize == 2)
        {
            return updateTwo(supportSet, p);
        }
        else if(supportSize == 3)
        {
            return updateThree(supportSet, p);
        }
        else if(supportSize == 4)
        {
            return updateFour(supportSet, p);
        }

        return false;
    }
    bool
    BoundingSphere::updateFour(std::vector<Vector>& supportSet, Vector p)
    {
        // init a total of 14 possible spheres
        std::array<BoundingSphere, 14> spheres;
        float minRad2 = FLT_MAX;

        size_t index = 0;

        // init the points in the permutation tests
        Vector p0 = supportSet[0];
        Vector p1 = supportSet[1];
        Vector p2 = supportSet[2];
        Vector p3 = supportSet[3];

        spheres[0] = BoundingSphere(p0, p);

        if(spheres[0].contains(p1) && spheres[0].contains(p2) && spheres[0].contains(p3))
        {
            minRad2 = spheres[0].m_radius;
            index   = 0;
        }

        // construct a circle from p1 and p3 and check if p0 and p2 are inside it
        spheres[1] = BoundingSphere(p1, p);

        if(spheres[1].m_radius < minRad2 &&
                spheres[1].contains(p0) && spheres[1].contains(p2) && spheres[1].contains(p3))
        {
            minRad2 = spheres[1].m_radius;
            index   = 1;
        }

        // construct a circle from p2 and p3 and check if p0 and p1 are inside it
        spheres[2] = BoundingSphere(p2, p);

        if(spheres[2].m_radius < minRad2 &&
                spheres[2].contains(p0) && spheres[2].contains(p1) && spheres[2].contains(p3))
        {
            minRad2 = spheres[2].m_radius;
            index   = 2;
        }

        spheres[3] = BoundingSphere(p3, p);

        if(spheres[3].m_radius < minRad2 &&
                spheres[3].contains(p0) && spheres[3].contains(p1) && spheres[3].contains(p2))
        {
            minRad2 = spheres[3].m_radius;
            index   = 3;
        }

        // construct a circle from p0, p1 and p3 and check if p2 is inside it
        spheres[4] = BoundingSphere(p0, p1, p);

        if(spheres[4].m_radius < minRad2 &&
                spheres[4].contains(p2) && spheres[4].contains(p3))
        {
            minRad2 = spheres[4].m_radius;
            index   = 4;
        }

        // construct a circle from p0, p2 and p3 and check if p1 is inside it
        spheres[5] = BoundingSphere(p0, p2, p);

        if(spheres[5].m_radius < minRad2 &&
                spheres[5].contains(p1) && spheres[5].contains(p3))
        {
            minRad2 = spheres[5].m_radius;
            index   = 5;
        }

        // construct a circle from p1, p2 and p3 and check if p0 is inside it
        spheres[6] = BoundingSphere(p1, p2, p);

        if(spheres[6].m_radius < minRad2 &&
                spheres[6].contains(p0) && spheres[6].contains(p3))
        {
            minRad2 = spheres[6].m_radius;
            index   = 6;
        }

        spheres[7] = BoundingSphere(p0, p3, p);

        if(spheres[7].m_radius < minRad2 &&
                spheres[7].contains(p1) && spheres[7].contains(p2))
        {
            minRad2 = spheres[7].m_radius;
            index   = 7;
        }

        spheres[8] = BoundingSphere(p1, p3, p);

        if(spheres[8].m_radius < minRad2 &&
                spheres[8].contains(p0) && spheres[8].contains(p2))
        {
            minRad2 = spheres[8].m_radius;
            index   = 8;
        }

        spheres[9] = BoundingSphere(p2, p3, p);

        if(spheres[9].m_radius < minRad2 &&
                spheres[9].contains(p0) && spheres[9].contains(p1))
        {
            minRad2 = spheres[9].m_radius;
            index   = 9;
        }

        spheres[10] = BoundingSphere(p0, p1, p2, p);

        if(spheres[10].m_radius < minRad2 &&
                spheres[10].contains(p3))
        {
            minRad2 = spheres[10].m_radius;
            index = 10;
        }

        spheres[11] = BoundingSphere(p0, p1, p3, p);

        if(spheres[11].m_radius < minRad2 &&
                spheres[11].contains(p2))
        {
            minRad2 = spheres[11].m_radius;
            index   = 11;
        }

        spheres[12] = BoundingSphere(p0, p2, p3, p);

        if(spheres[12].m_radius < minRad2 &&
                spheres[12].contains(p1))
        {
            minRad2 = spheres[12].m_radius;
            index   = 12;
        }

        spheres[13] = BoundingSphere(p1, p2, p3, p);

        if(spheres[13].m_radius < minRad2 &&
                spheres[13].contains(p0))
        {
            index   = 13;
        }

        BoundingSphere minc = spheres[index];
        Vector op;
        // update sphere and support set

        switch(index)
        {
        case 0:
        {
            // two points, p replaces second point
            supportSet.pop_back();
            supportSet.pop_back();
            supportSet[1] = p;
        }
        break;

        case 1:
        {
            // two points, p replaces first point
            supportSet.pop_back();
            supportSet.pop_back();
            supportSet[0] = p;
        }
        break;

        case 2:
        {
            // two points, p replaces second point, third point replaces first point
            op = p2;
            supportSet.pop_back();
            supportSet.pop_back();
            supportSet[0] = op;
            supportSet[1] = p;
        }
        break;

        case 3:
        {
            op = p3;
            supportSet.pop_back();
            supportSet.pop_back();
            supportSet[0] = op;
            supportSet[1] = p;
        }
        break;

        case 4:
        {
            supportSet.pop_back();
            supportSet[2] = p;
        }
        break;

        case 5:
        {
            // three points, p replaces second point
            supportSet.pop_back();
            supportSet[1] = p;
        }
        break;

        case 6:
        {
            // three points, p replaces first point
            supportSet.pop_back();
            supportSet[0] = p;
        }
        break;

        case 7:
        {
            supportSet[0] = p0;
            supportSet[1] = p3;
            supportSet[2] = p;
            supportSet.pop_back();
        }
        break;

        case 8:
        {
            supportSet[0] = p1;
            supportSet[1] = p3;
            supportSet[2] = p;
            supportSet.pop_back();
        }
        break;

        case 9:
        {
            supportSet[0] = p2;
            supportSet[1] = p3;
            supportSet[2] = p;
            supportSet.pop_back();
        }
        break;

        case 10:
        {
            supportSet[3] = p;
        }
        break;

        case 11:
        {
            supportSet[2] = p;
        }
        break;

        case 12:
        {
            supportSet[1] = p;
        }
        break;

        case 13:
        {
            supportSet[0] = p;
        }
        break;
        }

        if(minc.m_radius > m_radius)
        {
            m_radius = minc.m_radius;
            m_center = minc.m_center;
            return true;
        }

        return false;
    }

    bool
    BoundingSphere::updateThree(std::vector<Vector>& supportSet, Vector p)
    {
        // init a total of 6 possible spheres
        std::array<BoundingSphere, 6> spheres;
        float minRad2 = FLT_MAX;
        size_t index    = std::numeric_limits<size_t>::max();

        // init the points in the permutation tests
        Vector p0 = supportSet[0];
        Vector p1 = supportSet[1];
        Vector p2 = supportSet[2];
        Vector p3 = p;

        // construct a circle from p0 and p3 and check if p1 and p2 are inside it
        spheres[0] = BoundingSphere(p0, p3);

        if(spheres[0].contains(p1) && spheres[0].contains(p2))
        {
            minRad2 = spheres[0].m_radius;
            index   = 0;
        }

        // construct a circle from p1 and p3 and check if p0 and p2 are inside it
        spheres[1] = BoundingSphere(p1, p3);

        if(spheres[1].m_radius < minRad2 &&
                spheres[1].contains(p0) && spheres[1].contains(p2))
        {
            minRad2 = spheres[1].m_radius;
            index   = 1;
        }

        // construct a circle from p2 and p3 and check if p0 and p1 are inside it
        spheres[2] = BoundingSphere(p2, p3);

        if(spheres[2].m_radius < minRad2 &&
                spheres[2].contains(p0) && spheres[2].contains(p1))
        {
            minRad2 = spheres[2].m_radius;
            index   = 2;
        }

        // construct a circle from p0, p1 and p3 and check if p2 is inside it
        spheres[3] = BoundingSphere(p0, p1, p3);

        if(spheres[3].m_radius < minRad2 &&
                spheres[3].contains(p2))
        {
            minRad2 = spheres[3].m_radius;
            index   = 3;
        }

        // construct a circle from p0, p2 and p3 and check if p1 is inside it
        spheres[4] = BoundingSphere(p0, p2, p3);

        if(spheres[4].m_radius < minRad2 &&
                spheres[4].contains(p1))
        {
            minRad2 = spheres[4].m_radius;
            index   = 4;
        }

        // construct a circle from p1, p2 and p3 and check if p0 is inside it
        spheres[5] = BoundingSphere(p1, p2, p3);

        if(spheres[5].m_radius < minRad2 &&
                spheres[5].contains(p0))
        {
            index   = 5;
        }

        BoundingSphere minc;

        if(index != std::numeric_limits<size_t>::max())
        {
            // get the minimum circle
            minc = spheres[index];

            // update set of support
            switch(index)
            {
            case 0:
            {
                // two points, p replaces second point
                supportSet.pop_back();
                supportSet[1] = p;
            }
            break;

            case 1:
            {
                // two points, p replaces first point
                supportSet.pop_back();
                supportSet[0] = p;

            }
            break;

            case 2:
            {
                // two points, p replaces second point, third point replaces first point
                Vector op = supportSet[2];
                supportSet.pop_back();
                supportSet[0] = op;
                supportSet[1] = p;
            }
            break;

            case 3:
            {
                // three points, p replaces third point
                supportSet[2] = p;
            }
            break;

            case 4:
            {
                // three points, p replaces second point
                supportSet[1] = p;
            }
            break;

            case 5:
            {
                // three points, p replaces first point
                supportSet[0] = p;
            }
            break;
            }

            if(minc.m_radius > m_radius)
            {
                m_radius = minc.m_radius;
                m_center = minc.m_center;
                return true;
            }
        }
        else
        {
            minc = BoundingSphere(p0, p1, p2, p3);
            supportSet.push_back(p3);

            if(minc.m_radius > m_radius)
            {
                m_radius = minc.m_radius;
                m_center = minc.m_center;
                return true;
            }
        }

        return false;
    }
    bool
    BoundingSphere::updateTwo(std::vector<Vector>& supportSet, Vector p)
    {
        Vector p0 = supportSet[0];
        Vector p1 = supportSet[1];

        std::array<BoundingSphere, 3> spheres;
        float minRad2 = FLT_MAX;
        size_t index = std::numeric_limits<size_t>::max();

        // create circle from point 0 and p and see if point 1 is within it
        spheres[0] = BoundingSphere(p0, p);

        if(spheres[0].contains(p1))
        {
            // keep it
            minRad2 = spheres[0].m_radius;
            index   = 0;
        }

        // create circle from point 1 and p and see if point 0 is within it
        spheres[1] = BoundingSphere(p1, p);

        if(spheres[1].m_radius < minRad2 &&
                spheres[1].contains(p0))
        {
            // keep it
            index   = 1;
        }

        BoundingSphere minc;

        if(index != std::numeric_limits<size_t>::max())
        {
            // one of the permutations contains all points, keep it
            supportSet[ 1 - index ] = p;

            if(spheres[index].m_radius > m_radius)
            {
                m_radius = spheres[index].m_radius;
                m_center = spheres[index].m_center;
                return true;
            }
        }
        else
        {
            // enlarge circle to encompass three points and add p to set
            minc = BoundingSphere(p0, p1, p);
            supportSet.push_back(p);

            if(minc.m_radius > m_radius)
            {
                m_radius = minc.m_radius;
                m_center = minc.m_center;
                return true;
            }
        }

        return false;
    }
    bool
    BoundingSphere::updateOne(std::vector<Vector>& supportSet, Vector p)
    {
        // add the new point to the set
        supportSet.push_back(p);

        // create new minimal circle
        Vector p0 = supportSet[0];
        return setSphere(p0, p);
    }
    bool
    BoundingSphere::contains(const Vector& p) const
    {
        float dx = p.x - m_center.x;
        float dy = p.y - m_center.y;
        float dz = p.z - m_center.z;
        float dist2 = dx * dx + dy * dy + dz * dz;
        return (dist2 - m_radius <= 0);
    }
    void
    BoundingSphere::setSphere(const Vector& c)
    {
        m_center = c;
        m_radius = 0.0f;
    }
    bool
    BoundingSphere::setSphere(const Vector& p1, const Vector& p2)
    {
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float dz = p2.z - p1.z;
        float _radius = 0.25f * (dx * dx + dy * dy + dz * dz);

        if(_radius > m_radius)
        {
            m_center = Vector((p1.x + p2.x) * 0.5f, (p1.y + p2.y) * 0.5f, (p1.z + p2.z) * 0.5f);
            m_radius = _radius;
            return true;
        }

        return false;
    }
    void
    BoundingSphere::setSphere(const Vector&  p1, const Vector&  p2, const Vector&  p3)
    {
        Vector  e1 = p2 - p1;
        Vector  e2 = p3 - p1;
        Vector  aCrossB(e1 % e2);
        float denom = 2.0f * (aCrossB % aCrossB);

        if(denom == 0.0f)
        {
            m_center = Vector();
            m_radius = 0.0f;
        }
        else
        {
            Vector oVector = ((aCrossB * e1) * (e2 % e2) + ((e2 * aCrossB) * (e1 % e2))) / denom;
            m_radius = oVector.length();
            m_center = p1 + oVector;
        }
    }
    void
    BoundingSphere::setSphere(const Vector& p1, const Vector& p2, const Vector& p3, const Vector& p4)
    {
        Vector e1 = p2 - p1;
        Vector e2 = p3 - p1;
        Vector e3 = p4 - p1;
        float   denom = 2.0f * (e1.x * (e2.y * e3.z - e3.y * e2.z) - e2.x * (e1.y * e3.z - e3.y * e1.z) + e3.x * (e1.y * e2.z - e2.y * e1.z));

        if(denom == 0)
        {
            m_center = Vector();
            m_radius = 0.0f;
        }
        else
        {
            Vector oVector = (((e1 * e2) * (e3 % e3))  + ((e3 * e1) * (e2 % e2)) + ((e2 * e3) * (e1 % e1))) / denom;
            m_radius = oVector.length();
            m_center = p1 + oVector;
        }
    }

    
    void
    BoundingSphere::scale(float x)
    {
        m_radius *= x;
        m_center = m_center * x;
    }
    void
    BoundingSphere::move(float x, float y, float z)
    {
        m_center.x += x;
        m_center.y += y;
        m_center.z += z;
    }

    INTERSECTION
    BoundingSphere::frustumTest(const Frustum& f)
    {

        // various distances
        INTERSECTION ret = INTERSECTION::IN;

        // calculate our distances to each of the planes
        for(auto plane : f.getPlanes())
        {

            // find the distance to this plane
            float fDistance = (plane.normal()) % (this->center()) + plane.get_d();
            float radius = this->radius();

            // if this distance is < -sphere.radius, we are outside
            if(fDistance < -radius)
            {
                return(INTERSECTION::OUT);
            }

            // else if the distance is between +- radius, then we intersect
            if(fabs(fDistance) < radius)
            {
                ret = INTERSECTION::INTERSECT;
            }
        }

        // otherwise we are fully in view
        return ret;
    }

}
