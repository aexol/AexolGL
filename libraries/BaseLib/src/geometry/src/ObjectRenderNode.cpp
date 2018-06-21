/*
 * File:   ObjectRenderNode.h
 * Author: mritke
 *
 * Created on November 4, 2013, 11:14 AM
 */



#include <aex/ObjectRenderNode.hpp>
#include <aex/Logger.hpp>



namespace aex {

    ObjectRenderNode::ObjectRenderNode(GameObject* parent) :
    GameObjectNode(parent),
    m_gameObjectRenderElements(),
    m_rendererRootPtr(),
    n_elementsNames(),
    m_updated(false),
    m_branchInTree(false),
    m_isVisible(false)
    {


    }

    ObjectRenderNode::ObjectRenderNode() :
    GameObjectNode(nullptr),
    m_gameObjectRenderElements(),
    m_rendererRootPtr(),
    n_elementsNames(),
    m_updated(false),
    m_branchInTree(false),
    m_isVisible(false)
    {

    }

    ObjectRenderNode::~ObjectRenderNode()
    {
        if (m_branchInTree)
        {
            removeBranch();
        }
    }

    ObjectRenderNode_ptr
    ObjectRenderNode::makeRenderNode()
    {
        return sharedCreator<ObjectRenderNode>::makeSharedOfBase();
    }

    ObjectRenderNode_ptr
    ObjectRenderNode::makeRenderNode(GameObject* parent)
    {
        return sharedCreator<ObjectRenderNode>::makeSharedOfBase(parent);
    }

    void ObjectRenderNode::update(aexTimeUnit time)
    {
    }

    bool ObjectRenderNode::findNode()
    {
        return false;
    }

    void ObjectRenderNode::setRendererRootPtr(DrawNode_ptr rendererRootPtr)
    {
        m_rendererRootPtr = rendererRootPtr;
    }

    DrawNode_ptr ObjectRenderNode::getRendererRootPtr() const
    {
        return m_rendererRootPtr;
    }

    void ObjectRenderNode::setBranchInTree(bool branchInTree)
    {
        m_branchInTree = branchInTree;
    }

    bool ObjectRenderNode::isBranchInTree() const
    {
        return m_branchInTree;
    }

    void ObjectRenderNode::startDrawing()
    {
        if (!m_isVisible)
        {
            if (m_branchInTree)
            {
                m_gameObjectRenderBranchLeafs->visible();

            } else
            {
                addElementsToRenderTree();
                m_gameObjectRenderBranchLeafs->visible();

            }

            m_isVisible = true;
        }

    }

    void ObjectRenderNode::startDrawing(DrawNode_ptr rendererRootPtr)
    {
        setRootRenderNode(rendererRootPtr);
        startDrawing();
    }

    void ObjectRenderNode::stopDrawing()
    {

        if (m_branchInTree && m_isVisible)
        {
            m_gameObjectRenderBranchLeafs->invisible();
        } else
        {

        }

        m_isVisible = false;
    }

    void ChangeRenderObject(aex::shared_ptr<DrawNode> node, DrawObject_ptr object)
    {

        node->setObject(object);

    }

    void ObjectRenderNode::addRenderElement(DrawObject_ptr newNode)
    {

        m_gameObjectRenderElements.push_back(newNode);

    }

    void ObjectRenderNode::addElementToRenderTree(DrawObject_ptr object)
    {

        // m_rendererRootPtr->addChild(object);

    }

    void ObjectRenderNode::addElementsToRenderTree(DrawNode_ptr root, bool front)
    {

        m_rendererRootPtr = root;
        addElementsToRenderTree();

    }

    void ObjectRenderNode::addElementsToRenderTree(bool front)
    {
        if (m_rendererRootPtr != nullptr)
        {

            DrawNode_ptr tempTarget = m_rendererRootPtr;
            bool first = true;

            if (m_gameObjectRenderElements.size() > 0)
            {
                for (auto element : m_gameObjectRenderElements)
                {
                    if (tempTarget->findNodeRecursive(element) == nullptr && first)
                    {

                        tempTarget->addChildFromObject(element);
#ifdef DEBUG
                        //LOG_DEBUG("Element not found in tree: ADD in");
#endif
                        tempTarget = tempTarget->findNodeRecursive(element);

                    } else if (tempTarget->findNodeInChilds(element) == nullptr)
                    {

                        tempTarget->addChildFromObject(element);
#ifdef DEBUG
                        // LOG_DEBUG("Element not found in tree: ADD in");
#endif
                        tempTarget = tempTarget->findNodeInChilds(element);

                    } else if (first)
                    {
                        tempTarget = tempTarget->findNodeRecursive(element);

#ifdef DEBUG
                        // LOG_DEBUG("Element found in tree:Check Next element");
#endif

                    } else
                    {

                        tempTarget = tempTarget->findNodeInChilds(element);
#ifdef DEBUG
                        // LOG_DEBUG("Element found in tree:Check Next element.");
#endif

                    }

                    if (first)
                    {
                        m_gameObjectRenderBranchRoot = tempTarget;
                        first = false;
                    }
                }

                m_gameObjectRenderBranchLeafs = tempTarget;
                m_branchInTree = true;
            }

            /*if(tempTarget!=m_rendererRootPtr)
            {}*/

        } else
        {
            LOG_ERR("Render Root Not Set!!!");
        }
    }

