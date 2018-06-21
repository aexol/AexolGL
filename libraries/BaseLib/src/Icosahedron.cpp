//
//  Icosahendron.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 16.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#define _USE_MATH_DEFINES
#include <cmath>
#include <aex/Icosahedron.hpp>
#include <aex/Logger.hpp>
#include <string>

namespace aex {
#define X 0.525731112119133606f
#define Z 0.850650808352039932f
    static const Array2D<float, 20, 3>::type vdata ={
        {
            {
                { -X, 0.0f, Z}
            },
            {
                {X, 0.0f, Z}
            },
            {
                { -X, 0.0f, -Z}
            },
            {
                {X, 0.0f, -Z}
            },
            {
                {0.0f, Z, X}
            },
            {
                {0.0f, Z, -X}
            },
            {
                {0.0f, -Z, X}
            },
            {
                {0.0f, -Z, -X}
            },
            {
                {Z, X, 0.0f}
            },
            {
                { -Z, X, 0.0f}
            },
            {
                {Z, -X, 0.0f}
            },
            {
                { -Z, -X, 0.0f}
            }
        }
    };
    
    static const Array2D<int16_t, 20, 3>::type tindices ={
        {
            {
                {0, 4, 1}
            },
            {
                {0, 9, 4}
            },
            {
                {9, 5, 4}
            },
            {
                {4, 5, 8}
            },
            {
                {4, 8, 1}
            },
            {
                {8, 10, 1}
            },
            {
                {8, 3, 10}
            },
            {
                {5, 3, 8}
            },
            {
                {5, 2, 3}
            },
            {
                {2, 7, 3}
            },
            {
                {7, 10, 3}
            },
            {
                {7, 6, 10}
            },
            {
                {7, 11, 6}
            },
            {
                {11, 0, 6}
            },
            {
                {0, 1, 6}
            },
            {
                {6, 1, 10}
            },
            {
                {9, 0, 11}
            },
            {
                {9, 11, 2}
            },
            {
                {9, 2, 5}
            },
            {
                {7, 2, 11}
            }
        }
    };



    Icosahedron::Icosahedron(int16_t level, aex::string name) : Mesh(name.append(aex::to_string(level)), false, false, true) {

        for (int i = 0; i < 20; ++i) {

            subdivide(vdata[tindices[i][0]],
                    vdata[tindices[i][1]],
                    vdata[tindices[i][2]], level);
        }
    }

    void
    Icosahedron::norm(std::array<float, 3>& v) {

        float len = 0;

        for (int i = 0; i < 3; ++i) {
            len += v[i] * v[i];
        }

        len = sqrt(len);

        for (int i = 0; i < 3; ++i) {
            v[i] /= len;
        }
    }

    void
    Icosahedron::add(std::array<float, 3>& v) {
        for (int k = 0; k < 3; ++k) {
            vertexList.push_back(v[k]);
            vertexNormalsList.push_back(v[k]);
        }
    }

    void
    Icosahedron::subdivide(std::array<float, 3> v1, std::array<float, 3> v2, std::array<float, 3> v3, int16_t depth) {

        if (depth == 0) {
            add(v1);
            add(v2);
            add(v3);
            return;
        }

        std::array<float, 3> v12;
        std::array<float, 3> v23;
        std::array<float, 3> v31;

        for (int i = 0; i < 3; ++i) {
            v12[i] = (v1[i] + v2[i]) / 2.0f;
            v23[i] = (v2[i] + v3[i]) / 2.0f;
            v31[i] = (v3[i] + v1[i]) / 2.0f;
        }

        norm(v12);
        norm(v23);
        norm(v31);

        subdivide(v1, v12, v31, depth - 1);
        subdivide(v2, v23, v12, depth - 1);
        subdivide(v3, v31, v23, depth - 1);
        subdivide(v12, v23, v31, depth - 1);
    }

