#include <aex/Area.hpp>
#include <aex/Logger.hpp>
#include <cmath>

#define RAD(stop)  ((stop*M_PI)/180.0f) 

namespace aex
{
    namespace gui
    {
        
        NormalArea::NormalArea() :
        pos(aex::FPoint2D(0.0f, 0.0f)),
        size(aex::FPoint2D(1.0f, 1.0f))
        {
            m_needNormalize = true;
        }

        NormalArea::NormalArea(const aex::FPoint2D& pos, const aex::FPoint2D& size) :
        pos(pos),
        size(size)
        {
            m_needNormalize = true;
        }
        
        void NormalArea::setNeedNormalize(bool value)
        {
            m_needNormalize = value;
        }
            
        bool NormalArea::getNeedNormalize()
        {
            return m_needNormalize;
        }

        bool NormalArea::isInArea(const aex::FPoint2D& p) const
        {
            if ((p.x() >= pos.x()) && (p.x() <= (pos.x() + size.x())))
            {
                if ((p.y() >= pos.y()) && (p.y() <= (pos.y() + size.y())))
                {
                    return true;
                }
            }

            return false;
        }

        NormalArea NormalArea::getInternalAreaWithSizeYPadding(float paddingPercent) const
        {
            float padSize = paddingPercent * size.y();
            return getInternalAreaWithPaddingSize(padSize);
        }

        NormalArea NormalArea::getInternalAreaWithSizeXPadding(float paddingPercent) const
        {
            float padSize = paddingPercent * size.x();
            return getInternalAreaWithPaddingSize(padSize);
        }

        NormalArea NormalArea::getCuttedAreaBeforePosX(float posX) const
        {
            if (posX <= pos.x()) // before left side of rect
            {
                return NormalArea(pos, FPoint2D(0.0f, size.y()));
            } else
                if (posX >= (pos.x() + size.x()))
            {
                return NormalArea(pos, size); // after right side of rect
            } else
            {
                return NormalArea(pos, FPoint2D(posX - pos.x(), size.y()));
            }
        }

        NormalArea NormalArea::getCuttedAreaBeforePosY(float posY) const
        {
            if (posY <= pos.y()) // before top side of rect
            {
                return NormalArea(pos, FPoint2D(size.x(), 0.0f));
            } else
                if (posY >= (pos.y() + size.y()))
            {
                return NormalArea(pos, size); // after bottom side of rect
            } else
            {
                // in
                return NormalArea(pos, FPoint2D(size.x(), posY - pos.y()));
            }
        }

        NormalArea NormalArea::getCuttedAreaAfterPosX(float posX) const
        {
            if (posX <= pos.x()) // before left side of rect
            {
                return NormalArea(pos, size);
            } else
                if (posX >= (pos.x() + size.x())) // after right side of rect
            {
                return NormalArea(FPoint2D(pos.x() + size.x(), pos.y()), FPoint2D(0.0f, size.y()));
            } else
            {
                // in rect
                return NormalArea(FPoint2D(posX, pos.y()), FPoint2D((pos.x() + size.x()) - posX, size.y()));
            }
        }

        NormalArea NormalArea::getCuttedAreaAfterPosY(float posY) const
        {
            if (posY <= pos.y()) // before top side of rect
            {
                return NormalArea(pos, size);
            } else
                if (posY >= (pos.y() + size.y())) // after bottom side of rect
            {
                return NormalArea(FPoint2D(pos.x(), pos.y() + size.y()), FPoint2D(size.x(), 0.0f));
            } else
            {
                // in rect
                return NormalArea(FPoint2D(pos.x(), posY), FPoint2D(size.x(), (pos.y() + size.y()) - posY));
            }
        }

        void NormalArea::scale(float scaleX, float scaleY)
        {
            size.x() *= scaleX;
            size.y() *= scaleY;
        }

