//
//  MtlMaterial.cpp
//  AexolGL
//
//  Created by Kamil Matysiewicz on 13.09.2013.
//
//

#include <aex/MtlMaterial.hpp>

namespace aex
{
    MtlMaterial::MtlMaterial() :
        m_has_ambient_light(false),
        m_has_diffuse_light(false),
        m_has_specular_light(false),
        m_has_specular_weight(false),
        m_has_transparency(false),
        m_has_illumination(false),
        m_ambient_map(),
        m_diffuse_map(),
        m_specular_color_map(),
        m_specular_highlight_map(),
        m_transparency_map(),
        m_bump_map(),
        m_disp_map(),
        m_decal_map(),
        m_refl_map()
    {

    }

    bool
    MtlMaterial::getAmbientLight(Vector* ambient_light)
    {
        if(m_has_ambient_light)
        {
            ambient_light->x = m_ambient_light.x;
            ambient_light->y = m_ambient_light.y;
            ambient_light->z = m_ambient_light.z;

            return true;
        }

        return false;
    }

    bool
    MtlMaterial::getDiffuseLight(Vector* diffuse_light)
    {
        if(m_has_diffuse_light)
        {
            diffuse_light->x = m_diffuse_light.x;
            diffuse_light->y = m_diffuse_light.y;
            diffuse_light->z = m_diffuse_light.z;

            return true;
        }

        return false;
    }

    bool
    MtlMaterial::getSpecularLight(Vector* specular_light)
    {
        if(m_has_specular_light)
        {
            specular_light->x = m_specular_light.x;
            specular_light->y = m_specular_light.y;
            specular_light->z = m_specular_light.z;

            return true;
        }

        return false;
    }

    bool
    MtlMaterial::getSpecularWeight(float* specular_weight)
    {
        if(m_has_specular_weight)
        {
            *specular_weight = m_specular_weight;

            return true;
        }

        return false;
    }

    bool
    MtlMaterial::getOpticalDensity(float* optical_density)
    {
        if(m_has_optical_density)
        {
            *optical_density = m_optical_density;

            return true;
        }

        return false;
    }

    bool
    MtlMaterial::getTransparencyWeight(float* transparency)
    {
        if(m_has_transparency)
        {
            *transparency = m_transparency;

            return true;
        }

        return false;
    }

    bool
    MtlMaterial::getTransmissionFilter(Vector* transmission_filter)
    {
        if(m_has_transmission_filter)
        {
            *transmission_filter = m_transmission_filter;

            return true;
        }

        return false;
    }

    bool
    MtlMaterial::getIllumination(int16_t* illumination)
    {
        if(m_has_illumination)
        {
            *illumination = m_illumination;

            return true;
        }

        return false;
    }
}
