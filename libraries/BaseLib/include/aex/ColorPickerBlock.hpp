/* 
 * File:   ColorPickerBlock.hpp
 * Author: mritke
 *
 * Created on 15 stycznia 2016, 10:33
 */

#ifndef COLORPICKERBLOCK_HPP
#define	COLORPICKERBLOCK_HPP

#include <aex/RenderingBlock.hpp>

#include <aex/MouseColorPicker.hpp>

class AEX_PUBLIC_API AexRenderer;
class AEX_PUBLIC_API RenderingTreeNode;
namespace aex
{
    
    class AEX_PUBLIC_API ColorPickerBlock
    {        
    public:
        using TraitsType = RunInitOnlyBlock<ColorPickerBlock>;
        typedef RenderingBlock<ColorPickerBlock, TraitsType> ColorPickerBlockT;
        
        ColorPickerBlock();
        
        void setContext(AexRenderer* gl);
        void setNootNoode(RenderingTreeNode* rootNoode);
        void startColorPick(); 
        void pause();
        void resume();
        bool init();
        void beginFrame();
        void endFrame();
        
    private:
        AexRenderer* m_aexRenderer = nullptr;
        RenderingTreeNode* m_rootNoode = nullptr;
        bool m_isColorPick = false;
        
    protected:
        friend CameraRenderBlock;
        friend TraitsType;
        
    };
    
    
}

#endif	/* COLORPICKERBLOCK_HPP */

