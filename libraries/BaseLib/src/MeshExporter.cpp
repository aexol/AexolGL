#include <limits>
#include <aex/Common.hpp>
#include <aex/MeshExporter.hpp>
#include <aex/Mesh.hpp>
#include <aex/Logger.hpp>
#include <aex/NumericCast.hpp>

#ifdef USING_ASSIMP

#include <assimp/Exporter.hpp>

namespace aex
{
    void AexMeshToAssimpMesh(Mesh* mesh, aiMesh* ai_mesh)
    {
        if (!ai_mesh || !mesh)
        {
            return;
        }

        Mesh::index_array_type& indices = mesh->getIndices();
        Mesh::vertex_array_type& vertices = mesh->getVertices();

        Mesh::vertex_array_type* verticesPtr = new aex::Mesh::vertex_array_type();
        Mesh::vertex_array_type* fakeUV = new aex::Mesh::vertex_array_type();
        fakeUV->resize(vertices.size(), 0.0f);
        verticesPtr->insert(verticesPtr->begin(), vertices.begin(), vertices.end());


        using AssimpVertCountType = decltype(ai_mesh->mNumVertices);
        AssimpVertCountType numVertices = numeric_cast<AssimpVertCountType>(vertices.size() / 3);
        ai_mesh->mNumVertices = numVertices;
        ai_mesh->mNumUVComponents[0] = numVertices;

        ai_mesh->mVertices = (aiVector3D*) verticesPtr->data();
        ai_mesh->mTextureCoords[0] = (aiVector3D*) fakeUV->data();

        using AssimpFaceCountType = decltype(ai_mesh->mNumFaces);
        AssimpFaceCountType numFaces = numeric_cast<AssimpFaceCountType>(indices.size() / 3);
        ai_mesh->mNumFaces = numFaces;
        ai_mesh->mFaces = new aiFace[numFaces];


        for (int ii = 0; ii < numFaces; ii++)
        {
            aiFace& face = ai_mesh->mFaces[ii];
            face.mNumIndices = 3;
            face.mIndices = new unsigned int[3];

            for (uint jj = 0; jj < face.mNumIndices; jj++)
            {
                face.mIndices[jj] = indices[(ii * 3) + jj];
            }
        }

    }

    /******************SharedExport********************/

    SharedExport::SharedExport() :
    m_impl(nullptr, [](void* p)
    {

        delete static_cast<Assimp::Exporter*> (p);
    }) { }

    bool
    SharedExport::prepare()
    {
        if (!m_impl)
        {
            m_impl.reset(new Assimp::Exporter);
            Assimp::Exporter* tmp = static_cast<Assimp::Exporter*> (m_impl.get());
            tmp->SetIOHandler(new AexAssimpIOSystem(true));
        }
        return true;
    }

    void*
    SharedExport::GetExporter() const
    {
        return m_impl.get();
    }

    /******************ExportBase********************/
    const SharedExport&
    ExporterBase::getFile(const aex::string& fileName)
    {

        if (!prepare(fileName))
        {
            aex::string error_msg = "Could not write file: ";
            error_msg += fileName;
            throw std::runtime_error(error_msg);
        }

        return *m_base;
    }

    const SharedExport&
    ExporterBase::getFile()
    {
        if (!prepare())
        {
            aex::string error_msg = "Could not write file: ";
            error_msg += m_fileName;
            throw std::runtime_error(error_msg);
        }

        return *m_base;
    }

    bool
    ExporterBase::prepare(const aex::string& fileName)
    {
        if (m_fileName.compare(fileName) != 0)
        {
            m_base.reset(new SharedExport);
            m_fileName = fileName;
        }

        return m_base->prepare();
    }

    bool
    ExporterBase::prepare()
    {
        bool valid = true;

        if (!m_base)
        {
            m_base.reset(new SharedExport);
            valid = m_base->prepare();
        }

        return valid;
    }

    /******************MeshExporter********************/

    MeshExporter::MeshExporter()
    {
        prepare();
    }

    MeshExporter::MeshExporter(const aex::string& fileName)
    {
        prepare(fileName);
    }

    void MeshExporter::exportMesh(const aex::string& filename, Mesh* mesh)
    {

        aiScene scene;
        scene.mRootNode = new aiNode();
        scene.mRootNode->mName = "root";
        //scene.mRootNode->mTransformation= aiMatrix4x4
        scene.mMaterials = new aiMaterial*[1];
        scene.mMaterials[0] = new aiMaterial();
        scene.mMaterials[0]->mNumProperties = 0;
        scene.mMaterials[0]->mProperties = new aiMaterialProperty*[1];
        scene.mMaterials[0]->mProperties[0] = new aiMaterialProperty();
        scene.mMaterials[0]->mProperties[0]->mKey = "Name";
        scene.mNumMaterials = 1;

        char* a = new char;
        scene.mMaterials[0]->mProperties[0]->mData = a;

        scene.mMeshes = new aiMesh*[1];
        scene.mMeshes[0] = new aiMesh();
        scene.mMeshes[0]->mMaterialIndex = 0;
        scene.mNumMeshes = 1;

        scene.mRootNode->mMeshes = new unsigned int[1];
        scene.mRootNode->mMeshes[0] = 0;
        scene.mRootNode->mNumMeshes = 1;

 
        AexMeshToAssimpMesh(mesh, scene.mMeshes[0]);
    }
}
#endif/* USING_ASSIMP */
