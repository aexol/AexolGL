#ifndef _AEX_TAEX_IPP
#define _AEX_TAEX_IPP
#include <aex/Box.hpp>
#include <aex/GPUApi.hpp>
#include <aex/ASceneNode.hpp>
#include <aex/Aex.hpp>
#include <aex/AWindow.hpp>
#include <aex/Instance.hpp>
#include <aex/AexRenderer.hpp>
#include <aex/ShaderDrw.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/Uniforms.hpp>
#include <aex/Logger.hpp>
#include <aex/Exceptions.hpp>
#include <string>
namespace aex {

    template<class T>
    TAex<T>::AexRendererUpdateListener::AexRendererUpdateListener(TAex* self) :
    parent(self) {
        assert(self != nullptr);
    }

    template<class T>
    typename TAex<T>::AexRendererUpdateListener&
    TAex<T>::AexRendererUpdateListener::operator=(const IOnAexRendererUpdate& rhs) {
        IOnAexRendererUpdate::operator=(rhs);
        return *this;
    }

    template<class T>
    typename TAex<T>::AexRendererUpdateListener&
    TAex<T>::AexRendererUpdateListener::operator=(const AexRendererUpdateListener& rhs) {
        IOnAexRendererUpdate::operator=(rhs);
        return *this;
    }

    template<class T>
    void TAex<T>::AexRendererUpdateListener::onViewProjectionUpdate() {
        parent->m_refreshViewProjection = true;
    }

    template<class T>
    void TAex<T>::AexRendererUpdateListener::onLightsUpdate() {


    }

    template<class T>
    template<class... Args>
    TAex<T>::TAex(Args&&... args) :
    T(aex::forward<Args>(args)...),
    m_aexId(__detail_taex::nextId()),
    m_name("Node" + aex::to_string(m_aexId)),
    m_colorID(Vector3::intIDtoVectorID(m_aexId)),
    m_ModelMatrixUniform("_gl_ModelMatrix"),
    m_normalMatrixUniform("normalMatrix"),
    m_preComputeMVPUniform("_gl_ModelViewProjectionMatrix"),
    m_colorPickUniform(m_colorID, "colorPick"),
    m_updateListener(this) {
    }

    template<class T>
    TAex<T>::TAex() : TAex((ASceneNode*) nullptr) {
    }

    template<class T>
    TAex<T>::~TAex() {
        for (auto instance : m_instances)
            instance->reset();
    }

    template<class T>
    void TAex<T>::addUniform(Uniform_ptr& newUniform) {
        m_uniforms.push_back(newUniform);
    }

    template<class T>
    void TAex<T>::addUniform(Uniform* newUniform) {
        m_uniformsRaw.push_back(newUniform);
    }

    template<class T>
    void TAex<T>::removeUniform(Uniform* remove) {
        for (UniformsRaw::iterator iter = m_uniformsRaw.begin(); iter != m_uniformsRaw.end(); iter++) {
            if ((*iter) == remove) {
                m_uniformsRaw.erase(iter);
                break;
            }
        }
    }

    template<class T>
    void
    TAex<T>::setUniforms(AexRenderer* gl) {
        gl->SetUniforms(m_uniforms);
    }

    template<class T>
    void
    TAex<T>::set_lights(AexRenderer* gl, aex::shared_ptr<aex::Uniform> lights) {
        Uniforms uniforms;
        uniforms.push_back(lights);
        gl->SetUniforms(uniforms);
    }

    template<class T>
    void
    TAex<T>::set_lights(AexRenderer* gl, aex::Uniform* lights) {
        Uniforms uniforms;
        uniforms.push_back(aex::shared_ptr<Uniform>(lights->copy()));
        gl->SetUniforms(uniforms);
    }
    
