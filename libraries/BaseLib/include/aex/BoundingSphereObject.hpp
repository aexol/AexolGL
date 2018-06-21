/** @file BoundingSphereObject.hpp
 *  @brief Definition of BoundingSphereObject.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__BoundingSphere__
#define __aexolGL__BoundingSphere__

#include <aex/Common.hpp>
#include <aex/Vector3.hpp>
#include <aex/Math.hpp>
#include <aex/BoundingSphere.hpp>

namespace aex
{
    class AEX_PUBLIC_API Aex;

    typedef aex::math::Vector3 Vector;
    typedef aex::Aex Aex;
    typedef aex::shared_ptr<Aex> Aex_ptr;

    /**
     *	@brief	Wireframe for BoundingSphere
     */
    class AEX_PUBLIC_API BoundingSphereObject
    {
    public:
        /**
         *	@brief	Ctor
         *
         *	@param 	draw 	Visibility
         */
        BoundingSphereObject(bool draw = false);
        //
        /**
         *	@brief	Copy ctor
         */
        BoundingSphereObject(const BoundingSphereObject&);
        /**
         *	@brief	Ctor
         *
         *	@param 	parent 	Parent object
         *	@param 	bSphereObject 	Sphere to copy.
         *	@param 	draw 	Visibility
         */
        BoundingSphereObject(Aex& parent, const BoundingSphereObject& bSphereObject, bool draw = false);
        //
        /**
         *	@brief	Assigment
         */
        BoundingSphereObject& operator=(const BoundingSphereObject& bSphere);




        /**
         *	@brief	Make bounding sphere from vertices.
         *
         *	@param 	vertices 	Array of vertices.
         */
        void welzlCompute(std::vector<float>& vertices);
        //Method used to compute smallest bounding sphere of given set of vertices

        //Transforms
        /**
         *	@brief	Scale
         *
         *	@param 	x 	Multiplier.
         */
        void scale(float x);

        /**
         *	@brief	Move sphere.
         *
         *	@param 	x 	X axis translation.
         *	@param 	y 	Y axis translation.
         *	@param 	z 	Z axis translation.
         */
        void move(float x, float y, float z);


        //Access
        /**
         *	@brief	Get sphere center.
         *
         *	@return	Center.
         */
        Vector      center() const
        {
            return m_bSphere.center();
        }
        /**
         *	@brief	Get sphere data.
         *
         *	@return	Sphere data.
         */
        const BoundingSphere*  bSphere() const
        {
            return &m_bSphere;
        }

        /**
         *	@brief	Get sphere radius.
         *
         *	@return	Radius.
         */
        float     radius() const
        {
            return m_bSphere.radius();
        }
        /**
         *	@brief	Set sphere center.
         *
         *	@param 	center 	New center,
         */
        void        set_center(const Vector& center)
        {
            m_bSphere.set_center(center);
        }
        /**
         *	@brief	Set sphere radius.
         *
         *	@param 	radius 	Radius.
         */
        void        set_radius(float radius)
        {
            m_bSphere.set_radius(radius);
        }
        /**
         *	@brief	Is wireframe drawn.
         */
        bool        isDraw()
        {
            return m_draw;
        }
        /**
         *	@brief	Set wireframe draw.
         */
        void        setDraw(bool draw)
        {
            m_draw = draw;
        }
        /**
         *	@brief	Wireframe data.
         */
        const   Aex_ptr&    getBoundingObjectWireFrameAex()
        {
            return m_sphereWireframe;
        }
        void        set_sphere_wireframe(Aex& parent);
        /**
         *	@brief	Is inifinitely small sphere.
         */
        bool        isNull() const
        {
            return (aex::math::Math::closeEnough(m_bSphere.radius(), 0.0f, m_Epsilon)) ? false : true;
        }
        
    private:
        BoundingSphere  m_bSphere;
        float           m_Epsilon = 0.001f;
        bool            m_draw;
        Aex_ptr         m_sphereWireframe;
        
    };
}

#endif /* defined(__aexolGL__BoundingSphere__) */
