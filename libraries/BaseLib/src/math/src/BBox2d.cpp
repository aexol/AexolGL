/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <aex/BBox2d.hpp>

namespace aex {
    namespace math {
        BoundingBox2D::BoundingBox2D()
        {

        }

        BoundingBox2D::BoundingBox2D(std::vector<Vector2> points)
        {
            findBoundingBox(points);
        }

        BoundingBox2D::BoundingBox2D(Vector2 center, Vector2 halfdist) :
            m_center(center),
            m_halfdist(halfdist)
        {

        }

        void BoundingBox2D::findBoundingBox(const std::vector<Vector2>& points)
        {
            if (points.size() > 0)
            {
                Vector2 min = points[0];
                Vector2 max = points[0];
                for (const Vector2& vec : points)
                {
                    if (vec.x < max.x)
                    {
                        max.x = vec.x;
                    }
                    if (vec.y < max.y)
                    {
                        max.y = vec.y;
                    }
                    if (vec.x > min.x)
                    {
                        min.x = vec.x;
                    }
                    if (vec.y > min.y)
                    {
                        min.y = vec.y;
                    }
                }

                m_halfdist = ((max - min) / 2.0f).abs();
                m_center = min + m_halfdist;
            }
        }

        bool BoundingBox2D::isPointInBox(Vector2 point)
        {
            Vector2 min = m_center - m_halfdist;
            Vector2 max = m_center + m_halfdist;

            if ((point.x >= min.x && point.x <= max.x) && (point.y >= min.y && point.y <= max.y))
            {
                return true;
            } else
            {
                return false;
            }
        }

        bool BoundingBox2D::checkBoxCollision(const BoundingBox2D& b)
        {

            Vector2 min = m_center - m_halfdist;
            Vector2 max = m_center + m_halfdist;
            return (fabs(m_center.x - b.m_center.x) * 2 < ((m_halfdist.x * 2.0f) + (b.m_halfdist.x * 2.0f))) &&
                (fabs(m_center.y - b.m_center.y) * 2 < ((m_halfdist.y * 2.0f) + (b.m_halfdist.y * 2.0f)));
        }

        void BoundingBox2D::setHalfdist(Vector2 halfdist)
        {
            m_halfdist = halfdist;
        }

        Vector2 BoundingBox2D::getHalfdist() const
        {
            return m_halfdist;
        }

        void BoundingBox2D::setCenter(Vector2 center)
        {
            m_center = center;
        }

        Vector2 BoundingBox2D::getCenter() const
        {
            return m_center;
        }
    } /* math */
} /* aex */
