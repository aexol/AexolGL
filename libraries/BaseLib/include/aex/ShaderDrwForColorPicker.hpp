/*
 * File:   ShaderDrwForColorPicker.hpp
 * Author: mritke
 *
 * Created on April 16, 2014, 4:07 PM
 */

#ifndef SHADERDRWFORCOLORPICKER_HPP
#define	SHADERDRWFORCOLORPICKER_HPP


#include <aex/ShaderDrw.hpp>
#include <string>
#include <vector>
#include <memory>
#include <map>


namespace aex
{

    class AEX_PUBLIC_API ShaderDrwForColorPicker : public ShaderDrw
    {
    private:
        mutable ThreadLocalStorage<AexShader> m_pickerProgram;
        mutable ThreadLocalStorage<AexShader> m_drawProgram;
        bool m_debug = false;
    public:

        ShaderDrwForColorPicker();
        ShaderDrwForColorPicker(const ShaderDrwForColorPicker&);
        ShaderDrwForColorPicker& operator=(const ShaderDrwForColorPicker&);
        ShaderDrwForColorPicker(const aex::string& vertex_source, const aex::string& fragment_source, aex::string vertex_attr = "");
        
        void build() const;
        void draw(AexRenderer* gl);
        void finishdraw(AexRenderer* gl);
        void setDebug(bool set);
    };

    typedef aex::shared_ptr<ShaderDrwForColorPicker> ShaderDrwForColorPicker_ptr;

}
#endif	/* SHADERDRWFORCOLORPICKER_HPP */

