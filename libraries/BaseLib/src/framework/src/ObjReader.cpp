//
//  ObjReader.cpp
//  AexolGL
//
//  Created by Kamil Matysiewicz on 10.09.2013.
//
//

#include <aex/ObjReader.hpp>
#include <aex/Mesh.hpp>
#include <aex/Material.hpp>
#include <aex/Vector3.hpp>
#include <aex/utils.hpp>
#include <aex/Math.hpp>
#include <aex/Mesh.hpp>
#include <aex/Material.hpp>
#include <limits>
#include <ios>
#if DEBUG==1
#include <iomanip>
#endif
#include <aex/fstream.hpp>
#include <vector>
#include <type_traits>

namespace aex
{

    bool
    ObjReader::next_line(std::istream& is)
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
    ObjReader::read_mtl_data(const aex::string& full_path)
    {
        MtlParser parser;

        if(!parser.read_mtl_file(m_working_dir + full_path))
        {
            return false;
        }

        const std::list<std::pair<aex::string, MtlMaterial>>& new_mats = parser.get_materials();
        m_materials.insert(new_mats.begin(), new_mats.end());
        return true;
    }

    bool
    ObjReader::read_obj_file(aex::Mesh* mesh, const aex::string& f_name, const aex::string& path)
    {
        aex::string absolute_path;

        if(!path.empty())
        {
            m_working_dir = path;

            if(m_working_dir.back() != '/')
            {
                m_working_dir += '/';
            }
        }

        absolute_path = m_working_dir + f_name;
        aex::ifstream ifs(absolute_path);

        if(!ifs)
        {
            return false; //TO-DO Error openning file exception
        }

        if(ifs.peek() != aex::ifstream::traits_type::eof())
        {
            while(next_line(ifs))
            {
                if(analyze_obj_line(ifs) == false)
                {
                    return false;    //TO-DO error parsing line exception
                }
            }
        }
        else
        {
            return false;    //TO-DO Empty file exception
        }


        m_texcoords.reserve((m_texcoords_list.size() * m_tex_coord_size) / 3);

        for(std::list<Mesh::vertex_type>::iterator it = m_texcoords_list.begin(); it != m_texcoords_list.end();)
        {
            m_texcoords.push_back((*it));
            it++;
            m_texcoords.push_back((*it));
            it++;

            if(m_tex_coord_size == 3)
            {
                m_texcoords.push_back((*it));
            }

            it++;
        }

        m_texcoords_list.clear();

        m_vertices.reserve(m_vertices_list.size());

        for(std::list<Mesh::vertex_type>::iterator it = m_vertices_list.begin(); it != m_vertices_list.end(); it++)
        {
            m_vertices.push_back(*it);
        }

        m_vertices_list.clear();

        m_normals.reserve(m_normals_list.size());

        for(std::list<Mesh::vertex_type>::iterator it = m_normals_list.begin(); it != m_normals_list.end(); it++)
        {
            m_normals.push_back(*it);
        }

        m_normals_list.clear();

        m_indices_vert.reserve(m_indices_vert_list.size());

        for(std::list<Mesh::indices_type>::iterator it = m_indices_vert_list.begin(); it != m_indices_vert_list.end(); it++)
        {
            m_indices_vert.push_back(*it);
        }

        m_indices_vert_list.clear();

        m_indices_coord.reserve(m_indices_coord_list.size());

        for(std::list<Mesh::indices_type>::iterator it = m_indices_coord_list.begin(); it != m_indices_coord_list.end(); it++)
        {
            m_indices_coord.push_back(*it);
        }

        m_indices_coord_list.clear();

        m_indices_norm.reserve(m_indices_norm_list.size());

        for(std::list<Mesh::indices_type>::iterator it = m_indices_norm_list.begin(); it != m_indices_norm_list.end(); it++)
        {
            m_indices_norm.push_back(*it);
        }

        m_indices_norm_list.clear();

        return load_parsed_data(mesh);
    }
    bool
    ObjReader::analyze_obj_line(std::istream& is)
    {
        aex::string ele_id;
        float x, y, z;

        if(!(is >> ele_id))
        {
            return false;
        }

        if("mtllib" == ele_id)
        {
            aex::string strFileName;
            is >> strFileName;
            read_mtl_data(strFileName);
        }
        else if("usemtl" == ele_id)
        {
            aex::string strMtlName;
            is >> strMtlName;
        }
        else if("v" == ele_id)  	//	vertex data
        {
            is >> x >> y >> z;
            m_vertices_list.push_back(x);
            m_vertices_list.push_back(y);
            m_vertices_list.push_back(z);
        }
        else if("vt" == ele_id)  	// texture data
        {
            is >> x >> y >> z;
            m_texcoords_list.push_back(x);
            m_texcoords_list.push_back(y);

            if(is.fail())
            {
                is.clear();                           // (u,v)
                m_tex_coord_size = 2;
            }
            else
            {
                m_texcoords_list.push_back(z);               // (u, v, w)

                if(!aex::math::Math::closeEnough(z, 0.0f, 0.000001f))
                {
                    m_tex_coord_size = 3
                                       ;
                }
            }
        }
        else if("vn" == ele_id)  	// normal data
        {
            is >> x >> y >> z;

            if(!is.good())                       // in case it is -1#IND00
            {
                x = y = z = 0.0;
                is.clear();
                is.ignore(std::numeric_limits<std::streamsize>().max(), '\n');
            }

            m_normals_list.push_back(x);
            m_normals_list.push_back(y);
            m_normals_list.push_back(z);
        }
        else if("f" == ele_id)  	//	face data
        {
            //	Handle only traignles.
            int16_t vi[3];                          // vertex indices.
            int16_t ni[3] = { -1, -1, -1 };         // normal indices.
            int16_t ti[3] = { -1, -1, -1 };         // tex indices.
            char c;

            for(int16_t i = 0; i < 3; ++i)
            {
                if(m_normals_list.empty() && m_texcoords_list.empty())
                {
                    is >> vi[i];
                    m_indices_vert_list.push_back(vi[i]);
                }
                else if(m_texcoords_list.empty())
                {
                    is >> vi[i] >> c >> c >> ni[i];
                    m_indices_vert_list.push_back(vi[i]);
                    m_indices_norm_list.push_back(ni[i]);
                }
                else if(m_normals_list.empty())
                {
                    is >> vi[i] >> c >> ti[i];
                    m_indices_vert_list.push_back(vi[i]);
                    m_indices_coord_list.push_back(ti[i]);
                }
                else
                {
                    is >> vi[i] >> c >> ti[i] >> c >>  ni[i];
                    m_indices_vert_list.push_back(vi[i]);
                    m_indices_norm_list.push_back(ni[i]);
                    m_indices_coord_list.push_back(ti[i]);
                }

                if(!is.good())
                {
                    return false;    //TO-DO Mesh not made from triangles exception
                }
            }
        }
        else
        {
            is.ignore(std::numeric_limits<std::streamsize>().max(), '\n');
        }

        return true;
    }

