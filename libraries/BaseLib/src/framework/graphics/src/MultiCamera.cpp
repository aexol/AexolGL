/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <aex/MultiCamera.hpp>
#include <aex/SavePng.hpp>

namespace aex {

    MultiCameraFov::MultiCameraFov() {

    }

    MultiCameraFov::MultiCameraFov(int x, int y, float angleX, float angleY) {

    }

    const std::vector<Vector3> EnvironmentMap::m_directions =

            std::vector<aex::math::Vector3>({aex::math::Vector3(1.0f, 0.0f, 0.0f),
        aex::math::Vector3(-1.0f, 0.0f, 0.0f),

        aex::math::Vector3(0.0f, 1.0f, 0.0f),
        aex::math::Vector3(0.0f, -1.0f, 0.0f),

        aex::math::Vector3(0.0f, 0.0f, 1.0f),
        aex::math::Vector3(0.0f, 0.0f, -1.0f)});


    const std::vector<Vector3> EnvironmentMap::m_upVector =
            std::vector<aex::math::Vector3>(
        {aex::math::Vector3(0.0f, -1.0f, 0.0f),
        aex::math::Vector3(0.0f, -1.0f, 0.0f),

        aex::math::Vector3(0.0f, 0.0f, 1.0f),
        aex::math::Vector3(0.0f, 0.0f, -1.0f),

        aex::math::Vector3(0.0f, -1.0f, 0.0f),
        aex::math::Vector3(0.0f, -1.0f, 0.0f)});

    EnvironmentMap::EnvironmentMap() {
        init();
    }

    EnvironmentMap::EnvironmentMap(int tex_size, aex::math::Vector3 position, bool multiFbo) :
    m_resolution(tex_size),
    m_fbo(tex_size, tex_size, false, !multiFbo),
    m_position(position),
    m_multiFbo(multiFbo){
        LOG_DEBUG("m_position(position): ", position);
        init();
        
        if(multiFbo)
        {
            m_fbos.emplace_back(new FrameBufferOGL(tex_size, tex_size, false, false));
            m_fbos.emplace_back(new FrameBufferOGL(tex_size, tex_size, false, false));
            m_fbos.emplace_back(new FrameBufferOGL(tex_size, tex_size, false, false));
            m_fbos.emplace_back(new FrameBufferOGL(tex_size, tex_size, false, false));
            m_fbos.emplace_back(new FrameBufferOGL(tex_size, tex_size, false, false));

            m_fbosPick[0]=&m_fbo;
            m_fbosPick[1]=m_fbos[0].get();
            m_fbosPick[2]=m_fbos[1].get();
            m_fbosPick[3]=m_fbos[2].get();
            m_fbosPick[4]=m_fbos[3].get();
            m_fbosPick[5]=m_fbos[4].get();
        }
        
        
    }

    void EnvironmentMap::init() {
        m_camera.perspective(90.0f, (float) m_resolution, (float) m_resolution, 1.0f, 5000.0f);
    }

    QuaternionCamera& EnvironmentMap::getCamera() {
        return m_camera;
    }

    void EnvironmentMap::setPosition(aex::math::Vector3 pos) {
        m_position = pos;
    }

    void EnvironmentMap::draw(AexRenderer* gl) {
        firstStage(gl);
        cameraUpdate(gl);
    }
    
    void EnvironmentMap::firstStage(AexRenderer* gl)
    {
        if (m_firstPass || gl->isColorPick()) {
            return;
        }
        m_oldCamera = gl->getQuatCamera();
        LOG_DEBUG("firstStage: ", m_cameraSide);
        //m_fbo.getTexture()->unbind();
    }

