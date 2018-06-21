#include <aex/TextureManager.hpp>
#include <aex/Logger.hpp>
#include <aex/Texture.hpp>

namespace aex
{

TextureManager::TextureManager()
{}

TextureManager::~TextureManager()
{
}

Texture*
TextureManager::findTexture(const aex::string& tKey)
{
    Texture* texture = nullptr;

    for(uint32_t i = 0; i < m_resources.size(); i++)
    {
        if(m_resourceNames[i] == tKey)
        {
            ++m_resRevCount[i];

            texture = m_resources[i].get();
        }
    }

    return texture;
}

void
TextureManager::addTexture(const aex::string& tKey, const aex::shared_ptr<Texture>& texture)
{
    m_resRevCount.push_back(1);
    m_resourceNames.push_back(tKey);
    m_resources.push_back(texture);
}

Texture*
TextureManager::getTexture(const aex::string& TexName, const TextureConfig& cfg)
{
    Texture* texture = findTexture(TexName);

    if(!texture)
    {
        FileManager fManager;
        typedef boost::filesystem::path path;
        path p;
        p /= path {TexName};


        aex::shared_ptr <Texture> newTex = aex::make_shared<Texture>(p.string(), cfg);
        addTexture(TexName, newTex);

        texture = newTex.get();
    }

    return texture;
}

Texture*
TextureManager::getTexture(const aex::string& TexName, bool isMipMapped)
{
    TextureConfig cfg = TextureConfig::makeDefaultTextureConfig();

    if(!isMipMapped)
    {
        cfg.setMagFilterLinear();
        cfg.setMinFilterNearest();

        if(cfg.hasGeneratedMipmaps())
        {
            cfg.toggleGenerateMipmaps();
        }
    }

    return getTexture(TexName, cfg);
}

Texture*
TextureManager::getEmptyTexture(const aex::string& TexName, const TextureConfig& cfg)
{
    Texture* texture = findTexture(TexName);

    if(!texture)
    {
        aex::shared_ptr<Texture> newTex(new Texture(TexName, cfg));
        addTexture(TexName, newTex);
        texture = newTex.get();
    }

    return texture;
}

Texture*
TextureManager::getEmptyTexture(const aex::string& TexName)
{
    return getEmptyTexture(TexName, TextureConfig::makeDefaultTextureConfig());
}

void
TextureManager::releaseTexture(Texture* tex)
{
    if(tex != nullptr)
    {

        for(uint32_t i = 0; i < m_resources.size(); i++)
        {
            // LOG_DEBUG(i, " ", m_resRevCount[i]);
            if(m_resources[i].get() == tex)
            {
                if(m_resRevCount[i] > 0)
                {
                    m_resRevCount[i]--;

                    if(m_resRevCount[i] < 1)
                    {
                        //LOG_DEBUG("To delete: ", m_resourceNames[i], " ", m_resRevCount[i]);
                        m_resourceNames.erase(m_resourceNames.begin() + i);
                        m_resources.erase(m_resources.begin() + i);
                        m_resRevCount.erase(m_resRevCount.begin() + i);
                    }
                }

                break;
            }
        }
    }
}

void
TextureManager::releaseTexture(const aex::string& texName)
{
    for(uint32_t i = 0; i < m_resources.size(); i++)
    {
        // LOG_DEBUG(i, " ", m_resRevCount[i]);
        if(m_resourceNames[i].compare(texName) == 0)
        {
            if(m_resRevCount[i] > 0)
            {

                //LOG_DEBUG("To reduce: ", m_resourceNames[i], ": ", m_resRevCount[i]);
                m_resRevCount[i]--;

                if(m_resRevCount[i] < 1)
                {

                    LOG_DEBUG("To delete: ", m_resourceNames[i], " ", m_resRevCount[i]);
                    m_resRevCount.erase(m_resRevCount.begin() + i);
                    m_resourceNames.erase(m_resourceNames.begin() + i);
                    m_resources.erase(m_resources.begin() + i);
                }
            }

            break;
        }
    }
}

aex::string TextureManager::getTextureName(Texture* tex)
{
    aex::string ret = "";

    
    for(uint32_t i = 0; i < m_resources.size(); i++)
    {

        if(m_resources[i].get() == tex)
        {

            ret = m_resourceNames[i];
        }
    }

    return ret;
}

aex::unique_ptr<TextureManager>& instance()
{
    static aex::unique_ptr<TextureManager> instance_;
    return instance_;
}

TextureManager&
TextureManager::GetInstance()
{
    static std::once_flag only_one;

    std::call_once(only_one,
                   []()
    {
        instance().reset(new TextureManager());
    }
                  );

    return *instance();
}
}
