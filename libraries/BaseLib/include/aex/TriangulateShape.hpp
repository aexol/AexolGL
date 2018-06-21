

#ifndef TRIANGULATESHAPE_HPP
#define	TRIANGULATESHAPE_HPP
#include <aex/Common.hpp>
#include <aex/Vector3.hpp>

namespace aex
{

    typedef aex::math::Vector3 Vector3;
    typedef std::vector<Vector3> Vector3dVector;

    class AEX_PUBLIC_API TriangulateShape
    {
    public:

        static bool Process(const Vector3dVector& contour,
                            Vector3dVector& result, std::vector<aex::mesh_indices_type>& indices);

        static std::vector<aex::mesh_indices_type> Process(const Vector3dVector& contour);

        static float Area(const Vector3dVector& contour);
        static bool InsideTriangle(float Ax, float Ay,
                                   float Bx, float By,
                                   float Cx, float Cy,
                                   float Px, float Py);


    private:
        static bool Snip(const Vector3dVector& contour, int u, int v, int w, int n, int* V);

    };

}
#endif	/* TRIANGULATESHAPE_HPP */

