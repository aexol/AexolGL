#include <aex/VRCamera.hpp>
#include <aex/RenderingTreeBlock.hpp>
namespace aex {

    Matrix VRCamera::transforms() {
        return get_view_projection_matrix();
    }

    Matrix getOffsetView(Matrix view,  Vector3 xAxis,  Vector3 yAxis,  Vector3 zAxis,  Vector3 position)
    {
        Vector3 neg_eye = -position;

        view[3]  =  xAxis % neg_eye;
        view[7]  =  yAxis % neg_eye;
        view[11] =  zAxis % neg_eye;
        return view;
    }
    
    Matrix VRCamera::get_view_matrix(){
        Matrix ret =getOffsetView(QuaternionCamera::get_view_matrix(),getXAxis(), getYAxis(), getZAxis(), GetPosition());
        return ret;
    }
    
    Matrix VRCamera::get_view_projection_matrix(){
        Matrix ret = QuaternionCamera::get_projection_matrix() * get_view_matrix();
        return ret;
    }

    Vector3 VRCamera::GetPosition() {
        Vector3 ret = QuaternionCamera::GetPosition();
        Vector3 offset=   getXAxis() * m_offset* (-1.0f+(2.0f*float(m_currentEye)));  
        return ret +offset;
    }

    void VRCamera::setEye(bool eye) {
        if(m_currentEye!=eye)
        {
            setDirtyFlag();
        }
        m_currentEye = eye;
        
    }
    
    void VRCamera::toggleEye() {
        m_currentEye = !m_currentEye;
        setDirtyFlag();

    }

    void VRCamera::setOffset(float offset) {
        m_offset = offset;
    }

    float VRCamera::getOffset() const {
        return m_offset;
    }

