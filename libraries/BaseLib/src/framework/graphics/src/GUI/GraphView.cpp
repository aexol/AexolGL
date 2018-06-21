#include "aex/gui/GraphView.hpp"
#include "aex/gui/Frame.hpp"

namespace aex
{
    namespace gui
    {
        const float GraphView::SPEED_SECONDS = 0.5f;

        GraphView::GraphView(const Color& color, const GraphRanges& gr, float thickness, bool rolling, bool resizing) :
        m_graphRanges(gr),
        m_thickness(thickness),
        m_rolling(rolling),
        m_resizing(resizing),
        m_parts(100),
        m_dot(Area(Size(thickness * 5, thickness * 5)), color, Color(255, 255, 255), thickness * 5.0f, 100, 0)
        {
            m_mesh.reset(new ColorMesh(color, m_batch));
            m_dot.setStartPercent(0);
            m_dot.setPercent(100);
        }

        GraphView::~GraphView()
        {
        }

        void GraphView::add(const GPoint& p)
        {
            bool bezier = true;
            if (bezier)
            {
                if (m_data.size() < 3)
                {
                    m_data.push_back(p);
                    previous = Position(p.x, p.y);
                    return;
                }

                tmp = Position(p.x, p.y);

                float partWidth = m_graphRanges.xRange() / m_parts;
                const GPoint& pBef2 = m_data[m_data.size() - 2];
                const GPoint& pBef1 = m_data[m_data.size() - 1];

                Position p0 = Position(pBef2.x, pBef2.y);
                Position p1 = Position(pBef1.x, pBef1.y);
                Position p2 = previous;
                Position p3 = tmp;

                double retX = p2.x(), retY = p2.y();
                bool colide = lineIntersection(p0.x(), p0.y(), p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y(), &retX, &retY);
                if ((!colide) || (retX < p1.x()) || (retX > p2.x()))
                {
                    retX = p2.x();
                    retY = p2.y();
                }

                Position intersect3(retX, retY);
                int neededParts = ((p.x - m_data[m_data.size() - 1].x) / partWidth) + 1;

                for (int i = 1; i < neededParts; i++)
                {
                    float partClampf = i / (float) neededParts;
                    Position nextP = bezierPoint(p1, intersect3, p2, partClampf);
                    m_data.push_back(GPoint(nextP.x(), nextP.y()));
                }

                m_data.push_back(GPoint(previous.x(), previous.y()));
                previous = tmp;
            } else
            {
                m_data.push_back(p);
            }

            m_endPoint = graphToAreaCords(m_data[m_data.size() - 1], m_graphRanges, getInnerArea());
            setRefresh(true);
        }

        void GraphView::clear()
        {
            m_data.clear();
            setRefresh(true);
        }

        void GraphView::setParts(unsigned parts)
        {
            m_parts = parts;
        }

        void GraphView::recalcGraphics()
        {
            if (m_data.empty())
            {
                m_batch.clear();
                m_mesh->setBatch(m_batch);
                return;
            }

            float minX = m_graphRanges.fromX;
            float maxX = m_graphRanges.toX;
            float xRange = m_graphRanges.xRange();
            float endX = m_data[m_data.size() - 1].x;

            float minY = m_graphRanges.fromY;
            float maxY = m_graphRanges.toY;

            if (m_resizing)
            {
                minY = m_data[m_data.size() - 1].y;
                maxY = m_data[m_data.size() - 1].y;
                for (size_t i = 0; i < m_data.size(); i++)
                {
                    if ((endX - m_data[i].x) > xRange)
                        continue;

                    maxY = std::max(maxY, m_data[i].y);
                    minY = std::min(minY, m_data[i].y);
                }
            }

            GraphRanges gr(minX, maxX, minY, maxY);
            generateSimpleTrianglesBatch(gr);
        }

        void GraphView::generateSimpleTrianglesBatch(const GraphRanges& gr)
        {
            m_batch.clear();
            const GPoint& end = m_data[m_data.size() - 1];
            float xRange = gr.toX - gr.fromX;
            float firstX = end.x - xRange;

            for (size_t i = 0; i < m_data.size() - 1; i++)
            {
                if (m_data[i].x < firstX)
                {
                    if (m_data[i + 1].x < firstX)
                        continue;
                }
                if (m_data[i].x > firstX)
                {
                    if (m_data[i + 1].x < firstX)
                        continue;
                }

                Position p = graphToAreaCords(m_data[i], gr, getInnerArea());
                Position p2 = graphToAreaCords(m_data[i + 1], gr, getInnerArea());
                addLineToBatch(p, p2);
            }

            m_maxDelta = calcDelta(gr);
            m_maxDelta = m_maxDelta + Position(-0.05, 0);
            m_lastDelta = m_delta;
            m_delta = m_maxDelta; // quick fix, no lagodne przesuwanie TODO 


            m_dotPos = graphToAreaCords(m_data[m_data.size() - 1], gr, getInnerArea()) + m_maxDelta;
            m_dotPos = m_dotPos - Position(m_dot.getArea().s().w() / 2, m_dot.getArea().s().h() / 2); // center dot in point

            m_dot.setPosition(m_dotPos);
            m_dot.forceRefresh();

            m_mesh->setDelta(m_delta);
            m_mesh->setBatch(m_batch);
        }

