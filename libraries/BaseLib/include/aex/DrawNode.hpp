//
//  DrawNode.h
//  AexolGL
//
//  Created by Kamil Matysiewicz on 24.10.2013.
//
//

#ifndef AexolGL_DrawNode_h
#define AexolGL_DrawNode_h
#include <aex/AexDrawable.hpp>
#include <vector> 
#include <boost/pool/pool_alloc.hpp>
#include <mutex>
namespace aex
{

    class AEX_PUBLIC_API DrawNode : public DrawVisibility, public enable_shared_from_this<DrawNode>
    {
        typedef aex::shared_ptr<DrawNode> DrawNode_ptr;
        typedef std::vector<DrawNode_ptr,boost::fast_pool_allocator<DrawNode_ptr>> ChildrenVec;

    public:

        /**
         *	@brief	Default ctor
         *
         */
        DrawNode();

        /**
         *	@brief	Ctor of DrawNode with DrawObject ready to put in draw tree
         *
         *	@param 	object 	shared pointer to DrawObject
         */
        explicit DrawNode(DrawObject_ptr object);

        /**
         *	@brief	Ctor of DrawNode with pointer to parent DrawNode, can be used later
         *	@param 	parent 	shared pointer to DrawNode
         */
        explicit DrawNode(DrawNode* parent);

        /**
         *	@brief	Ctor of DrawNode with pointer to parent DrawNode and DrawObject. Ready to put in render tree
         *
         *	@param 	parent 	shared pointer to parent DrawNode
         *	@param 	object 	shared pointer to DrawObject
         */
        DrawNode(DrawNode* parent, DrawObject_ptr object);

        /**
         *	@brief	Main function of DrawNode. Calls draw() function of DrawObject(if exists) and calls all children DrawNodes.
         *
         *	@param 	gl 	<#gl description#>
         */
        void draw(AexRenderer* gl);

        void finishdraw();
        void update();
        /**
         *	@brief	Add shared pointer to DrawNode to child list
         *
         *	@param 	newChild 	shared pointer to  DrawNode
         */
        void addChild(DrawNode_ptr newChild);
        void addChildFront(DrawNode_ptr newChild);

        /**
         *	@brief	Make new DrawNode wrapped around Drawbject and put it into child list;
         *
         *	@param 	newChild 	shared pointer to DrawObject;
         */
        void addChildFromObject(DrawObject_ptr newChild);

        void needsUpdate();
        /**
         *	@brief	Set DrawObject for this DrawNode
         *
         *	@param 	object 	shared pointer to DrawObject
         */
        void setObject(DrawObject_ptr object);

        /**
         *	@brief	Set parent DrawNode for this ParentNode
         *
         *	@param 	parent 	shared pointer to DrawNode
         */
        void setParent(DrawNode* parent);

        /**
         *	@brief	Remove this node form render tree
         */
        void remove();
        /**
         *	@brief	If pointed children is in child list, then remove it from list.
         *
         *	@param 	child 	shared pointer to DrawNode
         */
        void removeChild(DrawNode_ptr child);

        void removeAll();
        /**
         *	@brief	Set this nade as invisible and sends this info to parent (if exists)
         */
        void invisible();

        /**
         *	@brief	Set this node as visible, if got parent node then set it to visible too
         */
        void visible();

        /**
         *	@brief	If node gets info about invisible childs. If any other child is visible, then this node sets itself to invisible too.
         */
        void childInvisible();

        int childDepth(int dep);

        int nodeCount();
        /**
         *	@brief	Finds DrawNode with pointed DrawObject
         *
         *	@param 	object shared pointer to DrawObject
         *
         *	@return	shared pointer to DrawNode
         */
        DrawNode_ptr findNodeRecursive(DrawObject_ptr object);
        DrawNode_ptr findNodeInChilds(DrawObject_ptr object);
        DrawNode_ptr thisObject(DrawObject_ptr object);

        void logBranch();
        void logTree(int i = 0 );


        ~DrawNode();
    protected:
        void draw_self(AexRenderer* gl, DrawObject* const object);
        virtual void draw_children(AexRenderer* gl);
        DrawObject_ptr m_object;

        DrawNode* m_parent;
        ChildrenVec m_children;
        bool m_isRemovable;
        bool m_isInTree;
        std::mutex Active;
        std::mutex m_StartStopMutex;
    };
    typedef aex::shared_ptr<DrawNode> DrawNode_ptr;
}


#endif
