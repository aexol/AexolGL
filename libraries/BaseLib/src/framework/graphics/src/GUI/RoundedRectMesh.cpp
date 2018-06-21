#include "aex/gui/RoundedRectMesh.hpp"
#include "aex/gui/Frame.hpp"
#include "aex/gui/ShaderManager.hpp"
#include <aex/ShaderDrw.hpp>
#include <aex/VertexData.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/ShaderProgram.hpp>

namespace aex
{
    namespace gui
    {
        // if u wanna macro here use \n before it. eg: \n#define MACRO 123
        const char* RoundedRectMesh::ShaderName = "RoundedRectMesh";

        const char* RoundedRectMesh::Attribs = STRINGIFY(
                attribute vec4 aVertex2; \n
                );

        const char* RoundedRectMesh::VSH = STRINGIFY(
                uniform vec2 uFrame; \n
                uniform vec2 uDelta; \n
                varying vec4 vPosition; \n
                void main()
                {
                    vec4 vertex = aVertex2 + vec4(uDelta, 0.0, 0.0);
                    vPosition = vertex;

                    vec4 oglVertex = vertex;
                    oglVertex.x = 2.0 * (vertex.x / uFrame.x) - 1.0;
                    oglVertex.y = -(2.0 * (vertex.y / uFrame.y) - 1.0);
                    gl_Position = oglVertex;
                }
              );

        const char* RoundedRectMesh::FSH = STRINGIFY(
                uniform vec4 uColor; \n
                uniform vec2 uCenter; \n
                uniform float uCircleRadius; \n
                uniform vec2 uCenter2CircCenterDelta; \n
                uniform vec2 uCenterToCornerDelta; \n
                uniform float m_uBlurrPercent; \n
                varying vec4 vPosition; \n
                \n
                void main()     \n
                {     \n
                  float alpha = 1.0;     \n
                  float dist = distance(uCenter, vPosition.xy);     \n

                  vec2 deltas;     \n
                  deltas.x = (vPosition.x < uCenter.x) ? -uCenter2CircCenterDelta.x : uCenter2CircCenterDelta.x;     \n
                  deltas.y = (vPosition.y < uCenter.y) ? -uCenter2CircCenterDelta.y : uCenter2CircCenterDelta.y;     \n
                  vec2 circCenter = vec2(uCenter.x + deltas.x, uCenter.y + deltas.y);     \n

                  float blurrEdgeSize = (uCircleRadius * m_uBlurrPercent);

                  if((vPosition.y > uCenter.y) == (vPosition.y > circCenter.y) && (vPosition.x > uCenter.x) == (vPosition.x > circCenter.x) ) // correct
                  {
                    float dist2 = abs(distance(circCenter, vPosition.xy));     \n
                    if(dist2 <= uCircleRadius)
                    {
                        float edge1 = uCircleRadius - blurrEdgeSize;     \n
                        float edge2 = uCircleRadius;     \n
                        alpha = 1.0 - smoothstep(edge1, edge2, dist2);      \n
                    }
                    else
                    {
                        alpha = 0.0;
                    }
                 }else{
                   if( abs( uCenter.x - vPosition.x ) >= (uCenterToCornerDelta.x - blurrEdgeSize) )
                   {
                     float edge1 = 0.0 ;     \n
                     float edge2 = blurrEdgeSize;     \n
                     alpha = 1.0 - smoothstep(edge1, edge2, abs( uCenter.x - vPosition.x ) - (uCenterToCornerDelta.x - blurrEdgeSize) );      \n
                   }

                   if( abs( uCenter.y - vPosition.y ) >= (uCenterToCornerDelta.y - blurrEdgeSize) )
                   {
                     float edge1 = 0.0 ;     \n
                     float edge2 = blurrEdgeSize;     \n
                     alpha = 1.0 - smoothstep(edge1, edge2, abs( uCenter.y - vPosition.y ) - (uCenterToCornerDelta.y - blurrEdgeSize) );      \n
                   }

                 }

                  gl_FragColor = vec4(uColor.xyz, alpha);     \n
                }     \n
              );

