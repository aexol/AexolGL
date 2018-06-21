#ifndef AEX_GUI_MATH_HPP
#define AEX_GUI_MATH_HPP

#include <iostream>
#include <cmath>

#include <aex/Common.hpp>
#include "GUIDeftypes.hpp"

namespace aex {
    namespace gui {

        template <typename T> std::ostream& operator<<(std::ostream& out, const GPosition2D<T>& pos);

        template <typename T>
        class GPosition2D {
        public:
            static T length(const GPosition2D<T>& from, const GPosition2D<T>& to) {
                return std::sqrt(((to.m_x - from.m_x)*(to.m_x - from.m_x)) + ((to.m_y - from.m_y)*(to.m_y - from.m_y)));
            }
        public:

            GPosition2D() : m_x(0), m_y(0) {
            }

            GPosition2D(T x, T y) : m_x(x), m_y(y) {
            }

            void move(T x, T y) {
                m_x += x;
                m_y += y;
            }

            T x() const {
                return m_x;
            }

            T y() const {
                return m_y;
            }

            void setX(T x) {
                m_x = x;
            }

            void setY(T y) {
                m_y = y;
            }

            GPosition2D<T> operator+(const GPosition2D<T>& p2) const {
                return GPosition2D<T>(m_x + p2.m_x, m_y + p2.m_y);
            }

            GPosition2D<T> operator-(const GPosition2D<T>& p2) const {
                return GPosition2D<T>(m_x - p2.m_x, m_y - p2.m_y);
            }

            GPosition2D<T> operator*(double val) const {
                return GPosition2D<T>(m_x* val, m_y * val);
            }

            bool operator==(const GPosition2D<T>& p2) const {
                return (m_x == p2.m_x) && (m_y == p2.m_y);
            }

            bool operator!=(const GPosition2D<T>& p2) const {
                return !((*this) == p2);
            }
            template <typename> friend class GArea2D;
            template <typename> friend class GInterspace2D;
            friend std::ostream& operator<< <> (std::ostream& out, const GPosition2D<T>& pos);
        protected:
            T m_x, m_y;
        };

        template <typename T> std::ostream& operator<<(std::ostream& out, const GPosition2D<T>& pos) {
            out << "(" << pos.m_x << "," << pos.m_y << ")";
            return out;
        }

        class Scale {
        public:

            Scale() : m_sX(1.0f), m_sY(1.0f), m_center(true) {
            }

            Scale(float sX, float sY, bool center = true) : m_sX(sX), m_sY(sY), m_center(center) {
            }

            float sX() const {
                return m_sX;
            }

            float sY() const {
                return m_sY;
            }

            bool getNeedCenter() const {
                return m_center;
            }
            bool operator==(const Scale& s2) const;
            bool operator!=(const Scale& s2) const;
            template <typename> friend class GArea2D;
            template <typename> friend class GInterspace2D;
        protected:
            float m_sX, m_sY;
            bool m_center;
        };

        template <typename T> std::ostream& operator<<(std::ostream& out, const GSize2D<T>& s);
        template <typename T> class GSize2D {
        public:

            GSize2D() : m_w(0), m_h(0) {
            }

            GSize2D(T w, T h) : m_w(w), m_h(h) {
            }

            T w() const {
                return m_w;
            }

            T h() const {
                return m_h;
            }

            void setW(const T& w)
            {
                m_w = w;
            }

            void setH(const T& h)
            {
                m_h = h;
            }

            void scale(const Scale& s) {
                m_w *= s.sX();
                m_h *= s.sY();
            }
            template <typename> friend class GArea2D;
            template <typename> friend class GInterspace2D;

            bool operator==(const GSize2D<T>& size) const {
                return ((m_w == size.m_w) && (m_h == size.m_h));
            }

            bool operator!=(const GSize2D<T>& size) const {
                return !((*this) == size);
            }
            friend std::ostream& operator<< <>(std::ostream& out, const GSize2D<T>& s);
        protected:
            T m_w, m_h;
        };
        template <typename T> std::ostream& operator<<(std::ostream& out, const GSize2D<T>& s) {
            out << "(" << s.w() << "," << s.h() << ")";
            return out;
        }

        template <typename T> std::ostream& operator<<(std::ostream& out, const GArea2D<T>& a);
        template <typename T>
        class GArea2D {
        public:
            template <typename> friend class GInterspace2D;

            GArea2D() : m_p(GPosition2D<T>()), m_s(GSize2D<T>()) {
            }

            GArea2D(const GPosition2D<T>& p, const GSize2D<T>& s) : m_p(p), m_s(s) {
            }

            GArea2D(const GSize2D<T>& s) : m_p(GPosition2D<T>()), m_s(s) {
            }

            const GPosition2D<T>& p() const {
                return m_p;
            }

            const GSize2D<T>& s() const {
                return m_s;
            }

            void setPosition(const GPosition2D<T>& p) {
                m_p = p;
            }

            void setSize(const GSize2D<T>& s) {
                m_s = s;
            }

