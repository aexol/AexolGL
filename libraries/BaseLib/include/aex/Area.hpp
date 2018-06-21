#ifndef NORMAL_AREA_HPP
#define NORMAL_AREA_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <aex/Point.hpp>
#include <aex/Common.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/ShaderDrw.hpp>
#include <aex/VertexData.hpp>
#include <aex/UniformMatrix4fv.hpp>
#include <aex/UniformSampler.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/AWindow.hpp>
#include <aex/HudElement.hpp>
#include <aex/GlobalGameEvents.hpp>
#include <aex/AexRenderTree.hpp>
#include <aex/DeviceOrientationObservator.hpp>
#include <aex/GlobalGameEvents.hpp>
#include <aex/Area.hpp>
#include <aex/TouchEvent.hpp>
#include <aex/FingerJoystick.hpp>
#include <aex/ALoopNode.hpp>
#include <aex/AWindow.hpp>
#include <aex/GlobalGameEvents.hpp>
#include <aex/BuildInEvents.hpp>
#include <aex/Event.hpp>
#include <aex/GestureHandler.hpp>
#include <aex/Logger.hpp>
#include <aex/KeyboardEvent.hpp>
#include <aex/Aex.hpp>
#include <aex/TextDrw.hpp>
#include <aex/TextureManager.hpp>
#include <aex/VertexData.hpp>
#include <vector>

namespace aex {
    namespace gui {
        class NormalArea {
        public:
            FPoint2D pos;
            FPoint2D size;
            FPoint2D m_frameArea;
            bool m_needNormalize;
        public:
            NormalArea();
            NormalArea(const FPoint2D& pos, const FPoint2D& size);
            
            void setNeedNormalize(bool value);
            bool getNeedNormalize();
            
            bool isInArea(const FPoint2D& p) const;
            NormalArea getInternalAreaWithSizeYPadding(float paddingPercent) const;
            NormalArea getInternalAreaWithSizeXPadding(float paddingPercent) const;
            
            NormalArea getCuttedAreaBeforePosX(float posX) const;
            NormalArea getCuttedAreaBeforePosY(float posY) const;
            
            NormalArea getCuttedAreaAfterPosX(float posX) const;
            NormalArea getCuttedAreaAfterPosY(float posY) const;
            
            void scale(float scaleX, float scaleY);
            void move(float moveX, float moveY);
            
            std::vector<float> getRectangleData(const FPoint2D& frameArea);
            std::vector<uint16_t> getDrawOrderData();
            
            std::vector<float> getRectangleDataForCirclePercent(float percentTo100, const FPoint2D& frameArea);
            std::vector<uint16_t> getDrawOrderDataForCirclePercent();
            
            FPoint2D getLeftCenter() const;
            FPoint2D getTopCenter() const;
            FPoint2D getRightCenter() const;
            FPoint2D getBottomCenter() const;
            FPoint2D getCenter() const;
            FPoint2D getLeftTop() const;
            FPoint2D getRightTop() const;
            FPoint2D getRightBottom() const;
            FPoint2D getLeftBottom() const;
            
        protected:
            // Normal To Cartesian:
            float nTocPX(float x);
            float nTocPY(float y);
            float nTocSX(float sizeX);
            float nTocSY(float sizeY);

            std::vector<float> genRectData(const FPoint2D& frameArea);
        private:
            NormalArea getInternalAreaWithPaddingSize(float padSize) const;
        };

        struct ColorRGB {
            float r;
            float g;
            float b;

            ColorRGB();
            ColorRGB(float r, float g, float b);
        };
        
         
        class GUIRectRGB : public DrawObject {
        public:
            GUIRectRGB(const NormalArea& area, const ColorRGB& color, const FPoint2D& windowSizeNormal);
            GUIRectRGB(const NormalArea& area, const ColorRGB& color, float circlePercent, const FPoint2D& windowSizeNormal );
            GUIRectRGB(GUIRectRGB& gr) = delete;
            GUIRectRGB(GUIRectRGB&& gr) = delete;
            virtual ~GUIRectRGB();
            
            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();
            
            std::shared_ptr<ShaderDrw> getShaderInstance();
            
            const NormalArea& getNormalArea();
            void setNormalArea(const NormalArea& area, const FPoint2D& windowSizeNormal);
            void setColor(const ColorRGB& color);
            ColorRGB getColor() const;
            void setArea(const NormalArea& area, const FPoint2D& windowSizeNormal);
            void setCirclePercent(float circlePercent, const FPoint2D& windowSizeNormal);
        private:
            NormalArea normArea;
            ColorRGB m_color;
            
            aex::math::Vector3 vecRGB; // 3 RGB vals
            UniformFloat3 uniColor;

            VertexData vboRect;
            IndexData vboDrawOrder;
            
            ShaderDrw* m_shader;
            bool m_circlePercentBackground;
            float m_percent;
            std::vector<uint16_t> m_lastDrawIndexes;
        protected:
            static int shaderCounter;
            static std::unique_ptr<ShaderDrw> g_shaderInstance;
        };
        
    }
}



#endif // NORMAL_AREA_HPP