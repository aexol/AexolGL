#include <aex/Hud.hpp>
#include <aex/TextureIncludes.hpp>
#include <aex/TextureManager.hpp>
#include <aex/Logger.hpp>
#include <aex/TextDrw.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/VertexData.hpp>
#include <aex/HudElement.hpp>
#include <json/json.h>
#include <sstream>

namespace aex
{
    const std::string HudParseError::he_tag = "Hud parse failure: ";
    HudParseError::HudParseError(const std::string& msg) : m_msg(he_tag+msg){}
    HudParseError::HudParseError(const char* msg) : m_msg(he_tag+msg){}
    const char* HudParseError::what() const noexcept
    {
        return m_msg.c_str();
    }
    static const std::vector<float> SimpleHudElementVertCornerRoot = {
        {
            0.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0
        }
    };
    static const std::vector<float> SimpleHudElementVertCenterRoot = {
        {
            -0.5, -0.5, 0.0,
            -0.5, 0.5, 0.0,
            0.5, 0.5, 0.0,
            0.5, -0.5, 0.0
        }
    };
    static const std::vector<float> SimpleHudElementUV = {
        {
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0
        }
    };

    static const std::vector<uint16_t> SimpleHudElementIndexTriangleStrip = {
        {1, 2, 0, 3}
    };
    static std::unique_ptr<VertexData> SimpleHudElementVertCornerRootCache{nullptr};
    static uint32_t CornerRootCacheReferences;
    static std::mutex CornerRootLock;
    static std::unique_ptr<VertexData> SimpleHudElementVertCenterRootCache{nullptr};
    static uint32_t CenterRootCacheReferences;
    static std::mutex CenterRootLock;
    static std::unique_ptr<IndexData> SimpleHudElementIndexCache{nullptr};
    static std::unique_ptr<IndexData> SimpleHudElementIndexTriangleStripCache{nullptr};
    static uint32_t IndexStripCacheReferences;
    static std::mutex IndexStripLock;

    static VertexData* getVertCornerRoot()
    {
        std::lock_guard<std::mutex> lock{CornerRootLock};

        if (!SimpleHudElementVertCornerRootCache)
        {
            SimpleHudElementVertCornerRootCache.reset(
                    VertexData::createVertexData(SimpleHudElementVertCornerRoot, "gl_Vertex", 3)
                    );
        }

        CornerRootCacheReferences++;
        return SimpleHudElementVertCornerRootCache.get();
    }

    static void releaseVertCornerRoot()
    {
        std::lock_guard<std::mutex> lock{CornerRootLock};
        CornerRootCacheReferences--;

        if (CornerRootCacheReferences == 0)
        {
            SimpleHudElementVertCornerRootCache.reset();
        }
    }
    //    static VertexData* getVertCenterRoot()
    //    {
    //        std::lock_guard<std::mutex> lock{CenterRootLock};
    //        if(!SimpleHudElementVertCenterRootCache)
    //        {
    //            SimpleHudElementVertCenterRootCache.reset(
    //              VertexData::createVertexData(SimpleHudElementVertCenterRoot, "gl_Vertex", 3)
    //            );
    //        }
    //        CenterRootCacheReferences++;
    //        return SimpleHudElementVertCenterRootCache.get();
    //    }

    static void releaseVertCenterRoot()
    {
        std::lock_guard<std::mutex> lock{CenterRootLock};
        CenterRootCacheReferences--;

        if (CenterRootCacheReferences == 0)
        {
            SimpleHudElementVertCenterRootCache.reset();
        }
    }

    static IndexData* getIndexStripVBO()
    {
        std::lock_guard<std::mutex> lock{IndexStripLock};

        if (!SimpleHudElementIndexTriangleStripCache)
        {
            SimpleHudElementIndexTriangleStripCache.reset(
                    new IndexData(IndexData::createTraingleStrip(SimpleHudElementIndexTriangleStrip))
                );
        }

        IndexStripCacheReferences++;
        return SimpleHudElementIndexTriangleStripCache.get();
    }

    void releaseIndexStrip()
    {
        std::lock_guard<std::mutex> lock{IndexStripLock};
        IndexStripCacheReferences--;

        if (IndexStripCacheReferences == 0)
        {
            SimpleHudElementIndexTriangleStripCache.reset();
        }
    }

    class sharedHudForward : public Hud
    {
    public:

        sharedHudForward(Texture* tex) :
        Hud(tex) { }
    };

    Hud_ptr
    Hud::makeHud(Texture* tex)
    {
        Hud_ptr ret = make_shared<sharedHudForward>(tex);
        return ret;
    }

