#include "aex/gui/TriangleBatch.hpp"
#include "aex/gui/Frame.hpp"

namespace aex
{
    namespace gui
    {

        TriangleBatch::TriangleBatch(bool genTriangleStrip) :
        m_genTriangleStrip(genTriangleStrip),
        m_flipAreaX(false),
        m_flipAreaY(false)
        {
        }

        TriangleBatch::TriangleBatch(const Area& area, bool genTriangleStrip) :
        m_genTriangleStrip(genTriangleStrip),
        m_flipAreaX(false),
        m_flipAreaY(false)
        {
            refillBatch(area);
        }

        TriangleBatch::TriangleBatch(const Area& area, const Area& UVs, bool genTriangleStrip) :
        m_genTriangleStrip(genTriangleStrip),
        m_flipAreaX(false),
        m_flipAreaY(false)
        {
            refillBatch(area, UVs);
        }

        TriangleBatch::~TriangleBatch()
        {
        }

        void TriangleBatch::refillBatch(const Area& area)
        {
            refillBatch(area, getFullTextureUVsArea());
        }

        void TriangleBatch::refillBatch(const Area& area, const Area& UVs)
        {
            clear();
            addArea(area, UVs);
        }

        void TriangleBatch::setFlipAreas(bool flipX, bool flipY)
        {
          m_flipAreaX = flipX;
          m_flipAreaY = flipY;
        }

        void TriangleBatch::addArea(const Area& area, const Area& UVs)
        {
            // Position uv1 = (m_flipAreaX) ? (m_flipAreaY) ? UVs.rightUp() : UVs.rightDown() : (m_flipAreaY) ? UVs.leftUp() : UVs.leftDown();
            Position uv1 = UVs.leftDown();
            Position uv2 = UVs.leftUp();
            Position uv3 = UVs.rightDown();
            Position uv4 = UVs.rightUp();

            if(m_flipAreaX && m_flipAreaY)
            {
              std::swap(uv1, uv4);
              std::swap(uv2, uv3);
            }else{
              if(m_flipAreaX)
              {
                std::swap(uv1, uv3);
                std::swap(uv2, uv4);
              }
              if(m_flipAreaY)
              {
                std::swap(uv1, uv2);
                std::swap(uv3, uv4);
              }
            }

            addPoint(area.leftUp(), uv1); // 1
            addPoint(area.leftDown(), uv2); // 2
            addPoint(area.rightUp(), uv3); // 3
            if (!m_genTriangleStrip)
            {
                //   addPoint(area.rightUp(), uv3); // 3 // TODO add changing indexes tab, dont clone points (50% of memory more in this caseis needed)
                //   addPoint(area.leftDown(), uv2); // 2
                m_indexes.push_back(m_points.size() - 1); // 3
                m_indexes.push_back(m_points.size() - 2); // 2
            }
            addPoint(area.rightDown(), uv4); // 4
        }

        void TriangleBatch::addPoint(const Position& p)
        {
            addUV();
            m_points.push_back(p);
            m_indexes.push_back(m_points.size() - 1);
        }

        void TriangleBatch::addPoint(const Position& p, const Position& UV)
        {
            m_uvData.push_back(UV);
            m_points.push_back(p);
            m_indexes.push_back(m_points.size() - 1);
        }

        std::vector<uint16_t> TriangleBatch::getIndexes() const
        {
            return m_indexes;
        }

        std::vector<float> TriangleBatch::getVBOData() const
        {
            std::vector<float> vboData;
            for (size_t i = 0; i < m_points.size(); i++)
            {
                vboData.push_back(m_points[i].x());
                vboData.push_back(m_points[i].y());
            }
            return vboData;
        }

        std::vector<float> TriangleBatch::getUVData() const
        {
            std::vector<float> uvData;
            for (size_t i = 0; i < m_uvData.size(); i++)
            {
                uvData.push_back(m_uvData[i].x()); // U
                uvData.push_back(m_uvData[i].y()); // V
            }
            return uvData;
        }

        void TriangleBatch::clear()
        {
            m_indexes.clear();
            m_points.clear();
            m_uvData.clear();
        }

        Area TriangleBatch::getFullTextureUVsArea()
        {
            return Area(Position(0, 0), Size(1, 1));
        }

        void TriangleBatch::addUV()
        {
            int mod = m_points.size() % 4;
            if (mod == 0)
            {
                m_uvData.push_back(Position(0.0f, 1.0f));
            }
            if (mod == 1)
            {
                m_uvData.push_back(Position(0.0f, 0.0f));
            }
            if (mod == 2)
            {
                m_uvData.push_back(Position(1.0f, 1.0f));
            }
            if (mod == 3)
            {
                m_uvData.push_back(Position(1.0f, 0.0f));
            }
        }

        float TriangleBatch::norm2OGL_X(float x) const
        {
            float percent = x / (Frame::getWidthNormal());
            return 2.0f * percent - 1.0f;
        }

        float TriangleBatch::norm2OGL_Y(float y) const
        {
            float percent = y / (Frame::getHeightNormal());
            return -(2.0f * percent - 1);
        }
    }
}
