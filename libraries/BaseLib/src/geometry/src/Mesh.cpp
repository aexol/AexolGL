//
//  Mesh.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 03.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include <aex/Mesh.hpp>
#include <aex/Indexer.hpp>
#include <aex/Math.hpp>
#include <aex/FileManager.hpp>
#include <aex/Icosahedron.hpp>
#include <aex/Uniforms.hpp>
#include <aex/AABB.hpp>
#include <aex/Logger.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/VertexData.hpp>
#include <aex/AexRenderer.hpp>
#include <aex/ShaderDrw.hpp>
#include <string>
#include <fstream>
#include <list>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cctype>

#ifdef DEBUG
#include <cfloat>
#endif

#define POSITION_DATA_SIZE 3
#define NORMAL_DATA_SIZE 3
#define TEXTURE_COORDINATE_DATA_SIZE 2
#define BYTES_PER_FLOAT 4

namespace aex {

    template<typename V> class Indexer;
    class Icosahedron;
    template<typename T> std::vector<T> getVectorFromFile(const aex::string&, size_t&);

    std::atomic<uint32_t> Mesh::m_namecounter(0);

    Mesh::Mesh() :
    m_name("Mesh" + aex::to_string(m_namecounter++)),
    m_bCoords(false),
    m_bNormals(false),
    m_compiled(false),
    m_usePacked(false),
    m_isBufferBinded(false),
    //     m_usePacked(false),
    m_dim(3),
    m_scaledUV(1.0f),
    m_spacing(0),
    m_coordsSize(2) {
    }

    Mesh::Mesh(const aex::string& name, bool usePacked, bool bCoords, bool bNormals) :
    m_name(name),
    m_bCoords(bCoords),
    m_bNormals(bNormals),
    m_compiled(false),
    m_usePacked(usePacked),
    m_isBufferBinded(false),
    m_dim(3),
    m_scaledUV(1.0f),
    m_spacing(0),
    m_coordsSize(2) {
    }

    Mesh::~Mesh() {
        //Unload();
    };

    bool
    Mesh::Load(const aex::string& name, uint32_t numVertices, bool usePacked, bool bCoords, bool bNormals) {
        m_dim = 3;
        m_bCoords = bCoords;
        m_usePacked = usePacked;
        m_bNormals = bNormals;
        m_spacing = 0;
        m_scaledUV = 1.0f;
        m_isBufferBinded = false;
        m_compiled = false;

        if (name.empty()) {
            m_name = "Mesh" + aex::to_string(m_namecounter++);
        } else {
            m_name = name;
        }

        Unload();
        addVertexBuffer("gl_Vertex");

        //addVertexBuffer("nextFrame", "gl_nextFrame");
        if (bCoords) {
            addVertexBuffer("gl_TexCoord");
        }

        if (bNormals) {
            addVertexBuffer("gl_Normal");
        }

        addIndexBuffer("triangles");
        addIndexBuffer("lines");

        return true;
    }

    void
    Mesh::LoadFromJSONFile(const aex::string& fName) //Redo zeby bylo bardziej cpp a nie c uzywajac istream, ale to kiedy indziej
    {
        std::ifstream fileHandle;
        FileManager fManager;

        fileHandle.open(fName, std::ios::in);
        //define file stream object, and open the file

        aex::string buff;
        buff.reserve(fManager.file_size(fName));

        buff.assign((std::istreambuf_iterator<char>(fileHandle)),
                std::istreambuf_iterator<char>());

        fileHandle.close();

        size_t len = buff.length();

        for (size_t i = 0; i < len; i++) {
            if (buff.substr(i, 8).compare("vertices") == 0) {
                size_t entry_pos = i + 8;

                while (buff[entry_pos] != '[') {
                    entry_pos++;

                    if (entry_pos == len) {
                        break;
                    }
                }

                entry_pos++;

                std::vector<float> vertices = getVectorFromFile<float>(buff, entry_pos);

                setVertices(std::move(vertices));

                i = entry_pos;

                for (; i < len; i++) {
                    if (buff.substr(i, 7).compare("normals") == 0) {
                        entry_pos = i + 7;

                        while (buff[entry_pos] != '[') {
                            entry_pos++;

                            if (entry_pos == len) {
                                break;
                            }
                        }

                        entry_pos++;

                        std::vector<float> normals = getVectorFromFile<float>(buff, entry_pos);

                        setNormals(std::move(normals));

                        i = entry_pos;
                    }

                    if (buff.substr(i, 6).compare("coords") == 0) {
                        entry_pos = i + 6;

                        while (buff[entry_pos] != '[') {
                            entry_pos++;

                            if (entry_pos == len) {
                                break;
                            }
                        }

                        entry_pos++;

                        std::vector<float> coords = getVectorFromFile<float>(buff, entry_pos);

                        setCoords(std::move(coords));

                        i = entry_pos;
                    }

                    if (buff.substr(i, 9).compare("triangles") == 0) {
                        entry_pos = i + 9;

                        while (buff[entry_pos] != '[') {
                            entry_pos++;

                            if (entry_pos == len) {
                                break;
                            }
                        }

                        entry_pos++;

                        std::vector<indices_type> triangles = getVectorFromFile<indices_type>(buff, entry_pos);

                        setTriangles(std::move(triangles));

                        i = entry_pos;
                    }
                }
            }
        }
    }

    void
    Mesh::Unload() {
        m_vertexBuffers.clear();
        m_indexBuffers.clear();

    }

    VertexData*
    Mesh::addVertexBuffer(const aex::string& name) {
        size_t components = 3;

        if (name.compare("gl_TexCoord") == 0) {
            components = 2;
        }

        return addVertexBuffer(name, components);
    }

    VertexData*
    Mesh::addVertexBuffer(const aex::string& name, size_t components) {
        m_vertexData.emplace(name, std::make_pair(vertex_array_type{}, components));
        return m_vertexBuffers[name].get();
    }

    IndexData*
    Mesh::addIndexBuffer(const aex::string& name) {
        return m_indexBuffers[name].get();
    }

    void
    Mesh::setIndexBuffer(const aex::string& name, const IndexDataRef& ref) {
        m_indexBuffers[name] = ref;
    }

    const IndexData*
    Mesh::getTrianglesHandle() const {
        return m_indexBuffers.at("triangles").get();
    }

    IndexData*
    Mesh::getTrianglesHandle() {
        compile();
        return m_indexBuffers["triangles"].get();
    }

    const Mesh::index_array_type&
    Mesh::getTriangles() const {
        return m_indexData.at("triangles");
    }

    Mesh::index_array_type&
    Mesh::getTriangles() {
        try {
            return m_indexData.at("triangles");
        } catch (std::out_of_range& e) {
            addIndexBuffer("triangles");
        }

        return m_indexData["triangles"];
    }

    const IndexData*
    Mesh::getLinesHandle() const {
        return m_indexBuffers.at("lines").get();
    }

    IndexData*
    Mesh::getLinesHandle() {
        return m_indexBuffers["lines"].get();

    }

    const Mesh::index_array_type&
    Mesh::getLines() const {
        return m_indexData.at("lines");
    }

    Mesh::index_array_type&
    Mesh::getLines() {
        return m_indexData["lines"];
    }

    void
    Mesh::delete_buffer(const aex::string& name) {
        m_vertexBuffers.erase(name);
    }

    void
    Mesh::delete_buffers() {
        m_vertexBuffers.clear();
        m_indexBuffers.clear();
    }

