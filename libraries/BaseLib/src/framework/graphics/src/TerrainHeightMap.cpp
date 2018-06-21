#include <aex/Math.hpp>
#include <aex/Quaternion.hpp>
#include <aex/TerrainHeightMap.hpp>
#include <aex/Texture.hpp>
#include <aex/QuaternionCamera.hpp>

using namespace std;
namespace aex {

    TerrainQuadNode::TerrainQuadNode() :
    m_children{
        {nullptr, nullptr, nullptr, nullptr}
    }
    {

    }

    TerrainQuadNode::TerrainQuadNode(aex::math::Vector2 a, aex::math::Vector2 b, uint32_t lvl) :
    m_children{
        {nullptr, nullptr, nullptr, nullptr}
    }

    ,
    m_lvl(lvl)
    {
        set_range(a, b, 0.0f);
        m_sizeUni.setName("mapSize");
        m_uvSizeUni.setName("uvSize");
        m_uvOffsetUni.setName("uvOffset");
        m_colorUni.setName("colorMod");
    }

    TerrainQuadNode::TerrainQuadNode(aex::math::Vector2 a, aex::math::Vector2 b, uint32_t lvl, aex::math::Vector2 size, float m_topSize) :
    m_children{
        {nullptr, nullptr, nullptr, nullptr}
    }

    ,
    m_lvl(lvl)
    {
        
        set_range(a, b, size.y);
        m_sizeUni.setName("mapSize");
        m_sizeUni.setValue(size);
        m_uvSizeUni.setName("uvSize");
        m_uvSizeUni.setValue(size.x / m_topSize);
        m_uvOffsetUni.setName("uvOffset");
        m_uvOffsetUni.setValue(a / (m_topSize));
        m_colorUni.setName("colorMod");
        m_colorUni.setValue(1.0f, 1.0f, 1.0f);
    }

    TerrainQuadNode& TerrainQuadNode::operator=(aex::TerrainQuadNode& b)
    {

        for (size_t i = 0; i < 4; ++i)
        {
            m_children[i] = std::move(b.m_children[i]);
        }
        m_box = b.m_box;
        m_aex = std::move(b.m_aex);

        m_lvl = b.m_lvl;
        m_drawThis = b.m_drawThis;
        m_drawChilds = b.m_drawChilds;
        m_hasChilds = b.m_hasChilds;
        m_sizeUni = b.m_sizeUni;
        m_uvOffsetUni = b.m_uvOffsetUni;
        m_colorUni = b.m_colorUni;
        
        
        return *this;
    }

    void TerrainQuadNode::set_range(aex::math::Vector2 a, aex::math::Vector2 b, float height)
    {

        if (!m_aex)
        {
            m_aex.reset(new Aex);
        }

        m_aex->setPosition(aex::math::Vector3(a));
        m_box = AABB(aex::math::Vector3(a.x, 0.0f, a.y), aex::math::Vector3(b.x, height, b.y));
    }

    void TerrainQuadNode::drawThis(AexRenderer* gl)
    {
        const auto& shader = gl->getShader();
        if (shader)
        {
            const auto& program = shader->GetProgram();

            m_sizeUni.setUniform(program);
            m_uvOffsetUni.setUniform(program);
            m_uvSizeUni.setUniform(program);
            m_colorUni.setUniform(program);
        }

        m_aex->draw(gl);
        m_aex->finishdraw(gl);
    }

    void TerrainQuadNode::drawNodes(AexRenderer* gl)
    {
        for (size_t i = 0; i < 4; ++i)
        {
            m_children[i]->draw(gl);
        }
    }

    void TerrainQuadNode::draw(AexRenderer* gl)
    {

        if (!gl->isShadowMapPass())
        {
            if (m_drawThis)
            {
                drawThis(gl);
            } else if (m_hasChilds && m_drawChilds)
            {
                drawNodes(gl);
            }
        } else
        {
            if (m_hasChilds)
            {
                drawNodes(gl);
            } else
            {
                drawThis(gl);
            }
        }


    }