        void GraphView::generateBezierTriangleBatch(const GraphRanges& gr)
        {
            if (m_data.size() < 3)
                return;

            m_batch.clear();
            Position delta = calcDelta(gr);
            Position befP;
            bool befPSetted = false;
            
            for (size_t i = 1; i <= m_parts; i++)
            {
                float partX = ((i / (float) m_parts) * getInnerArea().s().w()) + getInnerArea().p().x();
                partX -= delta.x();

                size_t id = 0;
                for (size_t j = 0; j < m_data.size() - 1; j++)
                {
                    Position tmpP = graphToAreaCords(m_data[j], gr, getInnerArea());
                    Position tmpPNext = graphToAreaCords(m_data[j + 1], gr, getInnerArea());

                    if ((partX >= tmpP.x()) && (partX <= tmpPNext.x()))
                    {
                        id = ((int) (j) / (int) 3) * 3; // 0, 3, 6, 9 itd
                        break;
                    }
                }

                Position p1 = graphToAreaCords(m_data[id], gr, getInnerArea());
                Position p2 = graphToAreaCords(m_data[(id + 1)<(m_data.size()) ? id + 1 : id], gr, getInnerArea());
                Position p3 = graphToAreaCords(m_data[(id + 2)<(m_data.size()) ? id + 2 : (id + 1)<(m_data.size()) ? id + 1 : id ], gr, getInnerArea());
                float partClampf;
                if ((p3.x() - p1.x()) > 0)
                {
                    partClampf = (partX - p1.x()) / (p3.x() - p1.x());
                } else
                {
                    partClampf = 0.0f;
                }

                Position p = bezierPoint(p1, p2, p3, partClampf);
                if (!befPSetted)
                {
                    befP = p;
                    befPSetted = true;
                }
                addLineToBatch(befP, p); // 100 rects
                befP = p;
            }

            m_mesh->setDelta(delta);
            m_mesh->setBatch(m_batch);
        }

        Position GraphView::graphToAreaCords(const GPoint& p, const GraphRanges& gr, const Area& a) const
        {
            float xRange = gr.toX - gr.fromX;
            float yRange = gr.toY - gr.fromY;

            float xPerc = (p.x - gr.fromX) / ((xRange > 0) ? xRange : (float) (p.x - gr.fromX));
            float yPerc = (p.y - gr.fromY) / ((yRange > 0) ? yRange : (float) (p.y - gr.fromY));

            float areaX = a.p().x() + (xPerc * a.s().w());
            float areaY = a.p().y() + (yPerc * a.s().h());

            return Position(areaX, areaY);
        }

        void GraphView::addLineToBatch(const Position& from, const Position& to)
        {
            float delta = Position::length(from, to);
            float smallDeltaX;
            float smallDeltaY;
            float bigDeltaX;
            float bigDeltaY;

            if (delta == 0.0)
            {
                bigDeltaX = to.x() - from.x();
                bigDeltaY = to.y() - from.y();
                smallDeltaX = bigDeltaY;
                smallDeltaY = bigDeltaX;
            } else
            {
                float cPerc = m_thickness / delta;
                bigDeltaX = to.x() - from.x();
                bigDeltaY = to.y() - from.y();
                smallDeltaX = cPerc * bigDeltaY;
                smallDeltaY = cPerc * bigDeltaX;
            }

            Position p1 = to;
            Position p2 = to;
            Position p3 = from;
            Position p4 = from;

            p1.move(-smallDeltaX, smallDeltaY);
            p2.move(smallDeltaX, -smallDeltaY);
            p3.move(-smallDeltaX, smallDeltaY);
            p4.move(smallDeltaX, -smallDeltaY);

            m_batch.addPoint(p3);
            m_batch.addPoint(p4);
            m_batch.addPoint(p1);
            m_batch.addPoint(p2);
        }

        Position GraphView::calcDelta(const GraphRanges& gr) const
        {
            Position delta;
            Position rightUp = getInnerArea().rightUp();
            if (m_endPoint.x() > rightUp.x())
            {
                delta.setX(rightUp.x() - m_endPoint.x());
                delta.setY(0);
            } else
            {
                delta = Position(0, 0);
            }
            return delta;
        }

        void GraphView::refresh(int flags)
        {
            recalcGraphics();
            m_dot.forceRefresh();
        }

        void GraphView::draw(aex::AexRenderer* gl)
        {
            if (m_data.size() > 3)
            {
                m_finishdraw = true;
                m_dot.draw(gl);
                m_dot.finishdraw(gl);
                m_mesh->draw(gl);
            } else
            {
                m_finishdraw = false;
            }
            m_timestamp = Frame::timestampSeconds();
        }

        void GraphView::finishdraw(aex::AexRenderer* gl)
        {
            if (m_finishdraw)
            {
                m_mesh->finishdraw(gl);
            }
        }

        void GraphView::needsUpdate()
        {
            m_mesh->needsUpdate();
        }

        ComponentPtr GraphView::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
        {
            return nullptr;
        }
    }
}