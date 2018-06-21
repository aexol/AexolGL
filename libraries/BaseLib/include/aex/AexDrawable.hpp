//////
//  AexDrawable.h
//  AexolGL
//
//  Created by Kamil Matysiewicz on 22.10.2013.
//
//

#ifndef AexolGL_AexDrawable_h
#define AexolGL_AexDrawable_h

#include <aex/AexRenderer.hpp>
#include <aex/AABB.hpp>
#include <aex/Common.hpp>
#include <aex/GPUApi.hpp>
#include <boost/signals2.hpp>
#include <list>

namespace aex
{
    class RenderingTreeNode;
    /**
     *	@brief	Interface for DrawObjects
     */
    class AEX_PUBLIC_API DrawVisibility
    {

    public:
        DrawVisibility() :
            m_isVisible(false),
            m_name(),
            m_type(-1),
            m_needsUpdate(false),
            m_bounds(AABB::Infinite())
        {}
        DrawVisibility(const DrawVisibility& dv) :
            m_isVisible(dv.m_isVisible),
            m_name(dv.m_name),
            m_type(dv.m_type),
            m_needsUpdate(dv.m_needsUpdate),
            m_bounds(dv.m_bounds)
        {}
        /**
         *	@brief	Set visibility of object
         *
         *	@param 	visibility 	bool
         */
        inline void setIsVisible(bool visibility)
        {
            m_isVisible = visibility;
        }

        /**
         *	@brief	Get visivility of object
         *
         *	@return	bool
         */
        inline bool getIsVisible() const
        {
            return m_isVisible;
        }

        /**
         *	@brief	Set name of object
         *
         *	@param 	name 	aex::string
         */
        inline void setName(const aex::string& name)
        {
            m_name = name;
        }

        /**
         *	@brief	Get object name
         *
         *	@return	aex::string
         */
        inline const aex::string& getName() const
        {
            return m_name;
        }


        /**
         *	@brief	Set type of object
         *
         *	@param 	type 	int
         */
        inline void setType(int type)
        {
            m_type = type;
        }
        inline int getType() const
        {
            return m_type;
        }
        
    private:
        bool            m_isVisible;
    protected:
        aex::string     m_name;
        int             m_type;
        bool            m_needsUpdate;
        AABB            m_bounds;
        
    };

    /**
     *	@brief	Interface for DrawObjects. Needs to implement: void draw( AexRenderer* gl ), void finishdraw( ), needsUpdate().
     */
    class AEX_PUBLIC_API DrawObject : public IOnContextDirty
    {
    protected:
        virtual void onContextDirty(){}
        friend class RenderingTreeNode;
        
        void removeTrackedNode(RenderingTreeNode* node);
        std::list<RenderingTreeNode*> m_trackedNodes;
    public:
        /**
         *	@brief	draw function called by DrawNode.
         *
         *	@param 	gl Drawing context.
         */
        virtual void draw(AexRenderer* gl) = 0;

        /**
         *	@brief	finishdraw function called by DrawNode.
         *
         * @param gl Drawing context.
         */
        virtual void finishdraw(AexRenderer* gl) = 0;

        virtual void needsUpdate() = 0;
        
        void addTrackedNode(RenderingTreeNode* node);
        
        virtual ~DrawObject();
    };
    typedef shared_ptr<DrawObject> DrawObject_ptr;
}

#endif