    void makeBarrelVert(int gridSize, float radius, HudElement* hudelement)
    {
        float cellsize = 1.0 / float(gridSize - 1);
        float uvcellsize = 1.0 / float (gridSize - 1);
        float halfRadius = radius / 2.0f;

        std::vector<mesh_vectors_type> Temp_Vertices;
        std::vector<mesh_vectors_type> Temp_Normals;
        std::vector<mesh_vectors_type> Temp_TextCoords;
        std::vector<mesh_indices_type> Temp_Indices;

        int walls = 1;
        LOG_DEBUG("Temp_Vertices size: ");

        Vector3 newv;
        
        int v = 0;
        int n = 0;
        int t = 0;


        int numStripsRequired = gridSize - 1;
        int numDegensRequired = 2 * (numStripsRequired - 1);
        int verticesPerStrip = 2 * gridSize;
        int size = ((verticesPerStrip * numStripsRequired) + numDegensRequired) * walls + (walls * 2 - 1);

        Temp_Vertices.resize(gridSize * gridSize * walls * 3, 0.0f);
        Temp_Normals.resize(gridSize * gridSize * 3 * walls, 0.0f);
        Temp_TextCoords.resize(gridSize * gridSize * 2 * walls, 0.0f);
        Temp_Indices.resize(size,0);
        LOG_DEBUG("TriangleStrip size: ", size);

      
        for (int x = 0; x < gridSize; ++x)
        {
            for (int y = 0; y < gridSize; ++y)
            {
                newv.x = ((float) x * cellsize) - halfRadius;
                newv.y = ((float) y * cellsize) - halfRadius;
                newv.z = halfRadius;

                newv.normalizeThis();
                Temp_Normals[n++] = newv.x;
                Temp_Normals[n++] = newv.y;
                Temp_Normals[n++] = newv.z;


                Temp_Vertices[v++] = newv.x+0.5f;
                Temp_Vertices[v++] = newv.y+0.5f;
                Temp_Vertices[v++] = 0.0f;

                Temp_TextCoords[t++] = (float) 1.0 - (x * uvcellsize);
                Temp_TextCoords[t++] = (float) y * uvcellsize;
            }
        }
        LOG_DEBUG("TriangleStrip forfor vert: ", size);
        int offset = 0;
        int tempoff = 0;

        int i=0;
        
        for (int a = 0; a < gridSize - 1; a++)
        {
            if (a > 0)
            {
                // Degenerate begin: repeat first vertex
                Temp_Indices[i++] = (mesh_indices_type) ((a * gridSize) + offset);
                tempoff++;
            }

            for (int b = 0; b < gridSize; b++)
            {
                // One part of the strip
                
                Temp_Indices[i++] = (mesh_indices_type) (((a + 1) * gridSize) + b + offset);
                Temp_Indices[i++] = (mesh_indices_type) ((a * gridSize) + b + offset);
                //Temp_Indices[i++] = (mesh_indices_type) (((a + 1) * gridSize) + b + offset);
                tempoff += 2;
            }

            if (a < gridSize)
            {
                // Degenerate end: repeat last vertex
                Temp_Indices[i++] = (mesh_indices_type) ((a * gridSize) + (gridSize - 1) + offset);
                //Temp_Indices[i++] = (mesh_indices_type) (((a + 1) * gridSize) + (gridSize - 1) + offset);
                tempoff++;
            }
        }
        LOG_DEBUG("TriangleStrip forfor int: ", size);
        hudelement->setCustomIndexData(Temp_Indices);
        hudelement->setCustomVertexData(Temp_Vertices);
        hudelement->setCustomUVData(Temp_TextCoords);
        LOG_DEBUG("TriangleStrip forfor out: ", size);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////VRCamera
    ///////////////////////////////////////////////////////////////////////////////////////////////DrawHMD
    
    ShaderDrw* hmdDisplayShader()
    {
        ShaderDrw* HudElementShaderOrthoOffCenter = new ShaderDrw(

        "varying vec2 vTexCoord;\n"
        "varying vec2 vTexCoordPos;\n"
        "uniform vec2 position;\n"
        "uniform vec2 size;\n"
        "void main(void)\n"
        "{\n"
            "vTexCoord = vec2(gl_Vertex.x,1.0- gl_Vertex.y);\n"
            "vTexCoord=gl_TexCoord;"
            "vTexCoordPos=vec2(gl_Vertex.x*size.x,gl_Vertex.y*size.y);\n"
            "mat4 MVP_mat =gl_ProjectionMatrix;\n"
            "gl_Position =MVP_mat*vec4(-vTexCoordPos.xy-position.xy,0.0,1.0);"
        "}\n"

        ,

        "uniform sampler2D hudElement;\n"
        "varying vec2 vTexCoordPos;\n"
        "varying vec2 vTexCoord;\n"
        "void main(void)\n"
        "{\n"
            "vec4 clr = texture2D( hudElement, vTexCoord );\n"
            "clr = clr*vec4(vTexCoordPos.x, vTexCoordPos.y, 0.0, 1.0 );\n"
            "clr.a=1.0; "
            //"clr.gb=vTexCoord;"
            "gl_FragColor = clr;\n"
        "}\n"
        );
        return HudElementShaderOrthoOffCenter;

    }
    
    DrawHMD::DrawHMD()
    {
        m_huds[0].reset(new HudElement(nullptr , 0.5f, 1.0f, 0.0f, 0.0f));
        m_huds[0]->setNeedOwnShader(false);
        m_huds[1].reset(new HudElement(nullptr , 0.5f, 1.0f, 0.5f, 0.0f));
        m_huds[1]->setNeedOwnShader(false);
        m_shader.reset(hmdDisplayShader());
    }
    
    DrawHMD::DrawHMD(int sizeX, int sizeY): DrawHMD()
    {

    }
        
    DrawHMD& DrawHMD::operator=(aex::DrawHMD& b)
    {
        DrawObject::operator=(b);
        m_huds[0]=std::move(b.m_huds[0]);
        m_huds[1]=std::move(b.m_huds[1]);
        m_shader=std::move(b.m_shader);
        
        return *this;
    }
    
    DrawHMD& DrawHMD::operator=(aex::DrawHMD&& b)
    {
        DrawObject::operator=(b);
        m_huds[0]=std::move(b.m_huds[0]);
        m_huds[1]=std::move(b.m_huds[1]);
        m_shader=std::move(b.m_shader);
        return *this;
    }
    
    void DrawHMD::draw(AexRenderer* gl)
    {
        m_shader->draw(gl);
        LOG_DEBUG(" ");
        m_huds[0]->draw(gl);
        m_huds[0]->finishdraw(gl);
        m_huds[1]->draw(gl);
        m_huds[1]->finishdraw(gl);
        m_shader->finishdraw(gl);
    }
    
    void DrawHMD::finishdraw(AexRenderer* gl)
    {
        
    }
    
    void DrawHMD::needsUpdate()
    {
        
    }
    
    void DrawHMD::setTextures(Texture* left, Texture* right)
    {
        m_huds[0]->setTexture(left);
        m_huds[1]->setTexture(right);
    }
    

    ///////////////////////////////////////////////////////////////////////////////////////////////DrawHMD
    ///////////////////////////////////////////////////////////////////////////////////////////////HMDRender
    
#define NPOTS false
    
    HMDRender::HMDRender()
    {
       m_viewport[0]= new FrameBufferOGL(1280, 720, NPOTS,false);
       m_viewport[1]= new FrameBufferOGL(1280, 720, NPOTS,false);
       m_selfdraw.setTextures(m_viewport[0]->getTexture(), m_viewport[1]->getTexture());
       LOG_DEBUG("DrawHMD::draw(AexRenderer* gl", m_viewport[0]->getTextureDepth(), " ", m_viewport[1]->getTextureDepth());
    }
    
    HMDRender::HMDRender(int sizeX, int sizeY)
    {
       m_viewport[0]= new FrameBufferOGL(sizeX, sizeY, NPOTS,false);
       m_viewport[1]= new FrameBufferOGL(sizeX, sizeY, NPOTS,false);
       m_selfdraw.setTextures(m_viewport[0]->getTexture(), m_viewport[1]->getTexture());
       LOG_DEBUG("DrawHMD::draw(AexRenderer* gl", m_viewport[0]->getTextureDepth(), " ", m_viewport[1]->getTextureDepth());
    }
    
    bool HMDRender::init()
    {
        return true;
    }
    
    void HMDRender::drawCurrentEye(AexRenderer* gl)
    {
        m_viewport[m_eye]->draw(gl);
        m_vrCamera->setEye(m_eye);
        gl->setViewProjectionMatrix(m_vrCamera->transforms());
        gl->setViewMatrix(m_vrCamera->get_view_matrix());
    }
    
    void HMDRender::finishDrawCurrentEye(AexRenderer* gl)
    {
        m_viewport[m_eye]->finishdraw(gl);
    }

    void HMDRender::draw(AexRenderer* gl)
    {
        drawCurrentEye(gl);
        m_trackedNodes.front()->drawChildren(gl);
        m_viewport[m_eye]->finishdraw(gl);
        toggleEye();
        drawCurrentEye(gl);
    }

    void HMDRender::finishdraw(AexRenderer* gl)
    {
        m_viewport[m_eye]->finishdraw(gl);
        toggleEye();
    }

    void HMDRender::drawSelf(AexRenderer* gl)
    {
       m_selfdraw.draw(gl);
       m_selfdraw.finishdraw(gl);
    }

    void HMDRender::needsUpdate()
    {
        
    }
    
    void HMDRender::combineTextures()
    {
         
    }
    
    void HMDRender::toggleEye() {
        m_eye = !m_eye;
    }

    VRCamera* HMDRender::getCameraPointer() {
        return m_vrCamera;
    }

    void HMDRender::setVrCamera(VRCamera* vrCamera)
    {
        m_vrCamera = vrCamera;
    }

    VRCamera* HMDRender::getVrCamera() const
    {
        return m_vrCamera;
    }
    
}