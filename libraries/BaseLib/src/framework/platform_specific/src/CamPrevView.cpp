#include "../include/CamPrevView.hpp"
#include "../include/CamPrevViewShaders.hpp"

#include <sstream>
#include <memory>

namespace aex
{
    namespace cam
    {

        BasePreviewView::BasePreviewView(PreviewSource& src) :
        PreviewView(src)
        {
            shaderPtr = nullptr;
            LOG_DEBUG("preview size:", src.getPreviewSize());
        }

        BasePreviewView::~BasePreviewView()
        {
            if (shaderPtr != nullptr)
            {
                delete shaderPtr;
                shaderPtr = nullptr;
            }
        }

        void BasePreviewView::draw(AexRenderer* gl)
        {
            if (!isValid())
                return;

            lazyInit();
            shaderPtr->draw(gl);
            const AexShader& program = shaderPtr->GetProgram();

            for (auto& t : texturesPtrs)
            {
                t->bind();
            }
            for (auto& t : ptrTexturesPtrs)
            {
                (*t)->bind();
            }

            for (auto& u : uniformsPtrs)
            {
                u->setUniform(program);
            }
            for (auto& vd : vertexDataPtrs)
            {
                program->setVertexData(**vd);
            }
            for (auto& id : indexDataPtrs)
            {
                program->setIndices(*id);
            }

            program->draw();
        }

        void BasePreviewView::finishdraw(AexRenderer* gl)
        {
            if (!isValid())
                return;

            const AexShader& program = shaderPtr->GetProgram();
            shaderPtr->finishdraw(gl);

            for(int i = ptrTexturesPtrs.size()-1 ; i >= 0 ; i--)
            {
                auto& t = ptrTexturesPtrs[i];
                (*t)->unbind();
            }

            for(int i = texturesPtrs.size()-1 ; i>= 0 ; i--)
            {
                auto& t = texturesPtrs[i];
                t->unbind();
            }

            for (auto& vd : vertexDataPtrs)
            {
                program->releaseVertexData(**vd);
            }
        }

        void BasePreviewView::needsUpdate()
        {
        }

        void BasePreviewView::lazyInit()
        {
            if (shaderPtr == nullptr)
            {
                shaderPtr = new ShaderDrw(getVertexShaderSource(), getFragmentShaderSource(), "attribute vec4 aPosition; \n attribute vec2 aTexCoord;\n");
                #ifdef BUILD_FOR_ANDROID 
                shaderPtr->addExtension("GL_OES_EGL_image_external", "require");
                #endif
            }
        }

        void BasePreviewView::connectUniform(Uniform* uniform)
        {
            uniformsPtrs.push_back(uniform);
        }

        void BasePreviewView::connectTexture(Texture* texture)
        {
            texturesPtrs.push_back(texture);
        }

        void BasePreviewView::connectTexture(Texture** texture)
        {
            ptrTexturesPtrs.push_back(texture);
        }

        void BasePreviewView::connectVertexData(std::unique_ptr<VertexData>* vertexData)
        {
            vertexDataPtrs.push_back(vertexData);
        }

        void BasePreviewView::connectIndexData(IndexData* indexData)
        {
            indexDataPtrs.push_back(indexData);
        }

        std::string BasePreviewView::getShaderDefines() const
        {
            std::string retStr = "";
//#if   __TARGET_PLATFORM__ == __ANDROID_OS__
//            retStr = "#define ANDROID\n";
//#endif
//#if   __TARGET_PLATFORM__ == __APPLE_IOS__
//            retStr = "#define IOS\n";
//#endif
            return retStr;
        }

        std::string BasePreviewView::generateShaderWithSrc(const char* src) const
        {
            std::stringstream ss;
            ss << getShaderDefines() << src;
//            LOG_DEBUG("Poskladano:\n ", ss.str());
            return ss.str();
        }

        SimplePreviewView::SimplePreviewView(PreviewSource& src) :
        BasePreviewView(src),
#if   __TARGET_PLATFORM__ == __ANDROID_OS__
        textureOES(src.getTextureOES()),
        uTexOESSampler(textureOES, "textureOES"),
#endif
#if   __TARGET_PLATFORM__ == __APPLE_IOS__
        textureY(src.getTextureY()),
        textureUV(src.getTextureUV()),
        uTexYSampler(textureY, "textureY"),
        uTexUVSampler(textureUV, "textureUV"),
#endif
        uUVTransMatrix
        {
            std::array<float, 16>(), "uTextureTransform"
        }