    Hud::Hud(Texture* tex) :
    m_name(""),

    m_shader(ShaderDrw::BasicHudElementShaderOrthoOffCenterAtlas()),
    m_shaderLoaded(false),
    m_hudOffsetX(0.0f),
    m_hudOffsetY(0.0f),
    m_centerPosition(false),
    m_isDepthTestEnabledChange(false),
    m_blendStatechanged(false),
    m_canChangeDepthTestState(true),
    m_canChangeBlendState(true),
    m_textureAtlas(tex),
    m_HudAtlasUniform(m_textureAtlas, "hudAtlas"),
    m_NextLayer(nullptr),
    m_cleanup([](ShaderDrw*)
    {

        ShaderDrw::FreeHudElementShaderOrthoOffCenterAtlas();
    })
    {
        m_hudElementsDrawPtr = nullptr;
        //m_textureAtlas->handle<Texture2DSDL, ImageFile>();

        m_spriteVbo = getVertCornerRoot();
        m_indexVbo = getIndexStripVBO();

        m_viewMatrixU.setName("_gl_ViewMatrix");
        m_ProjectionMatrixU.setName("_gl_ProjectionMatrix");
        m_hudOffsetU.setName("hudOffset");
        m_hudTxtSizeU.setName("txtSize");
        m_hudTxtSizeU.setValue(static_cast<float> (tex->getSizeW()), static_cast<float> (tex->getSizeH()));
    }

    Hud::Hud(const std::string& texturePath, const aex::TextureConfig& cfg) :
    m_name(""),

    m_shader(ShaderDrw::BasicHudElementShaderOrthoOffCenterAtlas()),
    m_shaderLoaded(false),
    m_hudOffsetX(0.0f),
    m_hudOffsetY(0.0f),
    m_centerPosition(false),
    m_isDepthTestEnabledChange(false),
    m_blendStatechanged(false),
    m_canChangeDepthTestState(true),
    m_canChangeBlendState(true),
    m_textureAtlas(aex::TextureManager::GetInstance().getTexture(texturePath, cfg)),
    m_HudAtlasUniform(m_textureAtlas, "hudAtlas"),
    m_NextLayer(nullptr),
    m_cleanup([](ShaderDrw*)
    {
        ShaderDrw::FreeHudElementShaderOrthoOffCenterAtlas();
    })
    {
        m_hudElementsDrawPtr = nullptr;
        //m_textureAtlas->handle<Texture2DSDL, ImageFile>();

        m_spriteVbo = getVertCornerRoot();
        m_indexVbo = getIndexStripVBO();

        m_viewMatrixU.setName("_gl_ViewMatrix");
        m_ProjectionMatrixU.setName("_gl_ProjectionMatrix");
        m_hudOffsetU.setName("hudOffset");
        m_hudTxtSizeU.setName("txtSize");
        m_hudTxtSizeU.setValue(static_cast<float> (m_textureAtlas->getSizeW()), static_cast<float> (m_textureAtlas->getSizeH()));
    }

    Hud::Hud() :
    m_name(""),
    m_shader(ShaderDrw::BasicHudElementShaderOrthoOffCenterAtlas()),
    m_shaderLoaded(false),
    m_hudOffsetX(0.0f),
    m_hudOffsetY(0.0f),
    m_centerPosition(false),
    m_isDepthTestEnabledChange(false),
    m_blendStatechanged(false),
    m_canChangeDepthTestState(true),
    m_canChangeBlendState(true),
    m_textureAtlas(nullptr),
    m_HudAtlasUniform(nullptr, "hudAtlas"),
    m_NextLayer(nullptr),
    m_cleanup([](ShaderDrw*)
    {
        ShaderDrw::FreeHudElementShaderOrthoOffCenterAtlas();
    })
    {
        m_hudElementsDrawPtr = nullptr;
        m_spriteVbo = getVertCornerRoot();
        m_indexVbo = getIndexStripVBO();

        m_viewMatrixU.setName("_gl_ViewMatrix");
        m_ProjectionMatrixU.setName("_gl_ProjectionMatrix");
        m_hudOffsetU.setName("hudOffset");
        m_hudTxtSizeU.setName("txtSize");
        if (m_textureAtlas)
        {
            m_HudAtlasUniform.setValue(m_textureAtlas);
            m_HudAtlasUniform.setName("hudAtlas");
        }

    }

    void Hud::setShader(const ShaderDrw& shader)
    {
        if (m_cleanup && m_shader)
        {
            m_cleanup(m_shader);
        }

        m_shader = new ShaderDrw(shader);
        m_cleanup = [](ShaderDrw * s)
        {
            delete s;
        };
        m_shaderLoaded = true;
    }

