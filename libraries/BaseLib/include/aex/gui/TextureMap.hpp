#ifndef TEXTURE_MAP_AEX_GUI_HEADER
#define TEXTURE_MAP_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Sprite.hpp"
#include <memory>


namespace aex {
    class Texture;
    namespace gui {

        class TextureMap {
        public:

            struct SpriteInfo {
                std::string name;
                GArea2D<int> frame;
                bool rotated;
                bool trimmed;
                GArea2D<int> spriteSourceSize;
                GArea2D<int> sourceSize;
                float pivotX, pivotY;
                friend std::ostream& operator<<(std::ostream& out, const TextureMap::SpriteInfo& si);
            };

            struct Meta {
                std::string app;
                std::string version;
                std::string image;
                std::string format;
                GSize2D<int> size;
                float scale;
                std::string smartupdate;
                friend std::ostream& operator<<(std::ostream& out, const TextureMap::Meta& m);
            };

        public:
            virtual ~TextureMap();
            TextureMap(const std::string& jsonPath);
            TextureMap(const std::string& jsonPath, const std::string& imgPath);
            TextureMap(const std::string& jsonPath, GSize2D<int> mockupSize);
            std::vector<std::string> getNames() const;
            void setMockupSize(GSize2D<int> mockupSize);
            void setTextureConfig(const TextureConfig& tc);
            std::unique_ptr<Sprite> getSprite(const std::string& name, const Area& area, Position atPos = Position());
            std::unique_ptr<Sprite> getSprite(const std::string& name, Position atPos = Position());
            std::unique_ptr<Sprite> getSprite(const std::string& name, float normWidth, Position atPos = Position());
            std::unique_ptr<Sprite> getSpriteByHeight(const std::string& name, float normHeight, Position atPos = Position());

          protected:
            std::unique_ptr<Sprite> generateSprite(const SpriteInfo* si, const Area& spriteArea);
            SpriteInfo* getSpriteInfo(const std::string& name);
        protected:
            void parseJsonFile(const std::string& jsonPath);

        protected:
            std::string m_jsonPath;
            std::vector<SpriteInfo> m_spritesInfos;
            Meta m_meta;
            TextureConfig m_tCfg;
            GSize2D<int> m_mockupSize;
            bool m_bImgPathFromJson;
            std::string m_imgPath;
        };

        std::ostream& operator<<(std::ostream& out, const TextureMap::SpriteInfo& si);
        std::ostream& operator<<(std::ostream& out, const TextureMap::Meta& m);
    }
}


#endif