    void
    Icosahedron::sphere(float radius, int Hsegments, int Wsegments, Mesh* mesh, bool isTriangleStrip) {

        float R = 1.0f / (GLfloat) (Hsegments - 1);
        float S = 1.0f / (GLfloat) (Wsegments - 1);

        std::vector<mesh_vectors_type> Temp_Vertices;
        std::vector<mesh_vectors_type> Temp_Normals;
        std::vector<mesh_vectors_type> Temp_TextCoords;

        Temp_Vertices.resize(Hsegments * Wsegments * 3);
        Temp_Normals.resize(Hsegments * Wsegments * 3);
        Temp_TextCoords.resize(Hsegments * Wsegments * 2);

        std::vector<mesh_vectors_type>::iterator v = Temp_Vertices.begin();
        std::vector<mesh_vectors_type>::iterator n = Temp_Normals.begin();
        std::vector<mesh_vectors_type>::iterator t = Temp_TextCoords.begin();


        for (int r = 0; r < Hsegments; r++) {
            for (int s = 0; s < Wsegments; s++) {

                float const y = sin(-M_PI_2 + M_PI * r * R);
                float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
                float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

                *t++ = s * S;
                *t++ = r * R;

                *v++ = x * radius;
                *v++ = y * radius;
                *v++ = z * radius;

                *n++ = x;
                *n++ = y;
                *n++ = z;

            }
        }

        std::vector<mesh_indices_type> Temp_Indices;

        if (isTriangleStrip) {

            int numStripsRequired = Hsegments - 1;
            int numDegensRequired = 2 * (numStripsRequired - 1);
            int verticesPerStrip = 2 * Wsegments;
            int size = (verticesPerStrip * numStripsRequired) + numDegensRequired;
            LOG_DEBUG("size = ", size);
            Temp_Indices.resize(size);

            std::vector<mesh_indices_type>::iterator i = Temp_Indices.begin();

            for (int y = 0; y < Hsegments - 1; y++) {
                if (y > 0) {
                    // Degenerate begin: repeat first vertex
                    *i++ = (mesh_indices_type) (y * Hsegments);
                }

                for (int x = 0; x < Wsegments; x++) {
                    // One part of the strip
                    *i++ = (mesh_indices_type) (((y + 1) * Hsegments) + x);
                    *i++ = (mesh_indices_type) ((y * Hsegments) + x);
                }

                if (y < Hsegments - 2) {
                    // Degenerate end: repeat last vertex
                    *i++ = (mesh_indices_type) (((y + 1) * Hsegments) + (Wsegments - 1));
                }
            }

            mesh->setDrawMode(GL_TRIANGLE_STRIP);
        } else {

            int size = (Hsegments - 1) * (Wsegments - 1) * 2 * 3;
            LOG_DEBUG("size = ", size);
            Temp_Indices.resize(size);
            std::vector<mesh_indices_type>::iterator i = Temp_Indices.begin();

            for (int y = 0; y < Hsegments - 1; ++y) {
                for (int x = 0; x < Wsegments; ++x) {
                    if (y < Hsegments - 1 && x < Wsegments - 1) {

                        int currentVertex = (y * (Wsegments)) + x;

                        *i++ = currentVertex;
                        *i++ = currentVertex + 1;
                        *i++ = currentVertex + Wsegments;

                        *i++ = currentVertex + 1;
                        *i++ = currentVertex + Wsegments + 1;
                        *i++ = currentVertex + Wsegments;
                    }
                }
            }
        }

        mesh->setFloatAttribute(std::move(Temp_Vertices), "gl_Vertex", 3);
        mesh->setFloatAttribute(std::move(Temp_Normals), "gl_Normal", 3);
        mesh->setFloatAttribute(std::move(Temp_TextCoords), "gl_TexCoord", 2);
        mesh->setTriangles(std::move(Temp_Indices));

    }