    const ShaderDrw& Hud::getShader() const
    {
        return *m_shader;
    }

    void Hud::setName(std::string name)
    {
        m_name = name;
    }

    std::string Hud::getName()
    {
        return m_name;
    }

    Uniforms Hud::getUniforms() const
    {
        return m_uniforms;
    }

    float Hud::getHudOffsetY() const
    {
        return m_hudOffsetY;
    }

    float Hud::getHudOffsetX() const
    {
        return m_hudOffsetX;
    }

    void Hud::addHudOffset(aex::math::Vector2 offset)
    {
        addHudOffsetXY(offset.x, offset.y);
    }

    void Hud::setHudOffset(aex::math::Vector2 offset)
    {
        setHudOffsetXY(offset.x, offset.y);
    }

    void Hud::addHudOffsetXY(float hudOffsetX, float hudOffsetY)
    {
        m_hudOffsetX += hudOffsetX;
        m_hudOffsetY += hudOffsetY;

        for (Hud_ptr& layer : m_HudLayers)
        {
            layer->addHudOffsetXY(hudOffsetX, hudOffsetY);
        }

        /*if (m_NextLayer != nullptr)
        {
            m_NextLayer->addHudOffsetXY(hudOffsetX, hudOffsetY);
        }*/
    }

    void Hud::setHudOffsetXY(float hudOffsetX, float hudOffsetY)
    {
        m_hudOffsetX = hudOffsetX;
        m_hudOffsetY = hudOffsetY;


        for (Hud_ptr& layer : m_HudLayers)
        {
            layer->setHudOffsetXY(hudOffsetX, hudOffsetY);
        }

        /*if (m_NextLayer != nullptr)
        {
            m_NextLayer->setHudOffsetXY(hudOffsetX, hudOffsetY);
        }*/
    }

    void Hud::scale(float hudscaleX, float hudscaleY)
    {
        m_scaleX = hudscaleX;
        m_scaleY = hudscaleY;

        for (HudElementPtr element : m_hudElements)
        {
            element->scale(hudscaleX, hudscaleY);
        }

        for (Hud_ptr& layer : m_HudLayers)
        {
            layer->scale(hudscaleX, hudscaleY);
        }

        /* if (m_NextLayer != nullptr)
         {
             m_NextLayer->scale(hudscaleX, hudscaleY);
         }*/

    }

    void Hud::move(float offsetX, float offsetY)
    {

        for (HudElementPtr element : m_hudElements)
        {
            element->addPos(offsetX, offsetY);
        }

        for (Hud_ptr& layer : m_HudLayers)
        {
            layer->move(offsetX, offsetY);
        }

        /*if (m_NextLayer != nullptr)
        {
            m_NextLayer->move(offsetX, offsetY);
        }*/
    }

    void Hud::setCanChangeDepthTestState(bool canChangeDepthTestState)
    {
        m_canChangeDepthTestState = canChangeDepthTestState;
    }

    bool Hud::isCanChangeDepthTestState() const
    {
        return m_canChangeDepthTestState;
    }

    void Hud::setCanChangeBlendState(bool canChangeBlendState)
    {
        m_canChangeBlendState = canChangeBlendState;
    }

    bool Hud::isCanChangeBlendState() const
    {
        return m_canChangeBlendState;
    }

    void Hud::addNextLayer(Hud_ptr nextLayer)
    {

        bool exists = false;
        for (Hud_ptr& layer : m_HudLayers)
        {
            if (layer == nextLayer)
            {
                exists = true;
            }
        }
        if (!exists)
        {
            m_HudLayers.push_back(nextLayer);
        }

        /*if (m_NextLayer != nullptr)
        {
            if (m_NextLayer != nextLayer)
            {
                m_NextLayer->addNextLayer(nextLayer);
            }
        } else
        {
            m_NextLayer = nextLayer;
        }*/
    }

    void Hud::removeLayer(Hud_ptr nextLayer)
    {
        for (std::vector<Hud_ptr>::iterator iterator = m_HudLayers.begin(); iterator != m_HudLayers.end(); iterator++)
        {
            if (iterator->get() == nextLayer.get())
            {
                m_HudLayers.erase(iterator);
                break;
            }
        }
    }

    void Hud::removeAllLayers()
    {

        m_HudLayers.clear();
    }

    void Hud::addHudElement(const HudElementPtr& hudelement)
    {
        m_hudElements.push_back(hudelement);
    }

