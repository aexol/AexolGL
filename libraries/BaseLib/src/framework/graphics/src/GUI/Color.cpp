#include "aex/gui/Color.hpp"

namespace aex {
    namespace gui {

        Color::Color() :
        m_r(0),
        m_g(0),
        m_b(0),
        m_a(0)
        {
        }

        Color::Color(int r, int g, int b, int a)
        {
            m_r = r / 255.0f;
            m_g = g / 255.0f;
            m_b = b / 255.0f;
            m_a = a / 255.0f;
        }

        Color::~Color()
        {
        }

        float Color::getClampfR() const
        {
            return (m_r);
        }

        float Color::getClampfG() const
        {
            return (m_g);
        }

        float Color::getClampfB() const
        {
            return (m_b);
        }

        float Color::getClampfA() const
        {
            return (m_a);
        }

        int Color::getR() const
        {
            return (int) (m_r * 255);
        }

        int Color::getG() const
        {
            return (int) (m_g * 255);
        }

        int Color::getB() const
        {
            return (int) (m_b * 255);
        }

        int Color::getA() const
        {
            return (int) (m_a * 255);
        }

        void Color::setR(int value)
        {
            m_r = value / 255.0f;
        }

        void Color::setG(int value)
        {
            m_g = value / 255.0f;
        }

        void Color::setB(int value)
        {
            m_b = value / 255.0f;
        }

        void Color::setA(int value)
        {
            m_a = value / 255.0f;
        }

        void Color::getClampfR(float value)
        {
            m_r = value;
        }

        void Color::getClampfG(float value)
        {
            m_g = value;
        }

        void Color::getClampfB(float value)
        {
            m_b = value;
        }

        void Color::getClampfA(float value)
        {
            m_a = value;
        }

        bool operator== (const Color& lhs, const Color& rhs)
        {
            return ((lhs.getClampfR() == rhs.getClampfR()) &&
                    (lhs.getClampfG() == rhs.getClampfG()) && 
                    (lhs.getClampfB() == rhs.getClampfB()) &&
                    (lhs.getClampfA() == rhs.getClampfA()) );
        }

        bool operator!= (const Color& lhs, const Color& rhs)
        {
            return !(lhs == rhs);
        }
    }
}