        void NormalArea::move(float moveX, float moveY)
        {
            pos.x() += moveX;
            pos.y() += moveY;
        }

        NormalArea NormalArea::getInternalAreaWithPaddingSize(float padSize) const
        {
            return NormalArea(FPoint2D(pos.x() + padSize, pos.y() + padSize), FPoint2D(size.x() - 2 * padSize, size.y() - 2 * padSize));
        }

        std::vector<float> NormalArea::getRectangleData(const FPoint2D& frameArea)
        {
            return genRectData(frameArea);
        }

        std::vector<uint16_t> NormalArea::getDrawOrderData()
        {
            return std::vector<uint16_t>{3, 0, 2, 1};
        }
        
        std::vector<float> NormalArea::getRectangleDataForCirclePercent(float percentTo100, const FPoint2D& frameArea)
        {   
            m_frameArea = frameArea;
            while(percentTo100 > 100)
                percentTo100 -= 100;
            
            std::vector<float> circRects;
            float oglCenterX, oglCenterY;
            oglCenterX = nTocPX(getCenter().x());
            oglCenterY = nTocPY(getCenter().y());
            if(percentTo100 <= 0)
            {
                for(int i = 0 ; i < 18 ; i++){
                    circRects.push_back(oglCenterX);
                    circRects.push_back(oglCenterY);
                }
                return circRects;
            }
            
            float posX, posY;
            if(percentTo100 > 0)
            {
                circRects.push_back(oglCenterX);
                circRects.push_back(oglCenterY);
                
                posX = nTocPX(getTopCenter().x());
                posY = nTocPY(getTopCenter().y());
                circRects.push_back(posX);
                circRects.push_back(posY);
            }
            
            
            float stopnie = 360*(percentTo100/100.0f);
            int fullParts = stopnie / 45;
            for(int i = 1 ; i <= fullParts ; i++)
            {
                switch(i){
                    break;
                    case 1:
                        posX = nTocPX(getRightTop().x());
                        posY = nTocPY(getRightTop().y());
                        circRects.push_back(posX);
                        circRects.push_back(posY);
                        circRects.push_back(oglCenterX);
                        circRects.push_back(oglCenterY);
                    break;
                    case 2:
                        posX = nTocPX(getRightCenter().x());
                        posY = nTocPY(getRightCenter().y());
                        circRects.push_back(posX);
                        circRects.push_back(posY);
                        circRects.push_back(oglCenterX);
                        circRects.push_back(oglCenterY);
                    break;
                    case 3:
                        posX = nTocPX(getRightBottom().x());
                        posY = nTocPY(getRightBottom().y());
                        circRects.push_back(posX);
                        circRects.push_back(posY);
                        circRects.push_back(oglCenterX);
                        circRects.push_back(oglCenterY);
                    break;
                    case 4:
                        posX = nTocPX(getBottomCenter().x());
                        posY = nTocPY(getBottomCenter().y());
                        circRects.push_back(posX);
                        circRects.push_back(posY);
                        circRects.push_back(oglCenterX);
                        circRects.push_back(oglCenterY);
                    break;
                    case 5:
                        posX = nTocPX(getLeftBottom().x());
                        posY = nTocPY(getLeftBottom().y());
                        circRects.push_back(posX);
                        circRects.push_back(posY);
                        circRects.push_back(oglCenterX);
                        circRects.push_back(oglCenterY);
                    break;
                    case 6:
                        posX = nTocPX(getLeftCenter().x());
                        posY = nTocPY(getLeftCenter().y());
                        circRects.push_back(posX);
                        circRects.push_back(posY);
                        circRects.push_back(oglCenterX);
                        circRects.push_back(oglCenterY);
                    break;
                    case 7:
                        posX = nTocPX(getLeftTop().x());
                        posY = nTocPY(getLeftTop().y());
                        circRects.push_back(posX);
                        circRects.push_back(posY);
                        circRects.push_back(oglCenterX);
                        circRects.push_back(oglCenterY);
                    break;
                    case 8:
                        posX = nTocPX(getTopCenter().x());
                        posY = nTocPY(getTopCenter().y());
                        circRects.push_back(posX);
                        circRects.push_back(posY);
                        circRects.push_back(oglCenterX);
                        circRects.push_back(oglCenterY);
                    break;
                    
                    default:
                        break;
                }
            }

            float alfa = stopnie - (fullParts * 45);
            if (alfa != 0)
            {
                float opAlfa = 45 - alfa;

                float b = size.y() / 2;
                float c = b / cos(RAD(alfa));
                float a = sin(RAD(alfa)) * c;

                float b2 = size.y() / 2;
                float c2 = b2 / cos(RAD(opAlfa));
                float a2 = sin(RAD(opAlfa)) * c2;
                // last triangle point calculation

                switch (fullParts)
                {
                    case 0:
                        posX = nTocPX(getTopCenter().x() + a);
                        posY = nTocPY(getTopCenter().y());
                        circRects.push_back(posX);
                        circRects.push_back(posY);
                        break;
                    case 1:
                        posX = nTocPX(getRightTop().x());
                        posY = nTocPY(getRightCenter().y() - a2);
                        circRects.push_back(posX);
                        circRects.push_back(posY);
                        break;
                    case 2:
                        posX = nTocPX(getRightCenter().x());
                        posY = nTocPY(getRightCenter().y() + a);
                        circRects.push_back(posX);
                        circRects.push_back(posY);
                        break;
                    case 3:
                        posX = nTocPX(getBottomCenter().x() + a2);
                        posY = nTocPY(getRightBottom().y());
                        circRects.push_back(posX);
                        circRects.push_back(posY);
                        break;
                    case 4:
                        posX = nTocPX(getBottomCenter().x() - a);
                        posY = nTocPY(getBottomCenter().y());
                        circRects.push_back(posX);
                        circRects.push_back(posY);
                        break;
                    case 5:
                        posX = nTocPX(getLeftBottom().x());
                        posY = nTocPY(getLeftCenter().y() + a2);
                        circRects.push_back(posX);
                        circRects.push_back(posY);
                        break;
                    case 6:
                        posX = nTocPX(getLeftCenter().x());
                        posY = nTocPY(getLeftCenter().y() - a);
                        circRects.push_back(posX);
                        circRects.push_back(posY);
                        break;
                    case 7:
                        posX = nTocPX(getTopCenter().x() - a2);
                        posY = nTocPY(getLeftTop().y());
                        circRects.push_back(posX);
                        circRects.push_back(posY);
                        break;
                    default:
                        break;
                }
                if(fullParts < 8)
                {
                    circRects.push_back(oglCenterX);
                    circRects.push_back(oglCenterY);
                }
            }else{
                if(fullParts < 8){
                    circRects.push_back(oglCenterX);
                    circRects.push_back(oglCenterY);
                    circRects.push_back(oglCenterX);
                    circRects.push_back(oglCenterY);
                }
            }
            
//            float alfa = stopnie - (fullParts * 45);
//            float r = size.y() / 2;
//            float pX = getCenter().x() + (r*sin(RAD(stopnie)));
//            float pY = getCenter().y() - (r*cos(RAD(stopnie)));

            
            for(int i = 0 ; i < 7-fullParts ; i++)
            {
                circRects.push_back(oglCenterX);
                circRects.push_back(oglCenterY);
                
                circRects.push_back(oglCenterX);
                circRects.push_back(oglCenterY);
            }
            
            return circRects;
        }
        