    void Hud::removeHudElement(const HudElementPtr& hudelement)
    {
        auto it = std::find(m_hudElements.begin(), m_hudElements.end(), hudelement);
        if (it != m_hudElements.end())
        {
            m_hudElements.erase(it);
        }
    }

    void Hud::replaceHudElement(const HudElementPtr& oldElem, const HudElementPtr& newElem)
    {
        for (size_t i = 0; i < m_hudElements.size(); i++)
        {
            if (m_hudElements[i] == oldElem)
            {
                LOG_DEBUG("replace ", m_hudElements[i]->getName(), " on ", newElem->getName());
                m_hudElements[i] = newElem;
                break;
            }
        }
    }

    void Hud::setTextureAtlas(Texture* textureAtlas)
    {
        if (m_textureAtlas != nullptr)
        {
            TextureManager::GetInstance().releaseTexture(m_textureAtlas);
        }

        m_textureAtlas = textureAtlas;
    }

    Texture* Hud::getTextureAtlas() const
    {
        return m_textureAtlas;
    }

    Hud_ptr Hud::getNextLayer() const
    {
        return m_NextLayer;
    }

    aex::ShaderDrw* Hud::createShader()
    {
        return ShaderDrw::BasicHudElementShaderOrthoOffCenterAtlas();
    }

    void Hud::releaseShader(aex::ShaderDrw* s)
    {
        ShaderDrw::FreeBasicBatchedTextStringAtlas();
    }

    void
    Hud::setQuatCamera(const aex::shared_ptr<aex::QuaternionCamera>& camera)
    {
        m_quatCamera = camera.get();
    }

    void Hud::setQuatCamera(aex::QuaternionCamera* camera)
    {
        m_quatCamera = camera;
    }

    HudElementPtr
    Hud::checkPointIsInHud(float x, float y, bool buttonDown, bool changeTouchValue)
    {
        HudElementPtr ret = nullptr;
        int i = 0;

        for (auto & element : m_hudElements)
        {
            /*
            float posX = element->getPosX() + m_hudOffsetX;
            float posY = element->getPosY() + m_hudOffsetY;

            float sizeX = element->getSizeX();
            float sizeY = element->getSizeY();

            HudHitbox_ptr hitbox = element->getHitbox();

            if (hitbox != nullptr)
            {

                if (hitbox->IsPositionRelativeToElement())
                {
                    posX += hitbox->GetPositionOffset().x* m_scaleX;
                    posY += hitbox->GetPositionOffset().y* m_scaleY;
                } else
                {
                    posX = hitbox->GetPositionOffset().x* m_scaleX;
                    posY = hitbox->GetPositionOffset().y* m_scaleY;
                }

                if (hitbox->IsSizeRelativeToElement())
                {
                    sizeX *= hitbox->GetSizeOffset().x;
                    sizeY *= hitbox->GetSizeOffset().y;
                } else
                {
                    sizeX = hitbox->GetSizeOffset().x* m_scaleX;
                    sizeY = hitbox->GetSizeOffset().y* m_scaleY;
                }

            }
            else
            {

            }*/

            // LOG_DEBUG("posX ", posX, ", sizeX ", sizeX, ", posY ", posY, ", sizeY ", sizeY);
            if (element->isPointIsInElement(aex::math::Vector2(x, y), aex::math::Vector2(m_hudOffsetX, m_hudOffsetY), aex::math::Vector2(m_scaleX, m_scaleY)))
            {
                //LOG_DEBUG(x, " ", posX, " ", (posX + sizeX));
                ret = element;
                if (changeTouchValue)
                    element->setTouched(buttonDown);
                //break;
            }

            i++;
        }

        HudElementPtr temp;
        for (Hud_ptr& layer : m_HudLayers)
        {
            temp = layer->checkPointIsInHud(x, y, buttonDown, changeTouchValue);
            if (temp != nullptr)
            {
                ret = temp;
            }
        }

        return ret;
    }

    HudElementPtr
    Hud::getElementByName(const std::string& name)
    {
        HudElementPtr ret = nullptr;

        for (auto rit = m_hudElements.rbegin(); rit != m_hudElements.rend(); rit++)
        {
            if ((*rit)->getName() == name)
            {
                ret = *rit;
                break;
            }
        }

        if(ret == nullptr && !m_HudLayers.empty())
        {
            for (auto rit = m_HudLayers.rbegin(); rit != m_HudLayers.rend(); rit++)
            {
                HudElementPtr element  = (*rit)->getElementByName(name);
                if (element != nullptr)
                {
                    ret = element;
                    break;
                }

            }
        }

        return ret;
    }

