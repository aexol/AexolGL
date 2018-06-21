/*
 * File:   ObjectRenderNodeRaw.h
 * Author: mritke
 *
 * Created on November 4, 2013, 11:14 AM
 */

#ifndef OBJECTRENDERNODE_H
#define	OBJECTRENDERNODE_H
#include <aex/GameObjectNode.hpp>
#include <aex/RenderingTreeBlock.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/Common.hpp>
#include <vector>

#include "EventDispatcher.hpp"

namespace aex
{

    class ObjectRenderNodeRaw;

    class AEX_PUBLIC_API IWantRenderRoot : public aex::GameEvent
    {
    private:
        ObjectRenderNodeRaw* m_visual; 
    public:

        IWantRenderRoot(ObjectRenderNodeRaw* visual) : m_visual(visual)
        {

        }

        ObjectRenderNodeRaw* GetVisual() const
        {
            return m_visual;
        }

    };

    class AEX_PUBLIC_API ObjectRenderNodeRaw : public GameObjectNode
    {
    private:

        std::vector<DrawObject*> m_gameObjectRenderElements;
        RenderingTreeNode* m_gameObjectRenderBranchRoot;
        RenderingTreeNode* m_gameObjectRenderBranchLeaf;
        RenderingTreeNode* m_rendererRootPtr;
        std::vector<std::string> n_elementsNames;
        bool m_updated;
        bool m_branchInTree;
        bool m_isAnimated;
        bool m_isVisible;

        aexTimeUnit m_objectRenderTime;
        void setVisible(bool Visible);
    public:
         /**
         *	@brief	Default ctor
         */

        ObjectRenderNodeRaw();
        ObjectRenderNodeRaw(GameObject* parent);
        ~ObjectRenderNodeRaw();




        /**
         *	@brief	Set root of render tree or node where you want to start draw object from
         *
         *	@param 	root pointer to render tree root(RenderingTreeNode)
         */
        void setRootRenderNode(RenderingTreeNode* root)
        {
            m_rendererRootPtr = root;
        }

        /**
         *	@brief	Enable "isVisible" option if elements of object are in tree, or adds elements of object to render tree
         */
        void startDrawing();

        void startDrawing(RenderingTreeNode* rendererRootPtr);

        /**
         *	@brief	Sets the leaf of render tree as invisible
         */
        void stopDrawing();

        /**
         *	@brief	Adds new Draw Object to this node(shader, mesh, material etc.)
         *
         *	@param 	newNode 	pointer to DrawObject
         */
        void addRenderElement(DrawObject* newNode);

       

        void addElementToRenderTree(DrawObject* object);

        /**
         *	@brief	Adds DrawObjects to render tree in given place
         *
         *	@param 	root 	root of render tree
         */
        void addElementsToRenderTree(RenderingTreeNode* root, bool front = false);


        /**
         *	@brief	Adds DrawObjects to render tree
         */
        void addElementsToRenderTree(bool front = false);

       

        /**
         *	@brief	Removes local branch of RenderingTreeNodes from render tree TODO
         */
        void removeBranch();

        /**
         *	@brief	Find render tree root TODO
         */
        void getRenderRoot(EventHandler* eventHandler);
        
        void clear();
        
        int getBranchSize();
        void changeRenderObject(RenderingTreeNode* node, DrawObject* object);
        void setRendererRootPtr(RenderingTreeNode* rendererRootPtr);
        RenderingTreeNode* getRendererRootPtr() const;
        void setBranchInTree(bool branchInTree);
        bool isBranchInTree() const;
        void swapElementsInBranch(DrawObject* objectInBranch, DrawObject* objetToSwapIn);
        void logBranch();
        bool isVisible() const;

        RenderingTreeNode* getGameObjectRenderBranchLeafs() ;
        friend ObjectRenderNodeRaw& operator<<(ObjectRenderNodeRaw& visual,  DrawObject* newNode);

        // void

    };
        ObjectRenderNodeRaw& operator<<(ObjectRenderNodeRaw& visual, DrawObject* newNode);
}


#endif	/* OBJECTRENDERNODE_H */

