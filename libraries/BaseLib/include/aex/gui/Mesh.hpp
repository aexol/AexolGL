#ifndef MESH_AEX_GUI_HEADER
#define MESH_AEX_GUI_HEADER

#include "TriangleBatch.hpp"
#include <aex/VertexData.hpp>
#include <aex/AexDrawable.hpp>

namespace aex {
    namespace gui {

        class MeshInterface {
        public:
            virtual void setDelta(const Position& delta) = 0;
            virtual void setRotationZ(unsigned degrees) = 0;
        };
        
        // defines what every mesh in gui must have
        class MeshBase : public MeshInterface
        {
        public:
            MeshBase();
            virtual ~MeshBase();
            
            virtual void setDelta(const Position& delta);
            virtual void setRotationZ(unsigned degrees);
            
            float getScale() const;
            float getRotDegZ() const;
            
        protected:
            aex::math::Vector2 m_frameSize;
            UniformFloat2 m_uFrame;
            UniformFloat2 m_uDelta;
            
            float m_scale;
            float m_rotDegreesZ;
        };

        class AEX_PUBLIC_API Mesh : public DrawObject, public MeshBase {
        public:
            Mesh(const TriangleBatch& batch);
            Mesh(Mesh& cm) = delete;
            Mesh(Mesh&& cm) = delete;

            virtual ~Mesh() {}
            void setBatch(const TriangleBatch& batch);
            const TriangleBatch& getBatch() const;
            virtual void batchChanged() = 0;

            virtual void draw(AexRenderer* gl) = 0;
            virtual void finishdraw(AexRenderer* gl) = 0;
            virtual void needsUpdate();
        protected:
            TriangleBatch m_batch;

            VertexData m_aVertex2;
            VertexData m_aUV2;
            IndexData m_indexes;

            unsigned long long m_counterek;
        };
    }
}

#endif