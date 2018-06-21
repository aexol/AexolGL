/*
 * File:   TextureManager.hpp
 * Author: mritke
 *
 * Created on February 4, 2014, 10:49 AM
 */

#ifndef TEXTUREMANAGER_HPP
#define	TEXTUREMANAGER_HPP

#include <aex/Common.hpp>
#include <aex/FileManager.hpp>
#include <vector>
#include <mutex>

namespace aex
{
    class Texture;
    class TextureConfig;
    class AEX_PUBLIC_API TextureManager
    {

    public:
        static TextureManager& GetInstance();
    
        Texture* getTexture(const aex::string& TexName, bool isMipMapped = true);
        Texture* getTexture(const aex::string& TexName, const TextureConfig& cfg);
        Texture* getEmptyTexture(const aex::string& TexName);
        Texture* getEmptyTexture(const aex::string& TexName, const TextureConfig& cfg);
        aex::string getTextureName(Texture* tex);
        void releaseTexture(Texture* tex);
        void releaseTexture(const aex::string& texName);
        ~TextureManager();
        //~AexTimer();
        
private:

        std::vector<aex::shared_ptr<Texture>> m_resources;
        std::vector<int32_t> m_resRevCount;
        std::vector<aex::string> m_resourceNames;
        aexFramework::FileManager fManager;

        TextureManager();
        TextureManager(const TextureManager& rs) = delete;
        TextureManager& operator =(const TextureManager& rs) = delete;
        Texture* findTexture(const aex::string& tKey);
        void addTexture(const aex::string& tKey, const aex::shared_ptr<Texture>& texture);
        
        
    };



}

#endif	/* TEXTUREMANAGER_HPP */