    void Mesh::compile() {
        // delete_buffers();
        if (m_usePacked) {
            /*if(m_vertexBufferPacked->empty()){
                    //vertexBufferPacked = new VertexBufferPacked(bNormals, bCoords);
                    //constructPacked();
            }*/
            //vertexBufferPacked.setValues(packedData);
        } else {
            for (auto& attrib : m_vertexBuffers) {
                auto attribIter = m_vertexData.find(attrib.first);

                if (attrib.second && attribIter->second.first.empty()) {
                    attrib.second->enable();
                } else {
                    if (attribIter != m_vertexData.end()) {
                        attrib.second.reset(VertexData::createVertexData(attribIter->second.first, attrib.first, attribIter->second.second));
                        attrib.second->enable();
                    }
                }
            }
        }

        for (auto& attrib : m_indexBuffers) {
            auto attribIter = m_indexData.find(attrib.first);

            if (attrib.second && attribIter->second.empty()) {
                attrib.second->enable();
            } else {
                if (attribIter != m_indexData.end()) {
                    if (m_drawMode != GL_TRIANGLE_STRIP) {
                        attrib.second.reset(new IndexData(IndexData::createTriangles(attribIter->second)));
                    } else {
                        attrib.second.reset(new IndexData(IndexData::createTraingleStrip(attribIter->second)));
                    }

                    attrib.second->enable();
                }
            }
        }

        m_compiled = true;

    }

    void
    Mesh::transform(aex::math::Matrix matV, bool r) {
        vertex_array_type* vertices = nullptr;
        vertex_array_type* normals = nullptr;
        vertex_array_type nVertex;
        auto vertIter = m_vertexData.find("gl_Vertex");

        if (vertIter != m_vertexData.end()) {
            vertices = &vertIter->second.first;
        }

        auto normIter = m_vertexData.find("gl_Normal");

        if (normIter != m_vertexData.end()) {
            normals = &normIter->second.first;
        }

        if (!vertices) {
            return;
        }

        size_t verticesLen = vertices->size();


        nVertex.resize(verticesLen);
        for (std::size_t i = 0; i < verticesLen; i += m_dim) {
            aex::math::Vector3 vec = matV.transformPoint(getVertexV(i));
            nVertex[i] = vec.x;
            nVertex[i + 1] = vec.y;
            nVertex[i + 2] = vec.z;
        }

        this->setVertices(std::move(nVertex), 3);

        if (normals && !normals->empty() && !r) {
            size_t normalsLen = normals->size();
            aex::math::Matrix invTrans = matV.inverse().transpose();

            for (size_t i = 0; i < normalsLen; i += m_dim) {
                aex::math::Vector3 v((*normals)[i], (*normals)[i + 1], (*normals)[i + 1]);
                float* v2 = invTrans.transformVector(v).unit().toArray();

                for (int j = 0; j < 3; j++) {
                    (*normals)[i + j] = v2[j];
                }
            }
        }

        if (m_usePacked) {
            //constructPacked();
        }

        compile();
    }

    //Transforms

    void
    Mesh::_transform(aex::math::Matrix matV, bool r) {
        std::size_t size = 0;
        vertex_array_type* normals = nullptr;
        vertex_array_type nVertex;
        auto vertIter = m_vertexData.find("gl_Vertex");

        if (vertIter != m_vertexData.end()) {
            size = vertIter->second.first.size();
        }

        auto normIter = m_vertexData.find("gl_Normal");

        if (normIter != m_vertexData.end()) {
            normals = &normIter->second.first;
        }

        nVertex.resize(size);
        for (std::size_t i = 0; i < size; i += m_dim) {
            aex::math::Vector3 vec = matV.transformPoint(aex::math::Vector3(vertIter->second.first[i], vertIter->second.first[i + 1], vertIter->second.first[i + 2]));
            nVertex[i] = vec.x;
            nVertex[i + 1] = vec.y;
            nVertex[i + 2] = vec.z;
        }

        this->setVertices(std::move(nVertex), 3);

        if (!normals->empty() && !r) {
            aex::math::Matrix invTrans = matV.inverse().transpose();

            for (size_t i = 0; i < normals->size(); i += m_dim) {
                aex::math::Vector3 v;
                v.x = (*normals)[i];
                v.y = (*normals)[i + 1];
                v.z = (*normals)[i + 2];
                float* v2 = invTrans.transformVector(v).unit().toArray();

                for (int j = 0; j < 3; j++) {
                    (*normals)[i + j] = v2[j];
                }
            }
        }

        if (m_usePacked) {
            //constructPacked();
        }

        compile(); //Te transformacje rozpirza atrybut jezeli bedzie z parametrem clearAfterUpload ^^ poprawie pozniej
    }

    void
    Mesh::move(float x, float y, float z) {
        /*if(bSphere != null){
                bSphere.move(x, y, z);
        }*/
        return transform(aex::math::translate(x, y, z), true);
    }

    void
    Mesh::rotate(float x, float y, float z) {
        transform(aex::math::rotate(x, 1.0f, 0.0f, 0.0f), true);
        transform(aex::math::rotate(y, 0.0f, 1.0f, 0.0f), true);
        transform(aex::math::rotate(z, 0.0f, 0.0f, 1.0f), true);
    }

    void
    Mesh::scale(float x, float y, float z) {
        /*if(bSphere != null && ( x == y && y == z)){
                bSphere.scale(x);
        }*/
        return transform(aex::math::scale(x, y, z), true);
    }

    void
    Mesh::computeNormals() {
        vertex_array_type* normals = NULL;
        vertex_array_type* vertices = NULL;
        auto vertIter = m_vertexData.find("gl_Vertex");

        if (vertIter != m_vertexData.end()) {
            vertices = &vertIter->second.first;
        }

        auto normIter = m_vertexData.find("gl_Normals");

        if (normIter != m_vertexData.end()) {
            normals = &normIter->second.first;
        }

        if (!normals) {
            addVertexBuffer("gl_Normals");
            normIter = m_vertexData.find("gl_Normals");
            normals = &normIter->second.first;
            normals->resize(vertices->size());
            std::fill(normals->begin(), normals->end(), 0.0f);
        }

        index_array_type& triangles = m_indexData["triangles"];

        for (size_t i = 0; i < triangles.size(); i += 3) {
            aex::math::Vector3 a;
            aex::math::Vector3 b;
            aex::math::Vector3 c;

            a.x = (*vertices)[triangles[i] * m_dim];
            a.y = (*vertices)[triangles[i] * m_dim + 1];
            a.z = (*vertices)[triangles[i] * m_dim + 2];

            b.x = (*vertices)[triangles[i + 1] * m_dim];
            b.y = (*vertices)[triangles[i + 1] * m_dim + 1];
            b.z = (*vertices)[triangles[i + 1] * m_dim + 2];

            c.x = (*vertices)[triangles[i + 2] * m_dim];
            c.y = (*vertices)[triangles[i + 2] * m_dim + 1];
            c.z = (*vertices)[triangles[i + 2] * m_dim + 2];

            float* normal = ((b - a) * (c - a)).unit().toArray();

            (*normals)[triangles[i] * m_dim] += normal[0];
            (*normals)[triangles[i] * m_dim + 1] += normal[1];
            (*normals)[triangles[i] * m_dim + 2] += normal[2];

            (*normals)[triangles[i + 1] * m_dim] += normal[0];
            (*normals)[triangles[i + 1] * m_dim + 1] += normal[1];
            (*normals)[triangles[i + 1] * m_dim + 2] += normal[2];

            (*normals)[triangles[i + 2] * m_dim] += normal[0];
            (*normals)[triangles[i + 2] * m_dim + 1] += normal[1];
            (*normals)[triangles[i + 2] * m_dim + 2] += normal[2];
        }

        for (size_t i = 0; i < vertices->size(); i += 3) {
            aex::math::Vector3 v((*normals)[i], (*normals)[i + 1], (*normals)[i + 2]);

            v = v.unit();

            (*normals)[i] = v.x;
            (*normals)[i + 1] = v.y;
            (*normals)[i + 2] = v.z;
        }

        setNormals(*normals, 3);
    }

