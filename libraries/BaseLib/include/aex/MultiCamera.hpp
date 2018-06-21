/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MultiCamera.hpp
 * Author: rafa
 *
 * Created on April 12, 2016, 4:38 PM
 */

#ifndef MULTICAMERA_HPP
#define MULTICAMERA_HPP

#include <aex/QuaternionCamera.hpp>
#include <aex/Vector2.hpp>
#include <aex/Aex.hpp>
#include <aex/FrameBuffer.hpp>

namespace aex{
    
class AEX_PUBLIC_API MultiCameraFov {
    public:

        MultiCameraFov();
        MultiCameraFov(int x, int y, float angleX, float angleY);


    private:
        aex::math::Vector2 m_fovAngle;
        aex::math::Vector2 m_subFovAngle;
        aex::math::Vector2 m_offsetAngle;
        aex::math::Vector2 m_offsetDistance;
        aex::Aex m_aex;

    };

    class AEX_PUBLIC_API EnvironmentMap : public DrawObject {
    public:

        EnvironmentMap();
        EnvironmentMap(int tex_size, aex::math::Vector3 position, bool multiFbo = false);
        void draw(AexRenderer* gl);
        void firstStage(AexRenderer* gl);
        bool cameraUpdate(AexRenderer* gl);
        void finishdraw(AexRenderer* gl);
        void finishfbo(AexRenderer* gl);

        void needsUpdate() {
        }
        void init();
        void setPosition(aex::math::Vector3 pos);
        QuaternionCamera& getCamera();
        Texture* getTexture();
        void setStatic(bool Static);
        bool isStatic() const;
        void setSampler(UniformSampler* sampler);
        void saveFrame(/*const std::string& filename*/ );
        UniformSampler* getSampler() const;
    protected:

    private:
        void onContextDirty();
        static const std::vector<Vector3> m_directions;
        static const std::vector<Vector3> m_upVector;
        int m_cameraSide = 0;
        //int cameraSide = 0;
        int m_resolution = 0;
        bool m_isStatic = false;
        bool m_firstPass = true;
        QuaternionCamera m_camera;
        QuaternionCamera* m_oldCamera;
        FrameBufferOGL m_fbo;
        std::vector<aex::unique_ptr<FrameBufferOGL>> m_fbos;
        std::array<FrameBufferOGL*, 6> m_fbosPick;
        aex::math::Vector3 m_position;
        UniformSampler* m_sampler = nullptr;
        bool m_multiFbo;
        int frameocounter=0; 
        AexRenderer* m_gl= nullptr;
        std::array< std::vector<uint32_t>, 6> data_buffer; 
    };
}

#endif /* MULTICAMERA_HPP */

