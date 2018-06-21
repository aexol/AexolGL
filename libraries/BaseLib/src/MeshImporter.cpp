#include <aex/MeshImporter.hpp>
#include <aex/Mesh.hpp>
#include <aex/QuaternionCamera.hpp>
#include <aex/Logger.hpp>
#include <aex/Exceptions.hpp>
#include <aex/NumericCast.hpp>
#include <map>

#ifdef USING_ASSIMP

namespace aex {

    using FileModes = std::map<aex::string, std::ios_base::openmode>; 
#if defined(_MSC_VER) && !defined(__clang__)

    static std::map<aex::string, std::ios_base::openmode> create_modes_map() {
        std::map<aex::string, std::ios_base::openmode> _map;
        _map["wb"] = std::ios_base::in | std::ios_base::out | std::ios_base::binary;
        _map["w"] = std::ios_base::in | std::ios_base::out;
        _map["wt"] = std::ios_base::in | std::ios_base::out | std::ios_base::trunc;
        _map["rb"] = std::ios_base::in | std::ios_base::binary;
        _map["r"] = std::ios_base::in;
        _map["rt"] = std::ios_base::in | std::ios_base::trunc;
        return _map;
    }
    const FileModes __modesMap = create_modes_map();
#else
    const FileModes __modesMap = FileModes{
        std::make_pair("wb", std::ios_base::in | std::ios_base::out | std::ios_base::binary),
        std::make_pair("w", std::ios_base::in | std::ios_base::out),
        std::make_pair("wt", std::ios_base::in | std::ios_base::out | std::ios_base::trunc),
        std::make_pair("rb", std::ios_base::in | std::ios_base::binary),
        std::make_pair("r", std::ios_base::in),
        std::make_pair("rt", std::ios_base::in | std::ios_base::trunc)
    };
#endif

    AexAssimpIOStream::AexAssimpIOStream(const aex::string& file, const aex::string& mode) {
        FileManager fm;
        try {
            fstream f = fm.open_rw_file(file, __modesMap.at(mode));
            m_fstream = std::move(f);
        } catch (const std::out_of_range& e) {
            throw MeshImporterError("Invalid file mode.");
        }
    }

    AexAssimpIOStream::~AexAssimpIOStream() {
    }

    size_t AexAssimpIOStream::Read(void* pvBuffer, size_t pSize, size_t pCount) {
        return m_fstream.read((char*) pvBuffer, pSize * pCount).gcount();
    }

    size_t AexAssimpIOStream::Write(const void* pvBuffer, size_t pSize, size_t pCount) {
        std::streampos current_position = m_fstream.tellp();
        m_fstream.write((char*) pvBuffer, pSize * pCount);
        m_fstream.flush();
        std::streampos post_write_position = m_fstream.tellp();
        std::streamoff dist = post_write_position - current_position;
        return numeric_cast<size_t>(dist);
    }

    aiReturn AexAssimpIOStream::Seek(size_t pOffset, aiOrigin pOrigin) {
        aiReturn status = aiReturn::aiReturn_SUCCESS;
        std::ios_base::seekdir way;
        switch (pOrigin) {
            case aiOrigin::aiOrigin_SET:
                way = aex::ifstream::beg;
                break;
            case aiOrigin::aiOrigin_END:
                way = aex::ifstream::end;
                break;
            case aiOrigin::aiOrigin_CUR:
            default:
                way = aex::ifstream::cur;
        }
        m_fstream.seekg(pOffset, way);
        if (!m_fstream.good()) {
            if (m_fstream.eof())
                status = aiReturn::aiReturn_OUTOFMEMORY;
            else
                status = aiReturn::aiReturn_FAILURE;
        }
        return status;
    }

    size_t AexAssimpIOStream::Tell() const {
        // Assimp's Tell() and STDC++ Tell do not mesh to well
        // as one is binary and the other is implementation definied.
        // In all implementations I saw it could be directly casted
        // to long long and it would be fine. Being pedantic here
        // until decided otherwise.
        std::lock_guard<std::mutex> lock(m_lock);
        auto current_position = m_fstream.tellg();
        m_fstream.seekg(0, std::ios::beg);
        auto beg = m_fstream.tellg();
        m_fstream.seekg(current_position, std::ios::beg);
        return numeric_cast<size_t>(current_position - beg);
    }