    void
    Mesh::computeWireFrame(bool forceCompute) {
        if (forceCompute || !m_wireframeDone) {
            index_array_type& triangles = m_indexData["triangles"];

            if (!triangles.empty()) {
                Indexer<Line> indexer(triangles.size() + triangles.size() / 3);

                for (size_t i = 0; i < triangles.size(); i += 3) {
                    for (int j = 0; j < 3; j++) {
                        Line line;
                        line.line[0] = aex::math::Math::min(triangles[i + j], triangles[i + (j + 1) % 3]);
                        line.line[1] = aex::math::Math::max(triangles[i + j], triangles[i + (j + 1) % 3]);
                        indexer.add(line);
                    }
                }

                addIndexBuffer("lines");
                compile();
                m_wireframeDone = true;
            }
        }
    }

    void
    Mesh::computeWireFrame() {
        return computeWireFrame(false);
    }

    AABB
    Mesh::getAABB() {
        AABBBuilder aabbb;
        if(m_aabb.halfdist() == aex::math::Vector3::Zero())
        {
            auto vertIter = m_vertexData.find("gl_Vertex");

            if (vertIter != m_vertexData.end()) {
                auto& vertices = vertIter->second.first;

                for (size_t i = 0; i < vertices.size(); i += m_dim) {
                    Vector3 v = aex::math::fromArray(vertices, i, i + m_dim);
                    aabbb.m_min = aex::math::min(aabbb.m_min, v);
                    aabbb.m_max = aex::math::max(aabbb.m_max, v);

                }
            }
            
            m_aabb = AABB(aabbb);
        }
      
        return m_aabb;
    }

    void
    Mesh::combine(const Mesh* mesh) //This stuff is not gonna work, but well, magic word -> TODO
    {
        const vertex_array_type* verticesAttrib = NULL;
        const vertex_array_type* normalsAttrib = NULL;
        const vertex_array_type* coordsAttrib = NULL;
        const index_array_type* trianglesAttrib = NULL;
        vertex_array_type* m_verticesAttrib = NULL;
        vertex_array_type* m_normalsAttrib = NULL;
        vertex_array_type* m_coordsAttrib = NULL;
        index_array_type* m_trianglesAttrib = NULL;

        auto iterVertexData = m_vertexData.find("gl_Vertex");

        if (iterVertexData != m_vertexData.end()) {
            m_verticesAttrib = &iterVertexData->second.first;
        }

        iterVertexData = m_vertexData.find("gl_TexCoord");

        if (iterVertexData != m_vertexData.end()) {
            m_coordsAttrib = &iterVertexData->second.first;
        }

        iterVertexData = m_vertexData.find("gl_Normal");

        if (iterVertexData != m_vertexData.end()) {
            m_normalsAttrib = &iterVertexData->second.first;
        }

        auto iterIndexData = m_indexData.find("triangles");

        if (iterIndexData != m_indexData.end()) {
            m_trianglesAttrib = &iterIndexData->second;
        }

        auto meshIterVertexData = mesh->m_vertexData.find("gl_Vertex");

        if (meshIterVertexData != mesh->m_vertexData.end()) {
            verticesAttrib = &meshIterVertexData->second.first;
        }

        meshIterVertexData = mesh->m_vertexData.find("gl_TexCoord");

        if (iterVertexData != mesh->m_vertexData.end()) {
            coordsAttrib = &meshIterVertexData->second.first;
        }

        meshIterVertexData = mesh->m_vertexData.find("gl_Normal");

        if (iterVertexData != mesh->m_vertexData.end()) {
            normalsAttrib = &meshIterVertexData->second.first;
        }

        auto meshIterIndexData = mesh->m_indexData.find("triangles");

        if (meshIterIndexData != mesh->m_indexData.end()) {
            trianglesAttrib = &meshIterIndexData->second;
        }

        if (verticesAttrib && m_verticesAttrib) {
            vertex_array_type& vertices = *m_verticesAttrib;
            const vertex_array_type& mesh_vertices = *verticesAttrib;

            vertices.reserve(vertices.size() + mesh_vertices.size());
            std::copy(mesh_vertices.begin(), mesh_vertices.end(), vertices.end());
        }

        if (normalsAttrib && m_normalsAttrib) {
            vertex_array_type& normals = *m_normalsAttrib;
            const vertex_array_type& mesh_normals = *normalsAttrib;

            normals.reserve(normals.size() + mesh_normals.size());
            std::copy(mesh_normals.begin(), mesh_normals.end(), normals.end());

        }

        if (coordsAttrib && m_coordsAttrib) {
            vertex_array_type& coords = *m_coordsAttrib;
            const vertex_array_type& mesh_coords = *coordsAttrib;

            coords.reserve(coords.size() + mesh_coords.size());
            std::copy(mesh_coords.begin(), mesh_coords.end(), coords.end());
        }

        if (trianglesAttrib && m_trianglesAttrib) {
            index_array_type& triangles = *m_trianglesAttrib;
            index_array_type mesh_triangles(*trianglesAttrib);




            triangles.reserve(triangles.size() + mesh_triangles.size());
            std::copy(mesh_triangles.begin(), mesh_triangles.end(), triangles.end());

            for (size_t i = 0; i < mesh_triangles.size(); i += m_dim) {
                mesh_triangles[i] += triangles.size() * 2 / 3;
                mesh_triangles[i + 1] += triangles.size() * 2 / 3;
                mesh_triangles[i + 2] += triangles.size() * 2 / 3;
            }

            triangles.reserve(triangles.size() + mesh_triangles.size());
            std::copy(mesh_triangles.begin(), mesh_triangles.end(), triangles.end());
        }

        compile();
    }

    Vector3
    Mesh::getVertexV(size_t startingIndex) const {
        auto iter = m_vertexData.find("gl_Vertex");

        if (iter != m_vertexData.end()) {
            const vertex_array_type& vertices = iter->second.first;

            try {
                return Vector3{vertices[startingIndex], vertices[startingIndex + 1], vertices[startingIndex + 2]};
            } catch (std::out_of_range& e) {
            }
        }

        return Vector3();
    }

    void
    Mesh::setVertex(size_t num, Vector3 vector) {
        auto iter = m_vertexData.find("gl_Vertex");

        if (iter != m_vertexData.end()) {
            vertex_array_type& vertices = iter->second.first;

            try {
                vertices[num * m_dim] = vector[0];
                vertices[num * m_dim + 1] = vector[1];
                vertices[num * m_dim + 2] = vector[2];
            } catch (std::out_of_range& e) {

            }
        }
    }

    void
    Mesh::setVertex(size_t num, std::vector<float> vector) {
        auto iter = m_vertexData.find("gl_Vertex");

        if (iter != m_vertexData.end()) {
            vertex_array_type& vertices = iter->second.first;

            try {
                for (size_t ii = 0; ii < vector.size(); ++ii) {
                    vertices[num * m_dim + ii] = vector[ii];
                }
            } catch (std::out_of_range& e) {


            }
        }
    }

