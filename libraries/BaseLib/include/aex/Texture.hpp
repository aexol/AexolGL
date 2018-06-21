/** @file Texture.hpp
 *  @brief Definition of Texture.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__Texture__
#define __aexolGL__Texture__

#include <aex/AexTypes.hpp>
#include <aex/GPUApi.hpp>
#include <aex/AexDrawable.hpp>
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <array>
#include <memory>

namespace aex
{
    class MtlParser;
	class Camera;

    class AEX_PUBLIC_API TextureConfig
    {
    public:
        TextureConfig(uint32_t textureType,
                      uint32_t format,
                      uint32_t dataType,
                      uint32_t minFilter,
                      uint32_t magFilter,
                      uint32_t wrapS,
                      uint32_t wrapT,
                      bool generateMipMaps);
        TextureConfig();
        static TextureConfig makeDefaultTextureConfig();
        static TextureConfig makeDefaultTextureCubeConfig();
        static TextureConfig makeDefaultTextureCameraConfig();
        void setRGBFormat();
        void setRGBAFormat();
        void setDepthComponentFormat();
        void setAlphaChannelFormat();
        void setCubeTextureType();
        void set2DTextureType();
        void set3DTextureType();
        void set8bitPerChannel();
        void set16bit565();
        void set16bit4444();
        void set16bit5551();
        void set16bitDepth();
        void set24bitDepth();
        void set32bitDepth();
        void setMagFilterNearest();
        void setMagFilterLinear();
        void setMinFilterNearest();
        void setMinFilterLinear();
        void setMinFilterNearestMipmapNearest();
        void setMinFilterLinearMipmapNearest();
        void setMinFilterNearestMipmapLinear();
        void setMinFilterLinearMipmapLinear();
        void setWrapSClamToEdge();
        void setWrapTClamToEdge();
        void setWrapSRepeat();
        void setWrapTRepeat();
        void setWrapSMirroredRepeat();
        void setWrapTMirroredRepeat();
        void enableMipMapping();
        void disableMipMapping();
        void toggleGenerateMipmaps();
        uint32_t getTextureType() const
        {
            return m_textureType;
        }
        uint32_t getFormat() const
        {
            return m_format;
        }
        uint32_t getDataType() const
        {
            return m_dataType;
        }
        uint32_t getMinFilter() const
        {
            return m_minFilter;
        }
        uint32_t getMagFilter() const
        {
            return m_magFilter;
        }
        uint32_t getWrapS() const
        {
            return m_wrapS;
        }
        uint32_t getWrapT() const
        {
            return m_wrapT;
        }
        bool hasGeneratedMipmaps() const
        {
            return m_generateMipmaps;
        }
        
        bool operator==(const TextureConfig& cfg) const;
    private:
        uint32_t m_textureType;
        uint32_t m_format;
        uint32_t m_dataType;
        uint32_t m_minFilter;
        uint32_t m_magFilter;
        uint32_t m_wrapS;
        uint32_t m_wrapT;
        bool m_generateMipmaps;
        friend class Texture;
    };

    class AEX_PUBLIC_API Texture : public IOnContextDirty
    {
    public:
        enum class CubeSide 
        {
            POS_X,
            NEG_X,
            POS_Y,
            NEG_Y,
            POS_Z,
            NEG_Z
        };
    private:
        struct delete_texture_impl
        {
            void operator()(void* p);
        };
    public:
        typedef std::unique_ptr<void, delete_texture_impl> impl_ptr;
        typedef std::pair<aex::string, CubeSide> FileKey;
        typedef std::pair<void*, CubeSide> DataKey;
        typedef std::array<FileKey, 6> CubeDataArray;

        Texture();
        explicit Texture(const TextureConfig& cfg);
        explicit Texture(const aex::string& filename);
        Texture(const aex::string& name, const TextureConfig& cfg);
        explicit Texture(const std::array<FileKey, 6>& filename);
        Texture(const std::array<FileKey, 6>& filenames, const TextureConfig& cfg);
        explicit Texture(void* camImplPtr);
        /**
         *	@brief	Ctor
         *
         *	@param 	name 	Array of filenames and targets.
         *	@param 	target 	Target.
         *	@param 	format 	Tex format.
         *	@param 	type 	Data type.
         *	@param 	minFilter 	Minifying function.
         *	@param 	magFilter 	Magnifying function.
         *	@param 	wrapS 	Coord S wrap param.
         *	@param 	wrapT 	Coord T wrap param.
         */
        Texture(Texture&& tex);
        Texture& operator=(Texture&& tex);

        /**
         *	@brief	Bind texture to texture unit.
         */
        void bind();
        void handle();
        /**
         *	@brief	Unbind texture.
         */
        void unbind();
        void unload();
        /**
         *	@brief	Get texture id.
         *
         *	@return	texId.
         */
        virtual ~Texture();
        /**
         *	@brief	Load texture with data.
         *
         *	@param 	name 	Array of filenames and targets.
         *	@param 	target 	Target.
         *	@param 	format 	Tex format.
         *	@param 	type 	Data type.
         *	@param 	minFilter 	Minifying function.
         *	@param 	magFilter 	Magnifying function.
         *	@param 	wrapS 	Coord S wrap param.
         *	@param 	wrapT 	Coord T wrap param.
         */
        void load(const aex::string& filename);
        void load(const aex::string& filename, const TextureConfig& cfg);
        void load(const std::array<FileKey, 6>& filenames);
        void load(const std::array<FileKey, 6>& filenames, const TextureConfig& cfg);
        void setName(const aex::string& name);
        const aex::string& getName() const;
        void setFilename(const aex::string& name);
        void setFilename(const std::array<FileKey, 6>& name);
        void setData(void* data, uint32_t w, uint32_t h);
        void setData(void* data, uint32_t w, uint32_t h, uint32_t d);
        void setData(const std::array<DataKey, 6>& data, uint32_t w, uint32_t h);
        void* getData(void* data = nullptr , int lvl = 0 );
        void allocate2D(uint32_t w, uint32_t h);
        void allocateCube(uint32_t w, uint32_t h);
        void allocateCubeFramebuffer(uint32_t w, uint32_t h);
        void setMipmap(uint32_t level);
        void setMipmap(const std::array<FileKey, 6>& name, uint32_t level);
        void setRawMipmapData(void* data, uint32_t w, uint32_t h, uint32_t level);
        void setRawMipmapData(const std::array<DataKey, 6>& data, uint32_t w, uint32_t h, uint32_t level);
        void setSizeH(uint32_t sizeH);
        uint32_t getSizeH() const;
        void setSizeW(uint32_t sizeW);
        uint32_t getSizeW() const;
        const void* getTexture() const;
        TextureConfig getTextureConfig() const;
    protected:
        void onContextDirty();
    private:
        void finalize();
        void handle2D();
        void handleCube();
        bool m_isBound=false;
        uint32_t m_sizeW;
        uint32_t m_sizeH;
        uint32_t m_sizeD;
        TextureConfig m_cfg;
        impl_ptr m_tex;
        bool m_created;
        std::vector<FileKey> m_filename;
        aex::string m_name;
        friend class aex::MtlParser;
    };
    
     #ifdef TEXDRW
    
    class AexRenderer;
    class UniformSampler;
    
    class AEX_PUBLIC_API TextureDrw: public DrawObject
    {
        Texture* m_texture;
        UniformSampler m_samplerUniform;
        TextureDrw(aex::Texture* tex, const std::string& name );
        void draw(AexRenderer* gl);
        void finishdraw(AexRenderer* gl);

        void needsUpdate(){};
    };
    
    #endif
}

#endif /* defined(__aexolGL__Texture__) */