    size_t AexAssimpIOStream::FileSize() const {
        std::lock_guard<std::mutex> lock(m_lock);
        auto current_position = m_fstream.tellg();
        m_fstream.seekg(0, std::ios::end);
        auto ending_position = m_fstream.tellg();
        m_fstream.seekg(0, std::ios::beg);
        auto begin_postion = m_fstream.tellg();
        m_fstream.seekg(current_position, std::ios::beg);
        return numeric_cast<size_t>(ending_position - begin_postion);
    }

    void AexAssimpIOStream::Flush() {
        m_fstream.flush();
    }

    bool AexAssimpIOStream::Opened() {
        return m_fstream.is_open();
    }

    AexAssimpIOSystem::AexAssimpIOSystem(bool trunc) {
    }

    AexAssimpIOSystem::~AexAssimpIOSystem() {
    }

    bool AexAssimpIOSystem::Exists(const aex::string& pFile) const {
        FileManager fm;
        ifstream ReadFile = fm.open_read_file(pFile, std::ios_base::in | std::ios_base::binary);
        return ReadFile.is_open();

    }

    bool AexAssimpIOSystem::Exists(const char* pFile) const {
        FileManager fm;
        ifstream ReadFile = fm.open_read_file(aex::string(pFile), std::ios_base::in | std::ios_base::binary);
        return ReadFile.is_open();
    }

    char AexAssimpIOSystem::getOsSeparator() const {
        return '/';
    }

    Assimp::IOStream* AexAssimpIOSystem::Open(const aex::string& pFile, const aex::string& pMode) {
        AexAssimpIOStream* istream = new AexAssimpIOStream(pFile, pMode);
        if (istream && !istream->Opened()) {
            delete istream;
            istream = nullptr;
        }
        return istream;
    }

    Assimp::IOStream* AexAssimpIOSystem::Open(const char* pFile, const char* pMode) {
        return Open(aex::string(pFile), aex::string(pMode));
    }

    void AexAssimpIOSystem::Close(Assimp::IOStream* pFile) {
        static_cast<AexAssimpIOStream*> (pFile)->m_fstream.close();
    }

    int seekMesh(const aiScene* scene, const aex::string& name) {
        int meshIndex = -1;
        if (scene->HasMeshes() && !name.empty()) {
            for (size_t ii = 0; ii < scene->mNumMeshes; ii++) {
                if (name.compare(scene->mMeshes[ii]->mName.C_Str()) == 0) {
                    meshIndex = ii;
                }
            }
        }

        return meshIndex;
    }

    int seekCamera(const aiScene* scene, const aex::string& name) {
        if (scene->HasCameras() && !name.empty()) {
            for (size_t ii = 0; ii < scene->mNumCameras; ii++) {
                if (name.compare(scene->mCameras[ii]->mName.C_Str()) == 0) {
                    return ii;
                }
            }
        }

        return -1;
    }