    Mesh::vertex_array_type&
    Mesh::getVertices() {
        return m_vertexData["gl_Vertex"].first;
    }

    void
    Mesh::setVertices(const std::vector<float>& vertices, uint32_t spacing) {
        setFloatAttribute(vertices, "gl_Vertex", spacing);
    }

    void
    Mesh::setVertices(std::vector<float>&& vertices, uint32_t spacing) {
        setFloatAttribute(std::move(vertices), "gl_Vertex", spacing);
    }

    void
    Mesh::setFloatAttribute(std::vector<float>&& attribute, const aex::string& name, uint32_t spacing) {
        auto iter = m_vertexData.find(name);

        if (iter == m_vertexData.end()) {
            addVertexBuffer(name, spacing);
        }

        m_vertexData[name].first.swap(attribute);
        m_compiled = false;
    }

    void
    Mesh::setFloatAttribute(const std::vector<float>& attribute, const aex::string& name, uint32_t spacing) {
        auto iter = m_vertexData.find(name);

        if (iter == m_vertexData.end()) {
            addVertexBuffer(name, spacing);
        }

        m_vertexData[name].first = attribute;
        m_compiled = false;
    }

    void Mesh::setDrawMode(int drawMode) {
        m_drawMode = drawMode;
    }

    int Mesh::getDrawMode() const {
        return m_drawMode;
    }

    Mesh::vertex_array_type*
    Mesh::getNormals() {
        return getAttribute("gl_Normal");
    }

    const Mesh::vertex_array_type*
    Mesh::getNormals() const {
        return getAttribute("gl_Normal");
    }

    const Mesh::vertex_array_type*
    Mesh::getAttribute(const aex::string& name) const {
        auto iter = m_vertexData.find(name);
        const vertex_array_type* ret = nullptr;

        if (iter != m_vertexData.end()) {
            ret = &iter->second.first;
        }

        return ret;
    }

    Mesh::vertex_array_type*
    Mesh::getAttribute(const aex::string& name) {
        auto iter = m_vertexData.find(name);
        vertex_array_type* ret = nullptr;

        if (iter != m_vertexData.end()) {
            ret = &iter->second.first;
        }

        return ret;
    }

    void
    Mesh::setNormals(const std::vector<Mesh::vertex_type>& normals, uint32_t spacing) {
        setFloatAttribute(normals, "gl_Normal", spacing);
    }

    void
    Mesh::setNormals(std::vector<Mesh::vertex_type>&& normals, uint32_t spacing) {
        setFloatAttribute(std::move(normals), "gl_Normal", spacing);
    }

    Mesh::vertex_array_type*
    Mesh::getCoords() {
        return getAttribute("gl_TexCoord");
    }
    

    void
    Mesh::setCoords(const std::vector<Mesh::vertex_type>& coords, uint32_t spacing) {
        setFloatAttribute(coords, "gl_TexCoord", spacing);
    }

    void
    Mesh::setCoords(std::vector<Mesh::vertex_type>&& coords, uint32_t spacing) {
        setFloatAttribute(std::move(coords), "gl_TexCoord", spacing);
    }
    
    uint32_t
    Mesh::getIndexSize()
    {   if(m_indexData.find("triangles")!=m_indexData.end())
        {
            return m_indexData.at("triangles").size();
        }
        else
        {
            return 0;
        }
    }
    
    const Mesh::index_array_type&
    Mesh::getIndices() const {
        return m_indexData.at("triangles");
    }

    Mesh::index_array_type&
    Mesh::getIndices() {
        return m_indexData["triangles"];
    }

    void
    Mesh::setTriangles(const std::vector<Mesh::indices_type>& triangles) {
        auto iter = m_indexData.find("triangles");

        if (iter == m_indexData.end()) {
            addIndexBuffer("triangles");
        }

        m_indexData["triangles"] = triangles;
        m_compiled = false;
    }

    void
    Mesh::setTriangles(std::vector<Mesh::indices_type>&& triangles) {
        auto iter = m_indexData.find("triangles");

        if (iter == m_indexData.end()) {
            addIndexBuffer("triangles");
        }

        m_indexData["triangles"].swap(triangles);
        m_compiled = false;
    }

    void
    Mesh::setNextFrame(const std::vector<Mesh::vertex_type>& frame, uint32_t spacing) {
        setFloatAttribute(frame, "gl_NextFrame", spacing);
    }

    void
    Mesh::setNextFrame(std::vector<Mesh::vertex_type>&& frame, uint32_t spacing) {
        setFloatAttribute(std::move(frame), "gl_NextFrame", spacing);
    }
    //
    //    const aex::Attrib<Mesh::indices_type>&
    //    Mesh::getLines() const {
    //        ShortAttrib* m_linesAttrib = NULL;
    //        for (auto & attrib : m_indexBuffers) {
    //            if (attrib->resName() == "lines") {
    //                m_linesAttrib = &(*attrib);
    //            }
    //        }
    //
    //        return (*m_linesAttrib);
    //    }

    bool triangle_intersection(
            const aex::math::Vector3& V1, 
            const aex::math::Vector3& V2,
            const aex::math::Vector3& V3,
            const aex::math::Vector3& origin,
            const aex::math::Vector3& direction, 
            float& out) {
        aex::math::Vector3 edge1, edge2; 
        aex::math::Vector3 determinant, Q, distanceVector;
        float det, inv_det, u, v;
        float t;

        edge1= V2 - V1;
        edge2= V3 - V1;

        determinant = aex::math::cross(direction, edge2);
        det = aex::math::dot(edge1, determinant);

        if (det > -0.001 && det < 0.001) 
        {
            return 0;
        }
        
        inv_det = 1.f / det;
        distanceVector= origin - V1;
        u = aex::math::dot(distanceVector, determinant) * inv_det;

        if (u < 0.f || u > 1.f)
        {
            return 0;
        }

        Q = aex::math::cross(distanceVector, edge1);
        v = aex::math::dot(direction, Q) * inv_det;

        if (v < 0.f || u + v > 1.f)
        {
            return 0;
        }

        t = aex::math::dot(edge2, Q) * inv_det;

        if (t > 0.001) { 
            out = t;
            return 1;
        }
        
        return 0;
    }