        std::vector<uint16_t> NormalArea::getDrawOrderDataForCirclePercent()
        {      
            std::vector<uint16_t> circDrawOrder;         
            for(int i = 0 ; i < 18 ; i++)
            {
                circDrawOrder.push_back(i);
            }

            return circDrawOrder;
        }
        
        FPoint2D NormalArea::getLeftCenter() const
        {
            return FPoint2D(pos.x(), pos.y()+size.y()/2);
        }    
        
        FPoint2D NormalArea::getTopCenter() const
        {
            return FPoint2D(pos.x()+size.x()/2, pos.y());
        }
        
        FPoint2D NormalArea::getRightCenter() const
        {
            return FPoint2D(pos.x() + size.x(), pos.y() + size.y()/2);
        }
        
        FPoint2D NormalArea::getBottomCenter() const
        {
            return FPoint2D(pos.x()+size.x()/2, pos.y()+size.y());
        }
        
        FPoint2D NormalArea::getCenter() const
        {
            return FPoint2D(pos.x() + size.x()/2, pos.y() + size.y()/2);
        }
        
        FPoint2D NormalArea::getLeftTop() const
        {
            return pos;
        }
        
        FPoint2D NormalArea::getRightTop() const
        {
            return FPoint2D(pos.x()+size.x(), pos.y());
        }
        
