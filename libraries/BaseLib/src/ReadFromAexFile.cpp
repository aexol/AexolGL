#include <aex/ReadFromAexFile.hpp>
#include <aex/ConvertVector.hpp>
#include <aex/FileManager.hpp>
#include <aex/ResourceDirector.hpp>
#include <aex/Mesh.hpp>
#include <ios>
#include <SDL_rwops.h>

namespace aex
{
using ios = std::ios;

typedef std::vector<aex::shared_ptr<IReadVector>> TGeomData;
typedef std::vector<mesh_vectors_type> VertexDataArray;
typedef std::vector<mesh_indices_type> IndicesDataArray;

ReadFromAexFile::ReadFromAexFile() :
    m_GeomData(0)
{

}

ReadFromAexFile::~ReadFromAexFile()
{
    m_GeomData.clear();
}

std::size_t ReadFromAexFile::getArraySizeForOffset(fstream& file, const FileOffsetData& offset)
{
    uint32_t size;

    file.seekp(offset.attribOffset, ios::beg);
    file.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
    return size;
}

void ReadFromAexFile::_getVectorFromFile(fstream& file, void* result, const std::size_t elementSize, const std::size_t elementCount)
{
    char* byte_array = reinterpret_cast<char*>(result);
    file.read(byte_array, elementSize*elementCount);
}

void ReadFromAexFile::getVectorFromFile(fstream& file, std::vector<mesh_vectors_type>& vec, const FileOffsetData& offset)
{
    uint32_t size;

    file.seekp(offset.attribOffset, ios::beg);
    file.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
    vec.resize(size);


        file.read((char*) &vec[0], size*sizeof(mesh_vectors_type));
    
}

void ReadFromAexFile::getVectorFromFile(fstream& file, std::vector<mesh_indices_type>& vec, const FileOffsetData& offset)
{
    uint32_t size;

    file.seekp(offset.attribOffset, ios::beg);
    file.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
    vec.resize(size);


        file.read((char*) &vec[0], size*sizeof(mesh_indices_type));
 
}

void ReadFromAexFile::getVectorFromFile(std::stringstream& file, std::vector<mesh_vectors_type>& vec, const FileOffsetData& offset)
{
    uint32_t size;

    file.seekp(offset.attribOffset, ios::beg);
    file.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
    vec.resize(size);


    file.read((char*) &vec[0], size *sizeof(mesh_vectors_type));
}

void ReadFromAexFile::getVectorFromFile(std::stringstream& file, std::vector<mesh_indices_type>& vec, const FileOffsetData& offset)
{
    uint32_t size;

    file.seekp(offset.attribOffset, ios::beg);
    file.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
    vec.resize(size);

    file.read((char*) &vec[0], size *sizeof(mesh_indices_type));
   
}

void
ReadFromAexFile::loadData(const aex::string& filename)
{
    FileManager fm;
    fstream ReadFile = fm.open_rw_file(filename, std::ios_base::in | std::ios_base::binary);

    if(ReadFile.is_open())
    {
        std::vector<FileOffsetData> offsets;
        MeshType meshT;
        readFileHeader(ReadFile, meshT, offsets);

        for(FileOffsetData offset : offsets)
        {
            if(offset.attrT == GL_INDICES)
            {
                aex::shared_ptr <ReadVector <mesh_indices_type>> i_ptr = aex::make_shared <ReadVector <mesh_indices_type>> (offset.attrT);
                i_ptr->ReadFromFile(ReadFile, offset);
                m_GeomData.push_back(i_ptr);
            }
            else
            {
                aex::shared_ptr <ReadVector <mesh_vectors_type>> v_ptr = aex::make_shared <ReadVector <mesh_vectors_type>> (offset.attrT);
                v_ptr->ReadFromFile(ReadFile, offset);
                m_GeomData.push_back(v_ptr);
            }
        }
    }
    else
    {
        aex::string msg = "File ";
        msg += filename;
        msg += " not found.";
        throw std::runtime_error {msg};
    }
}

MeshType
ReadFromAexFile::peekMeshType(const aex::string& filename)
{
    MeshType m = NONE;
    std::vector<FileOffsetData> offsets;
    FileManager fm;
    fstream ReadFile = fm.open_rw_file(filename, std::ios_base::in | std::ios_base::binary);

    if(ReadFile.is_open())
    {
        readFileHeader(ReadFile, m, offsets);
    }

    return m;
}

template<typename HANDLE>
static void setAttribInMesh(HANDLE& handle, aex::shared_ptr<IReadVector>& data, const aex::string& attribName, size_t size)
{
    aex::shared_ptr <ReadVector <mesh_vectors_type>> ptr = aex::static_pointer_cast <ReadVector <mesh_vectors_type>> (data);
    (*handle)([&](Mesh & mesh)
    {
        mesh.setFloatAttribute(std::move(ptr->m_vec), attribName, size);
    });
}

template <typename
          HANDLE>
static void setTrianglesInMesh(HANDLE& handle, aex::shared_ptr<IReadVector>& data)
{
    aex::shared_ptr <ReadVector <mesh_indices_type>> ptr = aex::static_pointer_cast <ReadVector <mesh_indices_type>> (data);
    (*handle)([&](Mesh & mesh)
    {
        mesh.setTriangles(std::move(ptr->m_vec));
    });

}

template<typename T, class alloc = std::allocator<T>>
typename Manager<T, alloc>::uh_type_ptr getHandle(const aex::string& filename)
{
    ResourceDirector& rDirector = ResourceDirector::GetInstance();
    auto& mgrT = rDirector.GetManager<T>();
    return mgrT.GetResourceHandle(filename);
}

//template<typename HANDLE_TYPE>
//void setFrameData(HANDLE_TYPE& handle, aex::shared_ptr<IReadVector>& data, AnimationDrw& anim, int fIdx)
//{
//    aex::shared_ptr <ReadVector <mesh_vectors_type>> data_ptr = aex::static_pointer_cast <ReadVector <mesh_vectors_type>> (data);
//
//    if(data->m_attrType == GL_KEYFRAME)
//    {
//        (*handle)([&](AnimationDrw::FrameData & frame)
//        {
//            frame.m_frames.push_back(KeyframeVertex(fIdx * anim.GetUniformFrameDuration(), (fIdx + 1) * anim.GetUniformFrameDuration(), move(data_ptr->m_vec)));
//        });
//    }
//    else if(data->m_attrType == GL_KEYFRAME_NORMALS)
//    {
//        (*handle)([&](AnimationDrw::FrameData & frame)
//        {
//            frame.m_frames.back().SetFrameNormals(std::move(data_ptr->m_vec));
//        });
//    }
//}

template<typename HANDLE_TYPE>
void setCommonData(HANDLE_TYPE& handle, aex::shared_ptr<IReadVector>& data, const aex::string& filename)
{
    (*handle)([&](Mesh & mesh)
    {
        mesh.setMeshName(filename);
    });

    if(data->m_attrType == GL_VERTICES)
    {
        setAttribInMesh(handle, data, "gl_Vertex", 3);
    }
    else if(data->m_attrType == GL_UV)
    {

        setAttribInMesh(handle, data, "gl_TexCoord", 2);
    }
    else if(data->m_attrType == GL_NORMALS)
    {

        setAttribInMesh(handle, data, "gl_Normal", 3);
    }
    else if(data->m_attrType == GL_INDICES)
    {
        setTrianglesInMesh(handle, data);
    }
}

//void
//ReadFromAexFile::ImportFromAexFile(const aex::string& file, AnimationDrw& anim)
//{
//    FileManager fManager;
//    typedef boost::filesystem::path path;
//    auto p = fManager.work_dir();
//    p /= path {file};
//    aex::string filename = p.string();
//
//
//    MeshType meshT = peekMeshType(filename);
//    loadData(filename);
//
//    if(meshT == STATIC_MESH)
//    {
//        auto h_mesh = getHandle<Mesh>(filename);
//
//        if(h_mesh)
//        {
//            if(h_mesh->isNew())
//            {
//                for(auto data : m_GeomData)
//                {
//                    setCommonData(h_mesh, data, filename);
//                }
//            }
//
//            anim.setBindPose(filename);
//        }
//    }
//    else if(meshT == ANIMATED_MESH)
//    {
//        int i = 0;
//        auto h_mesh = getHandle<Mesh>(filename);
//        auto h_framesData = getHandle<AnimationDrw::FrameData>(filename);
//        anim.SetAnimationSize(m_framesNumber);
//        anim.SetDurationTime(m_animationTime);
//
//        if(h_mesh && h_framesData && (h_mesh->isNew() || h_framesData->isNew()))
//        {
//            if(h_mesh->isNew())
//            {
//                (*h_mesh)([&](Mesh & mesh)
//                {
//                    mesh.setMeshName(filename);
//                });
//            }
//
//            for(auto data : m_GeomData)
//            {
//                if(h_framesData &&
//                        h_framesData->isNew() &&
//                        ((data->m_attrType == GL_KEYFRAME) || (data->m_attrType == GL_KEYFRAME_NORMALS)))
//                {
//                    if(data->m_attrType == GL_KEYFRAME)
//                    {
//                        i++;
//                    }
//
//                    setFrameData(h_framesData, data, anim, i);
//                }
//                else if(h_mesh && h_mesh->isNew())
//                {
//                    setCommonData(h_mesh, data, filename);
//                }
//            }
//        }
//
//        if(h_mesh)
//        {
//            anim.setBindPose(filename);
//        }
//
//        if(h_framesData)
//        {
//            anim.setFrameData(filename);
//        }
//
//
//    }
//    else if(meshT == SKINNED_MESH)
//    {
//
//
//    }
//    else if(meshT == LOD_MESH)
//    {
//
//
//    }
//
//}

void ReadFromAexFile::ImportFromAexFileStaticMesh(const aex::string& filename, std::vector<mesh_vectors_type>& vertices, std::vector<mesh_vectors_type>& uv, std::vector<mesh_vectors_type>& normals, std::vector<mesh_indices_type>& indices)
{
    aex::fstream ReadFile;
    ReadFile.open(filename, ios::in | ios::binary);

    uint32_t headerSize;
    MeshType meshT;

    std::vector<FileOffsetData> offsets;

    if(ReadFile.is_open())
    {
        bool meshtypeKnown = false;
        //ReadFile.seekg(ReadFile.beg);
        ReadFile.read((char*) &headerSize, sizeof(uint32_t));

        while(ReadFile.tellg() < headerSize)
        {
            if(!meshtypeKnown)
            {
                ReadFile.read(reinterpret_cast<char*>(&meshT), sizeof(MeshType));
                meshtypeKnown = true;
            }
            else
            {
                FileOffsetData newOffset;
                ReadFile.read(reinterpret_cast<char*>(&newOffset.attrT), sizeof(OffsetType));
                ReadFile.read(reinterpret_cast<char*>(&newOffset.attribOffset), sizeof(uint32_t));
                offsets.push_back(newOffset);
            }
        }

        for(FileOffsetData offset : offsets)
        {

            if(offset.attrT == GL_VERTICES)
            {
                getVectorFromFile(ReadFile, vertices, offset);
            }
            else if(offset.attrT == GL_UV)
            {
                getVectorFromFile(ReadFile, uv, offset);
            }
            else if(offset.attrT == GL_NORMALS)
            {
                getVectorFromFile(ReadFile, normals, offset);
            }
            else if(offset.attrT == GL_INDICES)
            {
                getVectorFromFile(ReadFile, indices, offset);
            }
        }
    }

    ReadFile.close();
}

void ReadFromAexFile::readAexFile(const aex::string& filename)
{
    loadData(filename);
}

void ReadFromAexFile::readFileHeader(fstream& ReadFile, MeshType& meshT, std::vector<FileOffsetData>& offsets)
{
    uint32_t headerSize;
    ReadFile.seekg(ReadFile.beg);
    ReadFile.read((char*) &headerSize, sizeof(uint32_t));
    ReadFile.read(reinterpret_cast<char*>(&meshT), sizeof(MeshType));

    if(meshT == ANIMATED_MESH)
    {
        ReadFile.read(reinterpret_cast<char*>(&m_framesNumber), sizeof(uint16_t));
        ReadFile.read(reinterpret_cast<char*>(&m_animationTime), sizeof(double));
    }
    else if(meshT == LOD_MESH)
    {
        ReadFile.read(reinterpret_cast<char*>(&m_LODnumber), sizeof(uint16_t));
    }
    else if(meshT == SKINNED_MESH)
    {

    }

    while(ReadFile.tellg() < headerSize)
    {
        FileOffsetData newOffset;
        ReadFile.read(reinterpret_cast<char*>(&newOffset.attrT), sizeof(OffsetType));
        ReadFile.read(reinterpret_cast<char*>(&newOffset.attribOffset), sizeof(uint32_t));
        offsets.push_back(newOffset);
    }
}

void ReadFromAexFile::readFileHeader(std::stringstream& ReadFile, MeshType& meshT, std::vector<FileOffsetData>& offsets)
{
    uint32_t headerSize;
    ReadFile.seekg(ReadFile.beg);
    ReadFile.read((char*) &headerSize, sizeof(uint32_t));
    ReadFile.read(reinterpret_cast<char*>(&meshT), sizeof(MeshType));

    if(meshT == ANIMATED_MESH)
    {
        ReadFile.read(reinterpret_cast<char*>(&m_framesNumber), sizeof(uint16_t));
        ReadFile.read(reinterpret_cast<char*>(&m_animationTime), sizeof(double));
    }
    else if(meshT == LOD_MESH)
    {
        ReadFile.read(reinterpret_cast<char*>(&m_LODnumber), sizeof(uint16_t));
    }
    else if(meshT == SKINNED_MESH)
    {

    }



    while(ReadFile.tellg() < headerSize)
    {
        FileOffsetData newOffset;
        ReadFile.read(reinterpret_cast<char*>(&newOffset.attrT), sizeof(OffsetType));
        ReadFile.read(reinterpret_cast<char*>(&newOffset.attribOffset), sizeof(uint32_t));
        offsets.push_back(newOffset);
    }
}

}