    void
    Icosahedron::pipe(float radius, float height, int Hsegments, int Wsegments, Mesh* mesh, bool isTriangleStrip) {

        Wsegments++;
        float R = 1.0f / (GLfloat) (Hsegments - 1);
        float S = 1.0f / (GLfloat) (Wsegments - 1);
        float halfheight = height / 2.0f;
        std::vector<mesh_vectors_type> Temp_Vertices;
        std::vector<mesh_vectors_type> Temp_Normals;
        std::vector<mesh_vectors_type> Temp_TextCoords;

        Temp_Vertices.resize(Hsegments * Wsegments * 3);
        Temp_Normals.resize(Hsegments * Wsegments * 3);
        Temp_TextCoords.resize(Hsegments * Wsegments * 2);

        std::vector<mesh_vectors_type>::iterator v = Temp_Vertices.begin();
        std::vector<mesh_vectors_type>::iterator n = Temp_Normals.begin();
        std::vector<mesh_vectors_type>::iterator t = Temp_TextCoords.begin();



        for (int r = 0; r < Hsegments; r++) {
            for (int s = 0; s < Wsegments; s++) {

                float const y = (r * R * height) - halfheight;
                float const x = cos(2 * M_PI * s * S);
                float const z = sin(2 * M_PI * s * S);

                *t++ = s * S;
                *t++ = r * R;

                *n++ = x;
                *n++ = 0.0f;
                *n++ = z;

                *v++ = x * radius;
                *v++ = y;
                *v++ = z * radius;


            }
        }

        std::vector<mesh_indices_type> Temp_Indices;

        if (isTriangleStrip) {

            int numStripsRequired = Hsegments;
            int numDegensRequired = 2 * (numStripsRequired - 1);
            int verticesPerStrip = 2 * Wsegments;
            int size = (verticesPerStrip * numStripsRequired) + numDegensRequired - 1;
            LOG_DEBUG("size = ", size);
            Temp_Indices.resize(size);

            std::vector<mesh_indices_type>::iterator i = Temp_Indices.begin();

            for (int y = 0; y < Hsegments; y++) {
                if (y > 0) {
                    // Degenerate begin: repeat first vertex
                    *i++ = (mesh_indices_type) (y * Hsegments);
                }

                for (int x = 0; x < Wsegments; x++) {
                    // One part of the strip
                    *i++ = (mesh_indices_type) (((y + 1) * Hsegments) + x);
                    *i++ = (mesh_indices_type) ((y * Hsegments) + x);
                }

                if (y < Hsegments - 1) {
                    // Degenerate end: repeat last vertex
                    *i++ = (mesh_indices_type) ((y * Hsegments) + (Wsegments - 1));
                }
            }

            mesh->setDrawMode(GL_TRIANGLE_STRIP);
        } else {

            int size = (Hsegments - 1) * (Wsegments - 1) * 2 * 3;
            LOG_DEBUG("size = ", size);
            Temp_Indices.resize(size);
            std::vector<mesh_indices_type>::iterator i = Temp_Indices.begin();

            for (int y = 0; y < Hsegments - 1; ++y) {
                for (int x = 0; x < Wsegments; ++x) {
                    if (y < Hsegments - 1 && x < Wsegments - 1) {

                        int currentVertex = (y * (Wsegments)) + x;

                        *i++ = currentVertex;
                        *i++ = currentVertex + 1;
                        *i++ = currentVertex + Wsegments;


                        *i++ = currentVertex + 1;
                        *i++ = currentVertex + Wsegments + 1;
                        *i++ = currentVertex + Wsegments;
                    }
                }
            }
        }


        mesh->setFloatAttribute(std::move(Temp_Vertices), "gl_Vertex", 3);
        mesh->setFloatAttribute(std::move(Temp_Normals), "gl_Normal", 3);
        mesh->setFloatAttribute(std::move(Temp_TextCoords), "gl_TexCoord", 2);
        mesh->setTriangles(std::move(Temp_Indices));

    }

