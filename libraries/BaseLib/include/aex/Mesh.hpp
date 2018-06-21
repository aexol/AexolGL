//
//  Mesh.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 03.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#ifndef __aexolGL__Mesh__
#define __aexolGL__Mesh__
#include <aex/Common.hpp>
#include <aex/Vector3.hpp>
#include <aex/Matrix.hpp>
#include <aex/BoundingSphereObject.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/AABB.hpp>
#include <string>
#include <list>
#include <cfloat>
#include <atomic>
#include <stack>
#include <map>
//#include <aex/Uniforms.hpp>


namespace aex
{
class ObjReader;
typedef aex::math::Vector3 Vector3;
class VertexData;
class IndexData;

class Mesh;
class AexRenderer;

struct SimpleGeometryData;

typedef shared_ptr<Mesh> MeshPtr; //To remove

/**
 *	@brief	Geometry of 3D object.
 */

class AEX_PUBLIC_API Mesh : public DrawObject, public DrawVisibility
{
public:
    typedef mesh_vectors_type vertex_type;
    typedef mesh_indices_type indices_type;
    typedef IndexData IndexBuffer;
    typedef std::shared_ptr<VertexData> VertexDataRef;
    typedef std::shared_ptr<IndexBuffer> IndexDataRef;
    typedef IndexDataRef IndexBufferRef;
    typedef std::vector<vertex_type> vertex_array_type;
    typedef std::vector<indices_type> index_array_type;
    typedef std::map<aex::string, VertexDataRef> VertexBuffers;
    typedef std::map<aex::string, IndexDataRef> IndexBuffers;
    typedef std::map<aex::string, std::pair<vertex_array_type, size_t>> VertexCache;
    typedef std::map<aex::string, index_array_type> IndexCache;
    

public:
    /**
     *	@brief	Default ctor.
     */
    Mesh();



    /**
     *	@brief	Ctor
     *
     *	@param 	name 	Mesh name.
     *	@param 	usePacked 	Use packed buffer.
     *	@param 	bcoords 	Have UVs.
     *	@param 	bNormals 	Have normals
     */
    Mesh(const aex::string& name, bool usePacked = false, bool bCoords = false, bool bNormals = false);

    //TODO - Copy, move ctors, ?data_based_ctor?
    virtual ~Mesh();

    /**
     *	@brief	Load data into a mesh
     *
     *	@param 	name 	Mesh name.
     *  @param  numVertices Number of vertices.
     *	@param 	usePacked 	Use packed buffer.
     *	@param 	bcoords 	Have UVs.
     *	@param 	bNormals 	Have normals
     *
     *  @return True if suceeded.
     */
    bool Load(const aex::string& name, uint32_t numVertices = 0, bool usePacked = false, bool bCoords = false, bool bNormals = false);
    /**
     *	@brief	Load data from JSON file.
     *
     *	@param 	fileName 	File name.
     *
     *	@return	Reference to this.
     */
    void LoadFromJSONFile(const aex::string& fileName);

    void Unload();

    /**
     *	@brief	Adds new vertexbuffer from AttribManager
     *
     *	@param 	name 	Name of buffer
     *	@param 	aex::string 	name of attribiute
     *
     *	@return	return dereference to attribiute
     */
    VertexData* addVertexBuffer(const aex::string& name);
    VertexData* addVertexBuffer(const aex::string& name, size_t components);

    /**
     *	@brief	Adds new indexBuffer from Atttrib Manager
     *
     *	@param 	name 	Name of buffer
     *
     *	@return	return dereference to indexBuffer attribiute
     */
    IndexData* addIndexBuffer(const aex::string& name);
    void setIndexBuffer(const aex::string& name, const IndexDataRef& ref);
    void delete_buffer(const aex::string& name);

    /**
     *	@brief	Deletes all mesh buffers
     */
    void delete_buffers();

    /**
     *	@brief	Compile buffers
     */
    void compile();


    void SetVector(std::vector<Mesh::indices_type>&& triangles);

    void SetVector(std::vector<Mesh::vertex_type>&& vec);

    //TRANSFORMS

    /**
     *	@brief	Transforms object normals by matrix on cpu(hard change)
     *
     *	@param 	matV 	transforation matrix
     *	@param 	r       transform true or false
     *
     *	@return	return transformed mesh
     */
    void transform(aex::math::Matrix matV, bool r);

    //TRANSFORMS

    /**
     *	@brief	Transforms object normals by matrix on cpu(hard change) relative to first vertex in buffer
     *
     *	@param 	matV 	transforation matrix
     *	@param 	r       transform true or false
     *
     *	@return	return *this
     */
    void _transform(aex::math::Matrix matV, bool r);

