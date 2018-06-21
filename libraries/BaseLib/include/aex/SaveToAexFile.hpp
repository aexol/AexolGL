/*
 * File:   SaveToAexFile.hpp
 * Author: mritke
 *
 * Created on December 17, 2013, 12:07 PM
 */

#ifndef SAVETOAEXFILE_HPP
#define	SAVETOAEXFILE_HPP


#include <aex/Common.hpp>
#include <aex/ConvertVector.hpp>
//#include <aex/AnimationFactory.hpp>
#include <stdexcept>
#include <ostream>
#include <fstream>
#include <vector>

namespace aex
{

    class AEX_PUBLIC_API SaveToAexFile
    {

    public:
        SaveToAexFile();

        void exportToAexFileStaticMesh(const aex::string& filename,
                                       std::vector<mesh_vectors_type>& vertices,
                                       std::vector<mesh_vectors_type>& uv,
                                       std::vector<mesh_vectors_type>& normals,
                                       std::vector <mesh_indices_type>& indices);

        void exportToAexFile(const aex::string& filename, std::vector<aex::shared_ptr<IWritableVector>> vectors);

        void makeFileHeader(std::fstream& newFile, MeshType meshT, std::vector<FileOffsetData>& offsets);
        void updateHeaderInfo(std::fstream& newFile, std::vector<FileOffsetData>& offsets);

        void PutVectorInFile(std::fstream& newFile,  std::vector<mesh_vectors_type>& vec, FileOffsetData& offset);
        void PutVectorInFile(std::fstream& newFile,  std::vector<mesh_indices_type>& vec, FileOffsetData& offset);
        void setAnimationTime(double animationTime);
        double getAnimationTime() const;
        
        private:
        double         m_animationTime;
        uint16_t       m_framesNumber;
        uint16_t       m_LODnumber;
        
        template<class T>
        using vector = std::vector<T>;


    };
}

#endif	/* SAVETOAEXFILE_HPP */

