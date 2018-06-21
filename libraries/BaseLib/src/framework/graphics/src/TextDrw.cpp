#include <aex/TextDrw.hpp>
#include <aex/TextureManager.hpp>
#include <aex/HudElement.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/VertexData.hpp>


//#include <SDL2/SDL_pixels.h>
#include <sstream>
namespace aex
{

    FontInfo::~FontInfo() {
        //        for (Texture * tex : m_pageTextures) {
        //
        //        }
    }

    FontInfo::FontInfo() { }
    
    
    
    TextString::TextString(const std::string& text, bool batched) :
    m_text(text), 
    m_batched(batched),
    m_positionsVBO(nullptr),
    m_sizesVBO(nullptr),
    m_uVOffsetsVBO(nullptr),
    m_uVSizesVBO(nullptr)
    {
        m_colorModUniform = aex::make_shared<UniformFloat3>(aex::math::Vector3::OneOneOne(),"colorMod");
        m_alphaUniform = aex::make_shared<UniformFloat>(1.0, "alphaMod");
        m_spriteVbo = nullptr;
        m_indexVbo = nullptr;
    }

    TextString::TextString(const std::string& text, float size, bool batched) :
        m_sizeX(size),
        m_sizeY(size),
        m_text(text),
        m_batched(batched),
        m_positionsVBO(nullptr),
        m_sizesVBO(nullptr),
        m_uVOffsetsVBO(nullptr),
        m_uVSizesVBO(nullptr)
    {
        m_colorModUniform = aex::make_shared<UniformFloat3>(aex::math::Vector3::OneOneOne() ,"colorMod");
        m_alphaUniform = aex::make_shared<UniformFloat>(1.0, "alphaMod");
        m_spriteVbo = nullptr;
        m_indexVbo = nullptr;
    }

    TextString::TextString(bool batched ) : 
    m_text(""),
    m_batched(batched),
    m_positionsVBO(nullptr),
    m_sizesVBO(nullptr),
    m_uVOffsetsVBO(nullptr),
    m_uVSizesVBO(nullptr)
    {
        m_colorModUniform = aex::make_shared<UniformFloat3>(aex::math::Vector3::OneOneOne(),"colorMod");
        m_alphaUniform = aex::make_shared<UniformFloat>(1.0, "alphaMod");
        m_spriteVbo = nullptr;
        m_indexVbo = nullptr;
    }
    
    TextString::~TextString()
    {
        delete m_spriteVbo;
        delete m_indexVbo;
        delete m_positionsVBO;
        delete m_sizesVBO;
        delete m_uVOffsetsVBO;
        delete m_uVSizesVBO;

        m_spriteVbo = nullptr;
        m_indexVbo = nullptr;
        m_positionsVBO = nullptr;
        m_sizesVBO = nullptr;
        m_uVOffsetsVBO = nullptr;
        m_uVSizesVBO = nullptr;
     }

    void TextString::clearBatches() 
    {
        m_spriteBatch.clear();
        m_indexBatch.clear();
        m_PositionsBatch.clear();
        m_SizesBatch.clear();
        m_UVOffsetsBatch.clear();
        m_UVSizesBatch.clear();
    }
    
    
    
    void TextString::draw(AexRenderer* gl)
    {
        const ShaderDrw* shader = gl->getShader();
        if (shader)
        {
            const AexShader& program = shader->GetProgram();
            
            if (m_spriteVbo)
            {
                program->setVertexData(*m_spriteVbo);
            }
            
            if (m_indexVbo)
            {
                program->setIndices(*m_indexVbo);
            }
            
            if(m_positionsVBO)
            {
                program->setVertexData(*m_positionsVBO);
            }
            
            if(m_sizesVBO)
            {
                program->setVertexData(*m_sizesVBO);
            }
            
            if(m_uVOffsetsVBO)
            {
                program->setVertexData(*m_uVOffsetsVBO);
            }
            
            if(m_uVSizesVBO)
            {
                program->setVertexData(*m_uVSizesVBO);
            }
            
            m_colorModUniform->setUniform(program);
            m_alphaUniform->setUniform(program);
            program->draw();
        }
    }
    
