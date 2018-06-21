#include "aex/gui/FontRenderer.hpp"
#include "aex/gui/Frame.hpp"
#include <aex/TextureManager.hpp>

namespace aex
{
    namespace gui
    {
        std::map<std::string, FontRenderer::FontCounter> FontRenderer::g_fontsRenderers;

        FontRenderer::FontRenderer(float height, const BitmapFontGroup& font) :
        m_height(height)
        {
            float pixSize = Frame::h2Pix(height);
            const BitmapFont& bf = font.getBestFontFor(pixSize);
            m_glyphsPath = bf.getGlyphsPath();

            // flyiweight
            if (g_fontsRenderers.find(m_glyphsPath) != g_fontsRenderers.end())
            {
                m_textRef = g_fontsRenderers[m_glyphsPath].texts;
                g_fontsRenderers[m_glyphsPath].counter++;
            } else
            {
                TextDrw_ptr txdrw = aex::make_shared<aex::TextDrw>(aex::TextureManager::GetInstance().getTexture(bf.getTexturePath(), true));
                txdrw->setCanChangeDepthTestState(false);
                txdrw->readGlyphsFromLitteraFull(bf.getGlyphsPath());

                FontCounter fock(txdrw);
                g_fontsRenderers[m_glyphsPath] = fock;
                m_textRef = txdrw;
            }
            setText("", false);
        }

        FontRenderer::~FontRenderer()
        {
            if (g_fontsRenderers.find(m_glyphsPath) != g_fontsRenderers.end())
            {
                g_fontsRenderers[m_glyphsPath].counter--;
                if (g_fontsRenderers[m_glyphsPath].counter == 0)
                {
                    g_fontsRenderers.erase(g_fontsRenderers.find(m_glyphsPath));
                }
            }
        }

        void FontRenderer::draw(AexRenderer* gl)
        {
            m_textRef->setHudOffsetXY(Frame::x2N(getInnerArea().p().x()), Frame::y2N(getInnerArea().p().y()));
            m_textString->setVisible(true);
            m_textRef->draw(gl);
            m_textRef->finishdraw(gl);
            m_textString->setVisible(false);
        }

        void FontRenderer::finishdraw(AexRenderer* gl)
        {
        }

        void FontRenderer::needsUpdate()
        {
        }

        void FontRenderer::setText(const std::string& text, bool callback)
        {
            if (!m_textString)
            {
                const FontInfo& fontInfo = m_textRef->getFontInfo();
                m_scale = Frame::h2Pix(m_height) / fontInfo.m_lineHeight;
                m_textString = m_textRef->newString(text, m_scale);
            } else
            {
                m_textString->setText(text);
            }
            m_textString->setVisible(false); // on only when render
            m_textRef->calculateWidth(m_textString, Frame::getWidthPixels(), Frame::getHeightPixels());
            setRefresh(true);
            setFlag(flags::FLAG::SIZE_CHANGED);
            m_string = text;

            if (callback)
                onTextChanged();
        }
    }
}