    aex::math::Vector3 Mesh::raycastMesh(const aex::math::Matrix& transform, const aex::math::Vector3& startPoint, const aex::math::Vector3& dir,aex::math::Vector3*  retNormal ) {
        std::vector<mesh_indices_type>& indices = getIndices();
        std::vector<mesh_vectors_type>& vertices = getVertices();
        //std::vector<mesh_vectors_type>* normals = getNormals();
        
        size_t size = indices.size();
        aex::math::Vector3 normalDir = dir.unit();
        if (size % 3 != 0) {
            LOG_ERR("raycast on mesh error, maybe not triangles");
        }
        aex::math::Vector3 ret;
        float retDet = FLT_MAX;
        bool isHit = false;
        size_t hit_triangle = 0;
        for (size_t triangle = 0; triangle < size; triangle += 3) {
            size_t trx1 = indices[triangle]*3;
            size_t trx2 = indices[triangle + 1]*3;
            size_t trx3 = indices[triangle + 2]*3;
            aex::math::Vector3 A = transform.transformPoint(aex::math::Vector3(vertices[trx1], vertices[trx1 + 1], vertices[trx1 + 2]));
            aex::math::Vector3 B = transform.transformPoint(aex::math::Vector3(vertices[trx2], vertices[trx2 + 1], vertices[trx2 + 2]));
            aex::math::Vector3 C = transform.transformPoint(aex::math::Vector3(vertices[trx3], vertices[trx3 + 1], vertices[trx3 + 2]));

            float det = 0.0f;
            bool hit = triangle_intersection(A, B, C, startPoint, normalDir, det);
            if (hit) {

                isHit = true;
                //LOG_DEBUG("raycast mesh: ", det, ", ", startPoint, startPoint + (normalDir * det), A, B, C);

                if (retDet > det) {
                    retDet = det;
                    hit_triangle = triangle;
                }
            }
        }

        if (isHit) 
        {
            
            if(retNormal)
            {
                size_t trx1 = indices[hit_triangle]*3;
                size_t trx2 = indices[hit_triangle + 1]*3;
                size_t trx3 = indices[hit_triangle + 2]*3;
                aex::math::Vector3 A = transform.transformPoint(aex::math::Vector3(vertices[trx1], vertices[trx1 + 1], vertices[trx1 + 2]));
                aex::math::Vector3 B = transform.transformPoint(aex::math::Vector3(vertices[trx2], vertices[trx2 + 1], vertices[trx2 + 2]));
                aex::math::Vector3 C = transform.transformPoint(aex::math::Vector3(vertices[trx3], vertices[trx3 + 1], vertices[trx3 + 2]));
                
                aex::math::Vector3 faceNorm = (B - A).cross((C - A));
                faceNorm.normalizeThis();
                
                retNormal->x=faceNorm.x;
                retNormal->y=faceNorm.y;
                retNormal->z=faceNorm.z;
 
            }
            
            return (startPoint + (normalDir * retDet));
        } else {
            return startPoint;
            if(retNormal)
            {
                retNormal->x=0.0f;
                retNormal->y=1.0f;
                retNormal->z=0.0f;
            }
            //return (startPoint+(normalDir*(startPoint.y/abs(normalDir.y))));
        }
    }

    void Mesh::buildPerFacePerVertexNoramals() {
        std::vector<mesh_vectors_type> new_noramls;
        std::vector<mesh_vectors_type> new_noramlsPrim;
        std::vector<mesh_vectors_type> new_vertices;
        std::vector<mesh_vectors_type> new_uvs;
        std::vector<mesh_indices_type> new_indices;


        std::vector<mesh_vectors_type> old_vertices = getVertices();
        std::vector<mesh_vectors_type>* old_uvs = getCoords();
        std::vector<mesh_indices_type>& old_indices = getIndices();
        bool hasuvs = false;

        if (old_uvs != nullptr) {
            //LOG_DEBUG("old_uvs, size: ", old_uvs->size());

            if (old_uvs->size() > 0) {
                hasuvs = true;
            }
        }

        if (hasuvs) {
            for (mesh_indices_type i = 0; i < old_indices.size(); ++i) {

                new_vertices.push_back(old_vertices[old_indices[i] * 3]);
                new_vertices.push_back(old_vertices[(old_indices[i] * 3) + 1]);
                new_vertices.push_back(old_vertices[(old_indices[i] * 3) + 2]);

                new_uvs.push_back((*old_uvs)[old_indices[i] * 2]);
                new_uvs.push_back((*old_uvs)[(old_indices[i] * 2) + 1]);
                new_indices.push_back(i);
            }
        } else {
            for (mesh_indices_type i = 0; i < old_indices.size(); ++i) {

                new_vertices.push_back(old_vertices[old_indices[i] * 3]);
                new_vertices.push_back(old_vertices[(old_indices[i] * 3) + 1]);
                new_vertices.push_back(old_vertices[(old_indices[i] * 3) + 2]);

                new_indices.push_back(i);
            }
        }

        new_noramls = makeFaceNormals(new_vertices, new_indices);

        for (size_t uj = 0; uj < new_noramls.size(); uj += 3) {
            for (int b = 0; b < 3; b++) {
                new_noramlsPrim.push_back(new_noramls[uj]);
                new_noramlsPrim.push_back(new_noramls[uj + 1]);
                new_noramlsPrim.push_back(new_noramls[uj + 2]);
            }
        }

        setFloatAttribute(std::move(new_vertices), "gl_Vertex", 3);
        setFloatAttribute(std::move(new_noramlsPrim), "gl_Normal", 3);

        if (hasuvs) {
            setFloatAttribute(std::move(new_uvs), "gl_TexCoord", 2);
        }

        setTriangles(std::move(new_indices));

    }

    std::vector<mesh_vectors_type> scaleUvForTpacker(
            const std::vector<mesh_vectors_type>& uv,
            int Ax, int Ay,
            int Sizex, int Sizey,
            int txtsize) {

        aex::math::Vector2 uvOffset((float (Ax) / (float) txtsize),  (float (Ay+Sizey) / (float) txtsize));
        aex::math::Vector2 Bcoord((float) Sizex / (float) txtsize, (float) Sizey / (float) txtsize);

        std::vector<mesh_vectors_type> newuv;
        newuv.resize(uv.size());

        aex::math::Vector2 newSize = Bcoord - uvOffset;

        if (uv.size() % 2 != 0) {
            //LOG_ERR("Bad UV");
        }
        //LOG_DEBUG("uv Scale: ", Bcoord, " ", "uv offset", uvOffset);
        for (size_t i = 0; i < uv.size(); i += 2) {
            newuv[i]=(uv[i] * Bcoord.x)+(uvOffset.x);
            newuv[i+1]=(uv[i + 1] * Bcoord.y)-(uvOffset.y);
            //LOG_DEBUG("new uv: ",newuv[i]," ",newuv[i+1],"old :",uv[i]," ",uv[i + 1]  );
        }

        return newuv;

    }

    void
    Mesh::rebuildCoordsForTpacker(int Posx, int Posy, int Sizex, int Sizey, int textsize) {
        std::vector<mesh_vectors_type>* old_uvs = getCoords();

        setFloatAttribute(scaleUvForTpacker(*old_uvs, Posx, Posy, Sizex, Sizey, textsize), "gl_TexCoord", 2);
    }

    //
    //    void
    //    Mesh::constructPacked() {
    //        FloatAttrib* verticesAttrib = NULL;
    //        FloatAttrib* normalsAttrib = NULL;
    //        FloatAttrib* coordsAttrib = NULL;
    //
    //        for (auto & attrib : m_vertexBuffers) {
    //            if (attrib->resName() == "vertices") {
    //                verticesAttrib = &(*attrib);
    //            }
    //            if (attrib->resName() == "coords") {
    //                coordsAttrib = &(*attrib);
    //            }
    //            if (attrib->resName() == "normals") {
    //                normalsAttrib = &(*attrib);
    //            }
    //        }
    //        std::vector<float>& vertices = (*verticesAttrib->array());
    //        std::vector<float>& normals = (*normalsAttrib->array());
    //        std::vector<float>& coords = (*coordsAttrib->array());
    //
    //        aex::AttribManager<float>& manager = aex::AttribManager<float>::getInstance();
    //
    //        aex::HAttrib<float>::type handle = manager.GetAttrib(m_name + "packed", "packed");
    //        aex::Attrib<float> *attrib = manager.Dereference(handle);
    //
    //        m_vertexBufferPacked.m_attrib = attrib;
    //        size_t numVertices = (vertices.size() + 1) / 3;
    //        uint32_t strideNorm = 0;
    //        uint32_t strideCoord = 0;
    //
    //
    //        if (m_bNormals)
    //            strideNorm = NORMAL_DATA_SIZE;
    //        else strideNorm = 0;
    //
    //        if (m_bCoords)
    //            strideCoord = m_coordsSize;
    //        else strideCoord = 0;
    //
    //        uint32_t stride = POSITION_DATA_SIZE + strideNorm + strideCoord;
    //
    //        if (!vertices.empty()) {
    //
    //            attrib->resize(numVertices * stride);
    //            for (int i = 0; i < numVertices; i++) {
    //                (*attrib)[i * stride] = vertices[i * 3];
    //                (*attrib)[i * stride + 1] = vertices[i * 3 + 1];
    //                (*attrib)[i * stride + 2] = vertices[i * 3 + 2];
    //                int k = 3;
    //                if (m_bNormals) {
    //                    (*attrib)[i * stride + k] = normals[i * 3];
    //                    (*attrib)[i * stride + k + 1] = normals[i * 3 + 1];
    //                    (*attrib)[i * stride + k + 2] = normals[i * 3 + 2];
    //                    k = 6;
    //                }
    //                if (m_bCoords) {
    //                    (*attrib)[i * stride + k] = coords[i * m_coordsSize];
    //                    (*attrib)[i * stride + k + 1] = coords[i * m_coordsSize + 1];
    //                    if (m_coordsSize == 2) {
    //                        (*attrib)[i * stride + k + 2] = coords[i * m_coordsSize + 2];
    //                    }
    //                }
    //            }
    //        }
    //    }