    void TextString::finishdraw(AexRenderer* gl)
    {
        
        const ShaderDrw* shader = gl->getShader();
        
        if (shader)
        {
            // TODO - maeybe set it just in TextString destructor
            const AexShader& program = shader->GetProgram();
            if (m_spriteVbo)
            {
                program->releaseVertexData(*m_spriteVbo);
            }
            
            if (m_positionsVBO)
            {
                
                program->releaseVertexData(*m_positionsVBO);
            }
            if (m_sizesVBO)
            {
                
                program->releaseVertexData(*m_sizesVBO);
            }
            if (m_uVOffsetsVBO)
            {
                
                program->releaseVertexData(*m_uVOffsetsVBO);
            }
            if (m_uVSizesVBO)
            {
                
                program->releaseVertexData(*m_uVSizesVBO);
            }
        }
        
        HudElement::finishdraw(gl);
    }
        
   
    void TextString::setText(std::string text)
    {
        m_text = text;
        m_textChanged = true;
    }

    std::string TextString::getText()
    {
        return m_text;
    }

    void TextString::setSizeY(float sizeY)
    {
        m_sizeY = sizeY;
    }

    float TextString::getSizeY() const
    {
        return m_sizeY;
    }

    void TextString::setSizeX(float sizeX)
    {
        m_sizeX = sizeX;
    }

    float TextString::getSizeX() const
    {
        return m_sizeX;
    }
    
    float TextString::getWidth() const
    {
        return m_width;
    }
    
    void TextString::setPosition(float x, float y)
    {
        aex::math::Vector2 position=  aex::math::Vector2(x,y);
        
        if(m_glyphs.size() == 0){
            m_position = position;
            setTextChanged(true);
            return;
        }
        
        if(!m_batched){
            float deltaX = position.x - m_position.x;
            float deltaY = position.y - m_position.y;
            m_position = position;
            for(HudElementPtr& glyph : m_glyphs)
            {
                glyph->setPosX(glyph->getPosX()+deltaX);
                glyph->setPosY(glyph->getPosY()+deltaY);
            }
        }else{
            m_position = position;
            setTextChanged(true);
        }
    }

    void TextString::setPosition(aex::math::Vector2 position)
    {
        setPosition(position.x, position.y);
    }

    void TextString::addPosition(float x, float y)
    {
        setPosition(m_position+ aex::math::Vector2(x,y));
                
    }
    
    void TextString::addPosition(aex::math::Vector2 position)
    {
        addPosition(position.x, position.y);
    }
    

    aex::math::Vector2 TextString::getPosition() const
    {
        return m_position;
    }

    void TextString::setTextChanged(bool textChanged)
    {
        m_textChanged = textChanged;
    }

    bool TextString::isTextChanged() const
    {
        return m_textChanged;
    }

    /*
    void TextString::setVisible(bool visible)
    {
        m_isVisible = visible;
    }

    bool TextString::isVisible() const
    {
        return m_isVisible;
    }*/

    void TextString::setGlyphs(std::vector<HudElementPtr> glyphs)
    {
        m_glyphs = glyphs;
    }
    
    std::vector<HudElementPtr>* TextString::getGlyphs()
    {
        return &m_glyphs;
    }
    
    void TextString::setWidth(float width)
    {
        m_width = width;
    }

    TextDrw::TextDrw(std::string font_name, int ptsize, std::string text, bool batched) : Hud::Hud(),
    m_text(text),
    m_textChanged(false),
    m_scale(1.0f, 1.0f),
    m_batched(batched){
        /*if(TTF_WasInit()==0)
        {
            TTF_Init();
            atexit(TTF_Quit);
        }
         m_font = TTF_OpenFont(font_name.c_str(), ptsize);*/ }

