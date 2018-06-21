#include <aex/MouseColorPicker.hpp>

namespace aex {

    MouseColorPicker::MouseColorPicker() :
    m_renderTree(nullptr),
    m_globalEvents(nullptr),
    m_frameBuffer(){

    }

    MouseColorPicker::MouseColorPicker(AexRenderTree* aexRenderTree, GlobalGameEventsPtr globalEvents) : ALoopNode(5000),
    m_renderTree(aexRenderTree),
    m_globalEvents(globalEvents) {
        //m_pickerShader = AnimatedMeshPickerShader();
        m_globalEvents->registerEventFunc(this, &MouseColorPicker::pickObjectEventHandler);
        m_frameBuffer.reset(new aex::FrameBufferOGL(m_renderTree->getQuatCamera()->getWindowW(), m_renderTree->getQuatCamera()->getWindowH(), true));

    }
    
    
    void MouseColorPicker::prepareFramebufffer(AexRenderer* gl)
    {
        gl->setColorPick(true);
        if(!m_frameBuffer)
        {
            m_frameBuffer.reset(new aex::FrameBufferOGL(gl->getQuatCamera()->getWindowW(), gl->getQuatCamera()->getWindowH(), true));
        }
        m_frameBuffer->draw(gl);
        m_oldClearColor = gl->getClearColor();
        gl->setClearColor(1.0, 1.0, 1.0, 0.0);
        colorMask(false, false, false, false);
    }
    
    int MouseColorPicker::getPixelId(int x, int y)
    {
        int ret = -1;
        unsigned char data[4] = {0, 0, 0, 0};
        GL_CHECK(glReadPixels(x, m_frameBuffer->getTexSizeY() - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data));
        //GLuint datadata;

        int temp = decode_id((int) data[0], (int) data[1], (int) data[2]);

        //LOG_DEBUG((int) data[0], " ", (int) data[1], " ", (int) data[2], (int) data[3]);
        //LOG_DEBUG(temp);
        if (temp != 0 /*&& (int) datadata >= 0.0*/) {
            ret = temp;
        }

        //m_renderTree->getAexRenderer()->setDepthTestState(true);

        if (m_globalEvents) {
            IdPickedEvent* event = new IdPickedEvent(ret);
            m_globalEvents->handleEvent(event);
            delete event;
        }
        return ret;
    }
    
    void MouseColorPicker::cleanupFramebufffer(AexRenderer* gl)
    {
        colorMask(true, true, true, true);
        m_frameBuffer->finishdraw(gl);
                
        gl->setClearColor(m_oldClearColor[0],m_oldClearColor[1],m_oldClearColor[2],m_oldClearColor[3]);
        gl->setColorPick(false);
    }

    int MouseColorPicker::pickObject(int x, int y) { 
       int ret = -1;
        
        AexRenderer* gl = m_renderTree->getAexRenderer();
        prepareFramebufffer(gl);
        
        renderPickerFrame();
        ret=getPixelId(x,y);
        
        cleanupFramebufffer(gl);
        return ret;

    }

    int MouseColorPicker::decode_id(int r, int g, int b) {
        return b | (g << 8) | (r << 16); /*b + g * 256 + r * 256 * 256*/
    }

    Texture* MouseColorPicker::getTexture()
    {
        return m_frameBuffer->getTexture();
    }
    
    void MouseColorPicker::renderPickerFrame() {
        if (m_renderTree) {
            AexRenderer* renderer = m_renderTree->getAexRenderer();
            m_renderTree->getRenderRoot()->draw(renderer);
            colorMask(false, false, false, false);
            m_renderTree->getLastToRender()->draw(renderer);
            
        }
    }

    void MouseColorPicker::pickObjectEventHandler(const PickObjectEvent* pickEvet) {
        pickObject(pickEvet->GetX(), pickEvet->GetY());
    }

    void MouseColorPicker::update() {
        if (m_pickObject) {
            pickObject(m_x, m_y);
            m_pickObject = false;
        }

    }

    Vector3 MouseColorPicker::intToColor(uint32_t id) {

        return Vector3::intIDtoVectorID(id);
    }

    uint32_t MouseColorPicker::colorToint(Vector3 idColor) {

        return Vector3::vectorIDtoIntID(idColor);
    }

    ShaderDrw MouseColorPicker::simplePickerShader() {
        return ShaderDrw(

                "uniform vec3 colorPick;\n"
                "void main(){"
                "	gl_Position = gl_ViewProjectionMatrix * gl_ModelMatrix * vec4(gl_Vertex, 1.0);\n"
                "}",
                "uniform vec3 colorPick;\n"
                "void main(){"
                "	gl_FragColor = vec4(colorPick.r ,colorPick.g, colorPick.b, 1.0);\n"
                "}"
                );
    }

    ShaderDrw MouseColorPicker::animatedMeshPickerShader() {
        return ShaderDrw(

                "uniform vec3 colorPick;\n"
                "void main(){"
                "vec3 beetwenFrame=  mix( gl_Vertex, gl_nextFrame , tweenFactor);\n"
                "	gl_Position = gl_ViewProjectionMatrix * gl_ModelMatrix * vec4(beetwenFrame, 1.0);\n"
                "}",
                "uniform vec3 colorPick;\n"
                "void main(){"
                "	gl_FragColor = vec4(colorPick.r ,colorPick.g, colorPick.b, 1.0);\n"
                "}"
                );
    }

    void MouseColorPicker::setDebug(bool debug) {
        m_debug = debug;

        if (m_debug == true && m_pickerDebugViewVisual == nullptr) {

            aex::HudElement_ptr pickerView = aex::HudElement::makeHudElement(m_frameBuffer->getTexture(), 0.2, 0.2, 0.0, 0.0);
            pickerView->setNeedOwnShader(true);
            pickerView->setCanChangeDepthTestState(true);
            pickerView->setCanChangeBlendState(false);

            m_pickerDebugViewVisual = aex::make_shared<aex::Visual>();
            m_pickerDebugViewVisual << pickerView;
            m_pickerDebugViewVisual->setRootRenderNode(m_renderTree->getLastToRender());
            m_pickerDebugViewVisual->startDrawing();
            
        } else if (m_debug == true && m_pickerDebugViewVisual != nullptr) {
            m_pickerDebugViewVisual->startDrawing();
        } else {
            if (m_pickerDebugViewVisual != nullptr) {
                m_pickerDebugViewVisual->stopDrawing();
            }
        }
    }

    bool MouseColorPicker::isDebug() const {
        return m_debug;
    }

    void MouseColorPicker::setRenderTree(AexRenderTree* renderTree) {
        m_renderTree = renderTree;
    }

    AexRenderTree* MouseColorPicker::getRenderTree() const {
        return m_renderTree;
    }

    void MouseColorPicker::setPickObject(bool pickObject) {
        m_pickObject = pickObject;
    }

    bool MouseColorPicker::isPickObject() const {
        return m_pickObject;
    }
}