    void Icosahedron::cube(float radius, int segments, Mesh* mesh, bool isTriangleStrip, bool textureCube) {
        segments = segments + 1;
        float cellsize = radius / (float) (segments - 1);
        float uvcellsize = 1.0 / (float) (segments - 1);
        float halfRadius = radius / 2.0f;
        std::vector<mesh_vectors_type> Temp_Vertices;
        std::vector<mesh_vectors_type> Temp_Normals;
        std::vector<mesh_vectors_type> Temp_TextCoords;
        std::vector<mesh_indices_type> Temp_Indices;

        int walls = 6;

        Temp_Vertices.resize(segments * segments * 3 * walls);
        Temp_Normals.resize(segments * segments * 3 * walls);

        if(!textureCube)
        {
            Temp_TextCoords.resize(segments * segments * 2 * walls);
        }
        else
        {
            Temp_TextCoords.resize(segments * segments * 3 * walls);
        }

        std::vector<mesh_vectors_type>::iterator v = Temp_Vertices.begin();
        std::vector<mesh_vectors_type>::iterator n = Temp_Normals.begin();
        std::vector<mesh_vectors_type>::iterator t = Temp_TextCoords.begin();

        if (isTriangleStrip) {
            int numStripsRequired = segments - 1;
            int numDegensRequired = 2 * (numStripsRequired - 1);
            int verticesPerStrip = 2 * segments;
            int size = ((verticesPerStrip * numStripsRequired) + numDegensRequired) * walls + (walls * 2 - 1);

            Temp_Indices.resize(size);

        } else {
            int size = (segments - 1) * (segments - 1) * 2 * 3 * 6;
            Temp_Indices.resize(size, 0);
        }


        std::vector<mesh_indices_type>::iterator i = Temp_Indices.begin();
        int offset = 0;
        int tempoff = 0;

        for (int cubeface = 0; cubeface < walls; ++cubeface) {

            for (int x = 0; x < segments; ++x) {
                for (int y = 0; y < segments; ++y) {

                    if (cubeface == 0) {
                        *v++ = ((float) x * cellsize) - halfRadius;
                        *v++ = ((float) y * cellsize) - halfRadius;
                        *v++ = -halfRadius;

                        *t++ = (float) 1.0 - (x * uvcellsize);
                        *t++ = (float) y * uvcellsize;

                        if (textureCube) {
                            *t++ = (float) - 1.0f;
                        }

                        *n++ = 0.0f;
                        *n++ = 0.0f;
                        *n++ = -1.0f;

                    } else if (cubeface == 1) {
                        *v++ = ((float) y * cellsize) - halfRadius;
                        *v++ = ((float) x * cellsize) - halfRadius;
                        *v++ = halfRadius;

                        *t++ = (float) y * uvcellsize;
                        *t++ = (float) x * uvcellsize;

                        if (textureCube) {
                            *t++ = 1.0f;
                        }

                        *n++ = 0.0f;
                        *n++ = 0.0f;
                        *n++ = 1.0f;
                    } else if (cubeface == 2) {

                        *v++ = ((float) y * cellsize) - halfRadius;
                        *v++ = -halfRadius;
                        *v++ = ((float) x * cellsize) - halfRadius;

                        *t++ = (float) x * uvcellsize;

                        if (textureCube) {
                            *t++ = -1.0f;
                        }

                        *t++ = ((float) y * uvcellsize);

                        *n++ = 0.0f;
                        *n++ = -1.0f;
                        *n++ = 0.0f;
                    } else if (cubeface == 3) {
                        *v++ = ((float) x * cellsize) - halfRadius;
                        *v++ = halfRadius;
                        *v++ = ((float) y * cellsize) - halfRadius;

                        *t++ = (float) x * uvcellsize;

                        if (textureCube) {
                            *t++ = 1.0f;
                        }

                        *t++ = (float) y * uvcellsize;

                        *n++ = 0.0f;
                        *n++ = 1.0f;
                        *n++ = 0.0f;
                    } else if (cubeface == 4) {
                        *v++ = -halfRadius;
                        *v++ = ((float) x * cellsize) - halfRadius;
                        *v++ = ((float) y * cellsize) - halfRadius;

                        if (textureCube) {
                            *t++ = -1.0f;
                        }

                        *t++ = (float) y * uvcellsize;
                        *t++ = (float) x * uvcellsize;

                        *n++ = -1.0f;
                        *n++ = 0.0f;
                        *n++ = 0.0f;
                    } else if (cubeface == 5) {
                        *v++ = halfRadius;
                        *v++ = ((float) y * cellsize) - halfRadius;
                        *v++ = ((float) x * cellsize) - halfRadius;

                        if (textureCube) {
                            *t++ = 1.0f;
                        }

                        *t++ = (float) 1.0 - (x * uvcellsize);
                        *t++ = (float) y * uvcellsize;

                        *n++ = 1.0f;
                        *n++ = 0.0f;
                        *n++ = 0.0f;
                    }
                }
            }


            if (isTriangleStrip) {
                for (int a = 0; a < segments - 1; a++) {

                    if (cubeface > 0 && a == 0) {
                        *i++ = (mesh_indices_type) offset;

                        tempoff++;
                    }

                    if (a > 0) {
                        // Degenerate begin: repeat first vertex
                        *i++ = (mesh_indices_type) ((a * segments) + offset);

                        tempoff++;
                    }

                    for (int b = 0; b < segments; b++) {
                        // One part of the strip
                        *i++ = (mesh_indices_type) ((a * segments) + b + offset);
                        *i++ = (mesh_indices_type) (((a + 1) * segments) + b + offset);

                        tempoff += 2;
                    }

                    if (a < segments) {
                        // Degenerate end: repeat last vertex
                        *i++ = (mesh_indices_type) (((a + 1) * segments) + (segments - 1) + offset);

                        tempoff++;
                    }
                }

                offset += segments * segments;

            } else {
                //mesh_indices_type counter = 0;
                for (mesh_indices_type y = 0; y < segments; ++y) {
                    for (mesh_indices_type x = 0; x < segments; ++x) {
                        if (y < segments - 1 && x < segments - 1) {

                            mesh_indices_type currentVertex = (y * (segments)) + x + offset;

                            *i++ = currentVertex + segments;
                            *i++ = currentVertex + 1;
                            *i++ = currentVertex;

                            *i++ = currentVertex + segments;
                            *i++ = currentVertex + segments + 1;
                            *i++ = currentVertex + 1;
                            //counter+=2;
                            //LOG("counter: ",(offset + counter)/2);
                        }
                    }
                }

                offset += segments * segments;
            }
        }

        if (isTriangleStrip) {
            mesh->setDrawMode(GL_TRIANGLE_STRIP);
        }

        mesh->setFloatAttribute(std::move(Temp_Vertices), "gl_Vertex", 3);
        mesh->setFloatAttribute(std::move(Temp_Normals), "gl_Normal", 3);

        if (!textureCube) {
            mesh->setFloatAttribute(std::move(Temp_TextCoords), "gl_TexCoord", 2);
        } else {
            mesh->setFloatAttribute(std::move(Temp_TextCoords), "gl_TexCoord", 3);
        }

        mesh->setTriangles(std::move(Temp_Indices));

    }