    /**
     *	@brief	Move transform
     *
     *	@param 	x 	x lenght
     *	@param 	y 	y lenght
     *	@param 	z 	z lenght
     *
     *	@return	return *this
     */
    void move(float x, float y, float z);

    /**
     *	@brief	Rotation transform in degrees
     *
     *	@param 	x 	x axis degrees
     *	@param 	y 	y axis degrees
     *	@param 	z 	z axis degrees
     *
     *	@return	return *this
     */
    void rotate(float x, float y, float z);

    /**
     *	@brief	Scale transformation
     *
     *	@param 	x 	x size scale
     *	@param 	y 	y size scale
     *	@param 	z 	z size scale
     *
     *	@return
     */
    void scale(float x, float y, float z);

    /**
     *	@brief	Computes normals for mesh
     *
     *	@return	<#return value description#>
     */
    void computeNormals();

    /**
     *	@brief	Computes index buffer to draw mesh as wireframe
     *
     *	@param 	forceCompute 	force compute even if alredy done
     *
     *
     */
    void computeWireFrame(bool forceCompute);

    /**
     *	@brief	Computes index buffer to draw mesh as wireframe
     *
     */
    void computeWireFrame();


    /**
     *	@brief	Combines two meshes in one
     *
     *	@param 	mesh 	mesh to combine witch
     *
     */
    void combine(const Mesh* mesh);


    /**
     *	@brief	Gets Axis aligned bouding box
     *
     */
    AABB getAABB();


    //Access
    /**
     *	@brief	Get vertex at index in vertexbuffer
     *
     *	@param 	size_t 	index of wanted vertex
     *
     *	@return	Vector3
     */
    Vector3 getVertexV(size_t) const;

    /**
     *	@brief	Set vertex at index in vertexbuffer
     *
     *	@param 	size_t 	index of wanted vertex
     *
     *	@param	Vector3 new vertex
     */
    void setVertex(size_t, Vector3);

    /**
     *	@brief	Set vertex at index in vertexbuffer
     *
     *	@param 	size_t 	index of wanted vertex
     *
     *	@param	std::vector<float> new vertex
     */
    void setVertex(size_t, std::vector<float>);


    /**
     *	@brief	Get Vertices
     *
     */
    vertex_array_type& getVertices();


    /**
     *	@brief	Set new vertices in mesh
     *
     *	@param 	std::vector<float>& 	vector of new veritces in form: x,y,z,x,y,z...
     *	@param 	3 	number of dimensions for vertex
     */
    void setVertices(const std::vector<float>&, uint32_t = 3);

    /**
     *	@brief	Set new vertices in mesh
     *
     *	@param 	std::vector<float>& 	vector of new veritces in form: x,y,z,x,y,z...
     *	@param 	3 	number of dimensions for vertex
     */
    void setVertices(std::vector<float>&&, uint32_t = 3);

    void setFloatAttribute(std::vector<vertex_type>&&, const aex::string& name, uint32_t = 3);
    void setFloatAttribute(const std::vector<vertex_type>&, const aex::string& name, uint32_t = 3);
    const vertex_array_type* getAttribute(const aex::string& name) const;
    vertex_array_type* getAttribute(const aex::string& name);


    /**
     *	@brief	Get mesh normals
     *
     */

    vertex_array_type* getNormals();
    const vertex_array_type* getNormals() const;
    /**
     *	@brief	Set new normals in mesh
     *
     *	@param 	std::vector<float>& 	vector of new normals in form: x,y,z,x,y,z...
     *	@param 	3 	number of dimensions for normal
     */
    void setNormals(const std::vector<float>&, uint32_t = 3);

    /**
     *	@brief	Set new normals in mesh
     *
     *	@param 	std::vector<float>& 	vector of new normals in form: x,y,z,x,y,z...
     *	@param 	3 	number of dimensions for normal
     */
    void setNormals(std::vector<float>&&, uint32_t = 3);

    /**
     *	@brief	Get UV coords
     *
     */
    vertex_array_type* getCoords();
    

    /**
     *	@brief	Set new UV coords in mesh
     *
     *	@param 	std::vector<float>& 	vector of new UV coords in form: x,y,z,x,y,z...
     *	@param 	3 	number of dimensions for UV
     */
    void setCoords(const std::vector<float>&, uint32_t = 2);

    /**
     *	@brief	Set new UV coords in mesh
     *
     *	@param 	std::vector<float>& 	vector of new UV coords in form: x,y,z,x,y,z...
     *	@param 	3 	number of dimensions for UV
     */
    void setCoords(std::vector<float>&&, uint32_t = 2);