    TextDrw::~TextDrw() {
        /* if (m_textureAtlas)
             TextureManager::GetInstance().ReleaseTexture(m_textureAtlas);

        //
        //        GL_CHECK(glDeleteProgram(m_shader.GetProgramNumber()));
        //
        //        if (m_attribLocCache != -1)
        //            GL_CHECK(glDisableVertexAttribArray(m_attribLocCache));
        //        GL_CHECK(glDeleteBuffers(1, &m_spriteVbo));
        //        GL_CHECK(glDeleteBuffers(1, &m_indexVbo));*/ }

    void TextDrw::renderFont(std::string fontName)
    {
        /*if(TTF_WasInit()==0)
        {
            TTF_Init();
            atexit(TTF_Quit);
        }
         m_font = TTF_OpenFont(font_name.c_str(), ptsize);*/

        for (int x = 0; x < 16; x++)
        {
            for (int y = 0; y < 16; y++)
            {
                //in grid  cell xy render char (x*16) +y;
            }
        }

        /* if(m_font)
        {
            TTF_CloseFont(m_font);
        }*/
    }

    void TextDrw::move(float x, float y)
    {
        for (auto& txtstring : m_renderStrings)
        {
            txtstring->addPosition(x, y);
        }

        for (Hud_ptr& layer : m_HudLayers)
        {
            layer->move(x, y);
        }

        /*if (m_NextLayer)
        {
            m_NextLayer->move(x, y);
        }*/
    }

    void TextDrw::setText(std::string text)
    {
        m_textChanged = true;
        m_text = text;
    }

    std::string TextDrw::getText() const
    {
        return m_text;
    }

    TextString_ptr TextDrw::newString(std::string text)
    {
        TextString_ptr ret = aex::make_shared<TextString>(text, m_batched);
        m_renderStrings.push_back(ret);
        return ret;
    }

    TextString_ptr TextDrw::newString(std::string text, float size)
    {
        TextString_ptr ret = aex::make_shared<TextString>(text, size, m_batched);
        m_renderStrings.push_back(ret);
        return ret;
    }

    TextDrw::TextDrw(Texture* tex, std::string text,bool batched) : Hud::Hud(tex),
    m_text(text),
    m_textChanged(true),
    m_scale(1.0f, 1.0f),
    m_batched(batched){
 }

    TextDrw::TextDrw(Texture* tex, bool batched) : Hud::Hud(tex),
    m_text(""),
    m_textChanged(true),
    m_scale(1.0f, 1.0f),
    m_batched(batched){
 }
    
    static const std::vector<float> SimpleHudElementVertCornerRoot = {
        {
            0.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0
        }
    };
    static const std::vector<uint16_t> SimpleHudElementIndexTriangleStrip = {
        {1, 2, 0, 3}
    };