    std::vector<HudElementPtr>& Hud::getElements()
    {
        return m_hudElements;
    }

    void Hud::onOffByName(const std::string& name, bool on)
    {
        HudElementPtr temp = getElementByName(name);

        if (temp != nullptr)
        {
            {
                temp->setVisible(on);
            }
        }
    }

    ElementsMap
    Hud::readTmapFromJson(const std::string& fName) //TODO
    {
        FileManager fManager;
        auto p = fManager.work_dir();
        typedef boost::filesystem::path path;
        // aex::ifstream fileHandle;
        auto fileHandle = fManager.open_read_file(p /= path{fName});
        ElementsMap ret;
        //define file stream object, and open the file

        Json::Value root;
        Json::Reader reader;
        //LOG_DEBUG(fallback.string());

        if (fileHandle.is_open())
        {
            bool parsingSuccessful = reader.parse(fileHandle, root);

            if (!parsingSuccessful)
            {
                // report to the user the failure and their locations in the document.
                LOG_ERR("Failed to parse configuration");
                LOG_ERR(reader.getFormattedErrorMessages());
                return ret;
            }
            Json::Value Elements = root["frames"];
            Json::Value meta = root["meta"];

            float textureSizeX;
            float textureSizeY;
            
            if (!meta.empty())
            {
                textureSizeX = meta["size"]["w"].asFloat();
                textureSizeY = meta["size"]["h"].asFloat();
                if (m_textureAtlas == nullptr)
                {
                    Json::Value file = meta["image"];

                    if (!file.empty())
                    {
                        setTextureAtlas(TextureManager::GetInstance().getTexture("Data/" + file.asString()));
                        //LOG_DEBUG("file in file: ", file.asString());
                    }
                }
            }
            
            UNUSED(textureSizeX);
            UNUSED(textureSizeY);

            if (!Elements.empty())
            {
                std::vector<std::string> filesNames = Elements.getMemberNames();
                float sqr = (filesNames.size() == 1) ? 1 : sqrt((float) filesNames.size());
                float size = 1.0f / floor(sqr+0.000001);
                int counter = sqr;

                //LOG_DEBUG("Elements filesNames.size(): ", filesNames.size());
                for (size_t i = 0; i < filesNames.size(); ++i)
                {
                    //LOG_DEBUG(i, " ", filesNames[i]);
                    Json::Value frameinfo = Elements[filesNames[i]]["frame"];
                    ret.emplace(filesNames[i], aex::make_shared<HudElement>(filesNames[i], frameinfo["x"].asFloat(), frameinfo["y"].asFloat(), frameinfo["w"].asFloat(), frameinfo["h"].asFloat(), 0.1f, 0.1f, size * (float) (i % counter), size * (float) (i / counter)));
                    //m_hudElements.push_back(aex::make_shared<HudElement>(filesNames[i], frameinfo["x"].asFloat(), frameinfo["y"].asFloat(), frameinfo["w"].asFloat(), frameinfo["h"].asFloat(), 0.1f , 0.1f,  size*(float)(i%counter), size * (float)(i/counter)));
                }
            }
        }
        return ret;
    }

    Json::Value getValueCaseInsensitive(const Json::Value& element, std::string valuename)
    {
        Json::Value ret = element[valuename];

        if (ret.empty())
        {
            if (islower(valuename[0]))
            {
                valuename[0] = toupper(valuename[0]);
            } else
            {
                valuename[0] = tolower(valuename[0]);
            }
            ret = element[valuename];
        }

        return ret;
    }

    HudHitbox_ptr readElementHitBox(const Json::Value& element)
    {
        Json::Value jsonPos = getValueCaseInsensitive(element, "position");
        Json::Value jsonSize = getValueCaseInsensitive(element, "size");
        Json::Value jsonSizeRel = getValueCaseInsensitive(element, "sizeIsRelative");
        Json::Value jsonPosRel = getValueCaseInsensitive(element, "positionIsRelative");

        HudHitbox_ptr ret = aex::make_shared<HudHitbox>();

        if (!jsonPos.empty())
        {
            ret->SetPositionOffset(aex::math::Vector2(jsonPos["x"].asFloat(), jsonPos["y"].asFloat()));
        }

        if (!jsonSize.empty())
        {
            ret->SetSizeOffset(aex::math::Vector2(jsonSize["x"].asFloat(), jsonSize["y"].asFloat()));
        }

        if (!jsonSizeRel.empty())
        {
            ret->SetPositionRelativeToElement(jsonSizeRel.asBool());
        }

        if (!jsonPosRel.empty())
        {
            ret->SetSizeRelativeToElement(jsonPosRel.asBool());
        }

        return ret;

    }