    Mesh*
    plane(uint32_t detailX, uint32_t detailY, bool bCoords, bool bNormals) {

        if (detailX <= 0) {
            detailX = 1;
        }

        if (detailY <= 0) {
            detailY = 1;
        }

        aex::string sbCoords;
        aex::string sbNorms;

        if (bCoords) {
            sbCoords = "1";
        } else {
            sbCoords = "0";
        }

        if (bNormals) {
            sbNorms = "1";
        } else {
            sbNorms = "0";
        }

        aex::string name = "plane_" + aex::to_string(detailX) + "_" + aex::to_string(detailX) + "_" + sbCoords + "_" + sbNorms;
        Mesh* mesh = new Mesh(name, false, bCoords, bNormals);

        Mesh::vertex_array_type& vertices = mesh->getVertices();
        Mesh::vertex_array_type* normalsAttrib = mesh->getAttribute("gl_Normal");
        Mesh::vertex_array_type* coordsAttrib = mesh->getAttribute("gl_TexCoord");
        Mesh::index_array_type& triangles = mesh->getIndices();

        triangles = std::vector<Mesh::indices_type>((detailX + detailY - 2) * 6 + 5 + 1);

        for (uint32_t y = 0; y <= detailY; y++) {
            float t = y / (float) detailY;

            for (uint32_t x = 0; x <= detailX; x++) {
                float s = x / (float) detailX;

                vertices[(x + y) * 3] = 2 * s - 1;
                vertices[(x + y) * 3 + 1] = 2 * t - 1;
                vertices[(x + y) * 3 + 2] = 0;

                if (mesh->m_bCoords && coordsAttrib) {
                    std::vector<Mesh::vertex_type>& coords = *coordsAttrib;
                    coords[(x + y) * 2] = s;
                    coords[(x + y) * 2 + 1] = t;
                }

                if (mesh->m_bNormals && normalsAttrib) {
                    std::vector<Mesh::vertex_type>& normals = *normalsAttrib;
                    normals[(x + y) * 3] = 0;
                    normals[(x + y) * 3 + 1] = 0;
                    normals[(x + y) * 3 + 2] = 1;
                }

                if (x < detailX && y < detailY) {
                    int i = x + y * (detailX + 1);

                    triangles[(x + y) * 6] = (short) i;
                    triangles[(x + y) * 6 + 1] = (short) (i + 1);
                    triangles[(x + y) * 6 + 2] = (short) (i + detailX + 1);
                    triangles[(x + y) * 6 + 3] = (short) (i + detailX + 1);
                    triangles[(x + y) * 6 + 4] = (short) (i + 1);
                    triangles[(x + y) * 6 + 5] = (short) (i + detailX + 2);
                }
            }
        }

        mesh->compile();
        return mesh;
    }

    Mesh*
    sphere(uint32_t detail) {
        return new Icosahedron(detail);
    }

    Mesh*
    load(aex::string fName) {
        std::ifstream fileHandle;
        FileManager fManager;

        fileHandle.open(fName, std::ios::in);
        //define file stream object, and open the file

        aex::string buff;
        buff.reserve(fManager.file_size(fName));

        buff.assign((std::istreambuf_iterator<char>(fileHandle)),
                std::istreambuf_iterator<char>());

        fileHandle.close();

        size_t len = buff.length();

        Mesh* mesh = new Mesh();

        for (size_t i = 0; i < len; i++) {
            if (buff.substr(i, 8).compare("vertices") == 0) {
                size_t entry_pos = i + 8;

                while (buff[entry_pos] != '[') {
                    entry_pos++;

                    if (entry_pos == len) {
                        break;
                    }
                }

                entry_pos++;

                uint32_t spacing = 1;

                for (aex::string::iterator iter = buff.begin() + entry_pos; (*iter) != ']'; ++iter) {
                    if ((*iter) == ',') {
                        spacing++;
                    }
                }

                std::vector<float> vertices = getVectorFromFile<float>(buff, entry_pos);

                mesh->setVertices(std::move(vertices));

                i = entry_pos;

                for (; i < len; i++) {
                    if (buff.substr(i, 7).compare("normals") == 0) {
                        entry_pos = i + 7;

                        while (buff[entry_pos] != '[') {
                            entry_pos++;

                            if (entry_pos == len) {
                                break;
                            }
                        }

                        entry_pos++;

                        std::vector<float> normals = getVectorFromFile<float>(buff, entry_pos);

                        mesh->setNormals(std::move(normals));

                        i = entry_pos;
                    }

                    if (buff.substr(i, 6).compare("coords") == 0) {
                        entry_pos = i + 6;

                        while (buff[entry_pos] != '[') {
                            entry_pos++;

                            if (entry_pos == len) {
                                break;
                            }
                        }

                        entry_pos++;

                        std::vector<float> coords = getVectorFromFile<float>(buff, entry_pos);

                        mesh->setCoords(std::move(coords));

                        i = entry_pos;
                    }

                    if (buff.substr(i, 9).compare("triangles") == 0) {
                        entry_pos = i + 9;

                        while (buff[entry_pos] != '[') {
                            entry_pos++;

                            if (entry_pos == len) {
                                break;
                            }
                        }

                        entry_pos++;

                        std::vector<Mesh::indices_type> triangles = getVectorFromFile<Mesh::indices_type>(buff, entry_pos);

                        mesh->setTriangles(std::move(triangles));

                        i = entry_pos;
                    }
                }
            }
        }

        return mesh;
    }

