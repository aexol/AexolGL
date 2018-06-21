/*
 * File:   ObjectRenderNode.h
 * Author: mritke
 *
 * Created on November 4, 2013, 11:14 AM
 */

#ifndef OBJECTRENDERNODE_H
#define	OBJECTRENDERNODE_H
#include <aex/GameObjectNode.hpp>
#include <aex/DrawNode.hpp>
#include <aex/Common.hpp>
#include <vector>

#include "EventDispatcher.hpp"

namespace aex
{

    typedef aex::shared_ptr<DrawNode> DrawNode_ptr;
    class ObjectRenderNode;
    typedef ObjectRenderNode Visual;
    typedef aex::shared_ptr<ObjectRenderNode> ObjectRenderNode_ptr;
    typedef ObjectRenderNode_ptr Visual_ptr;

    class AEX_PUBLIC_API iWantRenderRoot : public aex::GameEvent
    {
    private:
        ObjectRenderNode* m_visual; 
    public:

        iWantRenderRoot(ObjectRenderNode* visual) : m_visual(visual)
        {

        }

        ObjectRenderNode* getVisual() const
        {
            return m_visual;
        }

    };

    class AEX_PUBLIC_API ObjectRenderNode : public GameObjectNode, public enable_shared_from_this<ObjectRenderNode>
    {
    public:
        ObjectRenderNode();
        ObjectRenderNode(GameObject* parent);
        ~ObjectRenderNode();

        /**
         *	@brief	Default ctor
         */

        static ObjectRenderNode_ptr makeRenderNode();

        /**
         *	@brief	Factory method.
         *
         *	@param 	parent 	pointer to parent GameObject
         */
        static ObjectRenderNode_ptr makeRenderNode(GameObject* parent);

        void update(aexTimeUnit time);

        /**
         *	@brief	Set root of render tree or node where you want to start draw object from
         *
         *	@param 	root pointer to render tree root(DrawNode)
         */
        void setRootRenderNode(DrawNode_ptr root)
        {
            m_rendererRootPtr = root;
        }

        /**
         *	@brief	Enable "isVisible" option if elements of object are in tree, or adds elements of object to render tree
         */
        void startDrawing();

        void startDrawing(DrawNode_ptr rendererRootPtr);

        /**
         *	@brief	Sets the leaf of render tree as invisible
         */
        void stopDrawing();

        /**
         *	@brief	Adds new Draw Object to this node(shader, mesh, material etc.)
         *
         *	@param 	newNode 	pointer to DrawObject
         */
        void addRenderElement(DrawObject_ptr newNode);

        friend ObjectRenderNode& operator<<(ObjectRenderNode& visual, const DrawObject_ptr& newNode);
        friend ObjectRenderNode_ptr& operator<<(ObjectRenderNode_ptr& visual, const DrawObject_ptr& newNode);

        void addElementToRenderTree(DrawObject_ptr object);

        /**
         *	@brief	Adds DrawObjects to render tree in given place
         *
         *	@param 	root 	root of render tree
         */
        void addElementsToRenderTree(DrawNode_ptr root, bool front = false);


        /**
         *	@brief	Adds DrawObjects to render tree
         */
        void addElementsToRenderTree(bool front = false);


        void sortDrawElements();

        /**
         *	@brief	Get render object from manager by given name
         *
         *	@param 	name 	string name of object
         */
        void getRenderElement(aex::string name);

        void updateRenderTree();


        /**
         *	@brief	Makes local brach of DrawNodes with current DrawObjects TODO
         */
        void makeBranch();

        /**
         *	@brief	Adds local branch of DrawNodes to render tree. NOT OPTIMAL IN USE(REDUNDANT ELEMENTS)!!! TODO
         *
         *	@param 	targetNode 	render tree root
         */
        void addBranch(DrawObject_ptr object);

        void addToBranch(DrawObject_ptr object, DrawNode_ptr branchRoot);

        /**
         *	@brief	Removes local branch of DrawNodes from render tree TODO
         */
        void removeBranch();

        /**
         *	@brief	Find render tree root TODO
         */
        void getRenderRoot(EventHandler* eventHandler);
        
        void clear();
        
        int getBranchSize();
        void changeRenderObject(DrawNode_ptr node, DrawObject_ptr object);
        bool findNode();
        void setRendererRootPtr(DrawNode_ptr rendererRootPtr);
        DrawNode_ptr getRendererRootPtr() const;
        void setBranchInTree(bool branchInTree);
        bool isBranchInTree() const;
        void swapElementsInBranch(DrawObject_ptr objectInBranch, DrawObject_ptr objetToSwapIn, bool leaveInPlace =false);
        void logBranch();
        bool isVisible() const;

        DrawNode_ptr getGameObjectRenderBranchLeafs() ;

    private:

        std::vector<DrawObject_ptr> m_gameObjectRenderElements;
        DrawNode_ptr m_gameObjectRenderBranchRoot;
        DrawNode_ptr m_gameObjectRenderBranchLeafs;
        DrawNode_ptr m_rendererRootPtr;
        std::vector<aex::string> n_elementsNames;
        bool m_updated;
        bool m_branchInTree;
        bool m_isAnimated;
        bool m_isVisible;

        aexTimeUnit m_objectRenderTime;
        void setVisible(bool Visible);

    };

    ObjectRenderNode_ptr createObjectRenderNode_ptr();
    ObjectRenderNode_ptr createVisual_ptr();

}


#endif	/* OBJECTRENDERNODE_H */

