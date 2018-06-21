#include "aex/gui/TextSettings.hpp"
#include "aex/gui/Frame.hpp"
#include <boost/locale.hpp>
//#include <codecvt>
#include <string>
#include "aex/utf8.h"


namespace aex
{
    namespace gui
    {

        FontSettings::FontSettings() :
        channels(FONT_TEX_CHANNELS::ALPHA),
        smooth(FONT_SMOOTH::SHADED)
        {
        }

        FontSettings::FontSettings(FONT_TEX_CHANNELS channels, FONT_SMOOTH smooth) :
        channels(channels),
        smooth(smooth)
        {
        }

        std::ostream& operator<<(std::ostream& out, const FontSettings& fntSettings)
        {
            return out << aex::to_string((int) fntSettings.channels) << "_" << aex::to_string((int) fntSettings.smooth);
        }

        FontSize::FontSize(int sizePt) :
        m_sizePt(sizePt),
        m_mockupSize(GSize2D<int>(Frame::getWidthPixels(), Frame::getHeightPixels())),
        m_scaler(Scaler(m_mockupSize, SCALE_PLANE::HORIZONTAL))
        {
        }

        FontSize::FontSize(int sizePt, const GSize2D<int>& mockupSize, SCALE_PLANE scalePlane) :
        m_sizePt(sizePt),
        m_mockupSize(mockupSize),
        m_scaler(mockupSize, scalePlane)
        {
        }

        FontSize::FontSize(float normalizedSize) :
        m_sizePt(Frame::norm2Pix(normalizedSize)),
        m_mockupSize(GSize2D<int>(Frame::getWidthPixels(), Frame::getHeightPixels())),
        m_scaler(m_mockupSize, SCALE_PLANE::HORIZONTAL)
        {
        }

        FontSize::FontSize(double normalizedSize) :
        FontSize((float)normalizedSize)
        {
        }

        int FontSize::getSizePt() const
        {
            return m_sizePt;
        }

        float FontSize::getNormSize() const
        {
            return m_scaler.pix(m_sizePt);
        }

        GSize2D<int> FontSize::getFontMockupSize() const
        {
            return m_mockupSize;
        }

        bool FontSize::operator==(const FontSize& rightHand)
        {
            return m_normSize == rightHand.m_normSize;
        }

        bool FontSize::operator!=(const FontSize& rightHand)
        {
            return !(*this == rightHand);
        }

        bool FontSize::operator<(const FontSize& rightHand)
        {
            return m_normSize < rightHand.m_normSize;
        }

        bool FontSize::operator>(const FontSize& rightHand)
        {
            return m_normSize > rightHand.m_normSize;
        }

        bool FontSize::operator<=(const FontSize& rightHand)
        {
            return (*this == rightHand) || (*this < rightHand);
        }

        bool FontSize::operator>=(const FontSize& rightHand)
        {
            return (*this == rightHand) || (*this > rightHand);
        }

        TextSettings::TextSettings() :
        text(L"empty text"),
        fontSize(48, GSize2D<int>(600, 800)),
        withKerning(false),
        area(Frame::getFrameArea()),
        lineWrap(true),
        align(ALIGN::LEFT),
        discardTruncated(true),
        discardOuted(true),
        apllyXOffsetAtWordBeginning(true),
        trimLeft(true),
        trimRight(true),
        tracking(0),
        color(Color(255,255,255,255))
        {
        }

        TextSettings::TextSettings(const aex::wstring& text, const FontSize& fontSize, bool withKern, const Area& area, bool lineWrap, ALIGN align, bool discardTruncated, bool discardOuted) :
        text(text),
        fontSize(fontSize),
        withKerning(withKern),
        area(area),
        lineWrap(lineWrap),
        align(align),
        discardTruncated(discardTruncated),
        discardOuted(discardOuted),
        apllyXOffsetAtWordBeginning(true),
        trimLeft(true),
        trimRight(true),
        tracking(0),
        color(Color(255,255,255,255))
        {
        }

        TextSettings::~TextSettings()
        {
        }

        namespace text
        {
            aex::wstring s2ws(const std::string& str)
            {
                return aex::wstring(str.begin(), str.end());
            }

            std::string ws2s(const aex::wstring& fstr)
            {
                return std::string(fstr.begin(), fstr.end());
            }

            size_t to_narrow(const wchar_t* src, char* dest, size_t dest_len)
            {
                size_t i;
                wchar_t code;
                i = 0;
                while (src[i] != '\0' && i < (dest_len - 1))
                {
                    code = src[i];
                    if (code < 128)
                        dest[i] = char(code);
                    else
                    {
                        dest[i] = '?';
                        if (code >= 0xD800 && code <= 0xD8FF)
                            // lead surrogate, skip the next code unit, which is the trail
                            i++;
                    }
                    i++;
                }
                dest[i] = '\0';
                return i - 1;
            }

            char wc2c(const wchar_t& fc)
            {
                char retC;
                if (fc < 128)
                {
                    retC = char(fc);
                } else
                {
                    retC = '?';
                }
                return retC;
            }

            wchar_t c2wc(const char& c)
            {
                return static_cast<wchar_t> (c);
            }

            aex::wstring utf8_to_wstring(const std::string& utf8Str)
            {
                //return boost::locale::conv::utf_to_utf<wchar_t>(utf8Str.c_str());
                aex::wstring wstr;
                utf8::utf8to32(utf8Str.begin(), utf8Str.end(), std::back_inserter(wstr));
                return wstr;
            }

            aex::string wstring_to_utf8(const aex::wstring& wstr)
            {
                //return boost::locale::conv::to_utf<char>(wstr.c_str());
                aex::string utf8str;
                utf8::utf32to8(wstr.begin(), wstr.end(), std::back_inserter(utf8str));
                return utf8str;
            }

//            inline void decode_utf8(const std::string& bytes, std::wstring& wstr)
//            {
//                utf8::utf8to32(bytes.begin(), bytes.end(), std::back_inserter(wstr));
//            }
//            inline void encode_utf8(const std::wstring& wstr, std::string& bytes)
//            {
//                utf8::utf32to8(wstr.begin(), wstr.end(), std::back_inserter(bytes));
//            }
        }
    }
}
