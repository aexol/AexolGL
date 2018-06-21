#include "aex/gui/SimplePanel.hpp"

namespace aex
{
    namespace gui
    {
        void SimplePanel::refresh(int flags)
        {
            //if (checkFlag(FLAG::POSITION_CHANGED | FLAG::SIZE_CHANGED, flags))
            //{
                for (unsigned i = 0; i < m_components.size(); i++)
                {
                    m_components[i]->setArea(getArea()); // Base Panels gives for childrens the same area
                }
            //}
        }
    }
}