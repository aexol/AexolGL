#include <aex/EventRelayNode.hpp>

namespace aex
{


    EventRelayNode::~EventRelayNode()
    {
        LOG_DEBUG("m_globalLink->unregisterObject(this) ", this, " ", m_parent);
        m_globalLink->unregisterObject(this);
        m_globalLink->unregisterObject(m_parent);
    }


}
