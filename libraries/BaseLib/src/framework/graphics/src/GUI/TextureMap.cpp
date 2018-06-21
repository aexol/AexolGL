#include "aex/gui/TextureMap.hpp"
#include "aex/gui/Sprite.hpp"
#include "aex/gui/Frame.hpp"

#include <aex/Common.hpp>
#include <aex/Exceptions.hpp>
#include <aex/Logger.hpp>
#include <aex/FileManager.hpp>
#include <aex/Texture.hpp>

#include <json/json.h>

#include <string>
#include <sstream>
#include <cmath>

namespace aex
{
    namespace gui
    {

        TextureMap::~TextureMap()
        {
        }

        TextureMap::TextureMap(const std::string& jsonPath) :
        TextureMap(jsonPath,GSize2D<int>(Frame::getWidthPixels(), Frame::getHeightPixels()) )
        {}

        TextureMap::TextureMap(const std::string& jsonPath, const std::string& imgPath) :
        TextureMap(jsonPath,GSize2D<int>(Frame::getWidthPixels(), Frame::getHeightPixels()) )
        {
          m_bImgPathFromJson = false;
          m_imgPath = imgPath;
        }

        TextureMap::TextureMap(const std::string& jsonPath, GSize2D<int> mockupSize) :
        m_jsonPath(jsonPath),
        m_mockupSize(mockupSize),
        m_bImgPathFromJson(true)
        {
            parseJsonFile(jsonPath);
            m_tCfg = TextureConfig::makeDefaultTextureConfig();
        }

        std::vector<std::string> TextureMap::getNames() const
        {
            std::vector<std::string> names;
            for (size_t i = 0; i < m_spritesInfos.size(); i++)
            {
                names.push_back(m_spritesInfos[i].name);
            }

            return names;
        }

        void TextureMap::setMockupSize(GSize2D<int> mockupSize)
        {
            m_mockupSize = mockupSize;
        }

        void TextureMap::setTextureConfig(const TextureConfig& tc)
        {
            m_tCfg = tc;
        }

        std::unique_ptr<Sprite> TextureMap::getSprite(const std::string& name, const Area& area, Position atPos)
        {
            SpriteInfo* si = getSpriteInfo(name);
            if(!si)
              return nullptr;

            return generateSprite(si, area);
        }

        std::unique_ptr<Sprite> TextureMap::getSprite(const std::string& name, Position atPos)
        {
            SpriteInfo* si = getSpriteInfo(name);
            if(!si)
              return nullptr;

            unsigned smallerMockupSize = std::min(m_mockupSize.w(), m_mockupSize.h());
            float normalizedW = si->frame.s().w() / (float)smallerMockupSize;
            float normalizedH = si->frame.s().h() / (float)smallerMockupSize;
            aex::gui::Area spriteArea(atPos, aex::gui::Size(normalizedW, normalizedH));
            return generateSprite(si, spriteArea);
        }

        std::unique_ptr<Sprite> TextureMap::getSprite(const std::string& name, float normWidth, Position atPos)
        {
            SpriteInfo* si = getSpriteInfo(name);
            if(!si)
              return nullptr;

            unsigned smallerMockupSize = std::min(m_mockupSize.w(), m_mockupSize.h());
            float normalizedW = si->frame.s().w() / (float)smallerMockupSize;
            float normalizedH = si->frame.s().h() / (float)smallerMockupSize;
            normalizedH = (normWidth / normalizedW) * normalizedH;
            normalizedW = normWidth;
            aex::gui::Area spriteArea(atPos, aex::gui::Size(normalizedW, normalizedH));
            return generateSprite(si, spriteArea);
        }

        std::unique_ptr<Sprite> TextureMap::getSpriteByHeight(const std::string& name, float normHeight, Position atPos)
        {
          SpriteInfo* si = getSpriteInfo(name);
          if(!si)
            return nullptr;

          unsigned smallerMockupSize = std::min(m_mockupSize.w(), m_mockupSize.h());
          float normalizedW = si->frame.s().w() / (float)smallerMockupSize;
          float normalizedH = si->frame.s().h() / (float)smallerMockupSize;
          normalizedW = (normHeight / normalizedH) * normalizedW;
          normalizedH = normHeight;
          aex::gui::Area spriteArea(atPos, aex::gui::Size(normalizedW, normalizedH));
          return generateSprite(si, spriteArea);
        }

