#ifndef __RENDERINGTREEBLOCK_HPP
#define __RENDERINGTREEBLOCK_HPP
#include <aex/RenderingBlock.hpp>
#include <aex/AexDrawable.hpp>
#include <memory>
#include <list>

namespace aex
{
    /**
     * @brief Simple tree like structure ordering objects for drawing.
     */
    class AEX_PUBLIC_API RenderingTreeNode 
    {
            typedef std::unique_ptr<RenderingTreeNode> Child;
            typedef std::vector<Child> ChildrenVec;
        public:
            typedef DrawObject* self_ptr;

            /**
             *	@brief	Default ctor
             *
             */
            RenderingTreeNode();

            /**
             *  @brief Copy node without children
             */
            RenderingTreeNode(const RenderingTreeNode& node) = delete;
            RenderingTreeNode& operator=(const RenderingTreeNode& node) = delete;

            RenderingTreeNode( RenderingTreeNode&& node);
            RenderingTreeNode& operator=( RenderingTreeNode&& node);

            /**
             *	@brief Add object to be rendered.
             *
             *	@param 	object New object.
             */
            RenderingTreeNode(self_ptr object);

            /**
             *	@brief	Main function of RenderingTreeNode. Calls draw() function of DrawObject(if exists) and calls all children RenderingTreeNodes.
             *
             *	@param 	gl 	<#gl description#>
             */
            virtual void draw(AexRenderer* gl);

            /**
             * @brief Clean up after drawing.
             *
             * @param AexRenderer* Drawing context.
             */

            virtual void finishdraw(AexRenderer* gl);
            /**
             *	@brief	Add new object to be drawn as child.
             *
             *	@param 	newChild Object to be drawn.
             *	@returns Pointer to new node.
             */
            virtual RenderingTreeNode* addChild(self_ptr newChild);

            /**
             * @brief Set backing object for drawing.
             *
             * @param self_ptr New object to be drawn.
             */
            virtual void setObject(self_ptr object);

            /**
             *	@brief Remove first occurence of object.
             *
             *	Recursively breadth-first search for object in tree and remove it's first occurence.
             *
             *	@param object Object address.
             *
             *	@returns True if object removed from the tree.
             */
            bool remove(self_ptr object);
            
            void remove();

            void removeChild(RenderingTreeNode* child);
            /**
             * @brief Remove all descendant nodes from the tree.
             */
            void clear();
            /**
             *	@brief	Set this nade as invisible.
             */
            void hide();

            /**
             *	@brief	Set this node as visible.
             */
            void show();

            /**
             *	@brief	Finds first occurence of node managing object.
             *
             *	@param 	object  being searched.
             *
             *	@return	Pointer to node in tree or nullptr if not found.
             */
            RenderingTreeNode* findFirst(self_ptr object);

            /**
             *	@brief	Check if object is a child of this node.
             *
             *	@param 	object  being searched.
             *
             *	@return	Pointer to node in tree or nullptr if not found.
             */
            RenderingTreeNode* hasChild(self_ptr object);
            
             /**
             * @brief Draw children if visible.
             */
            virtual void drawChildren(AexRenderer* gl);

            /**
             * @brief Dtor
             */
            ~RenderingTreeNode();
            
            void logBranch();
        protected:
            /**
             * @brief Find node in children vector.
             *
             * @returns Iterator pointing to descendant.
             */
            bool _hasChild(self_ptr object, ChildrenVec::iterator& it);
            /*
             * @brief Called by descendant when it becomes invisible.
             */
            void childBecameInvisible();

            /*
             * @brief Called by descendant when it becomes invisible.
             */
            void childBecameVisible();

            /**
             * @brief Ctor
             *
             * Create descendant node.
             */ 
            RenderingTreeNode(self_ptr object, RenderingTreeNode* parent);

            /**
             * @brief Draw self if visible and atleast one of the children is visible.
             */
            virtual void drawSelf(AexRenderer* gl);

            /**
             * @brief Finish draw self if visible and atleast one of the children is visible.
             */
            virtual void finishdrawSelf(AexRenderer* gl);

            /**
             * @brief Release backing object.
             */
            self_ptr releaseObject();

            /**
             * @brief Check if object should be drawn.
             */
            bool isVisible();

            RenderingTreeNode* m_parent = nullptr;
            self_ptr m_object = nullptr;
            ChildrenVec m_children;
            uint32_t m_visibleChildren;
            bool m_visible;
    };

    class AEX_PUBLIC_API RenderingTreeBlockImpl
    {
        public:
            typedef RunOnlyBlock<RenderingTreeBlockImpl> TraitsType;
            typedef RenderingBlock<RenderingTreeBlockImpl, TraitsType> RenderingTreeBlock;
            /**
             * @brief Default constructor.
             */
            RenderingTreeBlockImpl() = default;

            /**
             * @brief Constructor.
             *
             * @param Drawing context.
             */
            RenderingTreeBlockImpl(AexRenderer* gl);

            /**
             * @brief Set new context.
             *
             * @param AexRenderer* Drawing context.
             */
            void setContext(AexRenderer* gl);

            /**
             * @brief Get tree root.
             *
             * @param Tree root.
             */
            RenderingTreeNode& root();
        protected:
            friend RenderingTreeBlock;
            friend TraitsType;

            /**
             * @brief Init tree.
             *
             * @returns True if has renderer.
             */
            bool init();

            /**
             * @brief Start drawing tree.
             */
            void beginFrame();

            /**
             * @breif Finish drawing tree.
             */
            void endFrame();
            RenderingTreeNode m_root;
            AexRenderer* m_gl;
    };
    typedef RenderingTreeBlockImpl::RenderingTreeBlock RenderingTreeBlock;
}

#endif // __RENDERINGTREEBLOCK_HPP