    void aiMeshToAexMesh(const aiMesh* ai_mesh, Mesh* mesh) {
        if (!ai_mesh || !mesh) {
            return;
        }

        int dataSize = ai_mesh->mNumVertices * 3;
        if (ai_mesh->HasNormals()) {
            mesh_vectors_type* first = &ai_mesh->mNormals[0].x;
            std::vector<mesh_vectors_type> normals(first, first + dataSize);
            mesh->setNormals(std::move(normals));
        }

        //TODO: Support mutiple channels in mesh?
        int n = 0;

        while (n < AI_MAX_NUMBER_OF_TEXTURECOORDS && !ai_mesh->HasTextureCoords(n)) {
            n++;
        }

        if (ai_mesh->HasTextureCoords(n)) {
            bool z = false;
            bool y = false;
            mesh_vectors_type* first = &ai_mesh->mTextureCoords[n][0].x;
            std::vector<mesh_vectors_type> uvs_temp(first, first + dataSize);

            size_t k = 2;

            while (!z && k < uvs_temp.size()) {
                if (uvs_temp[k - 1] != 0.0f) {
                    y = true;
                }

                if (uvs_temp[k] != 0.0f) {
                    y = z = true;
                    break;
                }

                k += 3;
            }

            const int step = 1 + (1 && y) + (1 && z);
            std::vector<mesh_vectors_type> uvs((uvs_temp.size() / 3) * step);

            for (size_t ii = 0, jj = 0; jj < uvs_temp.size(); ii += step, jj += 3)
                for (int kk = 0; kk < step; kk++) {
                    uvs[ii + kk] = uvs_temp[jj + kk];
                }



            mesh->setCoords(std::move(uvs));
        }

        mesh_vectors_type* first = &ai_mesh->mVertices[0].x;
        std::vector<mesh_vectors_type> positions(first, first + dataSize);
        mesh->setVertices(std::move(positions));
        std::vector<mesh_indices_type> indices(ai_mesh->mNumFaces * 3); //Triangles only, that's why *assume* 3

        for (uint ii = 0; ii < ai_mesh->mNumFaces; ii++) {
            aiFace& face = ai_mesh->mFaces[ii];
            if (face.mNumIndices != 3)
                throw MeshImporterError(aex::string("Face of model ") + ai_mesh->mName.C_Str() + " is not a triangle.");

            for (uint jj = 0; jj < face.mNumIndices; jj++) {
                indices[ii * 3 + jj] = face.mIndices[jj];
            }
        }

        mesh->setTriangles(std::move(indices));
        mesh->setMeshName(aex::string(ai_mesh->mName.C_Str()));
    }

    aiMatrix4x4 transformVectorToWorld(const aiNode* node) {
        if (node->mParent) {
            return transformVectorToWorld(node->mParent) * node->mTransformation;
        }

        return node->mTransformation;
    }

    aiVector3D transformVectorToWorld(const aiNode* node, const aiVector3D& vector) {
        if (node->mParent) {
            return transformVectorToWorld(node->mParent) * node->mTransformation * vector;
        }

        return node->mTransformation * vector;
    }

    void aiCameraToQuaternionCamera(const aiScene* scene, const aiCamera* ai_camera, QuaternionCamera* camera) {
        if (!ai_camera || !camera) {
            return;
        }

        const aiNode* cameraNode = scene->mRootNode->FindNode(ai_camera->mName);
        aiVector3D eye = transformVectorToWorld(cameraNode, ai_camera->mPosition);
        aiVector3D target = transformVectorToWorld(cameraNode, ai_camera->mLookAt);
        aiVector3D up = transformVectorToWorld(cameraNode, ai_camera->mUp);
        camera->lookAt(Vector(eye.x, eye.y, eye.z), Vector(target.x, target.y, target.z), Vector(up.x, up.y, up.z));
        camera->perspective(
                ai_camera->mHorizontalFOV,
                ai_camera->mAspect,
                ai_camera->mClipPlaneNear,
                ai_camera->mClipPlaneFar
                );
        camera->setName(aex::string(ai_camera->mName.C_Str()));
    }

    /******************SharedImport********************/
    bool checkScene(const aiScene* scene, const aex::string& fileName) {
        bool valid = true;

        if (!scene || !scene->mRootNode) {
            //            aex::string error_msg = "Could not read file: ";
            //            error_msg += fileName;
            //            throw std::runtime_error(error_msg);
            valid = false;
        }

        return valid;
    }
    SharedImport::SharedImport() :
    m_impl(nullptr, [](void* p) {

        delete static_cast<Assimp::Importer*> (p);
    }) {
    }

    bool
    SharedImport::prepare(const aex::string& fileName) {
        if (!m_impl) {
            m_impl.reset(new Assimp::Importer);
            Assimp::Importer* tmp = static_cast<Assimp::Importer*> (m_impl.get());
            tmp->SetIOHandler(new AexAssimpIOSystem(false));
        }

        unsigned int flags = aiPostProcessSteps::aiProcess_Triangulate;
        flags |= aiPostProcessSteps::aiProcess_JoinIdenticalVertices;\
        auto scene = static_cast<Assimp::Importer*> (m_impl.get())->ReadFile(fileName, flags);
        return checkScene(scene, fileName);
    }

    const void*
    SharedImport::GetImporter() const {
        return m_impl.get();
    }

