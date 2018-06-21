//
//  AexRenderer.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 24.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include <aex/AexRenderer.hpp>
#include <aex/AWindow.hpp>
#include <aex/ShaderDrw.hpp>
#include <aex/Logger.hpp>
#include <aex/ax_Macros.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/VertexData.hpp>
#include <aex/GPUApi.hpp>
#include <cassert>
#include <cmath>

#include "aex/AexRenderTree.hpp"

namespace aex {


    namespace __detail_passId {
        std::atomic<int> _nextPassId(1);
        std::map<std::string, int> m_passNameMap;

        int nextPassId()
        {
            return _nextPassId.fetch_add(1);
        }

        int getPassId(const std::string& name)
        {
            if (m_passNameMap.find(name) != m_passNameMap.end())
            {
                return m_passNameMap[name];
            } else
            {
                int passId = nextPassId();
                m_passNameMap.emplace(name, passId);
                return passId;
            }
        }
    }

    AexRenderer::AexRenderer() : AexRenderer(nullptr)
    {
    }
    AexRenderer::AexRenderer(AWindow* window) :
    m_gpu_normal(true),
    m_frame(0.0f),
    m_VPMatrices{
        {Matrix(), Matrix(), Matrix()}
    }

    ,
    m_quatCamera(),
    m_actualshader(nullptr),
    m_window(window),
    m_BlendEnableState(false),
    m_DepthTestEnableState(true)
    {
        m_clearColor[0] = 0.2f;
        m_clearColor[1] = 0.2f;
        m_clearColor[2] = 0.2f;
        m_clearColor[3] = 1.0f;
    }

    void AexRenderer::setGeometryBatch(bool geometryBatch)
    {
        m_geometryBatch = geometryBatch;
    }

    bool AexRenderer::isGeometryBatch() const
    {
        return m_geometryBatch;
    }

    AexRenderer::~AexRenderer()
    {
        LOG_DEBUG("AexRenderer removed");
    }

    int AexRenderer::getPassId(std::string name)
    {
       return __detail_passId::getPassId(name);
    }

    bool AexRenderer::setBlendState(bool state)
    {


        if (m_BlendEnableState != state)
        {
            //LOG_DEBUG("AexRenderer::setBlendState", m_BlendEnableState != state ," ",state);
            if (state)
            {
                enableBlending();
            } else
            {
                disableBlending();
            }

            m_BlendEnableState = state;
            return true;
        }

        return false;
    }

    void AexRenderer::refreshDrawState()
    {
        if (m_BlendEnableState)
        {
            enableBlending();
        } else
        {
            disableBlending();
        }

        if (m_DepthTestEnableState)
        {
            enableState(DEPTH_TEST);
        } else
        {
            disableState(DEPTH_TEST);
        }

        clearColor(m_clearColor[0],
                m_clearColor[1],
                m_clearColor[2],
                m_clearColor[3]);


    }

    void AexRenderer::setLineWidth(float width)
    {
        if (!aex::math::Math::closeEnough(m_lineWidth, width))
        {
            m_lineWidth = width;
            glLineWidth(width);
        }
    }

    bool AexRenderer::setDepthTestState(bool state)
    {
        if (m_DepthTestEnableState != state)
        {
            if (state)
            {
                enableState(DEPTH_TEST);
            } else
            {
                disableState(DEPTH_TEST);
            }

            m_DepthTestEnableState = state;

            return true;
        }

        return false;
    }

    void AexRenderer::setClearColor(float r, float g, float b, float a)
    {
        clearColor(r, g, b, a);
        m_clearColor[0] = r;
        m_clearColor[1] = g;
        m_clearColor[2] = b;
        m_clearColor[3] = a;
    }

    void AexRenderer::setClearColor(aex::math::Vector3 color, float a)
    {
        clearColor(color.x, color.y, color.z, a);
    }

    void AexRenderer::setQuatCamera(QuaternionCamera* quatCamera)
    {
        m_quatCamera = quatCamera;
        updateListeners();
    }

    QuaternionCamera* AexRenderer::getQuatCamera() const
    {
        return m_quatCamera;
    }

    bool AexRenderer::isDepthTestEnableState() const
    {
        return m_DepthTestEnableState;
    }

    bool AexRenderer::isBlendEnableState() const
    {
        return m_BlendEnableState;
    }

    ShaderDrw* AexRenderer::getShader() const
    {
        return m_actualshader;
    }

    void AexRenderer::setWindow(AWindow* window)
    {
        m_window = window;
    }

    AWindow* AexRenderer::getWindow() const
    {
        return m_window;
    }

    void AexRenderer::setColorPick(bool colorPick)
    {
        m_colorPick = colorPick;
    }

    bool AexRenderer::isColorPick() const
    {
        return m_colorPick;
    }