    bool
    ObjReader::load_parsed_data(aex::Mesh* mesh)
    {
        //std::vector<Mesh::indices_type>* mesh_triangles    = NULL;
        Mesh::index_array_type* mesh_triangles    = nullptr;
        mesh_triangles = &mesh->getTriangles();

        if(mesh_triangles)
        {
#ifdef DEBUG

            if(m_vertices.size() % 3 != 0)
            {
                return false; //TO-DO more informative exception
            }

            if(m_normals.size() % 3 != 0)
            {
                return false; //TO-DO more informative exception
            }

            if(m_texcoords.size() % m_tex_coord_size != 0)
            {
                return false; //TO-DO more informative exception
            }

            if(m_indices_vert.size() % 3 != 0)
            {
                return false; //TO-DO more informative exception
            }

            if(!m_normals.empty() && m_indices_vert.size() != m_indices_norm.size())
            {
                return false;
            }

            if(!m_texcoords.empty() && m_indices_vert.size() / 3 != m_indices_coord.size() / 3)
            {
                return false;
            }

#endif
            //mesh_normals->resize( m_vertices.size() );
            //mesh_coords->resize( ((m_vertices.size())/3)*m_tex_coord_size );

            if(!prepare_data())
            {
                return false;
            }

            move_to_mesh(mesh);


        }

        return true;
    }

