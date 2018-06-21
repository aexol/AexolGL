#include <aex/ReadFromAexFile.hpp>
#include <aex/SaveToAexFile.hpp>
#include <aex/AnimationDrw.hpp>
#include <aex/Logger.hpp>
#include <aex/ResourceDirector.hpp>
#include <aex/Mesh.hpp>
#include <aex/VertexData.hpp>
#include <aex/ShaderDrw.hpp>
#include <aex/ShaderProgram.hpp>


///Ku pamięci ::WILEOWĄTKOWOŚĆ:: compile nie moze się wywoływać z innego wątku niż wątek contextu GL.
namespace aex
{


    std::atomic<uint32_t> AnimationDrw::NAME_COUNTER(0);

    AnimationDrw::AnimationDrw() :
        m_animName("AnimationDrw" + aex::to_string(AnimationDrw::NAME_COUNTER++)),
        m_animationTime(0),
        m_uniformFrameDuration(41),
        m_rewind(false),
        m_framesUniformTime(true),
        m_paused(false),
        m_loaded(false),
        m_currentFrame(0),
        m_bindPose(),
        m_frames(nullptr)
    {
    }

    AnimationDrw::AnimationDrw(const aex::string& name) :
        m_animName(name),
        m_animationTime(0),
        m_uniformFrameDuration(40),
        m_rewind(false),
        m_framesUniformTime(true),
        m_paused(false),
        m_loaded(false),
        m_currentFrame(0),
        m_bindPose(),
        m_frames(nullptr)
    {

    }

    void AnimationDrw::SetCurrentTime(aexTimeUnit CurrentTime)
    {
        m_currentTime = CurrentTime;
    }

    aexTimeUnit AnimationDrw::GetCurrentTime() const
    {
        return m_currentTime;
    }


//    void AnimationDrw::SetNormalsForLastFrame(vector<mesh_vectors_type>&& vertNormals)
//    {
//        m_frames->m_frames[m_frames->m_frames.size()-1].SetFrameNormals(move(vertNormals));
//    }

    void
    AnimationDrw::LoadFramesFromAexMesh(const aex::string& file)
    {
        ReadFromAexFile readFile;

        readFile.ImportFromAexFile(file, *this);
    }

    void AnimationDrw::SetAnimatedMesh()
    {
        m_animatedMesh.m_indexVbo = IndexData::createTriangles(m_bindPose->getTriangles());
        m_animatedMesh.m_vertexVbo = VertexData::makeVertexData(m_bindPose->getVertices(), "gl_Vertex", 3);

        if(m_bindPose->getNormals())
        {
            m_animatedMesh.m_normalsVbo = VertexData::makeVertexData(*m_bindPose->getNormals(), "gl_Normal", 3);
        }

        if(m_bindPose->getCoords())
        {
            m_animatedMesh.m_uvVbo = VertexData::makeVertexData(*m_bindPose->getCoords(), "gl_TexCoord", 2);
        }

        if(m_frameDataHandle && m_frames->m_frames.size() != 0)
        {
            m_animatedMesh.m_nextFrameVbo = m_frames->m_frames[1].GetFrame();
        }

        m_loaded = true;
    }

    void AnimationDrw::UpdateMesh(IAnimatedMesh& mesh)
    {
        if(!m_animatedMesh)
        {
            SetAnimatedMesh();
        }
        mesh.setPosition(m_animatedMesh.m_vertexVbo);
        mesh.setNormals(m_animatedMesh.m_normalsVbo);
        mesh.setPosition(m_animatedMesh.m_uvVbo);
        mesh.setNextFrame(m_animatedMesh.m_nextFrameVbo);
        mesh.setIndices(m_animatedMesh.m_indexVbo);
        mesh.SetTweenFactor(m_animatedMesh.m_TweenFactor);
    }

    void AnimationDrw::SetCurrentFrame(int currentFrame)
    {
        m_currentFrame = currentFrame;
    }

