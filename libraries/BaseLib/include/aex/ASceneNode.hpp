#ifndef __AEX_ASCENENODE_HPP
#define __AEX_ASCENENODE_HPP
#include <aex/Vector3.hpp>
#include <aex/AABB.hpp>
#include <aex/Quaternion.hpp>
#include <aex/Matrix.hpp>
#include <vector>
namespace aex
{
    class AEX_PUBLIC_API ASceneNode
    {
        typedef aex::math::Matrix Matrix;
        typedef std::vector<ASceneNode*> ChildVec;
        public:
            /**
             * @brief Default ctor
             */
            ASceneNode();
            /**
             * @brief Constructor
             *
             * Create new scene node which transformation will be relative to parentNode.
             *
             * @param ASCeneNode* Parent node.
             */
            ASceneNode(ASceneNode* parenNode);

            /**
             * @brief Copy constructor.
             *
             * Copy constructor that copies everything except for nodes children.
             *
             * @param ASceneNode& Original node.
             */
            ASceneNode(const ASceneNode&);
            /**
             * @brief Copy asignment operator.
             *
             * Copy everything except for children.
             *
             * @param ASceneNode& Original node.
             */
            ASceneNode& operator=(const ASceneNode&);

            /**
             * @brief Move constructor.
             *
             * @param ASceneNode& Original node.
             */
            ASceneNode(ASceneNode&&);
            /**
             * @brief Move asignment operator.
             *
             * @param ASceneNode& Original node.
             */
            ASceneNode& operator=(ASceneNode&&);

            /**
             *  @brief Set new parent for node.
             *
             *  @param ASceneNode& New parent.
             */
            void setParent(ASceneNode& parent);

            /**
             *  @brief Set new parent for node.
             *
             *  @param ASceneNode* New parent.
             */
            void setParent(ASceneNode* parent);
            
             /**
             *  @brief Unsets parent for node.
             *
             */
            void unsetParent();

            /**
             * @brief Add new child node.
             *
             * @param ASceneNode& New child.
             */
            void addChild(ASceneNode& child);

            /**
             * @brief Add new child node.
             *
             * @param ASceneNode* New child.
             */
            void addChild(ASceneNode* child);

            /**
             * @brief Remove child node.
             *
             * @param ASceneNode* Child to be removed.
             */
            void removeChild(ASceneNode* child);

            /**
             * @brief Transformation matrix relative to parent.
             *
             * @returns Matrix Relative transformation matrix.
             */
            Matrix getModelMatrix() const;

            /**
             * @brief Transformation matrix relative to parent.
             *
             * @returns Matrix Relative transformation matrix.
             */
            Matrix& getModelMatrix();

            /**
             * @brief Return transformation matrix multiplied by parent transformation matrix.
             *
             * @returns Matrix Final transformation matrix.
             */
            Matrix getFinalModelMatrix() const;

            /**
             * @brief Return transformation matrix multiplied by parent transformation matrix.
             *
             * @returns Matrix Final transformation matrix.
             */
            Matrix& getFinalModelMatrix();

            /**
             * @brief Return relative parent transformation matrix.
             *
             * This function returns relative parent transformation matrix.
             *
             * @returns Matrix Parent transformation matrix.
             */
            virtual const Matrix& getParentMatrix();

            /**
             * @brief Return relative parent transformation matrix.
             *
             * This function returns relative parent transformation matrix.
             *
             * @returns Matrix Parent transformation matrix.
             */
            virtual Matrix getParentMatrix() const;

            /**
             * @brief Return final parent transformation matrix.
             *
             * This function returns final parent transformation matrix.
             *
             * @returns Matrix Final parent transformation matrix.
             */
            virtual Matrix getFinalParentMatrix() const;

            /**
             * @brief Return final parent transformation matrix.
             *
             * This function returns final parent transformation matrix.
             *
             * @returns Matrix Final parent transformation matrix.
             */
            virtual const Matrix& getFinalParentMatrix();

            /**
             * @brief Destructor.
             */
            virtual ~ASceneNode();
            bool isUpdateAABB() const;
        protected:
            /*
             * @brief Set parent node.
             *
             * This differs from public setParent in such way that while parent will be added to child node, the other way around won't happend.
             */
            void _setParent(ASceneNode* parent);

            /*
             * @brief Inform this about change in transformation.
             *
             * Should be called each time objects relative transformation changes in some way.
             */
            void transformChanged(); 

            /*
             * @brief Inform this about change in final transformation.
             *
             * Should be called each time objects final transformation changes in some way (be it either relative change or parent change).
             */
            void updateFinal();

            /**
             *  @brief Return true if member model matrix is out of sync.
             *
             *  @returns bool True if out of sync.
             */
            bool transformNeedsUpdate() const;

            /**
             *  @brief Return true if member final model matrix is out of sync.
             *
             *  @returns bool True if out of synch.
             */
            bool finalNeedsUpdate() const;

            /**
             *  @brief Return true if member final model matrix is out of sync.
             *
             *  @returns bool True if out of synch.
             */
            bool finalNeedsUpdate();

            bool drawNeedsUpdate();
            void drawUpdated();
            bool aabbNeedsUpdate();
            void aabbUpdated();
            /**
             * @brief Pure virtual method defining how model matrix is computed.
             *
             * @returns Matrix Computed model matrix.
             */
            virtual Matrix computeModelMatrix() const = 0;
            
            
            bool hasParent() const;
            ASceneNode* getParent() const;
            