        FPoint2D NormalArea::getRightBottom() const
        {
            return FPoint2D(pos.x() + size.x(), pos.y()+size.y());
        }
        
        FPoint2D NormalArea::getLeftBottom() const
        {
            return FPoint2D(pos.x(), pos.y()+size.y());
        }

        float NormalArea::nTocPX(float x)
        {
            float divisor = (m_needNormalize) ? m_frameArea.x() : 1.0f;
            return (x / divisor) * 2 - 1;
        }

        float NormalArea::nTocPY(float y)
        {
            float divisor = (m_needNormalize) ? m_frameArea.y() : 1.0f;
            return ((y / divisor) * 2 - 1) * -1;
        }

        float NormalArea::nTocSX(float sizeX)
        {
            float divisor = (m_needNormalize) ? m_frameArea.x() : 1.0f;
            return (sizeX / divisor) * 2;
        }

        float NormalArea::nTocSY(float sizeY)
        {
             float divisor = (m_needNormalize) ? m_frameArea.y() : 1.0f;
            return (sizeY / divisor) * 2;
        }

        std::vector<float> NormalArea::genRectData(const FPoint2D& frameArea)
        {
            m_frameArea = frameArea;
            float pX = nTocPX(pos.x());
            float pY = nTocPY(pos.y());
            float sX = nTocSX(size.x());
            float sY = nTocSY(size.y());
            return std::vector<float>{
                // v2 top left, v2 bottom left, v2 bottom right, v2 top right
                pX, pY,
                pX, pY - sY,
                pX + sX, pY - sY,
                pX + sX, pY
            };
        }

        ColorRGB::ColorRGB() :
        r(0.0f),
        g(0.0f),
        b(0.0f)
        {
        }

        ColorRGB::ColorRGB(float r, float g, float b) :
        r(r),
        g(g),
        b(b)
        {
        }
        
       const char* vshSource =
                // wyciete z tad atrybuty dodaje sie w 3 parametrze shadera
                "void main() {\n"
                "   gl_Position = aPosition;\n"
                "}\n";

        const char* fshSource =
                "precision mediump float;\n"
                "uniform vec3 uColor;\n"
                "void main () {\n"
                "    gl_FragColor = vec4(uColor, 1.0);\n"
                "}\n";

        
        int GUIRectRGB::shaderCounter = 0;
        std::unique_ptr<ShaderDrw> GUIRectRGB::g_shaderInstance;
        GUIRectRGB::GUIRectRGB(const NormalArea& area, const ColorRGB& color, const FPoint2D& windowSizeNormal) :
        normArea(area),
        m_color(color),
        vecRGB
        {
            color.r, color.g, color.b
        },
        uniColor(vecRGB, "uColor"),
        vboRect(VertexData::makeVertexData(normArea.getRectangleData(windowSizeNormal), "aPosition", 2)),
        vboDrawOrder{IndexData::createTraingleStrip(normArea.getDrawOrderData())},
        m_circlePercentBackground(false),
        m_percent(0)
        {
            if(g_shaderInstance.get() == nullptr){
                shaderCounter = 0;
                g_shaderInstance.reset(new ShaderDrw(vshSource, fshSource, "attribute vec4 aPosition;\n"));
                
            }
            
            m_shader = g_shaderInstance.get();
            shaderCounter++;
        }
        
