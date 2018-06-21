#include "aex/gui/GUIMath.hpp"
#include<aex/Logger.hpp>

namespace aex
{
    namespace gui
    {
        bool Scale::operator==(const Scale& s2) const
        {
            return (m_sX == s2.m_sX) && (m_sY == s2.m_sY);
        }

        bool Scale::operator!=(const Scale& s2) const
        {
            return !((*this) == s2);
        }

        float getPt(float n1, float n2, float perc)
        {
            float diff = n2 - n1;
            return n1 + (diff * perc);
        }

        Position bezierPoint(const Position& p0, const Position& p1, const Position& p2, float partClampf)
        {
            // The Green Line
            float xa = getPt(p0.x(), p1.x(), partClampf);
            float ya = getPt(p0.y(), p1.y(), partClampf);
            float xb = getPt(p1.x(), p2.x(), partClampf);
            float yb = getPt(p1.y(), p2.y(), partClampf);

            // The Black Dot
            float x = getPt(xa, xb, partClampf);
            float y = getPt(ya, yb, partClampf);

            return Position(x, y);
        }

        bool lineCollision(const Position& A1, const Position& A2, const Position& B1, const Position& B2, Position* collisionPoint)
        {
            Position a(A2 - A1);
            Position b(B2 - B1);

            double f = perpDot(a, b);
            if (!f) // lines are parallel
            {
                return false;
            }
            Position c(B2 - A2);
            double aa = perpDot(a, c);
            double bb = perpDot(b, c);

            LOG_DEBUG("aa: ", aa);
            LOG_DEBUG("bb: ", bb);

            //            if (f < 0) {
            //                if (aa > 0) return false;
            //                if (bb > 0) return false;
            //                if (aa < f) return false;
            //                if (bb < f) return false;
            //            } else {
            //                if (aa < 0) return false;
            //                if (bb < 0) return false;
            //                if (aa > f) return false;
            //                if (bb > f) return false;
            //            }

            if (collisionPoint)
            {
                double out = 1.0 - (aa / f);
                *collisionPoint = ((B2 - B1) * out) + B1;
            }
            return true;
        }

        // Finds the intersection of two lines, or returns false.
        // The lines are defined by (o1, p1) and (o2, p2).

        bool intersection(const Position& o1, const Position& p1, const Position& o2, const Position& p2, Position* r)
        {
            Position x = o2 - o1;
            Position d1 = p1 - o1;
            Position d2 = p2 - o2;

            float cross = d1.x() * d2.y() - d1.y() * d2.x();
            if (std::abs(cross) < /*EPS*/1e-8)
                return false;

            double t1 = (x.x() * d2.y() - x.y() * d2.x()) / cross;

            if (r)
                *r = o1 + d1 * t1;

            return true;
        }

        bool lineIntersection(
                double Ax, double Ay,
                double Bx, double By,
                double Cx, double Cy,
                double Dx, double Dy,
                double *X, double *Y)
        {

            double distAB, theCos, theSin, newX, ABpos;

            //  Fail if either line is undefined.
            if ((Ax == Bx && Ay == By) || (Cx == Dx && Cy == Dy)) return false;

            //  (1) Translate the system so that point A is on the origin.
            Bx -= Ax;
            By -= Ay;
            Cx -= Ax;
            Cy -= Ay;
            Dx -= Ax;
            Dy -= Ay;

            //  Discover the length of segment A-B.
            distAB = sqrt(Bx * Bx + By * By);

            //  (2) Rotate the system so that point B is on the positive X axis.
            theCos = Bx / distAB;
            theSin = By / distAB;
            newX = Cx * theCos + Cy*theSin;
            Cy = Cy * theCos - Cx*theSin;
            Cx = newX;
            newX = Dx * theCos + Dy*theSin;
            Dy = Dy * theCos - Dx*theSin;
            Dx = newX;

            //  Fail if the lines are parallel.
            if (Cy == Dy) return false;

            //  (3) Discover the position of the intersection point along line A-B.
            ABpos = Dx + (Cx - Dx) * Dy / (Dy - Cy);

            //  (4) Apply the discovered position to line A-B in the original coordinate system.
            *X = Ax + ABpos*theCos;
            *Y = Ay + ABpos*theSin;

            //  Success.
            return true;
        }

        bool lineSegmentIntersection(
                double Ax, double Ay,
                double Bx, double By,
                double Cx, double Cy,
                double Dx, double Dy,
                double *X, double *Y)
        {

            double distAB, theCos, theSin, newX, ABpos;

            //  Fail if either line segment is zero-length.
            if (((Ax == Bx) && (Ay == By)) || ((Cx == Dx) && (Cy == Dy))) return false;

            //  Fail if the segments share an end-point.
            if (((Ax == Cx) && (Ay == Cy)) || ((Bx == Cx) && (By == Cy))
                    || ((Ax == Dx) && (Ay == Dy)) || ((Bx == Dx) && (By == Dy)))
            {
                return false;
            }

            //  (1) Translate the system so that point A is on the origin.
            Bx -= Ax;
            By -= Ay;
            Cx -= Ax;
            Cy -= Ay;
            Dx -= Ax;
            Dy -= Ay;

            //  Discover the length of segment A-B.
            distAB = sqrt(Bx * Bx + By * By);

            //  (2) Rotate the system so that point B is on the positive X axis.
            theCos = Bx / distAB;
            theSin = By / distAB;
            newX = Cx * theCos + Cy*theSin;
            Cy = Cy * theCos - Cx*theSin;
            Cx = newX;
            newX = Dx * theCos + Dy*theSin;
            Dy = Dy * theCos - Dx*theSin;
            Dx = newX;

            //  Fail if segment C-D doesn't cross line A-B.
            if ((Cy < 0. && Dy < 0.) || (Cy >= 0. && Dy >= 0.)) return false;

            //  (3) Discover the position of the intersection point along line A-B.
            ABpos = Dx + (Cx - Dx) * Dy / (Dy - Cy);

            //  Fail if segment C-D crosses line A-B outside of segment A-B.
            if (ABpos < 0. || ABpos > distAB) return false;

            //  (4) Apply the discovered position to line A-B in the original coordinate system.
            *X = Ax + ABpos*theCos;
            *Y = Ay + ABpos*theSin;

            //  Success.
            return true;
        }
    }
}