    int AnimationDrw::GetCurrentFrame() const
    {
        return m_currentFrame;
    }

    void AnimationDrw::SetFramesUniformTime(bool framesUniformTime)
    {
        m_framesUniformTime = framesUniformTime;
    }

    bool AnimationDrw::IsFramesUniformTime() const
    {
        return m_framesUniformTime;
    }

    void AnimationDrw::SetRewind(bool rewind)
    {
        m_rewind = rewind;
    }

    bool AnimationDrw::IsRewind() const
    {
        return m_rewind;
    }

    float AnimationDrw::GetTweenFactor() const
    {
        return m_animatedMesh.m_TweenFactor;
    }

    void AnimationDrw::SetFramesPerSecond(uint16_t framesPerSecond)
    {
        m_framesPerSecond = framesPerSecond;

    }

    uint16_t AnimationDrw::GetFramesPerSecond() const
    {
        return m_framesPerSecond;
    }

    void AnimationDrw::SetUniformFrameDuration(aexTimeUnit uniformFrameDuration)
    {
        m_uniformFrameDuration = uniformFrameDuration;
    }

    aexTimeUnit AnimationDrw::GetUniformFrameDuration() const
    {
        return m_uniformFrameDuration;
    }

    aexTimeUnit AnimationDrw::GetDurationTime() const
    {
        return m_durationTime;
    }

    void AnimationDrw::SetEndTime(aexTimeUnit endTime)
    {
        m_endTime = endTime;
    }

    aexTimeUnit AnimationDrw::GetEndTime() const
    {
        return m_endTime;
    }

    KeyframeVertex::KeyframeVertex(aexTimeUnit startT, aexTimeUnit endT, const vector<mesh_vectors_type>& frame) :
        m_startTime(startT),
        m_endTime(endT),
        m_frame(),
        m_frameData(frame)
    {
        m_durationTime = m_endTime - m_startTime;

    }

    KeyframeVertex::KeyframeVertex(aexTimeUnit startT, aexTimeUnit endT, vector<mesh_vectors_type>&& frame) :
        m_startTime(startT),
        m_endTime(endT),
        m_frame(),
        m_frameData(std::move(frame))
    {
        m_durationTime = m_endTime - m_startTime;
    }

    KeyframeVertex::KeyframeVertex(aexTimeUnit startT, aexTimeUnit endT, std::vector<mesh_vectors_type>&& frame, std::vector<mesh_vectors_type>&& frameNormals) :
        m_startTime(startT),
        m_endTime(endT),
        m_frame(),
        m_frameNormals(),
        m_frameData(std::move(frame)),
        m_frameNormalsData(std::move(frameNormals))
    {
        m_durationTime = m_endTime - m_startTime;
    }

    KeyframeVertex::~KeyframeVertex()
    {}

    void
    KeyframeVertex::SetFrame(const vector<mesh_vectors_type>& vert)
    {
        m_frameData = vert;
    }

    void
    KeyframeVertex::SetFrameNormals(const vector<mesh_vectors_type>& vertNormals)
    {
        m_frameNormalsData = vertNormals;
    }

    VertexData&
    KeyframeVertex::GetFrame()
    {
        if(!m_frameData.empty())
        {
            m_frame = VertexData::makeVertexData(m_frameData, "gl_Vertex", 3);
            m_frameData.clear();
        }

        return m_frame;
    }

    VertexData&
    KeyframeVertex::GetNormals()
    {
        if(!m_frameNormalsData.empty())
        {
            m_frameNormals = VertexData::makeVertexData(m_frameNormalsData, "gl_Normal", 3);
            m_frameNormalsData.clear();
        }

        return m_frameNormals;
    }

    const VertexData&
    KeyframeVertex::GetFrame() const
    {
        return m_frame;
    }

    const std::vector<mesh_vectors_type>&
    KeyframeVertex::GetFrameData() const
    {
        return m_frameData;
    }

    const VertexData&
    KeyframeVertex::GetNormals() const
    {
        return m_frameNormals;
    }

