//
//  ShaderDrw.cpp
//  AexolGL
//
//  Created by Kamil Matysiewicz on 22.10.2013.
//
//

#include "../include/ApiInternal.hpp"
#include <aex/ShaderDrw.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/ShaderDrwForColorPicker.hpp>
#include <aex/utils.hpp>
#include <aex/Logger.hpp>
#include <aex/FileManager.hpp>
#include <aex/GPUApi.hpp>
//#include <aex/Common.hpp>

namespace aex
{

    aex::string insertAtBegin(const aex::string& expr, aex::string source)
    {
        aex::regex match_ext("(^(uniform|varying|struct|void|float|vec4|vec3|vec2|int|mat4|mat3)+)"); //find first uniform

        aex::string::const_iterator start, end;

        start = source.begin();
        end = source.end();

        aex::smatch what;
        size_t match_len = 0;
        aex::match_flag_type flags = aex::match_default;

        //s = boost::match_default | boost::match_not_dot_newline;
        while(aex::regex_search(start, end, what, match_ext, flags))
        {
            start = what[0].second;

            match_len = (start - source.begin()) - what.length();
            //LOG_DEBUG("match_len: ", match_len);
            //LOG_DEBUG("what: ", what.str());
            break;
        }

        return  source.substr(0, match_len) + '\n' + expr + source.substr(match_len, source.length());
    }

        void addAtBegginAfterVersion(aex::string& shaderSrc , aex::string& insertPart)
    {
        aex::regex match_ver("#version(.*)(\n+)");
        aex::smatch where;
        aex::match_flag_type flags = aex::match_default;
        aex::string::const_iterator start, end;
        
        start = shaderSrc.begin();
        end = shaderSrc.end();
        
        if( aex::regex_search(start, end, where, match_ver, flags) )
        {
           aex::string match= where[0];
           shaderSrc= shaderSrc.substr(0, match.size()) + '\n' + insertPart + shaderSrc.substr(match.size(), shaderSrc.length());
        }
        else
        {
            shaderSrc= insertPart+shaderSrc;
        }
         
    }
    
    void insertIosTextureFunc(std::string& fragmentSrc) {

    std::string androidExtensionDef = ("#ifdef ANDROID \n"
            "#extension GL_OES_EGL_image_external : require\n"
            "#endif \n");

    fragmentSrc = insertAtBegin(androidExtensionDef, fragmentSrc);

    std::string iosDefs = ("uniform sampler2D textureY;\n"
            "uniform sampler2D textureUV;\n");


    std::string androidDefs = "uniform samplerExternalOES textureOES;\n";

    std::string iosFuncDef = ("vec4 texture2DCameraIOS(vec2 uv)\n"
            "{ \n"
            " vec3 yuv; \n"
            "   yuv.x  = texture2D(textureY, uv).r;\n"
            "   yuv.yz = texture2D(textureUV, uv).rg - vec2(0.5, 0.5);\n"
            "   vec3 rgb = mat3(1,1,1,  0,-.18732,1.8556,   1.57481,-.46813,0) * yuv;\n" // SDTV = rgb = mat3(1,1,1, 0, -.34413, 1.772, 1.402, -.71414, 0) * yuv;\n
            "   return vec4(rgb, 1);\n"
            "}\n"
            "void main")
            ;

    std::string iosUseFunc = "texture2DCameraIOS(";

    aex::regex match_androidDefs(androidDefs);
    aex::regex match_voidMain("void main");
    aex::regex match_texture2D("texture2D[\\(]\\s*textureOES\\s*,");

    fragmentSrc = aex::regex_replace(fragmentSrc, match_androidDefs, iosDefs);
    fragmentSrc = aex::regex_replace(fragmentSrc, match_voidMain, iosFuncDef);
    fragmentSrc = aex::regex_replace(fragmentSrc, match_texture2D, iosUseFunc);
}
    
    aex::string ShaderDrw::fix(const aex::string& header, aex::string source)
    {

        source = insertAtBegin(header,source);

        aex::regex match_uniforms("(\\b|\\t)gl_[[:alnum:]]+(\\n|\\b)");

        aex::string::const_iterator start, end;

        start = header.begin();
        end = header.end();

        aex::smatch what;
        aex::match_flag_type flags = aex::match_default;
        while(aex::regex_search(start, end, what, match_uniforms, flags))
        {
            aex::string match = what[0];
            start = what[0].second;
            size_t found = 0;
            found = source.find(match, found);

            while(found != aex::string::npos)
            {
                source.insert(found, 1, '_');
                found += match.length() + 1;
                found = source.find(match, found);
            }
        }
        return source;
    }
    
    aex::string cookExtensions(std::vector<Extension> m_extensions)
    {
        aex::string ret;
        
        for(Extension& ext : m_extensions)
        {
            ret += "#extension "+ ext.extensionName +" : "+ ext.extensionBehavior+ "\n";
        }
        
        return ret;
    }

