//
//  Frustum.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 02.08.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//


#include <aex/Frustum.hpp>
#include <aex/Matrix.hpp>
#include <aex/BoundingSphere.hpp>
#include <aex/Logger.hpp>
#include <vector>

#ifdef DEBUG
#include <iostream>
#endif

namespace aex {

    Vector tempHelper(const Matrix& pespective, const std::array<float, 4>& hvec) {
        Matrix invPerspective = pespective.inverse();
        std::array<float, 4> hworld = {
            {
                invPerspective[0] * hvec[0] + invPerspective[1] * hvec[1] + invPerspective[2] * hvec[2] + invPerspective[3] * hvec[3],
                invPerspective[4] * hvec[0] + invPerspective[5] * hvec[1] + invPerspective[6] * hvec[2] + invPerspective[7] * hvec[3],
                invPerspective[8] * hvec[0] + invPerspective[9] * hvec[1] + invPerspective[10] * hvec[2] + invPerspective[11] * hvec[3],
                invPerspective[12] * hvec[0] + invPerspective[13] * hvec[1] + invPerspective[14] * hvec[2] + invPerspective[15] * hvec[3],
            }
        };
        return Vector(hworld[0] / hworld[3], hworld[1] / hworld[3], hworld[2] / hworld[3]);
    }

    void
    Frustum::setFrustum(const Matrix& m) {
        auto& mat = m.getM();
        Plane plane;
        plane.set_coefficients(mat[8] + mat[12], mat[9] + mat[13], mat[10] + mat[14], mat[11] + mat[15]);
        plane.set_plane_name(PLANES::NEAR);
        planes[0] = plane;
        plane = Plane();
        plane.set_coefficients(-mat[8] + mat[12], -mat[9] + mat[13], -mat[10] + mat[14], -mat[11] + mat[15]);
        plane.set_plane_name(PLANES::FAR);
        planes[1] = plane;
        plane = Plane();
        plane.set_coefficients(mat[4] + mat[12], mat[5] + mat[13], mat[6] + mat[14], mat[7] + mat[15]);
        plane.set_plane_name(PLANES::BOTTOM);
        planes[2] = plane;
        plane = Plane();
        plane.set_coefficients(-mat[4] + mat[12], -mat[5] + mat[13], -mat[6] + mat[14], -mat[7] + mat[15]);
        plane.set_plane_name(PLANES::TOP);
        planes[3] = plane;
        plane = Plane();
        plane.set_coefficients(mat[0] + mat[12], mat[1] + mat[13], mat[2] + mat[14], mat[3] + mat[15]);
        plane.set_plane_name(PLANES::LEFT);
        planes[4] = plane;
        plane = Plane();
        plane.set_coefficients(-mat[0] + mat[12], -mat[1] + mat[13], -mat[2] + mat[14], -mat[3] + mat[15]);
        plane.set_plane_name(PLANES::RIGHT);
        planes[5] = plane;
        /*For colum major
         * Plane plane = new Plane();
         plane.setCoefficients(mat[2]+mat[3], mat[6]+mat[7], mat[10]+mat[11], mat[14]+mat[15]);
         planes.put(Planes.NEAR, plane);
         plane = new Plane();
         plane.setCoefficients(-mat[2]+mat[3], -mat[6]+mat[7], -mat[10]+mat[11], -mat[14]+mat[15]);
         planes.put(Planes.FAR, plane);
         plane = new Plane();
         plane.setCoefficients(mat[1]+mat[3], mat[5]+mat[7], mat[9]+mat[11], mat[13]+mat[15]);
         planes.put(Planes.BOTTOM, plane);
         plane = new Plane();
         plane.setCoefficients(-mat[1]+mat[3], -mat[5]+mat[7], -mat[9]+mat[11], -mat[13]+mat[15]);
         planes.put(Planes.TOP, plane);
         plane = new Plane();
         plane.setCoefficients(mat[0]+mat[3], mat[4]+mat[7], mat[8]+mat[11], mat[12]+mat[15]);
         planes.put(Planes.LEFT, plane);
         plane = new Plane();
         plane.setCoefficients(-mat[0]+mat[3], -mat[4]+mat[7], -mat[8]+mat[11], -mat[12]+mat[15]);
         planes.put(Planes.RIGHT, plane);
         //*/
#ifdef DEBUG
        std::array<std::array<float, 4>, 8> corners = {
            {
                {
                    { -1.0f, -1.0f, -1.0f, 1.0f}
                },
                {
                    { 1.0f, -1.0f, -1.0f, 1.0f}
                },
                {
                    { -1.0f, 1.0f, -1.0f, 1.0f}
                },
                {
                    { 1.0f, 1.0f, -1.0f, 1.0f}
                },
                {
                    { -1.0f, -1.0f, 1.0f, 1.0f}
                },
                {
                    { 1.0f, -1.0f, 1.0f, 1.0f}
                },
                {
                    { -1.0f, 1.0f, 1.0f, 1.0f}
                },
                {
                    { 1.0f, 1.0f, 1.0f, 1.0f}
                },
            }
        };

        for (auto& a : corners) {
            Vector world = tempHelper(m, a);
            // LOG_DEBUG("[ ",world.m_x,", ",world.m_y,", ",world.m_z," ]");
        }

#endif
    }

