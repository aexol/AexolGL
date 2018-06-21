//
//  TreeNode.h
//  AexolGL
//
//  Created by Kamil Matysiewicz on 30.10.2013.
//
//

#ifndef AexolGL_TreeNode_h
#define AexolGL_TreeNode_h

#include <aex/Common.hpp>
#include <vector>

namespace aex
{
    template <typename T>
    class TreeNode : public enable_shared_from_this<TreeNode<T>>
    {
        typedef shared_ptr<TreeNode<T>> ThisNodePtr;
        typedef weak_ptr<TreeNode<T>> ParentPtr;
    public:
        typedef T this_type;
        typedef shared_ptr<this_type>   this_type_ptr;
        typedef std::vector<ThisNodePtr> children_type;
        static ThisNodePtr makeTreeNodePtr(const this_type_ptr& object, ThisNodePtr parent = nullptr);
        void addChild(ThisNodePtr newChild);
        void remove();
        void removeChild(this_type_ptr child);
        bool operator==(const ThisNodePtr& node_ptr);
        bool operator==(const this_type_ptr& node_ptr);
        this_type_ptr get()
        {
            return m_object;
        }
        children_type& getChildren()
        {
            return m_children;
        }
        virtual ~TreeNode() {}
    protected:
        this_type_ptr m_object;
        ParentPtr m_parent;
        children_type m_children;
        TreeNode(const this_type_ptr& object);
        ThisNodePtr getPtr()
        {
            return enable_shared_from_this<TreeNode<T>>::shared_from_this();
        }
    };
    template <typename T> using TreeNodePtr = shared_ptr<TreeNode<T>>;
    template <typename T> TreeNodePtr<T>
    TreeNode<T>::makeTreeNodePtr(const this_type_ptr& object, TreeNodePtr<T> parent)
    {
        ThisNodePtr ret(new TreeNode<T>(object));

        if(parent)
        {
            ret->m_parent = parent;

            if(auto sp = ret->m_parent.lock())
            {
                sp->addChild(ret->getPtr());
            }
        }

        return ret;
    }

    template <typename T>
    TreeNode<T>::TreeNode(const this_type_ptr& object) :
        m_object(object),
        m_parent()
    {
    }

    template <typename T> void
    TreeNode<T>::addChild(ThisNodePtr newChild)
    {
        m_children.push_back(newChild);
    }

    template<typename T> void
    TreeNode<T>::remove()
    {
        if(auto sp = m_parent.lock())
        {
            sp->removeChild(m_object);
        }
    }

    template <typename T> void
    TreeNode<T>::removeChild(this_type_ptr child)
    {
        for(auto iter = m_children.begin(); iter != m_children.end(); iter++)
        {
            if(child == (*iter))
            {
                m_children.erase(iter);
                iter--;
            }
        }
    }

    template <typename T> bool
    TreeNode<T>::operator==(const TreeNodePtr<T>& node_ptr)
    {
        if(m_object == node_ptr.m_object)
        {
            return true;
        }

        return false;
    }


    template <typename T> bool
    TreeNode<T>::operator==(const this_type_ptr& node_ptr)
    {
        if(m_object == node_ptr)
        {
            return true;
        }

        return false;
    }

}


#endif
