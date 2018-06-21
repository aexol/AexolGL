/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VRCamera.hpp
 * Author: rafa
 *
 * Created on July 11, 2016, 10:30 AM
 */

#ifndef VRCAMERA_HPP
#define VRCAMERA_HPP

#include <aex/QuaternionCamera.hpp>
#include <aex/Vector2.hpp>
#include <aex/Aex.hpp>
#include <aex/FrameBuffer.hpp>
#include <aex/PostEffect.hpp>

#include "ObjectRenderNode.hpp"

namespace aex {

    Matrix getOffsetView(Matrix view,  Vector3 xAxis,  Vector3 yAxis,  Vector3 zAxis,  Vector3 position);
    
    class AEX_PUBLIC_API VRCamera : public QuaternionCamera {

    public:
        
        using QuaternionCamera::QuaternionCamera;
        
        Matrix transforms();
        
        /**
         *	@brief	Get view matrix
         *
         *	@return	Matrix
         */
        Matrix get_view_matrix();
        
         /**
         *	@brief	Get ViewProjection matrix
         *
         *	@return	Matrix
         */
        Matrix get_view_projection_matrix();

         
        
        Vector3 GetPosition();
        void setEye(bool eye);
        void toggleEye();
        void setOffset(float offset);
        float getOffset() const;
        
    private:

        float m_offset=0.0f;
        bool m_currentEye=false;
        
        Matrix m_OffsetView;
        Matrix m_OffsetViewProjection;
                
    };
    
    
    class DrawHMD : public DrawObject 
    {

        
    public:
        DrawHMD();
        DrawHMD(int sizeX, int sizeY);
        
        DrawHMD& operator=(aex::DrawHMD& b);
        DrawHMD& operator=(aex::DrawHMD&& b);
        
        void draw(AexRenderer* gl);
        void finishdraw(AexRenderer* gl);
        void needsUpdate();
        void setTextures(Texture* left, Texture* right);
        
    private:
        std::array<aex::unique_ptr<HudElement>,2> m_huds;
        aex::unique_ptr<ShaderDrw> m_shader;
    protected:        

    };
    
    std::vector<float> makeBarrel(int gridSize, float radius);
    
    class AEX_PUBLIC_API HMDRender : public DrawObject 
    {

       
    public:
        HMDRender();
        HMDRender(int sizeX, int sizeY);
        
        bool init();
        
        void drawCurrentEye(AexRenderer* gl);
        void finishDrawCurrentEye(AexRenderer* gl);
        void drawSelf(AexRenderer* gl);
        
        void draw(AexRenderer* gl);
        void finishdraw(AexRenderer* gl);

        void needsUpdate();
        void toggleEye();
        void combineTextures();
        void getHMDdraw();
        VRCamera* getCameraPointer();
        void setVrCamera(VRCamera* vrCamera);
        VRCamera* getVrCamera() const;
        
    private:
        std::array<FrameBufferOGL*,2> m_viewport;
        VRCamera* m_vrCamera;
        bool m_eye = false; 
        DrawHMD m_selfdraw;
        
    protected:        
    };

}

#endif /* VRCAMERA_HPP */