    HudElement_ptr readHudElementFromJson(std::string name, const Json::Value& element, ElementsMap& Tmapfile, float scaleX, float scaleY, float scaleInvX, float scaleInvY, float rightOrBottomSizeMove = true)
    {
        //LOG_DEBUG(" ", name);
        Json::Value elementImage = element["image"];

        if (Tmapfile.find(elementImage.asString()) != Tmapfile.end())
        {
            HudElement_ptr temp = aex::make_shared<aex::HudElement>(*Tmapfile.find(elementImage.asString())->second);
            Json::Value elementName = element["name"];
            if (elementName.empty())
            {
                elementName = element["Name"];
            }

            if (!elementName.empty())
            {
                name = elementName.asString();
            }

            temp->setName(name);
            Json::Value elementScale = element["scale"];
            float fieleScaleX = 1.0f;
            float fieleScaleY = 1.0f;

            if (!elementScale.empty())
            {
                if (!element["scale"]["x"].empty())
                {
                    fieleScaleX = element["scale" ]["x"].asFloat();
                }

                if (!element["scale"]["y"].empty())
                {
                    fieleScaleY = element["scale"]["y"].asFloat();
                }

                if (!element["scale"]["u"].empty())
                {
                    fieleScaleY = fieleScaleX = element["scale"]["u"].asFloat();
                }
            }

            Json::Value elementSize = element["size"];
            if (!elementSize.empty())
            {
                temp->setSize(elementSize["x"].asFloat() * scaleX * fieleScaleX, elementSize["y"].asFloat() * scaleY * fieleScaleY);
            }

            Json::Value HorizontalAlignment = element["HorizontalAlignment"];
            if (HorizontalAlignment.empty())
            {
                HorizontalAlignment = element["horizontalAlignment"];
            }

            Json::Value VeriticalAlignment = element["VerticalAlignment"];
            if (VeriticalAlignment.empty())
            {
                VeriticalAlignment = element["verticalAlignment"];
            }

            if(VeriticalAlignment.empty())
            {
                VeriticalAlignment = element["VeriticalAlignment"];
                if (VeriticalAlignment.empty())
                {
                    VeriticalAlignment = element["veriticalAlignment"];
                }
            }



            Json::Value elementOffset = element["position"];
            float posX = elementOffset["x"].asFloat();
            float posY = elementOffset["y"].asFloat();


            if (!HorizontalAlignment.empty())
            {
                if (HorizontalAlignment.asString() == "left")
                {

                } else if (HorizontalAlignment.asString() == "right")
                {
                    if (rightOrBottomSizeMove)
                    {
                        posX = (scaleInvX - posX) - (temp->getSizeX() * scaleInvX);
                    } else
                    {
                        posX = (scaleInvX - posX);
                    }
                } else if (HorizontalAlignment.asString() == "center")
                {
                    posX += 0.5f * scaleInvX;
                }
            } else
            {
                //posX*=scaleX;
            }

            if (!VeriticalAlignment.empty())
            {
                if (VeriticalAlignment.asString() == "top")
                {
                    //posY*=scaleInvY;
                } else if (VeriticalAlignment.asString() == "bottom")
                {
                    if (rightOrBottomSizeMove)
                    {
                        posY = (scaleInvY - posY) - (temp->getSizeY() * scaleInvY);
                    } else
                    {
                        posY = (scaleInvY - posY);
                    }
                } else if (VeriticalAlignment.asString() == "center")
                {
                    posY += 0.5f * scaleInvY;
                }
            } else
            {
                //posY*=scaleY;
            }


            temp->setPos(posX * scaleX, posY * scaleY);

            Json::Value isVisible = element["visible"];
            if (isVisible.empty())
            {
                isVisible = element["Visible"];
            }

            if (!isVisible.empty())
            {
                temp->setVisible(isVisible.asBool());
            }

            Json::Value isClickable = element["clickable"];
            if (isClickable.empty())
            {
                isClickable = element["Clickable"];
            }
            if (!isClickable.empty())
            {
                temp->setClickable(isClickable.asBool());
            }

            Json::Value needBlending = element["needBlending"];
            if (needBlending.empty())
            {
                needBlending = element["NeedBlending"];
            }


            if (!needBlending.empty())
            {
                temp->setCanChangeBlendState(true);
                temp->setTargetBlendState(needBlending.asBool());
            }

            Json::Value hitbox = getValueCaseInsensitive(element, "Hitbox");

            if (!hitbox.empty())
            {
                temp->setHitbox(readElementHitBox(hitbox));
            }

            // parse hudElem on touch representation if needed
            Json::Value jsHudElemOnTouch = element["hudElementOnTouch"];
            if (!jsHudElemOnTouch.empty())
            {
                HudElement_ptr hudElemOnTouch = readHudElementFromJson("nameNotImportant", jsHudElemOnTouch, Tmapfile, scaleX, scaleY, scaleInvX, scaleInvY, rightOrBottomSizeMove);
                if (hudElemOnTouch.get() != nullptr)
                {
                    temp->setHudElemOnTouch(hudElemOnTouch);
                }
            } else
            {
                // Dont have
            }
            return temp;
        }
        return nullptr;
    }