    void Icosahedron::cubedSphere(float radius, int segments, Mesh* mesh, bool isTriangleStrip, bool textureCube) {
        segments += 1;

        float cellsize = radius / (float) (segments - 1);
        float uvcellsize = 1.0 / (float) (segments - 1);
        float halfRadius = radius / 2.0f;

        std::vector<mesh_vectors_type> Temp_Vertices;
        std::vector<mesh_vectors_type> Temp_Normals;
        std::vector<mesh_vectors_type> Temp_TextCoords;
        std::vector<mesh_indices_type> Temp_Indices;

        int walls = 6;

        Temp_Vertices.resize(segments * segments * walls * 3);
        Temp_Normals.resize(segments * segments * 3 * walls);
        if(!textureCube)
        {
            Temp_TextCoords.resize(segments * segments * 2 * walls);
        }
        else
        {
            Temp_TextCoords.resize(segments * segments * 3 * walls);
        }

        int v = 0;
        int n = 0;
        int t = 0;


        if (isTriangleStrip) {
            int numStripsRequired = segments - 1;
            int numDegensRequired = 2 * (numStripsRequired - 1);
            int verticesPerStrip = 2 * segments;
            int size = ((verticesPerStrip * numStripsRequired) + numDegensRequired) * walls + (walls * 2 - 1);

            Temp_Indices.resize(size);
            LOG_DEBUG("isTriangleStrip size: ", size);

        } else {
            int size = (segments - 1) * (segments - 1) * 2 * 3 * walls;
            Temp_Indices.resize(size);
            LOG_DEBUG("noisTriangleStrip size: ", size);
        }

        int i = 0;

        int offset = 0;
        int tempoff = 0;
        Vector3 newv;

        for (int cubeface = 0; cubeface < walls; ++cubeface) {

            for (int x = 0; x < segments; ++x) {
                for (int y = 0; y < segments; ++y) {

                    if (cubeface == 0) {
                        newv.x = ((float) x * cellsize) - halfRadius;
                        newv.y = ((float) y * cellsize) - halfRadius;
                        newv.z = -halfRadius;

                        newv.normalizeThis();
                        Temp_Normals[n++] = newv.x;
                        Temp_Normals[n++] = newv.y;
                        Temp_Normals[n++] = newv.z;

                        newv *= halfRadius;

                        Temp_Vertices[v++] = newv.x;
                        Temp_Vertices[v++] = newv.y;
                        Temp_Vertices[v++] = newv.z;

                        Temp_TextCoords[t++] = (float) 1.0 - (x * uvcellsize);
                        Temp_TextCoords[t++] = (float) y * uvcellsize;

                        if (textureCube) {
                            Temp_TextCoords[t++] = (float) - 1.0f;
                        }


                    } else if (cubeface == 1) {

                        newv.x = ((float) y * cellsize) - halfRadius;
                        newv.y = ((float) x * cellsize) - halfRadius;
                        newv.z = halfRadius;

                        newv.normalizeThis();

                        Temp_Normals[n++] = newv.x;
                        Temp_Normals[n++] = newv.y;
                        Temp_Normals[n++] = newv.z;

                        newv *= halfRadius;

                        Temp_Vertices[v++] = newv.x;
                        Temp_Vertices[v++] = newv.y;
                        Temp_Vertices[v++] = newv.z;


                        Temp_TextCoords[t++] = (float) y * uvcellsize;
                        Temp_TextCoords[t++] = (float) x * uvcellsize;

                        if (textureCube) {
                            Temp_TextCoords[t++] = 1.0f;
                        }


                    } else if (cubeface == 2) {

                        newv.x = ((float) y * cellsize) - halfRadius;
                        newv.y = -halfRadius;
                        newv.z = ((float) x * cellsize) - halfRadius;

                        newv.normalizeThis();
                        Temp_Normals[n++] = newv.x;
                        Temp_Normals[n++] = newv.y;
                        Temp_Normals[n++] = newv.z;

                        newv *= halfRadius;

                        Temp_Vertices[v++] = newv.x;
                        Temp_Vertices[v++] = newv.y;
                        Temp_Vertices[v++] = newv.z;

                        Temp_TextCoords[t++] = (float) x * uvcellsize;

                        if (textureCube) {
                            Temp_TextCoords[t++] = -1.0f;
                        }

                        Temp_TextCoords[t++] = ((float) y * uvcellsize);

                    } else if (cubeface == 3) {
                        newv.x = ((float) x * cellsize) - halfRadius;
                        newv.y = halfRadius;
                        newv.z = ((float) y * cellsize) - halfRadius;

                        newv.normalizeThis();
                        Temp_Normals[n++] = newv.x;
                        Temp_Normals[n++] = newv.y;
                        Temp_Normals[n++] = newv.z;

                        newv *= halfRadius;

                        Temp_Vertices[v++] = newv.x;
                        Temp_Vertices[v++] = newv.y;
                        Temp_Vertices[v++] = newv.z;

                        Temp_TextCoords[t++] = (float) x * uvcellsize;

                        if (textureCube) {
                            Temp_TextCoords[t++] = 1.0f;
                        }

                        Temp_TextCoords[t++] = (float) y * uvcellsize;


                    } else if (cubeface == 4) {
                        newv.x = -halfRadius;
                        newv.y = ((float) x * cellsize) - halfRadius;
                        newv.z = ((float) y * cellsize) - halfRadius;


                        newv.normalizeThis();
                        Temp_Normals[n++] = newv.x;
                        Temp_Normals[n++] = newv.y;
                        Temp_Normals[n++] = newv.z;

                        newv *= halfRadius;

                        Temp_Vertices[v++] = newv.x;
                        Temp_Vertices[v++] = newv.y;
                        Temp_Vertices[v++] = newv.z;

                        if (textureCube) {
                            Temp_TextCoords[t++] = -1.0f;
                        }

                        Temp_TextCoords[t++] = (float) y * uvcellsize;
                        Temp_TextCoords[t++] = (float) x * uvcellsize;


                    } else if (cubeface == 5) {
                        newv.x = halfRadius;
                        newv.y = ((float) y * cellsize) - halfRadius;
                        newv.z = ((float) x * cellsize) - halfRadius;

                        newv.normalizeThis();
                        Temp_Normals[n++] = newv.x;
                        Temp_Normals[n++] = newv.y;
                        Temp_Normals[n++]= newv.z;

                        newv *= halfRadius;

                        Temp_Vertices[v++] = newv.x;
                        Temp_Vertices[v++] = newv.y;
                        Temp_Vertices[v++] = newv.z;

                        if (textureCube) {
                            Temp_TextCoords[t++] = 1.0f;
                        }

                        Temp_TextCoords[t++] = (float) 1.0 - (x * uvcellsize);
                        Temp_TextCoords[t++] = (float) y * uvcellsize;

                    }
                }
            }


            if (isTriangleStrip) {
                for (int a = 0; a < segments - 1; a++) {

                    if (cubeface > 0 && a == 0) {
                        Temp_Indices[i++] = (mesh_indices_type) offset;

                        tempoff++;
                    }

                    if (a > 0) {
                        // Degenerate begin: repeat first vertex
                        Temp_Indices[i++] = (mesh_indices_type) ((a * segments) + offset);

                        tempoff++;
                    }

                    for (int b = 0; b < segments; b++) {
                        // One part of the strip
                        Temp_Indices[i++] = (mesh_indices_type) ((a * segments) + b + offset);
                        Temp_Indices[i++] = (mesh_indices_type) (((a + 1) * segments) + b + offset);

                        tempoff += 2;
                    }

                    if (a < segments) {
                        // Degenerate end: repeat last vertex
                        Temp_Indices[i++] = (mesh_indices_type) (((a + 1) * segments) + (segments - 1) + offset);

                        tempoff++;
                    }
                }

                offset += segments * segments;

            } else {

                for (int y = 0; y < segments - 1; ++y) {
                    for (int x = 0; x < segments; ++x) {
                        if (y < segments - 1 && x < segments - 1) {

                            int currentVertex = (y * (segments)) + x + offset;

                            Temp_Indices[i++] = currentVertex + segments;
                            Temp_Indices[i++] = currentVertex + 1;
                            Temp_Indices[i++]=  currentVertex;

                            Temp_Indices[i++] = currentVertex + segments;
                            Temp_Indices[i++]=  currentVertex + segments + 1;
                            Temp_Indices[i++] = currentVertex + 1;
                        }
                    }
                }
                LOG_DEBUG("iterator: ",i);
                offset += segments * segments;
            }
        }

        if (isTriangleStrip) {
            mesh->setDrawMode(GL_TRIANGLE_STRIP);
        }


        if(Temp_Vertices.size() % 3 != 0)
        {
            LOG_ERR("Temp_Vertices.size()%3!=0");
        }

        if(Temp_Normals.size()%3!=0)
        {
            LOG_ERR("Temp_Normals.size()%3!=0");
        }

        if(Temp_Indices.size()%3!=0)
        {
            LOG_ERR("Temp_Indices.size()%3!=0");
        }

        if (!textureCube) {
            mesh->setFloatAttribute(std::move(Temp_TextCoords), "gl_TexCoord", 2);
        } else {
            mesh->setFloatAttribute(std::move(Temp_TextCoords), "gl_TexCoord", 3);
        }

        mesh->setFloatAttribute(std::move(Temp_Vertices), "gl_Vertex", 3);
        mesh->setFloatAttribute(std::move(Temp_Normals), "gl_Normal", 3);
        mesh->setTriangles(std::move(Temp_Indices));
        mesh->compile();
    }

