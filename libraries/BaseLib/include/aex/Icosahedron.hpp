
//  Icosahendron.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 16.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#ifndef __aexolGL__Icosahendron__
#define __aexolGL__Icosahendron__

#include <aex/Mesh.hpp>
#include <list>
#include <array>

namespace aex
{
    class Mesh;
    typedef aex::shared_ptr<Mesh> Mesh_ptr;

    template <class T, size_t ROW, size_t COL>
    struct AEX_PUBLIC_API Array2D
    {
        typedef std::array<std::array<T, COL>, ROW> type;
    };

    class AEX_PUBLIC_API Icosahedron : public Mesh
    {
    public:

        
        Icosahedron(int16_t = 1, aex::string = "Icosahedron_level");
        
        
        static void sphere(float radius, int HSegments, int WSegments, Mesh_ptr mesh, bool TriangleStrip = false);
        static void pipe(float radius, float height, int HSegments, int WSegments, Mesh_ptr mesh, bool TriangleStrip = false);
        static void cube(float radius, int Segments, Mesh_ptr mesh, bool TriangleStrip = false, bool textureCube = false);
        static void cube(Mesh_ptr mesh);
        static void cubedSphere(float radius, int Segments, Mesh_ptr mesh, bool TriangleStrip = false, bool textureCube = false);
        
        static void sphere(float radius, int HSegments, int WSegments, Mesh* mesh, bool TriangleStrip = false);
        static void pipe(float radius, float height, int HSegments, int WSegments, Mesh* mesh, bool TriangleStrip = false);
        static void cube(float radius, int Segments, Mesh* mesh, bool TriangleStrip = false, bool textureCube = false);
        static void cube(Mesh* mesh);
        static void cubedSphere(float radius, int Segments, Mesh* mesh, bool TriangleStrip = false, bool textureCube = false);


    private:
        std::list<float> vertexNormalsList;
        std::list<float> vertexList;
        void norm(std::array<float, 3>&);
        void add(std::array<float, 3>&);
        void subdivide(std::array<float, 3>, std::array<float, 3>, std::array<float, 3>, int16_t);

    };
    
    
    Mesh makeSphere(float radius, int HSegments, int WSegment, bool TriangleStrip = false);
    Mesh makeCubeSphere(float radius, int Segments, bool TriangleStrip = false, bool textureCube = false);
    AEX_PUBLIC_API Mesh makePipe(float radius, float height, int HSegments, int WSegments, bool TriangleStrip = false);
    AEX_PUBLIC_API Mesh makeCube(float radius, int Segments, bool TriangleStrip = false, bool textureCube = false);
    
}

#endif /* defined(__aexolGL__Icosahendron__) */