    void SortDrawElements()
    {


    }

    void ObjectRenderNode::getRenderElement(aex::string name)
    {

        /*Manager& mgr = aex::MeshManager::getInstance();

         AddRenderElement(mgr.GetElement(name));



         */

    }

    void ObjectRenderNode::getRenderRoot(EventHandler* eventHandler)
    {
        iWantRenderRoot event(this);
        eventHandler->handleEvent(&event);
    }

    int ObjectRenderNode::getBranchSize()
    {
        return m_gameObjectRenderElements.size();
    }

    void ObjectRenderNode::updateRenderTree()
    {



    }

    void ObjectRenderNode::clear()
    {
        removeBranch();
        m_gameObjectRenderBranchRoot.reset();
        m_gameObjectRenderElements.clear();
        m_gameObjectRenderBranchLeafs.reset();
        m_rendererRootPtr.reset();
    }

    void ObjectRenderNode::addBranch(DrawObject_ptr object)
    {

        m_gameObjectRenderBranchRoot->addChildFromObject(object);
    }

    void ObjectRenderNode::removeBranch()
    {
        stopDrawing();

        if (m_branchInTree)
        {
            m_gameObjectRenderBranchLeafs->invisible();
            m_gameObjectRenderBranchLeafs->remove();

            //m_gameObjectRenderBranchLeaf->remove();
        }

        m_branchInTree = false;
        m_isVisible = false;
    }

    void ObjectRenderNode::makeBranch()
    {

    }

    void ObjectRenderNode::swapElementsInBranch(DrawObject_ptr objectInBranch, DrawObject_ptr objetToSwapIn, bool leaveInPlace)
    {
        bool isNodeFound = false;

        for (size_t i = 0; i < m_gameObjectRenderElements.size(); ++i)
        {
            if (m_gameObjectRenderElements[i] == objectInBranch)
            {
                m_gameObjectRenderElements[i] = objetToSwapIn;
                isNodeFound = true;
                break;
            }
        }


        if (isNodeFound)
        {
            if (m_branchInTree)
            {
                if (!leaveInPlace)
                {
                    bool visibleTemp = m_isVisible;
                    removeBranch();
                    addElementsToRenderTree();
                    if (visibleTemp)
                    {
                        startDrawing();
                    }

                } else
                {
                    if (m_gameObjectRenderBranchRoot)
                    {
                        DrawNode_ptr tempTarget = m_gameObjectRenderBranchRoot->findNodeRecursive(objectInBranch);

                        if (tempTarget != nullptr)
                        {

                            bool visibleTemp = m_isVisible;

                            if (visibleTemp)
                            {
                                tempTarget->invisible();
                            }
                            LOG_DEBUG("tempTarget->setObject(objetToSwapIn);");
                            tempTarget->setObject(objetToSwapIn);

                            if (visibleTemp)
                            {
                                tempTarget->visible();
                            }
                        }
                    }
                }
            }           
        }
    }


            //        if(m_gameObjectRenderBranchRoot)
            //        {
            //            DrawNode_ptr tempTarget = m_gameObjectRenderBranchRoot-> FindNodeRecursive(objectInBranch);
            //
            //            if(tempTarget != nullptr)
            //            {
            //                tempTarget->Invisible();
            //                tempTarget->setObject(objetToSwapIn);
            //                tempTarget->Visible();
            //            }
            //        }
            //        else
            //        {
            //            bool isNodeFound= false; 
            //            for(size_t i = 0;i<m_gameObjectRenderElements.size() ; ++i )
            //            {
            //                if(m_gameObjectRenderElements[i] == objectInBranch)
            //                {
            //                    m_gameObjectRenderElements[i] = objetToSwapIn;
            //                    isNodeFound = true;
            //                    break;
            //                }
            //            }
            //        }

    void ObjectRenderNode::logBranch()
    {
        LOG_DEBUG("Visual elements of object(", this, "): ");
        m_gameObjectRenderBranchLeafs->logBranch();
    }

    void ObjectRenderNode::setVisible(bool Visible)
    {
        m_isVisible = Visible;
    }

    bool ObjectRenderNode::isVisible() const
    {
        return m_isVisible;
    }

    DrawNode_ptr ObjectRenderNode::getGameObjectRenderBranchLeafs()
    {
        return m_gameObjectRenderBranchLeafs;
    }

    ObjectRenderNode& operator<<(ObjectRenderNode& visual, const DrawObject_ptr & newNode)
    {
        visual.m_gameObjectRenderElements.push_back(newNode);
        return visual;
    }

    ObjectRenderNode_ptr& operator<<(ObjectRenderNode_ptr& visual, const DrawObject_ptr & newNode)
    {
        visual->m_gameObjectRenderElements.push_back(newNode);
        return visual;
    }

    ObjectRenderNode_ptr createObjectRenderNode_ptr()
    {
        return aex::make_shared<ObjectRenderNode>();
    }

    ObjectRenderNode_ptr createVisual_ptr()
    {
        return aex::make_shared<ObjectRenderNode>();
    }

}
