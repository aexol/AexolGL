//
//  MtlMaterial.h
//  AexolGL
//
//  Created by Kamil Matysiewicz on 13.09.2013.
//
//

#ifndef AexolGL_MtlMaterial_h
#define AexolGL_MtlMaterial_h

#include <aex/Vector3.hpp>
#include <aex/Common.hpp>


namespace aex
{
    class Texture;
}

namespace aex
{
    typedef aex::math::Vector3 Vector;
    typedef aex::shared_ptr<aex::Texture> Texture_ptr;
    class MtlMaterial
    {
    public:
        MtlMaterial();
        bool        getAmbientLight(Vector* ambient_light);      // Assign value to parameter, return true at success, false at fail
        bool        getDiffuseLight(Vector* diffuse_light);      // Assign value to parameter, return true at success, false at fail
        bool        getSpecularLight(Vector* specular_light);    // Assign value to parameter, return true at success, false at fail
        bool        getSpecularWeight(float* specular_weight);   // Assign value to parameter, return true at success, false at fail
        bool        getOpticalDensity(float* optical_density);   // Assign value to parameter, return true at success, false at
        bool        getTransparencyWeight(float* transparency);   // Assign value to parameter, return true at success, false at fail
        bool        getTransmissionFilter(Vector* transmission_filter);   // Assign value to parameter, return true at success, false at fail
        bool        getIllumination(int16_t* illumination);   // Assign value to parameter, return true at success, false at fail
        Texture_ptr getAmbientMap();
        Texture_ptr getDiffuseMap();
        Texture_ptr getSpecularMap();
        Texture_ptr getSpecularHighlight();
        Texture_ptr getTransparency();
        Texture_ptr getBump();
        Texture_ptr getDisp();
        Texture_ptr getDecal();
        Texture_ptr getRefl();

    private:
        Vector      m_ambient_light;                // Ka
        bool        m_has_ambient_light;
        Vector      m_diffuse_light;                // Kd
        bool        m_has_diffuse_light;
        Vector      m_specular_light;               // Ks
        bool        m_has_specular_light;
        float       m_specular_weight;              // Ns
        bool        m_has_specular_weight;
        float       m_optical_density;              // Ni
        bool        m_has_optical_density;
        float       m_transparency;                 // d/Tr
        bool        m_has_transparency;
        Vector      m_transmission_filter;          // Tf
        bool        m_has_transmission_filter;
        int16_t     m_illumination;
        bool        m_has_illumination;             // illum
        Texture_ptr m_ambient_map;                  // map_Ka
        Texture_ptr m_diffuse_map;                  // map_Kd
        Texture_ptr m_specular_color_map;           // map_Ks
        Texture_ptr m_specular_highlight_map;       // map_Ns
        Texture_ptr m_transparency_map;             // map_d
        Texture_ptr m_bump_map;                     // map_bump/bump
        Texture_ptr m_disp_map;                     // disp
        Texture_ptr m_decal_map;                    // decal
        Texture_ptr m_refl_map;                     // refl/map_refl

        friend class MtlParser;
    };
}


#endif