        ,
        vecUV(std::vector<float>{0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f}),
        // vecVertexData; set in renderIn method
        vecDrawOrder(std::vector<uint16_t>{3, 0, 2, 1}),
        vboUV
        {
            VertexData::createVertexData(vecUV, "aTexCoord", 2)
        },
        //vboVertexData; // set in renderIn method
        vboDrawOrder{IndexData::createTraingleStrip(vecDrawOrder)}
        {
#if   __TARGET_PLATFORM__ == __ANDROID_OS__
            connectUniform(&uTexOESSampler);
            connectTexture(textureOES);
#endif
#if   __TARGET_PLATFORM__ == __APPLE_IOS__
            connectUniform(&uTexYSampler);
            connectUniform(&uTexUVSampler);
            connectTexture(textureY);
            connectTexture(textureUV);
#endif
            connectUniform(&uUVTransMatrix);
            connectVertexData(&vboVertexData);
            connectVertexData(&vboUV);
            connectIndexData(&vboDrawOrder);
        }

        void SimplePreviewView::renderIn(const Size& frameSize, const Size& componetSize, int posX, int posY, bool fullFill)
        {
            bool needRecalc = false;
            if (lastWindowSize != frameSize)
            {
                lastWindowSize = frameSize;
                needRecalc = true;
            }else
            if (lastHudElemSize != componetSize)
            {
                lastHudElemSize = componetSize;
                needRecalc = true;
            }else
            if( (lastPosX != posX) || (lastPosY != posY) )
            {
                lastPosX = posX;
                lastPosY = posY;
                needRecalc = true;
            }else
                if(lastWindowSize.getWidth() == 0)
                {
                    needRecalc = true;
                }

            // calculate vertex for triangles of view
            if (needRecalc)
            {
                if (!m_bIsFBOTextureRenderer)
                {
                    vecVertexData = genVertexData(lastWindowSize, componetSize, posX, posY, fullFill);
                }
                else
                {
                    vecVertexData = std::vector<float>{
                        -1.0f, 1.0f, 0.0f, // left top
                        -1.0f, -1.0f, 0.0f, // left bottom
                        1.0f, -1.0f, 0.0f, // right bottom
                        1.0f, 1.0f, 0.0f // right top
                    }; // full fill of texture for FBO
                }
                vboVertexData.reset(VertexData::createVertexData(vecVertexData, "aPosition", 3));
            }
        }

        void SimplePreviewView::newUVMatrixCallback(RotationMatrix m)
        {
            uUVTransMatrix.setValue(m.getMatrixArray()); // update matrix
        }

        void SimplePreviewView::newPreviewFrameCallback(PreviewSource& src)
        {
        }

        std::string SimplePreviewView::getVertexShaderSource() const
        {
            return generateShaderWithSrc(SimplePreviewViewVS);
        }

        std::string SimplePreviewView::getFragmentShaderSource() const
        {
            return generateShaderWithSrc(SimplePreviewViewFS);
        }



        /* +++++++++ Noctovision Preview View ++++++++++ */
        NoctovisionPreviewView::NoctovisionPreviewView(PreviewSource& src) :
        SimplePreviewView(src)
        {
        }

        std::string NoctovisionPreviewView::getVertexShaderSource() const
        {
            return SimplePreviewView::getVertexShaderSource(); // same like in SimplePreviewView
        }

        std::string NoctovisionPreviewView::getFragmentShaderSource() const
        {
            return generateShaderWithSrc(NoctovisionPreviewViewFS);
        }

        /* +++++++++ HRM Preview View ++++++++++ */
        HRMPreviewView::HRMPreviewView(PreviewSource& src) :
        SimplePreviewView(src)
        {
        }
        std::string HRMPreviewView::getVertexShaderSource() const
        {
            return SimplePreviewView::getVertexShaderSource();
        }

        std::string HRMPreviewView::getFragmentShaderSource() const
        {
            return generateShaderWithSrc(HRMPreviewViewFS);
        }


