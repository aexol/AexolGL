/*
 * File:   TextDrw.hpp
 * Author: mritke
 *
 * Created on February 10, 2014, 1:19 PM
 */

#ifndef TEXTDRW_HPP
#define	TEXTDRW_HPP
#include <cstdlib>
#include <cmath>
#include <aex/AexDrawable.hpp>
#include <aex/AexRenderer.hpp>
#include <aex/VertexData.hpp>

#include <aex/Hud.hpp>
#include <aex/HudElement.hpp>
//#include <SDL2/SDL_ttf.h>

namespace aex
{
    
class TextDrw;
    
class AEX_PUBLIC_API FontInfo
{
public:
    float m_size;
    float m_paddingLeft, m_paddingRight, m_paddingTop, m_paddingbBottom;
    float m_lineHeight;
    float m_texSizeW, m_texSizeH;
    int m_pagesNumber;
    std::vector<std::string> m_pageFiles;
    std::vector<Texture*> m_pageTextures;
    std::string m_fontaname;

    FontInfo();
    ~FontInfo();

};

class AEX_PUBLIC_API Glyph
{
public:
    int m_glyphID;
    float m_x, m_y;
    float m_width, m_height;
    float m_xoffset, m_yoffset;
    float m_xadvance;
    std::map<int, float> m_kerningMap;
    int m_page;

    Glyph()
    {
    }

    Glyph(int glyphID, float x, float y, float width, float height, float xoffset, float yoffset, float xadvance, int m_page = 0) : m_glyphID(glyphID), m_x(x), m_y(y), m_width(width), m_height(height), m_xoffset(xoffset), m_yoffset(yoffset), m_xadvance(xadvance)
    {
    }

};

class AEX_PUBLIC_API TextString : public HudElement
{
public:
    friend class TextDrw;
    std::vector<HudElementPtr> m_glyphs;

    TextString(const std::string& text, bool batched = false);
    TextString(const std::string& text, float size, bool batched = false);
    TextString(bool batched = false);
    
    virtual ~TextString();

    void clearBatches() ;
    void draw(AexRenderer* gl);
    void finishdraw(AexRenderer* gl);
    void setText(std::string text);
    std::string getText();
    void setSizeY(float sizeY);
    float getSizeY() const;
    void setSizeX(float sizeX);
    float getSizeX()const;
    float getWidth() const;
    void setPosition(aex::math::Vector2 position);
    void setPosition(float x, float y);
    void addPosition(aex::math::Vector2 position);
    void addPosition(float x, float y);
    
    aex::math::Vector2 getPosition()const;
    void setTextChanged(bool textChanged);
    bool isTextChanged() const;
    /*void setVisible(bool visible);
    bool isVisible() const;*/
    void setGlyphs(std::vector<HudElementPtr> glyphs);
    std::vector<HudElementPtr>* getGlyphs();
private:
    void setWidth(float width);
    
    private:
    aex::math::Vector2 m_position;
    float m_sizeX = 1.0f;
    float m_sizeY = 1.0f;
    std::string m_text;
    bool m_textChanged = true;
    float m_width = 0.0f;
    //bool m_isVisible= true;
    bool m_batched;
    
    std::vector<float> m_spriteBatch;
    std::vector<uint16_t> m_indexBatch;
    std::vector<float> m_PositionsBatch;
    std::vector<float> m_SizesBatch;
    std::vector<float> m_UVOffsetsBatch;    
    std::vector<float> m_UVSizesBatch;
    
    // m_spriteVBO to set
    // m_indexVBO to set
    VertexData* m_positionsVBO;
    VertexData* m_sizesVBO;
    VertexData* m_uVOffsetsVBO;
    VertexData* m_uVSizesVBO;
    
};

typedef aex::shared_ptr<TextString> TextString_ptr;

class AEX_PUBLIC_API TextDrw : public Hud
{

public:

    TextDrw(std::string font_name, int ptsize, std::string text, bool batched = true);
    TextDrw(Texture* tex, std::string text, bool batched = true);
    TextDrw(Texture* tex, bool batched = true);
    virtual ~TextDrw();
    
    void draw(AexRenderer* gl);
    void finishdraw(AexRenderer* gl);
    void needsUpdate();
    //void scale(float sx, float sy);
    void move(float x, float y);
    void renderFont(std::string fontName);
    void renderWallOfTextToSingleQuad(std::string fontName); //TODO: deprecated remove
    void setText(std::string text);                          //TODO:deprecated remove
    std::string getText() const;
    TextString_ptr newString(std::string text);
    TextString_ptr newString(std::string text, float size);
    float getMaxAdvance(TextString_ptr stringPtr);
    void readGlypsFromLittera(std::string filename);
    void readGlyphsFromLitteraFull(std::string filename);
    void setKerningON(bool kerningON);
    bool isKerningON() const;
    void clear();
    void removeString(TextString_ptr textString);
    const FontInfo& getFontInfo() const;
    void calculateWidth(TextString_ptr m_renderString, unsigned windowWidthPix, unsigned windowHeightPix);
    float getPositionForCursor(TextString_ptr m_renderString, unsigned windowWidthPix, unsigned windowHeightPix, uint cursor);
    int getCursorForPosition(TextString_ptr m_renderString, unsigned windowWidthPix, unsigned windowHeightPix, float pos);
    virtual aex::ShaderDrw* createShader();
    virtual void releaseShader(aex::ShaderDrw* s);
    
private:
    std::string m_text;             //TODO: deprecated remove
    bool m_textChanged = true;      //TODO: deprecated remove
    std::map<int, Glyph> m_glyphs;
    FontInfo m_fontInfo;
    //TTF_Font* m_font;
    bool m_monospace = true;
    std::vector<TextString_ptr> m_renderStrings;
    aex::math::Vector2 m_scale;
    bool m_kerningON = false;
    bool m_batched;
    bool m_drawed = false;
};



typedef aex::shared_ptr<TextDrw> TextDrw_ptr;
}


#endif	/* TEXTDRW_HPP */