    void
    AnimationDrw::FindCurrentFrame(aexTimeUnit time)
    {
        m_currentTime = time;
        FindCurrentFrame();

    }

    void
    AnimationDrw::FindCurrentFrame()
    {

        if(!m_rewind)
        {
            m_animationTime = fmod((m_animationTime + (m_currentTime - m_lastFrameTime)), m_durationTime);

        }
        else
        {
            m_animationTime = fmod((m_animationTime - (m_currentTime - m_lastFrameTime)), m_durationTime);

            if(m_animationTime < 0)
            {

                m_animationTime = m_durationTime + m_animationTime;

            }
        }

        if(!m_framesUniformTime)
        {
            int tempframe = 0;


            for(KeyframeVertex frame : m_frames->m_frames)
            {

                if(frame.GetEndTime() < m_animationTime)
                {
                    m_currentFrame = tempframe;
                    break;
                }

                tempframe++;
            }

        }
        else
        {
            float temp = (float) m_animationTime / (float) m_uniformFrameDuration;
            m_currentFrame = ((int) temp) % m_animationSize;
        }
    }

    float
    AnimationDrw::CalcTweenFactor()
    {

        aexTimeUnit frameTime = m_animationTime - m_frames->m_frames[m_currentFrame].GetStartTime();

        if(m_rewind)
        {
            frameTime = m_frames->m_frames[m_currentFrame].GetEndTime() - m_animationTime;
        }

        float ret = (float) frameTime / (float)(m_frames->m_frames[m_currentFrame].GetDurationTime());

        if(m_rewind)
        {
            ret = 1.0f - ret;
        }

        m_animatedMesh.m_TweenFactor = ret;
        LOG_DEBUG("Tween: ", ret);
        return ret;
    }

    void
    AnimationDrw::ChangeMeshFrame()
    {

        if(m_lastMeshFrame != m_currentFrame)
        {
            m_lastMeshFrame = m_currentFrame;
            m_animatedMesh.m_vertexVbo = m_frames->m_frames[m_currentFrame].GetFrame();
            m_animatedMesh.m_normalsVbo = m_frames->m_frames[m_currentFrame].GetNormals();

            if(!m_rewind)
            {

                m_animatedMesh.m_nextFrameVbo = m_frames->m_frames[(m_currentFrame + 1) % (m_animationSize)].GetFrame();
                LOG_DEBUG("Between frame: ", m_currentFrame, " and ", (m_currentFrame + 1) % (m_animationSize));

            }
            else
            {
                int noNegative = m_currentFrame - 1;

                if(noNegative < 0)
                {
                    noNegative = m_animationSize - 1;
                }

                m_animatedMesh.m_nextFrameVbo = m_frames->m_frames[noNegative].GetFrame();
            }
        }
    }

    void
    AnimationDrw::Update(aexTimeUnit time)
    {
        if(!m_paused)
        {
            FindCurrentFrame(time);
            ChangeMeshFrame();
            CalcTweenFactor();
            m_lastFrameTime = time;
        }

    }

    void
    AnimationDrw::StartAnimation(aexTimeUnit time)
    {
        m_startTime = time;
        m_endTime = time + m_durationTime;
    }

    void
    AnimationDrw::EndAnimation()
    {


    }

    void
    AnimationDrw::PauseAnimation()
    {

        m_paused = true;

    }

    void
    AnimationDrw::setBindPose(const aex::string& name)
    {
        ResourceDirector& rDirector = ResourceDirector::GetInstance();
        auto& meshManager = rDirector.GetManager<Mesh>();
        m_bindPoseHandle = meshManager.GetResourceHandle(name);
        (*m_bindPoseHandle)([this](Mesh & mesh)
        {
            m_bindPose = &mesh;
        });
    }

    void
    AnimationDrw::setFrameData(const aex::string& name)
    {
        ResourceDirector& rDirector = ResourceDirector::GetInstance();
        auto& frameManager = rDirector.GetManager<FrameData>();
        m_frameDataHandle = frameManager.GetResourceHandle(name);
        (*m_frameDataHandle)([this](FrameData & data)
        {
            m_frames = &data;
        });
    }