        /* ++++++++++++++ Sharp1 Preview View ++++++++++++++*/
        FilterPreviewView::FilterPreviewView(PreviewSource& src, std::array<float, 9> kernel) :
        SimplePreviewView(src),
        uPreviewWidth
        {
            (float) src.getPreviewSize().getWidth(), "uWidth"
        },
        uPreviewHeight{(float) src.getPreviewSize().getHeight(), "uHeight"},
        uKernel{kernel, "uKernel"},
        uKernelDivisor{sumOfKernelElements(kernel), "uKernelDivisor"}
        {
            connectUniform(&uPreviewWidth);
            connectUniform(&uPreviewHeight);
            connectUniform(&uKernel);
            connectUniform(&uKernelDivisor);
        }

        std::string FilterPreviewView::getVertexShaderSource() const
        {
            return SimplePreviewView::getVertexShaderSource();
        }

        std::string FilterPreviewView::getFragmentShaderSource() const
        {
            return generateShaderWithSrc(FilterPreviewViewFS);
        }

        float FilterPreviewView::sumOfKernelElements(const std::array<float, 9>& kernel)
        {
            float sum = 0;
            for (int i = 0; i < 9; i++)
            {
                sum += kernel[i];
            }
            return (sum != 0) ? sum : 1;

        }

        std::vector<aex::math::Vector2> FilterPreviewView::genOffsetsVector(Size previewSize)
        {
            float width = previewSize.getWidth();
            float height = previewSize.getHeight();
            float step_w = (1.0 / width);
            float step_h = (1.0 / height);

            std::vector<aex::math::Vector2> offsets;
            offsets.push_back(aex::math::Vector2(-step_w, step_h)); // north west
            offsets.push_back(aex::math::Vector2(0.0f, step_h)); // north
            offsets.push_back(aex::math::Vector2(step_w, step_h)); // north east

            offsets.push_back(aex::math::Vector2(-step_w, 0.0f)); //  west
            offsets.push_back(aex::math::Vector2(0.0f, 0.0f)); // center
            offsets.push_back(aex::math::Vector2(step_w, 0.0f)); //  east

            offsets.push_back(aex::math::Vector2(-step_w, -step_h)); // south west
            offsets.push_back(aex::math::Vector2(0.0f, -step_h)); // south
            offsets.push_back(aex::math::Vector2(step_w, -step_h)); // south east

            return offsets;
        }

        std::array<float, 9> FilterPreviewView::kernelSharpSample()
        {
            return std::array<float, 9>{{
                -1.0f, -1.0f, -1.0f,
                -1.0f, 9.0f, -1.0f,
                -1.0f, -1.0f, -1.0f
            }};
        }

        MovementPreviewView::MovementPreviewView(PreviewSource& src) :
        SimplePreviewView(src),
        uMoveX
        {
            0.00f, "uMoveX"
        },
        uMoveY{0.00f, "uMoveY"}
        {
            connectUniform(&uMoveX);
            connectUniform(&uMoveY);
        }

        void MovementPreviewView::setData(FilterData* data)
        {
            if (data->filter == FILTER::MOVEMENT)
            {
                FilterMovementData* mData = (FilterMovementData*) data;
                uMoveX.setValue(mData->moveX);
                uMoveY.setValue(mData->moveY);
            }
        }

        std::string MovementPreviewView::getVertexShaderSource() const
        {
            return SimplePreviewView::getVertexShaderSource();
        }

        std::string MovementPreviewView::getFragmentShaderSource() const
        {
            return generateShaderWithSrc(MovementPreviewViewFS);
        }

        FBOTextureSource::FBOTextureSource(std::unique_ptr<SimplePreviewView> prevView) :
        PreviewView(*prevView->_parent),
        srcPreviewView
        {
            std::move(prevView)
        },
        fbo{srcPreviewView->getPreviewSize().getWidth(), srcPreviewView->getPreviewSize().getHeight()}
        {
            srcPreviewView->setIsFBOTextureRenderer(true); // always render on whole screen (fbo size is the same like preview texture size)
        }

        void FBOTextureSource::draw(AexRenderer* gl)
        {
            if (!isValid())
                return;

            fbo.draw(gl);
            srcPreviewView->draw(gl);
        }

