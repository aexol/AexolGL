//
//  MtlParser.cpp
//  AexolGL
//
//  Created by Kamil Matysiewicz on 13.09.2013.
//
//

#include <aex/MtlParser.hpp>
#include <aex/Texture.hpp>
#include <ios>
#include <fstream>
#include <aex/fstream.hpp>
#include <string>
#include <algorithm>
#include <sstream>

namespace aex
{
bool
MtlParser::next_line(std::istream& is)
{

    char c;

    while(is >> std::skipws >> c)
    {
        if(c == '#')
        {
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            c = is.peek();
        }
        else
        {
            is.putback(c);
            return true;
        }
    }

    return false;
}

bool
MtlParser::read_mtl_file(const aex::string& full_path)
{
    aex::fstream ifs(full_path, io::ios_base::in);

    if(!ifs)
    {
        std::stringstream ss;
        ss << "Could not open MTL file " << full_path;
        throw std::runtime_error {ss.str()};
    }

    if(ifs.peek() != std::ifstream::traits_type::eof())
    {
        while(next_line(ifs))
        {
            if(analyze_mtl_line(ifs) == false)
            {
                return false;    //TO-DO error parsing line exception
            }
        }
    }
    else
    {
        std::stringstream ss;
        ss << "Empty MTL file " << full_path;
        throw std::runtime_error {ss.str()};
    }

    return true;
}

bool
MtlParser::analyze_mtl_line(std::istream& is)
{
    aex::string ele_id;
    float x, y, z;

    if(!(is >> ele_id))
    {
        return false;
    }

    std::transform(ele_id.begin(), ele_id.end(), ele_id.begin(), ::tolower);

    if("newmtl" == ele_id)
    {
        aex::string material_name;
        is >> material_name;
        m_material_map.push_back(std::make_pair(material_name, MtlMaterial()));
    }
    else if("ka" == ele_id)  	//	vertex data
    {
        is >> x >> y >> z;
        m_material_map.back().second.m_has_ambient_light = true;
        m_material_map.back().second.m_ambient_light.x = x;
        m_material_map.back().second.m_ambient_light.y = y;
        m_material_map.back().second.m_ambient_light.z = z;
    }
    else if("kd" == ele_id)  	// texture data
    {
        is >> x >> y >> z;
        m_material_map.back().second.m_has_diffuse_light = true;
        m_material_map.back().second.m_diffuse_light.x = x;
        m_material_map.back().second.m_diffuse_light.y = y;
        m_material_map.back().second.m_diffuse_light.z = z;
    }
    else if("ks" == ele_id)  	// normal data
    {
        is >> x >> y >> z;
        m_material_map.back().second.m_has_specular_light = true;
        m_material_map.back().second.m_specular_light.x = x;
        m_material_map.back().second.m_specular_light.y = y;
        m_material_map.back().second.m_specular_light.z = z;
    }
    else if("ns" == ele_id)  	//	face data
    {
        is >> x;
        m_material_map.back().second.m_has_specular_weight = true;
        m_material_map.back().second.m_specular_weight = x;
    }
    else if("ni" == ele_id)  	//	face data
    {
        is >> x;
        m_material_map.back().second.m_has_optical_density = true;
        m_material_map.back().second.m_optical_density = x;
    }
    else if("d" == ele_id || "tr" == ele_id)
    {
        is >> x;

        if(!m_material_map.back().second.m_has_transparency)
        {
            m_material_map.back().second.m_has_transparency = true;
            m_material_map.back().second.m_transparency = x;
        }
    }
    else if("tf" == ele_id)  	// normal data
    {
        is >> x >> y >> z;
        m_material_map.back().second.m_has_transmission_filter = true;
        m_material_map.back().second.m_transmission_filter.x = x;
        m_material_map.back().second.m_transmission_filter.y = y;
        m_material_map.back().second.m_transmission_filter.z = z;
    }
    else if("illum" == ele_id)
    {
        int16_t i;
        is >> i;
        m_material_map.back().second.m_has_illumination = true;
        m_material_map.back().second.m_illumination = i;
    }
    else if("map_ka" == ele_id)
    {
        m_material_map.back().second.m_ambient_map = analyze_texture(is);
    }
    else if("map_kd" == ele_id)
    {
        m_material_map.back().second.m_diffuse_map = analyze_texture(is);
    }
    else if("map_ks" == ele_id)
    {
        m_material_map.back().second.m_specular_color_map = analyze_texture(is);
    }
    else if("map_ns" == ele_id)
    {
        m_material_map.back().second.m_specular_highlight_map = analyze_texture(is);
    }
    else if("map_d" == ele_id)
    {
        m_material_map.back().second.m_transparency_map = analyze_texture(is);
    }
    else if("map_bump" == ele_id || "bump" == ele_id)
    {
        if(!m_material_map.back().second.m_bump_map)
        {
            m_material_map.back().second.m_bump_map = analyze_texture(is);
        }
    }
    else if("disp" == ele_id)
    {
        m_material_map.back().second.m_disp_map = analyze_texture(is);
    }
    else if("decal" == ele_id)
    {
        m_material_map.back().second.m_decal_map = analyze_texture(is);
    }
    else if("map_refl" == ele_id || "refl" == ele_id)
    {
        if(!m_material_map.back().second.m_refl_map)
        {
            m_material_map.back().second.m_refl_map = analyze_texture(is);
        }
    }
    else
    {
        is.ignore(std::numeric_limits<std::streamsize>().max(), '\n');
    }

    return true;
}


Texture_ptr
MtlParser::analyze_texture(std::istream& is)
{
    bool        blendu = true;
    bool        blendv = true;
    float     boost = 1.0f;
    float     mm_base = 0.0f;
    float     mm_gain = 1.0f;
    Vector      o(0.0f, 0.0f, 0.0f);
    Vector      s(1.0f, 1.0f, 1.0f);
    Vector      t(0.0f, 0.0f, 0.0f);
    int16_t     texres = 0;
    bool        clamp = false;
    float     bm = 1.0f;
    aex::string imfchan;
    aex::string type;
    aex::string file_name;

    aex::string texture_line;
    std::getline(is, texture_line, '\n');
    std::istringstream iss(texture_line);

    aex::string ele_id;

    Texture_ptr ret;

    UNUSED(blendu);
    UNUSED(blendv);
    UNUSED(clamp);
    
    while(iss.good())
    {
        iss >> ele_id;

        if(ele_id.front() == '#')
        {
            break;
        }

        if("-blendu" == ele_id)
        {
            aex::string value;
            iss >> value;

            if("on" == value)
            {
                blendu = true;
            }
            else if("off" == value)
            {
                blendu = false;
            }
            else
            {
                break;    //TO-DO exception
            }
        }
        else if("-blendv" == ele_id)
        {
            aex::string value;
            iss >> value;

            if("on" == value)
            {
                blendv = true;
            }
            else if("off" == value)
            {
                blendv = false;
            }
            else
            {
                break;    //TO-DO exception
            }
        }
        else if("-boost" == ele_id)
        {
            iss >> boost;
        }
        else if("-mm" == ele_id)
        {
            aex::string value;
            iss >> mm_base >> mm_gain;
        }
        else if("-o" == ele_id)
        {
            iss >> o.x >> o.y;

            if(iss.good())
            {
                iss >> o.z;
            }

            iss.clear();
        }
        else if("-s" == ele_id)
        {
            iss >> s.x >> s.y;

            if(iss.good())
            {
                iss >> s.z;
            }

            iss.clear();
        }
        else if("-t" == ele_id)
        {
            iss >> t.x >> t.y;

            if(iss.good())
            {
                iss >> t.z;
            }

            iss.clear();
        }
        else if("-texres" == ele_id)
        {
            iss >> texres;
        }
        else if("-clamp" == ele_id)
        {
            aex::string value;
            iss >> value;

            if("on" == value)
            {
                clamp = true;
            }
            else if("off" == value)
            {
                clamp = false;
            }
            else
            {
                break;    //TO-DO exception
            }
        }
        else if("-bm" == ele_id)
        {
            iss >> bm;
        }
        else if("-imfchan" == ele_id)
        {
            iss >> imfchan;
        }
        else if("-type" == ele_id)
        {
            iss >> type;
            /* TODO: Reflection mapy bo ni cholery nie wiem jak to w MTL wygląda */
        }
        else
        {
            iss >> file_name;
#if __TARGET_AEX_API__ == __GLES__ || __TARGET_AEX_API__ == __GL__
            ret = Texture_ptr(new aex::Texture(file_name, TextureConfig::makeDefaultTextureConfig()));
#endif
        }
    }

    return ret;
}
}
