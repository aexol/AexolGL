/** @file Plane.hpp
 *  @brief Definition of Plane.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__Plane__
#define __aexolGL__Plane__

#include <aex/Vector3.hpp>

namespace aex
{
    typedef aex::math::Vector3 Vector;
    enum class PLANES : unsigned char
    {
        NONE = 0, LEFT = 1, RIGHT = 2, TOP = 3, BOTTOM = 4, NEAR = 5, FAR = 6
    };
    /**
     *	@brief	3D plane representation.
     */
    class AEX_PUBLIC_API Plane3D
    {
    protected:
        Vector  m_normal; /* Plane normal */
        Vector  m_absNormal; /* Abs plane normal, just to save on calc */
        float   m_d; /* Distance from origin */
    public:
        Plane3D() : m_normal(), m_d(0.0f) {}
        float   get_d() const
        {
            return m_d;
        }
        void    set_d(float d)
        {
            m_d = d;
        }
        const Vector&  normal() const
        {
            return m_normal;
        }
        const Vector&  absNormal() const
        {
            return m_absNormal;
        }
        Vector& normal()
        {
            return m_normal;
        }
        Vector& absNormal()
        {
            return m_absNormal;
        }
        void    set_normal(const Vector& normal)
        {
            m_normal = normal;
        }

        friend class Plane;
    };
    /**
     *	@brief	Frustum plane.
     */
    class AEX_PUBLIC_API Plane

    {
    protected:
        Plane3D m_plane;
        PLANES  m_PLANE; /* If plane represents*/
    public:
        /**
         *	@brief	Ctor
         */
        Plane() : m_plane(), m_PLANE(PLANES::NONE) {}
        /**
         *	@brief	Sets plane from matrix coefficients.
         *
         *	@param 	a 	float
         *	@param 	b 	float
         *	@param 	c 	float
         *	@param 	d 	float
         */
        void            set_coefficients(float a, float b, float c, float d);

        /**
         *	@brief	Return distance to origin
         *
         *	@return	Distance
         */
        float           get_d() const
        {
            return m_plane.get_d();
        }
        /**
         *	@brief	Set distance to origin.
         *
         *	@param 	d 	distance
         */
        void            set_d(float d)
        {
            m_plane.set_d(d);
        }
        /**
         *	@brief	Get plane normal.
         *
         *	@return	Normal.
         */
        const Vector&   normal() const
        {
            return m_plane.normal();
        }
        /**
         *	@brief	Get abs normal.
         *
         *	@return	Abs normal.
         */
        const Vector&   absNormal() const
        {
            return m_plane.absNormal();
        }

        /**
         *	@brief	Get normal.
         *
         *	@return	Normal.
         */
        Vector&         normal()
        {
            return m_plane.normal();
        }

        /**
         *	@brief	Get abs normal.
         *
         *	@return	Abs normal.
         */
        Vector&         absNormal()
        {
            return m_plane.absNormal();
        }

        /**
         *	@brief	Get plain 3D plane.
         *
         *	@return	Plane.
         */
        const Plane3D&  getPlane() const
        {
            return m_plane;
        }

        /**
         *	@brief	Get plain 3D plane.
         *
         *	@return	Plane.
         */
        Plane3D&        getPlane()
        {
            return m_plane;
        }

        /**
         *	@brief	Set plane normal.
         *
         *	@param 	normal 	New normal.
         */
        void            set_normal(const Vector& normal)
        {
            m_plane.set_normal(normal);
        }

        /**
         *	@brief	Get name of frustum plane.
         *
         *	@return	Frustum name.
         */
        PLANES          get_plane_name()
        {
            return m_PLANE;
        }

        /**
         *	@brief	Set frustum plane name.
         *
         *	@param 	PLANE 	New name.
         */
        void            set_plane_name(PLANES PLANE)
        {
            m_PLANE = PLANE;
        }


        friend bool intersectionPointofPlanes(const Plane& p1, const Plane& p2, const Plane& p3, Vector& vec);
    };

    /**
     *	@brief	Check if planes have common point and set Vector3 arg to result.
     *
     *	@retval True if yes.
     *  @retval False if no common point.
     */
    bool intersectionPointofPlanes(const Plane3D& p1, const Plane3D& p2, const Plane3D& p3,
                                   Vector& vec);

    /**
     *	@brief	Check if planes have common point and set Vector3 arg to result.
     *
     *	@retval True if yes.
     *  @retval False if no common point.
     */
    bool intersectionPointofPlanes(const Plane& p1, const Plane& p2, const Plane& p3,
                                   Vector& vec);
}

#endif /* defined(__aexolGL__Plane__) */
