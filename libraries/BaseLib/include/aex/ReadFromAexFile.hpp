/*
 * File:   ReadFromAexFile.hpp
 * Author: mritke
 *
 * Created on December 17, 2013, 2:11 PM
 */

#ifndef READFROMAEXFILE_HPP
#define	READFROMAEXFILE_HPP
#include <aex/Common.hpp>
#include <aex/ConvertVector.hpp>
#include <aex/fstream.hpp>
#include <aex/Exceptions.hpp>
#include <stdexcept>
#include <vector>

namespace aex
{


class AEX_PUBLIC_API ReadFromAexFile
{
private:
    typedef std::vector<aex::shared_ptr<IReadVector>> TGeomData;
    TGeomData m_GeomData;
    double m_animationTime;
    uint16_t m_framesNumber;
    uint16_t m_LODnumber;
    MeshType peekMeshType(const aex::string& filename);
    void loadData(const aex::string& filename);
    void _getVectorFromFile(fstream& file, void* result, const std::size_t elementSize, const std::size_t elementCount);
    std::size_t getArraySizeForOffset(fstream& file, const FileOffsetData& offset);
public:
    using VertexDataArray = std::vector<mesh_vectors_type>;
    using IndicesDataArray = std::vector<mesh_indices_type>;

    ReadFromAexFile();
    ~ReadFromAexFile();

    void ImportFromAexFileStaticMesh(const aex::string& filename,
                                     VertexDataArray& vertices,
                                     VertexDataArray& uv,
                                     VertexDataArray& normals,
                                     std::vector <mesh_indices_type>& indices);

    // void ImportFromAexFile(const aex::string& filename, AnimationDrw& anim);



    void readFileHeader(fstream& ReadFile, MeshType& meshT, std::vector<FileOffsetData>& offsets);
    void readFileHeader(std::stringstream& ReadFile, MeshType& meshT, std::vector<FileOffsetData>& offsets);
    void readAexFile(const aex::string& filename);
    void getVectorFromFile(fstream& file, VertexDataArray& vec, const FileOffsetData& offset);
    void getVectorFromFile(fstream& file, IndicesDataArray& vec, const FileOffsetData& offset);
    void getVectorFromFile(std::stringstream& file, VertexDataArray& vec, const FileOffsetData& offset);
    void getVectorFromFile(std::stringstream& file, IndicesDataArray& vec, const FileOffsetData& offset);
    template<class T>
    void getArrayFromFile(fstream& file, std::vector<T>& result, const FileOffsetData& offset)
    {
        const std::size_t elementCount = getArraySizeForOffset(file, offset);
        result.resize(elementCount);
        _getVectorFromFile(file, static_cast<void*>(result.data()), sizeof(T), elementCount);
    }
};

template<typename T, bool swap> struct endian_helper;

template<typename T> struct endian_helper<T, true>
{

    T operator()(T t)
    {
        T temp {};

        for(size_t ii = 0; ii < sizeof(T); ++ii)
        {
            (&temp)[ii] = (&t)[sizeof(T) - 1 - ii];
        }

        return temp;
    }
};

template<typename T> struct endian_helper<T, false>
{

    T operator()(T t)
    {
        return t;
    }
};
}


#endif	/* READFROMAEXFILE_HPP */