        void FBOTextureSource::finishdraw(AexRenderer* gl)
        {
            if (!isValid())
                return;

            srcPreviewView->finishdraw(gl);
            fbo.finishdraw(gl); // now fbo can provide texture using .getTexture() method
        }

        void FBOTextureSource::renderIn(const Size& frameSize, const Size& componetSize, int posX, int posY, bool fullFill)
        {
            srcPreviewView->renderIn(frameSize, componetSize, posX, posY, fullFill);
        }

        Texture* FBOTextureSource::getFBOTexture()
        {
            return fbo.getTexture();
        }

        FBOTextureChainElement::FBOTextureChainElement(PreviewSource& src) :
        BasePreviewView(src),
        bRenderInFBO(true),
        fbo(src.getPreviewSize().getWidth(), src.getPreviewSize().getHeight()),
        uTexture(nullptr, "uTexture")
        {
            connectTexture(&texturePtr);
        }

        void FBOTextureChainElement::draw(AexRenderer* gl)
        {
            if (bRenderInFBO)
                fbo.draw(gl);

            this->BasePreviewView::draw(gl); // virtual parent method invocation
        }

        void FBOTextureChainElement::finishdraw(AexRenderer* gl)
        {
            this->BasePreviewView::finishdraw(gl);

            if (bRenderInFBO)
                fbo.finishdraw(gl);
        }

        void FBOTextureChainElement::setTexture(Texture* texture)
        {
            this->texturePtr = texture;
            uTexture.setValue(texture);
        }

        void FBOTextureChainElement::renderInFBO(bool bInFBO)
        {
            bRenderInFBO = bInFBO;
        }

        Texture* FBOTextureChainElement::getTexture()
        {
            return fbo.getTexture();
        }

        FBOTextureSimpleChainElement::FBOTextureSimpleChainElement(PreviewSource& src) :
        FBOTextureChainElement(src),
        vecUV(std::vector<float>{0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f}),
        vecVertexData
        {
            FBOTextureSimpleChainElement::genFullScreenOGLVector()
        }

        ,
        vecDrawOrder(std::vector<uint16_t>{3, 0, 2, 1}),
        vboVertexData
        {
            VertexData::createVertexData(vecVertexData, "aPosition", 3)
        },
        vboUV{VertexData::createVertexData(vecUV, "aTexCoord", 2)},
        vboDrawOrder{IndexData::createTraingleStrip(vecDrawOrder)}
        {
            connectUniform(&uTexture);
            connectVertexData(&vboVertexData);
            connectVertexData(&vboUV);
            connectIndexData(&vboDrawOrder);
        }

        std::string FBOTextureSimpleChainElement::getVertexShaderSource() const
        {
            return std::string(SimpleChainElementVS);
        }

        std::string FBOTextureSimpleChainElement::getFragmentShaderSource() const
        {
            return std::string(SimpleChainElementFS);
        }

        std::vector<float> FBOTextureSimpleChainElement::genFullScreenOGLVector()
        {
            return std::vector<float>{
                -1.0f, 1.0f, 0.0f, // left top
                -1.0f, -1.0f, 0.0f, // left bottom
                1.0f, -1.0f, 0.0f, // right bottom
                1.0f, 1.0f, 0.0f // right top
            };
        }

        FBOTextureChain::FBOTextureChain(std::unique_ptr<FBOTextureSource> fboTexSrc) :
        PreviewView(*(fboTexSrc->_parent)),
        fboTexSrc(std::move(fboTexSrc))
        {

        }

        void FBOTextureChain::draw(AexRenderer* gl)
        {
            if (chainElements.empty())
                return;

            if (!isValid())
                return;

            Texture* nextTexFromFBO;

            fboTexSrc->draw(gl);
            fboTexSrc->finishdraw(gl);
            nextTexFromFBO = fboTexSrc->getFBOTexture();

            for (size_t i = 0; i < chainElements.size() - 1; i++)
            {
                chainElements[i]->setTexture(nextTexFromFBO);
                chainElements[i]->draw(gl);
                chainElements[i]->finishdraw(gl);
                nextTexFromFBO = chainElements[i]->getTexture();
            }

            chainElements[chainElements.size() - 1]->setTexture(nextTexFromFBO);
            chainElements[chainElements.size() - 1]->renderInFBO(false);
            chainElements[chainElements.size() - 1]->draw(gl);
            chainElements[chainElements.size() - 1]->finishdraw(gl);
            chainElements[chainElements.size() - 1]->renderInFBO(true);
        }