    /******************ImporterBase********************/
    const SharedImport&
    ImporterBase::getFile(const aex::string& fileName) {

        if (!prepare(fileName)) {
            aex::string error_msg = "Could not read file: ";
            error_msg += fileName;
            LOG_ERR(error_msg);
            throw std::runtime_error(error_msg);
        }

        return *m_base;
    }

    aex::string ImporterBase::getFileName() {
        return m_fileName;
    }

    const SharedImport&
    ImporterBase::getFile() {
        if (!prepare()) {
            aex::string error_msg = "Could not read file: ";
            error_msg += m_fileName;
            throw std::runtime_error(error_msg);
        }

        return *m_base;
    }

    bool
    ImporterBase::prepare(const aex::string& fileName) {
        if (m_fileName.compare(fileName) != 0) {
            m_base.reset(new SharedImport);
            m_fileName = fileName;
        }
        return m_base->prepare(fileName);
    }

    bool
    ImporterBase::prepare() {
        bool valid = true;

        if (!m_base) {
            m_base.reset(new SharedImport);
            valid = m_base->prepare(m_fileName);
        }

        return valid;
    }

    const aiScene* getSceneFromBase(ImporterBase* ib, const aex::string& fileName) {
        const auto& ip = ib->getFile(fileName);
        const Assimp::Importer& impr = *static_cast<const Assimp::Importer*> (ip.GetImporter());    
        const aiScene* scene = impr.GetScene();
        checkScene(scene, fileName);
        return scene;
    }

    const aiScene* getSceneFromBase(ImporterBase* ib) {
        const auto& ip = ib->getFile();
        const Assimp::Importer& impr = *static_cast<const Assimp::Importer*> (ip.GetImporter());
        const aiScene* scene = impr.GetScene();
        if (!scene)
            throw MeshImporterError(aex::string("Could not get scene from file: " )+ ib->getFileName());
        return scene;
    }

    /******************MeshImporter********************/
    MeshImporter::MeshImporter(const ImporterBase& ib) : ImporterBase(ib) {
    }

    MeshImporter::MeshImporter(const aex::string& fileName) {
        m_isValid = prepare(fileName);
    }

    int
    MeshImporter::importMesh(Mesh* mesh) {
        return importMesh(mesh, 0);
    }

    int
    MeshImporter::importMesh(Mesh* mesh, uint meshNum) {
        const aiScene* scene = getSceneFromBase(this);
        if (scene->mNumMeshes > (unsigned)meshNum) {
            aiMeshToAexMesh(scene->mMeshes[meshNum], mesh);
            return meshNum;
        }
        return -1;
    }

    int
    MeshImporter::importMesh(Mesh* mesh, uint meshNum, const aex::string& fileName) {
        getSceneFromBase(this, fileName);
        return importMesh(mesh, meshNum);
    }

    int
    MeshImporter::importMesh(aex::Mesh* mesh, const aex::string& meshName) {
        const aiScene* scene = getSceneFromBase(this);
        return importMesh(mesh, seekMesh(scene, meshName));
    }

    int
    MeshImporter::importMesh(aex::Mesh* mesh, const aex::string& meshName, const aex::string& fileName) {
        const aiScene* scene = getSceneFromBase(this, fileName);
        return importMesh(mesh, seekMesh(scene, meshName));
    }
    
    //aiScene dumbfix;
    
    int
    MeshImporter::importAll(aex::Mesh* allMeshes) {
        const aiScene* scene = getSceneFromBase(this);
        int n = scene->mNumMeshes;

        for (int ii = 0; ii < n; ii++) {
            aiMeshToAexMesh(scene->mMeshes[ii], &(allMeshes[ii]));
        }

        return n;
    }

    int
    MeshImporter::importAll(aex::Mesh* allMeshes, const aex::string& fileName) {
        const aiScene* scene = getSceneFromBase(this, fileName);
        int n = scene->mNumMeshes;

        for (int ii = 0; ii < n; ii++) {
            aiMeshToAexMesh(scene->mMeshes[ii], &(allMeshes[ii]));
        }

        return n;
    }

    int
    MeshImporter::importAll(std::vector<Mesh>& meshes) {
        const aiScene* scene = getSceneFromBase(this);
        int n = scene->mNumMeshes;

        meshes.resize(scene->mNumMeshes);


        for (int ii = 0; ii < n; ii++) {
            aiMeshToAexMesh(scene->mMeshes[ii], &meshes[ii]);
        }

        return n;
    }

