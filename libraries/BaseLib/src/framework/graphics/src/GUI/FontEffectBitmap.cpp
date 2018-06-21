#include "aex/gui/FontEffectBitmap.hpp"

namespace aex
{
    namespace gui
    {
        // if u wanna macro here use \n before it. eg: \n#define MACRO 123
        namespace {
            const char* ShaderName = "FontEffectBitmap";

            const char* Attribs = STRINGIFY(
                    attribute vec4 aVertex2; \n
                    attribute vec2 aUV; \n
                    );

            const char* VSH = STRINGIFY(
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

            const char* FSH = STRINGIFY(
                    uniform sampler2D uSampler; \n
                    uniform vec4 uColor;
                    varying vec2 vUV;
                    void main()
                    {
                        gl_FragColor = texture2D(uSampler, vUV) * uColor; \n
                    }
                );
        }

        FontEffectBitmap::FontEffectBitmap(std::unique_ptr<FontSource>&& fntSrc) :
            FontEffect(ShaderManager::get(ShaderName, VSH, FSH, Attribs), std::move(fntSrc), Color(255,255,255,255)),
            m_uColor(m_color.getClampfR(), m_color.getClampfG(), m_color.getClampfB(), m_color.getClampfA(), "uColor")
        {
        }

        FontEffectBitmap::~FontEffectBitmap()
        {
        }

        void FontEffectBitmap::draw(AexRenderer* gl)
        {
            FontEffect::draw(gl);
            m_shader->draw(gl);
            const AexShader& program = m_shader->GetProgram();

            m_sampler.setUniform(program);
            m_uFrame.setUniform(program);
            m_uDelta.setUniform(program);
            m_uColor.setUniform(program);

            m_texture->bind();

            program->setVertexData(m_aVertex2);
            program->setVertexData(m_aUV2);
            program->setIndices(m_indexes);

            program->draw();
        }

        void FontEffectBitmap::finishdraw(AexRenderer* gl)
        {
            const AexShader& program = m_shader->GetProgram();
            m_shader->finishdraw(gl);
            m_texture->unbind();
            program->releaseVertexData(m_aUV2);
            program->releaseVertexData(m_aVertex2);
        }

        void FontEffectBitmap::needsUpdate()
        {
        }

        void FontEffectBitmap::colorChanged()
        {
            m_uColor.setValue(m_color.getClampfR(), m_color.getClampfG(), m_color.getClampfB(), m_color.getClampfA());
        }
    }
}