    /**
     *	@brief	Gets triangles buffer
     *
     */
    const index_array_type& getTriangles() const;
    index_array_type& getTriangles();
    const IndexData* getTrianglesHandle() const;
    IndexData* getTrianglesHandle();
    const index_array_type& getLines() const;
    index_array_type& getLines();
    const IndexData* getLinesHandle() const;
    IndexData* getLinesHandle();
    
    uint32_t getIndexSize();

    const index_array_type& getIndices() const;
    index_array_type& getIndices();

    /**
     *	@brief	Set new UV coords in mesh
     *
     *	@param 	std::vector<float>& 	vector of new UV coords in form: x,y,z,x,y,z...
     *	@param 	3
     */

    /**
     *	@brief	Set triangles buffer
     *
     *	@param 	std::vector<int16_t>& 	new triangles buffer
     */
    void setTriangles(const std::vector<indices_type>&);

    /**
     *	@brief	Set triangles buffer
     *
     *	
     */
    void setTriangles(std::vector<indices_type>&&);

    void setNextFrame(const std::vector<vertex_type>&, uint32_t = 3);

    void setNextFrame(std::vector<vertex_type>&&, uint32_t = 3);
 
   /**
     *	@brief	Builds hard edges mesh.
     *
     */
    void buildPerFacePerVertexNoramals();
    
   /**
     *	@brief	Rebuilds uv`s for texture Atlas.
     *
     */
    void rebuildCoordsForTpacker(int Posx, int Posy, int Sizex, int Sizey, int textsize);
    
   /**
     *	@brief	Adds reversed faces to mesh.
     *
     */
    void addReversedFaces();
    
   /**
     *	@brief	Adds barycentric coordinates to mesh.
     *
     */
    void addBarycentricCoordinates();
    
   /**
     *	@brief Reverse Faces of mesh.
     *
     */
    void reverseFaces();


    void setGeometryData(SimpleGeometryData& data);
    
    /**
     *	@brief	Get mesh name
     *
     */

    aex::string getName()
    {
        return m_name;
    }

    /**
     *	@brief	Get vertex buffers
     */
    const VertexBuffers& get_vertex_buffers() const
    {
        return m_vertexBuffers;
    }

    /**
     *	@brief	Get index buffers
     */
    const IndexBuffers& get_index_buffers() const
    {
        return m_indexBuffers;
    }


    /**
     *	@brief	Get square texture size
     */
    int16_t get_tex_size() const
    {
        return m_coordsSize;
    };

    /**
     *	@brief	Is mesh has wireframe
     *
     *	@return	bool
     */
    bool hasWireframe()
    {
        return m_wireframeDone;
    }

    /**
     *	@brief	Set wireframe done
     */
    void doneWireframe(bool wireframeDone = true)
    {
        m_wireframeDone = wireframeDone;
    }

    /**
     *	@brief Is mesh has UV coords
     *
     *	@return	bool
     */
    bool hasCoords()
    {
        return m_bCoords;
    }

    /**
     *	@brief	Set mesh has UV coords
     *
     *	@param 	bool
     */
    void useCoords(bool bCoords = true)
    {
        m_bCoords = bCoords;
    }

    /**
     *	@brief	Is mesh has normals
     *
     *	@return	bool
     */
    bool hasNormals()
    {
        return m_bNormals;
    }

    /**
     *	@brief	Set is mesh usinf normals
     *
     *	@param 	bool
     */
    void useNormals(bool bNormals = true)
    {
        m_bNormals = bNormals;
    }

    /**
     *	@brief	Is mesh packed
     *
     *	@return	bool
     */
    bool isPacked() const
    {
        return m_usePacked;
    }

    /**
     *	@brief	Set is mesh packed
     *
     *	@param 	bool default true
     */
    void usePacked(bool usePacked = true)
    {
        m_usePacked = usePacked;
    }

    /**
     *	@brief	Get bounding sphere of mesh
     *
     *	@param 	 	<# description#>
     *
     *	@return	BoundingSphere
     */
    BoundingSphere* getBoundingSphere()
    {
        return &m_bSphere;
    }

    /**
     *	@brief	Compute boundindg sphere for object
     *
     */
    void createBoundingSphere();

    /**
     *	@brief	Draws buffers of mesh in gl context (glDrawElements())
     *
     *	@param 	gl 	AexRenderer
     *	@param 	mode 	Draw mode(triangles)
     */
    void drawBuffers(AexRenderer* gl, int mode);

    /**
     *	@brief	Bind buffers of mesh in gl context
     *
     *	@param 	gl 	AexRenderer
     *	@param 	mode 	Draw mode(triangles)
     */
    void bindBuffers(AexRenderer* gl, int mode);

    /**
     *	@brief	draw function for render tree
     *
     *	@param 	gl 	pointer to AexRenderer
     */
    void draw(const AexRenderer* gl);