    void GLSL_ES_3_0_Compatible(std::string& vertexSrc, std::string& fragmentSrc)
    {
	//LOG_DEBUG("GLSL_ES_3_0_Compatible");
        aex::regex match_ver("#version");

        if(!aex::regex_search(vertexSrc.begin(), vertexSrc.end(),match_ver))
        {
            vertexSrc= "#version 300 es\n"+vertexSrc;
        }

        if(!aex::regex_search(fragmentSrc.begin(), fragmentSrc.end(),match_ver))
        {
            fragmentSrc= "#version 300 es\n"+fragmentSrc;
        }


        aex::regex match_attr("attribute");
        aex::regex match_var("varying");
        aex::regex match_texture("texture((\\dD)|(Cube))");


        std::string out= "out";
        std::string in= "in";
        std::string texture= "texture";

        vertexSrc = aex::regex_replace(vertexSrc, match_attr, in);
	vertexSrc = aex::regex_replace(vertexSrc ,match_var, out);
        vertexSrc = aex::regex_replace(vertexSrc ,match_texture, texture);

        std::string fragcolor= "fragmentColor";
        std::string fragdata= "fragmentData";

        std::string fragment_def = "out vec4 fragmentColor;\n";
        std::string fragmentData_def = "out vec4 fragmentData";

        aex::regex match_frag("gl_FragColor");


        aex::regex match_fragmentData("gl_FragData\\[\\d\\]");

	fragmentSrc = aex::regex_replace(fragmentSrc ,match_var, in);
	fragmentSrc = aex::regex_replace(fragmentSrc ,match_texture, texture);
        std::string multipledataoutput="";
        int dataNumber=0;
        while(aex::regex_search(fragmentSrc.begin(), fragmentSrc.end(),match_fragmentData))
        {
            multipledataoutput += fragmentData_def + aex::to_string(dataNumber) + ";\n";
            fragmentSrc = aex::regex_replace(fragmentSrc ,match_fragmentData, fragdata + aex::to_string(dataNumber));
            ++dataNumber;
        }

        if(multipledataoutput.size()>0)
        {
            fragmentSrc = insertAtBegin(multipledataoutput,fragmentSrc);
        }

        if(aex::regex_search(fragmentSrc.begin(), fragmentSrc.end(),match_frag))
        {
            fragmentSrc = insertAtBegin(fragment_def,fragmentSrc);
            fragmentSrc = aex::regex_replace(fragmentSrc ,match_frag, fragcolor);
        }
    }

/*
 TO-DO Shadery sa tak naprawde statyczne, wiec zrodlo/program powinien byc wspoldzielony miedzy instancjami?
 */

ShaderDrw::ThreadSpecificData::ThreadSpecificData() :
    m_program
{
    nullptr, [](ShaderProgram * s)
    {
        delete s;
    }
}

,
m_isReady(false) { }

ShaderDrw::ThreadSpecificData::ThreadSpecificData(const ThreadSpecificData& tsd) :
    m_program(tsd.m_program),
    m_ViewProjectionMatrixUniform(tsd.m_ViewProjectionMatrixUniform),
    m_ViewMatrixUniform(tsd.m_ViewMatrixUniform),
    m_camPosUniform(tsd.m_camPosUniform),
    m_uniforms(tsd.m_uniforms),
    m_uniformsRaw(tsd.m_uniformsRaw),
    m_refreshUniforms(tsd.m_refreshUniforms),
    m_isReady(false) { }

ShaderDrw::ThreadSpecificData&
ShaderDrw::ThreadSpecificData::operator=(const ThreadSpecificData& tsd)
{
    if(this != &tsd)
    {
        m_program = tsd.m_program;
        m_ViewProjectionMatrixUniform = tsd.m_ViewProjectionMatrixUniform;
        m_ViewMatrixUniform = tsd.m_ViewMatrixUniform;
        m_camPosUniform = tsd.m_camPosUniform;
        m_uniforms = tsd.m_uniforms;
        m_uniformsRaw = tsd.m_uniformsRaw;
        m_refreshUniforms=tsd.m_refreshUniforms,
        m_isReady = false;
    }

    return *this;
}

ShaderDrw::ThreadSpecificData::ThreadSpecificData(ThreadSpecificData&& tsd) :
    m_program(std::move(tsd.m_program)),
    m_ViewProjectionMatrixUniform(std::move(tsd.m_ViewProjectionMatrixUniform)),
    m_ViewMatrixUniform(std::move(tsd.m_ViewMatrixUniform)),
    m_camPosUniform(std::move(tsd.m_camPosUniform)),
    m_uniforms(std::move(tsd.m_uniforms)),
    m_uniformsRaw(std::move(tsd.m_uniformsRaw)),
    m_refreshUniforms(tsd.m_refreshUniforms),
    m_isReady(tsd.m_isReady)
{
    tsd.m_isReady = false;
}

ShaderDrw::ThreadSpecificData&
ShaderDrw::ThreadSpecificData::operator=(ThreadSpecificData&& tsd)
{
    if(this != &tsd)
    {
        m_program = std::move(tsd.m_program);
        m_ViewProjectionMatrixUniform = std::move(tsd.m_ViewProjectionMatrixUniform);
        m_ViewMatrixUniform = std::move(tsd.m_ViewMatrixUniform);
        m_camPosUniform = std::move(tsd.m_camPosUniform);
        m_uniforms = std::move(tsd.m_uniforms);
        m_uniformsRaw = std::move(tsd.m_uniformsRaw);
        m_refreshUniforms = tsd.m_refreshUniforms;
        m_isReady = tsd.m_isReady;
        tsd.m_isReady = false;
    }

    return *this;
}

ShaderDrw::AexRendererUpdateListener::AexRendererUpdateListener(ShaderDrw* self):
parent(self)
{
    assert(self != nullptr);
}

ShaderDrw::AexRendererUpdateListener&
ShaderDrw::AexRendererUpdateListener::operator=(const IOnAexRendererUpdate& rhs)
{
    IOnAexRendererUpdate::operator=(rhs);
    return *this;
}

ShaderDrw::AexRendererUpdateListener&
ShaderDrw::AexRendererUpdateListener::operator=(const AexRendererUpdateListener& rhs)
{
    IOnAexRendererUpdate::operator=(rhs);
    return *this;
}

void
ShaderDrw::AexRendererUpdateListener::onViewProjectionUpdate()
{
    parent->m_refreshViewProjection = true;
}

void
ShaderDrw::AexRendererUpdateListener::onLightsUpdate()
{
    parent->m_refreshLights = true;
}

ShaderDrw::ShaderDrw() :
    DrawObject(),
    m_header(),
    m_vertex_header(),
    m_fragment_header(),
    m_vertex_lines(),
    m_fragment_lines(),
    m_refreshViewProjection(true),
    m_refreshLights(true),
    m_updateListener(this)
{}

ShaderDrw::ShaderDrw(const aex::string& vertex_source, const aex::string& fragment_source, const  aex::string& vertex_attr) :
ShaderDrw()
{
    m_header = "uniform mat4 gl_ModelViewMatrix;\n"
                "uniform mat4 gl_ViewProjectionMatrix;\n"
                "uniform mat4 gl_ProjectionMatrix;\n"
                "uniform mat4 gl_ViewMatrix;\n"
                "uniform mat4 gl_ModelMatrix;\n"
                "uniform mat4 gl_ModelViewProjectionMatrix;\n"
                "uniform mat4 normalMatrix;\n";

    m_vertex_header  = "attribute vec3 gl_Vertex;\n"
                        "attribute vec2 gl_TexCoord;\n"
                        "attribute vec3 gl_nextFrame;\n"
                        "attribute vec3 gl_Normal;\n";


    m_vertex_attr = vertex_attr;

    m_vertex_header += vertex_attr + m_header;

#if __TARGET_AEX_API__ == __GLES__
    m_fragment_header = "#ifdef GL_FRAGMENT_PRECISION_HIGH\n precision highp float;\n#else\n precision mediump float;\n#endif\n" + m_header;
#else
    m_fragment_header = m_header;
#endif
    m_vertex_lines = vertex_source;
    m_fragment_lines = fragment_source;
}

ShaderDrw::ShaderDrw(const ShaderDrw& shader):
ShaderDrw()
{
    *this = shader;
}

ShaderDrw&
ShaderDrw::operator=(const aex::ShaderDrw& shader)
{
    cleanup();

    if(this != &shader)
    {
        m_header = shader.m_header;
        m_vertex_header = shader.m_vertex_header;
        m_vertex_attr = shader.m_vertex_attr;
        m_fragment_header = shader.m_fragment_header;
        m_vertex_lines = shader.m_vertex_lines;
        m_fragment_lines = shader.m_fragment_lines;
        m_refreshViewProjection = shader.m_refreshViewProjection;
        m_currentThreadData = *shader.m_currentThreadData;
        m_cameraPosNeeded = shader.m_cameraPosNeeded;
        m_needsNormalMatrix = shader.m_needsNormalMatrix;
    }

    return *this;
}

ShaderDrw::ShaderDrw(ShaderDrw&& shader) :
ShaderDrw()
{
    *this = std::move(shader);
}

ShaderDrw&
ShaderDrw::operator=(aex::ShaderDrw&& shader)
{
    cleanup();

    if(this != &shader)
    {
        std::swap(m_header, shader.m_header);
        std::swap(m_vertex_header, shader.m_vertex_header);
        std::swap(m_vertex_attr,  shader.m_vertex_attr);
        std::swap(m_fragment_header, shader.m_fragment_header);
        std::swap(m_vertex_lines, shader.m_vertex_lines);
        std::swap(m_fragment_lines, shader.m_fragment_lines);
        std::swap(m_refreshViewProjection, shader.m_refreshViewProjection);
        std::swap(*m_currentThreadData, *(shader.m_currentThreadData));
        m_cameraPosNeeded = shader.m_cameraPosNeeded;
        m_needsNormalMatrix = shader.m_needsNormalMatrix;
    }

    return *this;
}


void ShaderDrw::addExtension(aex::string extension, aex::string behavior)
{
    Extension newExtension(extension, behavior);
    m_extensions.push_back(newExtension);
}

void
ShaderDrw::lateInit() const
{
    ThreadSpecificData& tds = *m_currentThreadData;

    if(tds.m_isReady)
    {
        return;
    }

    build();
    tds.m_ViewMatrixUniform = UniformMatrix4fv();
    tds.m_ViewProjectionMatrixUniform = UniformMatrix4fv();
    tds.m_camPosUniform = UniformFloat3(Vector3(0.0f, 0.0f, 0.0f), "cameraPosition");
    tds.m_ViewMatrixUniform.setName("_gl_ViewMatrix");
    tds.m_ViewProjectionMatrixUniform.setName("_gl_ViewProjectionMatrix");
    tds.m_isReady = true;
}

void
ShaderDrw::onContextDirty()
{
    ThreadSpecificData& tds = *m_currentThreadData;
    tds.m_isReady = false;
}

ShaderDrw::~ShaderDrw()
{
    try
    {
        cleanup();
    }
    catch(std::exception& e)
    {
        LOG_ERR(e.what());
    }
};

void ShaderDrw::build() const
{
    assert(getVertexDataLimit() >= 4);
    m_currentThreadData->m_program = ShaderProgram::makeProgram();
    relink();
}

void
ShaderDrw::relink() const
{
    aex::string vertex_source = fix(m_vertex_header, m_vertex_lines);
    aex::string fragment_source = fix(m_fragment_header, m_fragment_lines);


#if defined(HAVE_GLES3_GL3_H) || defined(HAVE_GL_LOAD_GEN)
    GLSL_ES_3_0_Compatible(vertex_source, fragment_source);
#endif

    if(m_extensions.size()>0)
    {
        aex::string extStr =  cookExtensions(m_extensions);
        addAtBegginAfterVersion(vertex_source, extStr);
        addAtBegginAfterVersion(fragment_source, extStr);
    }
    
    if(m_currentThreadData->m_program)
    {
        auto shaderV = ShaderObject::makeVertexShader( vertex_source);
        auto shaderF = ShaderObject::makeFragmentShader(fragment_source);
        buildProgramObject(m_currentThreadData->m_program, shaderV, shaderF);
    }

    m_currentThreadData->m_needs_mvp =
        (vertex_source.find("gl_ModelViewProjectionMatrix") != aex::string::npos) ||
        (fragment_source.find("gl_ModelViewProjectionMatrix") != aex::string::npos);

    m_currentThreadData->m_needs_normalMatrix=
        (vertex_source.find("normalMatrix") != aex::string::npos) ||
        (fragment_source.find("normalMatrix") != aex::string::npos);


}


void
ShaderDrw::cleanup()
{
    if(m_currentThreadData->m_program)
    {
    }
}

void ShaderDrw::draw(AexRenderer* gl)
{
    lateInit();
    ThreadSpecificData& tds = *m_currentThreadData;

    if(gl && tds.m_program)
    {

        if(m_updateListener != gl)
        {
             m_updateListener.setAexRenderer(gl);
        }

        tds.m_program->enable();
        gl->setShader(this);
        if(m_refreshViewProjection)
        {
            tds.m_ViewProjectionMatrixUniform.setValue(gl->getViewProjectionMatrix().getM());
            tds.m_ViewProjectionMatrixUniform.setUniform(tds.m_program);
            tds.m_ViewMatrixUniform.setValue(gl->getViewMatrix().getM());
            tds.m_ViewMatrixUniform.setUniform(tds.m_program);

            if(m_cameraPosNeeded)
            {
                tds.m_camPosUniform.setValue(gl->getQuatCamera()->GetPosition());
                tds.m_camPosUniform.setUniform(tds.m_program);
            }

            m_refreshViewProjection=false;
        }

        if(m_refreshLights)
            gl->UniformLights();

        if(tds.m_refreshUniforms)
            uniformsUpdate();
    }
}


void ShaderDrw::updateVPMatix(AexRenderer* gl) const
{
    ThreadSpecificData& tds = *m_currentThreadData;
    if(tds.m_program)
    {
            tds.m_ViewProjectionMatrixUniform.setValue(gl->getViewProjectionMatrix().getM());
            tds.m_ViewProjectionMatrixUniform.setUniform(tds.m_program);
            tds.m_ViewMatrixUniform.setValue(gl->getViewMatrix().getM());
            tds.m_ViewMatrixUniform.setUniform(tds.m_program);

            if(m_cameraPosNeeded)
            {
                tds.m_camPosUniform.setValue(gl->getQuatCamera()->GetPosition());
                tds.m_camPosUniform.setUniform(tds.m_program);
            }
    }
}

void ShaderDrw::uniformsUpdate() const
{
    ThreadSpecificData& tds = *m_currentThreadData;

    for(size_t u = 0 ;u< tds.m_uniforms.size();++u)
    {
        tds.m_uniforms[u]->setUniform(tds.m_program);

    }

    for(size_t u = 0 ;u< tds.m_uniformsRaw.size();++u)
    {
        tds.m_uniformsRaw[u]->setUniform(tds.m_program);

    }

}

void ShaderDrw::finishdraw(AexRenderer* gl)
{
    gl->setShader(nullptr);
}

void ShaderDrw::addUniform(const aex::shared_ptr<aex::Uniform>& uniform)
{
    auto& tsd = *m_currentThreadData;
    tsd.m_uniforms.push_back(uniform);
    tsd.m_refreshUniforms = true;
}

void ShaderDrw::addUniform(Uniform* uniform)
{
    auto& tsd = *m_currentThreadData;
    tsd.m_uniformsRaw.push_back(uniform);
    tsd.m_refreshUniforms = true;
}

void ShaderDrw::removeUniform(Uniform* remove)
{
         auto& tsd = *m_currentThreadData;
        for (UniformsRaw::iterator iter  = tsd.m_uniformsRaw.begin();iter!=tsd.m_uniformsRaw.end(); iter++)
        {
            if((*iter)==remove)
            {
                tsd.m_uniformsRaw.erase(iter);
                break;
            }
        }
}

std::vector<Uniform*>& ShaderDrw::getUniformList() const
{
    auto& tsd = *m_currentThreadData;
    return tsd.m_uniformsRaw;
}
void ShaderDrw::setUniformList(std::vector<Uniform*> uniformlist)
{
    auto& tsd = *m_currentThreadData;
     tsd.m_uniformsRaw =uniformlist;
}

void ShaderDrw::setUniforms(Uniforms uniforms)
{
    m_currentThreadData->m_uniforms = uniforms;
}

Uniforms& ShaderDrw::getUniforms()
{
    return m_currentThreadData->m_uniforms;
}

void ShaderDrw::setCameraPosNeeded(bool cameraPosNeeded)
{
    m_cameraPosNeeded = cameraPosNeeded;
}

bool ShaderDrw::isCameraPosNeeded() const
{
    return m_cameraPosNeeded;
}

bool ShaderDrw::isNeeds_MVP() const
{
    return  m_currentThreadData->m_needs_mvp;
}

void ShaderDrw::setNeedsNormalMatrix(bool needsNormalMatrix) {
    m_currentThreadData->m_needs_normalMatrix = needsNormalMatrix;
}

bool ShaderDrw::isNeedsNormalMatrix() const {
    return m_currentThreadData->m_needs_normalMatrix;
}


aex::string ShaderDrw::getVertex_lines() const {
    return m_vertex_lines;
}

aex::string ShaderDrw::getFragment_lines() const {
    return m_fragment_lines;
}

aex::string ShaderDrw::getVertex_attr() const {
    return m_vertex_attr;
}

ShaderDrwShrd_Ptr ShaderDrw::makeColorPickerShader()
{
    return aex::make_shared<ShaderDrw>(m_vertex_lines,
                                       "uniform vec3 colorPick;\n"
                                       "void main(){"
                                       "	gl_FragColor = vec4(colorPick.r ,colorPick.g, colorPick.b, 1.0);\n"
                                       "}"
                                      );
}

ShaderDrw
ShaderDrw::simplyBlack()
{
    return ShaderDrw(

               "void main(){\n"
               "   gl_Position =gl_ViewProjectionMatrix* gl_ModelMatrix *  vec4(gl_Vertex, 1.0);\n"
               "}",
               "void main(){"
               "	gl_FragColor = vec4(0.0,0.0,0.0, 1.0);\n"
               "}"
           );
}

ShaderDrw
ShaderDrw::simplyWhite()
{
    return ShaderDrw(

               "void main(){\n"
               "    gl_Position =gl_ViewProjectionMatrix* gl_ModelMatrix *  vec4(gl_Vertex, 1.0);\n"
               "}",
               "void main(){"
               "    gl_FragColor = vec4(1.0,1.0,1.0, 1.0);\n"
               "}"
           );
}

ShaderDrw
ShaderDrw::simplyRed()
{
    return ShaderDrw(
               "varying vec4 vPosition;\n"

               "void main()\n"
               "{\n"
               "    vPosition =  vec4(gl_Vertex, 1.0);\n"
               "    gl_Position = gl_ViewProjectionMatrix* gl_ModelMatrix * vPosition;\n"
               "}",

               "void main(){"
               "    gl_FragColor = vec4(1.0,0.0,0.0, 1.0);\n"
               "}"
           );
}

ShaderDrw
ShaderDrw::simplyRedAnimated()
{
    return ShaderDrw(
               "varying vec4 vPosition;\n"
               "uniform float tweenFactor;\n"
               "void main(){\n"
               "    vec3 beetwenFrame=  mix( gl_Vertex, gl_nextFrame , tweenFactor);\n"
               "    vPosition =  vec4(beetwenFrame, 1.0);\n"
               "    gl_Position =gl_ViewProjectionMatrix* gl_ModelMatrix * vPosition;\n"
               "}",
               "void main(){"
               "    gl_FragColor = vec4(1.0,0.0,0.0, 1.0);\n"
               "}"
           );
}

ShaderDrw
ShaderDrw::simplyNormalToColor()
{
    return ShaderDrw(
               "varying vec3 vNormal;\n"

               "void main(){\n"

               "  vNormal =  gl_Normal;\n"
               "   gl_Position =gl_ViewProjectionMatrix* gl_ModelMatrix * vec4(gl_Vertex, 1.0);\n"
               "}",

               "varying vec3 vNormal;\n"
               "void main(){"
               "	gl_FragColor = vec4(vNormal, 1.0);\n"
               "}"
           );
}

ShaderDrw LoadShaderFromFile(const aex::string& vertName, const aex::string& fragName, const aex::string& vertex_attr)
{
    FileManager fm;
    typedef boost::filesystem::path path;


    auto vertdir = fm.work_dir();
    vertdir /= path {vertName};

    auto fragdir = fm.work_dir();
    fragdir /= path {fragName};



    aex::string vertShader;
    aex::string fragmentShader;

    auto vfileHandle = fm.open_read_file(vertdir);

    if(vfileHandle)
    {
        vfileHandle.seekg(0, std::ios::end);
        vertShader.reserve(vfileHandle.tellg());
        vfileHandle.seekg(0, std::ios::beg);

        vertShader.assign((std::istreambuf_iterator<char>(vfileHandle)),
                          std::istreambuf_iterator<char>());
        vfileHandle.close();
    }
    else
    {
        LOG_ERR("Can not open the file for vertex shader: ", vertdir.string());
    }

    //LOG_DEBUG(vertShader);


    auto ffileHandle = fm.open_read_file(fragdir);

    if(ffileHandle)
    {
        ffileHandle.seekg(0, std::ios::end);
        fragmentShader.reserve(ffileHandle.tellg());
        ffileHandle.seekg(0, std::ios::beg);

        fragmentShader.assign((std::istreambuf_iterator<char>(ffileHandle)),
                              std::istreambuf_iterator<char>());
        ffileHandle.close();
    }
    else
    {
        LOG_ERR("Can not open the file for fragment shader: ", fragdir.string());
    }

    return ShaderDrw(vertShader, fragmentShader, vertex_attr);
}

ShaderDrw
DropletShaderDrw()
{
    return ShaderDrw(
               "varying vec2 vTex;\n"
               "varying float nrmY;\n"
               "varying vec3 dZ;\n"
               "uniform vec3 colorMod;\n"
               "uniform vec3 cameraPosition;\n"
               "varying vec4 vPosition;\n"
               "varying vec3 vNormal;\n"
               "varying mat3 invgl;\n"
               "varying float dp;"
               "uniform float tweenFactor;\n"

               "void main(void) {\n"
               "vec3 beetwenFrame = gl_Vertex +(vec3(1.0,0.0,0.0)*sin(tweenFactor))*gl_Vertex.y;\n"
               "vPosition =  vec4(gl_Vertex, 1.0);\n"
               "vTex =vec2( gl_TexCoord.x,1.0-gl_TexCoord.y);\n"
               "gl_Position =gl_ViewProjectionMatrix* gl_ModelMatrix * vPosition;\n"

               "vec3 target = normalize(cameraPosition.xyz-gl_Position.xyz);"
               "vNormal = gl_Normal;\n"

               "vec4 aaa =gl_ViewProjectionMatrix* gl_ModelMatrix * vec4(gl_Normal,0.0);\n"
               "dp =(dot(target.xyz,aaa.xyx))/4.0f;"
               "}\n"
               ,
               "varying vec2 vTex;\n"
               "varying float nrmY;\n"
               "varying vec4 vPosition;\n"
               "varying vec3 dZ;\n"
               "varying vec3 vNormal;\n"
               "varying mat3 invgl;\n"

               "struct Light\n"
               "{\n"
               "vec3 lightPosition;\n"
               "vec3 color;\n"
               "float attenuation;\n"
               "float intensity;\n"
               "};\n"

               "uniform sampler2D diffuse;\n"
               "uniform sampler2D specular;\n"
               "uniform	sampler2D bump;\n"
               "uniform Light lights[32];\n"
               "uniform float numlights;\n"
               "uniform vec2 tiling;\n"
               "varying float dp;"

               "void main(void) {\n"

               "vec4 crl   = texture2D(diffuse, vTex);\n"
               "vec4 color = vec4(0.6,0.7,0.95,0.0);\n"
               "gl_FragColor = color+vec4(dp,dp,dp,0.3);//+vec4(colorMod, 0.0);\n"
               "}\n"
           );
}

ShaderDrw
basicShaderDrw()
{
    return ShaderDrw("varying vec2 vTex;\n"
                     "varying float nrmY;\n"
                     "varying vec3 dZ;\n"
                     "varying vec4 vPosition;\n"
                     "varying vec3 vNormal;\n"
                     "varying mat3 invgl;\n"
                     "uniform float tweenFactor;\n"
                     "mat3 nfm(mat4 mat){\n"
                     "float a00 = mat[0][0];\n"
                     "float a01 = mat[0][1];\n"
                     "float a02 = mat[0][2];\n"
                     "float a10 = mat[1][0];\n"
                     "float a11 = mat[1][1];\n"
                     "float a12 = mat[1][2];\n"
                     "float a20 = mat[2][0];\n"
                     "float a21 = mat[2][1];\n"
                     "float a22 = mat[2][2];\n"
                     "float b01 = a22 * a11 - a12 * a21;\n"
                     "float b11 = -a22 * a10 + a12 * a20;\n"
                     "float b21 = a21 * a10 - a11 * a20;\n"
                     "float d = a00 * b01 + a01 * b11 + a02 * b21;\n"
                     "float id = 1.0/d;\n"
                     "mat3 dest = mat3(0.0);\n"
                     "dest[0][0] = b01 * id;\n"
                     "dest[0][1] = b11 * id;\n"
                     "dest[0][2] = b21 * id;\n"
                     "dest[1][0] = (-a22 * a01 + a02 * a21) * id;\n"
                     "dest[1][1] = (a22 * a00 - a02 * a20) * id;\n"
                     "dest[1][2] = (-a21 * a00 + a01 * a20) * id;\n"
                     "dest[2][0] = (a12 * a01 - a02 * a11) * id;\n"
                     "dest[2][1] = (-a12 * a00 + a02 * a10) * id;\n"
                     "dest[2][2] = (a11 * a00 - a01 * a10) * id;\n"
                     "return dest;\n"
                     "}\n"
                     "void main(void) {\n"
                     "invgl = nfm(gl_ModelMatrix);\n"
                     "vNormal = gl_Normal;\n"
                     "vec3 beetwenFrame=  mix( gl_Vertex, gl_nextFrame , tweenFactor);\n"
                     "vPosition =  vec4(beetwenFrame, 1.0);\n"
                     // "vPosition2 = gl_ModelViewMatrix * vec4(gl_nextFrame, 1.0);\n"
                     // "vPosition = mix( vPosition, vPosition2, tweenFactor);\n"

                     //"if(gl_VertexID<  400) {discard;}"
                     "dZ = (gl_ProjectionMatrix * vPosition).xyz;\n"
                     "nrmY = abs(gl_Normal.y);\n"
                     "vTex =vec2( gl_TexCoord.x,1.0-gl_TexCoord.y);\n"
                     "gl_Position =gl_ViewProjectionMatrix* gl_ModelMatrix * vPosition;\n"

                     //"vNormal = vec3(( gl_ProjectionMatrix * vec4(gl_Normal.xyz,1.0)).xyz);\n"
                     "}\n"
                     ,
                     "varying vec2 vTex;\n"
                     "varying float nrmY;\n"
                     "varying vec4 vPosition;\n"
                     "varying vec3 dZ;\n"
                     "varying vec3 vNormal;\n"
                     "varying mat3 invgl;\n"

                     "struct Light\n"
                     "{\n"
                     "vec3 lightPosition;\n"
                     "vec3 color;\n"
                     "float attenuation;\n"
                     "float intensity;\n"
                     "};\n"
                     "struct Material\n"
                     "{\n"
                     "vec3 color;\n"
                     "float shininess;\n"
                     "bool useDiffuse;\n"
                     "bool useSpecular;\n"
                     "bool useLights;\n"
                     "float specularWeight;\n"
                     "bool useBump;\n"
                     "float bumpWeight;\n"
                     "bool useDof;\n"
                     "float mappingType;\n"
                     "float dofWeight;\n"
                     "bool useRefraction;\n"
                     "float refraction;\n"
                     "};\n"
                     "uniform sampler2D diffuse;\n"
                     "uniform sampler2D specular;\n"
                     "uniform	sampler2D bump;\n"
                     "uniform Light lights[32];\n"
                     "uniform Material material;\n"
                     "uniform float numlights;\n"
                     "uniform vec2 tiling;\n"

                     "float rand(vec2 co){\n"
                     "return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);\n"
                     "}\n"
                     "float lightPow(vec3 vpos,vec3 ne,vec3 lp,float attenuation,vec2 til){\n"
                     "vec3 lightDirection = normalize(lp - vpos);\n"
                     "float att = length(lp - vpos);\n"
                     "if(att>attenuation){;\n"
                     "att = attenuation;\n"
                     "};\n"
                     "att = 1.0-att/attenuation;\n"
                     "float dW = att*max(dot(ne,lightDirection),0.0) ;\n"
                     "vec3 eyeDirection = normalize(-dZ.xyz);\n"
                     "vec3 reflectionDirection = reflect(lightDirection, ne);\n"
                     "float shininess = material.shininess;\n"
                     "if(material.useBump){\n"
                     "vec3 bmpp = texture2D(bump, til).xyz;\n"
                     "bmpp = (bmpp -0.5) * 2.0;\n"
                     "reflectionDirection = (reflectionDirection*bmpp)*(material.bumpWeight) + reflectionDirection*(1.0-material.bumpWeight);\n"
                     "dW = dW*max(dot(bmpp, lightDirection), 0.0)*(material.bumpWeight)+dW*(1.0-material.bumpWeight);\n"
                     "};\n"
                     "if (material.useSpecular) {\n"
                     "shininess = texture2D(specular, til).r * 255.0;\n"
                     "}\n"
                     "float specularLightWeighting = pow(max(dot(reflectionDirection, eyeDirection), 0.0), shininess);\n"
                     "dW = dW + specularLightWeighting*material.specularWeight*att;\n"
                     "return dW;\n"
                     "}\n"
                     "void main(void) {\n"
                     "vec2 tiler;\n"
                     "if(tiling.x == 0.0){\n"
                     "tiler = vTex;\n"
                     "}\n"
                     "else{\n"
                     //"tiler = vec2(vTex.s*tiling.x,vTex.t*tiling.y);\n"
                     "tiler=vTex;\n"
                     "}\n"
                     "if(material.mappingType == 2.0){\n"
                     "if(abs(vNormal.z)>0.5){\n"
                     // "tiler = vec2(vPosition.x*tiling.x,vPosition.y*tiling.y);\n"
                     "}else{\n"
                     "if(abs(vNormal.y)>0.5){\n"
                     //"tiler = vec2(vPosition.z*tiling.x,vPosition.x*tiling.x);\n"
                     "}\n"
                     "else{\n"
                     // "tiler = vec2(vPosition.z*tiling.x,vPosition.y*tiling.y);\n"
                     "}\n"
                     "}\n"
                     "}\n"
                     "vec3 normalEye = normalize(vNormal*invgl);\n"
                     "vec3 dWei = vec3(0.0,0.0,0.0);\n"
                     "vec4 clr = vec4(material.color,1.0);\n"
                     "if(material.useDiffuse){\n"
                     "clr += texture2D(diffuse, tiler);\n"
                     "};\n"
                     "if(material.useLights){\n"
                     "for(int i = 0;i<=32;i++){\n"
                     "if( i >= int(numlights)){\n"
                     "break;\n"
                     "};\n"
                     "dWei += lights[i].color+lightPow(vPosition.xyz,normalEye,lights[i].lightPosition,lights[i].attenuation,tiler)*lights[i].intensity;\n"
                     "};\n"
                     "clr = vec4(clr.rgb*dWei,clr.a);\n"
                     "}\n"
                     "if(material.useDof){\n"
                     "float ffac = dZ.z/30.0 ;\n"
                     "if(ffac>1.0){\n"
                     "ffac=1.0;\n"
                     "}\n"
                     "clr = vec4(clr.rgb+vec3(rand(vec2(dZ.x,dZ.y)),rand(vec2(dZ.x+0.1,dZ.y)),rand(vec2(dZ.x-0.1,dZ.y)))*ffac*material.dofWeight,clr.a);\n"
                     "};\n"
                     "if(material.useRefraction){\n"
                     //                          "clr = vec4(clr.rgb,clr.a*material.refraction);\n"
                     "};\n"
                     //                          "clrb = texture2D(bump, tiler);\n"
                     //                          "clr = vec4(clr.rgb,1.0);\n"

                     // "clr = vec4(1.0,0.0,0.0,1.0);\n"

                     "gl_FragColor = clr = texture2D(diffuse, vTex);\n"
                     //"gl_FragDepth = gl_FragCoord.z;"
                     //"colorOut= clr.rgb;\n"
                     /* "if(vPosition.z<(vPosition.z+vNormal.z))"
                     "{gl_FragColor=vec4( 0.0,0.0,0.0,1.0);}"*/
                     "}\n"
                    );
}

ShaderDrw
ShaderDrw::basicBackgroundBox()
{
    return ShaderDrw(
               "varying vec3 vTexCoord;\n"
               "void main(void)\n"
               "{\n"
               "vTexCoord = gl_Vertex;\n"
               "mat4 MVP_mat = gl_ProjectionMatrix * gl_ModelViewMatrix;\n"
               "MVP_mat[3] = vec4( 0.0, 0.0, 0.0, 1.0);\n"
               "vec4 vPosition = MVP_mat * vec4(gl_Vertex, 1.0);\n"
               "gl_Position = vPosition.xyww;\n"
               "}\n"
               ,
               "uniform samplerCube Background;\n"
               "varying vec3 vTexCoord;\n"
               "void main(void)\n"
               "{\n"
               "gl_FragColor = textureCube( Background, vTexCoord );\n"
               "}\n"
           );
}

ShaderDrw
ShaderDrw::BasicSpriteShader()
{
    return ShaderDrw(
               "varying vec2 vTexCoord;\n"
               "void main(void)\n"
               "{\n"
               "vTexCoord = gl_TexCoord;\n"
               "mat4 MVP_mat = gl_ProjectionMatrix*gl_ViewMatrix*gl_ModelMatrix;\n"
               "gl_Position = MVP_mat*gl_Vertex;\n"
               "}\n"
               ,
               "uniform sampler2D sprite;\n"
               "varying vec2 vTexCoord;\n"
               "void main(void)\n"
               "{\n"
               "vec4 clr = texture2D( sprite,  vTexCoord );\n"
               "gl_FragColor =vec4(clr.r,clr.g,clr.b,clr.a);\n"
               "}\n"
           );
}

ShaderDrw
ShaderDrw::BasicSpriteShaderCameraAligned()
{
    return ShaderDrw(
               "varying vec2 vTexCoord;//BasicSpriteShaderCameraAligned\n"
               "uniform vec3 Pos;\n"
               "varying float colorMod;"
               "void main(void)\n"
               "{\n"
               "vTexCoord = vec2(gl_TexCoord.x, 1.0 - gl_TexCoord.y);\n"
               "mat4 MVP_mat = gl_ProjectionMatrix*gl_ViewMatrix*gl_ModelMatrix;\n"
               "vec4 vPosition = gl_ProjectionMatrix* vec4(vec3(gl_ViewMatrix[3].xyz)+gl_Vertex,1.0);\n"
               "gl_Position = vPosition + (MVP_mat*vec4(gl_ModelMatrix[3].xyz,0.0));\n"
               "}\n"
               ,
               "uniform sampler2D sprite;\n"
               "uniform vec3 Pos;\n"
               "uniform vec3 colorM;\n"
               "varying float colorMod;"
               "varying vec2 vTexCoord;\n"
               "void main(void)\n"
               "{\n"
               "vec4 clr = texture2D( sprite,  vTexCoord );\n"

               "gl_FragColor =vec4(clr.r,clr.g,clr.b,clr.a);\n"
               "}"
           );
}

ShaderDrw
ShaderDrw::BasicHudElementShaderOrtho()
{
    return ShaderDrw(
               "varying vec2 vTexCoord;\n"
               "uniform vec2 position;\n"
               "uniform vec2 size;\n"
               "void main(void)\n"
               "{\n"
               "vTexCoord = vec2(gl_Vertex.x, gl_Vertex.y);\n"
               "vec2 Pos=vec2( gl_Vertex.x*size.x, gl_Vertex.y*size.y);\n"
               "mat4 MVP_mat = gl_ProjectionMatrix;\n"
               "gl_Position = MVP_mat*vec4(-Pos.xy-position.xy,0.0,1.0);"

               "}\n"
               ,
               "uniform sampler2D hudElement;\n"
               "varying vec2 vTexCoord;\n"
               "void main(void)\n"
               "{\n"
               "vec4 clr = texture2D( hudElement, vTexCoord );\n"

               "gl_FragColor = clr;\n"
               "}\n"
           );
}

ShaderDrw
ShaderDrw::TextDraw()
{
    return ShaderDrw(
               "varying vec2 vTexCoord;\n"
               "varying vec4 vPosition;\n"
               "void main(void)\n"
               "{\n"
               "vTexCoord = vec2(gl_TexCoord.x, gl_TexCoord.y);\n"
               "vPosition = vec4(gl_Vertex, 1.0);\n"
               "gl_Position =gl_ViewProjectionMatrix * gl_ModelMatrix * vPosition;\n"
               //                         "mat4 MVP_mat =gl_ProjectionMatrix;\n" //Include transformation
               //                         "MVP_mat[3] = vec4( 0.0, 0.0, 0.0, 1.0);\n"
               "}\n"
               ,
               "uniform sampler2D hudElement;\n"
               "uniform vec3 textColor;\n"
               "varying vec2 vTexCoord;\n"
               "void main(void)\n"
               "{\n"
               "vec4 clr = texture2D( hudElement, vTexCoord );\n"
               "if(clr.a < 0.5 ) discard;"
               "gl_FragColor = vec4(textColor, clr.a);\n"
               "}\n"
           );
}

ShaderDrw* HudElementShaderOrthoOffCenter = nullptr;
uint32_t HudElementShaderOrthoOffCenterRefCnt = 0;
std::mutex HudElementShaderOrthoOffCenterLock;

ShaderDrw*
ShaderDrw::BasicHudElementShaderOrthoOffCenter()
{
    {
        std::lock_guard<std::mutex> lock {HudElementShaderOrthoOffCenterLock};

        if(!HudElementShaderOrthoOffCenterRefCnt)
        {
            HudElementShaderOrthoOffCenter = new ShaderDrw(

                "varying vec2 vTexCoord;\n"
                "uniform vec2 position;\n"
                "uniform vec2 size;\n"
                "void main(void)\n"
                "{\n"
                    "vTexCoord = vec2(gl_Vertex.x,1.0 - gl_Vertex.y);\n"
                    "vec2 Pos=vec2(gl_Vertex.x*size.x,gl_Vertex.y*size.y);\n"
                    "mat4 MVP_mat =gl_ProjectionMatrix;\n"
                    "gl_Position =MVP_mat*vec4(-Pos.xy-position.xy,0.0,1.0);"
                "}\n"

                ,

                "uniform sampler2D hudElement;\n"
                "varying vec2 vTexCoord;\n"
                "void main(void)\n"
                "{\n"
                    "vec4 clr = texture2D( hudElement, vTexCoord );\n"
                    //"clr.a=1.0;clr.r=1.0; "
                    //"clr.gb=vTexCoord;"
                    "gl_FragColor = clr;\n"
                "}\n"
            );
            HudElementShaderOrthoOffCenterRefCnt++;

        }
    }
    return HudElementShaderOrthoOffCenter;
}

void
ShaderDrw::FreeHudElementShaderOrthoOffCenter()
{
    std::lock_guard<std::mutex> lock {HudElementShaderOrthoOffCenterLock};
    HudElementShaderOrthoOffCenterRefCnt++;

    if(!HudElementShaderOrthoOffCenterRefCnt && HudElementShaderOrthoOffCenter)
    {
        delete HudElementShaderOrthoOffCenter;
    }
}

ShaderDrw* BasicBatchedTextStringAtlasPtr = nullptr;
uint32_t BasicBatchedTextStringAtlasRefCnt = 0;
std::mutex BasicBatchedTextStringAtlasLock;