    void
    AnimationDrw::buildPerFacePerVertexNormals()
    {
        auto old_indices = m_bindPose->getTriangles();
        m_bindPose->buildPerFacePerVertexNoramals();

        if(m_frames)
        {
            for(auto& frame : m_frames->m_frames)
            {
                std::vector<mesh_vectors_type> new_noramls;
                std::vector<mesh_vectors_type> new_vertices;
                const std::vector<mesh_vectors_type>& old_vertices = frame.GetFrameData();

                for(mesh_indices_type i = 0; i < old_indices.size(); ++i)
                {
                    new_vertices.push_back(old_vertices[old_indices[i] * 3]);
                    new_vertices.push_back(old_vertices[old_indices[i] * 3 + 1]);
                    new_vertices.push_back(old_vertices[old_indices[i] * 3 + 2]);
                }

                new_noramls = makeFaceNormals(new_vertices, old_indices);
                frame.SetFrame(new_vertices);
                frame.SetFrameNormals(new_noramls);
            }
        }
    }

    void
    AnimationDrw::addReversedFaces()
    {
        m_bindPose->addReversedFaces();

    }

    void
    AnimationDrw::addBarycentricCoordinates()
    {
        m_bindPose->addBarycentricCoordinates();

    }


    void
    AnimatedMesh::draw(AexRenderer* gl)
    {
        const ShaderDrw* shader = gl->getShader();

        if(shader)
        {
            const AexShader& program = shader->GetProgram();

            if(m_vertexVbo)
            {
                program->setVertexData(m_vertexVbo);
            }

            if(m_uvVbo)
            {
                program->setVertexData(m_uvVbo);
            }

            if(m_normalsVbo)
            {
                program->setVertexData(m_normalsVbo);
            }

            if(m_indexVbo)
            {
                program->setIndices(m_indexVbo);
            }

            if(m_nextFrameVbo)
            {
                program->setVertexData(m_nextFrameVbo, "gl_nextFrame");
            }

            m_tweenFactorUniform.setValue(m_TweenFactor);
            m_tweenFactorUniform.setUniform(program);
        }
    }

    void
    AnimatedMesh::finishdraw(AexRenderer* gl)
    {
        const ShaderDrw* shader = gl->getShader();

        if(shader)
        {
            const AexShader& program = shader->GetProgram();

            if(m_vertexVbo)
            {
                program->releaseVertexData(m_vertexVbo);
            }

            if(m_uvVbo)
            {
                program->releaseVertexData(m_uvVbo);
            }

            if(m_normalsVbo)
            {
                program->releaseVertexData(m_normalsVbo);
            }

            if(m_nextFrameVbo)
            {
                program->releaseVertexData(m_nextFrameVbo, "gl_nextFrame");
            }
        }
    }

    void
    AnimatedMesh::needsUpdate()
    {
    }

    void
    AnimatedMesh::setName(const aex::string& name)
    {
        m_name = name;
    }

    void
    AnimatedMesh::SetTweenFactor(float tween)
    {
        m_TweenFactor = tween;
    }

    void
    AnimatedMesh::setPosition(const VertexData& position)
    {
        m_vertexVbo = position;
    }

    void
    AnimatedMesh::setNormals(const VertexData& normals)
    {
        m_normalsVbo = normals;
    }

    void
    AnimatedMesh::setUVs(const VertexData& uvs)
    {
        m_uvVbo = uvs;
    }

    void
    AnimatedMesh::setNextFrame(const VertexData& nextFrame)
    {
        m_nextFrameVbo = nextFrame;
    }

    void
    AnimatedMesh::setIndices(const IndexData& indices)
    {
        m_indexVbo = indices;
    }

    AnimationDrw::FrameDrawable::operator bool()
    {
        return bool(m_vertexVbo);
    }
}
