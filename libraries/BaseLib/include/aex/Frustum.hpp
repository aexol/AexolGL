/** @file Frustum.hpp
 *  @brief Definition of Frustum.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__Frustum__
#define __aexolGL__Frustum__


#include <aex/Plane.hpp>
#include <aex/AABB.hpp>
#include <array>

namespace aex
{
    namespace math
    {
        class Matrix;
    }
    class BoundingSphere;
    class Aex;
    class Frustum;
    typedef math::Matrix Matrix;

    enum class INTERSECTION : unsigned char
    {
        IN = 1, OUT = 2, INTERSECT = 3
    };

    namespace __frustum_detail
    {
        /**
         * @brief Helper overload in case tester object is a pointer or smart pointer.
         *
         * @tparam T
         */
        template<class T>
        struct AEX_PUBLIC_API get_intersection
        {
            template<class C>
            INTERSECTION operator()(const Frustum& f, C&& c){
                static_assert(std::is_same<typename std::decay<C>::type, typename std::decay<T>::type>::value, "Invalid parameters.");
                return c.frustumTest(f);
            }
        };

        template<class T>
        struct AEX_PUBLIC_API get_intersection<T*>
        {
            template<class C>
            INTERSECTION operator()(const Frustum& f, C&& c){
                static_assert(std::is_same<typename std::decay<C>::type, typename std::decay<T*>::type>::value, "Invalid parameters.");
                return c ? c->frustumTest(f) : INTERSECTION::IN;
            }
        };

        template<class T>
        struct AEX_PUBLIC_API get_intersection<std::unique_ptr<T>>
        {
            template<class C>
            INTERSECTION operator()(const Frustum& f, C&& c){
                static_assert(std::is_same<typename std::decay<C>::type, std::unique_ptr<T>>::value, "Invalid parameters.");
                return get_intersection<typename std::decay<T*>::type>()(f, c.get());
            }
        };

        template<class T>
        struct AEX_PUBLIC_API get_intersection<std::shared_ptr<T>>
        {
            template<class C>
            INTERSECTION operator()(const Frustum& f, C&& c){
                static_assert(std::is_same<typename std::decay<C>::type, std::shared_ptr<T>>::value, "Invalid parameters.");
                return get_intersection<typename std::decay<T*>::type>()(f, c.get());
            }
        };

        /**
         * @brief Test if object is inside frustum or outside.
         *
         * Helper for testing if an object is inside a frustum.
         *
         * @tparam Args Each type in Args must implement frustumTest(const Frustum&).
         *
         * TODO: frustumTest concept check.
         */
        template<class... Args> struct test_frustum;
        template<class T, class... Args> struct test_frustum<T, Args...>
        {
            template<class C, class... CArgs>
            bool operator()(const Frustum& f, C&& c, CArgs&&... cargs)
            {
                INTERSECTION intersection = get_intersection<typename std::decay<C>::type>()(f, aex::forward<C>(c));
                return intersection == INTERSECTION::INTERSECT ? test_frustum<typename std::decay<CArgs>::type...>()(f, aex::forward<CArgs>(cargs)...) : intersection == INTERSECTION::OUT;
            }
        };

        /**
         * @brief Inconclusive, last test resulted in intersect. Assume that the object is in and draw it.
         */
        template<> struct test_frustum<>
        {
            bool operator()(const Frustum& f)
            {
                UNUSED(f);
                return false;
            }
        };
    }


    /**
     *	@brief	Class representing camera Frustum.
     */
    class AEX_PUBLIC_API Frustum

    {
    private:
        std::array<Plane, 6> planes;
    public:
        /**
         *	@brief	Test frustum against sphere.
         *
         *	@param 	bSphere 	Test sphere.
         *
         *	@retval IN - object completly inside.
         *  @retval INTERSECTS - part of an object in frustum
         *  @retval OUT - object completly outside.
         */
        INTERSECTION containsSphere(const BoundingSphere* bSphere) const;
        int containsSphere2(const BoundingSphere* bSphere) const;

        /**
         *	@brief	Test frustum against AABB.
         *
         *	@param 	aabb 	Test aabb.
         *
         *	@retval IN - object completly inside.
         *  @retval INTERSECTS - part of an object in frustum
         *  @retval OUT - object completly outside.
         */
        INTERSECTION containsAABB(const AABB& aabb) const;
        int containsAABB2(const AABB& aabb) const;
        bool containsAABBbool(const AABB& aabb) const;

        
        INTERSECTION containsPointIntersect(const aex::math::Vector3& point) const;
        bool containsPoint(const aex::math::Vector3& point) const;
        /**
         *	@brief	Set frustum planes.
         *
         *	@param 	Matrix& 	Current View-Projection matrix.
         */
        void setFrustum(const Matrix&);


        const std::array<Plane, 6>& getPlanes() const
        {
            return planes;
        }
        /**
         *	@brief	Cull object using list of test objects.
         *
         *	Testing uses short circuit evaluation. It travels along the list of
         *	testers and the moment it finds either INTERSECTION::IN or INTERSECTION::OUT
         *	test is finished with a result.
         *
         *	In case of INTERSECTION::INTERSECT tests are continued either until the end of list
         *	or if INTERSECTION::IN or INTERSECTION::OUT is found.
         *
         * @tparam Args Tester types. Tester must implement frustumTest(const Frustum&)
         * @param args testers.
         *
         * @return False if tested list is IN or inconclusive (as in last test resulted in INTERSECTION::INTERSECT).
         * @return True If element in list returned INTERSECTION::OUT before any INTERSECTION::IN.
         */
        template<class... Args>
        bool frustumCull(Args&&... args) const
        {
            return __frustum_detail::test_frustum<typename std::decay<Args>::type...>()(
                    *this, aex::forward<Args>(args)...
                );
        }
    };
}
#endif /* defined(__aexolGL__Frustum__) */
