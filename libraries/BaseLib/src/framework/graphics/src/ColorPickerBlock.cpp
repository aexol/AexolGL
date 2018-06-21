#include <aex/ColorPickerBlock.hpp>
#include <aex/AexRenderer.hpp>

namespace aex {

    ColorPickerBlock::ColorPickerBlock() {

    }

    void 
    ColorPickerBlock::setContext(AexRenderer* gl)
    {
        m_aexRenderer = gl;
    }
    
    void 
    ColorPickerBlock::setNootNoode(RenderingTreeNode* rootNoode)
    {
        m_rootNoode = rootNoode;
    }
    
    void ColorPickerBlock::startColorPick()
    {
        m_isColorPick=true;
    }
    
    void ColorPickerBlock::pause() {

    }

    void ColorPickerBlock::resume() {

    }

    bool ColorPickerBlock::init() {

    }

    void ColorPickerBlock::beginFrame() {
        if(m_isColorPick)
        {
                  
        }
    }

    void ColorPickerBlock::endFrame() {
        
        if(m_isColorPick)
        {
            
            m_isColorPick=false;
        }
        
    }


}
