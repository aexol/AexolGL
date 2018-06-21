/** @file TransformGameNode.hpp
 *  @brief Definition of TransformGameNode.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef AexolGL_TransformGameNode_h
#define AexolGL_TransformGameNode_h

#include <aex/GameEvent.hpp>
#include <aex/GameObjectNode.hpp>
#include <aex/Quaternion.hpp>
#include <aex/Vector3.hpp>
#include <aex/TreeNode.hpp>

namespace aex
{
    /**
     *	@brief	Default event on transform.
     */
    class AEX_PUBLIC_API TRANSFORM_EVENT : public GameEvent
    {
    public:
        /**
         *	@brief	Ctor
         *
         *	@param translate Translation vector.
         *  @param orientation Orientation quaternion.
         *  @param scale    Scale vector.
         */
        TRANSFORM_EVENT(aex::math::Vector3 translate, aex::math::Quaternion orientation = aex::math::Quaternion(), aex::math::Vector3 scale = aex::math::Vector3(1.0f, 1.0f, 1.0f));

        /**
         *	@brief	Get translation vector.
         *
         *	@return	Translation.
         */
        aex::math::Vector3 getTranslate() const
        {
            return m_translate;
        }

        /**
         *	@brief	Get scale vector.
         *
         *	@return	Scale.
         */
        aex::math::Vector3 getScale()  const
        {
            return m_scale;
        }

        /**
         *	@brief	Get orientation quaternion.
         *
         *	@return	Orientation.
         */
        aex::math::Quaternion getOrientation()  const
        {
            return m_orientation_offset;
        }

        virtual ~TRANSFORM_EVENT() {}
    private:
        aex::math::Vector3 m_translate;
        aex::math::Vector3 m_scale;
        aex::math::Quaternion m_orientation_offset;
    };

    /**
     *	@brief	Game object node containing world space data.
     */
    class AEX_PUBLIC_API TransformGameNode : public GameObjectNode, public enable_shared_from_this<TransformGameNode>
    {
        typedef shared_ptr<TransformGameNode> TransformGameNodePtr;
        typedef TreeNodePtr<TransformGameNode> TransformTreePtr;
    public:
        static TransformGameNodePtr makeTransformGameNode(GameObject* parent,  TransformGameNodePtr tree_parent = nullptr);
        bool updated()
        {
            return m_updated;
        }
        void setTranslation(const aex::math::Vector3& translate);
        void move(const aex::math::Vector3& translate);
        void setOrientation(const aex::math::Quaternion& quat);
        void setOrientation(const aex::math::Vector3& vec);
        void rotate(const aex::math::Quaternion& quat);
        void rotate(const aex::math::Vector3& vec);
        void setScale(const aex::math::Vector3& scale);
        void scale(const aex::math::Vector3& scale);
        aex::math::Quaternion getOrientation() const;
        aex::math::Vector3 getSize() const;
        aex::math::Vector3 getPosition() const;
        bool isUpdated() const;
    protected:
        TransformGameNode(GameObject* parent);
        aex::math::Matrix m_transformationMatrix;
        bool    m_updated;
        aex::math::Vector3 m_position;
        aex::math::Vector3 m_size;
        aex::math::Quaternion m_orientation;
        TransformTreePtr m_thisTreeNode;
        TransformGameNodePtr getPtr()
        {
            return shared_from_this();
        };
    };

    typedef shared_ptr<TransformGameNode> TransformGameNodePtr;
    typedef TreeNode<TransformGameNode> TransformTreeNode;
    typedef TreeNodePtr<TransformGameNode> TransformTreeNodePtr;
}

#endif
