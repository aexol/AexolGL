/*
 * File:   Animation.hpp
 * Author: mritke
 *
 * Created on November 22, 2013, 2:00 PM
 */

#ifndef ANIMATION_H
#define	ANIMATION_H

#include <aex/Common.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/Mesh.hpp>
#include <aex/Timer.hpp>
#include <aex/UniversalManager.hpp>
#include <aex/VertexData.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <atomic>

namespace aex
{
    /* Forward declarations */
    class VertexData;
    class SimulationVisitor;
    class SimulatedElement;
    class TimedElement;
    class IAnimationVisitor;
    class KFAnimatedMesh;

    /* Interfaces */

    class AEX_PUBLIC_API IAnimatedMesh : public DrawObject
    {
        public:
            virtual void accept(IAnimationVisitor&) = 0;
            virtual ~IAnimatedMesh(){}
    };

    class AEX_PUBLIC_API IAnimationVisitor
    {
        public:
            virtual void visit(KFAnimatedMesh& mesh) = 0;
            virtual ~IAnimationVisitor() = 0;
    };
    inline IAnimationVisitor::~IAnimationVisitor(){}

    class AEX_PUBLIC_API SimulationVisitor
    {
        public:
            virtual void visit(SimpleSimulation&) = 0;
            virtual ~SimulationVisitor() = 0;
    }
    inline SimulationVisitor::~SimulationVisitor(){}


    class AEX_PUBLIC_API SimulatedElement
    {
        public:
            virtual void accept(const SimulationVisitor&) = 0;
            virtual ~SimulatedElement() = 0;
    }
    inline SimulatedElement::~SimulatedElement(){}

    class AEX_PUBLIC_API IAnimation : public SimulatedElement
    {
        public:
            virtual void Update(aexTimeUnit time) = 0;
            virtual void StartAnimation(aexTimeUnit time) = 0;
            virtual void EndAnimation() = 0;
            virtual void PauseAnimation() = 0;
            virtual ~IAnimation(){}
    };

    class AEX_PUBLIC_API KeyframeVertex
    {
    private:
        aexTimeUnit                     m_startTime;
        aexTimeUnit                     m_endTime;
        aexTimeUnit                     m_durationTime;
        Matrix                          m_Tmatrix;
        VertexData                      m_frame;
        VertexData                      m_frameNormals;
        std::vector<mesh_vectors_type>  m_frameData;
        std::vector<mesh_vectors_type>  m_frameNormalsData;

    public:
        KeyframeVertex() = default;
        KeyframeVertex(aexTimeUnit startT, aexTimeUnit endT, const vector<mesh_vectors_type>& frame);
        KeyframeVertex(aexTimeUnit startT, aexTimeUnit endT, std::vector<mesh_vectors_type>&& frame);
        KeyframeVertex(aexTimeUnit startT, aexTimeUnit endT, std::vector<mesh_vectors_type>&& frame, std::vector<mesh_vectors_type>&& frameNormals);
        ~KeyframeVertex();
        void SetFrame(const vector<mesh_vectors_type>& vert);
        void SetFrameNormals(const vector<mesh_vectors_type>& vertNormals);
        VertexData& GetFrame();
        VertexData& GetNormals();
        const VertexData& GetFrame() const;
        const VertexData& GetNormals() const;
        const std::vector<mesh_vectors_type>& GetFrameData() const;
        aexTimeUnit GetEndTime()
        {
            return m_endTime;
        };
        aexTimeUnit      GetStartTime()
        {
            return m_startTime;
        };
        aexTimeUnit      GetDurationTime()
        {
            return m_durationTime;
        };
    };


    class AEX_PUBLIC_API KFAnimatedMesh : public IAnimatedMesh
    {
        public:
            KFAnimatedMesh() = default;
            void draw(AexRenderer* gl);
            void finishdraw(AexRenderer* gl);
            void needsUpdate();
            /*
             * @brief Set position buffer.
             */
            void setPosition(const VertexData& position);

            /*
             * @brief Set normals buffer.
             */
            void setNormals(const VertexData& normals);

            /*
             * @brief Set uv coordinates buffer.
             */
            void setUVs(const VertexData& uvs);

            /*
             * @brief Set positions in next frame.
             */
            void setNextFrame(const VertexData& nextFrame);

            /*
             * @brief Set index buffer.
             */
            void setIndices(const IndexData& indices);

            /*
             * @brief Set animated mesh name.
             */
            void setName(const aex::string& name);

            /*
             * @brief Set factor between frames.
             */
            void SetTweenFactor(float tween);
            operator bool();
        private:
            VertexData      m_vertexVbo;
            VertexData      m_normalsVbo;
            VertexData      m_nextFrameVbo;
            VertexData      m_uvVbo;
            IndexData       m_indexVbo;
            UniformFloat    m_tweenFactorUniform;
            float           m_TweenFactor;
            aex::string     m_name;
    };

    class AEX_PUBLIC_API AnimationState
    {
        public:
            AnimationState() = default;
        private:
            aexTimeUnit                     m_currentTime;
            aexTimeUnit                     m_lastFrameTime;
            aexTimeUnit                     m_animationTime;
            aexTimeUnit                     m_startTime;
            aexTimeUnit                     m_endTime;
            aexTimeUnit                     m_durationTime;
            aexTimeUnit                     m_uniformFrameDuration;
            aexTimeUnit                     m_framesPerSecond;
            size_t                          m_animationSize;