    void TextDrw::draw(AexRenderer* gl)
    {

        m_drawed = false;
        for (auto& txtstring : m_renderStrings)
        {
            
            if (txtstring->isVisible())
            {
                
                if (txtstring->isTextChanged())
                {
                    int lastChar = 0;

                    float tempX = gl->getQuatCamera()->getWindowW();
                    float tempY = gl->getQuatCamera()->getWindowH();

                    txtstring->m_glyphs.erase(txtstring->m_glyphs.begin(), txtstring->m_glyphs.end());
                    float stringadwance = 0.0f;
                    float lineAdwance = 0.0f;
                    float maxAdvance = 0.0f;

                    float sizeX = txtstring->getSizeX();
                    float sizeY = txtstring->getSizeY();

                    std::string stringCopy = txtstring->getText();
                    const char* temp = stringCopy.c_str();

                    int c = 0;
                    for (size_t i = 0; i < txtstring->getText().size(); i++)
                    {
                        int num = (int) temp[i];

                        if (m_glyphs.find(num) != m_glyphs.end())
                        {
                            Glyph glyph = m_glyphs[num];
                            if (glyph.m_kerningMap.find(lastChar) != glyph.m_kerningMap.end())
                            {
                                //float kerning = glyph.m_kerningMap[lastChar];
                                stringadwance += (glyph.m_kerningMap[lastChar] / tempX) * m_scaleX;
                            }
                            
                            if( ! txtstring->m_batched){
                            // txtstring->m_glyphs.push_back(HudElement::makeHudElement("", (num % 16)*32, (num / 16)*32, 26, 32, txtstring->GetSizeX()/tempX, txtstring->GetSizeY()/tempY, i * (txtstring->GetSizeX()/tempX), 0.0));
                            txtstring->m_glyphs.push_back(HudElement::makeHudElement("",
                                    glyph.m_x,
                                    glyph.m_y,
                                    glyph.m_width,
                                    glyph.m_height,
                                    ((glyph.m_width / tempX) * sizeX) * m_scaleX,
                                    ((glyph.m_height / tempY) * sizeY) * m_scaleY,
                                    ((stringadwance + ((glyph.m_xoffset / tempX) * sizeX)) + txtstring->GetPosition().x) * m_scaleX,
                                    (((glyph.m_yoffset / tempY) * sizeY) + txtstring->GetPosition().y + lineAdwance) * m_scaleY
                                    ));
                            
                            }else{
                                txtstring->m_spriteBatch.insert(txtstring->m_spriteBatch.end(), SimpleHudElementVertCornerRoot.begin(), SimpleHudElementVertCornerRoot.end());
                                if(i != 0){
                                    txtstring->m_indexBatch.push_back(SimpleHudElementIndexTriangleStrip[0]+(i-c)*4);
                                    txtstring->m_indexBatch.push_back(SimpleHudElementIndexTriangleStrip[0]+(i-c)*4);
                                }
                                for(size_t j = 0 ; j < SimpleHudElementIndexTriangleStrip.size() ; j++){
                                    txtstring->m_indexBatch.push_back(SimpleHudElementIndexTriangleStrip[j]+(i-c)*4);
                                }
                                txtstring->m_indexBatch.push_back(SimpleHudElementIndexTriangleStrip[3]+(i-c)*4);
                                txtstring->m_indexBatch.push_back(SimpleHudElementIndexTriangleStrip[3]+(i-c)*4);
//                                LOG_DEBUG("Putting: ", (char)num );
                                for(int k = 0 ; k < 4 ; k++){
                                    txtstring->m_UVOffsetsBatch.push_back(glyph.m_x);
                                    txtstring->m_UVOffsetsBatch.push_back(glyph.m_y);
                                    txtstring->m_UVSizesBatch.push_back(glyph.m_width);
                                    txtstring->m_UVSizesBatch.push_back(glyph.m_height);
                                    txtstring->m_PositionsBatch.push_back(((stringadwance + ((glyph.m_xoffset / tempX) * sizeX)) + txtstring->GetPosition().x) * m_scaleX);
                                    txtstring->m_PositionsBatch.push_back((((glyph.m_yoffset / tempY) * sizeY) + txtstring->GetPosition().y + lineAdwance) * m_scaleY);
                                    txtstring->m_SizesBatch.push_back(((glyph.m_width / tempX) * sizeX) * m_scaleX);
                                    txtstring->m_SizesBatch.push_back(((glyph.m_height / tempY) * sizeY) * m_scaleY);
                                }
                            }
                            
                            stringadwance += ((glyph.m_xadvance / tempX) * sizeX);
                        } else
                        {
                            if (num == '\n')
                            {
                                maxAdvance = std::max(stringadwance, maxAdvance);
                                (lineAdwance += (m_fontInfo.m_lineHeight / tempY) * sizeY);
                                stringadwance = 0.0f;
                                c++;
                            } else if (num == '\t')
                            {
                                (stringadwance += ((m_glyphs[' '].m_xadvance * 4) / tempX) * sizeX);
                            }
                        }

                        lastChar = num; //for kerning
                    }
                    
                    maxAdvance = std::max(stringadwance, maxAdvance);
                    txtstring->setWidth(maxAdvance);
                    txtstring->setTextChanged(false);
                    
                    if (txtstring->m_batched)
                    {   
                        delete txtstring->m_spriteVbo;
                        delete txtstring->m_indexVbo;
                        delete txtstring->m_positionsVBO;
                        delete txtstring->m_sizesVBO;
                        delete txtstring->m_uVOffsetsVBO;
                        delete txtstring->m_uVSizesVBO;
                        
//                        LOG_DEBUG(txtstring->m_spriteBatch.size());
//                        LOG_DEBUG(txtstring->m_indexBatch.size());
//                        LOG_DEBUG(txtstring->m_PositionsBatch.size());
//                        LOG_DEBUG(txtstring->m_SizesBatch.size());
//                        LOG_DEBUG(txtstring->m_UVOffsetsBatch.size());
//                        LOG_DEBUG(txtstring->m_UVSizesBatch.size());
   
                        txtstring->m_spriteVbo = VertexData::createVertexData(txtstring->m_spriteBatch, "gl_Vertex", 3);
                        txtstring->m_indexVbo = new IndexData(IndexData::createTraingleStrip(txtstring->m_indexBatch));
                        txtstring->m_positionsVBO = VertexData::createVertexData(txtstring->m_PositionsBatch, "position", 2);
                        txtstring->m_sizesVBO = VertexData::createVertexData(txtstring->m_SizesBatch, "size", 2);
                        txtstring->m_uVOffsetsVBO = VertexData::createVertexData(txtstring->m_UVOffsetsBatch, "UvOffset", 2);
                        txtstring->m_uVSizesVBO = VertexData::createVertexData(txtstring->m_UVSizesBatch, "UvSize", 2);
                            
                        txtstring->m_glyphs.push_back(txtstring); // draw self (have batched code))
                        
                        txtstring->clearBatches();
                    }
                    //LOG_DEBUG("Text String Rebuilded !!!: ", txtstring->GetText());
                }
                // here because there can be many txtStrings in TextDrw
                m_hudElementsDrawPtr = &txtstring->m_glyphs;
                Hud::draw(gl);
                m_drawed = true;
            }
        }
    }

