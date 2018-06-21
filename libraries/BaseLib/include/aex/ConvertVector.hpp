/*
 * File:   ConvertVector.hpp
 * Author: mritke
 *
 * Created on December 18, 2013, 2:40 PM
 */

#ifndef CONVERTVECTOR_HPP
#define	CONVERTVECTOR_HPP

#include <aex/Common.hpp>
#include <aex/Logger.hpp>
#include <aex/Mesh.hpp>
#include "details/aex_file_details.hpp"
#include <fstream>
#include <vector>
namespace aex
{
    class AEX_PUBLIC_API IWritableVector
    {
    public:
        explicit IWritableVector(OffsetType attrType) : m_attrType(attrType)
        {
        }

        virtual uint32_t WriteToFile(std::fstream& file) = 0;

        OffsetType  m_attrType;
    };

    class AEX_PUBLIC_API IReadVector
    {
    public:
        explicit IReadVector(OffsetType attrType) : m_attrType(attrType), m_size(0)
        {
        }

        virtual void ReadFromFile(std::fstream& file, FileOffsetData& offset) = 0;
        virtual void ReadFromFile(std::iostream& file, FileOffsetData& offset) = 0;
        virtual ~IReadVector() = 0;
        virtual void SetMesh(const MeshPtr& msh) = 0;
        OffsetType  m_attrType;
        uint32_t m_size;
    };
    inline IReadVector::~IReadVector() { }

    template<class T> class ReadVector : public IReadVector
    {
    public:
        using ios = std::ios;

        ReadVector();
        explicit ReadVector(OffsetType attrType);
        ReadVector(const ReadVector& other);
        void ReadFromFile(std::fstream& file, FileOffsetData& offset);
        void ReadFromFile(std::iostream& file, FileOffsetData& offset);
        void SetVec(std::vector<T>& vec);
        void SetMesh(const MeshPtr& msh);
        ~ReadVector() {};
        ReadVector<T> operator=(ReadVector<T> tmp);
        std::vector<T> GetVec() const;
        std::vector<T> m_vec;
    };


    template <class T>
    ReadVector<T>
    ReadVector<T>::operator=(ReadVector<T> tmp)
    {
        std::swap(m_vec, tmp.m_vec);
        std::swap(m_attrType, tmp.m_attrType);
        return *this;
    }

    template <class T>
    std::vector<T> ReadVector<T>::GetVec() const
    {
        return m_vec;
    }

    template <class T>
    ReadVector<T>::ReadVector():
        //m_vec(nullptr),
        IReadVector(NONE_TYPE)
    {

    }

    template <class T>
    ReadVector<T>::ReadVector(OffsetType attrType):
        IReadVector(attrType)
    {

    }

    template <class T>
    ReadVector<T>::ReadVector(const ReadVector<T>& other):
        m_vec(other.m_vec),
        IReadVector(other.attrType)
    {

    }

    template <class T>
    void ReadVector<T>:: SetMesh(const MeshPtr& mesh)
    {
        mesh->SetVector(std::move(m_vec));
    }

    template <class T>
    void ReadVector<T>::ReadFromFile(std::fstream& file, FileOffsetData& offset)
    {
        uint32_t size;
        file.seekp(offset.attribOffset, ios::beg);
        file.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
        m_vec.resize(size);
        m_size = size;
    
            file.read((char*)&m_vec[0], size*sizeof(T));

    }
    

    template <class T>
    void ReadVector<T>:: ReadFromFile(std::iostream& file, FileOffsetData& offset)
    {
        uint32_t size;
        file.seekp(offset.attribOffset, ios::beg);
        file.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
        m_vec.resize(size);
        m_size = size;

            file.read((char*)&m_vec[0],size* sizeof(T));
        
    }


    template<class T> class WritableVector : public IWritableVector
    {
    public:

        WritableVector();
        WritableVector(const std::vector<T>& vec, OffsetType attrType);
        uint32_t WriteToFile(std::fstream& file);

        std::vector<T> m_vec;

    };

    template <class T>
    WritableVector<T>::WritableVector():
        IWritableVector(NONE_TYPE),
        m_vec()
    {

    }

    template <class T>
    WritableVector<T>::WritableVector(const std::vector<T>& vec, OffsetType attrType):
        IWritableVector(attrType),
        m_vec(vec)
    {
    }

    template <class T>
    uint32_t  WritableVector<T>::WriteToFile(std::fstream& file)
    {
        uint32_t ret = 0;

        uint32_t size = m_vec.size();
        ret = file.tellp();
        file.write(reinterpret_cast<char*>(&size), sizeof(uint32_t));
        file.write(reinterpret_cast<char*>(m_vec.data()), sizeof(T)*size);
        return ret;
    }
}

#endif	/* CONVERTVECTOR_HPP */