            Matrix                          m_tweenMatrix;
            float                           m_tweenFactor;
    }

    class AEX_PUBLIC_API KFAnimation : public IAnimation
    {
    public:
        KFAnimation() = default;
        struct FrameData
        {
            std::vector<KeyframeVertex>     m_frames;
        };

        AnimationDrw();
        AnimationDrw(const aex::string& animName);

        void LoadFramesFromAexMesh(const aex::string& file);

        void Update(aexTimeUnit time);
        void StartAnimation(aexTimeUnit time);
        void EndAnimation();
        void PauseAnimation();
        void UpdateMesh(IAnimatedMesh& mesh);

        ////////////////SetsyGetsy
        void SetStartTime(aexTimeUnit startTime)
        {
            m_startTime = startTime;
        }

        aexTimeUnit GetStartTime() const
        {
            return m_startTime;
        }

        void SetCurrentTime(aexTimeUnit CurrentTime);
        aexTimeUnit GetCurrentTime() const;

        void SetCurrentFrame(int currentFrame);
        int GetCurrentFrame() const;
        void SetFramesUniformTime(bool framesUniformTime);
        bool IsFramesUniformTime() const;
        void SetRewind(bool rewind);
        bool IsRewind() const;
        float GetTweenFactor() const;
        void SetFramesPerSecond(uint16_t framesPerSecond);
        uint16_t GetFramesPerSecond() const;
        void SetUniformFrameDuration(aexTimeUnit uniformFrameDuration);
        aexTimeUnit GetUniformFrameDuration() const;
        void SetDurationTime(aexTimeUnit durationTime);
        aexTimeUnit GetDurationTime() const;
        void SetEndTime(aexTimeUnit endTime);
        aexTimeUnit GetEndTime() const;
        size_t GetAnimationSize() const
        {
            return m_animationSize;
        }
        void SetAnimationSize(size_t animationSize);
        const std::vector<KeyframeVertex>& GetFrames() const
        {
            return m_frames->m_frames;
        }
        std::vector<KeyframeVertex>& GetFrames()
        {
            return m_frames->m_frames;
        }
        aex::string& getAnimName()
        {
            return m_animName;
        }
        const aex::string& getAnimName() const
        {
            return m_animName;
        }
        bool isLoaded() const
        {
            return m_loaded;
        }
        void setBindPose(const aex::string& name);
        void setFrameData(const aex::string& name);
        void buildPerFacePerVertexNormals();
        void addReversedFaces();
        void addBarycentricCoordinates();
        
        
    private:
        struct FrameDrawable
        {
                operator bool();
                VertexData      m_vertexVbo;
                VertexData      m_normalsVbo;
                VertexData      m_nextFrameVbo;
                VertexData      m_uvVbo;
                IndexData       m_indexVbo;
                float           m_TweenFactor;
                aex::string     m_name;
        };
        FrameDrawable                   m_animatedMesh;
        aex::string                     m_animName;
        int                             m_lastMeshFrame;

        bool                            m_rewind;
        bool                            m_framesUniformTime;
        bool                            m_paused;
        bool                            m_loaded;

        int                             m_currentFrame;

        Mesh*                           m_bindPose;
        typename Manager<Mesh>::uh_type_ptr m_bindPoseHandle;
        FrameData*                      m_frames;
        typename Manager<FrameData>::uh_type_ptr m_frameDataHandle;
        static std::atomic<uint32_t>    NAME_COUNTER;

        float CalcTweenFactor();
        void FindCurrentFrame(aexTimeUnit time);
        void FindCurrentFrame();
        void ChangeMeshFrame();
        void SetAnimatedMesh();
        
    };

    class AEX_PUBLIC_API SimpleSimulation : public IAnimationVisitor
    {
        public:
            void setCurrentTime(aexTimeUnit cTime);
        private:
    };

    class AEX_PUBLIC_API CacheClock : public SimulationVisitor
    {
        public:
            /*
             * @brief Update clock current time.
             */
            update();
            /*
             * @brief Set current time in simulation.
             */
            void visit(SimpleSimulation& sim);
    };

    inline void
    AnimationDrw::SetAnimationSize(size_t animationSize)
    {
        m_animationSize = animationSize;

        if(m_durationTime != 0.0f && m_animationSize != 0.0f && m_framesUniformTime)
        {
            m_framesPerSecond = (double)m_animationSize / m_durationTime;
            m_uniformFrameDuration = 1.0f / m_framesPerSecond;
        }
        else
        {
            m_framesPerSecond = 1.0f;
            m_uniformFrameDuration = 1.0f;
        }
    }

    inline void AnimationDrw::SetDurationTime(aexTimeUnit durationTime)
    {
        m_durationTime = durationTime;

        if(m_durationTime != 0.0f && m_animationSize != 0.0f && m_framesUniformTime)
        {
            m_framesPerSecond = (double)m_animationSize / m_durationTime;
            m_uniformFrameDuration = 1.0f / m_framesPerSecond;
        }
        else
        {
            m_framesPerSecond = 1.0f;
            m_uniformFrameDuration = 1.0f;
        }
    }
}


#endif	/* ANIMATION_H */