        void FBOTextureChain::finishdraw(AexRenderer* gl)
        {
            if (!isValid())
                return;
        }

        void FBOTextureChain::renderIn(const Size& frameSize, const Size& componetSize, int posX, int posY, bool fullFill)
        {

            fboTexSrc->renderIn(frameSize, componetSize, posX, posY, fullFill);
        }

        void FBOTextureChain::newUVMatrixCallback(RotationMatrix m)
        {
            // check RotationMatrix
        }

        void FBOTextureChain::newPreviewFrameCallback(PreviewSource& src)
        {

        }

        void FBOTextureChain::addChainElement(std::unique_ptr<FBOTextureChainElement> chainElement)
        {
            chainElements.push_back(std::move(chainElement));
        }

        PixelBufferPreviewView::PixelBufferPreviewView(PreviewSource& src, std::unique_ptr<PreviewView> pv) :
        PreviewView(src),
        m_fbo(pv->getPreviewSize().getWidth(), pv->getPreviewSize().getHeight(), true),
        m_pixBuff(pv->getPreviewSize().getWidth(), pv->getPreviewSize().getHeight()),
        m_newFrame(false),
        m_newPixBuff(false),
        m_preview(std::move(pv))
        {
            setIsFBOTextureRenderer(true);
            m_preview->setIsFBOTextureRenderer(true);
            m_preview->renderIn(Size(), Size(), 0, 0); // full fbo
        }

        PixelBufferPreviewView::PixelBufferPreviewView(PreviewSource& src, std::unique_ptr<PreviewView> pv, aex::cam::Size size) :
                PreviewView(src),
                m_fbo(size.getWidth(), size.getHeight()),
                m_pixBuff(size.getWidth(), size.getHeight()),
                m_newFrame(false),
                m_newPixBuff(false),
                m_preview(std::move(pv))
        {
            LOG_DEBUG("FBO SIZE: ", size);
            setIsFBOTextureRenderer(true);
            m_preview->setIsFBOTextureRenderer(true);
            m_preview->renderIn(Size(), Size(), 0, 0);
        }

        void PixelBufferPreviewView::draw(AexRenderer* gl)
        {
            if (!isValid())
                return;
            if(!m_newFrame)
                return;

            m_newFrame = false;
            // set fbo as output and draw into it
            m_fbo.draw(gl);
            //LOG_DEBUG("M, Preview: ", m_preview.get());
            m_preview->draw(gl);
           // LOG_DEBUG("M, Preview2: ", m_preview.get());

            // now can read raw data
            unsigned w = m_fbo.getTexSizeX();
            unsigned h = m_fbo.getTexSizeY();
            GL_CHECK(glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, m_pixBuff.m_data));
           // LOG_DEBUG("M, Preview3: ", m_preview.get());
            m_newPixBuff = true;
        }

        void PixelBufferPreviewView::finishdraw(AexRenderer* gl)
        {
            if (!isValid())
                return;
            if(!m_newPixBuff)
                return;
            m_newPixBuff = false;
            m_preview->finishdraw(gl);
            m_fbo.finishdraw(gl);

            // send signal onNewPixelBuffer
            if(this->m_onNewPixBuffAction)
                this->m_onNewPixBuffAction(m_pixBuff);
        }

        void  PixelBufferPreviewView::needsUpdate()
        {
            m_preview->needsUpdate();
        }

        void PixelBufferPreviewView::renderIn(const Size& frameSize, const Size& componetSize, int posX, int posY, bool fullFill)
        {
            // do nothing, unused method, its just PixelBuffer provider
        }

        void PixelBufferPreviewView::newPreviewFrameCallback(PreviewSource& src)
        {
            m_newFrame = true;
        }

        void PixelBufferPreviewView::newUVMatrixCallback(RotationMatrix m)
        {

        }

        void PixelBufferPreviewView::setAction(const std::function<void (aex::cam::PixelBuffer&)>& action)
        {
            this->m_onNewPixBuffAction = action;
        }
    }
}
