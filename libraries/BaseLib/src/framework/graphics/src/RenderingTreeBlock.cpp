#include <aex/RenderingTreeBlock.hpp>

namespace aex
{
    RenderingTreeNode::RenderingTreeNode() : RenderingTreeNode(nullptr)
    {}

    RenderingTreeNode::RenderingTreeNode( RenderingTreeNode&& node):
        m_object(nullptr),
        m_children(),
        m_visibleChildren(0),
        m_visible(false)
    {
        *this = std::move(node);
    }

    RenderingTreeNode&
    RenderingTreeNode::operator=(RenderingTreeNode&& node)
    {
        if(this != &node)
        {
            m_children = std::move(node.m_children);
            m_visibleChildren = node.m_visibleChildren;
            node.m_visibleChildren = 0;
            m_visible = node.m_visible;
            setObject(node.releaseObject());
        }
        return *this;
    }

    RenderingTreeNode::RenderingTreeNode(self_ptr object) : RenderingTreeNode(object, nullptr)
    {}

    RenderingTreeNode::RenderingTreeNode(self_ptr object, RenderingTreeNode* parent) : 
        m_parent(parent),
        m_object(nullptr),
        m_visibleChildren(0)
    {
        show();
        setObject(object);
    }

    void 
    RenderingTreeNode::draw(AexRenderer* gl)
    {
        if(isVisible())
        {
            if(m_object)
                drawSelf(gl);
            else
                drawChildren(gl);
        }
    }
    void 
    RenderingTreeNode::finishdraw(AexRenderer* gl)
    {
        if(isVisible())
        {
            if(m_object)
                finishdrawSelf(gl);
        }
    }

    RenderingTreeNode*
    RenderingTreeNode::addChild(self_ptr newChild)
    {
        RenderingTreeNode* child = nullptr;
        if((child = hasChild(newChild)) == nullptr)
        {
            m_children.push_back(std::unique_ptr<RenderingTreeNode>{new RenderingTreeNode{newChild, this}});
            child = m_children.back().get();
        }
        return child;
    }

    void
    RenderingTreeNode::setObject(self_ptr object)
    {
        releaseObject();
        m_object = object;
        if(m_object)
            m_object->addTrackedNode(this);
    }

    bool
    RenderingTreeNode::remove(self_ptr child)
    {
        ChildrenVec::iterator it{}; 
        if(_hasChild(child, it))
        { 
            m_children.erase(it);
            return true;
        }
        for(auto& node: m_children)
        {
            if(node->remove(child))
                return true;
        }
        return false;
    }
    
    void
    RenderingTreeNode::remove()
    {
        releaseObject();
        if(m_parent )
        {
            m_parent->removeChild(this);
        }
    }
    
    void
    RenderingTreeNode::removeChild(RenderingTreeNode* child)
    {
        if(m_children.size() > 0)
        {
            for(ChildrenVec::iterator iter =m_children.begin(); iter!= m_children.begin(); iter++)
            {
                if((*iter).get()==child)
                {
                    iter = m_children.erase(iter);
                }
            }
            
            if(m_children.size() == 0)
            {
                remove();
            }
        }
    }

    void
    RenderingTreeNode::clear()
    {
        m_children.clear();
    }

    void
    RenderingTreeNode::hide()
    {
        if(m_visible)
        {
            m_visible = false;
            if(m_parent)
                m_parent->childBecameInvisible();
        }
    }

    void
    RenderingTreeNode::show()
    {
        if(!m_visible)
        {
            m_visible = true;
            if(m_parent)
                m_parent->childBecameVisible();
        }
    }

    RenderingTreeNode*
    RenderingTreeNode::findFirst(self_ptr object)
    {
        ChildrenVec::iterator it{}; 
        if(m_object == object)
        { 
            return this;
        }
        for(auto& node: m_children)
        {
            RenderingTreeNode* element = nullptr;
            if((element = node->findFirst(object)) != nullptr)
                return element;
        }
        return nullptr;
    }

    RenderingTreeNode*
    RenderingTreeNode::hasChild(self_ptr object)
    {
        RenderingTreeNode* rt = nullptr;
        ChildrenVec::iterator it{}; 
        if(_hasChild(object, it))
            rt = it->get();
        return rt;
    }

    RenderingTreeNode::~RenderingTreeNode()
    {
        releaseObject();
        hide();
    }

    bool
    RenderingTreeNode::_hasChild(self_ptr object, ChildrenVec::iterator& it)
    {
        it = std::find_if(m_children.begin(), m_children.end(), [object](const Child& node) -> bool{ return node->m_object == object;});
        if(it == m_children.end())
            return false;
        return true;
    }

    void
    RenderingTreeNode::childBecameInvisible()
    {
        m_visibleChildren--;
    }

    void
    RenderingTreeNode::childBecameVisible()
    {
        m_visibleChildren++;
    }

    void
    RenderingTreeNode::drawSelf(AexRenderer* gl)
    {
        m_object->draw(gl);
        drawChildren(gl);
    }

    void
    RenderingTreeNode::finishdrawSelf(AexRenderer* gl)
    {
        m_object->finishdraw(gl);
    }

    void
    RenderingTreeNode::drawChildren(AexRenderer* gl)
    {
        for(size_t i = 0 ; i< m_children.size(); ++i)
        {
            m_children[i]->draw(gl);
            m_children[i]->finishdraw(gl);
        }
    }

    RenderingTreeNode::self_ptr
    RenderingTreeNode::releaseObject()
    {
        if(m_object)
        {
            m_object->removeTrackedNode(this);
        }
        self_ptr prev = m_object;
        m_object = nullptr;
        return prev;
    }

    bool
    RenderingTreeNode::isVisible()
    {
        return (m_visible && m_visibleChildren) || (m_visible && m_children.size() == 0);
    }
    
    
    void RenderingTreeNode::logBranch()
    {
        LOG_DEBUG("Node: ", this);

        if(m_parent)
        {
            m_parent->logBranch();
        }
    }

    RenderingTreeBlockImpl::RenderingTreeBlockImpl(AexRenderer* gl):
    m_gl(gl)
    {
    }

    bool
    RenderingTreeBlockImpl::init()
    {
        return m_gl != nullptr;
    }

    void
    RenderingTreeBlockImpl::beginFrame()
    {
        m_root.draw(m_gl);
    }

    void
    RenderingTreeBlockImpl::endFrame()
    {
        m_root.finishdraw(m_gl);
    }

    void
    RenderingTreeBlockImpl::setContext(AexRenderer* gl)
    {
        m_gl = gl;
    }
}