    template<typename T>
    std::vector<T>
    getVectorFromFile(const aex::string& buff, size_t& entry_pos) {
        bool endCheck = true;
        bool negative = false;
        bool decimalPart = false;
        bool numberFound = false;
        float decimalStep = 1.0f;
        std::list<T> fBuff;
        fBuff.push_back(0.0f);

        while (endCheck) {
            if (!(std::isdigit(buff[entry_pos]) || buff[entry_pos] == '[' || buff[entry_pos] == ']' || buff[entry_pos] == ',' || buff[entry_pos] == '.' || buff[entry_pos] == '-')) {
                //std::<<(entry_pos+": "+buff[entry_pos]);
                entry_pos++;
            }

            if (buff[entry_pos] == '.' || buff[entry_pos] == '-' || std::isdigit(buff[entry_pos])) {
                while ((buff[entry_pos] != ',' && buff[entry_pos] != ']')) {
                    if (!(std::isdigit(buff[entry_pos]) || buff[entry_pos] == '[' || buff[entry_pos] == ']' || buff[entry_pos] == ',' || buff[entry_pos] == '.' || buff[entry_pos] == '-')) {
                        entry_pos++;
                        continue;
                    }

                    numberFound = true;

                    if (buff[entry_pos] == '-') {
                        negative = true;
                        entry_pos++;
                        continue;
                    }

                    if (buff[entry_pos] == '.') {
                        decimalPart = true;
                        entry_pos++;
                        continue;
                    }

                    fBuff.back() *= 10.0f;

                    if (negative) {
                        fBuff.back() -= buff[entry_pos] - '0';
                    } else {
                        fBuff.back() += buff[entry_pos] - '0';

                    }

                    if (decimalPart) {
                        decimalStep *= 10.0f;
                    }

                    entry_pos++;
                }
            }

            if (numberFound) {
                fBuff.back() /= decimalStep;
                decimalPart = false;
                negative = false;
                decimalStep = 1.0f;
                numberFound = false;

                try {
                    fBuff.push_back(0.0f);
                } catch (const std::bad_alloc& e) {
                    std::cerr << "bad alloc";
                } catch (const std::exception& e) {
                    std::cerr << "exception";
                } catch (...) {
                    std::cerr << "anything";
                }
            }

            while (std::isspace(buff[entry_pos])) {
                entry_pos++;
            }

            if (buff[entry_pos] == ']') {
                entry_pos++;

                while (std::isspace(buff[entry_pos])) {
                    entry_pos++;
                }

                if (buff[entry_pos] == ']') {
                    endCheck = false;
                }
            } else {
                entry_pos++;
            }
        }

        fBuff.pop_back();

        std::vector<T> ret;
        ret.reserve(fBuff.size());
        ret.insert(ret.begin(), fBuff.begin(), fBuff.end());

        return ret;
    }

    void
    Mesh::createBoundingSphere() {
        m_bSphere = BoundingSphere();
        m_bSphere.welzlCompute(getVertices());
    }

    //////////////////////////////////Drawsy

    void Mesh::draw(AexRenderer* gl) {

        if (!m_isBufferBinded) {
            if (!m_compiled) {
                compile();
            }

            bindBuffers(gl, m_drawMode);
        }

        //drawBuffers(gl,GL_TRIANGLES);
        //        if (m_isAnimated) {
        //            m_tweenFactorUniform->setValue(m_TweenFactor);
        //m_tweenFactorUniform->setUniform(gl->getShaderProgram());
        //        }
    }

    void Mesh::bindBuffers(AexRenderer* gl, int mode) {

        const ShaderDrw* shader = gl->getShader();

        if (shader) {
            const AexShader& program = shader->GetProgram();

            for (auto buff : m_vertexBuffers) {
                program->setVertexData(*buff.second);
            }

            program->setIndices(*m_indexBuffers["triangles"]);
        }
    }

    void Mesh::drawBuffers(AexRenderer* gl, int mode) {
        const ShaderDrw* shader = gl->getShader();

        if (shader) {
            const AexShader& program = shader->GetProgram();
            program->draw();
        }
    }

    void Mesh::finishdraw(AexRenderer* gl) {
        const ShaderDrw* shader = gl->getShader();

        if (shader) {
            const AexShader& program = shader->GetProgram();

            for (auto buff : m_vertexBuffers) {
                program->releaseVertexData(*buff.second);
            }
        }

        m_isBufferBinded = false;
    }

    void Mesh::SetVector(std::vector<Mesh::indices_type>&& triangles) {
        setTriangles(std::move(triangles));

    }

    void Mesh::SetVector(std::vector<Mesh::vertex_type>&& vec) {
        setVertices(std::move(vec));

    }

    void addReversedFacesGeom(std::vector<mesh_indices_type>* indices) {

        if (indices->size() % 3 != 0) {
            LOG_ERR("Mesh: Something wrong about supplied indices, maybe not triangles?");
            return;
        }

        int size = indices->size();

        for (int i = 0; i < size; i += 3) {
            indices->push_back((*indices)[i + 2]);
            indices->push_back((*indices)[i + 1]);
            indices->push_back((*indices)[i]);
        }

    }

    void
    Mesh::addReversedFaces() {
        addReversedFacesGeom(&getIndices());

    }

    void Mesh::addBarycentricCoordinates() {

        std::vector<mesh_indices_type>& old_indices = getIndices();

        int size = old_indices.size();

        if (size % 3 != 0) {
            LOG_ERR("Mesh: Something wrong about supplied indices, maybe not triangles?");
            return;
        }

        int a = 0;
        std::vector<mesh_vectors_type> temp;
        temp.resize(getVertices().size());

        for (size_t i = 0; i < old_indices.size(); i += 3) {
            temp[old_indices[i] * 3] = 1.0f;
            temp[(old_indices[i] * 3) + 1] = 0.0f;
            temp[(old_indices[i] * 3) + 2] = 0.0f;

            temp[old_indices[i + 1] * 3] = 0.0f;
            temp[(old_indices[i + 1] * 3) + 1] = 1.0f;
            temp[(old_indices[i + 1] * 3) + 2] = 0.0f;

            temp[old_indices[i + 2] * 3] = 0.0f;
            temp[(old_indices[i + 2] * 3) + 1] = 0.0f;
            temp[(old_indices[i + 2] * 3) + 2] = 1.0f;
            a += 9;
        }

        LOG_DEBUG("getVertices().size(): ", getVertices().size(), " a: ", a, " temp: ", temp.size());

        setFloatAttribute(std::move(temp), "atr_Barycentric", 3);

    }

    void Mesh::setGeometryData(SimpleGeometryData& data)
    {
        this->setTriangles(std::move(data.indices));
        this->setFloatAttribute(std::move(data.vertices), std::string("gl_Vertex"), 3);
    }
    
    void Mesh::reverseFaces() {

        std::vector<mesh_indices_type>* indices = &getIndices();

        // LOG_DEBUG("std::vector<mesh_indices_type>* indices ", indices, " ", indices->at(2));

        int size = indices->size();

        if (size % 3 != 0 && m_drawMode != GL_TRIANGLE_STRIP) {
            LOG_ERR("Mesh: Something wrong about supplied indices, maybe not triangles?");
            return;
        }

        std::vector<mesh_indices_type> temp;
        temp.resize(size);

        if (m_drawMode != GL_TRIANGLE_STRIP) {
            std::vector<mesh_indices_type>::iterator iter = temp.begin();

            for (int i = 0; i < size; i += 3) {

                *iter++ = (*indices)[i + 2];
                *iter++ = (*indices)[i + 1];
                *iter++ = (*indices)[i];

            }
        } else {
            std::vector<mesh_indices_type>::reverse_iterator iter = temp.rbegin();

            for (int i = 0; i < size; i += 3) {

                *iter++ = (*indices)[i ];
                *iter++ = (*indices)[i + 1];
                *iter++ = (*indices)[i + 2];
            }
        }

        setTriangles(std::move(temp));
    }

    std::vector<mesh_vectors_type> scaleUvForTextureMap(
            const std::vector<mesh_vectors_type>& uv,
            aex::math::Vector2 Acoord,
            aex::math::Vector2 Bcoord) {

        std::vector<mesh_vectors_type> newuv;

        aex::math::Vector2 newSize = Bcoord - Acoord;

        if (uv.size() % 2 != 0) {
            LOG_ERR("Bad UV");
        }

        for (size_t i = 0; i < uv.size(); i += 2) {
            newuv.push_back((uv[i] * newSize.x) + Acoord.x);
            newuv.push_back((uv[i + 1] * newSize.y) + Acoord.y);
        }

        return newuv;

    }

