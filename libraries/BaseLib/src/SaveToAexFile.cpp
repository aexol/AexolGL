#include <aex/SaveToAexFile.hpp>
#include <aex/ConvertVector.hpp>
#include <limits>

namespace aex
{
    using offset_type = FileOffsetData::offset_type;

    /*template<typename T>
    size_t writeOutTypeSize( const std::vector<T> var )
    {
        switch(sizeof(T))
        {
            case 8:
                return 8;
                break;
            case 16:
                return 16;
                break;
            case 32:
                return 32;
                break;
            default:
                throw std::runtime_error("tralala");
        }
    }    */


    SaveToAexFile::SaveToAexFile():
        m_animationTime(1.0),
        m_framesNumber(0),
        m_LODnumber(0)
    {


    }

    void
    SaveToAexFile::PutVectorInFile(std::fstream& file,  std::vector<mesh_vectors_type>& vec, FileOffsetData& offset)
    {
        size_t size = vec.size();
        offset.attribOffset = file.tellp();
        file.write(reinterpret_cast<char*>(&size), sizeof(size_t));

        file.write(reinterpret_cast<char*>(&vec[0]), size*sizeof(mesh_vectors_type));
        
    }

    void
    SaveToAexFile::PutVectorInFile(std::fstream& file,  std::vector<mesh_indices_type>& vec, FileOffsetData& offset)
    {
        size_t size = vec.size();
        offset.attribOffset = file.tellp();
        file.write(reinterpret_cast<char*>(&size), sizeof(size_t));

            file.write(reinterpret_cast<char*>(&vec[0]),size* sizeof(mesh_indices_type));
    }

    void SaveToAexFile::setAnimationTime(double animationTime)
    {
        m_animationTime = animationTime;
    }

    double SaveToAexFile::getAnimationTime() const
    {
        return m_animationTime;
    }

    void SaveToAexFile::updateHeaderInfo(std::fstream& newFile, std::vector<FileOffsetData>& offsets)
    {
        for(auto offset : offsets)
        {
            newFile.flush();
            newFile.seekp(offset.headerOffset, std::ios::beg);
            newFile.write(reinterpret_cast<char*>(&offset.attribOffset), sizeof(size_t));
        }

    }

    void SaveToAexFile::makeFileHeader(std::fstream& newFile, MeshType meshT, std::vector<FileOffsetData>& offsets)
    {
        offset_type headerSize = 0;
        offset_type zero = 0;
        newFile.seekg(newFile.beg);

        newFile.write(reinterpret_cast<char*>(&zero), sizeof(offset_type));
        newFile.write(reinterpret_cast<char*>(&meshT), sizeof(MeshType));

        if(meshT == ANIMATED_MESH)
        {
            newFile.write(reinterpret_cast<char*>(&m_framesNumber), sizeof(uint16_t));
            newFile.write(reinterpret_cast<char*>(&m_animationTime), sizeof(double));
        }
        else if(meshT == LOD_MESH)
        {
            newFile.write(reinterpret_cast<char*>(&m_LODnumber), sizeof(uint16_t));
        }
        else if(meshT == SKINNED_MESH)
        {

        }

        for(size_t i = 0; i < offsets.size() ; i++)
        {
            newFile.write(reinterpret_cast<char*>(&offsets[i].attrT), sizeof(OffsetType));

            offsets[i].headerOffset = newFile.tellp();
            newFile.write(reinterpret_cast<char*>(&zero), sizeof(size_t));

        }

        headerSize = newFile.tellp();
        newFile.seekp(newFile.beg);
        newFile.write(reinterpret_cast<char*>(&headerSize), sizeof(size_t));
        newFile.seekp(headerSize);
    }