        RoundedRectMesh::RoundedRectMesh(const Color& color, const TriangleBatch& batch, const Area& area, float circleRadius) :
        Mesh(batch),
        m_color(color),
        m_uColor(color.getClampfR(), color.getClampfG(),color.getClampfB(),color.getClampfA(), "uColor"),
        m_area(area),
        m_uCenter(aex::math::Vector2(area.center().x(), area.center().y()), "uCenter"),
        m_circleRadius(circleRadius),
        m_uCircleRadius(m_circleRadius, "uCircleRadius"),
        m_uCenter2CircCenterDelta(aex::math::Vector2(0, 0), "uCenter2CircCenterDelta"),
        m_uCenter2CornerDelta(aex::math::Vector2(0, 0), "uCenterToCornerDelta"),
        m_uBlurrPercent(0.05, "m_uBlurrPercent"),
        m_shaderHandle(ShaderManager::get(ShaderName, VSH, FSH, Attribs))
        {
            m_shader = m_shaderHandle.get();
            calcAndSetUniforms();
            batchChanged();
        }

        RoundedRectMesh::~RoundedRectMesh()
        {
        }

        void RoundedRectMesh::setColor(const Color& color)
        {
            m_color = color;
            m_uColor.setValue(color.getClampfR(), color.getClampfG(), color.getClampfB(), color.getClampfA());
        }

        const Color& RoundedRectMesh::getColor() const
        {
            return m_color;
        }

        void RoundedRectMesh::setCircleRadius(float circleRadius)
        {
            m_circleRadius = circleRadius;
            calcAndSetUniforms();
        }

        float RoundedRectMesh::getCircleRadius() const
        {
            return m_circleRadius;
        }

        void RoundedRectMesh::setAreaForUniforms(const Area& area)
        {
            m_area = area;
            calcAndSetUniforms();
        }

        const Area& RoundedRectMesh::getAreaForUniforms() const
        {
            return m_area;
        }

        void RoundedRectMesh::setBlurrPercent(float percent0_to_1)
        {
            m_blurrPercent = percent0_to_1;
            m_uBlurrPercent.setValue(m_blurrPercent);
        }

        float RoundedRectMesh::getBlurrPercent() const
        {
            return m_blurrPercent;
        }

        void RoundedRectMesh::batchChanged()
        {

        }

        void RoundedRectMesh::draw(AexRenderer* gl)
        {
            m_shader->draw(gl);
            const AexShader& program = m_shader->GetProgram();

            m_uFrame.setUniform(program);
            m_uDelta.setUniform(program);
            m_uColor.setUniform(program);
            m_uCenter.setUniform(program);
            m_uCircleRadius.setUniform(program);
            m_uCenter2CircCenterDelta.setUniform(program);
            m_uCenter2CornerDelta.setUniform(program);
            m_uBlurrPercent.setUniform(program);
            program->setVertexData(m_aVertex2);
            program->setIndices(m_indexes);
            program->draw();
        }

        void RoundedRectMesh::finishdraw(AexRenderer* gl)
        {
            const AexShader& program = m_shader->GetProgram();
            m_shader->finishdraw(gl);
            program->releaseVertexData(m_aVertex2);
        }

        void RoundedRectMesh::needsUpdate()
        {
        }

        void RoundedRectMesh::calcAndSetUniforms()
        {
            Position leftUpCircCenter = m_area.leftUp();
            leftUpCircCenter.move(m_circleRadius, m_circleRadius);

            float dX = m_area.center().x() - leftUpCircCenter.x();
            float dY = m_area.center().y() - leftUpCircCenter.y();

            m_uCenter.setValue(aex::math::Vector2(m_area.center().x(), m_area.center().y()));
            m_uCenter2CircCenterDelta.setValue(aex::math::Vector2(dX, dY));

            dX = m_area.center().x() - m_area.leftUp().x();
            dY = m_area.center().y() - m_area.leftUp().y();

            m_uCenter2CornerDelta.setValue(aex::math::Vector2(dX, dY));
        }
    }
}