    bool TerrainQuadNode::update(QuaternionCamera* cam, float topSize)
    {
        if (cam->getFrustum().containsAABBbool(m_box) || m_drawMax)
        {

           
            aex::math::Vector2 distVec = m_aex->getCenterPoint().xz() - cam->GetPosition().xz();
            float dist = distVec.chebLength();
            
            if (dist >= topSize / pow(2.0f, (float) m_lvl - 1))
            {
                m_drawThis = true;
                m_drawChilds = false;
            } else
            {
                if (m_hasChilds)
                {
                    m_drawThis = false;
                    m_drawChilds = false;
                    for (size_t i = 0; i < 4; ++i)
                    {
                        
                       bool childs = m_children[i]->update(cam, topSize);
                       m_drawChilds = m_drawChilds || childs;
                       //LOG_DEBUG("m_drawChilds = m_drawChilds || m_children[i]->update(cam, topSize);", m_drawChilds, m_children[i]->update(cam, topSize));
                    }

                } else
                {
                    m_drawThis = true;
                    m_drawChilds = false;
                }
            }

        } else
        {
            m_drawThis = false;
            m_drawChilds = false;
            /*
            if (m_hasChilds)
            {
                for (size_t i = 0; i < 4; ++i)
                {
                   bool childs = m_children[i]->update(cam, topSize);
                   m_drawChilds = m_drawChilds || childs;
                }
            }
            else
            {
                //m_drawThis=true;
                //setColor(1.0,0.0,0.0);
            }*/
        }
        return m_drawThis || m_drawChilds;

    }
    
    void  TerrainQuadNode::setColor(float r,float g,float b)
    {
        m_colorUni.setValue(r,g,b);
    }

    void TerrainQuadNode::setDrawMax(bool drawMax)
    {
        m_drawMax = drawMax;
        if (m_hasChilds)
        {
              for (size_t i = 0; i < 4; ++i)
            {
                m_children[i]->setDrawMax(drawMax);
            }
        }
    }
    

    void TerrainQuadNode::split(uint max_lvl, aex::math::Vector2 size, float topSize)
    {
        aex::math::Vector2 midpoint = m_box.center().xz();
        aex::math::Vector2 halfdist = m_box.halfdist().xz();
        aex::math::Vector2 halfHalfDist = halfdist * 0.5;

        aex::math::Vector2 a = midpoint + aex::math::Vector2(halfHalfDist.x, halfHalfDist.y);
        aex::math::Vector2 b = midpoint + aex::math::Vector2(-halfHalfDist.x, halfHalfDist.y);
        aex::math::Vector2 c = midpoint + aex::math::Vector2(halfHalfDist.x, -halfHalfDist.y);
        aex::math::Vector2 d = midpoint + aex::math::Vector2(-halfHalfDist.x, -halfHalfDist.y);

        std::stringstream str;

        for (uint32_t i = 0; i < m_lvl; ++i)
        {
            str << "    ";
        }
        size.x /= 2.0;
        m_children[0].reset(new TerrainQuadNode(a, halfHalfDist, m_lvl + 1, size, topSize));
        m_children[1].reset(new TerrainQuadNode(b, halfHalfDist, m_lvl + 1, size, topSize));
        m_children[2].reset(new TerrainQuadNode(c, halfHalfDist, m_lvl + 1, size, topSize));
        m_children[3].reset(new TerrainQuadNode(d, halfHalfDist, m_lvl + 1, size, topSize));
        m_hasChilds = true;
        
        if (m_lvl + 1 < max_lvl)
        {
            for (size_t i = 0; i < 4; ++i)
            {
                m_children[i]->split(max_lvl, size, topSize);
            }
        }

    }

    void TerrainQuadNode::setBox(AABB box)
    {
        m_box = box;
    }

    AABB TerrainQuadNode::getBox() const
    {
        return m_box;
    }

    void TerrainQuadNode::setSize(aex::math::Vector2 size)
    {
        m_sizeUni.setValue(size);
    }

    aex::math::Vector2 TerrainQuadNode::getSize() const
    {
        return m_sizeUni.getValue();
    }