        GUIRectRGB::GUIRectRGB(const NormalArea& area, const ColorRGB& color, float circlePercent, const FPoint2D& windowSizeNormal) :
        normArea(area),
        m_color(color),
        vecRGB
        {
            color.r, color.g, color.b
        },
        uniColor(vecRGB, "uColor"),
        vboRect(VertexData::makeVertexData(normArea.getRectangleDataForCirclePercent(circlePercent,windowSizeNormal), "aPosition", 2)),
        vboDrawOrder{IndexData::createTraingleStrip(normArea.getDrawOrderDataForCirclePercent())},
        m_circlePercentBackground(true),
        m_percent(circlePercent)
        {
            if(g_shaderInstance.get() == nullptr){
                shaderCounter = 0;
                g_shaderInstance.reset(new ShaderDrw(vshSource, fshSource, "attribute vec4 aPosition;\n"));
            }
            
            m_shader = g_shaderInstance.get();
            shaderCounter++;
        }
        
        GUIRectRGB::~GUIRectRGB()
        {
            if(g_shaderInstance){
                shaderCounter--;
                if(shaderCounter == 0){
                    g_shaderInstance.reset();
                }
            }
        }

        void GUIRectRGB::draw(AexRenderer* gl)
        {
            m_shader->draw(gl);
            const AexShader& program = m_shader->GetProgram();

            uniColor.setUniform(program);
            program->setVertexData(vboRect);
            program->setIndices(vboDrawOrder);
            program->draw();
        }

        void GUIRectRGB::finishdraw(AexRenderer* gl)
        {
            const AexShader& program = m_shader->GetProgram();
            m_shader->finishdraw(gl);
            program->releaseVertexData(vboRect);
        }

        void GUIRectRGB::needsUpdate()
        {
            // empty
        }
        
        const NormalArea& GUIRectRGB::getNormalArea()
        {
            return normArea;
        }
        
        void GUIRectRGB::setNormalArea(const NormalArea& area, const FPoint2D& windowSizeNormal){
            normArea = area;
            vboRect = VertexData::makeVertexData(normArea.getRectangleData(windowSizeNormal), "aPosition", 2);
        }
        
        ColorRGB GUIRectRGB::getColor() const
        {
            return m_color;
        }
        
        void GUIRectRGB::setColor(const ColorRGB& color)
        {
            m_color = color;
            vecRGB.x = color.r;
            vecRGB.y = color.g;
            vecRGB.z = color.b;
            uniColor.setValue(vecRGB);
        }
        
         void GUIRectRGB::setArea(const NormalArea& area, const FPoint2D& windowSizeNormal)
         {
             normArea = area;
             if(m_circlePercentBackground)
             {
                 vboRect = VertexData(VertexData::makeVertexData(normArea.getRectangleDataForCirclePercent(m_percent,windowSizeNormal), "aPosition", 2));
             }else{
                 vboRect = VertexData(VertexData::makeVertexData(normArea.getRectangleData(windowSizeNormal), "aPosition", 2));
             }
         }
        
         void GUIRectRGB::setCirclePercent(float circlePercent, const FPoint2D& windowSizeNormal)
         {
             m_percent = circlePercent;
             if(m_circlePercentBackground)
             {
                 vboRect = VertexData(VertexData::makeVertexData(normArea.getRectangleDataForCirclePercent(m_percent, windowSizeNormal), "aPosition", 2));
             }
         }
    }
}