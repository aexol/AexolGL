//
//  ObjReader.h
//  AexolGL
//
//  Created by Kamil Matysiewicz on 10.09.2013.
//
//

#ifndef AexolGL_ObjReader_h
#define AexolGL_ObjReader_h

#include <aex/Common.hpp>
#include <aex/MtlParser.hpp>
#include <istream>
#include <string>
#include <map>

namespace aex
{
    class Mesh;
    class AEX_PUBLIC_API ObjReader
    {
    public:
        ObjReader() : m_tex_coord_size(2), m_working_dir("") {}
        bool                    read_mtl_data(const aex::string& full_path);
        bool                    read_obj_file(aex::Mesh* mesh, const aex::string& f_name, const aex::string& path = "");
    private:
        bool analyze_obj_line(std::istream& is);
        bool next_line(std::istream& is);
        bool load_parsed_data(aex::Mesh* mesh);
        bool prepare_data();
        void move_to_mesh(aex::Mesh* mesh);

        std::vector<mesh_vectors_type>                              m_vertices;
        std::list<mesh_vectors_type>                                m_vertices_list;
        std::vector<mesh_vectors_type>                              m_texcoords;
        std::list<mesh_vectors_type>                                m_texcoords_list;
        std::vector<mesh_vectors_type>                              m_normals;
        std::list<mesh_vectors_type>                                m_normals_list;
        std::vector<mesh_indices_type>                              m_indices_vert;
        std::list<mesh_indices_type>                                m_indices_vert_list;
        std::vector<mesh_indices_type>                              m_indices_norm;
        std::list<mesh_indices_type>                                m_indices_norm_list;
        std::vector<mesh_indices_type>                              m_indices_coord;
        std::list<mesh_indices_type>                                m_indices_coord_list;

        std::map<aex::string, MtlMaterial>                          m_materials;

        uint32_t                                                    m_tex_coord_size;
        aex::string                                                 m_working_dir;
    };
}

#endif