    TerrainQuadNode::~TerrainQuadNode()
    {
        //        if (m_hasChilds) {
        //            for (size_t i = 0; i < 4; ++i) {
        //                delete m_childs[i];
        //            }
        //        }
    }

    QuadTreeTerrain::QuadTreeTerrain() :
    m_data(nullptr, delete_array)
    {

    }

    QuadTreeTerrain::QuadTreeTerrain(QuaternionCamera* m_quatCamera, aex::math::Vector2 topsize, int gridSize, int depth) :
    m_quatCamera(m_quatCamera),
    m_topMaterial(new Material()),
    m_gridSize(gridSize),
    m_depth(depth),
    m_topSize(topsize),
    m_heightMapUniform("heightMap"),
    m_heightNormalMapUniform("heightNormalMap"),
    m_heightOffsetUniform(0.0f, "heightMapOffset"),
    m_data(nullptr, delete_array)
    {
        init();
    }

    float QuadTreeTerrain::getHeightSize() const
    {
        return m_topSize.y;
    }

    Material* QuadTreeTerrain::getMaterial() const
    {
        return m_topMaterial.get();
    }
    
    float QuadTreeTerrain::getEdgeSize() const
    {
        return m_topSize.x;
    }

    QuadTreeTerrain::~QuadTreeTerrain()
    {
        /*if(m_data)
        {    
            delete[] m_data;
        }*/
    }

    QuadTreeTerrain& QuadTreeTerrain::operator=(aex::QuadTreeTerrain& b)
    {
        m_quatCamera = b.m_quatCamera;
        m_topNode = std::move(b.m_topNode);
        m_topMaterial = std::move(b.m_topMaterial);
        m_plane = std::move(b.m_plane);
        m_data = std::move(b.m_data);
        std::swap(m_rawHeightData, b.m_rawHeightData);
        m_gridSize = b.m_gridSize;
        m_depth = b.m_depth;
        m_topSize = b.m_topSize;
        m_LODFactor = b.m_LODFactor;
        m_heightMapUniform = b.m_heightMapUniform;
        m_heightNormalMapUniform = b.m_heightNormalMapUniform;
        m_heightOffsetUniform = b.m_heightOffsetUniform;
        return *this;
    }

    QuadTreeTerrain& QuadTreeTerrain::operator=(aex::QuadTreeTerrain&& b)
    {
        m_quatCamera = b.m_quatCamera;
        m_topNode = std::move(b.m_topNode);
        m_topMaterial = std::move(b.m_topMaterial);
        m_plane = std::move(b.m_plane);
        m_data = std::move(b.m_data);
        std::swap(m_rawHeightData, b.m_rawHeightData);
        m_gridSize = b.m_gridSize;
        m_depth = b.m_depth;
        m_topSize = b.m_topSize;
        m_LODFactor = b.m_LODFactor;
        m_heightMapUniform = b.m_heightMapUniform;
        m_heightNormalMapUniform = b.m_heightNormalMapUniform;
        m_heightOffsetUniform = b.m_heightOffsetUniform;
        return *this;
    }

    void QuadTreeTerrain::init()
    {

        if (!m_plane)
        {
            m_plane.reset(new Mesh);
        }
        generatePlane(m_plane.get(), m_gridSize, true);
        m_topNode.reset(new TerrainQuadNode(aex::math::Vector2(0.0, 0.0) / 2.0, aex::math::Vector2(m_topSize.x, m_topSize.x) / 2.0, 0));
        m_topNode->split(m_depth, m_topSize, m_topSize.x);
    }

    void QuadTreeTerrain::update()
    {
        m_topNode->update(m_quatCamera, m_topSize.x);
    }

    void QuadTreeTerrain::draw(AexRenderer* gl)
    {
        m_plane->draw(gl);
        if (m_topMaterial)
        {
            m_topMaterial->draw(gl);
        }
        m_heightMap->bind();
        
        if(m_heightNormalMap)
        {
            m_heightNormalMap->bind();
        }
        
        const auto& shader = gl->getShader();
        if (shader)
        {
            const auto& program = shader->GetProgram();

            m_heightMapUniform.setUniform(program);
            m_heightOffsetUniform.setUniform(program);

        }
        m_topNode->draw(gl);
    }

