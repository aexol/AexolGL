#include <aex/UniformDrw.hpp>
#include <aex/ShaderDrw.hpp>
#include <aex/AexRenderer.hpp>
namespace aex {

    using  std::find;

    
    UniformListDrw::UniformListDrw() {

    }
    
    UniformListDrw::UniformListDrw(std::vector<Uniform*> uniforms):
    m_uniforms(uniforms)
    {
        
    }

    UniformListDrw::~UniformListDrw() {

    }

    void UniformListDrw::draw(AexRenderer* gl) {

        const ShaderDrw* shader = gl->getShader();
        
        if (shader) 
        {
            const auto& program = shader->GetProgram();
            for(size_t i = 0 ; i< m_uniforms.size(); ++i) 
            {
                m_uniforms[i]->setUniform(program);
            }
        }

    }

    void UniformListDrw::finishdraw(AexRenderer* gl) {

    }

    void UniformListDrw::needsUpdate() {

    }

    void UniformListDrw::addUniform(Uniform* uniform) {
        
        if(find(m_uniforms.begin(),m_uniforms.end(), uniform)==m_uniforms.end())
        {
            m_uniforms.push_back(uniform);
        }
    }

    void UniformListDrw::removeUniform(Uniform* uniform) {
        
        std::vector<Uniform*>::iterator found = find(m_uniforms.begin(),m_uniforms.end(), uniform);
        
        if(found!= m_uniforms.end())
        {
            m_uniforms.erase(found);
        }
       
    }


}
