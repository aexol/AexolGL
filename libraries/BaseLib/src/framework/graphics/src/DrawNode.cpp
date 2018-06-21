/*
 * File:   ObjectRenderNode.h
 * Author: mritke
 *
 * Created on November 4, 2013, 11:14 AM
 */

#include <aex/DrawNode.hpp>
#include <aex/Logger.hpp>

namespace aex {

    DrawNode::DrawNode() :
    m_parent(nullptr),
    m_isRemovable(true),
    m_isInTree(true)
    {

    }

    DrawNode::DrawNode(DrawObject_ptr object) :
    m_object(object),
    m_parent(nullptr),
    m_isRemovable(true),
    m_isInTree(true)
    {

    }

    DrawNode::DrawNode(DrawNode* parent) :
    m_parent(parent),
    m_isRemovable(true),
    m_isInTree(true)
    {

    }

    DrawNode::DrawNode(DrawNode* parent, DrawObject_ptr object) :
    m_object(object),
    m_parent(parent),
    m_isRemovable(true),
    m_isInTree(true)
    {
    }

    void
    DrawNode::draw_self(aex::AexRenderer* gl, DrawObject * const object)
    {
        object->draw(gl);
        draw_children(gl);
        object->finishdraw(gl);
    }

    void
    DrawNode::draw_children(aex::AexRenderer* gl)
    {
        const std::lock_guard<std::mutex> lock{Active};


        for (size_t iter = 0; iter < m_children.size(); ++iter)
        {
            m_children[iter]->draw(gl);
        }
    }

    void
    DrawNode::draw(AexRenderer* gl)
    {
        if (getIsVisible())
        {
            if (m_object)
            {
                draw_self(gl, m_object.get());
            } else
            {
                draw_children(gl);
            }
        }
    }

    void
    DrawNode::setObject(DrawObject_ptr object)
    {

        m_object = object;

    }

    void
    DrawNode::finishdraw()
    {


    }

    void
    DrawNode::update()
    {
        m_needsUpdate = false;
    }

    void
    DrawNode::addChildFromObject(DrawObject_ptr newChild) //TODO: RAW pointer
    {
        //DrawNode* newNode = new DrawNode(shared_from_this(), newChild);
        DrawNode_ptr newNodeptr = make_shared<DrawNode>(this, newChild);

        //        Active.lock();
        std::lock_guard<std::mutex> lock{Active};
        m_children.push_back(newNodeptr);

    }

    int DrawNode::childDepth(int dep)
    {
        if (m_parent)
        {
            dep++;
            return m_parent->childDepth(dep);
        } else
        {
            return dep;
        }
    }

    void
    DrawNode::addChild(DrawNode_ptr newChild)
    {
        //m_bounds = getBounds();
        newChild->setParent(this);
        m_children.push_back(newChild);
    }

    void
    DrawNode::invisible()
    {

        setIsVisible(false);

        if (m_parent)
        {
            m_parent->childInvisible();
        }

    }

    void
    DrawNode::childInvisible()
    {
        bool temp = false;

        for (auto child : m_children)
        {
            temp = temp || child->getIsVisible();
        }

        if (!temp)
        {
            invisible();
        }
    }

    void
    DrawNode::visible()
    {

        setIsVisible(true);

        if (m_parent)
        {
            m_parent->visible();
        }

    }

    void
    DrawNode::remove()
    {
        if (m_parent != nullptr && m_isRemovable)
        {
            m_parent->removeChild(shared_from_this());
        }
    }

    void
    DrawNode::removeChild(DrawNode_ptr child)
    {
        if (m_children.size() > 0)
        {
            m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
            if (m_children.size() == 0)
            {
                remove();
            }
        }
    }

    void
    DrawNode::removeAll()
    {
        if (m_children.size() > 0)
        {

            for (auto iter = m_children.begin(); iter != m_children.end(); ++iter)
            {
                (*iter)->removeAll();
            }
        }

        if (m_children.size() == 0)
        {
            remove();
        }
    }

    DrawNode_ptr
    DrawNode::findNodeInChilds(DrawObject_ptr object)
    {

        if (m_object == object)
        {
            return shared_from_this();
        } else if (m_children.size() > 0)
        {
            for (auto child : m_children)
            {

                if (child->thisObject(object) != nullptr)
                {
                    return child->thisObject(object);
                }
            }

            return nullptr;

        } else
        {
            return nullptr;
        }

    }

    DrawNode_ptr
    DrawNode::findNodeRecursive(DrawObject_ptr object)
    {

        if (m_object == object)
        {
            return shared_from_this();
        } else if (m_children.size() > 0)
        {
            for (auto child : m_children)
            {

                if (child->findNodeRecursive(object) != nullptr)
                {
                    return child->findNodeRecursive(object);

                }
            }

            return nullptr;

        } else
        {
            return nullptr;
        }

    }

    DrawNode_ptr
    DrawNode::thisObject(DrawObject_ptr object)
    {
        if (m_object == object)
        {
            return shared_from_this();
        } else
        {
            return nullptr;
        }
    }

    int DrawNode::nodeCount()
    {
        int i = 0;

        if (m_children.size() > 0)
        {
            i = m_children.size();

            for (auto child : m_children)
            {
                i += child->nodeCount();
            }
        }

        return i;
    }

    void DrawNode::needsUpdate()
    {
        m_needsUpdate = true;
    }

    void DrawNode::setParent(DrawNode* parent)
    {
        m_parent = parent;
    }

    DrawNode::~DrawNode()
    {

        //LOG_DEBUG("node dies ", ChildDepth(0));
        if (!m_parent)
        {
            LOG_DEBUG("root dies");
        }
    }
    
    void DrawNode::logTree(int i)
    {
        {
            std::stringstream offset;
            offset<<i;
            for (int o = 0; o < i; ++o)
            {
                offset << "    ";
            }

            LOG_DEBUG(offset.str(), "|---", this,"  ", m_object.get());
        }
        
        if (m_children.size() > 0)
        {
            for (auto child : m_children)
            {
                child->logTree(i + 1);
            }
        }

    }

    void DrawNode::logBranch()
    {
        LOG_DEBUG("Node: ", this);

        if (m_parent)
        {
            m_parent->logBranch();
        }
    }
}