        std::unique_ptr<Sprite> TextureMap::generateSprite(const SpriteInfo* si, const Area& spriteArea)
        {
          GPosition2D<int> leftUp(si->frame.p().x(), si->frame.p().y());
          GPosition2D<int> leftDown(si->frame.p().x(), si->frame.p().y() + si->frame.s().h() );
          GPosition2D<int> rightUp(si->frame.p().x()+si->frame.s().w(), si->frame.p().y());
          GPosition2D<int> rightDown(si->frame.p().x()+si->frame.s().w(), si->frame.p().y()+si->frame.s().h() );

          GSize2D<int> texSize = m_meta.size;

          //float bleedingDelta = 0.5; // add or remove from every UV corner if wanna implement in for linear filters, and add parametrize for user

          Position leftUpUV( (leftUp.x() /(float)texSize.w()),  (leftUp.y()/(float)texSize.h()) );
          Position leftDownUV( (leftDown.x() /(float)texSize.w()),  (leftDown.y()/(float)texSize.h()) );
          Position rightUpUV( (rightUp.x() /(float)texSize.w()), (rightUp.y()/(float)texSize.h()) );
          Position rightDownUV( (rightDown.x() /(float)texSize.w()), (rightDown.y()/(float)texSize.h()) );

          Area UVs(leftDownUV,  aex::gui::Size(rightUpUV.x() - leftUpUV.x(), rightUpUV.y()-rightDownUV.y()) );

          std::stringstream ss;
          ss << "Data/" << m_meta.image;

          std::string imgPath;
          if(m_bImgPathFromJson)
            imgPath = ss.str();
          else
            imgPath = m_imgPath;

          std::unique_ptr<Sprite> nextSprite(new Sprite(spriteArea, UVs, imgPath, m_tCfg));
          return nextSprite;
        }

        TextureMap::SpriteInfo* TextureMap::getSpriteInfo(const std::string& name)
        {
          SpriteInfo* si = nullptr;
          for(size_t i = 0 ; i < m_spritesInfos.size() ; i++)
          {
              if(name == m_spritesInfos[i].name)
              {
                  si = &m_spritesInfos[i];
                  break;
              }
          }
          return si;
        }

        void TextureMap::parseJsonFile(const std::string& jsonPath)
        {
            m_spritesInfos.clear();

            FileManager fManager;
            auto p = fManager.work_dir();
            typedef boost::filesystem::path path;
            auto fileHandle = fManager.open_read_file(p /= path{jsonPath});


            if (!fileHandle.is_open())
            {
                std::stringstream ss;
                ss << "Could not open " << jsonPath << " file";
                LOG_ERR(ss.str());
                throw aex::FileNotFoundError(ss.str());
            }

            Json::Value root;
            Json::Reader reader;
            if (!reader.parse(fileHandle, root))
            {
                std::stringstream ss;
                ss << "Failed to parse texture map json file: " << jsonPath << ", Err msg: " << reader.getFormattedErrorMessages();
                LOG_ERR(ss.str());
                throw aex::JsonError(ss.str());
            }

            Json::Value frames = root["frames"];
            Json::Value meta = root["meta"];

            m_meta.app = meta["app"].asString();
            m_meta.version = meta["version"].asString();
            m_meta.image = meta["image"].asString();
            m_meta.format = meta["format"].asString();
            m_meta.size.setW( meta["size"]["w"].asUInt());
            m_meta.size.setH( meta["size"]["h"].asUInt());
            m_meta.scale = atof(meta["scale"].asString().c_str());
            m_meta.smartupdate = meta["smartupdate"].asString();

            Json::Value::Members members = frames.getMemberNames();

            for (const string& member : members)
            {
                Json::Value jvFrame = frames[member]["frame"];
                Json::Value jvRotated = frames[member]["rotated"];
                Json::Value jvTrimmed = frames[member]["trimmed"];
                Json::Value jvSpriteSourceSize = frames[member]["spriteSourceSize"];
                Json::Value jvSourceSize = frames[member]["sourceSize"];
                Json::Value jvPivot = frames[member]["pivot"];

                SpriteInfo si;
                si.name = member;
                si.frame = GArea2D<int>(GPosition2D<int>(jvFrame["x"].asUInt(), jvFrame["y"].asUInt()),
                        GSize2D<int>(jvFrame["w"].asUInt(), jvFrame["h"].asUInt()));
                si.rotated = jvRotated.asBool();
                si.trimmed = jvTrimmed.asBool();
                si.spriteSourceSize = GArea2D<int>(GPosition2D<int>(jvSpriteSourceSize["x"].asUInt(), jvSpriteSourceSize["y"].asUInt()),
                        GSize2D<int>(jvSpriteSourceSize["w"].asUInt(), jvSpriteSourceSize["h"].asUInt()));
                si.sourceSize = GSize2D<int>(jvSourceSize["w"].asUInt(), jvSourceSize["h"].asUInt());
                si.pivotX = jvPivot["x"].asFloat();
                si.pivotY = jvPivot["y"].asFloat();

                m_spritesInfos.push_back(si);
            }
        }

        std::ostream& operator<<(std::ostream& out, const TextureMap::SpriteInfo& si)
        {
            out << "name: " << si.name << std::endl;
            out << "frame: " << si.frame << std::endl;
            out << "rotated: " << si.rotated << std::endl;
            out << "trimmed: " << si.trimmed << std::endl;
            out << "spriteSourceSize: " << si.spriteSourceSize << std::endl;
            out << "sourceSize: " << si.sourceSize << std::endl;
            out << "pivot: " << si.pivotX << ", " << si.pivotY;
            return out;
        }

        std::ostream& operator<<(std::ostream& out, const TextureMap::Meta& m)
        {
            out << "app: " << m.app << std::endl;
            out << "version: " << m.version << std::endl;
            out << "image: " << m.image << std::endl;
            out << "format: " << m.format << std::endl;
            out << "size: " << m.size << std::endl;
            out << "scale: " << m.scale << std::endl;
            out << "smartupdate: " << m.smartupdate;
            return out;
        }

    }
}