    void TextDrw::finishdraw(AexRenderer* gl)
    {
        if(m_drawed)
            Hud::finishdraw(gl);
    }

    void TextDrw::readGlypsFromLittera(std::string filename)
    {
        FileManager fManager;
        // aex::ifstream fileHandle;
        auto fileHandle = fManager.open_asset(filename);

        //define file stream object, and open the file

        if (fileHandle)
        {

            std::string line = "";

            while (getline(fileHandle, line))
            {
                // std::cout<<line<<std::endl;

                std::stringstream spliter(line);
                std::string token;
                std::vector<std::string> tokens;

                while (getline(spliter, token, ':'))
                {
                    //LOG_DEBUG(token);
                    tokens.push_back(token);
                }

                if (tokens.size() == 8)
                {
                    //std::cout << "addin hud element" << std::endl;
                    Glyph newglyph(
                            (int) std::atoi(tokens[0].c_str()),
                            (float) std::atof(tokens[1].c_str()),
                            (float) std::atof(tokens[2].c_str()),
                            (float) std::atof(tokens[3].c_str()),
                            (float) std::atof(tokens[4].c_str()),
                            (float) std::atof(tokens[5].c_str()),
                            (float) std::atof(tokens[6].c_str()),
                            (float) std::atof(tokens[7].c_str()));
                    m_glyphs.insert(std::pair<int, Glyph>((int) std::atoi(tokens[0].c_str()), newglyph));
                }
            }
        }

        fileHandle.close();
    }

