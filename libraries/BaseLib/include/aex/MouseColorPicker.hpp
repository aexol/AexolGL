/*
 * File:   MouseColorPicker.hpp
 * Author: mritke
 *
 * Created on April 14, 2014, 2:10 PM
 */

#ifndef MOUSECOLORPICKER_HPP
#define	MOUSECOLORPICKER_HPP

#include <aex/AexolGL.hpp>

namespace aex
{

    class AEX_PUBLIC_API PickObjectEvent : public GameEvent
    {
    private:
        int m_x, m_y;

    public:

        PickObjectEvent(int x, int y) : m_x(x), m_y(y)
        {
        }

        int GetY() const
        {
            return m_y;
        }

        int GetX() const
        {
            return m_x;
        }

    };

    class AEX_PUBLIC_API IdPickedEvent : public GameEvent
    {
    private:
        uint32_t m_ID;

    public:

        IdPickedEvent(uint32_t id) : m_ID(id)
        {
        }

        int GetID() const
        {
            return m_ID;
        }
    };

    typedef std::map<uint32_t, Aex*> ObjectMap;

    class AEX_PUBLIC_API MouseColorPicker : public ALoopNode
    {

    public:
        MouseColorPicker();
        MouseColorPicker(AexRenderTree* aexRenderTree, GlobalGameEventsPtr globalEvents);

        void renderPickerFrame();
        int pickObject(int x, int y);
        Vector3 intToColor(uint32_t id);
        uint32_t colorToint(Vector3 idColor);
        void pickObjectEventHandler(const PickObjectEvent* pickEvet);
        
        void prepareFramebufffer(AexRenderer* gl);
        int getPixelId(int x, int y);
        void cleanupFramebufffer(AexRenderer* gl);
        
        void update();

        void pause()
        {
            
        }

        void resume()
        {
            
        }

        void finish()
        {
            m_done = true;
        };

        bool init()
        {
            return true;
        };
        
        Texture* getTexture();
        int decode_id(int r, int g, int b);
        ShaderDrw simplePickerShader();
        ShaderDrw animatedMeshPickerShader();
        void setDebug(bool debug);
        bool isDebug() const;
        void setRenderTree(AexRenderTree* renderTree);
        AexRenderTree* getRenderTree() const;
        void setPickObject(bool pickObject);
        bool isPickObject() const;
        
    private:

        AexRenderTree* m_renderTree;
        GlobalGameEventsPtr m_globalEvents;
        DrawNode_ptr m_pickerNode;
        ShaderDrw m_pickerShader;
        ObjectMap m_objectMap;
        std::shared_ptr<FrameBufferOGL> m_frameBuffer;
        bool m_pickObject = false;
        bool m_debug = false;
        int m_x = 0, m_y = 0;
        std::array<float, 4> m_oldClearColor;

        aex::Visual_ptr m_pickerDebugViewVisual;
        
    };
}


#endif	/* MOUSECOLORPICKER_HPP */

