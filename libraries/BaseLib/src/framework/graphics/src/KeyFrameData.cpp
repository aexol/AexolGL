/**************************************************************
 *                                                            * 
 *      Filename:   KeyFrameData.cpp                          * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 wrz 30 14:34:17                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#include <aex/KeyFrameData.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/ReadFromAexFile.hpp>
#include <aex/SaveToAexFile.hpp>

namespace aex
{
    DEFINE_AEX_EXCEPTION(KeyFrameAnimationError);
    DEFINE_AEX_EXCEPTION(SimpleKeyFrameDataError);
    namespace _keyframe_detail {
        void enableVertexData(const ShaderProgram& program, const VertexData& vd, const AttribList& identity)
        {
            program.setVertexData(vd, identity);
        }
        void disableVertexData(const ShaderProgram& program, const VertexData& vd, const AttribList& identity)
        {
            program.setVertexData(vd, identity);
        }
        void enableIndices(const ShaderProgram& program, const IndexData& id)
        {
            program.setIndices(id);
        }
        void disableIndices(const ShaderProgram& program, const IndexData& id)
        {
            UNUSED(program);
            UNUSED(id);
        }
    } /* namespace _keyframe_detail */


    KeyFrameState::KeyFrameState(const VertexData& vd)
    {
        *this = vd;
    }
    
    KeyFrameState& KeyFrameState::operator=(const VertexData& vd)
    {
        if(&m_data != &vd)
            m_data = vd;
        return *this;
    }
    
    KeyFrameState::KeyFrameState(VertexData&& vd)
    {
        *this = std::move(vd);
    }
    
    KeyFrameState&
    KeyFrameState::operator=(VertexData&& vd)
    {
        if(&m_data != &vd)
            m_data = std::move(vd);
        return *this;
    }

    KeyFrameState::operator bool()
    {
        return bool(m_data);
    }

    void 
    KeyFrameState::setNextIdentity(const VertexAttribList& newIdentity)
    {
        m_nextMap = &newIdentity;
    }

    KeyFrameState::VertexAttribList 
    KeyFrameState::makeIdentityList(const AttribList& names)
    {
        if(names.size() != m_data.getAttribs().size())
            throw KeyFrameAnimationError("Cannot enable frame data, invalid identity for next frame.");
        auto newIdentity = m_data.getAttribs();
        unsigned i = 0;
        for(auto& attrib : newIdentity)
        {
            attrib.first = names[i];
            i++;
        }
        return newIdentity;
    }

    void 
    KeyFrameState::enableAsNext(const ShaderProgram& program) const
    {
        if(m_nextMap)
            _keyframe_detail::enableVertexData(program, m_data, *m_nextMap);
    }

    void 
    KeyFrameState::enable(const ShaderProgram& program) const
    {
        _keyframe_detail::enableVertexData(program, m_data, m_data.getAttribs());
    }

    void KeyFrameState::disableAsNext(const ShaderProgram& program) const
    {
        if(m_nextMap)
            _keyframe_detail::disableVertexData(program, m_data, *m_nextMap);
    }

    void 
    KeyFrameState::disable(const ShaderProgram& program) const
    {
        _keyframe_detail::disableVertexData(program, m_data, m_data.getAttribs());
    }

    const KeyFrameState::VertexAttribList& 
    KeyFrameState::defaultNames() const
    {
        return m_data.getAttribs();
    }

    SharedKeyFrameState::SharedKeyFrameState(const IndexData& indices, const VertexData& vd):
        m_indices(indices),
        m_sharedVertexData(vd)
    {
    }

    SharedKeyFrameState::SharedKeyFrameState(const IndexData& indices, VertexData&& vd):
        m_indices(indices),
        m_sharedVertexData(std::move(vd))
    {
    }

    SharedKeyFrameState::SharedKeyFrameState(IndexData&& indices, VertexData&& vd):
        m_indices(std::move(indices)),
        m_sharedVertexData(std::move(vd))
    {
    }

    SharedKeyFrameState::SharedKeyFrameState(IndexData&& indices, const VertexData& vd):
        m_indices(std::move(indices)),
        m_sharedVertexData(vd)
    {
    }

    SharedKeyFrameState& 
    SharedKeyFrameState::operator=(const IndexData& indices)
    {
        m_indices = indices;
        return *this;
    }

    SharedKeyFrameState&
    SharedKeyFrameState::operator=(IndexData&& indices)
    {
        m_indices = std::move(indices);
        return *this;
    }

    SharedKeyFrameState&
    SharedKeyFrameState::operator=(const VertexData& vd)
    {
        m_sharedVertexData = vd;
        return *this;
    }

    SharedKeyFrameState&
    SharedKeyFrameState::operator=(VertexData&& vd)
    {
        m_sharedVertexData = std::move(vd);
        return *this;
    }
    
    SharedKeyFrameState::~SharedKeyFrameState ()
    {
    }

    void
    SharedKeyFrameState::enable(const ShaderProgram& program)
    {
        _keyframe_detail::enableIndices(program, m_indices);
        m_sharedVertexData.enable(program);
    }

    void 
    SharedKeyFrameState::disable(const ShaderProgram& program)
    {
        m_sharedVertexData.disable(program);
        _keyframe_detail::disableIndices(program, m_indices);
    }

    KeyFrameState makeKeyFrame(const std::vector<PositionNormal>& data,
            const aex::string& posName,
            const aex::string& norName)
    {
        VertexData vd = PositionNormal::createVertexDataFromPositionNormal(data, posName, norName);
        return KeyFrameState(std::move(vd));
    }

    KeyFrameState makeKeyFrame(std::vector<PositionNormal>&& data,
            const aex::string& posName,
            const aex::string& norName)
    {
        VertexData vd = PositionNormal::createVertexDataFromPositionNormal(std::move(data), posName, norName);
        return KeyFrameState(std::move(vd));
    }

    SimpleKeyFrameData LoadSimpleKFDataFromAexFile(const aex::string& filename)
    {
        using IndicesDataArray = ReadFromAexFile::IndicesDataArray;
        using UvsArray = decltype(std::declval<SimpleKeyFrameData>().uvs);
        using KeyFrameDataArray = typename decltype(std::declval<SimpleKeyFrameData>().frames)::value_type;
        fstream animationFile(filename+".aex", std::ios::in | std::ios::binary);
        if(animationFile.is_open())
        {
            MeshType meshType;
            std::vector<FileOffsetData> offsets;
            ReadFromAexFile reader;
            reader.readFileHeader(animationFile, meshType, offsets);
            if(meshType == ANIMATED_MESH) 
            {
                SimpleKeyFrameData data;
                for(auto& offset: offsets)
                {
                    switch(offset.attrT)
                    {
                        case GL_INDICES:
                            {
                                IndicesDataArray indices;
                                reader.getVectorFromFile(animationFile, indices, offset);
                                data.indices = std::move(indices);
                                break;
                            }
                        case GL_UV:
                            {
                                UvsArray uvs;
                                reader.getArrayFromFile(animationFile, uvs, offset);
                                data.uvs = std::move(uvs);
                                break;
                            }
                        case GL_KEYFRAME:
                            {
                                KeyFrameDataArray frame;
                                reader.getArrayFromFile(animationFile, frame, offset);
                                data.frames.push_back(std::move(frame));
                                break;
                            }
                        default:
                            throw SimpleKeyFrameDataError("Corrupted data in file "+filename+".");
                    }
                }
                return data;
            } else throw SimpleKeyFrameDataError(aex::string("Data in file ") + filename + " is not an animation data.");
        } else throw SimpleKeyFrameDataError(aex::string("Could not open ")+filename+".");
    }

    void SaveSimpleKFDataToAexFile(const aex::string& filename, const SimpleKeyFrameData& data)
    {
        using indices_type = typename decltype(data.indices)::value_type;
        using uvs_type = typename decltype(data.uvs)::value_type;
        using frames_array = typename decltype(data.frames)::value_type;
        using frame_type = typename frames_array::value_type;
        using indices_vector_type = WritableVector<indices_type>;
        using uvs_vector_type = WritableVector<uvs_type>;
        using frame_vector_type = WritableVector<frame_type>;
        using i_vector_type = aex::shared_ptr<IWritableVector>;
        using i_vector_array = std::vector<i_vector_type>;

        i_vector_array iData;
        i_vector_type indices = aex::make_shared<indices_vector_type>(data.indices, GL_INDICES);
        i_vector_type uvs = aex::make_shared<uvs_vector_type>(data.uvs, GL_UV);

        iData.push_back(indices);
        iData.push_back(uvs);

        for(const auto& frame : data.frames)
        {
            i_vector_type frame_vector = aex::make_shared<frame_vector_type>(frame, GL_KEYFRAME);
            iData.push_back(frame_vector);
        }

        SaveToAexFile writer;
        writer.exportToAexFile(filename, iData);
    }

    SimpleKeyFrameAnimationData makeAnimationData(const SimpleKeyFrameData& data)
    {
        return makeAnimationData(SimpleKeyFrameData(data));
    }

    SimpleKeyFrameAnimationData makeAnimationData(SimpleKeyFrameData&& data)
    {
        SimpleKeyFrameArray kfArray(data.frames.size());
        for(unsigned i = 0; i < data.frames.size(); i++)
            kfArray[i] = makeKeyFrame(std::move(data.frames[i]));
        SimpleKeyFrameAnimationData animation(kfArray);
        animation.setSharedState(makeSharedTrianglesAndUVs(std::move(data.indices), std::move(data.uvs)));
        return animation;
    }
} /* namespace aex */