    void
    SaveToAexFile::exportToAexFile(const aex::string& filename, std::vector<aex::shared_ptr<IWritableVector>> vectors)
    {
        std::fstream newFile;
        newFile.open(filename + ".aex", std::ios::out |
                std::ios::trunc |
                std::ios::binary);

        std::vector<FileOffsetData> offsets;
        MeshType meshT = STATIC_MESH;

        for(auto vec : vectors)
        {
            FileOffsetData newOffset;
            newOffset.attrT = vec->m_attrType;
            newOffset.headerOffset = 0;
            newOffset.attribOffset = 0;

            if(newOffset.attrT == GL_KEYFRAME)
            {
                meshT = ANIMATED_MESH;
                m_framesNumber++;
            }
            else if(newOffset.attrT == GL_KEYFRAME_NORMALS)
            {

                meshT = ANIMATED_MESH;
            }
            else if(newOffset.attrT == GL_BONEWEIGHT)
            {

                meshT = SKINNED_MESH;
            }
            else if(newOffset.attrT == LOD_MESH_OFFSET)
            {

                meshT = LOD_MESH;
                m_LODnumber++;
            }

            offsets.push_back(newOffset);
        }

        if(newFile.is_open())
        {
            makeFileHeader(newFile,  meshT, offsets);

            int i = 0;

            for(auto vec : vectors)
            {

                offsets[i].attribOffset = vec->WriteToFile(newFile);
                i++;
            }

            updateHeaderInfo(newFile, offsets);
        }

        newFile.close();

    }


    void
    SaveToAexFile::exportToAexFileStaticMesh(const aex::string& filename, std::vector<mesh_vectors_type>& vertices, std::vector<mesh_vectors_type>& uv, std::vector<mesh_vectors_type>& normals, std::vector <mesh_indices_type>& indices)
    {
        std::fstream newFile;
        newFile.open(filename + ".aex", std::ios::out |
                std::ios::trunc |
                std::ios::binary);

        std::vector<FileOffsetData> offsets(4);
        offset_type zero = 0;
        offset_type headerSize;
        MeshType meshT = STATIC_MESH;

        OffsetType vertT = GL_VERTICES;
        OffsetType uvT = GL_UV;
        OffsetType normT = GL_NORMALS;
        OffsetType indT = GL_INDICES;

        if(newFile.is_open())
        {
            newFile.write(reinterpret_cast<char*>(&zero), sizeof(offset_type));
            newFile.write(reinterpret_cast<char*>(&meshT), sizeof(MeshType));

            newFile.write(reinterpret_cast<char*>(&vertT), sizeof(OffsetType));
            offsets[0].attrT = vertT;
            offsets[0].headerOffset = newFile.tellp();
            newFile.write(reinterpret_cast<char*>(&zero), sizeof(offset_type));

            newFile.write((char*)&uvT, sizeof(OffsetType));
            offsets[1].attrT = uvT;
            offsets[1].headerOffset = newFile.tellp();
            newFile.write(reinterpret_cast<char*>(&zero), sizeof(offset_type));

            newFile.write((char*)&normT, sizeof(OffsetType));
            offsets[2].attrT = normT;
            offsets[2].headerOffset = newFile.tellp();
            newFile.write((char*)&zero, sizeof(offset_type));

            newFile.write(reinterpret_cast<char*>(&indT), sizeof(OffsetType));
            offsets[3].attrT = indT;
            offsets[3].headerOffset = newFile.tellp();
            newFile.write(reinterpret_cast<char*>(&zero), sizeof(offset_type));

            headerSize = newFile.tellp();
            PutVectorInFile(newFile, vertices, offsets[0]);
            PutVectorInFile(newFile, uv, offsets[1]);
            PutVectorInFile(newFile, normals, offsets[2]);
            PutVectorInFile(newFile, indices, offsets[3]);

            newFile.seekp(newFile.beg);
            newFile.write(reinterpret_cast<char*>(&headerSize), sizeof(offset_type));

            for(auto offset : offsets)
            {
                if(offset.headerOffset != std::numeric_limits<size_t>::max())
                {
                    newFile.flush();
                    newFile.seekp(offset.headerOffset, std::ios::beg);
                    newFile.write(reinterpret_cast<char*>(&offset.attribOffset), sizeof(size_t));
                }
            }
        }

        newFile.close();
    }
}
