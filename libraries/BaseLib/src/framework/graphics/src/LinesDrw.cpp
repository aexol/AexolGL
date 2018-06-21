#include <aex/LinesDrw.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/VertexData.hpp>
#include <aex/Sprite.hpp>
#include <aex/Logger.hpp>
namespace aex
{

LinesDrw::LinesDrw() 
:  m_positions
{
    nullptr
},
m_colors {nullptr},
         m_indices {}

         ,
         m_isDepthTestEnabled(false),
         m_isDepthTestEnabledChange(false),
         m_lineStrip(false),
         m_canChangeDepthTestState(false),
         m_canChangeBlendState(false),
         m_blendStatechanged(false),
         m_usingPerVertexColor(false),
         m_lineColor(aex::math::Vector3(0.0f,1.0f,0.0f), "lineColor")
{
        
}
    
LinesDrw::LinesDrw(const std::vector<mesh_vectors_type>& vertices, aex::math::Vector3 lineColor, bool isLineStrip) :
    m_positions
{
    nullptr
},
m_colors {nullptr},
         m_indices {}

         ,
         m_isDepthTestEnabled(false),
         m_isDepthTestEnabledChange(false),
         m_lineStrip(isLineStrip),
         m_canChangeDepthTestState(false),
         m_canChangeBlendState(false),
         m_blendStatechanged(false),
         m_usingPerVertexColor(false),
         m_lineColor(lineColor, "lineColor")
{

    copyVertData(vertices);
    setVertexData();

    uint16_t i = 0;
    m_baseIndexCache.resize(vertices.size() / 3);

    for(auto iter = m_baseIndexCache.begin(); iter != m_baseIndexCache.end(); iter++)
    {
        *iter = i;
        i++;
    }

    setIndexData();

}

LinesDrw::LinesDrw(const std::vector<mesh_indices_type>& indices, const std::vector<mesh_vectors_type>& vertices, aex::math::Vector3 lineColor, bool isLineStrip) :
    m_positions
{
    nullptr
},
m_colors {nullptr},
m_indices {}

,
m_isDepthTestEnabled(false),
m_isDepthTestEnabledChange(false),
m_lineStrip(isLineStrip),
m_canChangeDepthTestState(false),
m_canChangeBlendState(false),
m_blendStatechanged(false),
m_usingPerVertexColor(false),
m_lineColor(lineColor, "lineColor")
{

    copyVertData(vertices);
    setVertexData();

    copyIndexData(indices);
    setIndexData();

}

LinesDrw::LinesDrw(const std::vector<mesh_indices_type>& indices, const std::vector<mesh_vectors_type>& vertices, std::vector<mesh_vectors_type>& color, bool isLineStrip) :
    m_positions
{
    nullptr
},
m_colors {nullptr},
m_indices {}

,
m_isDepthTestEnabled(false),
m_isDepthTestEnabledChange(false),
m_lineStrip(isLineStrip),
m_canChangeDepthTestState(false),
m_canChangeBlendState(false),
m_blendStatechanged(false),
m_usingPerVertexColor(true),
m_lineColor(Vector3(0.0f, 0.0f, 0.0f), "lineColor")
{

    copyVertData(vertices);
    setVertexData();

    copyColorData(color);
    setColorData();

    copyIndexData(indices);
    setIndexData();
}

LinesDrw::~LinesDrw()
{

}

void
LinesDrw::draw(AexRenderer* gl)
{

    const ShaderDrw* shader = gl->getShader();

    if(shader)
    {

        if(m_lazyColorUpdate)
        {
            setColorData();
            m_lazyColorUpdate = false;
        }

        if(m_lazyVertexUpdate)
        {
            setVertexData();
            m_lazyVertexUpdate = false;
        }

        if(m_lazyIndexUpdate)
        {
            setIndexData();
            m_lazyIndexUpdate = false;
        }



        const AexShader& program = shader->GetProgram();

        if(m_indices && m_positions)
        {
            program->setIndices(m_indices);
            program->setVertexData(*m_positions);

            if(m_usingPerVertexColor)
            {
                program->setVertexData(*m_colors);
            }

            m_lineColor.setUniform(program);
        }

        if(m_canChangeBlendState)
        {
            m_blendStatechanged =  gl->setBlendState(true);

        }

        if(m_canChangeDepthTestState)
        {
            m_isDepthTestEnabledChange =   gl->setDepthTestState(false);
        }

        gl->setLineWidth(m_lineWidth);
    }

}

void
LinesDrw::finishdraw(AexRenderer* gl)
{

    if(m_isDepthTestEnabledChange)
    {
        gl->setDepthTestState(true);
        m_isDepthTestEnabledChange = false;
    }

    if(m_blendStatechanged)
    {
        gl->setBlendState(false);
        m_blendStatechanged = false;
    }

    const ShaderDrw* shader = gl->getShader();

    if(shader)
    {
        const AexShader& program = shader->GetProgram();

        if(m_positions)
        {
            program->releaseVertexData(*m_positions);

            if(m_usingPerVertexColor)
            {
                program->releaseVertexData(*m_colors);
            }
        }
    }
}

/*template<class Input, class Output>
void fill_with(Output& out, const Input& in, int32_t times = -1) {
    if (out.size() % in.size() != 0 || (times != -1 && in.size() * times <= out.size()))
        std::runtime_error {
        "Output/Input size mismatch."
    };
    auto outNext = out.begin();
    auto outEnd = out.end();
    auto inBegin = in.begin();
    auto inEnd = in.end();
    while (outNext != outEnd)
        outNext = std::copy(inBegin, inEnd, outNext);
}*/

void
LinesDrw::copyVertData(const std::vector<float>& vertices)
{
    if(vertices.size() % 2 != 0 && !m_lineStrip)
    {
        std::runtime_error("Invalid data set.");
    }

    m_baseVertCache.clear();
    m_baseVertCache.resize(vertices.size());
    std::copy(vertices.begin(), vertices.end(), m_baseVertCache.begin());
    //fill_with(m_baseVertCache, vertices);
}

void
LinesDrw::copyColorData(const std::vector<float>& uvs)
{
    m_baseColorCache.clear();
    m_baseColorCache.resize(uvs.size());
    std::copy(uvs.begin(), uvs.end(), m_baseColorCache.begin());
    //fill_with(m_baseColorCache, uvs);

}

void
LinesDrw::copyIndexData(const std::vector<uint16_t>& index)
{
    // m_baseIndexCache = index;

    m_baseIndexCache.clear();
    m_baseIndexCache.resize(index.size());
    std::copy(index.begin(), index.end(), m_baseIndexCache.begin());
    //fill_with(m_baseIndexCache, index);
}

void
LinesDrw::setVertexData()
{
    m_positions.reset(VertexData::createVertexData(m_baseVertCache, "gl_Vertex", 3));
}

void
LinesDrw::setVertexData(VertexData* vData)
{
     m_positions.reset(vData);
}

void
LinesDrw::setColorData()
{
    m_colors.reset(VertexData::createVertexData(m_baseColorCache, "_gl_Color", 3));
    m_usingPerVertexColor = true;
}

void
LinesDrw::setColorData(VertexData* uvData)
{
    m_colors.reset(uvData);
}

void
LinesDrw::setIndexData()
{
    if(!m_lineStrip)
    {
        m_indices = IndexData::createLines(m_baseIndexCache);
    }
    else
    {
        m_indices = IndexData::createLineStrip(m_baseIndexCache);
    }
}

void
LinesDrw::setIndexData(IndexData* iData)
{
    m_indices = *iData;
}

void LinesDrw::setShape(const std::vector<mesh_vectors_type>& vertices, const std::vector<mesh_vectors_type>& color, const std::vector<mesh_indices_type>& indices, bool lazyupdate)
{
    copyVertData(vertices);
    copyColorData(color);
    copyIndexData(indices);

    if(!lazyupdate)
    {
        setVertexData();
        setColorData();
        setIndexData();
    }
    else
    {
        m_lazyColorUpdate = true;
        m_lazyVertexUpdate = true;
        m_lazyIndexUpdate = true;
    }
}

void LinesDrw::setShape(const std::vector<mesh_vectors_type>& vertices, const std::vector<mesh_indices_type>& indices, bool lazyupdate)
{
    copyVertData(vertices);
    copyIndexData(indices);

    if(!lazyupdate)
    {
        setVertexData();
        setIndexData();
    }
    else
    {
        m_lazyVertexUpdate = true;
        m_lazyIndexUpdate = true;
    }
}

void LinesDrw::setShape(const std::vector<mesh_vectors_type>& vertices, bool lazyupdate)
{
    copyVertData(vertices);


    m_baseIndexCache.resize(vertices.size() / 3);
    int i = 0;

    for(auto iter = m_baseIndexCache.begin(); iter != m_baseIndexCache.end(); iter++)
    {
        *iter = i;
        i++;
    }

    if(!lazyupdate)
    {
        setVertexData();
        setIndexData();

    }
    else
    {
        m_lazyVertexUpdate = true;
        m_lazyIndexUpdate = true;
    }

}
/*
void LinesDrw::setShape(const std::vector<aex::math::Vector3>& vertices, bool lazyupdate = false)
{
    
}*/

void LinesDrw::setShape(const std::vector<mesh_vectors_type>& vertices, const std::vector<mesh_vectors_type>& color, bool lazyupdate)
{
    copyVertData(vertices);
    copyColorData(color);

    m_baseIndexCache.resize(vertices.size() / 3);
    int i = 0;

    for(auto iter = m_baseIndexCache.begin(); iter != m_baseIndexCache.end(); iter++)
    {
        *iter = i++;

    }

    if(!lazyupdate)
    {
        setVertexData();
        setColorData();
        setIndexData();

    }
    else
    {
        m_lazyVertexUpdate = true;
        m_lazyIndexUpdate = true;
        m_lazyColorUpdate = true;
    }

}

void LinesDrw::setBlendStatechanged(bool blendStatechanged)
{
    m_blendStatechanged = blendStatechanged;
}

bool LinesDrw::isBlendStatechanged() const
{
    return m_blendStatechanged;
}

void LinesDrw::setCanChangeBlendState(bool canChangeBlendState)
{
    m_canChangeBlendState = canChangeBlendState;
}

bool LinesDrw::isCanChangeBlendState() const
{
    return m_canChangeBlendState;
}

void LinesDrw::setCanChangeDepthTestState(bool canChangeDepthTestState)
{
    m_canChangeDepthTestState = canChangeDepthTestState;
}

bool LinesDrw::isCanChangeDepthTestState() const
{
    return m_canChangeDepthTestState;
}

void LinesDrw::setLineStrip(bool lineStrip)
{
    m_lineStrip = lineStrip;
}

bool LinesDrw::isLineStrip() const
{
    return m_lineStrip;
}

void LinesDrw::setDepthTestEnabledChange(bool DepthTestEnabledChange)
{
    m_isDepthTestEnabledChange = DepthTestEnabledChange;
}

bool LinesDrw::isDepthTestEnabledChange() const
{
    return m_isDepthTestEnabledChange;
}

void LinesDrw::setDepthTestEnabled(bool DepthTestEnabled)
{
    m_isDepthTestEnabled = DepthTestEnabled;
}

bool LinesDrw::isDepthTestEnabled() const
{
    return m_isDepthTestEnabled;
}

void LinesDrw::setUsingPerVertexColor(bool usingPerVertexColor)
{
    m_usingPerVertexColor = usingPerVertexColor;
}

bool LinesDrw::isUsingPerVertexColor() const
{
    return m_usingPerVertexColor;
}

void LinesDrw::setLineWidth(float width)
{
    m_lineWidth = width;
    //LOG_DEBUG("m_lineWidth :",m_lineWidth);
}

aex::ShaderDrw_ptr LinesDrw::BasicLineShader()
{
    return aex::make_shared<ShaderDrw>(

               "void main(){\n"

               " vec4 vPosition =  vec4(gl_Vertex, 1.0);\n"
               " gl_Position =gl_ViewProjectionMatrix* gl_ModelMatrix * vPosition;\n"
               "}",

               "uniform vec3 lineColor;"
               "void main(){"
               "	gl_FragColor = vec4(lineColor.xyz, 0.0);\n"
               "}"
           );

}

aex::ShaderDrw_ptr LinesDrw::BasicLineVertexColorShader()
{
    return aex::make_shared<ShaderDrw>(

               "varying vec3 vecColor;\n"

               "void main(){\n"

               "vecColor=gl_Color;\n"
               " vec4 vPosition =  vec4(gl_Vertex, 1.0);\n"
               "   gl_Position =gl_ViewProjectionMatrix* gl_ModelMatrix * vPosition;\n"
               "}",

               "varying vec3 vecColor;\n"
               "uniform vec3 lineColor;\n"
               "void main()\n"
               "{\n"
               "	gl_FragColor = vec4(vecColor, 1.0);\n"
               "}",       "attribute vec3 gl_Color;\n"
           );


}
}
