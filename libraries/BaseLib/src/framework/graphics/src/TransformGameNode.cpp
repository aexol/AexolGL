//
//  TransformGameNode.cpp
//  AexolGL
//
//  Created by Kamil Matysiewicz on 30.10.2013.
//
//

#include <aex/TransformGameNode.hpp>

#include "aex/Mesh.hpp"

namespace aex
{

    TRANSFORM_EVENT::TRANSFORM_EVENT(aex::math::Vector3 translate, aex::math::Quaternion orientation, aex::math::Vector3 scale) :
        m_translate(translate),
        m_scale(scale),
        m_orientation_offset(orientation)
    {

    }

    TransformGameNode::TransformGameNode(GameObject* parent) :
        GameObjectNode(parent),
        m_updated(true),
        m_position(),
        m_size(),
        m_orientation(),
        m_thisTreeNode(nullptr)
    {

    }

    TransformGameNodePtr
    TransformGameNode::makeTransformGameNode(GameObject* parent, TransformGameNodePtr tree_parent)
    {
        TransformGameNodePtr ret;
        ret.reset(new TransformGameNode(parent));

        if(tree_parent)
        {
            auto temp = TransformTreeNode::makeTreeNodePtr(ret->getPtr(), tree_parent->m_thisTreeNode);
            ret->m_thisTreeNode.swap(temp);
        }
        else
        {
            auto temp = TransformTreeNode::makeTreeNodePtr(ret->getPtr());
            ret->m_thisTreeNode.swap(temp);
        }

        return ret;
    }

    void
    TransformGameNode::move(const aex::math::Vector3& translate)
    {
        m_position += translate;

        /* TO-DO - wrap event broadcast for smaller code */
        TRANSFORM_EVENT* transformEvent = new TRANSFORM_EVENT(translate);
        Post(transformEvent);
        delete transformEvent;

        for(auto a : m_thisTreeNode->getChildren())
        {
            a->get()->move(translate);
        }

    }

    void
    TransformGameNode::setTranslation(const aex::math::Vector3& translate)
    {
        m_position = translate;

        /* TO-DO - wrap event broadcast for smaller code */
        TRANSFORM_EVENT* transformEvent = new TRANSFORM_EVENT(translate);
        Post(transformEvent);
        delete transformEvent;

        for(auto a : m_thisTreeNode->getChildren())
        {
            a->get()->move(translate);
        }

    }

    void
    TransformGameNode::scale(const aex::math::Vector3& scale)
    {
        m_size *= scale;
        /* TO-DO - wrap event broadcast */
        TRANSFORM_EVENT* transformEvent = new TRANSFORM_EVENT(aex::math::Vector3(0.0f, 0.0f, 0.0f), aex::math::Quaternion(), scale);
        Post(transformEvent);
        delete transformEvent;

        for(auto a : m_thisTreeNode->getChildren())
        {
            a->get()->scale(scale);
        }
    }

    aex::math::Quaternion TransformGameNode::getOrientation() const
    {
        return m_orientation;
    }

    aex::math::Vector3 TransformGameNode::getSize() const
    {
        return m_size;
    }

    aex::math::Vector3 TransformGameNode::getPosition() const
    {
        return m_position;
    }

    bool TransformGameNode::isUpdated() const
    {
        return m_updated;
    }

    void TransformGameNode::setOrientation(const aex::math::Quaternion& quat)
    {
        m_orientation = quat;
        /* TO-DO - wrap event broadcast */
        TRANSFORM_EVENT* transformEvent = new TRANSFORM_EVENT(aex::math::Vector3(0.0f, 0.0f, 0.0f), quat);
        Post(transformEvent);
        delete transformEvent;

        for(auto a : m_thisTreeNode->getChildren())
        {
            a->get()->setOrientation(m_orientation);
        }
    }

    void TransformGameNode::setOrientation(const aex::math::Vector3& vec)
    {
        aex::math::Quaternion quat = aex::math::getQuaternionFromEulerDeg(vec.x, vec.y, vec.z);
        m_orientation = quat;
        /* TO-DO - wrap event broadcast */
        TRANSFORM_EVENT* transformEvent = new TRANSFORM_EVENT(aex::math::Vector3(0.0f, 0.0f, 0.0f), quat);
        Post(transformEvent);
        delete transformEvent;

        for(auto a : m_thisTreeNode->getChildren())
        {
            a->get()->setOrientation(m_orientation);
        }

    }

    void
    TransformGameNode::rotate(const aex::math::Quaternion& quat)
    {
        m_orientation *= quat;
        /* TO-DO - wrap event broadcast */
        TRANSFORM_EVENT* transformEvent = new TRANSFORM_EVENT(aex::math::Vector3(0.0f, 0.0f, 0.0f), quat);
        Post(transformEvent);
        delete transformEvent;

        for(auto a : m_thisTreeNode->getChildren())
        {
            a->get()->rotate(quat);
        }
    }

    void
    TransformGameNode::rotate(const aex::math::Vector3& vec)
    {
        aex::math::Quaternion quat = aex::math::getQuaternionFromEulerDeg(vec.x, vec.y, vec.z);
        m_orientation *= quat;
        /* TO-DO - wrap event broadcast */
        TRANSFORM_EVENT* transformEvent = new TRANSFORM_EVENT(aex::math::Vector3(0.0f, 0.0f, 0.0f), quat);
        Post(transformEvent);
        delete transformEvent;

        for(auto a : m_thisTreeNode->getChildren())
        {
            a->get()->rotate(quat);
        }
    }
}