    void Icosahedron::cube(Mesh* mesh)
    {
        std::vector<float> verts {
        -10.0f, -10.0f, -10.0f,
        10.0f, -10.0f, -10.0f,
        10.0f, 10.0f, -10.0f,
        -10.0f, 10.0f, -10.0f,
        -10.0f, -10.0f, 10.0f,
        10.0f, -10.0f, 10.0f,
        10.0f, 10.0f, 10.0f,
        -10.0f, 10.0f, 10.0f
        };

        std::vector<mesh_indices_type> indices
        {
        0, 1, 4,
        4, 1, 5,
        6, 2, 3,
        6, 3, 7,
        2, 1, 0,
        3, 2, 0,
        3, 0, 4,
        4, 7, 3,
        5, 1, 2,
        2, 6, 5,
        4, 5, 6,
        6, 7, 4
        };

        mesh->setTriangles(std::move(indices));
        mesh->setFloatAttribute(std::move(verts), "gl_Vertex", 3);
    }

    void Icosahedron::sphere(float radius, int Hsegments, int Wsegments, Mesh_ptr mesh, bool isTriangleStrip) {
        Icosahedron::sphere(radius, Hsegments, Wsegments, mesh.get(), isTriangleStrip);
    }

    void Icosahedron::pipe(float radius, float height, int Hsegments, int Wsegments, Mesh_ptr mesh, bool isTriangleStrip) {
        Icosahedron::pipe(radius, height, Hsegments, Wsegments, mesh.get(), isTriangleStrip);
    }