    bool
    ObjReader::prepare_data()
    {
        std::vector<unsigned char> normals_per_vertex((m_vertices.size()) / 3, 0);
        auto it_norm  = m_indices_norm.begin();

        //Load vertex data from OBJ to Mesh in correct order (for VBOs)

        std::vector<std::list<std::pair<int16_t, size_t>>> uv_per_vertex(m_vertices.size() / 3);
        std::vector<Mesh::vertex_type> avg_norm(m_vertices.size(), 0);

        size_t nOfUniqueIndices = 0;
        auto it_coordi = m_indices_coord.begin();

        for(auto it = m_indices_vert.begin(); it != m_indices_vert.end(); it++)
        {
            int16_t v = (*(it) - 1);

            if(it_coordi != m_indices_coord.end())
            {
                int16_t t = (*(it_coordi) - 1);

                if(!m_texcoords.empty())
                {
                    auto first = uv_per_vertex[v].begin();
                    auto last = uv_per_vertex[v].end();

                    // Create unique vertex - uv pairs
                    if(std::find_if(first, last, IsFirstEqual<int16_t, size_t>(t)) == last)
                    {
                        uv_per_vertex[v].push_back(std::make_pair(t, nOfUniqueIndices));
                        nOfUniqueIndices++;
                    }
                }

                it_coordi++;
            }

            v *= 3;

            if(!m_normals.empty())
            {
                int16_t n = (*it_norm - 1) * 3;
                avg_norm[v]          +=  m_normals[n];
                avg_norm[v + 1]      +=  m_normals[n + 1];
                avg_norm[v + 2]      +=  m_normals[n + 2];
                normals_per_vertex[v / 3]++;
                it_norm++;
            }
        }

        m_indices_norm.clear();
        //m_indices_coord.clear();

        //Avarge normals in case the values are differen for different faces
        //TO-DO Some check to skip this step if model was made with vertex normals could be usefull

        size_t index = 0;

        for(std::vector<Mesh::vertex_type>::iterator it = avg_norm.begin(); it != avg_norm.end(); it += 3)
        {
            float div = (float)(normals_per_vertex[index]);
            *it     = (*it) / div;
            *(it + 1) = (*(it + 1)) / div;
            *(it + 2) = (*(it + 2)) / div;
            float length = sqrt((*it) * (*it) + ((*(it + 1)) * (*(it + 1))) + ((*(it + 2)) * (*(it + 2))));
            *it     = (*it) / length;
            *(it + 1) = (*(it + 1)) / length;
            *(it + 2) = (*(it + 2)) / length;
            index++;
        }

        std::vector<Mesh::vertex_type> final_coords;

        if(nOfUniqueIndices == 0)
        {
            nOfUniqueIndices = m_indices_vert.size();
        }
        else
        {
            final_coords.resize(nOfUniqueIndices * m_tex_coord_size);
        }

        std::vector<Mesh::vertex_type> final_vertices(nOfUniqueIndices * 3);
        std::vector<Mesh::vertex_type> final_normals(nOfUniqueIndices * 3);
        std::vector<Mesh::indices_type> final_indices(nOfUniqueIndices);

        it_coordi = m_indices_coord.begin();

        for(size_t i = 0; i < m_indices_vert.size(); i++)
        {
            int16_t v = m_indices_vert[i] - 1;
//            int16_t t = *(it.second) - 1;

            int16_t final_index = v;

            if(it_coordi != m_indices_coord.end())
            {
                int16_t t = *it_coordi - 1;
                it_coordi++;

                for(auto& unique_entry : uv_per_vertex[v])
                {
                    final_index = unique_entry.second;
                    final_coords[final_index * m_tex_coord_size]   = m_texcoords[t * m_tex_coord_size];
                    final_coords[final_index * m_tex_coord_size + 1] = m_texcoords[t * m_tex_coord_size + 1];

                    if(m_tex_coord_size == 3)
                    {
                        final_coords[ final_index * m_tex_coord_size + 2 ]     = m_texcoords[ t * m_tex_coord_size + 2 ];
                    }

                    final_vertices[final_index * 3]     = m_vertices [v * 3];
                    final_vertices[final_index * 3 + 1]   = m_vertices [v * 3 + 1];
                    final_vertices[final_index * 3 + 2]   = m_vertices [v * 3 + 2];

                    final_normals[final_index * 3]      = avg_norm [v * 3];
                    final_normals[final_index * 3 + 1]    = avg_norm [v * 3 + 1];
                    final_normals[final_index * 3 + 2]    = avg_norm [v * 3 + 2];
                    final_indices[i] = final_index;
                }
            }
            else
            {
                final_vertices[final_index * 3]     = m_vertices [v * 3];
                final_vertices[final_index * 3 + 1]   = m_vertices [v * 3 + 1];
                final_vertices[final_index * 3 + 2]   = m_vertices [v * 3 + 2];

                final_normals[final_index * 3]      = avg_norm [v * 3];
                final_normals[final_index * 3 + 1]    = avg_norm [v * 3 + 1];
                final_normals[final_index * 3 + 2]    = avg_norm [v * 3 + 2];
                final_indices[i] = final_index;
            }
        }

        m_normals.swap(final_normals);
        m_vertices.swap(final_vertices);
        m_texcoords.swap(final_coords);
        m_indices_vert.swap(final_indices);
        m_indices_norm.clear();
        m_indices_coord.clear();

        return true;
    }

    void
    ObjReader::move_to_mesh(aex::Mesh* mesh)
    {
        Mesh::vertex_array_type& mesh_vertices     = mesh->getVertices();
        Mesh::vertex_array_type* mesh_coords       = mesh->getCoords();
        Mesh::vertex_array_type* mesh_normals      = mesh->getNormals();
        Mesh::index_array_type* mesh_triangles    = &mesh->getTriangles();

        if(mesh_triangles)
        {
            if(!m_normals.empty())
            {
                if(mesh_normals)
                {
                    mesh_normals->clear();
                }
                else
                {
                    mesh->addVertexBuffer("gl_Normal");
                    mesh_normals     = mesh->getNormals();
                }

                mesh->m_bNormals = true;
                mesh_normals->swap(m_normals);
            }

            if(!m_texcoords.empty())
            {
                if(mesh_coords)
                {
                    mesh_coords->clear();
                }
                else
                {
                    mesh->addVertexBuffer("gl_TexCoord");
                    mesh_coords     = mesh->getCoords();
                }

                mesh->m_bCoords = true;
                mesh_coords->swap(m_texcoords);
                mesh->m_coordsSize = m_tex_coord_size;
            }

            mesh_vertices.swap(m_vertices);
            mesh_triangles->swap(m_indices_vert);
            m_normals.clear();
            m_texcoords.clear();
            m_vertices.clear();
            m_indices_vert.clear();
        }
    }
}