    int
    MeshImporter::importAll(std::vector<Mesh>& meshes, const aex::string& fileName) {
        const aiScene* scene = getSceneFromBase(this, fileName);
        int n = scene->mNumMeshes;

        meshes.resize(scene->mNumMeshes);

        for (int ii = 0; ii < n; ii++) {
            aiMeshToAexMesh(scene->mMeshes[ii], &meshes[ii]);
        }

        return n;
    }

    int
    MeshImporter::GetNumMeshes() {
        const aiScene* scene = getSceneFromBase(this);
        return scene->mNumMeshes;
    }

    int
    MeshImporter::GetNumMeshes(const aex::string& fileName) {
        const aiScene* scene = getSceneFromBase(this, fileName);
        return scene->mNumMeshes;
    }

    /******************MeshImporter********************/
    CameraImporter::CameraImporter(const ImporterBase& ib) : ImporterBase(ib) {
    }

    CameraImporter::CameraImporter(const aex::string& fileName) {
        prepare(fileName);
    }

    int
    CameraImporter::importCamera(QuaternionCamera* camera) {
        return importCamera(camera, 0);
    }

    int
    CameraImporter::importCamera(QuaternionCamera* camera, uint cameraNum) {
        const aiScene* scene = getSceneFromBase(this);

        if (scene->mNumCameras > (unsigned)cameraNum) {
            aiCameraToQuaternionCamera(scene, scene->mCameras[cameraNum], camera);
            return cameraNum;
        }
        return -1;
    }

    int
    CameraImporter::importCamera(QuaternionCamera* camera, uint cameraNum, const aex::string& fileName) {
        return importCamera(camera, cameraNum);
    }

    int
    CameraImporter::importCamera(QuaternionCamera* camera, const aex::string& cameraName) {
        const aiScene* scene = getSceneFromBase(this);
        return importCamera(camera, seekCamera(scene, cameraName));
    }

    int
    CameraImporter::importCamera(QuaternionCamera* camera, const aex::string& cameraName, const aex::string& fileName) {
        const aiScene* scene = getSceneFromBase(this, fileName);
        return importCamera(camera, seekCamera(scene, cameraName));
    }

    int
    CameraImporter::importAll(QuaternionCamera* allCameras) {
        const aiScene* scene = getSceneFromBase(this);
        int n = scene->mNumCameras;

        for (int ii = 0; ii < n; ii++) {
            aiCameraToQuaternionCamera(scene, scene->mCameras[ii], &(allCameras[ii]));
        }

        return n;
    }

    int
    CameraImporter::importAll(QuaternionCamera* allCameras, const aex::string& fileName) {
        const aiScene* scene = getSceneFromBase(this, fileName);
        int n = scene->mNumCameras;

        for (int ii = 0; ii < n; ii++) {
            aiCameraToQuaternionCamera(scene, scene->mCameras[ii], &(allCameras[ii]));
        }

        return n;
    }

    int _importAll(std::vector<QuaternionCamera>& cameras, const aiScene* scene) {
        const int cam_size = numeric_cast<int>(cameras.size());
        int n = scene->mNumCameras >= cam_size ? scene->mNumCameras : cam_size;

        for (int ii = 0; ii < n; ii++) {
            aiCameraToQuaternionCamera(scene, scene->mCameras[ii], &(cameras[ii]));
        }

        return n;
    }

    int
    CameraImporter::importAll(std::vector<QuaternionCamera>& cameras) {
        const aiScene* scene = getSceneFromBase(this);
        return _importAll(cameras, scene);
    }

    int
    CameraImporter::importAll(std::vector<QuaternionCamera>& cameras, const aex::string& fileName) {
        const aiScene* scene = getSceneFromBase(this, fileName);
        return _importAll(cameras, scene);
    }

    int
    CameraImporter::GetNumCameras() {
        const aiScene* scene = getSceneFromBase(this);
        return scene->mNumCameras;
    }

    int
    CameraImporter::GetNumCameras(const aex::string& fileName) {
        const aiScene* scene = getSceneFromBase(this, fileName);
        return scene->mNumCameras;
    }
}
#endif