    bool EnvironmentMap::cameraUpdate(AexRenderer* gl) {
        m_gl= gl;
        if (m_firstPass || gl->isColorPick()) {
            return true;
        }
        
        if(m_multiFbo && m_cameraSide > 0)
        {
            m_fbosPick[m_cameraSide-1]->finishdraw(gl);
        }

        m_camera.lookAt(m_position, m_position + m_directions[m_cameraSide], m_upVector[m_cameraSide]);
        //m_camera.rotate(aex::math::Vector3(1.0,0.0,0.0));

        //LOG_DEBUG(m_position, m_position + m_directions[m_cameraSide]);

        gl->setQuatCamera(&m_camera);
        gl->setViewMatrix(m_camera.get_view_matrix());
        gl->setViewProjectionMatrix(m_camera.transforms());
        
        ShaderDrw* shader = gl->getShader();
        if(shader)
        {
            shader->updateVPMatix(gl);
        }

        if(!m_multiFbo)
        {
            m_fbo.setCubeSide(m_cameraSide);
            m_fbo.draw(gl);
        }
        else
        {
            m_fbosPick[m_cameraSide]->draw(gl);
        }
        LOG_DEBUG("++m_cameraSide: ", m_cameraSide);
        //cameraSide = m_cameraSide;
        ++m_cameraSide;
      
        if (m_cameraSide < 6) {
            return false;
        } else {
            m_cameraSide = 0;
            return true;
        }
    }
    
    std::string getFrameFolder(int i )
    {
        switch (i) {
            case 0:
                return "Xpos";
            case 1:
                return "Xneg";
            case 2:
                return "Ypos";
            case 3:
                return "Yneg";
            case 4:
                return "Zpos";
            case 5:
                return "Zneg";
                
            default:
                return "Xpos";
        }

    }
    
    void EnvironmentMap::saveFrame(/*const std::string& filename*/ )
    {   
        
        //int cameraSide = m_cameraSide;
        
        
        
        if(data_buffer[0].size() == 0)
        {
            int size = m_resolution*m_resolution;
            for (int cameraSide = 0; cameraSide < 6; ++cameraSide)
            {
                data_buffer[cameraSide].resize(size, 100);
            }
        }
        

           if (!m_multiFbo)
           {
                m_fbo.getTexture()->bind();
           }
        
        for (int cameraSide = 0; cameraSide < 6; ++cameraSide)
        {
           
            if (!m_multiFbo)
            {
                m_fbo.getTexture()->getData(data_buffer[cameraSide].data(), cameraSide);
        
            } else
            {
                //m_fbosPick[cameraSide]->finishdraw(m_gl);
                m_fbosPick[cameraSide]->getTexture()->getData(data_buffer[cameraSide].data(), cameraSide);
                //m_fbosPick[cameraSide]->draw(m_gl);
            }
            LOG_DEBUG("savePngFile: ", frameocounter, " ", cameraSide);
            
        
            /*savePngFile("photos//"+getFrameFolder(cameraSide)+"//"+aex::to_string(frameocounter) + "_" + aex::to_string(cameraSide) + ".png",
                    data_buffer[cameraSide].data(), m_resolution, m_resolution);*/
                    //data_buffer[cameraSide].clear();
        }
        
        if (!m_multiFbo)
        {
            m_fbo.getTexture()->unbind();
        }

    }
    
    
    void EnvironmentMap::finishfbo(AexRenderer* gl) {
        
        if (m_firstPass || gl->isColorPick()) {
            return;
        }
        
        if(!m_multiFbo)
        {
            m_fbo.finishdraw(gl);
        }
        else
        {
            m_fbosPick[5]->finishdraw(gl);
        }
        
        gl->setQuatCamera(m_oldCamera);
        gl->setViewMatrix(m_oldCamera->get_view_matrix());
        gl->setViewProjectionMatrix(m_oldCamera->transforms());
        ShaderDrw* shader = gl->getShader();
        if(shader)
        {
            shader->updateVPMatix(gl);
        }
        
        m_fbo.getTexture()->bind();
        
        if (m_sampler) {
            m_sampler->setValue(m_fbo.getTexture());
        }
        
    }

    void EnvironmentMap::finishdraw(AexRenderer* gl) {
      
        if (m_firstPass || gl->isColorPick()) {
            m_firstPass = false;
            return;
        }
       frameocounter++;
        
        
    }

    void EnvironmentMap::onContextDirty() {

    }

    Texture* EnvironmentMap::getTexture() {
        return m_fbo.getTexture();
    }

    void EnvironmentMap::setStatic(bool Static) {

        m_isStatic = Static;

        if (!m_isStatic) {

        }

    }

    bool EnvironmentMap::isStatic() const {
        return m_isStatic;
    }

    void EnvironmentMap::setSampler(UniformSampler* sampler) {
        m_sampler = sampler;
        m_sampler->setValue(m_fbo.getTexture());
    }

    UniformSampler* EnvironmentMap::getSampler() const {
        return m_sampler;
    }

}