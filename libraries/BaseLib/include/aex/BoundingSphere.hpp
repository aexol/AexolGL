/** @file BoundingSphere.hpp
 *  @brief Definition of BoundingSphere.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef AexolGL_BoundingSphere_h
#define AexolGL_BoundingSphere_h

#include <aex/Vector3.hpp>
#include <aex/Frustum.hpp>


namespace aex
{

    /**
     *	@brief	Bounding sphere class.
     *  @details Typical bounding sphere. Nothing special going on here.
     *  @note The construction of this sphere is quite costly. It's best to pre-compute it and use those values at run-time.
     */
    class AEX_PUBLIC_API BoundingSphere

    {
        typedef aex::math::Vector3 Vector;
    public:
        /**
         *	@brief	Constructs infinitly small sphere.
         */
        BoundingSphere();

        /**
         *	@brief	Copy values from sphere.
         *
         *	@param 	bSphere 	Sphere to be copied.
         *
         *	@return	Reference to caller.
         */
        BoundingSphere& operator=(const BoundingSphere& bSphere);

        /**
         *	@brief	Compute center and radius of bounding sphere.
         *
         *  @details Given set of vertices, this method computes smallest possible bounding sphere around them.
         *
         *	@param 	vertices 	Set of vertices for which we should compute the sphere.
         *
         *  @note This can be quite expensive so try to avoid using it during game run-time. Result of this can be cached in file and reused. In worst case O(n2), which is not that bad compared to other algorithms but it's still pain in the ....
         */
        void welzlCompute(const std::vector<float>& vertices);

        /**
         *	@brief	Scale the sphere.
         *
         *	@param 	x 	Sphere will be resized times x.
         */
        void scale(float x);

        /**
         *	@brief	Move the sphere.
         *
         *	@param 	x 	Move along X axis.
         *	@param 	y 	Move along Y axis.
         *	@param 	z 	Move along Z axis.
         */
        void move(float x, float y, float z);


        /**
         *	@brief	Get sphere center.
         *
         *	@return	aex::math::Vector3
         */
        Vector      center() const
        {
            return m_center;
        }

        /**
         *	@brief	Sphere radius.
         *
         *	@return	Float.
         */
        float     radius() const
        {
            return m_radius;
        }

        /**
         *	@brief	Set sphere center.
         *
         *	@param 	center 	New center.
         */
        void        set_center(const Vector& center)
        {
            m_center = center;
        }

        /**
         *	@brief	Set radius.
         *
         *	@param 	radius 	New radius.
         */
        void        set_radius(float radius)
        {
            m_radius = radius;
        }

        /**
         *	@brief	Set float precision.
         *
         *	@param 	epsilon 	New precision.
         */
        void        set_epsilon(float epsilon)
        {
            m_Epsilon = epsilon;
        }


        /**
         *	@brief	Check if infinitely small sphere
         *
         *	@return	True if infinitely small, false otherwise.
         */
        bool        isNull() const
        {
            return (aex::math::Math::closeEnough(m_radius, 0.0f, m_Epsilon)) ? false : true;
        }

        INTERSECTION frustumTest(const Frustum& f);

    private:
        /**
         *	@brief	Sphere center.
         */
        Vector      m_center;

        /**
         *	@brief	Sphere radius.
         */
        float       m_radius;

        /**
         *	@brief	Precision of float comparison.
         */
        float       m_Epsilon = 0.001f;

        /**
         *	@brief	Create sphere around point.
         *
         *	@param 	Vector 	Point to be contained by sphere.
         */
        BoundingSphere(Vector p);
        /**
         *	@brief	Create sphere around two points.
         *
         *	@param 	p1 	Point to be contained by sphere.
         *	@param 	p2 	Point to be contained by sphere.
         */
        BoundingSphere(Vector p1, Vector p2);
        /**
         *	@brief	Create sphere around three points.
         *
         *	@param 	p1 	Point to be contained by sphere.
         *	@param 	p2 	Point to be contained by sphere.
         *	@param 	p3 	Point to be contained by sphere.
         */
        BoundingSphere(Vector p1, Vector p2, Vector p3);
        /**
         *	@brief	Create sphere around three points.
         *
         *	@param 	p1 	Point to be contained by sphere.
         *	@param 	p2 	Point to be contained by sphere.
         *	@param 	p3 	Point to be contained by sphere.
         *	@param 	p4 	Point to be contained by sphere.
         */
        BoundingSphere(Vector p1, Vector p2, Vector p3, Vector p4);


        /**
         *	@brief	Shhhhh it's a secret :P
         *
         *	@param 	vertices 	Whatever.
         */
        void welzlCalc(std::vector<Vector>& vertices);

        /**
         *	@brief	Include new point in sphere.
         *
         *	@param 	supportSet 	Set of vertices on which curent sphere is build.
         *	@param 	p 	Point to be included.
         *
         *	@return	True if resized, false otherwise.
         */
        bool update(std::vector<Vector>& supportSet, Vector p);

        /**
         *	@brief	Update sphere based on four points.
         *
         *	@param 	supportSet 	Set of vertices on which curent sphere is build.
         *	@param 	p 	Point to be included.
         *
         *	@return	True if resized, false otherwise.
         */
        bool updateFour(std::vector<Vector>&, Vector);

        /**
         *	@brief	Update sphere based on three points.
         *
         *	@param 	supportSet 	Set of vertices on which curent sphere is build.
         *	@param 	p 	Point to be included.
         *
         *	@return	True if resized, false otherwise.
         */
        bool updateThree(std::vector<Vector>& supportSet, Vector p);

        /**
         *	@brief	Update sphere based on two points.
         *
         *	@param 	supportSet 	Set of vertices on which curent sphere is build.
         *	@param 	p 	Point to be included.
         *
         *	@return	True if resized, false otherwise.
         */
        bool updateTwo(std::vector<Vector>& supportSet, Vector p);

        /**
         *	@brief	Update sphere based on one points.
         *
         *	@param 	supportSet 	Set of vertices on which curent sphere is build.
         *	@param 	p 	Point to be included.
         *
         *	@return	True if resized, false otherwise.
         */
        bool updateOne(std::vector<Vector>& supportSet, Vector p);

        /**
         *	@brief	Check if point is contained by sphere.
         *
         *	@param 	p 	Point to be tested.
         *
         *	@return	True if in, false otherwise.
         */
        bool contains(const Vector& p) const;




        void        setSphere(const Vector&);
        bool        setSphere(const Vector&, const Vector&);
        //Depracated 'cause it's buggy and stuff above works ^^
        void        setSphere(const Vector&, const Vector&, const Vector&);
        void        setSphere(const Vector&, const Vector&, const Vector&, const Vector&);
    };
}

#endif
