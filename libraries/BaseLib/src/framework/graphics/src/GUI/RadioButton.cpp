#include "aex/gui/RadioButton.hpp"

namespace aex
{
    namespace gui
    {

        RadioButton::RadioButton(HudComponentPtr on, HudComponentPtr off, bool isOn) :
        m_onComp(on),
        m_offComp(off),
        m_bIsOn(isOn)
        {
            setUsing(*m_onComp);
            m_onComp->setVisible(isOn);
            m_offComp->setVisible(!isOn);
            m_drawComp = m_onComp;
        }

        void RadioButton::setOnChangeAction(const std::function<void(bool)>& onChange)
        {
            m_onChange = onChange;
        }

        bool RadioButton::isOn() const
        {
            return m_bIsOn;
        }

        void RadioButton::draw(aex::AexRenderer* gl)
        {
            if (m_onComp->isVisible())
            {
                m_drawComp = m_onComp;
            } else
            {
                m_drawComp = m_offComp;
            }

            m_drawComp->draw(gl);
        }

        void RadioButton::finishdraw(aex::AexRenderer* gl)
        {
            m_drawComp->finishdraw(gl);
        }

        void RadioButton::needsUpdate()
        {
            m_onComp->needsUpdate();
            m_offComp->needsUpdate();
        }

        void RadioButton::refresh(int flags)
        {
            m_onComp->setUsing(*this);
            m_offComp->setUsing(*this);
            m_onComp->forceRefresh();
            m_offComp->forceRefresh();
        }

        void RadioButton::touchDown(float x, float y, int fingerId)
        {
            if (isHitted(Position(x, y)))
            {
                // on another thread so cant change wsk
                m_bIsOn = !m_bIsOn;

                m_onComp->setVisible(m_bIsOn);
                m_offComp->setVisible(!m_bIsOn);

                if (m_onChange)
                    m_onChange(m_bIsOn);
            }
        }

        ComponentPtr RadioButton::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
        {
            if (isHitted(point))
            {
                if (type == FOCUS_TYPE::TOUCH)
                {
                    focusMe(this);
                    return this;
                }
            }

            return nullptr;
        }
    }
}