/**************************************************************
 *                                                            * 
 *      Filename:   KeyFrameData.hpp                          * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 wrz 30 12:29:28                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#ifndef KEYFRAMEDATA_HPP
#define KEYFRAMEDATA_HPP 

#include <aex/Common.hpp>
#include <aex/VertexData.hpp>
#include <aex/Uniforms.hpp>
#include <aex/FrameData.hpp>
#include <aex/AexRenderer.hpp>
#include <aex/ShaderDrw.hpp>
#include <aex/Exceptions.hpp>

namespace aex
{
    class AexRenderer;
    class ShaderProgram;
    DECLARE_AEX_EXCEPTION(KeyFrameAnimationError);
    DECLARE_AEX_EXCEPTION(SimpleKeyFrameDataError);
    namespace _keyframe_detail {
        using AttribList = VertexData::AttribList;
    } /* namespace _keyframe_detail */

    /**
     * @brief Unique frame state.
     *
     * Object keeps frame unique data.
     *
     * @tparam VertexDataType Frame data type.
     */
    class AEX_PUBLIC_API KeyFrameState
    {
    public:
        using VertexAttribList = _keyframe_detail::AttribList;
        using AttribList = std::vector<aex::string>;
        KeyFrameState() = default;
        ~KeyFrameState() = default;
        explicit KeyFrameState(const VertexData& vd);
        KeyFrameState& operator=(const VertexData& vd);
        explicit KeyFrameState(VertexData&& vd);
        KeyFrameState& operator=(VertexData&& vd);

        /**
         * @brief Convert to bool, value equal true if data is set.
         *
         * @return True if data is set.
         */
        operator bool();

        /**
         * @brief Set new names for attributes when frame is used as next frame in drawing.
         *
         * @param newIdentity Array of names.
         */
        void setNextIdentity(const VertexAttribList& newIdentity);

        /**
         * @brief Create new attribute maping using supplied names.
         *
         * @param names List of new names for attribute.
         *
         * @return New attribute mapping.
         */
        VertexAttribList makeIdentityList(const AttribList& names);

        /**
         * @brief Use frame as next frame in shader.
         *
         * @param program Rendering program.
         */
        void enableAsNext(const ShaderProgram& program) const;

        /**
         * @brief Use frame as first frame in shader.
         *
         * @param program Rendering program.
         */
        void enable(const ShaderProgram& program) const;

        /**
         * @brief Disable frame that was used as next frame in shader.
         *
         * @param program Rendering program.
         */
        void disableAsNext(const ShaderProgram& program) const;

        /**
         * @brief Disable frame that was used as first frame in shader.
         *
         * @param program Renderer pointer.
         */
        void disable(const ShaderProgram& program) const;

        const VertexAttribList& defaultNames() const;
    
    private:
        VertexData m_data;
        /**
         * @brief Attrib identities when frame is used as next frame.
         */
        const VertexAttribList* m_nextMap;
    };

    /**
     * @brief Frame data shared between states.
     *
     * Shared data between animation frames. Contains indices and arbitrary VertexData.
     *
     * @tparam VertexDataType
     */
    class AEX_PUBLIC_API SharedKeyFrameState
    {
        public:
            explicit SharedKeyFrameState (const IndexData& indices = IndexData(), const VertexData& vd = VertexData());
            explicit SharedKeyFrameState (const IndexData& indices, VertexData&& vd);
            explicit SharedKeyFrameState (IndexData&& indices, VertexData&& vd);
            explicit SharedKeyFrameState (IndexData&& indices, const VertexData& vd);
            SharedKeyFrameState& operator=(const IndexData& indices);
            SharedKeyFrameState& operator=(IndexData&& indices);
            SharedKeyFrameState& operator=(const VertexData& vd);
            SharedKeyFrameState& operator=(VertexData&& vd);
            ~SharedKeyFrameState();

            void enable(const ShaderProgram& program);

            void disable(const ShaderProgram& program);
    
        private:
            IndexData m_indices;
            KeyFrameState m_sharedVertexData;
    };

    /**
     * @brief Fully qualified frame.
     * 
     * Fully qualified frame, meanig, it's an object that contains all data needed to draw a frame. Be it
     * data shared between frames or unique to each frame.
     *
     * @tparam UniqueStateVertexType Unique data type.
     * Must define type:
     *  VertexAttribList (type of attribute definition list).
     *  AttribList (type of attribute name list).
     * Must implement:
     *  enable(const ShaderProgram& program),
     *  enableAsNext(const ShaderProgram& program),
     *  disable(const ShaderProgram& program),
     *  disableAsNext(const ShaderProgram& program),
     *  setNextIdentity(const VertexAttribList&),
     *  makeNewIdentity(const AttribList&)
     * @tparam SharedStateType Shared frame data type.
     * Must implement:
     *  enable(AexRenderer*),
     *  disable(AexRenderer*).
     */
    template<class UniqueStateVertexType, class SharedStateType>
    class KeyFrame
    {
        public:
            using shared_state_type = SharedStateType;
            using unique_vertex_type = UniqueStateVertexType;
            using next_frame_attrib_list_type = typename unique_vertex_type::VertexAttribList;
            using AttribList = typename unique_vertex_type::AttribList;
            KeyFrame() : m_uniqueState(), m_sharedState(nullptr) {}
            KeyFrame(const unique_vertex_type& unique, shared_state_type* shared = nullptr) :
                m_uniqueState(unique),
                m_sharedState(shared)
            {}
            KeyFrame& operator=(const unique_vertex_type& unique)
            {
                if(&m_uniqueState != &unique)
                    m_uniqueState = unique;
                return *this;
            }
            KeyFrame(unique_vertex_type&& unique, shared_state_type* shared = nullptr) :
                m_uniqueState(std::move(unique)),
                m_sharedState(shared)
            {}
            KeyFrame& operator=(unique_vertex_type&& unique)
            {
                if(&m_uniqueState != &unique)
                    m_uniqueState = std::move(unique);
                return *this;
            }

            /**
             * @brief Sets pointer to shared data.
             *
             * Sets pointer to shared data. User is responsible for ensuring the life time of
             * shared data.
             *
             * @param sharedState Frame shared state.
             */
            void setSharedState(shared_state_type* sharedState)
            {
                m_sharedState = sharedState;
            }

            /**
             * @brief Create attribute mapping for next frame.
             *
             * @param names List of new names for next frame.
             *
             * @return New mapping.
             */
            next_frame_attrib_list_type makeIdentityList(const AttribList& names)
            {
                return m_uniqueState.makeIdentityList(names);
            }

            /**
             * @brief Set name map for attributes when frame is used as next frame.
             *
             * @param nextMap Next frame name map.
             */
            void setNextMap(const next_frame_attrib_list_type& nextMap)
            {

                m_uniqueState.setNextIdentity(nextMap);
            }

            /**
             * @brief Enable frame shared state for drawing.
             *
             * @param program Renderer pointer.
             */
            void enableSharedState(const ShaderProgram& program) const
            {
                if(m_sharedState)
                    m_sharedState->enable(program);
            }

            /**
             * @brief Enable frame unique state.
             */
            void enable(const ShaderProgram& program) const
            {
                m_uniqueState.enable(program);
            }

            /**
             * @brief Enable frame unique state as next frame.
             */
            void enableAsNext(const ShaderProgram& program) const
            {
                m_uniqueState.enableAsNext(program);
            }

            /**
             * @brief Disable shared state.
             *
             * @param program Renderer pointer.
             */
            void disableSharedState(const ShaderProgram& program) const
            {
                if(m_sharedState)
                    m_sharedState->disable(program);
            }

            /**
             * @brief Disable frame unique state.
             */
            void disable(const ShaderProgram& program) const
            {
                m_uniqueState.disable(program);
            }

            /**
             * @brief Disable frame enabled as next frame.
             */
            void disableAsNext(const ShaderProgram& program) const
            {
                m_uniqueState.disableAsNext(program);
            }

        private:
            unique_vertex_type m_uniqueState;
            shared_state_type* m_sharedState;
    };

    template<class UniqueStateVertexType, class SharedStateType>
    using KeyFrameArray = FrameData<KeyFrame<UniqueStateVertexType, SharedStateType>>;

    template<class FrameArrayType>
    class KeyFrameAnimationData : public AnimatedFrameData<FrameArrayType>
    {
        public:
            using base_type = AnimatedFrameData<FrameArrayType>;
            using frame_data_type = typename base_type::frame_data_type;
            using frame_type = typename frame_data_type::frame_type;
            using next_frame_attrib_list_type = typename frame_type::next_frame_attrib_list_type;
            using AttribList = typename frame_type::AttribList;
            using size_type = typename base_type::size_type;
            using shared_state_type = typename frame_type::shared_state_type;

            explicit KeyFrameAnimationData(const size_type framesCount = 24u) : base_type(framesCount)
            {
                setSharedState();
            }

            /**
             * @brief Create new animated frame data by coping frames from rhs.
             *
             * @param rhs frame container to be copied.
             */
            explicit KeyFrameAnimationData(const frame_data_type& rhs) : base_type(rhs)
            {
                setSharedState();
            }

            /**
             * @brief  Create new animated frame data by moving frames from rhs.
             *
             * @param rhs 
             */
            explicit KeyFrameAnimationData(frame_data_type&& rhs) : base_type(std::move(rhs))
            {
                setSharedState();
            }
            
            /**
             * @brief Assign data for animated frame data by coping data from rhs.
             *
             * Assign previously created attrib name map if size matches new attrib size.
             *
             * @param rhs Frame data to be copied.
             *
             * @return *this
             */
            KeyFrameAnimationData& operator=(const frame_data_type& rhs)
            {
                base_type::operator=(rhs);
                setSharedState();
                try{
                    _setNextFrameMap();
                } catch(const KeyFrameAnimationError&)
                {
                }
                return *this;
            }

            /**
             * @brief Assign data for animated frame data by moving data from rhs.
             *
             * @param rhs
             *
             * @return 
             */
            KeyFrameAnimationData& operator=(frame_data_type&& rhs)
            {
                base_type::operator=(std::move(rhs));
                setSharedState();
                try{
                    _setNextFrameMap();
                } catch(const KeyFrameAnimationError&)
                {
                }
                return *this;
            }
            
            /**
             * @brief Set names for next frame attributes.
             *
             * @param nextFrameMap Array of strings.
             */
            void setNextFrameMap(const AttribList& nextFrameMap)
            {
                if(this->m_data.size() > 0)
                {
                    m_nextFrameNames = this->m_data[0].makeIdentityList(nextFrameMap);
                    _setNextFrameMap();
                }
            }

            void setSharedState(const shared_state_type& sharedState)
            {
                m_sharedState = sharedState;
                setSharedState();
            }

            void setSharedState(shared_state_type&& sharedState)
            {
                m_sharedState = std::move(sharedState);
                setSharedState();
            }
        private:
            void _setNextFrameMap()
            {
                for(auto& frame : this->m_data)
                {
                    frame.setNextMap(m_nextFrameNames);
                }
            }
            void setSharedState()
            {
                for(auto& frame : this->m_data)
                {
                    frame.setSharedState(&m_sharedState);
                }
            }

            shared_state_type m_sharedState;
            next_frame_attrib_list_type m_nextFrameNames;
    };

    namespace _keyframe_detail {
        template<class FactorType>
        class FactorWarpper
        {
            using factor_type = FactorType;
            using uniform_type = UniformFloat;
        };
    } /* namespace _keyframe_detail */

    template<class FrameType, class FactorType>
    class KeyFrameAnimationDrawObject
    {
        public:
            using frame_type = FrameType;
            using factor_wrapper = typename _keyframe_detail::FactorWarpper<FactorType>;
            using factor_type = typename factor_wrapper::factor_type;
            using uniform_type = typename factor_wrapper::uniform_type;
            void draw(AexRenderer* gl, const frame_type& first, const frame_type& second, const factor_type& factor)
            {
                aex::ShaderDrw* shader;
                if(gl && (shader = gl->getShader()))
                {
                    const auto& shaderProgram = *shader->GetProgram();
                    first.enableSharedState(shaderProgram);
                    first.enable(shaderProgram);
                    second.enableAsNext(shaderProgram);
                    m_factorUniform.setValue(factor);
                    m_factorUniform.setUniform(*shader);
                }
            }

            void finishdraw(AexRenderer* gl, const frame_type& first, const frame_type& second, const factor_type& factor)
            {
                aex::ShaderDrw* shader;
                if(gl && (shader = gl->getShader()))
                {
                    const auto& shaderProgram = *shader->GetProgram();
                    second.disableAsNext(shaderProgram);
                    first.disable(shaderProgram);
                    first.disableSharedState(shaderProgram);
                }
            }

            void setFactorUniformName(const aex::string& name)
            {
                m_factorUniform.setName(name);
            }

        private:
            uniform_type m_factorUniform;
    };

    template<class VertexDataType, class... Args>
    KeyFrameState makeKeyFrame(const std::vector<VertexDataType>& data, Args&&... args)
    {
        VertexData vd = VertexData::makeVertexData(data, aex::forward<Args>(args)...);
        return KeyFrameState(std::move(vd));
    }

    template<class VertexDataType, class... Args>
    KeyFrameState makeKeyFrame(std::vector<VertexDataType>&& data, Args&&... args)
    {
        VertexData vd = VertexData::makeVertexData(std::move(data), aex::forward<Args>(args)...);
        return KeyFrameState(std::move(vd));
    }

    template<class VertexDataType, class IndexDataType, class... Args>
    SharedKeyFrameState makeSharedKeyFrameTriangles(const std::vector<IndexDataType>& indices, const std::vector<VertexDataType>& data, Args&&... args)
    {
        VertexData vd = VertexData::makeVertexData(data, aex::forward<Args>(args)...);
        IndexData id = IndexData::createTriangles(indices);
        return SharedKeyFrameState(std::move(id), std::move(vd));
    }

    template<class VertexDataType, class IndexDataType, class... Args>
    SharedKeyFrameState makeSharedKeyFrameTriangles(std::vector<IndexDataType>&& indices, std::vector<VertexDataType>&& data, Args&&... args)
    {
        VertexData vd = VertexData::makeVertexData(std::move(data), aex::forward<Args>(args)...);
        IndexData id = IndexData::createTriangles(std::move(indices));
        return SharedKeyFrameState(std::move(id), std::move(vd));
    }

    template<class VertexDataType, class IndexDataType, class... Args>
    SharedKeyFrameState makeSharedKeyFrameTriangleStrip(const std::vector<IndexDataType>& indices, const std::vector<VertexDataType>& data, Args&&... args)
    {
        VertexData vd = VertexData::makeVertexData(data, aex::forward<Args>(args)...);
        IndexData id = IndexData::createTriangleStrip(indices);
        return SharedKeyFrameState(std::move(id), std::move(vd));
    }

    template<class VertexDataType, class IndexDataType, class... Args>
    SharedKeyFrameState makeSharedKeyFrameTriangleStrip(std::vector<IndexDataType>&& indices, std::vector<VertexDataType>&& data, Args&&... args)
    {
        VertexData vd = VertexData::makeVertexData(std::move(data), aex::forward<Args>(args)...);
        IndexData id = IndexData::createTriangleStrip(std::move(indices));
        return SharedKeyFrameState(std::move(id), std::move(vd));
    }

    template<class VertexDataType, class IndexDataType, class... Args>
    SharedKeyFrameState makeSharedKeyFrameLines(const std::vector<IndexDataType>& indices, const std::vector<VertexDataType>& data, Args&&... args)
    {
        VertexData vd = VertexData::makeVertexData(data, aex::forward<Args>(args)...);
        IndexData id = IndexData::createLines(indices);
        return SharedKeyFrameState(std::move(id), std::move(vd));
    }

    template<class VertexDataType, class IndexDataType, class... Args>
    SharedKeyFrameState makeSharedKeyFrameLines(std::vector<IndexDataType>&& indices, std::vector<VertexDataType>&& data, Args&&... args)
    {
        VertexData vd = VertexData::makeVertexData(std::move(data), aex::forward<Args>(args)...);
        IndexData id = IndexData::createLines(std::move(indices));
        return SharedKeyFrameState(std::move(id), std::move(vd));
    }

    template<class VertexDataType, class IndexDataType, class... Args>
    SharedKeyFrameState makeSharedKeyFrameLineStrip(const std::vector<IndexDataType>& indices, const std::vector<VertexDataType>& data, Args&&... args)
    {
        VertexData vd = VertexData::makeVertexData(data, aex::forward<Args>(args)...);
        IndexData id = IndexData::createLineStrip(indices);
        return SharedKeyFrameState(std::move(id), std::move(vd));
    }

    template<class VertexDataType, class IndexDataType, class... Args>
    SharedKeyFrameState makeSharedKeyFrameLineStrip(std::vector<IndexDataType>&& indices, std::vector<VertexDataType>&& data, Args&&... args)
    {
        VertexData vd = VertexData::makeVertexData(std::move(data), aex::forward<Args>(args)...);
        IndexData id = IndexData::createLineStrip(std::move(indices));
        return SharedKeyFrameState(std::move(id), std::move(vd));
    }

    KeyFrameState makeKeyFrame(const std::vector<PositionNormal>& data,
            const aex::string& posName = "gl_Vertex",
            const aex::string& norName = "gl_Normal");

    KeyFrameState makeKeyFrame(std::vector<PositionNormal>&& data,
            const aex::string& posName = "gl_Vertex",
            const aex::string& norName = "gl_Normal");

    template<class IndexDataType>
    SharedKeyFrameState sharedTrianglesAndUVs(const std::vector<IndexDataType>& indices,
            const std::vector<aex::math::Vector2>& data,
            const aex::string& uvName = "gl_TexCoord")
    {
        VertexData vd = VertexData::makeVertexData(data, uvName);
        IndexData id = IndexData::createTriangles(indices);
        return SharedKeyFrameState(std::move(id), std::move(vd));
    }

    template<class IndexDataType>
    SharedKeyFrameState makeSharedTrianglesAndUVs(std::vector<IndexDataType>&& indices,
            std::vector<aex::math::Vector2>&& data,
            const aex::string& uvName = "gl_TexCoord")
    {
        VertexData vd = VertexData::makeVertexData(std::move(data), uvName);
        IndexData id = IndexData::createTriangles(std::move(indices));
        return SharedKeyFrameState(std::move(id), std::move(vd));
    }

    using SimpleKeyFrameArray = KeyFrameArray<KeyFrameState, SharedKeyFrameState>;
    using SimpleKeyFrameAnimationData = KeyFrameAnimationData<SimpleKeyFrameArray>;
    struct AEX_PUBLIC_API SimpleKeyFrameData {
        using unique_vertex_data_type = std::vector<PositionNormal>;
        std::vector<mesh_indices_type> indices;
        std::vector<aex::math::Vector2> uvs;
        std::vector<unique_vertex_data_type> frames;
    };

    AEX_PUBLIC_API SimpleKeyFrameData LoadSimpleKFDataFromAexFile(const aex::string&);
    AEX_PUBLIC_API void SaveSimpleKFDataToAexFile(const aex::string&, const SimpleKeyFrameData&);
    AEX_PUBLIC_API SimpleKeyFrameAnimationData makeAnimationData(const SimpleKeyFrameData&);
    AEX_PUBLIC_API SimpleKeyFrameAnimationData makeAnimationData(SimpleKeyFrameData&&);
} /* namespace aex */

#endif /* KEYFRAMEDATA_HPP */
