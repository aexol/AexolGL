/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**/
#include <aex/Line2D.hpp>
#include <aex/Vector2.hpp>
namespace aex {
    namespace math {
        Line2D::Line2D()
        {

        }

        Line2D::Line2D(float ca, float cb) : a(ca), b(cb)
        {

        }

        Line2D::Line2D(const Vector2& pointA, const Vector2& pointB)
        {
            resolve(pointA, pointB);
        }

        bool Line2D::isPointUnder(Vector2 point)
        {
            return (a * point.x) +b < point.y;
        }

        void Line2D::resolve(const Vector2& pointA, const Vector2& pointB)
        {
            a = (pointB.y - pointA.y) / (pointB.x - pointA.x);
            b = pointA.y - a * pointA.x;
        }

        Vector2 Line2D::func(float x) const
        {
            return Vector2(x, (a * x) + b);
        }

        Vector2 Line2D::funcOnParallerDist(float x, float dist)
        {
            Vector2 primPoint = func(x);
            Line2D perpendicular = perpendicularAtPoint(Vector2(0.0, 0.0));

            Vector2 secPoint = perpendicular.func(1.0).unit();
            perpendicular = perpendicularAtPoint(primPoint);
            Line2D parallel = parallelAtPoint(primPoint + (secPoint * dist));

            return perpendicular.intersect(parallel);
        }

        Line2D Line2D::parallelAtPoint(const Vector2& point)const
        {
            float na = a;
            float nb = point.y - (na * (point.x));
            return Line2D(na, nb);
        }

        Line2D Line2D::perpendicularAtPoint(const Vector2& point)const
        {
            float dir =1.0f; 
            if(a < 0.0f)
            {
                dir = -1.0f;
            }

            float na = -(1.0 / a)*dir;
            float nb = point.y - (na * (point.x));
            return Line2D(na, nb);
        }

        float Line2D::perpendicularDistance(const Vector2& point) const
        {
            Line2D perpendicular = perpendicularAtPoint(point);
            Vector2 selfpoint = intersect(perpendicular);
            return (selfpoint - point).length();
        }

        Vector2 Line2D::intersect(const Line2D& line)const
        {
            Vector2 ret;
            if (a != line.a)
            {
                float deltaA = a - line.a;
                float deltaB = line.b - b;

                ret.x = (1.0 / deltaA) * deltaB;
                ret.y = (a * ret.x) + b;
            }
            return ret;
        }
    } /* math */
} /* aex */
