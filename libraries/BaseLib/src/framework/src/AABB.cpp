//
//  AABB.cpp
//  AexolGL
//
//  Created by Kamil Matysiewicz on 31.10.2013.
//
//

#include <aex/AABB.hpp>
#include <aex/Math.hpp>

namespace aex
{
    AABB::AABB() :
        m_center(0.0f, 0.0f, 0.0f),
        m_halfdist(0.0f, 0.0f, 0.0f)
    {
    }

    AABB::AABB(aex::math::Vector3 center, aex::math::Vector3  halfdist):
    m_center(center),
    m_halfdist(halfdist.positive())
    {
        
    }
    
    AABB::AABB(const AABBBuilder& builder)
    {
        fromBuilderToAABB(builder);
    }

    AABB::AABB(const AABB& a, const AABB& b)
    {
        aex::math::Vector3 a_min(a.m_center - a.m_halfdist);
        aex::math::Vector3 a_max(a.m_center + a.m_halfdist);
        aex::math::Vector3 b_min(b.m_center - b.m_halfdist);
        aex::math::Vector3 b_max(b.m_center + b.m_halfdist);
        AABBBuilder a_builder(a_min, a_max);
        AABBBuilder b_builder(b_min, b_max);
        fromBuilderToAABB(aex::AABBBuilder(a_builder, b_builder));

    }

    void
    AABB::fromBuilderToAABB(const AABBBuilder& builder)
    {
        m_halfdist.x = fabsf(builder.m_max.x - builder.m_min.x) / 2.0;
        m_halfdist.y = fabsf(builder.m_max.y - builder.m_min.y) / 2.0;
        m_halfdist.z = fabsf(builder.m_max.z - builder.m_min.z) / 2.0;
        m_center = aex::math::Vector3(builder.m_min.x + m_halfdist.x, builder.m_min.y + m_halfdist.y, builder.m_min.z + m_halfdist.z);
    }

    bool
    AABB::overlapsWith(const AABB& aabb) const
    {
        if(fabs((m_center.x - aabb.m_center.x)) > (m_halfdist.x + aabb.m_halfdist.x))
        {
            return false;
        }

        if(fabs((m_center.y - aabb.m_center.y)) > (m_halfdist.y + aabb.m_halfdist.y))
        {
            return false;
        }

        if(fabs((m_center.z - aabb.m_center.z)) > (m_halfdist.z + aabb.m_halfdist.z))
        {
            return false;
        }

        return true;
    }

    AABB& AABB::operator= (const AABB& sec)
    {
        m_center=sec.m_center;
        m_halfdist=sec.m_halfdist;
        
        return *this;
    }
    
    AABB
    AABB::Infinite()
    {
        AABB ret;
        ret.m_center = aex::math::Vector3(0.0f, 0.0f, 0.0f);
        ret.m_halfdist = aex::math::Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
        return ret;
    }

}