    void AexRenderer::setShadowMapPass(bool shadowMapPass)
    {
        m_shadowMapPass = shadowMapPass;
    }

    bool AexRenderer::isShadowMapPass() const
    {
        return m_shadowMapPass;
    }

    void AexRenderer::UniformLights()
    {
        if (m_lightsUniArray)
        {
            m_lightsUniArray->setUniform(m_currentShaderProgram);
            size_t size = ((aex::UniformArray*) m_lightsUniArray.get())->size();
            std::unique_ptr<Uniform> numlights(new UniformFloat(size, "numlights"));
            numlights->setUniform(m_currentShaderProgram);
        }
    }

    void
    AexRenderer::AddLights(Uniformptr lights)
    {
        m_lightsUniArray = lights;
        for (auto listener : m_updateListeners)
            listener->onLightsUpdate();
    }

    void
    AexRenderer::AddLights(Uniform* lights)
    {
        AddLights(Uniformptr(lights->copy()));
    }

    void AexRenderer::SetUniforms(Uniform* uniform)
    {
        uniform->setUniform(m_currentShaderProgram);
    }

    void AexRenderer::SetUniforms(Uniforms& uniforms)
    {
        for (auto& u : uniforms)
        {
            u->setUniform(m_currentShaderProgram);
        }
    }

    void AexRenderer::SetUniforms(std::vector<Uniform*>& uniforms)
    {
        for (auto& u : uniforms)
        {
            u->setUniform(m_currentShaderProgram);
        }
    }

    void AexRenderer::SetUniform(Uniform* uniform)
    {
        uniform->setUniform(m_currentShaderProgram);
    }

    Vector
    AexRenderer::project(float objX, float objY, float objZ) const
    {
        return project(objX, objY, objZ, m_VPMatrices[0]);
    };

    Vector
    AexRenderer::project(float objX, float objY, float objZ, const Matrix& view) const
    {
        return project(objX, objY, objZ, view, m_VPMatrices[1]);
    };

    Vector
    AexRenderer::project(float objX, float objY, float objZ, const Matrix& view, const Matrix& projection) const
    {
        return project(objX, objY, objZ, view, projection, CurrentViewport());
    };

    Vector
    AexRenderer::project(float objX, float objY, float objZ, const Matrix& view, const Matrix& projection, const ViewPort& viewport) const
    {
        Vector point = projection.transformPoint(view.transformPoint(Vector(objX, objY, objZ)));
        return Vector(viewport.x + viewport.w * (point.x * 0.5f + 0.5f), viewport.y + viewport.h * (point.y * 0.5f + 0.5f), point.z * 0.5f + 0.5f);
    }

    Vector
    AexRenderer::unProject(float winX, float winY) const
    {
        return unProject(winX, winY, 0);
    }

    Vector
    AexRenderer::unProject(float winX, float winY, float winZ) const
    {
        return unProject(winX, winY, winZ, m_VPMatrices[0]);
    }

    Vector
    AexRenderer::unProject(float winX, float winY, float winZ, const Matrix& modelView) const
    {
        return unProject(winX, winY, winZ, modelView, m_VPMatrices[1]);
    }

    Vector
    AexRenderer::unProject(float winX, float winY, float winZ, const Matrix& modelView, const Matrix& projection) const
    {
        return unProject(winX, winY, winZ, modelView, projection, CurrentViewport());
    }

    Vector
    AexRenderer::unProject(float winX, float winY, float winZ, const Matrix& modelView, const Matrix& projection, const ViewPort& viewport) const
    {
        Vector point;
        point.x = (winX - viewport.x) / viewport.w * 2 - 1;
        point.y = (winY - viewport.y) / viewport.h * 2 - 1;
        point.z = winZ * 2 - 1;
        return (projection * modelView).inverse().transformPoint(point);
    }

    Matrix
    AexRenderer::getViewMatrix()
    {
        return Matrix(m_VPMatrices[0]);
    }

    std::array<float, 4> AexRenderer::getClearColor()
    {
        return m_clearColor;
    }

    void AexRenderer::setViewMatrix(const Matrix& mat)
    {
        setViewMatrix(Matrix(mat));
    }

    void AexRenderer::updateListeners()
    {
        for (auto listener : m_updateListeners)
        {
            listener->onViewProjectionUpdate();
        }
    }

    void AexRenderer::setViewMatrix(Matrix&& mat)
    {
        m_VPMatrices[0] = std::move(mat);
        for (auto listener : m_updateListeners)
            listener->onViewProjectionUpdate();
    }

    Matrix
    AexRenderer::getProjectionMatrix()
    {
        return Matrix(m_VPMatrices[1]);
    }

    void AexRenderer::setProjectionMatrix(const Matrix& mat)
    {
        setProjectionMatrix(Matrix(mat));
    }