    void QuadTreeTerrain::finishdraw(AexRenderer* gl)
    {

        if (m_topMaterial)
        {
            m_topMaterial->finishdraw(gl);
        }
        m_heightMap->unbind();
        m_plane->finishdraw(gl);

    }

    float QuadTreeTerrain::getHeightAtCoordinates(aex::math::Vector2 coords)
    {
        aex::math::Vector2 normalcoords = coords / m_topSize.x;
//        float ret = 0.0f;
//        if (m_heightMap)
//        {
//            if (!m_data)
//            {
//                m_data = make_array_pointer(static_cast<uint*> (m_heightMap->getData()));
//            }
//
//            int w = m_heightMap->getSizeW();
//            int h = m_heightMap->getSizeH();
//
//
//            aex::math::Vector2 texCoord = aex::math::Vector2((1.0 - (normalcoords.y + 0.5)) * float(m_heightMap->getSizeW()), ((normalcoords.x + 0.5)) * float(m_heightMap->getSizeH()));
//
//            if ((normalcoords.x > 1.0 || normalcoords.x < 0.0) || (normalcoords.y > 1.0 || normalcoords.y < 0.0))
//            {
//                return 0.0;
//            }
//
//            size_t index = (int(texCoord.x) * w) + int(texCoord.y * h);
//            uint value = m_data.get()[index];
//            
//            std::array<uint8_t, 4> rgba = Vector3::intIDtoVectorRGBA(value);
//            //LOG_DEBUG("std::array<uint8_t, 4> rgba: ", (int)rgba[0] );
//            //LOG_DEBUG("uint value = m_data.get()[index]; ", normalcoords, " ", value, " ", int(rgba[0]), " ", int(rgba[1]), " ", int(rgba[2]), " ", int(rgba[3]), " ");
//
//            ret = (float(rgba[0]) / 255.0) * m_topSize.y;
//        }
//        return ret + m_heightOffsetUniform.getValue();
        
        if (m_rawHeightData.size() == 0)
        {
            getRawHeightData();
        }
      
            int w = m_heightMap->getSizeW();
     
            aex::math::Vector2 texCoord = aex::math::Vector2((1.0 - (normalcoords.y + 0.5)) * float(m_heightMap->getSizeW()), ((normalcoords.x + 0.5)) * float(m_heightMap->getSizeH()));
            texCoord.x =aex::math::Math::clamp_value_float(0.0, float(m_heightMap->getSizeW()), texCoord.x);
            texCoord.y =aex::math::Math::clamp_value_float(0.0, float(m_heightMap->getSizeH()), texCoord.y);      
            size_t index = (int(texCoord.x) * w) + (int(texCoord.y));
            //LOG_DEBUG("aex::math::Vector2 texCoord", texCoord, " ",index, " ", m_rawHeightData.size()," ", w ," ",h);
            
            
            return m_rawHeightData[index];
            
    }

    float* QuadTreeTerrain::getRawHeightData()
    {
        if (m_rawHeightData.size() == 0)
        {
            if (m_heightMap)
            {
                if (!m_data)
                {
                    m_data = make_array_pointer(static_cast<uint*> (m_heightMap->getData()));
                }

                uint w = m_heightMap->getSizeW();
                uint h = m_heightMap->getSizeH();

                if (m_rawHeightData.size() != (w * h))
                {
                    m_rawHeightData.resize(w * h);
                }
                float offset = m_heightOffsetUniform.getValue();
                for (uint x = 0; x < w; ++x)
                {
                    for (uint y = 0; y < h; ++y)
                    {
                        size_t index = (x * w) + y;
                        uint32_t value = m_data.get()[index];

                        std::array<uint8_t, 4> rgba = Vector3::intIDtoVectorRGBA(value);
                        m_rawHeightData[index] = ((float(rgba[0]) / 255.0f) * m_topSize.y) + offset;
                    }
                }
            } else
            {
                return nullptr;
            }
        }
        LOG_DEBUG("m_rawHeightDataSize: ", m_rawHeightData.size(), " ", m_heightMap->getSizeW() * m_heightMap->getSizeH());
        std::stringstream prt;
        for (int i = 0; i < 20; ++i)
        {
            prt << m_rawHeightData[i + 200] << ",";
        }
        LOG_DEBUG("std::stringstream prt: ", prt.str());
        return &m_rawHeightData[0];
    }