    void TextDrw::readGlyphsFromLitteraFull(std::string filename)
    {
        FileManager fManager;
        auto fileHandle = fManager.open_asset(filename);

        if (fileHandle)
        {

            std::string line = "";

            while (getline(fileHandle, line))
            {
                // std::cout<<line<<std::endl;

                std::stringstream spliter(line);
                std::string token;
                std::vector<std::string> tokens;

                while (getline(spliter, token, ' '))
                {
                    //LOG_DEBUG(token);
                    tokens.push_back(token);
                }

                if (tokens[0] == "info")
                {
                    for (std::string itoken : tokens)
                    {
                        std::stringstream tokensplitter(itoken);
                        std::string innertoken;
                        std::vector<std::string> innertokens;

                        while (getline(tokensplitter, innertoken, '='))
                        {
                            innertokens.push_back(innertoken);
                        }

                        if (innertokens[0] == "face")
                        {
                            m_fontInfo.m_fontaname = innertokens[1];
                        }

                        if (innertokens[0] == "size")
                        {
                            m_fontInfo.m_size = atoi(innertokens[1].c_str());
                        }

                    }
                } else if (tokens[0] == "common")
                {
                    for (std::string itoken : tokens)
                    {
                        std::stringstream tokensplitter(itoken);
                        std::string innertoken;
                        std::vector<std::string> innertokens;

                        while (getline(tokensplitter, innertoken, '='))
                        {
                            innertokens.push_back(innertoken);
                        }

                        if (innertokens[0] == "lineHeight")
                        {
                            m_fontInfo.m_lineHeight = (float) atof(innertokens[1].c_str());
                        } else if (innertokens[0] == "scaleW")
                        {
                            m_fontInfo.m_texSizeW = (float) atof(innertokens[1].c_str());
                        } else if (innertokens[0] == "scaleH")
                        {
                            m_fontInfo.m_texSizeH = (float) atof(innertokens[1].c_str());
                        } else if (innertokens[0] == "pages")
                        {
                            m_fontInfo.m_pagesNumber = (int) atoi(innertokens[1].c_str());
                        } else if (innertokens[0] == "packed")
                        {

                        }

                    }
                } else if (tokens[0] == "page")
                {
                    for (std::string itoken : tokens)
                    {
                        std::stringstream tokensplitter(itoken);
                        std::string innertoken;
                        std::vector<std::string> innertokens;

                        while (getline(tokensplitter, innertoken, '='))
                        {
                            innertokens.push_back(innertoken);
                        }

                        if (innertokens[0] == "id")
                        {
                            // newGlyph.m_glyphID = atoi(innertokens[1].c_str());
                        } else if (innertokens[0] == "x")
                        {
                            // newGlyph.m_x = (float) atof(innertokens[1].c_str());
                        }
                    }
                } else if (tokens[0] == "char")
                {

                    Glyph newGlyph;

                    for (std::string itoken : tokens)
                    {
                        std::stringstream tokensplitter(itoken);
                        std::string innertoken;
                        std::vector<std::string> innertokens;

                        while (getline(tokensplitter, innertoken, '='))
                        {
                            innertokens.push_back(innertoken);
                        }

                        if (innertokens[0] == "id")
                        {
                            newGlyph.m_glyphID = atoi(innertokens[1].c_str());
                        } else if (innertokens[0] == "x")
                        {
                            newGlyph.m_x = (float) atof(innertokens[1].c_str());
                        } else if (innertokens[0] == "y")
                        {
                            newGlyph.m_y = (float) atof(innertokens[1].c_str());
                        } else if (innertokens[0] == "width")
                        {
                            newGlyph.m_width = (float) atof(innertokens[1].c_str());
                        } else if (innertokens[0] == "height")
                        {
                            newGlyph.m_height = (float) atof(innertokens[1].c_str());
                        } else if (innertokens[0] == "xoffset")
                        {
                            newGlyph.m_xoffset = (float) atof(innertokens[1].c_str());
                        } else if (innertokens[0] == "yoffset")
                        {
                            newGlyph.m_yoffset = (float) atof(innertokens[1].c_str());
                        } else if (innertokens[0] == "xadvance")
                        {
                            newGlyph.m_xadvance = (float) atof(innertokens[1].c_str());
                        } else if (innertokens[0] == "page")
                        {
                            newGlyph.m_page = (float) atoi(innertokens[1].c_str());
                        }

                    }

                    m_glyphs.insert(std::pair<int, Glyph>(newGlyph.m_glyphID, newGlyph));
                } else if (tokens[0] == "kerning")
                {
                    int first = 0;
                    int second = 0;
                    float amount = 0.0f;

                    for (std::string itoken : tokens)
                    {

                        std::stringstream tokensplitter(itoken);
                        std::string innertoken;
                        std::vector<std::string> innertokens;

                        while (getline(tokensplitter, innertoken, '='))
                        {
                            innertokens.push_back(innertoken);
                        }

                        if (innertokens[0] == "first")
                        {
                            first = atoi(innertokens[1].c_str());
                        } else if (innertokens[0] == "second")
                        {
                            second = atoi(innertokens[1].c_str());
                        } else if (innertokens[0] == "amount")
                        {
                            amount = atof(innertokens[1].c_str());
                        }

                    }

                    if (m_glyphs.find(second) != m_glyphs.end())
                    {
                        m_glyphs[second].m_kerningMap.insert(std::pair<int, float>(first, amount));
                    }

                }
            }
        }

        fileHandle.close();
    }