   ShaderDrw* ShaderDrw::BasicBatchedTextStringAtlas()
    {
        std::lock_guard<std::mutex> lock {BasicBatchedTextStringAtlasLock};

    if(!BasicBatchedTextStringAtlasRefCnt)
    {
        BasicBatchedTextStringAtlasPtr = new ShaderDrw(
            "attribute vec2 position;\n"
            "attribute vec2 size;\n"
            "attribute vec2 UvOffset;\n"
            "attribute vec2 UvSize;\n"
            "varying vec2 vTexCoord;\n"

//            "uniform vec2 position;\n" // to vertex
//            "uniform vec2 size;\n" // to vertex
//            "uniform vec2 UvOffset;\n" // to vertex
//            "uniform vec2 UvSize;\n"  // to vertex
            "uniform vec2 hudOffset;\n"
            "uniform vec2 txtSize;\n"
            "void main(void)\n"
            "{\n"


            "vTexCoord = vec2((UvOffset.x+(gl_Vertex.x*UvSize.x))/txtSize.x, (UvOffset.y+(gl_Vertex.y*UvSize.y))/txtSize.y);\n"
            "vec2 Pos=vec2(gl_Vertex.x*size.x,gl_Vertex.y*size.y);\n"
            "mat4 MVP_mat =gl_ProjectionMatrix;\n"
            "gl_Position =MVP_mat*vec4(-Pos.xy-position.xy-hudOffset.xy,0.0,1.0);"

            "}\n"
            ,
            "uniform sampler2D hudAtlas;\n"
            "uniform vec3 colorMod;\n"
            "uniform float alphaMod;\n"
            "varying vec2 vTexCoord;\n"
            "void main(void)\n"
            "{\n"

            "vec4 clr = texture2D(hudAtlas, vec2(vTexCoord.x, vTexCoord.y));\n"
            // "if(clr.a<0.6){discard;}"

            "gl_FragColor = clr*vec4(colorMod.x,colorMod.y,colorMod.z,alphaMod);\n"
            "}\n"
        );
        BasicBatchedTextStringAtlasRefCnt++;
    }

    return BasicBatchedTextStringAtlasPtr;
    }