    int QuadTreeTerrain::getTextureSizeW() const
    {
        if (m_heightMap)
        {
            return m_heightMap->getSizeW();
        } else
        {
            LOG_ERR("No texture is set for QuadTreeTerrain!");
            return 0;
        }
    }

    int QuadTreeTerrain::getTextureSizeH() const
    {
        if (m_heightMap)
        {
            return m_heightMap->getSizeH();
        } else
        {
            LOG_ERR("No texture is set for QuadTreeTerrain!");
            return 0;
        }
    }
    
    std::string QuadTreeTerrain::getTextureName() const
    {
        if (m_heightMap)
        {
            return m_heightMap->getName();
        } else
        {
            LOG_ERR("No texture is set for QuadTreeTerrain!");
            return "none";
        }
    }
    
    void QuadTreeTerrain::setDrawMax(bool drawmax)
    {
        m_topNode->setDrawMax(drawmax);
    }

    void QuadTreeTerrain::setHeightOffset(float heightOffset)
    {
        m_heightOffsetUniform.setValue(heightOffset);
    }

    float QuadTreeTerrain::getHeightOffset() const
    {
        return m_heightOffsetUniform.getValue();
    }

    uint* QuadTreeTerrain::getData()
    {
        if (m_heightMap)
        {
            if (!m_data)
            {
                m_data = make_array_pointer(static_cast<uint*> (m_heightMap->getData()));
            }

            return m_data.get();
        }
        return nullptr;
    }

    void QuadTreeTerrain::setHeightMap(Texture* heightMap)
    {
        m_heightMap = heightMap;
        m_heightMap->bind();
        m_heightMap->unbind();
        m_heightMapUniform.setValue(m_heightMap);
    }
    
    void QuadTreeTerrain::setHeightNormalMap(Texture* heightMap)
    {
        m_heightNormalMap = heightMap;
        m_heightNormalMap->bind();
        m_heightNormalMap->unbind();
        m_heightNormalMapUniform.setValue(m_heightNormalMap);
    }

    void QuadTreeTerrain::needsUpdate()
    {

    }

    void QuadTreeTerrain::setQuatCamera(QuaternionCamera* quatCamera)
    {
        m_quatCamera = quatCamera;
    }

    QuaternionCamera* QuadTreeTerrain::getQuatCamera() const
    {
        return m_quatCamera;
    }

    void QuadTreeTerrain::setGridSize(int topGridSize)
    {
        m_gridSize = topGridSize;
    }

    int QuadTreeTerrain::getGridSize() const
    {
        return m_gridSize;
    }

    TerrainHeightMap::TerrainHeightMap()
    {

    }

    TerrainHeightMap::~TerrainHeightMap()
    {
        if (m_size != 0)
        {
            for (int j = 0; j < m_size; j++)
            {
                delete [] m_heightMapFloat[j];
            }
            delete [] m_heightMapFloat;
        }
    }

    void
    TerrainHeightMap::setSize(int size)
    {

        m_heightMapFloat = new float*[size];

        for (int j = 0; j < m_size; j++)
        {
            m_heightMapFloat[j] = new float[m_size]();
        }

    }

    int
    TerrainHeightMap::getSize() const
    {
        return m_size;
    }

    void
    TerrainHeightMap::normalizeMap()
    {
        float max = 0;
        float min = 0;

        for (int i = 0; i < m_size; i++)
        {

            for (int j = 0; j < m_size; j++)
            {
                if (m_heightMap[i][j] > max)
                {
                    max = m_heightMap[i][j];
                }

                if (m_heightMap[i][j] < min)
                {
                    min = m_heightMap[i][j];
                }
            }

        }

        for (int i = 0; i < m_size; i++)
        {
            for (int j = 0; j < m_size; j++)
            {
                m_heightMapFloat[i][j] = (m_heightMap[i][j] - min) / (max - min);
            }
        }
    }

