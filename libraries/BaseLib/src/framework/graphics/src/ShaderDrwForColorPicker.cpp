#include "../include/ApiInternal.hpp"
#include "../include/ContextInternal.hpp"

#include <aex/ShaderProgram.hpp>
#include <aex/ShaderDrwForColorPicker.hpp>
#include <aex/VertexData.hpp>
#include <aex/utils.hpp>
#include <aex/Logger.hpp>
namespace aex
{
    
    ShaderDrwForColorPicker::ShaderDrwForColorPicker() : ShaderDrw::ShaderDrw()
    {
    }

    ShaderDrwForColorPicker::ShaderDrwForColorPicker(const ShaderDrwForColorPicker& sd) :
        ShaderDrw(sd),
        m_pickerProgram(*sd.m_pickerProgram),
        m_drawProgram(*sd.m_drawProgram)
{
}
    ShaderDrwForColorPicker& ShaderDrwForColorPicker::operator=(const ShaderDrwForColorPicker& sd)
{
    if(this != &sd)
    {
        ShaderDrw::operator=(sd);
        m_pickerProgram = *sd.m_pickerProgram;
        m_drawProgram = *sd.m_drawProgram;
    }
    return *this;
}

    ShaderDrwForColorPicker::ShaderDrwForColorPicker(const aex::string& vertex_source, const aex::string& fragment_source, aex::string vertex_attr) :
    ShaderDrw::ShaderDrw(vertex_source, fragment_source, vertex_attr)
    {
#if __TARGET_AEX_API__ == __GLES__
        m_fragment_header = "#ifdef GL_FRAGMENT_PRECISION_HIGH\n precision highp float;\n#else\n precision mediump float;\n#endif\n" + m_header;
#endif
        build();
    }

    void
    ShaderDrwForColorPicker::build() const
    {
        ThreadSpecificData& tds = *m_currentThreadData;
        aex::string vertex_source = fix(m_vertex_header, m_vertex_lines);
        aex::string fragment_source = fix(m_fragment_header, m_fragment_lines);
        aex::string picker_source = fix(m_fragment_header,
                                             "uniform vec3 colorPick;\n"
                                             "void main(){\n"
                                             "	gl_FragColor = vec4(colorPick.r ,colorPick.g, colorPick.b, 1.0 );\n"
                                             "}");
#ifdef HAVE_GLES3_GL3_H
		GLSL_ES_3_0_Compatible(vertex_source, fragment_source);
                std::string null_str = "nullstring";
		GLSL_ES_3_0_Compatible( null_str , picker_source);
#endif

        tds.m_program = ShaderProgram::makeProgram();
        *m_pickerProgram = ShaderProgram::makeProgram();

        if(tds.m_program != 0 && (*m_pickerProgram) != 0)
        {

            auto shaderV = ShaderObject::makeVertexShader(vertex_source);
            auto shaderF = ShaderObject::makeFragmentShader(fragment_source);

            buildProgramObject(tds.m_program, shaderV, shaderF);
            auto shaderVp = ShaderObject::makeVertexShader(vertex_source);
            auto shaderFPicker = ShaderObject::makeFragmentShader(picker_source);
            buildProgramObject(*m_pickerProgram, shaderVp, shaderFPicker);
            *m_drawProgram = tds.m_program;
        }

        //this.isSampler = (String[])Shader.regexMap("uniform\\s+sampler(1D|2D|3D|Cube)\\s+(\\w+)\\s*;", vertexSource+fragmentSource, new Shader.SamplerChecker());
        tds.m_needs_mvp = (vertex_source.find("gl_ModelViewProjectionMatrix") != aex::string::npos) ||
            (fragment_source.find("gl_ModelViewProjectionMatrix") != aex::string::npos);
    }

    void ShaderDrwForColorPicker::setDebug(bool set)
    {
        m_debug= set;
    }


    void
    ShaderDrwForColorPicker::draw(AexRenderer* gl)
    {
        ThreadSpecificData& tds = *m_currentThreadData;
        if(!gl->isColorPick())
        {
            if(m_debug)
                LOG_DEBUG("gl->isColorPick() ", gl->isColorPick());
            tds.m_program = *m_drawProgram;
        }
        else
        {
            if(m_debug)
                LOG_DEBUG("gl->isColorPick() ", gl->isColorPick());
            m_refreshViewProjection=true;
            tds.m_program = *m_pickerProgram;
            colorMask(true, true, true, true);
        }

        ShaderDrw::draw(gl);
    }

    void
    ShaderDrwForColorPicker::finishdraw(AexRenderer* gl)
    {
        ShaderDrw::finishdraw(gl);
        if(gl->isColorPick())
        {
            colorMask(false, false, false, false);
        }
    }

}
