#include <aex/Sprite.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/VertexData.hpp>
#include <aex/TextureManager.hpp>
#include <aex/Mesh.hpp>

namespace aex
{

const std::array<float, 12> Sprite::SimpleSpriteVertBottomRoot =
{
    {
        -0.5, 0.0, 0.0,
        -0.5, 1.0, 0.0,
        0.5, 1.0, 0.0,
        0.5, 0.0, 0.0
    }
};

const std::array<float, 12> Sprite::SimpleSpriteVertCenterRoot =
{
    {
        -0.5, -0.5, 0.0,
        -0.5, 0.5, 0.0,
        0.5, 0.5, 0.0,
        0.5, -0.5, 0.0
    }
};

const std::array<float, 8> Sprite::SimpleSpriteUV =
{
    {
        0.0, 0.0,
        0.0, 1.0,
        1.0, 1.0,
        1.0, 0.0
    }
};

const std::array<uint16_t, 6> Sprite::SimpleSpriteIndex =
{
    {0, 1, 2, 0, 2, 3}
};

const std::array<uint16_t, 4> Sprite::SimpleSpriteIndexTriangleStrip =
{
    {1, 0, 2, 3}
};

class sharedSpriteForward : public Sprite
{
public:

    explicit sharedSpriteForward(Texture* tex) :
        Sprite(tex)
    {
    }
};

Sprite_ptr
Sprite::makeSprite(Texture* tex)
{
    Sprite_ptr ret = make_shared<sharedSpriteForward>(tex);
    return ret;
}

void Sprite::setSprite(Texture* Sprite)
{
    //m_Sprite->unbind();
    //m_Sprite->unload();
    if(m_Sprite != nullptr)
    {
        TextureManager::GetInstance().releaseTexture(m_Sprite);
    }

    m_Sprite = Sprite;
}

void Sprite::setSprite(const aex::string& SpriteFile)
{
    if(m_Sprite != nullptr)
    {
        TextureManager::GetInstance().releaseTexture(m_Sprite);
    }

    m_Sprite = TextureManager::GetInstance().getTexture(SpriteFile);
}

Texture* Sprite::getSprite() const
{
    return m_Sprite;
}

void Sprite::setCanChangeDepthTestState(bool canChangeState)
{
    m_canChangeDepthTestState = canChangeState;
}

bool Sprite::isCanChangeDepthTestState() const
{
    return m_canChangeDepthTestState;
}

/*void Sprite::setShader(ShaderDrwShrd_Ptr shader) {
    m_shader = shader;
    m_shaderLoaded = true;
}

ShaderDrwShrd_Ptr Sprite::getShader() {
    return m_shader;
}*/

void Sprite::setNeedOwnShader(bool needOwnShader)
{
    m_needOwnShader = needOwnShader;
}

bool Sprite::isNeedOwnShader() const
{
    return m_needOwnShader;
}

void Sprite::setQuatCamera(const aex::shared_ptr<aex::QuaternionCamera>& camera)
{
    m_quatCamera = camera.get();
}

void Sprite::setQuatCamera(aex::QuaternionCamera* camera)
{
    m_quatCamera = camera;
}

template<class Input, class Output>
void fill_with(Output& out, const Input& in, int32_t times = -1)
{
    if(out.size() % in.size() != 0 || (times != -1 && in.size() * times <= out.size()))
        std::runtime_error
    {
        "Output/Input size mismatch."
    };

    auto outNext = out.begin();

    auto outEnd = out.end();

    auto inBegin = in.begin();

    auto inEnd = in.end();

    while(outNext != outEnd)
    {
        outNext = std::copy(inBegin, inEnd, outNext);
    }
}

void
Sprite::setVertexData()
{
    m_positions.reset(VertexData::createVertexData(m_batchVertCache, "gl_Vertex", 3));
}

void
Sprite::setVertexData(const VertexData* vData)
{
    m_positions.reset();
    m_customPosition = vData;
}

void
Sprite::copyVertData(const std::array<float, 12>& vertices)
{
    m_baseVertCache.clear();
    m_baseVertCache.resize(vertices.size());
    std::copy(vertices.begin(), vertices.end(), m_baseVertCache.begin());
    m_batchVertCache.clear();
    m_batchVertCache.resize(vertices.size() * m_spriteCount);
    fill_with(m_batchVertCache, vertices);
}

void
Sprite::copyVertData(const std::vector<float>& vertices)
{
    if(vertices.size() % 3 != 0 && !m_triangleStrip)
    {
        std::runtime_error("Invalid data set.");
    }

    m_baseVertCache = vertices;
    m_batchVertCache.clear();
    m_batchVertCache.resize(vertices.size() * m_spriteCount);
    fill_with(m_batchVertCache, vertices);
}

void
Sprite::setUVData()
{
    m_uvs.reset(VertexData::createVertexData(m_batchUVCache, "gl_TexCoord", 2));
}

void
Sprite::setUVData(const VertexData* uvData)
{
    m_uvs.reset();
    m_customUVs = uvData;
}

void
Sprite::copyUVData(const std::array<float, 8>& uvs)
{
    m_baseUVCache.clear();
    m_baseUVCache.resize(uvs.size());
    std::copy(uvs.begin(), uvs.end(), m_baseUVCache.begin());
    m_batchUVCache.clear();
    m_batchUVCache.resize(uvs.size() * m_spriteCount);
    fill_with(m_batchUVCache, uvs);
}

void
Sprite::copyUVData(const std::vector<float>& uvs)
{
    m_baseUVCache = uvs;
    m_batchUVCache.clear();
    m_batchUVCache.resize(uvs.size() * m_spriteCount);
    fill_with(m_batchUVCache, uvs);
}

void
Sprite::setIndexData()
{
    if(!m_triangleStrip)
    {
        m_indices = IndexData::createTriangles(m_batchIndexCache);
    }
    else
    {
        m_indices = IndexData::createTraingleStrip(m_batchIndexCache);
    }
}

void
Sprite::setIndexData(const IndexData* iData)
{
    m_indices = IndexData();
    m_customIndices = iData;
}

template<class IndexData, class DataType>
void fixIndices(IndexData& indices, uint32_t spriteCount, size_t baseDataSize, DataType maxIndex)
{
    maxIndex++;

    for(uint32_t ii = baseDataSize; ii < spriteCount * baseDataSize; ii += baseDataSize)
    {
        auto step = ii / baseDataSize;

        for(uint32_t jj = ii; jj < ii + baseDataSize; ++jj)
        {
            indices[jj] += maxIndex * step;
        }
    }
}

void
Sprite::copyIndexData(const std::vector<uint16_t>& index)
{
    m_baseIndexCache = index;
    m_batchIndexCache.clear();
    m_batchIndexCache.resize(index.size() * m_spriteCount);
    fill_with(m_batchIndexCache, index);
    auto maxIndex = std::max_element(index.begin(), index.end());
    fixIndices(m_batchIndexCache, m_spriteCount, index.size(), *maxIndex);
}

void
Sprite::copyIndexData(const std::array<uint16_t, 6>& index)
{
    m_baseIndexCache.clear();
    m_baseIndexCache.resize(index.size());
    std::copy(index.begin(), index.end(), m_baseIndexCache.begin());
    m_batchIndexCache.clear();
    m_batchIndexCache.resize(index.size() * m_spriteCount);
    fill_with(m_batchIndexCache, index);
    auto maxIndex = std::max_element(index.begin(), index.end());
    fixIndices(m_batchIndexCache, m_spriteCount, index.size(), *maxIndex);
}

void
Sprite::copyIndexData(const std::array<uint16_t, 4>& index)
{
    m_baseIndexCache.clear();
    m_baseIndexCache.resize(index.size());
    std::copy(index.begin(), index.end(), m_baseIndexCache.begin());
    m_batchIndexCache.clear();
    m_batchIndexCache.resize(index.size() * m_spriteCount);
    fill_with(m_batchIndexCache, index);
    auto maxIndex = std::max_element(index.begin(), index.end());
    fixIndices(m_batchIndexCache, m_spriteCount, index.size(), *maxIndex);
}

void
Sprite::SetAnchorCenter()
{
    std::vector<float> temp;

    for(float a : SimpleSpriteVertCenterRoot)
    {
        temp.push_back(a);
    }

    copyVertData(temp);
    setVertexData();
}

void Sprite::SetShape(Mesh* mesh)
{
   SetShape(mesh->getIndices(), mesh->getVertices(), *mesh->getCoords());
}

void Sprite::SetShape(const std::vector<mesh_indices_type>& indices, const std::vector<mesh_vectors_type>& vertices, const std::vector<mesh_vectors_type>& texcoords)
{
    copyVertData(vertices);
    setVertexData();

    copyUVData(texcoords);
    setUVData();

    copyIndexData(indices);
    setIndexData();
}

void Sprite::SetShape(const aex::VertexData* vData)
{
    setVertexData(vData);
    setUVData();
    m_indices = IndexData();
    setIndexData();
}

void Sprite::SetNewUV(std::vector<float>& temp)
{
    copyUVData(temp);
    setUVData();
}

void
Sprite::setSpritesScaled(float scale)
{
    std::vector<float> temp;

    for(float a : m_baseVertCache)
    {
        temp.push_back(a * scale);
    }

    copyVertData(temp);
    setVertexData();
}

void Sprite::ScaleVerts(float scale)
{
    setSpritesScaled(scale);
}

void
Sprite::setSpriteCount(uint32_t spriteCount)
{
    if(!spriteCount)
        std::runtime_error
    {
        "setSpriteCount Invalid argument."
    };

    m_spriteCount = spriteCount;

    SetShape(m_baseIndexCache, m_baseVertCache, m_baseUVCache);
}

void
Sprite::setPositions(const std::vector<aex::math::Vector3>& positions)
{
    if(positions.size() != m_spriteCount)
        std::runtime_error
    {
        "Invalid argument."
    };

    auto dataSize = m_baseVertCache.size();

    int spriteNumber = 0;

    for(auto next = positions.begin(); next != positions.end(); ++spriteNumber, next++)
    {
        auto offset_x = next->x;
        auto offset_y = next->y;
        auto offset_z = next->z;

        for(uint32_t ii = 0; ii < dataSize; ii += 3)
        {
            auto pointIdx = spriteNumber * dataSize + ii;
            m_batchVertCache[pointIdx] += offset_x;
            m_batchVertCache[pointIdx + 1] += offset_y;
            m_batchVertCache[pointIdx + 2] += offset_z;
        }
    }

    setVertexData();
}

int
Sprite::getPointPerSpriteCount()
{
    return m_baseVertCache.size() / 3;
}

void Sprite::setTriangleStrip(bool triangleStrip)
{
    m_triangleStrip = triangleStrip;
}

bool Sprite::isTriangleStrip() const
{
    return m_triangleStrip;
}

void Sprite::setCanChangeBlendState(bool canChangeBlendState)
{
    m_canChangeBlendState = canChangeBlendState;
}

bool Sprite::isCanChangeBlendState() const
{
    return m_canChangeBlendState;
}

Sprite::Sprite(Texture* tex) :
    m_positions
{
    nullptr
},
m_uvs {nullptr},
      m_indices {}

