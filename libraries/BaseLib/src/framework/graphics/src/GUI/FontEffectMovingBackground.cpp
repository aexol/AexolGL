#include "aex/gui/FontEffectMovingBackground.hpp"

namespace aex
{
    namespace gui
    {
        namespace {
            const char* ShaderName = "FontEffectMovingBackground";

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
                    uniform sampler2D uBackTex;
                    uniform vec2 uBackDelta;
                    uniform vec4 uColor;
                    varying vec2 vUV;
                    void main()
            {
                float alfa = texture2D(uSampler, vUV).a * texture2D(uBackTex, vUV+uBackDelta).a;
                gl_FragColor = vec4(texture2D(uBackTex, vUV+uBackDelta).rgb, alfa) * uColor; \n
            }
            );
        }
        // if u wanna macro here use \n before it. eg: \n#define MACRO 123

        FontEffectMovingBackground::FontEffectMovingBackground(std::unique_ptr<FontSource> fntSrc, const std::string& backgroundTexturePath) :
        FontEffect(ShaderManager::get(ShaderName, VSH, FSH, Attribs), std::move(fntSrc), Color(255,255,255,255)),
        m_backgroundTexturePath(backgroundTexturePath),
        m_backTexHandler(aex::gui::TextureManager::get(backgroundTexturePath, TextureConfig::makeDefaultTextureConfig())),
        m_uBackDelta(aex::math::Vector2(0.0f, 0.0f), "uBackDelta"),
        m_uColor(m_color.getClampfR(), m_color.getClampfG(), m_color.getClampfB(), m_color.getClampfA(), "uColor")
        {
            m_deltaX = 0;
            m_deltaY = 0.5;
            m_modificatorX = 0.001;
            m_modificatorY = -0.001;
            m_backTex = m_backTexHandler.get();
            m_uBackTexSampler = UniformSampler(m_backTex, "uBackTex");
        }

        FontEffectMovingBackground::~FontEffectMovingBackground()
        {
        }

        void FontEffectMovingBackground::draw(AexRenderer* gl)
        {
            FontEffect::draw(gl);
            if(m_deltaX > 0.5)
            {
                m_modificatorX = -0.001;
            }
            if(m_deltaX < -0.5)
            {
                m_modificatorX = 0.001;
            }
            if(m_deltaY > 0.5)
            {
                m_modificatorY = -0.001;
            }
            if(m_deltaY < -0.5)
            {
                m_modificatorY = 0.001;
            }
//            m_modificatorX *= 2;
//            m_modificatorY *= 2;
            m_deltaX += m_modificatorX;
            m_deltaY += m_modificatorY;

            m_uBackDelta.setValue(m_deltaX, m_deltaY);

            m_shader->draw(gl);
            const AexShader& program = m_shader->GetProgram();

            m_sampler.setUniform(program);
            m_uBackTexSampler.setUniform(program);
            m_uFrame.setUniform(program);
            m_uDelta.setUniform(program);
            m_uBackDelta.setUniform(program);

            m_texture->bind();
            m_backTex->bind();

            program->setVertexData(m_aVertex2);
            program->setVertexData(m_aUV2);
            program->setIndices(m_indexes);

            program->draw();
        }

        void FontEffectMovingBackground::finishdraw(AexRenderer* gl)
        {
            const AexShader& program = m_shader->GetProgram();
            m_shader->finishdraw(gl);
            m_texture->unbind();
            m_backTex->unbind();
            program->releaseVertexData(m_aUV2);
            program->releaseVertexData(m_aVertex2);
        }

        void FontEffectMovingBackground::needsUpdate()
        {
        }

        void FontEffectMovingBackground::colorChanged()
        {
            m_uColor.setValue(m_color.getClampfR(), m_color.getClampfG(), m_color.getClampfB(), m_color.getClampfA());
        }
    }
}