    std::vector<mesh_vectors_type> makeFaceNormals
    (
            const std::vector<mesh_vectors_type>& vertices,
            const std::vector<mesh_indices_type>& indices
            ) {
        std::vector<mesh_vectors_type> ret(0);
        size_t faceCount = indices.size() / 3;

        if (indices.size() % 3 != 0) {
            LOG_ERR("Mesh: Something wrong about supplied indices, maybe not triangles?");
            return ret;
        }


        ret.reserve(indices.size());

        for (size_t face = 0; face < faceCount; ++face) {
            if (vertices.size() >= indices[face * 3] * 3) {
                size_t faceIdx1 = indices[face * 3];
                size_t faceIdx2 = indices[face * 3 + 1];
                size_t faceIdx3 = indices[face * 3 + 2];

                Vector3 A(
                        vertices[faceIdx1 * 3],
                        vertices[faceIdx1 * 3 + 1],
                        vertices[faceIdx1 * 3 + 2]
                        );
                Vector3 B(
                        vertices[faceIdx2 * 3],
                        vertices[faceIdx2 * 3 + 1],
                        vertices[faceIdx2 * 3 + 2]
                        );

                Vector3 C(
                        vertices[faceIdx3 * 3],
                        vertices[faceIdx3 * 3 + 1],
                        vertices[faceIdx3 * 3 + 2]
                        );

                aex::math::Vector3 faceNorm = (C - A).cross((B - A));
                faceNorm.normalizeThis();
                ret.push_back(faceNorm.x);
                ret.push_back(faceNorm.y);
                ret.push_back(faceNorm.z);
            } else {
                //Somethng was off just bail
                LOG_ERR("Mesh: Computing face normals failed,"
                        " seems size of vertices and indices does not match");
                return std::vector<mesh_vectors_type>(0);
            }

        }

        return ret;
    }

    std::vector<mesh_vectors_type> makeNormals
    (
            const std::vector<mesh_vectors_type>& vertices,
            const std::vector<mesh_indices_type>& indices
            ) {
        std::vector<mesh_vectors_type> faceNormals = makeFaceNormals(vertices, indices);
        std::vector<mesh_vectors_type> vertNormals(vertices.size());

        size_t faceCount = indices.size() / 3;

        if (indices.size() % 3 != 0) {
            LOG_ERR("Mesh: Something wrong about supplied indices, maybe not triangles?");
            return std::vector<mesh_vectors_type>(0);
        }

        if (faceNormals.size() != faceCount * 3) {
            LOG_ERR("Mesh: Something wrong about face normals.");
            return std::vector<mesh_vectors_type>(0);
        }

        std::fill(vertNormals.begin(), vertNormals.end(), 0.0f);

        for (size_t face = 0; face < faceCount; face++) {
            if (vertices.size() >= indices[face * 3 + 2]) {
                size_t faceIdx1 = indices[face * 3] * 3;
                size_t faceIdx2 = indices[face * 3 + 1] * 3;
                size_t faceIdx3 = indices[face * 3 + 2] * 3;

                aex::math::Vector3 faceNorm(
                        faceNormals[face * 3],
                        faceNormals[face * 3 + 1],
                        faceNormals[face * 3 + 2]
                        );


                aex::math::Vector3 faceVert1(vertices[faceIdx1], vertices[faceIdx1 + 1], vertices[faceIdx1 + 2]);
                aex::math::Vector3 faceVert2(vertices[faceIdx2], vertices[faceIdx2 + 1], vertices[faceIdx2 + 2]);
                aex::math::Vector3 faceVert3(vertices[faceIdx3], vertices[faceIdx3 + 1], vertices[faceIdx3 + 2]);

                aex::math::Vector3 v1Norm = aex::math::angleCentroidWeight(
                        faceVert1,
                        faceVert2,
                        faceVert3,
                        faceNorm
                        );

                aex::math::Vector3 v2Norm = aex::math::angleCentroidWeight(
                        faceVert2,
                        faceVert3,
                        faceVert1,
                        faceNorm
                        );

                aex::math::Vector3 v3Norm = aex::math::angleCentroidWeight(
                        faceVert3,
                        faceVert2,
                        faceVert1,
                        faceNorm
                        );

                vertNormals[faceIdx1] += v1Norm.x;
                vertNormals[faceIdx1 + 1] += v1Norm.y;
                vertNormals[faceIdx1 + 2] += v1Norm.z;

                vertNormals[faceIdx2] += v2Norm.x;
                vertNormals[faceIdx2 + 1] += v2Norm.y;
                vertNormals[faceIdx2 + 2] += v2Norm.z;

                vertNormals[faceIdx3] += v3Norm.x;
                vertNormals[faceIdx3 + 1] += v3Norm.y;
                vertNormals[faceIdx3 + 2] += v3Norm.z;
            } else {
                //Somethng was off just bail
                LOG_ERR("Mesh: Computing face normals failed,"
                        " seems size of vertices and indices does not match");
                return std::vector<mesh_vectors_type>(0);
            }
        }

        for (size_t normIdx = 0; normIdx < vertNormals.size(); normIdx += 3) {
            float mag = aex::math::Vector3(
                    vertNormals[normIdx],
                    vertNormals[normIdx + 1],
                    vertNormals[normIdx + 2]).length();

            vertNormals[normIdx] /= mag;
            vertNormals[normIdx + 1] /= mag;
            vertNormals[normIdx + 2] /= mag;
        }

        return vertNormals;
    }

    SimpleGeometryData weldVertices(
        const std::vector<mesh_vectors_type>& vertices,
        const std::vector<mesh_indices_type>& indices,
        float distance )
    {
        float squaredDistance = sqrt(distance);  
        SimpleGeometryData ret;
        ret.isHardEdged = false;
        ret.indices.resize(indices.size(), 0);
        Mesh::vertex_array_type retVerts;
        retVerts.reserve(vertices.size()*3);
        std::vector<aex::math::Vector3>* hardEdges = (std::vector<aex::math::Vector3>*)& vertices;
        
        for(size_t i = 0; i < indices.size() ; ++i){
            
            aex::math::Vector3 thisVert = (*hardEdges)[indices[i]];
            size_t vertFound = 0;
            bool vertFoundBool = false;
            
            for(size_t r = 0; r < retVerts.size()/3; ++r)
            {
                if((thisVert - aex::math::Vector3(retVerts[r*3],retVerts[(r*3)+1],retVerts[(r*3)+2])).squaredLength() < squaredDistance )
                {
                    vertFound=r;
                    vertFoundBool= true;
                        //LOG(i ,"   vertFound: ",vertFound , retVerts[r]);
                    break;
               }
            }
     
            if(vertFoundBool)
            {
                ret.indices[i]=vertFound;
            }
            else
            {
                ret.indices[i]=retVerts.size()/3;
                LOG(i, "not vertFoun: ",retVerts.size()/3," " , thisVert);
                retVerts.push_back(thisVert.x);
                retVerts.push_back(thisVert.y);
                retVerts.push_back(thisVert.z);
  
            }
           
        }
          
        retVerts.shrink_to_fit();
        ret.vertices = retVerts;
        LOG("ret.vertices: ", ret.vertices.size()/3, " ret.indices: ",ret.indices.size());
        return ret; 
    }
    
}
