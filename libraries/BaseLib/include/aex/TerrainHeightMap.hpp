/*
 * File:   Terrain.hpp
 * Author: mritke
 *
 * Created on November 27, 2013, 1:32 PM
 *
 */



#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <aex/Common.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/Mesh.hpp>
#include <aex/Aex.hpp>
#include <aex/Uniforms.hpp>
#include <aex/Material.hpp>

namespace aex {

    class AEX_PUBLIC_API TerrainQuadNode {
    private:
        std::array<aex::unique_ptr<TerrainQuadNode>, 4> m_children;
        AABB m_box;
        aex::unique_ptr<Aex> m_aex;
        uint32_t m_lvl = 0;
        bool m_drawThis = false;
        bool m_drawChilds = false;
        bool m_hasChilds = false;
        bool m_drawMax = false;
        UniformFloat2 m_sizeUni;
        UniformFloat m_uvSizeUni;
        UniformFloat2 m_uvOffsetUni;
        UniformFloat3 m_colorUni;
        
        
    protected:

    public:
        TerrainQuadNode();
        TerrainQuadNode(aex::math::Vector2 a, aex::math::Vector2 b, uint32_t lvl);
        TerrainQuadNode(aex::math::Vector2 a, aex::math::Vector2 b, uint32_t lvl, aex::math::Vector2 size, float m_topSize);
        ~TerrainQuadNode();

        TerrainQuadNode& operator=(aex::TerrainQuadNode& b);

        virtual void set_range(aex::math::Vector2 a, aex::math::Vector2 b, float height);
        virtual void split(uint max_lvl, aex::math::Vector2 size, float m_topSize);
        virtual void draw(AexRenderer* gl);
        virtual void drawThis(AexRenderer* gl);
        virtual void drawNodes(AexRenderer* gl);
        virtual bool update(QuaternionCamera* cam, float topSize);
        void setBox(AABB box);
        AABB getBox() const;
        void setSize(aex::math::Vector2 size);
        aex::math::Vector2 getSize() const;
        void  setColor(float r,float g,float b);
        void setDrawMax(bool drawMax);
    };

    class QuaternionCamera;

    void generatePlane(Mesh* mesh, int segments, bool triangleStrip, bool addSkirt=false);

    template<class T> void delete_array(T* ptr) {
        delete [] ptr;
    }
    template<class T>
    using array_pointer = std::unique_ptr<T, void(*)(T*)>;

    template<class T>
    array_pointer<T> make_array_pointer(T* ptr) {
        return array_pointer<T>(ptr, delete_array<T>);
    }

    class AEX_PUBLIC_API QuadTreeTerrain : public DrawObject {
    public:
        QuadTreeTerrain();
        QuadTreeTerrain(QuaternionCamera* m_quatCamera, aex::math::Vector2 topSize, int gridSize = 50, int depth = 4);
        virtual ~QuadTreeTerrain();

        virtual void update();
        virtual void init();
        virtual void draw(AexRenderer* gl);
        virtual void finishdraw(AexRenderer* gl);
        virtual void needsUpdate();

        QuadTreeTerrain& operator=(aex::QuadTreeTerrain& b);
        QuadTreeTerrain& operator=(aex::QuadTreeTerrain&& b);

        float getHeightAtCoordinates(aex::math::Vector2 coords);
        float getHeightSize() const;
        float getEdgeSize() const;
        Material* getMaterial() const;
        uint* getData();
        float* getRawHeightData();
        void setHeightMap(Texture* heightMap);
        void setHeightNormalMap(Texture* heightMap);
        void setQuatCamera(QuaternionCamera* quatCamera);
        QuaternionCamera* getQuatCamera() const;
        void setGridSize(int topGridSize);
        int getGridSize() const;
        int getTextureSizeW() const;
        int getTextureSizeH() const;
        void setHeightOffset(float heightOffset);
        float getHeightOffset() const;
        std::string getTextureName() const;
        void setDrawMax(bool drawmax);

    private:
        QuaternionCamera* m_quatCamera;
        aex::unique_ptr<TerrainQuadNode> m_topNode;
        aex::unique_ptr<Material> m_topMaterial;
        aex::unique_ptr<Mesh> m_plane;
        Texture* m_heightMap = nullptr;
        Texture* m_heightNormalMap = nullptr;
        Aex m_rootAex;

        int m_gridSize = 50;
        int m_depth = 4;
        aex::math::Vector2 m_topSize;
        float m_LODFactor;

        UniformSampler m_heightMapUniform;
        UniformSampler m_heightNormalMapUniform;
        UniformFloat m_heightOffsetUniform;
        array_pointer<uint> m_data;
        std::vector<float> m_rawHeightData;
    protected:

    };

    class AEX_PUBLIC_API TerrainHeightMap {

    public:
        TerrainHeightMap();
        ~TerrainHeightMap();

        void normalizeMap();
       

        void setSize(int size);
        int  getSize() const;

        void generateVboTriangles(MeshShrd_Ptr mesh);
        void generateSkydomeVbo(MeshShrd_Ptr mesh);

    private:

        int m_size;
        std::vector<std::vector<int>> m_heightMap;
        float** m_heightMapFloat;
    };


}

#endif /* TERRAIN_HPP */

