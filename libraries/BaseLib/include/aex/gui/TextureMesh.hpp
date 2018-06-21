#ifndef TEXTURE_MESH_AEX_GUI_HEADER
#define TEXTURE_MESH_AEX_GUI_HEADER

#include "Mesh.hpp"
#include "ShaderManager.hpp"
#include "Color.hpp"

namespace aex {
    namespace gui {

        class AEX_PUBLIC_API TextureMesh : public Mesh {
        public:
            static const char* ShaderName;
            static const char* Attribs;
            static const char* VSH;
            static const char* FSH;

            TextureMesh(Texture* texture, const TriangleBatch& batch);
            TextureMesh(TextureMesh& cm) = delete;
            TextureMesh(TextureMesh&& cm) = delete;
            virtual ~TextureMesh();

            void setTexture(Texture* texture);
            void setColorMod(const Color& color);
            const Color& getColorMod() const;
            virtual void batchChanged();

            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();

        protected:
            Texture* m_texture;
            UniformSampler m_sampler;
            Color m_colorMod;
            UniformFloat4 m_uColorMod;

            ShaderManager::handle_t m_shaderHandle;
            ShaderDrw* m_shader;
        };
    }
}

#endif