    void AexRenderer::setProjectionMatrix(Matrix&& mat)
    {
        m_VPMatrices[1] = std::move(mat);
        for (auto listener : m_updateListeners)
            listener->onViewProjectionUpdate();
    }

    Matrix
    AexRenderer::getViewProjectionMatrix()
    {
        return Matrix(m_VPMatrices[2]);
    }

    void AexRenderer::setViewProjectionMatrix(const Matrix& mat)
    {
        setViewProjectionMatrix(Matrix(mat));
    }

    void AexRenderer::setViewProjectionMatrix(Matrix&& mat)
    {
        m_VPMatrices[2] = std::move(mat);
        for (auto listener : m_updateListeners)
            listener->onViewProjectionUpdate();
    }

    void
    AexRenderer::setAttrib(const VertexData& data)
    {
        m_currentShaderProgram->setVertexData(data);
    }

    void
    AexRenderer::releaseAttrib(const VertexData& data)
    {
        m_currentShaderProgram->releaseVertexData(data);
    }

    void
    AexRenderer::setTriangles(const IndexData& id)
    {
        m_currentShaderProgram->setIndices(id);
    }

    void
    AexRenderer::releaseTriangles(const IndexData& id)
    {
        UNUSED(id);
    }

    void
    AexRenderer::setShader(aex::ShaderDrw* shaderProgram)
    {
        if (shaderProgram)
        {
            m_shaderProgramStack.push(shaderProgram);
            m_actualshader = shaderProgram;
            m_currentShaderProgram = shaderProgram->GetProgram();
        } else
        {
            if (!m_shaderProgramStack.empty())
                m_shaderProgramStack.pop();
            else
                LOG_ERR("Warn: trying pop shader from empty shaderstack");

            if (!m_shaderProgramStack.empty())
            {
                m_actualshader = m_shaderProgramStack.top();
                m_currentShaderProgram = m_actualshader->GetProgram();
                m_currentShaderProgram->enable();
            } else
            {
                m_actualshader = nullptr;
                m_currentShaderProgram.reset();
            }
        }
    }

    bool AexRenderer::InitAPI(AWindow* window, uint32_t enables, uint32_t disables)
    {
        return m_api.initAPI(window, enables, disables);
    }

    void AexRenderer::FinishAPI()
    {
        m_api.finishAPI();
    }

    void AexRenderer::BeginFrame()
    {
        m_api.beginFrame();
    }

    void AexRenderer::BeginFrame(uint32_t buffers)
    {
        m_api.beginFrame(buffers);
    }

    void AexRenderer::EndFrame()
    {
        m_api.endFrame();
    }

    void AexRenderer::pushCurrentPassID(int currentPassID)
    {
        m_passIDStack.push(currentPassID);
    }

    int AexRenderer::getCurrentPassID() const
    {
        return m_passIDStack.top();
    }

    void AexRenderer::popCurrentPassID()
    {
        m_passIDStack.pop();
    }

    IOnAexRendererUpdate::IOnAexRendererUpdate() : IOnAexRendererUpdate(nullptr)
    {
    }

    IOnAexRendererUpdate::IOnAexRendererUpdate(AexRenderer* gl) :
    m_gl(nullptr)
    {
        *this = gl;
    }

    IOnAexRendererUpdate::IOnAexRendererUpdate(const IOnAexRendererUpdate& rhs) :
    IOnAexRendererUpdate()
    {
        *this = rhs;
    }

    IOnAexRendererUpdate&
            IOnAexRendererUpdate::operator=(const IOnAexRendererUpdate& rhs)
    {
        if (this != &rhs)
        {
            *this = rhs.m_gl;
        }
        return *this;
    }

    IOnAexRendererUpdate&
            IOnAexRendererUpdate::operator=(AexRenderer* gl)
    {
        reset(gl);
        return *this;
    }

    void
    IOnAexRendererUpdate::setAexRenderer(AexRenderer* gl)
    {
        reset(gl);
    }

    void
    IOnAexRendererUpdate::reset(AexRenderer* gl)
    {

        if (m_gl == gl)
            return;

        if (m_gl)
        {
            m_gl->m_updateListeners.remove(this);
        }

        m_gl = gl;

        if (m_gl)
        {
            m_gl->m_updateListeners.push_back(this);
        }
    }

    IOnAexRendererUpdate::~IOnAexRendererUpdate()
    {
        reset();
    }

    AexRenderer*
    IOnAexRendererUpdate::gl() const
    {
        return m_gl;
    }

    bool operator==(const IOnAexRendererUpdate& lhs, AexRenderer* gl)
    {
        return lhs.m_gl == gl;
    }

    bool operator!=(const IOnAexRendererUpdate& lhs, AexRenderer* gl)
    {
        return !(lhs == gl);
    }
}
