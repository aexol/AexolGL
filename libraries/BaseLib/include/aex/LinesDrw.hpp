/*
 * File:   LinesDrw.hpp
 * Author: mritke
 *
 * Created on September 17, 2014, 1:43 PM
 */

#ifndef LINESDRW_HPP
#define	LINESDRW_HPP
#include <aex/Common.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/ShaderDrw.hpp>
#include <aex/VertexData.hpp>

namespace aex
{


    class AEX_PUBLIC_API LinesDrw;
    class VertexData;

    typedef shared_ptr<LinesDrw> LinesDrw_ptr;

    class AEX_PUBLIC_API LinesDrw : public DrawObject
    {
        
    public:
        LinesDrw();
        LinesDrw(const std::vector<mesh_vectors_type>& vertices, aex::math::Vector3 lineColor, bool isLineStrip = true);
        LinesDrw(const std::vector<mesh_indices_type>& indices, const std::vector<mesh_vectors_type>& vertices, aex::math::Vector3 lineColor, bool isLineStrip = false);
        LinesDrw(const std::vector<mesh_indices_type>& indices, const std::vector<mesh_vectors_type>& vertices, std::vector<mesh_vectors_type>& color, bool isLineStrip = false);
        ~LinesDrw();
        virtual void draw(AexRenderer* gl);
        virtual void finishdraw(AexRenderer* gl);

        void needsUpdate()
        {
        };

        void setLineWidth(float width);
        void setShape(const std::vector<mesh_vectors_type>& vertices, const std::vector<mesh_vectors_type>& color, const std::vector<mesh_indices_type>& indices, bool lazyupdate = false);
        void setShape(const std::vector<mesh_vectors_type>& vertices, const std::vector<mesh_indices_type>& indices, bool lazyupdate = false);
        void setShape(const std::vector<mesh_vectors_type>& vertices, const std::vector<mesh_vectors_type>& color, bool lazyupdate = false);
        void setShape(const std::vector<mesh_vectors_type>& vertices, bool lazyupdate = false);

        void setIndexData();
        void setIndexData(IndexData* uvData);
        void setColorData();
        void setColorData(VertexData* uvData);
        void setVertexData();
        void setVertexData(VertexData* vData);

        void setBlendStatechanged(bool blendStatechanged);
        bool isBlendStatechanged() const;
        void setCanChangeBlendState(bool canChangeBlendState);
        bool isCanChangeBlendState() const;
        void setCanChangeDepthTestState(bool canChangeDepthTestState);
        bool isCanChangeDepthTestState() const;
        void setLineStrip(bool lineStrip);
        bool isLineStrip() const;
        void setDepthTestEnabledChange(bool DepthTestEnabledChange);
        bool isDepthTestEnabledChange() const;
        void setDepthTestEnabled(bool DepthTestEnabled);
        bool isDepthTestEnabled() const;
        void setUsingPerVertexColor(bool usingPerVertexColor);
        bool isUsingPerVertexColor() const;

        static aex::ShaderDrw_ptr BasicLineShader();
        static aex::ShaderDrw_ptr BasicLineVertexColorShader();
        
    protected:
        aex::unique_ptr<VertexData> m_positions;
        aex::unique_ptr<VertexData> m_colors;
        IndexData m_indices;

        bool m_isDepthTestEnabled;
        bool m_isDepthTestEnabledChange;
        bool m_lineStrip;
        bool m_canChangeDepthTestState = false;
        bool m_canChangeBlendState = false;
        bool m_blendStatechanged;
        bool m_usingPerVertexColor;
        UniformFloat3 m_lineColor;
        float m_lineWidth = 1.0f;

        std::vector<float> m_baseVertCache;
        std::vector<float> m_baseColorCache;
        std::vector<uint16_t> m_baseIndexCache;

        bool m_lazyVertexUpdate = false;
        bool m_lazyColorUpdate = false;
        bool m_lazyIndexUpdate = false;

        void copyVertData(const std::vector<float>& vertices);
        void copyColorData(const std::vector<float>& uvs);
        void copyIndexData(const std::vector<uint16_t>& index);

    };

    typedef aex::shared_ptr<LinesDrw> LinesDrw_ptr;

}

#endif	/* LINESDRW_HPP */

