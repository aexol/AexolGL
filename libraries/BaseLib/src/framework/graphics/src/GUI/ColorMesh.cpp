#include "aex/gui/ColorMesh.hpp"
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
        namespace {
            const char* ShaderName = "ColorMesh";

            const char* Attribs = STRINGIFY(
                        attribute vec4 aVertex2; \n
                    );

            const char* VSH = STRINGIFY(
                    uniform vec2 uFrame;
                    uniform vec2 uDelta;
                    void main()
                    {
                        vec4 vertex = aVertex2 + vec4(uDelta, 0.0, 0.0);
                        vec4 oglVertex = vertex;
                        oglVertex.x = 2.0 * (vertex.x / uFrame.x) - 1.0;
                        oglVertex.y = -(2.0 * (vertex.y / uFrame.y) - 1.0);
                        gl_Position = oglVertex;
                    }
                );

            const char* FSH = STRINGIFY(
                    uniform vec4 uColor;
                    void main()
                    {
                        gl_FragColor = vec4(uColor);
                    }
                );

        }
        // if u wanna macro here use \n before it. eg: \n#define MACRO 123
        ColorMesh::ColorMesh(const Color& color, const TriangleBatch& batch) :
            Mesh(batch),
            m_color(color),
            m_uColor(color.getClampfR(), color.getClampfG(),color.getClampfB(),color.getClampfA(), "uColor"),
            m_shaderHandle(ShaderManager::get(ShaderName, VSH, FSH, Attribs))
        {
            m_shader = m_shaderHandle.get();
            batchChanged();
        }

        ColorMesh::~ColorMesh()
        {
        }

        void ColorMesh::setColor(const Color& color)
        {
            m_color = color;
            m_uColor.setValue(color.getClampfR(), color.getClampfG(),color.getClampfB(),color.getClampfA());
        }

        const Color& ColorMesh::getColor() const
        {
            return m_color;
        }

        void ColorMesh::batchChanged()
        {

        }

        void ColorMesh::draw(AexRenderer* gl)
        {
            m_shader->draw(gl);
            const AexShader& program = m_shader->GetProgram();

            m_uColor.setUniform(program);
            m_uFrame.setUniform(program);
            m_uDelta.setUniform(program);
            program->setVertexData(m_aVertex2);
            program->setIndices(m_indexes);
            program->draw();
        }

        void ColorMesh::finishdraw(AexRenderer* gl)
        {
            const AexShader& program = m_shader->GetProgram();
            m_shader->finishdraw(gl);
            program->releaseVertexData(m_aVertex2);
        }

        void ColorMesh::needsUpdate()
        {
        }
    }
}