    void ShaderDrw::FreeBasicBatchedTextStringAtlas()
    {
        std::lock_guard<std::mutex> lock {BasicBatchedTextStringAtlasLock};
        BasicBatchedTextStringAtlasRefCnt++;

        if(!BasicBatchedTextStringAtlasRefCnt && BasicBatchedTextStringAtlasPtr)
        {
            delete BasicBatchedTextStringAtlasPtr;
            BasicBatchedTextStringAtlasPtr = nullptr;
        }
    }

ShaderDrw* HudElementShaderOrthoOffCenterAtlas = nullptr;
uint32_t HudElementShaderOrthoOffCenterAtlasRefCnt = 0;
std::mutex HudElementShaderOrthoOffCenterAtlasLock;

ShaderDrw*
ShaderDrw::BasicHudElementShaderOrthoOffCenterAtlas()
{
    std::lock_guard<std::mutex> lock {HudElementShaderOrthoOffCenterAtlasLock};

    if(!HudElementShaderOrthoOffCenterAtlasRefCnt)
    {
        HudElementShaderOrthoOffCenterAtlas = new ShaderDrw(
            "varying vec2 vTexCoord;\n"
            "uniform vec2 position;\n"
            "uniform vec2 size;\n"
            "uniform vec2 UvOffset;\n"
            "uniform vec2 UvSize;\n"
            "uniform vec2 hudOffset;\n"
            "uniform vec2 txtSize;\n"

            "void main(void)\n"
            "{\n"

                "vTexCoord = vec2((UvOffset.x+(gl_Vertex.x*UvSize.x))/txtSize.x, (UvOffset.y+(gl_Vertex.y*UvSize.y))/txtSize.y);\n"
                "vec2 Pos=vec2(gl_Vertex.x*size.x,gl_Vertex.y*size.y);\n"
                "mat4 MVP_mat =gl_ProjectionMatrix;\n"
                "gl_Position =MVP_mat*vec4(-Pos.xy-position.xy-hudOffset.xy,0.0,1.0);"

            "}\n"
            ,
            "uniform sampler2D hudAtlas;\n"
            "uniform vec3 colorMod;\n"
            "uniform float alphaMod;\n"
            "varying vec2 vTexCoord;\n"
            "void main(void)\n"
            "{\n"

            "vec4 clr = texture2D( hudAtlas, vec2(vTexCoord.x, vTexCoord.y));\n"
            "clr.a = clr.a * alphaMod;"
            // "if(clr.a<0.6){discard;}"
            "gl_FragColor = clr+vec4(colorMod.x,colorMod.y,colorMod.z+0.0,0.0);\n"
            "}\n"
        );
        HudElementShaderOrthoOffCenterAtlasRefCnt++;
    }

    return HudElementShaderOrthoOffCenterAtlas;
}

void ShaderDrw::FreeHudElementShaderOrthoOffCenterAtlas()
{
    std::lock_guard<std::mutex> lock {HudElementShaderOrthoOffCenterAtlasLock};
    HudElementShaderOrthoOffCenterAtlasRefCnt++;

    if(!HudElementShaderOrthoOffCenterAtlasRefCnt && HudElementShaderOrthoOffCenterAtlas)
    {
        delete HudElementShaderOrthoOffCenterAtlas;
    }
}

}