    /**
     *	@brief	draw function for render tree
     *
     *	@param 	gl 	pointer to AexRenderer
     */
    void draw(AexRenderer* gl);

    /**
     *	@brief	update function for render tree
     *
     *	@param 	gl 	pointer to AexRenderer
     */
    void update()
    {
    }

    void finishdraw(AexRenderer* gl);

     /**
     *	@brief	simple raycast on geometry
     *
     *	@param 	transform 	const aex::math::Matrix& transform matrix
     *  @param 	startPoint 	const aex::math::Vector3& startPoint
     *  @param 	dir             const aex::math::Vector3& direction
     */
    aex::math::Vector3 raycastMesh(const aex::math::Matrix& transform, const aex::math::Vector3& startPoint, const aex::math::Vector3& dir, aex::math::Vector3*  retMormal = nullptr );
    
    /**
     *	@brief	needsUpdate function for render tree
     *
     *	@param 	gl 	pointer to AexRenderer
     */
    void needsUpdate()
    {
        m_needsUpdate = true;
    }

    /**
     *	@brief	isVisible function for render tree
     *
     *	@param 	gl 	pointer to AexRenderer
     */
    bool isVisible()
    {
        return getIsVisible();
    }

    AABB getBounds()
    {
        return AABB();
    }

    AABB getBounds(AABBBuilder children)
    {
        return AABB(children);
    }

    AABB getChildBounds()
    {
        return AABB {};
    }

    void setMeshName(const aex::string& name)
    {
        m_name = name;
    }
    void setDrawMode(int drawMode);
    int getDrawMode() const;

    //Mesh i przyjaciele
    friend Mesh* plane(uint32_t, uint32_t, bool, bool);
    friend Mesh* sphere(uint32_t);
    friend Mesh* load(aex::string);
    friend class aex::ObjReader;
    friend class MeshAccessor;
    
    protected:
        aex::string m_name;
        VertexBuffers m_vertexBuffers;
        IndexBuffers m_indexBuffers;
        std::map<aex::string, int32_t> m_attributes;
        int32_t m_attrE;
        int32_t m_locs;
        uint32_t m_handleToTriangleBuffer;
        bool m_wireframeDone;
        bool m_bCoords;
        bool m_bNormals;
        bool m_compiled = false;

        bool m_usePacked;
        bool m_isBufferBinded;
        size_t m_trianglesBufferSize;

        uint32_t m_dim;

        float m_scaledUV;
        uint32_t m_spacing;
        int16_t m_coordsSize;
        int m_drawMode = GL_TRIANGLES;

        static std::atomic<uint32_t> m_namecounter;
        BoundingSphere m_bSphere;
        AABB m_aabb;
        std::stack<int32_t> m_enabledArrays;
        VertexCache m_vertexData;
        IndexCache m_indexData;

    
};
struct SimpleGeometryData{
    bool isHardEdged=true;
    Mesh::vertex_array_type vertices;
    Mesh::index_array_type  indices;
};

struct MeshPartInfo
{
    Mesh::indices_type offset = 0;
    Mesh::indices_type size = 0;
    MeshPartInfo();
    MeshPartInfo(Mesh::indices_type cOffset, Mesh::indices_type cSize);
};

class AEX_PUBLIC_API CombinedMesh : public Mesh
{
private:
    std::map<std::string, size_t> m_nameMap;
    std::map<size_t, MeshPartInfo> m_meshMap;
    
public:
    
    CombinedMesh();
    uint32_t addMesh(Mesh& mesh);
    uint32_t addMesh(Mesh* mesh);
    MeshPartInfo getMeshInfo(size_t infonumber);
};

Mesh* plane(uint32_t, uint32_t, bool, bool);
Mesh* sphere(uint32_t);


typedef shared_ptr<CombinedMesh> CombinedMesh_ptr;
typedef shared_ptr<Mesh> MeshShrd_Ptr; //To remove
typedef shared_ptr<Mesh> Mesh_ptr;

Mesh* load(aex::string);

std::vector<mesh_vectors_type> makeNormals(
    const std::vector<mesh_vectors_type>& vertices,
    const std::vector<mesh_indices_type>& indices
);
std::vector<mesh_vectors_type> makeFaceNormals
(
    const std::vector<mesh_vectors_type>& vertices,
    const std::vector<mesh_indices_type>& indices
);

SimpleGeometryData weldVertices
(
    const std::vector<mesh_vectors_type>& vertices,
    const std::vector<mesh_indices_type>& indices,
    float distance = aex::math::Math::EPSILON
);

}
//TODO PackedBuff, OBJ parser i pewnie jakies inne bzdety
#endif /* defined(__aexolGL__Mesh__) */