            bool isPointInArea(const GPosition2D<T>& p) const {
                if ((p.x() >= m_p.x()) && (p.x() <= (m_p.x() + m_s.w())))
                    if ((p.y() >= m_p.y()) && (p.y() <= (m_p.y() + m_s.h())))
                        return true;

                return false;
            }

            bool intersectWith(const GArea2D<T>& a) const {
                T sumRW = (s().w()/2) + (a.s().w()/2);
                T lengthX = std::fabs(a.center().x() - center().x());

                T sumRH = (s().h()/2) + (a.s().h()/2);
                T lengthY = std::fabs(a.center().y() - center().y());

                bool bW = lengthX <= sumRW;
                bool bH = lengthY <= sumRH;
                return bW && bH;
            }

            bool contains(const GArea2D<T>& a) const
            {
                return isPointInArea(a.leftUp()) && isPointInArea(a.rightDown());
            }

            void move(const GPosition2D<T>& p) {
                m_p = m_p + p;
            }

            void scale(const Scale& s) {
                if (s.getNeedCenter()) {
                    GSize2D<T> size = m_s;
                    m_s.scale(s);
                    float dX = (m_s.w() - size.w()) / 2.0f;
                    float dY = (m_s.h() - size.h()) / 2.0f;
                    GPosition2D<T> movement(-dX, -dY);
                    m_p = m_p + movement;
                } else {
                    m_s.scale(s);
                }
            }

            GArea2D<T> inPosition(const GPosition2D<T>& p) const {
                Area tmp(*this);
                tmp.setPosition(p);
                return tmp;
            }

            GPosition2D<T> leftUp() const {
                return m_p;
            }

            GPosition2D<T> leftDown() const {
                return GPosition2D<T>(m_p.x(), m_p.y() + m_s.h());
            }

            GPosition2D<T> rightDown() const {
                return GPosition2D<T>(m_p.x() + m_s.w(), m_p.y() + m_s.h());
            }

            GPosition2D<T> rightUp() const {
                return GPosition2D<T>(m_p.x() + m_s.w(), m_p.y());
            }

            GPosition2D<T> center() const {
                return GPosition2D<T>(m_p.x() + m_s.w()/2, m_p.y() + m_s.h()/2);
            }

            GArea2D<T> operator+(const GInterspace2D<T>& is) const { // Area + margins
                GPosition2D<T> p = m_p;
                p.m_x += -is.l();
                p.m_y += -is.t();
                GSize2D<T> s = m_s;
                s.m_w += (is.l() + is.r());
                s.m_h += (is.t() + is.b());
                return Area(p, s);
            }

            GArea2D<T> operator-(const GInterspace2D<T>& is) const { // Area - paddings
                GPosition2D<T> p = m_p;
                p.m_x += is.l();
                p.m_y += is.t();
                GSize2D<T> s = m_s;
                s.m_w += (-is.l() - is.r());
                s.m_h += (-is.t() - is.b());
                return Area(p, s);
            }
            friend std::ostream& operator<< <>(std::ostream& out, const GArea2D<T>& a);
        protected:
            GPosition2D<T> m_p;
            GSize2D<T> m_s;
        };

        template <typename T>
        std::ostream& operator<<(std::ostream& out, const GArea2D<T>& a) {
            out << "p(" << a.p().x() << "," << a.p().y() << "), s(" << a.s().w() << "," << a.s().h() << ")";
            return out;
        }

        template <typename T>
        class GInterspace2D {
        public:

            GInterspace2D() : m_l(0), m_t(0), m_r(0), m_b(0) {
            }

            GInterspace2D(T left, T top, T right, T bottom) :
            m_l(left), m_t(top), m_r(right), m_b(bottom) {
            }

            T l() const {
                return m_l;
            }

            T t() const {
                return m_t;
            }

            T r() const {
                return m_r;
            }

            T b() const {
                return m_b;
            }
            bool operator==(const GInterspace2D<T>& i2) const
            {
                return (m_l == i2.m_l) && (m_t == i2.m_t) && (m_r == i2.m_r) && (m_b == i2.m_b);
            }
            bool operator!=(const GInterspace2D<T>& i2) const
            {
                return !((*this) == i2);
            }
            template <typename> friend class GArea2D;
        protected:
            T m_l; // left
            T m_t; // top
            T m_r; // right
            T m_b; // bottom
        };

        Position bezierPoint(const Position& p0, const Position& p1, const Position& p2, float partClampf);

        inline double dot(const Position& a, const Position& b) {
            return (a.x() * b.x()) + (a.y() * b.y());
        }

        inline double perpDot(const Position& a, const Position& b) {
            return (a.y() * b.x()) - (a.x() * b.y());
        }

        bool lineCollision(const Position& A1, const Position& A2, const Position& B1, const Position& B2, Position* collisionPoint);
        bool intersection(const Position& o1, const Position& p1, const Position& o2, const Position& p2, Position* r);
        bool lineIntersection(double Ax, double Ay, double Bx, double By, double Cx, double Cy, double Dx, double Dy, double *X, double *Y);
        bool lineSegmentIntersection(double Ax, double Ay, double Bx, double By, double Cx, double Cy, double Dx, double Dy, double *X, double *Y);
    }
}

#endif
