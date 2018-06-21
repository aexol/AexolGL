/*
 * File:   Sprite.hpp
 * Author: mritke
 *
 * Created on December 4, 2013, 4:11 PM
 */
#ifndef SPRITE_HPP
#define	SPRITE_HPP


#include <aex/Common.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/TextureIncludes.hpp>
#include <aex/ShaderDrw.hpp>
#include <aex/VertexData.hpp>
#include <array>

namespace aex
{

    class Sprite;
    class VertexData;
    class Mesh;

    typedef shared_ptr<Sprite> Sprite_ptr;

    class AEX_PUBLIC_API Sprite : public DrawObject
    {
    public:

        static Sprite_ptr makeSprite(Texture* tex);
        Sprite(Texture* tex);
        Sprite();
        ~Sprite();
        virtual void draw(AexRenderer* gl);

        virtual void finishdraw(AexRenderer* gl);

        virtual void needsUpdate()
        {
        }
        void SetAnchorCenter();
        void SetShape(const std::vector<mesh_indices_type>& indices, const std::vector<mesh_vectors_type>& vertices, const std::vector<mesh_vectors_type>& texcoords);
        void SetShape(Mesh* mesh);
        void SetShape(const VertexData* vData);
        void ScaleVerts(float scale);
        void SetNewUV(std::vector<float>& UV);
        void setQuatCamera(const aex::shared_ptr<aex::QuaternionCamera>& camera);
        void setQuatCamera(aex::QuaternionCamera* camera);
        QuaternionCamera& getQuatCamera() const;
        void setSprite(Texture* Sprite);
        void setSprite(const aex::string& SpriteFile);
        Texture* getSprite() const;
        void setCanChangeDepthTestState(bool canChangeState);
        bool isCanChangeDepthTestState() const;
        /*void setShader(ShaderDrwShrd_Ptr shader);
        ShaderDrwShrd_Ptr getShader();*/
        void setNeedOwnShader(bool needOwnShader);
        bool isNeedOwnShader() const;
        void setSpriteCount(uint32_t spriteCount);
        void setPositions(const std::vector<aex::math::Vector3>& positions);
        int getPointPerSpriteCount();
        void setTriangleStrip(bool triangleStrip);
        bool isTriangleStrip() const;
        void setCanChangeBlendState(bool canChangeBlendState);
        bool isCanChangeBlendState() const;
        
    protected:
        aex::unique_ptr<VertexData> m_positions;
        aex::unique_ptr<VertexData> m_uvs;
        IndexData m_indices;
        const VertexData* m_customPosition;
        const VertexData* m_customUVs;
        const IndexData* m_customIndices;
        uint32_t m_CamPosUniform;
        uint32_t m_ProjectionMatrixUniform;
        uint32_t m_ViewMatrixUniform;
        uint32_t m_spriteCount;

        UniformMatrix4fv m_viewMatrixU;
        UniformMatrix4fv m_ProjectionMatrixU;
        aex::shared_ptr<UniformSampler> m_spriteSamplerUniform;

        bool m_isDepthTestEnabled;
        bool m_isDepthTestEnabledChange;
        bool m_triangleStrip;
        bool m_canChangeDepthTestState;
        bool m_canChangeBlendState;
        bool m_blendStatechanged;
        Texture* m_Sprite;
        //ShaderDrwShrd_Ptr m_shader;
        bool m_shaderLoaded;
        bool m_needOwnShader = true;
        bool m_pojectionSet = false;

        std::vector<float> m_batchVertCache;
        std::vector<float> m_batchUVCache;
        std::vector<uint16_t> m_batchIndexCache;
        std::vector<float> m_baseVertCache;
        std::vector<float> m_baseUVCache;
        std::vector<uint16_t> m_baseIndexCache;

        QuaternionCamera* m_quatCamera;

        int temptext = 0;
        static const std::array<float, 12> SimpleSpriteVertBottomRoot;
        static const std::array<float, 12> SimpleSpriteVertCenterRoot;
        static const std::array<float, 8> SimpleSpriteUV;
        static const std::array<uint16_t, 6> SimpleSpriteIndex;
        static const std::array<uint16_t, 4> SimpleSpriteIndexTriangleStrip;
        void setSpritesScaled(float scale);
        void setVertexData();
        void setVertexData(const VertexData* vData);
        void copyVertData(const std::vector<float>& vertices);
        void copyVertData(const std::array<float, 12>& vertices);
        void setUVData();
        void setUVData(const VertexData* uvData);
        void copyUVData(const std::vector<float>& uvs);
        void copyUVData(const std::array<float, 8>& uvs);
        void setIndexData();
        void setIndexData(const IndexData* uvData);
        void copyIndexData(const std::vector<uint16_t>& index);
        void copyIndexData(const std::array<uint16_t, 6>& index);
        void copyIndexData(const std::array<uint16_t, 4>& index);
    };

    typedef shared_ptr<Sprite> Sprite_ptr;



}

#endif	/* SPRITE_HPP */

