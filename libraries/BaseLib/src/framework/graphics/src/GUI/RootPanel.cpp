#include "aex/gui/RootPanel.hpp"
#include "aex/gui/Frame.hpp"

namespace aex
{
    namespace gui
    {
        void RootPanel::draw(AexRenderer* gl)
        {   
            m_prevDepth = gl->setDepthTestState(false);
            if (needRefresh())
            {
                forceRefresh();
            }
            Panel::draw(gl);
        }

        void RootPanel::finishdraw(AexRenderer* gl)
        {
            Panel::finishdraw(gl);
            gl->setDepthTestState(m_prevDepth);
        }
    }
}