    void TextDrw::setKerningON(bool kerningON)
    {
        m_kerningON = kerningON;
    }

    bool TextDrw::isKerningON() const
    {
        return m_kerningON;
    }
    
    void TextDrw::clear(){
        m_renderStrings.clear();
    }
    
    void TextDrw::removeString(TextString_ptr textString){
        auto it = std::find(m_renderStrings.begin(), m_renderStrings.end(), textString);
        
        if(*it){
            if (it != m_renderStrings.end())
            {
                std::swap(*it, m_renderStrings.back());
                m_renderStrings.pop_back();
            }
        }
    }
    
    const FontInfo& TextDrw::getFontInfo() const
    {
        return m_fontInfo;
    }
    
    void TextDrw::calculateWidth(TextString_ptr m_renderString, unsigned windowWidthPix, unsigned windowHeightPix)
    {
        bool finded = false;
        for(size_t i = 0 ; i < m_renderStrings.size() ; i++)
        {
            if(m_renderString == m_renderStrings[i])
            {
                finded = true;
                break;
            }
        }
        
        if(!finded)
            return;
        
        int lastChar = 0;

        float tempX = windowWidthPix;
        float tempY = windowHeightPix;

        float stringadwance = 0.0f;
        float lineAdwance = 0.0f;
        float maxAdvance = 0.0f;

        float sizeX = m_renderString->getSizeX();
        float sizeY = m_renderString->getSizeY();

        std::string stringCopy = m_renderString->getText();
        const char* temp = stringCopy.c_str();

        for (size_t i = 0; i < m_renderString->getText().size(); i++)
        {
            int num = (int) temp[i];

            if (m_glyphs.find(num) != m_glyphs.end())
            {
                Glyph glyph = m_glyphs[num];
                if (glyph.m_kerningMap.find(lastChar) != glyph.m_kerningMap.end())
                {
                    stringadwance += (glyph.m_kerningMap[lastChar] / tempX) * m_scaleX;
                }
                stringadwance += ((glyph.m_xadvance / tempX) * sizeX);
            } else
            {
                if (num == '\n')
                {
                    maxAdvance = std::max(stringadwance, maxAdvance);
                    (lineAdwance += (m_fontInfo.m_lineHeight / tempY) * sizeY);
                    stringadwance = 0.0f;
                } else if (num == '\t')
                {
                    (stringadwance += ((m_glyphs[' '].m_xadvance * 4) / tempX) * sizeX);
                }
            }
            lastChar = num;
        }

        maxAdvance = std::max(stringadwance, maxAdvance);
        m_renderString->setWidth(maxAdvance);
    }
    
