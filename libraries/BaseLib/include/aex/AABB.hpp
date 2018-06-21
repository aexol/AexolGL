/** @file AABB.hpp
 *  @brief Contains almost everything relevant to AABB
 *
 *  @details This file contains both structure
 *  for AABBBuilder and AABB class. This objects are generally used
 *  collision detection, culling etc.
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef AexolGL_AABB_h
#define AexolGL_AABB_h

#include <aex/Vector3.hpp>
#include <float.h>


namespace aex
{
    /**
     *	@brief	Helper class used to construct Axis-Aligned Bounding Box.
     *
     *  @details Helper class used to construct Axis-Aligned Bounding Box using
     *   minimum and maximum corners of the box, which differs from AABB implementation
     *   that is implemented using center of AABB and distances from that center.
     *
     *
     *  @code
     int main( int argc, char **argv )
     {
            aex::math::Vector3 minimum1( 0.0f, 0.0f, 0.0f );
            aex::math::Vector3 maximum1( 10.0f, 10.f, 10.f );

            aex::math::Vector3 minimum2( 10.0f, 10.f, 10.f );
            aex::math::Vector3 maximum2( 20.0f, 20.f, 20.f );

            AABBBuilder builder1( minimum1, maximum1 );
            AABBBuilder builder2( minimum2, maximum2 );
            AABBBuilder builder3( builder1, builder2 );

            return 0;
     } @endcode
     */
    struct AEX_PUBLIC_API AABBBuilder

    {
        /**
         *	@brief	Constructs AABBBuilder given min and max corner
         *
         *	@param 	min 	Minimum corner, default value is lowest possible flot.
         *	@param 	max 	Maximum corner, default value is highest possible float.
         */
        AABBBuilder() : AABBBuilder(aex::math::Vector3(FLT_MAX, FLT_MAX, FLT_MAX)) { }
        /**
         * @brief
         *
         * @param min
         */
        AABBBuilder(aex::math::Vector3 min) : AABBBuilder(min, aex::math::Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX)) {}
        /**
         * @brief
         *
         * @param min
         * @param max
         */
        AABBBuilder(aex::math::Vector3 min, aex::math::Vector3 max) : m_min(min), m_max(max) { }

        /**
         *	@brief	Wraps two AABBs in one.
         *  @details This constructs a builder containing two boxes supplied
         *  as parameters.
         */
        AABBBuilder(AABBBuilder a, AABBBuilder b)

        {
            m_min = aex::math::min(a.m_min, b.m_min);
            m_max = aex::math::max(a.m_max, b.m_max);
        }
        
        
                /**
         *	@brief	Minimum corner of bounding box.
         */
        aex::math::Vector3 m_min;
        /**
         *	@brief	Maximum corner of bounding box.
         */
        aex::math::Vector3 m_max;
    };
    /**
     *	@brief	Axis-Aligned Bounding Box.
     *  @details AABB is a representation of
     */
    class AEX_PUBLIC_API AABB
    {
    public:
        /**
        *	@brief	Construct default, infinite AABB.
        */
        AABB();

        
        AABB(aex::math::Vector3 center, aex::math::Vector3  halfdist);

        
        /**
        *	@brief	Constructs AABB from builder.
        *
        *  @param builder Reference to a builder used for constructing AABB.
        */
        AABB(const AABBBuilder& builder);
        /**
         *	@brief	Contructs AABB wrapping two different AABBs.
         *
         *	@param 	a 	First AABB.
         *	@param 	b 	Second AABB.
         */
        AABB(const AABB& a, const AABB& b);

        /**
         *	@brief	Get reference to a center of AABB.
         *
         *	@return	Reference to center.
         */
        aex::math::Vector3&     center()
        {
            return m_center;
        }

        /**
         *	@brief	Copy of center of AABB.
         *
         *	@return	center.
         */
        aex::math::Vector3      center() const
        {
            return m_center;
        }

        /**
         *	@brief	Sets center of AABB.
         *
         *	@param 	center 	Value to set center to.
         */
        void                setCenter(const aex::math::Vector3& center)
        {
            m_center = center;
        }

        /**
         *	@brief	Returns a reference to halfdist, which is distances along
         *   in order X,Y and Z axes.
         *
         *	@return	Reference to halfdist
         */
        aex::math::Vector3&     halfdist()
        {
            return m_halfdist;
        }

        /**
         *	@brief	Returns a copy to halfdist, which is distances along
         *   in order X,Y and Z axes.
         *
         *	@return	Copy to halfdist.
         */
        aex::math::Vector3      halfdist() const
        {
            return m_halfdist;
        }

        /**
         *	@brief	Sets halfdist.
         *
         *	@param 	halfdist 	Value to set.
         */
        void                setHalfdist(const aex::math::Vector3& halfdist)
        {
            m_halfdist = halfdist;
        }

        /**
         *	@brief	Minimum corner of this AABB.
         *
         *	@return	Min.
         */
        aex::math::Vector3      min() const
        {
            return (m_center - m_halfdist);
        }

        /**
         *	@brief	Maximum corner of this AABB.
         *
         *	@return	Max.
         */
        aex::math::Vector3      max() const
        {
            return (m_center + m_halfdist);
        }

        /**
         *	@brief	Check if this AABB intersects with aabb at any point.
         *
         *	@param 	aabb 	AABB to test.
         *
         *	@return	True if aabb intersects with caller, false if outside.
         */
        bool                overlapsWith(const AABB& aabb) const;

        /**
         *	@brief	Convienience method returning Infinite AABB.
         *
         *	@return	Infinite AABB.
         */
        static AABB         Infinite();
        AABB& operator= (const AABB& sec);
        
        void setLastDiscardPlane(size_t lastDiscardPlane);
        size_t getLastDiscardPlane() const;
        friend std::ostream& operator<<(std::ostream& os, const AABB& vec);
    private:
        
        size_t m_lastDiscardPlane=0;
        
        /**
         *	@brief	Center of AABB.
         */
        aex::math::Vector3 m_center;

        /**
         *	@brief	Half lengths of X, Y and Z vectors of AABB.
         */
        aex::math::Vector3 m_halfdist;

        /**
         *	@brief	Private helper method to build AABB from builder.
         *
         *	@param 	builder
         */
        void fromBuilderToAABB(const AABBBuilder& builder);

    };
    
    inline std::ostream& operator<<(std::ostream& os, const AABB& aabb)
    {
        os << "c:" << aabb.center() << ",h: " << aabb.halfdist();
        return os;
    }
}


#endif
