#include "aex/gui/TextureMesh.hpp"
#include "aex/gui/TriangleBatch.hpp"
#include "aex/gui/Frame.hpp"
#include "aex/gui/ShaderManager.hpp"
#include <aex/ShaderDrw.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/Texture.hpp>


namespace aex
{
    namespace gui
    {
        // if u wanna macro here use \n before it. eg: \n#define MACRO 123

        const char* TextureMesh::ShaderName = "TextureMesh";

        const char* TextureMesh::Attribs = STRINGIFY(
        attribute vec4 aVertex2; \n
        attribute vec2 aUV;  \n
        );
        const char* TextureMesh::VSH = STRINGIFY(
                uniform vec2 uFrame;
                uniform vec2 uDelta;
                varying vec2 vUV;
                void main()
        {
            vUV = aUV;
                    vec4 vertex = aVertex2 + vec4(uDelta, 0.0, 0.0);
                    vec4 oglVertex = vertex;
                    oglVertex.x = 2.0 * (vertex.x / uFrame.x) - 1.0;
                    oglVertex.y = -(2.0 * (vertex.y / uFrame.y) - 1.0);
                    gl_Position = oglVertex;
        }
        );

        const char* TextureMesh::FSH = STRINGIFY(
                uniform sampler2D uSampler; \n
                uniform vec4 uColorMod; \n
                varying vec2 vUV;
                void main()
        {
            gl_FragColor = texture2D(uSampler, vUV) * uColorMod; \n
        }
        );

        TextureMesh::TextureMesh(Texture* texture, const TriangleBatch& batch) :
        Mesh(batch),
        m_texture(texture),
        m_sampler(texture, "uSampler"),
        m_colorMod(255,255,255,255),
        m_uColorMod(1,1,1,1, "uColorMod"),
        m_shaderHandle(ShaderManager::get(ShaderName, VSH, FSH, Attribs))
        {
            m_shader = m_shaderHandle.get();
            batchChanged(); // because in contructors virtual functions doesnt works
        }

        TextureMesh::~TextureMesh()
        {
        }

        void TextureMesh::setTexture(Texture* texture)
        {
            m_texture = texture;
            m_sampler.setValue(texture);
        }

        void TextureMesh::setColorMod(const Color& color)
        {
          m_colorMod = color;
          m_uColorMod.setValue(color.getClampfR(), color.getClampfG(),color.getClampfB(),color.getClampfA());
        }

        const Color& TextureMesh::getColorMod() const
        {
          return m_colorMod;
        }

        void TextureMesh::batchChanged()
        {
            m_aUV2 = VertexData::makeVertexData(m_batch.getUVData(), "aUV", 2);
        }

        void TextureMesh::draw(AexRenderer* gl)
        {
            m_shader->draw(gl);
            const AexShader& program = m_shader->GetProgram();

            m_texture->bind();

            m_sampler.setUniform(program);
            m_uColorMod.setUniform(program);
            m_uFrame.setUniform(program);
            m_uDelta.setUniform(program);

            program->setVertexData(m_aVertex2);
            program->setVertexData(m_aUV2);
            program->setIndices(m_indexes);
            program->draw();
        }

        void TextureMesh::finishdraw(AexRenderer* gl)
        {
            const AexShader& program = m_shader->GetProgram();
            m_shader->finishdraw(gl);
            m_texture->unbind();
            program->releaseVertexData(m_aUV2);
            program->releaseVertexData(m_aVertex2);
        }

        void TextureMesh::needsUpdate()
        {
        }
    }
}