    INTERSECTION
    Frustum::containsSphere(const BoundingSphere* bSphere) const {
        // various distances
        INTERSECTION ret = INTERSECTION::IN;

        // calculate our distances to each of the planes
        for (auto plane : planes) {

            // find the distance to this plane
            float fDistance = (plane.normal()) % (bSphere->center()) + plane.get_d();
            float radius = bSphere->radius();

            // if this distance is < -sphere.radius, we are outside
            if (fDistance < -radius) {
                return (INTERSECTION::OUT);
            }

            // else if the distance is between +- radius, then we intersect
            if (fabs(fDistance) < radius) {
                ret = INTERSECTION::INTERSECT;
            }
        }

        // otherwise we are fully in view
        return ret;
    }

    int
    Frustum::containsSphere2(const BoundingSphere* bSphere) const {
        auto intersection = containsSphere(bSphere);

        if (intersection == INTERSECTION::IN) {
            return 1;
        }
        if (intersection == INTERSECTION::INTERSECT) {
            return 0;
        }
        return -1;
    }

    INTERSECTION
    Frustum::containsAABB(const aex::AABB& aabb) const {
        INTERSECTION ret = INTERSECTION::IN;

        aex::math::Vector3 center = aabb.center();
        aex::math::Vector3 halfdist = aabb.halfdist();

        for (size_t  i = 0 ; i < planes.size() ; ++i) {
            const Plane& plane = planes[i];
            const aex::math::Vector3& planeNormal = plane.normal();
            const aex::math::Vector3& planeAbsNormal = plane.absNormal();
            float planeD = plane.get_d();

            float d =   center.x * planeNormal.x +
                        center.y * planeNormal.y +
                        center.z * planeNormal.z;

            float r =   halfdist.x * planeAbsNormal.x +
                        halfdist.y * planeAbsNormal.y +
                        halfdist.z * planeAbsNormal.z;

	float d_p_r = d + r + planeD;
	if(d_p_r<0)
	{
            ret = INTERSECTION::OUT;
            break;
	}

	float d_m_r = d - r + planeD;
        if(d_m_r<0)
            ret = INTERSECTION::INTERSECT;
        }

        return ret;
    }

    int
    Frustum::containsAABB2(const AABB& aabb) const {
        auto intersection = containsAABB(aabb);

        if (intersection == INTERSECTION::IN) {
            return 1;
        }

        if (intersection == INTERSECTION::INTERSECT) {
            return 0;
        }

        return -1;
    }

    bool Frustum::containsAABBbool(const AABB& aabb) const {
        auto intersection = containsAABB(aabb);

        if (intersection == INTERSECTION::IN || intersection == INTERSECTION::INTERSECT) {
            return true;
        } else {
            return false;

        }

    }

    INTERSECTION Frustum::containsPointIntersect(const aex::math::Vector3& point) const {
        INTERSECTION ret = INTERSECTION::IN;

        aex::math::Vector3 center = point;


        for (auto plane : planes) {
            const aex::math::Vector3& planeNormal = plane.normal();
            //const aex::math::Vector3& planeAbsNormal = plane.absNormal();
            plane.absNormal();
            float planeD = plane.get_d();

            float d = center.x * planeNormal.x +
                    center.y * planeNormal.y +
                    center.z * planeNormal.z;


            float d_p_r = d + planeD;

            if (d_p_r < 0) {
                ret = INTERSECTION::OUT;
                break;
            }

        }

        return ret;
    }

    bool Frustum::containsPoint(const aex::math::Vector3& point) const {
        INTERSECTION intersection = containsPointIntersect(point);

        if (intersection == INTERSECTION::IN) {
            return true;
        }

        if (intersection == INTERSECTION::OUT) {
            return false;
        }
        
        return false;
    }

}