    void Hud::readHudFromJson(const std::string& fName, bool forceLandscape, bool forcePortait, bool rightOrBottomSizeTo00)
    {
        FileManager fManager;
        auto p = fManager.work_dir();
        typedef boost::filesystem::path path;
        auto fileHandle = fManager.open_read_file(p /= path{fName});


        if (fileHandle.is_open())
        {

            float scaleX = 1.0f;
            float scaleY = 1.0f;

            if (m_quatCamera != nullptr)
            {
                int widthPix = m_quatCamera->getWindowW();
                int heightPix = m_quatCamera->getWindowH();

                LOG(m_quatCamera->getWindowW(), " - ", m_quatCamera->getWindowH());

                if (forceLandscape)
                {
                    if (widthPix < heightPix)
                        std::swap(widthPix, heightPix);
                } else
                    if (forcePortait)
                {
                    if (heightPix < widthPix)
                        std::swap(widthPix, heightPix);
                }

                if (heightPix < widthPix)
                {
                    scaleX = (float) heightPix / (float) widthPix;
                } else
                {
                    scaleY = (float) widthPix / (float) heightPix;
                }
            }
            float scaleInvX = 1.0f / scaleX;
            float scaleInvY = 1.0f / scaleY;

            Json::Value root;
            Json::Reader reader;

            bool parsingSuccessful = reader.parse(fileHandle, root);

            if (!parsingSuccessful)
            {
                // report to the user the failure and their locations in the document.
                LOG_ERR("Failed to parse configuration");
                LOG_ERR(reader.getFormattedErrorMessages());
                return;
            }

            Json::Value Elements = root["Hud"];
            Json::Value meta = root["meta"];
            Json::Value tmapfile = root["TmapFile"];
            if (!meta.empty())
            {
                if (!meta["TmapFile"].empty())
                {
                    tmapfile = meta["TmapFile"];
                }

            }

            ElementsMap Tmapfile = readTmapFromJson(tmapfile.asString());
            if (Tmapfile.size() == 0)
            {
                LOG_ERR("Failed to parse Tmap configuration");
                LOG_ERR(reader.getFormattedErrorMessages());
                return;
            }

            if (!Elements.empty())
            {
                std::vector<std::string> filesNames;
                int size = 0;

                if (Elements.isArray())
                    size = Elements.size();
                else
                {
                    filesNames = Elements.getMemberNames();
                    size = filesNames.size();
                }

                //  LOG_DEBUG("Elements filesNames.size(): ",);
                for (int i = 0; i < size; ++i)
                {
                    Json::Value elementImage;
                    Json::Value element;
                    std::string name;

                    if (Elements.isArray())
                    {
                        element = Elements[i];
                        std::vector<std::string> names = element.getMemberNames();
                        if (names.size() > 0)
                        {
                            name = names[0];
                        }
                        element = element[name];
                    } else
                    {
                        element = Elements[filesNames[i]];
                        name = filesNames[i];
                    }

                    HudElement_ptr temp = readHudElementFromJson(name, element, Tmapfile, scaleX, scaleY, scaleInvX, scaleInvY, rightOrBottomSizeTo00);
                    if (temp)
                    {
                        m_hudElements.push_back(temp);
                    }
                }

            }
        } else {
            std::string err_msg = "Could not open file ";
            err_msg += fName+".";
            throw HudParseError(err_msg);
        }
    }

