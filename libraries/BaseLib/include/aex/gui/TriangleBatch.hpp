#ifndef TRIANGLEBATCH_AEX_GUI_HEADER
#define TRIANGLEBATCH_AEX_GUI_HEADER

#include "GUIMath.hpp"

namespace aex {
    namespace gui {

        class AEX_PUBLIC_API TriangleBatch {
        public:
            TriangleBatch(bool genTriangleStrip = true);
            TriangleBatch(const Area& area, bool genTriangleStrip = true);
            TriangleBatch(const Area& area, const Area& UVs, bool genTriangleStrip = true);
            virtual ~TriangleBatch();

            void refillBatch(const Area& area);
            void refillBatch(const Area& area, const Area& UVs);

            void addArea(const Area& area, const Area& UVs);

            void addPoint(const Position& p);
            void addPoint(const Position& p, const Position& UV);
            std::vector<uint16_t> getIndexes() const;
            std::vector<float> getVBOData() const;
            std::vector<float> getUVData() const;
            void clear();
            void setFlipAreas(bool flipX, bool flipY);

            static Area getFullTextureUVsArea();
        protected:
            void  addUV();
            float norm2OGL_X(float x) const;
            float norm2OGL_Y(float y) const;
        protected:
            std::vector<Position> m_points;
            std::vector<Position> m_uvData;
            std::vector<uint16_t> m_indexes;
            bool m_genTriangleStrip;
            bool m_flipAreaX;
            bool m_flipAreaY;
        };
    }
}

#endif