      ,
      m_CamPosUniform(-1),
      m_ProjectionMatrixUniform(-1),
      m_ViewMatrixUniform(-1),
      m_spriteCount(1),
      m_isDepthTestEnabledChange(false),
      m_triangleStrip(false),
      m_canChangeDepthTestState(false),
      m_canChangeBlendState(true),
      m_blendStatechanged(false),
      m_Sprite(tex),
      m_shaderLoaded(false),
      m_quatCamera(nullptr)
{


    copyVertData(SimpleSpriteVertCenterRoot);
    setVertexData();

    copyUVData(SimpleSpriteUV);
    setUVData();

    if(!m_triangleStrip)
    {
        copyIndexData(SimpleSpriteIndex);
        setIndexData();
    }
    else
    {
        copyIndexData(SimpleSpriteIndexTriangleStrip);
        setIndexData();
    }

    m_viewMatrixU.setName("_gl_ViewMatrix");
    m_ProjectionMatrixU.setName("_gl_ProjectionMatrix");
    m_spriteSamplerUniform = make_shared<UniformSampler>(m_Sprite, "sprite");
}

Sprite::Sprite() :
    m_positions
{
    nullptr
},
m_uvs {nullptr},
m_indices {}

,
m_CamPosUniform(-1),
m_ProjectionMatrixUniform(-1),
m_ViewMatrixUniform(-1),
m_spriteCount(1),
m_isDepthTestEnabledChange(false),
m_triangleStrip(false),
m_canChangeDepthTestState(false),
m_canChangeBlendState(true),
m_blendStatechanged(false),
m_Sprite(nullptr),
m_shaderLoaded(false),
m_quatCamera(nullptr)
{


    copyVertData(SimpleSpriteVertCenterRoot);
    setVertexData();

    copyUVData(SimpleSpriteUV);
    setUVData();

    if(!m_triangleStrip)
    {
        copyIndexData(SimpleSpriteIndex);
        setIndexData();
    }
    else
    {
        copyIndexData(SimpleSpriteIndexTriangleStrip);
        setIndexData();
    }

    m_viewMatrixU.setName("_gl_ViewMatrix");
    m_ProjectionMatrixU.setName("_gl_ProjectionMatrix");
    m_spriteSamplerUniform = aex::make_shared<UniformSampler>(nullptr, "sprite");
}

void
Sprite::draw(AexRenderer* gl)
{

    /*if (m_needOwnShader) {
        if (!m_shaderLoaded) {
            m_shader = make_shared<ShaderDrw>(ShaderDrw::BasicSpriteShaderCameraAligned());
            m_shaderLoaded = true;
        }
        m_shader->draw(gl);
    }*/

    const ShaderDrw* shader = gl->getShader();

    if(shader)
    {
        const AexShader& program = shader->GetProgram();

        if(m_Sprite)
        {
            m_Sprite->bind();

            /* if (temptext != m_Sprite->getBinder()) {
                 LOG_DEBUG("old temptext: ", temptext, ", new temptext: ", m_Sprite->getBinder());
                 temptext = m_Sprite->getBinder();
             }**/

            m_spriteSamplerUniform->setValue(m_Sprite);
            m_spriteSamplerUniform->setUniform(program);
        }

        /*m_viewMatrixU.setValue(gl->getQuatCamera()->get_view_matrix().getM());
        m_viewMatrixU.setUniform(program);*/
        // if (!m_pojectionSet) {
        m_ProjectionMatrixU.setValue(gl->getQuatCamera()->get_projection_matrix().getM());
        m_ProjectionMatrixU.setUniform(program);
        //    m_pojectionSet=true;

        if(m_indices && m_positions && m_uvs)
        {
            program->setIndices(m_indices);
            program->setVertexData(*m_positions);
            program->setVertexData(*m_uvs);
        }
        else if(m_customIndices && m_customPosition && m_customUVs)
        {
            program->setIndices(*m_customIndices);
            program->setVertexData(*m_customPosition);
            program->setVertexData(*m_customUVs);
        }


        if(m_canChangeBlendState)
        {
            m_blendStatechanged = gl->setBlendState(true);
        }

        if(m_canChangeDepthTestState)
        {
            m_isDepthTestEnabledChange = gl->setDepthTestState(false);

        }
    }
}

void
Sprite::finishdraw(AexRenderer* gl)
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

        if(m_positions && m_uvs)
        {
            program->releaseVertexData(*m_positions);
            program->releaseVertexData(*m_uvs);
        }
        else if(m_customIndices && m_customPosition && m_customUVs)
        {
            program->releaseVertexData(*m_customPosition);
            program->releaseVertexData(*m_customUVs);
        }

    }
    
    if(m_Sprite)
    {
        m_Sprite->unbind();
    }
}

Sprite::~Sprite()
{

    if(m_Sprite)
    {
        TextureManager::GetInstance().releaseTexture(m_Sprite);
    }
}

}
