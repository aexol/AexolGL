#ifndef GRAPH_VIEW_AEX_GUI_HEADER
#define GRAPH_VIEW_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Component.hpp"
#include "TriangleBatch.hpp"
#include "ColorMesh.hpp"
#include "CircleBar.hpp"

namespace aex {
    namespace gui {

        struct GPoint {

            GPoint(float x, float y) : x(x), y(y) {
            }
            float x, y;
        };

        struct GraphRanges {

            GraphRanges(float fromX, float toX, float fromY, float toY) :
            fromX(fromX),
            toX(toX),
            fromY(fromY),
            toY(toY) {
            }
            float fromX;
            float toX;
            float fromY;
            float toY;

            float xRange() const {
                return toX - fromX;
            }

            float yRange() const {
                return toY - fromY;
            }
        };

        class AEX_PUBLIC_API GraphView : public Component {
        public:
            static const float SPEED_SECONDS;
            GraphView(const Color& color, const GraphRanges& gr, float thickness = 0.005f, bool rolling = true, bool resizing = true);
            virtual ~GraphView();
            void add(const GPoint& p);
            void clear();
            void setParts(unsigned parts);
        protected:
            void recalcGraphics();

            virtual void draw(aex::AexRenderer* gl);
            virtual void finishdraw(aex::AexRenderer* gl);
            virtual void needsUpdate();
            virtual void refresh(int flags);
        protected:
            virtual ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);
            void generateSimpleTrianglesBatch(const GraphRanges& gr);
            void generateBezierTriangleBatch(const GraphRanges& gr);
            Position graphToAreaCords(const GPoint& p, const GraphRanges& gr, const Area& a) const;
            void addLineToBatch(const Position& p1, const Position& p2);
            Position calcDelta(const GraphRanges& gr) const;
        protected:
            GraphRanges m_graphRanges;
            float m_thickness;
            bool m_rolling;
            bool m_resizing;
            TriangleBatch m_batch;
            std::unique_ptr<ColorMesh> m_mesh;
            std::vector<GPoint> m_data;
            unsigned m_parts;
            Position previous;
            Position tmp;
            CircleBar m_dot;
            Position m_dotPos;
            Position m_delta;
            Position m_lastDelta;
            Position m_maxDelta;
            double m_timestamp;
            Position m_endPoint;
            bool m_finishdraw;
        };
    }
}

#endif