    float TextDrw::getPositionForCursor(TextString_ptr m_renderString, unsigned windowWidthPix, unsigned windowHeightPix, uint cursor)
    {
        if(cursor <= 0)
            return 0.0f;
        
        bool finded = false;
        for(size_t i = 0 ; i < m_renderStrings.size() ; i++)
        {
            if(m_renderString == m_renderStrings[i])
            {
                finded = true;
                break;
            }
        }
        
        if(!finded)
            return 0;
        
        int lastChar = 0;

        float tempX = windowWidthPix;
        float tempY = windowHeightPix;

        float stringadwance = 0.0f;
        float lineAdwance = 0.0f;
        float maxAdvance = 0.0f;

        float sizeX = m_renderString->getSizeX();
        float sizeY = m_renderString->getSizeY();

        std::string stringCopy = m_renderString->getText();
        const char* temp = stringCopy.c_str();

        for (size_t i = 0; i < m_renderString->getText().size(); i++)
        {
            int num = (int) temp[i];

            if (m_glyphs.find(num) != m_glyphs.end())
            {
                Glyph glyph = m_glyphs[num];
                if (glyph.m_kerningMap.find(lastChar) != glyph.m_kerningMap.end())
                {
                    stringadwance += (glyph.m_kerningMap[lastChar] / tempX) * m_scaleX;
                }
                stringadwance += ((glyph.m_xadvance / tempX) * sizeX);
            } else
            {
                if (num == '\n')
                {
                    maxAdvance = std::max(stringadwance, maxAdvance);
                    (lineAdwance += (m_fontInfo.m_lineHeight / tempY) * sizeY);
                    stringadwance = 0.0f;
                } else if (num == '\t')
                {
                    (stringadwance += ((m_glyphs[' '].m_xadvance * 4) / tempX) * sizeX);
                }
            }
            lastChar = num;
            if(i == (cursor-1))
            {
                return stringadwance;
            }
        }
        
        maxAdvance = std::max(stringadwance, maxAdvance);
        return maxAdvance;
    }
    
    int TextDrw::getCursorForPosition(TextString_ptr m_renderString, unsigned windowWidthPix, unsigned windowHeightPix, float pos)
    {
        bool finded = false;
        for(size_t i = 0 ; i < m_renderStrings.size() ; i++)
        {
            if(m_renderString == m_renderStrings[i])
            {
                finded = true;
                break;
            }
        }
        
        if(!finded)
            return 0;
        
        int lastChar = 0;

        float tempX = windowWidthPix;
        float tempY = windowHeightPix;

        float stringadwance = 0.0f;
        float lineAdwance = 0.0f;

        float sizeX = m_renderString->getSizeX();
        float sizeY = m_renderString->getSizeY();

        std::string stringCopy = m_renderString->getText();
        const char* temp = stringCopy.c_str();

        for (size_t i = 0; i < m_renderString->getText().size(); i++)
        {
            int num = (int) temp[i];

            if (m_glyphs.find(num) != m_glyphs.end())
            {
                Glyph glyph = m_glyphs[num];
                if (glyph.m_kerningMap.find(lastChar) != glyph.m_kerningMap.end())
                {
                    stringadwance += (glyph.m_kerningMap[lastChar] / tempX) * m_scaleX;
                }
                stringadwance += ((glyph.m_xadvance / tempX) * sizeX);
            } else
            {
                if (num == '\n')
                {
                    (lineAdwance += (m_fontInfo.m_lineHeight / tempY) * sizeY);
                    stringadwance = 0.0f;
                } else if (num == '\t')
                {
                    (stringadwance += ((m_glyphs[' '].m_xadvance * 4) / tempX) * sizeX);
                }
            }
            lastChar = num;
            if(stringadwance > pos)
                return i;
        }
        
        return m_renderString->getText().size();

    }
    
    aex::ShaderDrw* TextDrw::createShader()
    {
        if(m_batched)
        {
            return ShaderDrw::BasicBatchedTextStringAtlas();
        }else{
            return Hud::createShader();
        }
    }
    void TextDrw::releaseShader(aex::ShaderDrw* s)
    {
        if(m_batched)
        {
            ShaderDrw::FreeBasicBatchedTextStringAtlas();
        }else{
            Hud::releaseShader(s);
        }
    }

    void TextDrw::needsUpdate() 
    {
    }

    /* void TextDrw::scale(float sx, float sy)
     {
         m_scale.m_x= sx;
         m_scale.m_y= sy;
     }*/
}