    void Icosahedron::cube(float radius, int segments, Mesh_ptr mesh, bool isTriangleStrip, bool textureCube) {
        Icosahedron::cube(radius, segments, mesh.get(), isTriangleStrip, textureCube);
    }

    void Icosahedron::cube(Mesh_ptr mesh)
    {
        Icosahedron::cube(mesh.get());
    }



    void Icosahedron::cubedSphere(float radius, int segments, Mesh_ptr mesh, bool isTriangleStrip, bool textureCube) {
        Icosahedron::cubedSphere(radius, segments, mesh.get(), isTriangleStrip, textureCube);
    }

    Mesh makeSphere(float radius, int Hsegments, int WSegment, bool isTriangleStrip) {
        Mesh ret;
        Icosahedron::sphere(radius, Hsegments, WSegment, &ret, isTriangleStrip);
        return ret;
    }

    Mesh makeCubeSphere(float radius, int HSegment, bool isTriangleStrip, bool textureCube) {
        Mesh ret;
        Icosahedron::cubedSphere(radius, HSegment, &ret, isTriangleStrip, textureCube);
        return ret;
    }

    Mesh makePipe(float radius, float height, int Hsegments, int Wsegments, bool isTriangleStrip) {
        Mesh ret;
        Icosahedron::pipe(radius, height, Hsegments, Wsegments, &ret, isTriangleStrip);
        return ret;
    }

    Mesh makeCube(float radius, int segments, bool isTriangleStrip , bool textureCube ) {
        Mesh ret;
        Icosahedron::cube(radius, segments, &ret, isTriangleStrip, textureCube);
        return ret;
    }

}
