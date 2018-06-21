#include "aex/gui/Mesh.hpp"
#include "aex/gui/Frame.hpp"

namespace aex
{
    namespace gui
    {

        MeshBase::MeshBase() :
        m_frameSize(Frame::getWidthNormal(), Frame::getHeightNormal()),
        m_uFrame(m_frameSize, "uFrame"),
        m_uDelta(aex::math::Vector2(0.0f, 0.0f), "uDelta")
        {
        }
        
        MeshBase::~MeshBase()
        {
        }
        
        void MeshBase::setDelta(const Position& delta)
        {
            m_uDelta.setValue(delta.x(), delta.y());
        }

        void MeshBase::setRotationZ(unsigned degrees)
        {
            m_rotDegreesZ = degrees;
        }
        
        float MeshBase::getScale() const
        {
            return m_scale;
        }

        float MeshBase::getRotDegZ() const
        {
            return m_rotDegreesZ;
        }
        
        Mesh::Mesh(const TriangleBatch& batch) :
        m_batch(batch),
        m_aVertex2(VertexData::makeVertexData(m_batch.getVBOData(), "aVertex2", 2)),
        m_indexes
        {
            IndexData::createTraingleStrip(m_batch.getIndexes())
        }
        {
            m_counterek = 0;
        }

        void Mesh::setBatch(const TriangleBatch& batch)
        {
            m_batch = batch;
            m_aVertex2 = VertexData::makeVertexData(m_batch.getVBOData(), "aVertex2", 2);
            m_indexes = IndexData::createTraingleStrip(m_batch.getIndexes());
            batchChanged();
        }

        const TriangleBatch& Mesh::getBatch() const
        {
            return m_batch;
        }

        void Mesh::needsUpdate()
        {

        }
    }
}