    void
    Hud::readHudFromTpacker(const std::string & fName)
    {

        std::string delimiter = ":";

        FileManager fManager;
        auto p = fManager.work_dir();
        typedef boost::filesystem::path path;
        // aex::ifstream fileHandle;
        auto fileHandle = fManager.open_read_file(p /= path{fName});

        //define file stream object, and open the file

        if (fileHandle)
        {

            std::string line = "";

            while (getline(fileHandle, line))
            {
                // std::cout<<line<<std::endl;

                std::stringstream spliter(line);
                std::string token;
                std::vector<std::string> tokens;

                while (getline(spliter, token, ':'))
                {
                    //LOG_DEBUG(token);
                    tokens.push_back(token);
                }

                if (tokens.size() == 9)
                {
                    //std::cout << "addin hud element" << std::endl;
                    m_hudElements.push_back(HudElement::makeHudElement(
                            tokens[0],
                            (float) std::atof(tokens[1].c_str()),
                            (float) std::atof(tokens[2].c_str()),
                            (float) std::atof(tokens[3].c_str()),
                            (float) std::atof(tokens[4].c_str()),
                            (float) std::atof(tokens[5].c_str()),
                            (float) std::atof(tokens[6].c_str()),
                            (float) std::atof(tokens[7].c_str()),
                            (float) std::atof(tokens[8].c_str()))
                            );
                }
            }
        }

        fileHandle.close();
    }

    void
    Hud::draw(AexRenderer * gl)
    {

        if (!m_shaderLoaded)
        {
            if (m_shader)
            {
                m_cleanup(m_shader);
            }

            m_shader = this->createShader();
            m_cleanup = [this](ShaderDrw * s)
            {
                releaseShader(s);
            };
            m_shaderLoaded = true;
        }

        m_shader->draw(gl);
        const AexShader& program = m_shader->GetProgram();

        if (m_textureAtlas)
        {
            m_textureAtlas->bind();
            m_HudAtlasUniform.setValue(m_textureAtlas);
            m_HudAtlasUniform.setUniform(program);

            m_hudTxtSizeU.setValue(float(m_textureAtlas->getSizeW()),float(m_textureAtlas->getSizeH()));
            m_hudTxtSizeU.setUniform(program);
        }


        if (!m_initialized)
        {
            m_ProjectionMatrixU.setValue(gl->getQuatCamera()->get_ortho_matrix().getM());
            m_ProjectionMatrixU.setUniform(program);

            m_initialized = true;
        }

        if (m_canChangeDepthTestState)
        {
            m_isDepthTestEnabledChange = gl->setDepthTestState(false);

        }

        if (m_canChangeBlendState)
        {
            m_blendStatechanged = gl->setBlendState(true);
        }

        program->setVertexData(*m_spriteVbo);
        program->setIndices(*m_indexVbo);

        m_hudOffsetU.setValue(m_hudOffsetX, m_hudOffsetY);
        m_hudOffsetU.setUniform(program);


        bool settedUPtr = false;
        if (m_hudElementsDrawPtr == nullptr)
        {
            m_hudElementsDrawPtr = &m_hudElements;
            settedUPtr = true;
        }
        for (auto hudElement : *m_hudElementsDrawPtr)
        {
            if (hudElement->isVisible())
            {
                hudElement->draw(gl);
                hudElement->finishdraw(gl);
            }
        }
        if (settedUPtr)
        {
            m_hudElementsDrawPtr = nullptr;
        }

        if (m_HudLayers.size() > 0)
        {

            if (m_textureAtlas)
            {
                m_textureAtlas->unbind();
            }

            for (Hud_ptr& layer : m_HudLayers)
            {
                layer->draw(gl);
                layer->finishdraw(gl);
            }
        }
        /*if (m_NextLayer != nullptr)
        {

            if (m_textureAtlas)
            {
                m_textureAtlas->unbind();
            }

            m_NextLayer->draw(gl);
            m_NextLayer->finishdraw(gl);
        }*/

    }

    void
    Hud::finishdraw(AexRenderer * gl)
    {
        if (m_canChangeDepthTestState)
        {
            gl->setDepthTestState(true);
            m_isDepthTestEnabledChange = false;
        }

        if (m_canChangeDepthTestState)
        {
            gl->setBlendState(false);
            m_blendStatechanged = false;
        }

        //m_initialized = false;

        const AexShader& program = m_shader->GetProgram();
        if (program)
        {
            program->releaseVertexData(*m_spriteVbo);
        }

        if (m_textureAtlas && !(m_HudLayers.size() > 0))
        {
            m_textureAtlas->unbind();
        }

        m_shader->finishdraw(gl);
    }

    void
    Hud::onContextDirty()
    {
        m_initialized = false;
    }

    Hud::~Hud()
    {
        if (m_textureAtlas)
        {
            TextureManager::GetInstance().releaseTexture(m_textureAtlas);
        }

        if (m_spriteVbo)
        {
            if (m_centerPosition)
            {
                releaseVertCenterRoot();
            } else
            {
                releaseVertCornerRoot();
            }
        }

        if (m_indexVbo)
        {
            releaseIndexStrip();
        }
    }

}