    void generatePlane(Mesh* mesh, int segments, bool triangleStrip, bool addSkirt)
    {
        std::vector<mesh_vectors_type> temp_Vertices;
        std::vector<mesh_vectors_type> temp_Normals;
        std::vector<mesh_vectors_type> temp_TextCoords;
        std::vector<mesh_indices_type> temp_Indices;

        temp_Vertices.resize(segments * segments * 3);
        temp_Normals.resize(segments * segments * 3);
        temp_TextCoords.resize(segments * segments * 2);



        std::vector<mesh_vectors_type>::iterator vi = temp_Vertices.begin();
        std::vector<mesh_vectors_type>::iterator ni = temp_Normals.begin();
        std::vector<mesh_vectors_type>::iterator uvi = temp_TextCoords.begin();

        float cellsize = 1.0 / float (segments - 1);

        for (int x = 0; x < segments; ++x)
        {
            for (int y = 0; y < segments; ++y)
            {
                *vi++ = float(x) * cellsize - 0.5f;
                *vi++ = 0.0;
                *vi++ = float(y) * cellsize - 0.5f;

                *ni++ = 0.0;
                *ni++ = 1.0;
                *ni++ = 0.0;

                *uvi++ = x * cellsize - 0.5f;
                *uvi++ = y * cellsize - 0.5f;
            }
        }


        if (triangleStrip)
        {
            int numStripsRequired = segments - 1;
            int numDegensRequired = 2 * (numStripsRequired - 1);
            int verticesPerStrip = 2 * segments;
            int size = (verticesPerStrip * numStripsRequired) + numDegensRequired + 1;
            int i = 0;
            temp_Indices.resize(size);
            std::vector<mesh_indices_type>::iterator indi = temp_Indices.begin();

            for (int a = 0; a < segments - 1; a++)
            {

                if (a > 0)
                {
                    // Degenerate begin: repeat first vertex
                    *indi++ = (mesh_indices_type) (((a + 1) * segments));
                    i++;
                }

                for (int b = 0; b < segments; b++)
                {
                    // One part of the strip

                    *indi++ = (mesh_indices_type) (((a + 1) * segments) + b);
                    *indi++ = (mesh_indices_type) ((a * segments) + b);
                    i += 2;
                }

                if (a < segments)
                {
                    // Degenerate end: repeat last vertex
                    *indi++ = (mesh_indices_type) ((a * segments) + (segments - 1));
                    i++;
                }

            }



            //            for (int y = 0; y < segments; y++) {
            //                if (y > 0) {
            //                    // Degenerate begin: repeat first vertex
            //                    *indi++ = (mesh_indices_type) (y * segments);
            //                    a++;
            //                }
            //
            //                for (int x = 0; x < segments; x++) {
            //                    // One part of the strip
            //                    *indi++ = (mesh_indices_type) (((y + 1) * segments) + x);
            //                    *indi++ = (mesh_indices_type) ((y * segments) + x);
            //                    a += 2;
            //                }
            //
            //                if (y < segments - 1) {
            //                    // Degenerate end: repeat last vertex
            //                    *indi++ = (mesh_indices_type) ((y * segments) + (segments - 1));
            //                    a++;
            //                }
            //            }

            mesh->setDrawMode(GL_TRIANGLE_STRIP);
        } else
        {

            int size = (segments - 1) * (segments - 1) * 2 * 3;
            temp_Indices.resize(size);
            std::vector<mesh_indices_type>::iterator indi = temp_Indices.begin();

            for (int y = 0; y < segments - 1; ++y)
            {
                for (int x = 0; x < segments; ++x)
                {
                    if (y < segments - 1 && x < segments - 1)
                    {

                        int currentVertex = (y * (segments)) + x;

                        *indi++ = (mesh_indices_type) currentVertex;
                        *indi++ = (mesh_indices_type) currentVertex + 1;
                        *indi++ = currentVertex + segments;


                        *indi++ = currentVertex + 1;
                        *indi++ = currentVertex + segments + 1;
                        *indi++ = currentVertex + segments;
                    }
                }
            }
        }


        mesh->setFloatAttribute(std::move(temp_Vertices), "gl_Vertex", 3);
        mesh->setFloatAttribute(std::move(temp_Normals), "gl_Normal", 3);
        mesh->setFloatAttribute(std::move(temp_TextCoords), "gl_TexCoord", 2);
        mesh->setTriangles(std::move(temp_Indices));

    }