        private:
            void releaseParent();
            /*
             * Tree structure to keep track of transformations.
             */
            ASceneNode* m_parent;
            ChildVec m_children;

            Matrix m_ModelMatrix;
            Matrix m_finalModelMatrix;
            bool m_transformChange=true;
            bool m_updateDraw=true;
            bool m_updateAABB=true;
            bool m_updateFinal=true;
    };

    class AEX_PUBLIC_API TRSNode : public ASceneNode
    {
        typedef aex::math::Vector3 Vector3;
        typedef aex::math::Quaternion Quaternion;
        typedef aex::math::Matrix Matrix;
        public:
            /**
             * @brief Default ctor
             */
            TRSNode();
            /**
             * @brief Constructor
             *
             * Create new scene node which transformation will be relative to parentNode.
             *
             * @param ASCeneNode* Parent node.
             */
            TRSNode(ASceneNode* parenNode);
            /**
             * @brief Constructor
             *
             * Create new scene node.
             *
             * @param Vector3 Node relative position.
             * @param Quaternion Node relative orientation.
             * @param Vector3 Node relative scale.
             * @param ASCeneNode* Parent node.
             */
            TRSNode(Vector3 position, Quaternion rotation, Vector3 scale, ASceneNode* parenNode = nullptr);
            /**
             *	@brief	Scale node
             *
             *	Scale node. If r is set to true scale is relative to previous scale.
             *
             *	@param 	float 	x dim
             *	@param 	float 	y dim
             *	@param 	float 	z dim
             *	@param 	bool    relative
             */
            virtual void scale(float, float, float, bool = false);

            /**
             *	@brief	Scale node
             *
             *	Scale node. If r is set to true scale is relative to previous scale.
             *
             *	@param 	Vector3 Scale.
             *	@param 	bool    relative
             */
            virtual void scale(Vector3 scaleVec, bool r = false);

            /**
             *	@brief	Scale by f in all dimensions.
             *
             *	Scale by f in all dimenstions. If r is set to true scale is relative to previous scale.
             *
             *	@param 	f 	scale
             *	@param 	bool 	relative
             */
            virtual void scale(float f, bool = false);

            /**
             *	@brief	Move node
             *
             *	Move node. If r is set to true translation is relative to previous scale.
             *
             *	@param 	float 	x dim
             *	@param 	float 	y dim
             *	@param 	float 	z dim
             *	@param 	bool    relative
             */
            virtual void move(float x, float, float, bool = false);

            /**
             *	@brief	Move node
             *
             *	Move node. If r is set to true translation is relative to previous scale.
             *
             *	@param 	Vector3 Move vector.
             *	@param 	bool    relative
             */
            virtual void move(Vector3 moveVec, bool r = false);

            /**
             *	@brief	Rotate aex bu Euler angles
             *
             *	@param 	float 	x angle
             *	@param 	float 	y angle
             *	@param 	float 	z angle
             *	@param 	bool 	relative
             */
            virtual void rotate(const Quaternion& rot, bool r = false);
            
            virtual void rotate(float, float, float, bool = false);

            /**
             *	@brief	Rotate aex bu Euler angles
             *
             *	@param 	Vector3 Rotation angles.
             *	@param 	bool 	relative
             */
            virtual void rotate(const Vector3& rotVec, bool r = false);

            /**
             * @brief Set final model matrix relative to parent.
             *
             * @param Matrix transform matrix.
             */
            void setMatrix(Matrix mat);
            
            /**
             * @brief Set position relative to parent.
             *
             * @param Vector3 Relative position.
             */
            void setPosition(Vector3 position);

            /**
             *  @brief Return position relative to parent.
             *
             *  @returns Vector3 Relative position.
             */
            Vector3 getPosition() const;

            /**
             * @brief Set X axis of position relative to parent.
             *
             * @param Vector3 Relative position.
             */
            void setPositionX(float value);

            /**
             * @brief Set Y axis of position relative to parent.
             *
             * @param Vector3 Relative position.
             */
            void setPositionY(float value);

            /**
             * @brief Set Z axis of position relative to parent.
             *
             * @param Vector3 Relative position.
             */
            void setPositionZ(float value);

            /**
             * @brief Set relative orientation.
             *
             * @param Quaternion Relative orientation.
             */
            void setOrientation(Quaternion orientation);

            /**
             * @brief Get relative orientation.
             *
             * @returns Quaternion Relative orientation.
             */
            Quaternion getOrientation() const;

            /**
             * @brief Set relative size.
             *
             * @param Vector3 Relative size.
             */
            void setSize(Vector3 size);

            /**
             * @brief Get relative size.
             *
             * @returns Vector3 Relative size.
             */
            Vector3 getSize() const;

            
             /**
             * @brief Get pivot point of transformation size.
             *
             * @returns Vector3  p.*/
            Vector3 getPivot() const;
            Vector3 getCenterPoint() const;
            Quaternion getFinalRotation() const;
            
            void setAabb(AABB aabb);
            void addAabb(AABB aabb);
            AABB getOabb() ;
           
           
            
        protected:
            /**
             * @brief Pure virtual method defining how model matrix is computed.
             *
             * @returns Matrix Computed model matrix.
             */
            virtual Matrix computeModelMatrix() const;
            

        private:
            Vector3 m_size;
            Vector3 m_position;
            Quaternion m_orientation;
            AABB m_aabb;
            AABB m_oabb;
    };
} // end of namespace aex.

#endif
