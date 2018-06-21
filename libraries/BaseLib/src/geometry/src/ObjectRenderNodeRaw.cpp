/*
 * File:   ObjectRenderNodeRaw.h
 * Author: mritke
 *
 * Created on November 4, 2013, 11:14 AM
 */



#include <aex/ObjectRenderNodeRaw.hpp>
#include <aex/Logger.hpp>



namespace aex
{

    ObjectRenderNodeRaw::ObjectRenderNodeRaw(GameObject* parent) :
        GameObjectNode(parent),
        m_gameObjectRenderElements(),
        m_rendererRootPtr(),
        n_elementsNames(),
        m_updated(false),
        m_branchInTree(false),
        m_isVisible(false)
    {


    }

    ObjectRenderNodeRaw::ObjectRenderNodeRaw() :
        GameObjectNode(nullptr),
        m_gameObjectRenderElements(),
        m_rendererRootPtr(),
        n_elementsNames(),
        m_updated(false),
        m_branchInTree(false),
        m_isVisible(false)
    {

    }

    ObjectRenderNodeRaw::~ObjectRenderNodeRaw()
    {
        if(m_branchInTree)
        {
            removeBranch();
        }
    }


    void ObjectRenderNodeRaw::setRendererRootPtr(RenderingTreeNode* rendererRootPtr)
    {
        m_rendererRootPtr = rendererRootPtr;
    }

    RenderingTreeNode* ObjectRenderNodeRaw::getRendererRootPtr() const
    {
        return m_rendererRootPtr;
    }

    void ObjectRenderNodeRaw::setBranchInTree(bool branchInTree)
    {
        m_branchInTree = branchInTree;
    }

    bool ObjectRenderNodeRaw::isBranchInTree() const
    {
        return m_branchInTree;
    }

    void ObjectRenderNodeRaw::startDrawing()
    {
        if(!m_isVisible)
        {
            if(m_branchInTree)
            {
                m_gameObjectRenderBranchLeaf->show();

            }
            else
            {
                addElementsToRenderTree();
                m_gameObjectRenderBranchLeaf->show();

            }

            m_isVisible = true;
        }

    }

    void ObjectRenderNodeRaw::startDrawing(RenderingTreeNode* rendererRootPtr)
    {
        setRootRenderNode(rendererRootPtr);
        startDrawing();
    }

    void ObjectRenderNodeRaw::stopDrawing()
    {

        if(m_branchInTree && m_isVisible)
        {
            m_gameObjectRenderBranchLeaf->hide();

        }
        else
        {

        }

        m_isVisible = false;
    }

    void ChangeRenderObject(RenderingTreeNode* node, DrawObject* object)
    {
        node->setObject(object);
    }

    void ObjectRenderNodeRaw::addRenderElement(DrawObject* newNode)
    {
        m_gameObjectRenderElements.push_back(newNode);
    }

    void ObjectRenderNodeRaw::addElementToRenderTree(DrawObject* object)
    {
        // m_rendererRootPtr->addChild(object);
    }

    void ObjectRenderNodeRaw::addElementsToRenderTree(RenderingTreeNode* root, bool front)
    {
        m_rendererRootPtr = root;
        addElementsToRenderTree();
    }

    void ObjectRenderNodeRaw::addElementsToRenderTree(bool front)
    {
        if(m_rendererRootPtr != nullptr)
        {

            RenderingTreeNode* tempTarget = m_rendererRootPtr;
            bool first = true;

            if(m_gameObjectRenderElements.size() > 0)
            {
                for(auto element : m_gameObjectRenderElements)
                {
                    if(tempTarget->findFirst(element) == nullptr && first)
                    {
                        tempTarget->addChild(element);
#ifdef DEBUG
                        //LOG_DEBUG("Element not found in tree: ADD in");
#endif
                        tempTarget = tempTarget->findFirst(element);
                    }
                    else if(tempTarget->hasChild(element) == nullptr)
                    {
                        tempTarget->addChild(element);
#ifdef DEBUG
                        // LOG_DEBUG("Element not found in tree: ADD in");
#endif
                        tempTarget = tempTarget->hasChild(element);
                    }
                    else if(first)
                    {
                        tempTarget = tempTarget->findFirst(element);
#ifdef DEBUG
                        // LOG_DEBUG("Element found in tree:Check Next element");
#endif
                    }
                    else
                    {
                        tempTarget = tempTarget->hasChild(element);
#ifdef DEBUG
                        // LOG_DEBUG("Element found in tree:Check Next element.");
#endif
                    }

                    if(first)
                    {
                        m_gameObjectRenderBranchRoot = tempTarget;
                        first = false;
                    }
                }

                m_gameObjectRenderBranchLeaf = tempTarget;
                m_branchInTree = true;
            }

            /*if(tempTarget!=m_rendererRootPtr)
            {}*/

        }
        else
        {
            LOG_ERR("Render Root Not Set!!!");
        }
    }



    void ObjectRenderNodeRaw::getRenderRoot(EventHandler* eventHandler)
    {
        IWantRenderRoot event(this);
        eventHandler->handleEvent(&event);
    }

    int ObjectRenderNodeRaw::getBranchSize()
    {
        return m_gameObjectRenderElements.size();
    }

    
    void ObjectRenderNodeRaw::clear()
    { 
        removeBranch();
        m_gameObjectRenderBranchRoot=nullptr;
        m_gameObjectRenderElements.clear();
        m_gameObjectRenderBranchLeaf=nullptr;
        m_rendererRootPtr=nullptr;
    }

    void ObjectRenderNodeRaw::removeBranch()
    {
        stopDrawing();

        if(m_branchInTree)
        {

            m_gameObjectRenderBranchLeaf->hide();
            m_gameObjectRenderBranchLeaf->remove();

            //m_gameObjectRenderBranchLeaf->remove();
        }

        m_branchInTree = false;
        m_isVisible = false;
    }

    void ObjectRenderNodeRaw::swapElementsInBranch(DrawObject* objectInBranch, DrawObject* objetToSwapIn)
    {
        RenderingTreeNode* tempTarget = m_gameObjectRenderBranchRoot-> findFirst(objectInBranch);

        if(tempTarget != nullptr)
        {
            tempTarget->hide();
            tempTarget->setObject(objetToSwapIn);
            tempTarget->show();
        }
    }

    void ObjectRenderNodeRaw::logBranch()
    {
        LOG_DEBUG("Visual elements of object(", this, "): ");
        m_gameObjectRenderBranchLeaf->logBranch();
    }

    void ObjectRenderNodeRaw::setVisible(bool Visible)
    {
        m_isVisible = Visible;
    }

    bool ObjectRenderNodeRaw::isVisible() const
    {
        return m_isVisible;
    }

    RenderingTreeNode* ObjectRenderNodeRaw::getGameObjectRenderBranchLeafs()
    {
        return m_gameObjectRenderBranchLeaf;
    }

    ObjectRenderNodeRaw& operator<<(ObjectRenderNodeRaw& visual, DrawObject* newNode)
    {
        visual.m_gameObjectRenderElements.push_back(newNode);
        return visual;
    }


}
