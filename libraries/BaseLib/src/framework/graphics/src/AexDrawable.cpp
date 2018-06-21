#include <aex/AexDrawable.hpp>
#include <aex/RenderingTreeBlock.hpp>

namespace aex
{
    DrawObject::~DrawObject()
    {
        for(auto& node : m_trackedNodes)
            node->setObject(nullptr);
    }
    
    void
    DrawObject::addTrackedNode(RenderingTreeNode* node)
    {
        m_trackedNodes.push_back(node);
    }

    void
    DrawObject::removeTrackedNode(RenderingTreeNode* node)
    {
        auto it = std::find_if(m_trackedNodes.begin(),
                m_trackedNodes.end(),
                [node](RenderingTreeNode* p) -> bool 
                {
                    return node == p;
                }
            );
        if(it != m_trackedNodes.end())
            m_trackedNodes.erase(it);
    }
}
