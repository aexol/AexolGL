#ifndef CAM_PREV_VIEW_HPP
#define CAM_PREV_VIEW_HPP


#include <aex/AWindow.hpp>
#include <aex/Common.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/ShaderDrw.hpp>
#include <aex/GameEvent.hpp>
#include <aex/HudElement.hpp>
#include <aex/VertexData.hpp>
#include <aex/UniformMatrix4fv.hpp>
#include <aex/UniformMatrix3fv.hpp>
#include <aex/UniformVector2fv.hpp>
#include <aex/UniformSampler.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/Texture.hpp>
#include <aex/Camera.hpp>
#include <aex/FrameBuffer.hpp>
#include <array>
#include <string>
#include <functional>

namespace aex {
    namespace cam {
        class FBOTextureSource;

        // defines draw, finishdraw and generating uber shaders methods

        class BasePreviewView : public PreviewView {
        public:
            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();
            virtual void renderIn(const Size& frameSize, const Size& componetSize, int posX, int posY, bool fullFill = false) = 0;
            virtual void newUVMatrixCallback(RotationMatrix m) = 0;
            virtual void newPreviewFrameCallback(PreviewSource& src) = 0;
            virtual ~BasePreviewView();
            friend class Camera;
        protected:
            BasePreviewView(PreviewSource& src);
            void lazyInit();
            void connectUniform(Uniform* uniform);
            void connectTexture(Texture* texture);
            void connectTexture(Texture** texture); // so tex objs can be changed
            void connectVertexData(std::unique_ptr<VertexData>* data);
            void connectIndexData(IndexData* data);
            std::string getShaderDefines() const;
            std::string generateShaderWithSrc(const char* src) const;
            virtual std::string getVertexShaderSource() const = 0;
            virtual std::string getFragmentShaderSource() const = 0;

            ShaderDrw* shaderPtr;
            std::vector<Uniform*> uniformsPtrs;
            std::vector<Texture*> texturesPtrs;
            std::vector<Texture**> ptrTexturesPtrs;
            std::vector<std::unique_ptr<VertexData>*> vertexDataPtrs;
            std::vector<IndexData*> indexDataPtrs;
        };

        class SimplePreviewView : public BasePreviewView {
        public:
            virtual void renderIn(const Size& frameSize, const Size& componetSize, int posX, int posY, bool fullFill = false);
            virtual void newUVMatrixCallback(RotationMatrix m);
            virtual void newPreviewFrameCallback(PreviewSource& src);

            virtual ~SimplePreviewView() {
            }
            friend class Camera;
            friend class FBOTextureSource;
        protected:
            SimplePreviewView(PreviewSource& src);
            virtual std::string getVertexShaderSource() const;
            virtual std::string getFragmentShaderSource() const;

            // ANDROID
            aex::Texture* textureOES;
            UniformSampler uTexOESSampler; // for above texture

            // iOS
            aex::Texture* textureY;
            aex::Texture* textureUV;
            UniformSampler uTexYSampler;
            UniformSampler uTexUVSampler;

            // BOTH
            UniformMatrix4fv uUVTransMatrix;

            std::vector<float> vecUV;
            std::vector<float> vecVertexData;
            std::vector<uint16_t> vecDrawOrder;

            std::unique_ptr<VertexData> vboUV;
            std::unique_ptr<VertexData> vboVertexData;
            IndexData vboDrawOrder;
        };

        class NoctovisionPreviewView : public SimplePreviewView {
        public:
            friend class Camera;
        protected:
            NoctovisionPreviewView(PreviewSource& src);
            virtual std::string getVertexShaderSource() const;
            virtual std::string getFragmentShaderSource() const;
        };
        
        class HRMPreviewView : public SimplePreviewView {
        public:
            friend class Camera;
        protected:
            HRMPreviewView(PreviewSource& src);
            virtual std::string getVertexShaderSource() const;
            virtual std::string getFragmentShaderSource() const;
        };

        class FilterPreviewView : public SimplePreviewView {
        public:
            friend class Camera;
        protected:
            FilterPreviewView(PreviewSource& src, std::array<float, 9> kernel);
            virtual std::string getVertexShaderSource() const;
            virtual std::string getFragmentShaderSource() const;
            float sumOfKernelElements(const std::array<float, 9>& kernel);
            static std::vector<aex::math::Vector2> genOffsetsVector(Size previewSize);
            static std::array<float, 9> kernelSharpSample();

            UniformFloat uPreviewWidth;
            UniformFloat uPreviewHeight;
            UniformMatrix3fv uKernel;
            UniformFloat uKernelDivisor;
            // UniformVector2fv uOffsets; // normalized texture coords offsets
        };