    void
    TerrainHeightMap::generateVboTriangles(MeshShrd_Ptr mesh)
    {
        vector<mesh_vectors_type> vertices;
        vector<mesh_indices_type> faces;
        vector<mesh_vectors_type> uV;

        int a = 0;

        for (int i = 0; i < m_size; i++)
        {

            for (int j = 0; j < m_size; j++)
            {

                vertices.push_back((float) (i * 4));
                vertices.push_back((float) (j * 4));
                vertices.push_back((m_heightMapFloat[i][j]) * 20);


                uV.push_back(float(i) / float(m_size));
                uV.push_back(float(j) / float(m_size));

                if (i < m_size - 1 && j < m_size - 1)
                {
                    int currentVertex = (i * (m_size)) + j;
                    faces.push_back(currentVertex + m_size);
                    faces.push_back(currentVertex + 1);
                    faces.push_back(currentVertex);

                    faces.push_back(currentVertex + m_size);
                    faces.push_back(currentVertex + m_size + 1);
                    faces.push_back(currentVertex + 1);
                }// if size

                a++;
            } //for j

        }//for 1

        mesh->setVertices(vertices, 3);
        mesh->setTriangles(faces);
        mesh->setCoords(uV, 2);

    }

    void
    TerrainHeightMap::generateSkydomeVbo(MeshShrd_Ptr mesh)
    {
        vector<mesh_vectors_type> vertices;
        vector<mesh_indices_type> faces;
        vector<mesh_vectors_type> uV;

        int height = 10;
        int sides = 6;
        float sideAngle = 360 / sides;
        float heightAngle = 90 / height;

        for (int i = 0; i < height; i++)
        {


            for (int j = 0; j < sides; j++)
            {


                float hAngle = heightAngle * (float) i;
                float sAngle = sideAngle * (float) j;

                if (sAngle > 180)
                {
                    hAngle = -hAngle;
                }

                aex::math::Vector3 vec(0.0, 0.0, 1.0f * (float) i);
                Matrix mat;
                // if(hangle)
                mat.identity();
                /*Matrix mat=aex::math::identity();
                mat *=aex::math::rotate(hAngle,0.0f,1.0f,0.0f);
                mat *=aex::math::rotate(sAngle,1.0f,0.0f,0.0f);*/

                // vec=mat.transformVector(vec);

                //vec = quat.rotate(vec);
                vec = mat.transformVector(vec);

                if (i == 0 && j == 0)
                {
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    uV.push_back(0.0f);
                    uV.push_back(0.5f);
                } else
                {
                    vertices.push_back(vec.x);
                    vertices.push_back(vec.y);
                    vertices.push_back(vec.z);


                    uV.push_back(float(j) / float(m_size));
                    uV.push_back(float(i) / float(m_size));
                }

                if (i < 1)
                {
                    faces.push_back(j + 1);
                    faces.push_back(j);
                    faces.push_back(0);
                } else if (i < height - 1 && j < sides - 1)
                {
                    int currentVertex = (i * (sides)) + j;

                    faces.push_back(currentVertex);
                    faces.push_back(currentVertex + 1);
                    faces.push_back(currentVertex + sides);


                    faces.push_back(currentVertex + 1);
                    faces.push_back(currentVertex + sides + 1);
                    faces.push_back(currentVertex + sides);
                }// if size

            } //for j

        }//for i

        mesh->setVertices(vertices, 3);
        mesh->setTriangles(faces);
        mesh->setCoords(uV, 2);

    }

}
