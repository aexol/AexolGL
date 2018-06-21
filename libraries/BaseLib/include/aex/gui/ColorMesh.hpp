#ifndef COLOR_MESH_AEX_GUI_HEADER
#define COLOR_MESH_AEX_GUI_HEADER

#include "Mesh.hpp"
#include "Color.hpp"
#include "ShaderManager.hpp"

namespace aex {
    namespace gui {
        /**
         * @brief Mesh used to draw a batch of triangles with color.
         *
         * @details This a simple effect that draw users batch
         * of triangles with color.
         */
        class AEX_PUBLIC_API ColorMesh : public Mesh {
        public:
            /**
             * @brief New ColorMesh.
             *
             * @param color Color.
             * @param Batch of triangles.
             */
            ColorMesh(const Color& color, const TriangleBatch& batch);
            ColorMesh(ColorMesh& cm) = delete;
            ColorMesh(ColorMesh&& cm) = delete;
            virtual ~ColorMesh();

            /**
             * @brief Change drawing color.
             *
             * @param color New color.
             */
            void setColor(const Color& color);
            /**
             * @brief Returns current color.
             *
             * @return Color.
             */
            const Color& getColor() const;

            virtual void batchChanged();

            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();

        protected:
            Color m_color;
            UniformFloat4 m_uColor;

            ShaderManager::handle_t m_shaderHandle;
            ShaderDrw* m_shader;
        };
    }
}

#endif
