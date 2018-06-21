//
//  MtlParser.h
//  AexolGL
//
//  Created by Kamil Matysiewicz on 13.09.2013.
//
//

#ifndef AexolGL_MtlParser_h
#define AexolGL_MtlParser_h

#include <aex/MtlMaterial.hpp>
#include <list>

namespace aex
{
    class AEX_PUBLIC_API MtlParser
    {
    public:
        bool                    read_mtl_file(const aex::string& full_path);
        const std::list<std::pair<aex::string, MtlMaterial>>& get_materials() const
        {
            return m_material_map;
        }
    private:
        bool analyze_mtl_line(std::istream& is);
        bool next_line(std::istream& is);
        Texture_ptr analyze_texture(std::istream& is);
        std::list<std::pair<aex::string, MtlMaterial>> m_material_map;
    };
}


#endif