    template<class T>
    void
    TAex<T>::updateDrawUniforms(AexRenderer* gl , const ShaderDrw* shader)
    {
        bool modelUpdated = false;

        if (T::transformNeedsUpdate() || T::finalNeedsUpdate() || T::drawNeedsUpdate()) {
            const Matrix& modelMatrix = T::getFinalModelMatrix();
            m_ModelMatrixUniform.setValue(modelMatrix);
            T::drawUpdated();
            modelUpdated = true;
        }

        if (!shader) {
            throw RenderHierarchyCorrupted("Wrong Rendering branch: No Shader set!!");
            return;
        }


        if (shader && (shader->isNeeds_MVP() || shader->isNeedsNormalMatrix()) && (m_refreshViewProjection || modelUpdated)) //if you need view or pshadowrojection view matix and model changed
        {
            if (m_updateListener != gl) {
                m_updateListener.setAexRenderer(gl);
            }

            if (shader->isNeeds_MVP()) {
                m_preComputeMVP = gl->getViewProjectionMatrix();
                m_preComputeMVP = m_preComputeMVP * Matrix(m_ModelMatrixUniform.getValueA());
                m_preComputeMVPUniform.setValue(m_preComputeMVP);
            }

            if (shader->isNeedsNormalMatrix()) {

                m_normalMatrixUniform.setValue(aex::math::normalMatrix(Matrix(m_ModelMatrixUniform.getValueA()), gl->getViewMatrix()));
                //m_normalMatrixUniform.setValue(Matrix(m_ModelMatrixUniform.getValueA()).inverse().transpose());
            }

            m_refreshViewProjection = false;
        }


        if (m_isTransparent) {
            m_RevertBlend = gl->setBlendState(true);
        }

        if (shader) {
            const auto& program = shader->GetProgram();
            m_ModelMatrixUniform.setUniform(program);

            if (shader->isNeedsNormalMatrix()) {
                m_normalMatrixUniform.setUniform(program);
            }

            if (shader->isNeeds_MVP()) {
                m_preComputeMVPUniform.setUniform(program);
            }

            if (gl->isColorPick()) {
                m_colorPickUniform.setUniform(program);
            }

            if (!m_uniforms.empty()) {
                for (size_t u = 0; u < m_uniforms.size(); ++u) {
                    m_uniforms[u]->setUniform(program);
                }
            }

            if (!m_uniformsRaw.empty()) {
                for (size_t u = 0; u < m_uniformsRaw.size(); ++u) {
                    m_uniformsRaw[u]->setUniform(program);
                }
            }

        }
    }

     template<class T>
    void TAex<T>::draw(AexRenderer* gl) {

        const ShaderDrw* shader = gl->getShader();
        updateDrawUniforms(gl, shader);
        
        if (shader) {
            const auto& program = shader->GetProgram();
            program->draw();
        }
            //LOG_DEBUG("program->draw();");
        
     }
    
    template<class T>
    void TAex<T>::finishdraw(AexRenderer* gl) {
        if (m_RevertBlend) {
            gl->setBlendState(!m_isTransparent);
            m_RevertBlend = false;
        }
    }

    template<class T>
    void TAex<T>::update() {
    }

    template<class T>
    void TAex<T>::needsUpdate() {
    }

    template<class T>
    AABB TAex<T>::getBounds() {
        return AABB();
    }

    template<class T>
    AABB TAex<T>::getBounds(AABBBuilder) {
        return AABB();
    }

    template<class T>
    AABB TAex<T>::getChildBounds() {
        return AABB();
    }

    template<class T>
    void TAex<T>::setUniforms(Uniforms uniforms) {
        m_uniforms = uniforms;
    }

    template<class T>
    Uniforms& TAex<T>::getUniforms() {
        return m_uniforms;
    }

    template<class T>
    uint32_t TAex<T>::getTAexId() const {
        return m_aexId;
    }

    template<class T>
    void TAex<T>::setTransparent(bool Transparent) {
        m_isTransparent = Transparent;
    }

    template<class T>
    bool TAex<T>::isTransparent() const {
        return m_isTransparent;
    }

    template<class T>
    void TAex<T>::setPreComputedMVP(bool preComputedMVP) {
        m_preComputedMVP = preComputedMVP;
    }

    template<class T>
    bool TAex<T>::isPreComputedMVP() const {
        return m_preComputedMVP;
    }

    template<class T>
    void TAex<T>::setNodeName(std::string nodeName) {
        m_nodeName = nodeName;
    }

    template<class T>
    std::string TAex<T>::getNodeName() const {
        return m_nodeName;
    }

    template<class T>
    void TAex<T>::addInstance(TInstance<T>* instance) {
        m_instances.push_back(instance);
    }

    template<class T>
    void TAex<T>::removeInstance(TInstance<T>* instance) {
        auto it = std::find(m_instances.begin(), m_instances.end(), instance);
        if (it != m_instances.end())
            m_instances.erase(it);
    }
}
#endif // _AEX_TAEX_IPP
