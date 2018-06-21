/*
 * File:   Aex_MeshImporter.hpp
 * Author: mritke
 *
 * Created on November 3, 2014, 10:42 AM
 */
#ifndef __AEX_MESHIMPORTER_HPP
#define __AEX_MESHIMPORTER_HPP
#include <aex/Common.hpp>
#include <aex/Aex.hpp>
#include <aex/fstream.hpp>
#include <aex/FileManager.hpp>
#include <map>
#include <memory>
#include <functional>
#include <mutex>

#ifdef USING_ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/matrix4x4.h>
#include <assimp/vector3.h>
#include <assimp/postprocess.h>
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

namespace aex
{
    class Mesh;
    class QuaternionCamera;
    class SharedImport;

    class AEX_PUBLIC_API AexAssimpIOStream : public Assimp::IOStream
    {
        friend class AexAssimpIOSystem;

    private:
        mutable std::mutex m_lock;
        mutable aex::fstream m_fstream;
    protected:
        AexAssimpIOStream(const aex::string& file, const aex::string& pMode);
    public:

        ~AexAssimpIOStream();
        size_t Read(void* pvBuffer, size_t pSize, size_t pCount);
        size_t Write(const void* pvBuffer, size_t pSize, size_t pCount);
        aiReturn Seek(size_t pOffset, aiOrigin pOrigin);
        size_t Tell() const;
        size_t FileSize() const;
        void Flush();
        bool Opened();
    };

    class AEX_PUBLIC_API AexAssimpIOSystem : public Assimp::IOSystem
    {
    protected:

    public:

        explicit AexAssimpIOSystem(bool trunc = false);
        ~AexAssimpIOSystem();

        bool Exists(const aex::string& pFile) const;
        bool Exists(const  char* pFile) const;
        char getOsSeparator() const;
        Assimp::IOStream* Open(const aex::string& pFile, const aex::string& pMode);
        Assimp::IOStream* Open(const char* pFile, const char* pMode);
        void Close(Assimp::IOStream* pFile);

    };

    class AEX_PUBLIC_API SharedImport
    {
    public:
        SharedImport();
        const void* GetImporter() const;
    private:
        bool prepare(const aex::string& fileName);
        std::unique_ptr<void, std::function<void(void*)>> m_impl;
        friend class ImporterBase;
    };

    class AEX_PUBLIC_API ImporterBase
    {
    public:
        const SharedImport& getFile(const aex::string& fileName);
        const SharedImport& getFile();
        aex::string getFileName();
    protected:
        bool prepare(const aex::string& fileName);
        bool prepare();
    private:
        aex::shared_ptr<SharedImport> m_base;
        aex::string m_fileName;
    };

    const aiScene* getSceneFromBase(ImporterBase* ib, const aex::string& fileName);
    const aiScene* getSceneFromBase(ImporterBase* ib);

    class AEX_PUBLIC_API MeshImporter : public ImporterBase
    {
        bool m_isValid = false;
    public:
        MeshImporter() = default;
        MeshImporter(const MeshImporter&) = default;
        MeshImporter(MeshImporter&&) = default;
        MeshImporter& operator=(const MeshImporter&) = default;
        MeshImporter& operator=(MeshImporter&&) = default;
        explicit MeshImporter(const ImporterBase& ib);
        explicit MeshImporter(const aex::string& fileName);
        int importMesh(Mesh* mesh);
        int importMesh(Mesh* mesh, uint meshNum);
        int importMesh(Mesh* mesh, uint meshNum, const aex::string& fileName);
        int importMesh(Mesh* mesh, const aex::string& meshName);
        int importMesh(Mesh* mesh, const aex::string& meshName, const aex::string& fileName);
        int importAll(Mesh* allMeshes);
        int importAll(Mesh* allMeshes, const aex::string& fileName);
        int importAll(std::vector<Mesh>& meshes);
        int importAll(std::vector<Mesh>& meshes, const aex::string& fileName);
        int GetNumMeshes();
        int GetNumMeshes(const aex::string& fileName);
    };


    struct AEX_PUBLIC_API AexNode
    {
        int nodeNumber = -1;
        aex::math::Vector3 scale;
        aex::math::Vector3 position ;
        aex::math::Quaternion orientation;
        aex::string nodename = "";
        std::vector<int> meshlist;
        std::vector<int> materialList;
        std::vector<std::string> materialNameList;
        bool isLeaf = false;
        int parentInt = -1;
        std::vector<int> childrenInt;
    };

    struct AexMesh
    {
         aex::Aex_ptr aex = nullptr;
         aex::Mesh_ptr mesh = nullptr;
         std::vector<std::string> materialNameList;
         bool  isRoot = false;
         AexMesh(){}
         AexMesh(aex::Aex_ptr caex, aex::Mesh_ptr cmesh );
         AexMesh(aex::Aex_ptr caex, aex::Mesh_ptr cmesh, std::vector<std::string> materialNameList);
    };

    void aiNodesToAexNodes(aiNode* assimpNode, std::vector<AexNode>& nodes, int index = -1);
    std::vector<AexNode> getAexNodes(aiNode* assimpNode, const aiScene* scene);

    class AEX_PUBLIC_API TransformationImporter : public ImporterBase
    {
        bool m_isValid = false;
    public:
        TransformationImporter() = default;
        TransformationImporter(const TransformationImporter&) = default;
        TransformationImporter(TransformationImporter&&) = default;
        TransformationImporter& operator=(const TransformationImporter&) = default;
        TransformationImporter& operator=(TransformationImporter&&) = default;
        explicit TransformationImporter(const ImporterBase& ib);
        explicit TransformationImporter(const aex::string& fileName);
        std::vector<AexNode> getNodes(const aex::string& fileName);
        std::vector<AexNode> getNodes( );
        std::map<aex::string , std::vector<AexMesh>>  importMap(const aex::string& fileName);
    };




    class AEX_PUBLIC_API CameraImporter : public ImporterBase
    {
    public:
        CameraImporter() = default;
        CameraImporter(const CameraImporter&) = default;
        CameraImporter(CameraImporter&&) = default;
        CameraImporter& operator=(const CameraImporter&) = default;
        CameraImporter& operator=(CameraImporter&&) = default;
        explicit CameraImporter(const ImporterBase& ib);
        explicit CameraImporter(const aex::string& fileName);
        int importCamera(QuaternionCamera* camera);
        int importCamera(QuaternionCamera* camera, uint cameraNum);
        int importCamera(QuaternionCamera* camera, uint cameraNum, const aex::string& fileName);
        int importCamera(QuaternionCamera* camera, const aex::string& cameraName);
        int importCamera(QuaternionCamera* camera, const aex::string& cameraName, const aex::string& fileName);
        int importAll(QuaternionCamera* allCameras);
        int importAll(QuaternionCamera* allCameras, const aex::string& fileName);
        int importAll(std::vector<QuaternionCamera>& cameras);
        int importAll(std::vector<QuaternionCamera>& cameras, const aex::string& fileName);
        int GetNumCameras();
        int GetNumCameras(const aex::string& fileName);
    };
}

#endif
#endif
