//
//  Plane.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 02.08.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include <aex/Plane.hpp>

namespace aex
{
    void
    Plane::set_coefficients(float a, float b, float c, float d)
    {
        // set the normal vector
        m_plane.m_normal.x = a;
        m_plane.m_normal.y = b;
        m_plane.m_normal.z = c;
        //compute the lenght of the vector
        float l = m_plane.m_normal.length();
        // normalize the vector
        m_plane.m_normal.x = a / l;
        m_plane.m_normal.y = b / l;
        m_plane.m_normal.z = c / l;
        // and divide d by th length as well
        m_plane.m_d = d / l;
        m_plane.m_absNormal = aex::math::Vector3(fabsf(m_plane.m_normal.x), fabsf(m_plane.m_normal.y), fabsf(m_plane.m_normal.z));
    }

    bool
    intersectionPointofPlanes(const Plane3D& p1, const Plane3D& p2, const Plane3D& p3, Vector& vec)
    {

        const Vector& n1 = p1.normal();
        const Vector& n2 = p2.normal();
        const Vector& n3 = p3.normal();
        float denom = n1.dot(n2.cross(n3));

        if(fabsf(denom) < 0.00000001f)
        {
            return false;
        }

        vec = (n2.cross(n3) * p1.get_d() +
               n3.cross(n1) * p2.get_d() +
               n1.cross(n2) * p3.get_d()) / -denom;

        return true;
    }
    bool intersectionPointofPlanes(const Plane& p1, const Plane& p2, const Plane& p3, Vector& vec)
    {
        return intersectionPointofPlanes(p1.m_plane, p2.m_plane, p3.m_plane, vec);
    }
}