        class MovementPreviewView : public SimplePreviewView {
        public:
            void setData(FilterData* data);
            friend class Camera;
        protected:
            MovementPreviewView(PreviewSource& src);
            virtual std::string getVertexShaderSource() const;
            virtual std::string getFragmentShaderSource() const;

            UniformFloat uMoveX;
            UniformFloat uMoveY;
        };


        /* first element in chain witch gets standard camera texture */
        class FBOTextureChain;

        class FBOTextureSource : public PreviewView {
        public:
            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void renderIn(const Size& frameSize, const Size& componetSize, int posX, int posY, bool fullFill = false);

            virtual void newUVMatrixCallback(RotationMatrix m) {
            }
            virtual void newPreviewFrameCallback(PreviewSource& src){  
            }

            virtual void needsUpdate() {
            };

            Texture* getFBOTexture();

            friend class Camera;
            friend class FBOTextureChain;
        protected:
            FBOTextureSource(std::unique_ptr<SimplePreviewView> prevView);
            std::unique_ptr<SimplePreviewView> srcPreviewView;
            FrameBufferOGL fbo;
        };

        class FBOTextureChainElement : public BasePreviewView {
        public:
            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);

            virtual void renderIn(const Size& frameSize, const Size& componetSize, int posX, int posY, bool fullFill = false)
            {
            }

            virtual void newUVMatrixCallback(RotationMatrix m) {
            }
            virtual void newPreviewFrameCallback(PreviewSource& src){  
            }
            void setTexture(Texture* texture);
            void renderInFBO(bool bInFBO);
            Texture* getTexture();

            friend class Camera;
            friend class FBOTextureChain;
        protected:
            FBOTextureChainElement(PreviewSource& src);
            virtual std::string getVertexShaderSource() const = 0;
            virtual std::string getFragmentShaderSource() const = 0;

            bool bRenderInFBO;
            FrameBufferOGL fbo;
            Texture* texturePtr;
            UniformSampler uTexture;
        };

        class FBOTextureSimpleChainElement : public FBOTextureChainElement {
        public:
            friend class Camera;
            friend class FBOTextureChain;
        protected:
            FBOTextureSimpleChainElement(PreviewSource& src);
            virtual std::string getVertexShaderSource() const;
            virtual std::string getFragmentShaderSource() const;
            static std::vector<float> genFullScreenOGLVector();

            std::vector<float> vecUV;
            std::vector<float> vecVertexData;
            std::vector<uint16_t> vecDrawOrder;

            std::unique_ptr<VertexData> vboVertexData;
            std::unique_ptr<VertexData> vboUV;
            IndexData vboDrawOrder;
        };

        class FBOTextureChain : public PreviewView {
        public:
            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void renderIn(const Size& frameSize, const Size& componetSize, int posX, int posY, bool fullFill = false);
            virtual void newUVMatrixCallback(RotationMatrix m);
            virtual void newPreviewFrameCallback(PreviewSource& src);


            virtual void needsUpdate() {
            }

            void addChainElement(std::unique_ptr<FBOTextureChainElement> chainElement);
            friend class Camera;
        protected:
            FBOTextureChain(std::unique_ptr<FBOTextureSource> fboTexSrc);
            std::unique_ptr<FBOTextureSource> fboTexSrc;
            std::vector< std::unique_ptr<FBOTextureChainElement> > chainElements;
        };

        class FBOTexturePreviewView : public PreviewView {
        public:
            friend class Camera;
        protected:
        };

        class PixelBufferPreviewView : public PreviewView {
        public:
            typedef boost::signals2::signal<void (PixelBuffer&) > sig_onpb_type;
            friend class Camera;
            
            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();
            virtual void renderIn(const Size& frameSize, const Size& componetSize, int posX, int posY, bool fullFill = false);
            
            virtual void newPreviewFrameCallback(PreviewSource& src);
            virtual void newUVMatrixCallback(RotationMatrix m);
            virtual void setAction(const std::function<void (aex::cam::PixelBuffer&)>& action);
        protected:
            
            PixelBufferPreviewView(PreviewSource& src, std::unique_ptr<PreviewView> pv);
            PixelBufferPreviewView(PreviewSource& src, std::unique_ptr<PreviewView> pv, aex::cam::Size size);
            FrameBufferOGL m_fbo;
            PixelBuffer m_pixBuff;
            bool m_newFrame;
            bool m_newPixBuff;
            std::function<void (PixelBuffer&)> m_onNewPixBuffAction;
            std::unique_ptr<PreviewView> m_preview;
        };
    }
}
#endif // CAM_PREV_VIEW_HPP