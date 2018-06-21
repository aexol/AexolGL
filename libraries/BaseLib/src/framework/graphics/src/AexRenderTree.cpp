#include <aex/AexRenderTree.hpp>
#include <aex/ChronoClock.hpp>
#include <aex/AWindow.hpp>
#include <aex/Logger.hpp>
#include <aex/FrameBuffer.hpp>
#include <thread>
namespace aex {

    /****************************AexRenderTreeBase*****************/
    AexRenderTreeBase::AexRenderTreeBase(QuaternionCamera* camera ) :
    m_quatCamera(camera)
    {
        if (m_quatCamera != nullptr)
        {
            m_ownsCamera = false;
        }
        initCamera();
    }

    AexRenderTreeBase::AexRenderTreeBase(aex::AWindow& window, QuaternionCamera* camera ) :
    m_quatCamera(camera)
    {
        m_aexRenderer = new AexRenderer(&window);
        if (m_quatCamera != nullptr)
        {
            m_ownsCamera = false;
        }
        initCamera();
    }

    AexRenderTreeBase::AexRenderTreeBase(aex::AexRenderer* renderer, QuaternionCamera* camera ) :
    m_aexRenderer(renderer),
    m_quatCamera(camera)
    {
        if (m_quatCamera != nullptr)
        {
            m_ownsCamera = false;
        }
        initCamera();
    }

    AexRenderTreeBase::~AexRenderTreeBase()
    {
        if (m_quatCamera && m_ownsCamera)
        {
            delete m_quatCamera;
        }
    }

    void
    AexRenderTreeBase::setAexRenderer(AexRenderer* aexRenderer)
    {
        m_aexRenderer = aexRenderer;
    }

    AexRenderer*
    AexRenderTreeBase::getAexRenderer() const
    {
        return m_aexRenderer;
    }

    void AexRenderTreeBase::setAverageFps(aexTimeUnit averageFps)
    {
        m_averageFps = averageFps;
    }

    aexTimeUnit AexRenderTreeBase::getAverageFps() const
    {
        return m_averageFps;
    }

    void AexRenderTreeBase::setComputeFramerate(bool computeFramerate)
    {
        m_computeFramerate = computeFramerate;
    }

    bool AexRenderTreeBase::isComputeFramerate() const
    {
        return m_computeFramerate;
    }

    void AexRenderTreeBase::setLogFrameDrawTime(bool logFrameDrawTime)
    {
        m_logFrameDrawTime = logFrameDrawTime;
    }

    bool AexRenderTreeBase::isLogFrameDrawTime() const
    {
        return m_logFrameDrawTime;
    }

    void AexRenderTreeBase::setOwnsCamera(bool ownsCamera)
    {
        m_ownsCamera = ownsCamera;
    }

    bool AexRenderTreeBase::isOwnsCamera() const
    {
        return m_ownsCamera;
    }
    
        
    QuaternionCamera* AexRenderTreeBase::getQuatCamera()
    {
        return m_quatCamera;
    }

    void
    AexRenderTreeBase::setQuadCamera(aex::QuaternionCamera* camera)
    {
        if (camera == m_quatCamera)
        {
            return;
        }

        if (m_quatCamera && m_ownsCamera)
        {
            delete m_quatCamera;
        }

        m_ownsCamera = false;
        m_quatCamera = camera;
        if (m_aexRenderer != nullptr)
        {
            m_aexRenderer->setQuatCamera(m_quatCamera);
        }

    }

    void AexRenderTreeBase::initCamera()
    {
        if (m_quatCamera == nullptr && m_ownsCamera)
        {

            m_quatCamera = new QuaternionCamera();
            m_ownsCamera = true;
            m_quatCamera->set_behavior(aex::CAMERA_MODES::FP_MODE);
            m_quatCamera->perspective(70.0f, (GLfloat) m_aexRenderer->GetWindow()->w(), (GLfloat) m_aexRenderer->GetWindow()->h(), 1.0f, 5000.0f);
            m_quatCamera->ortho(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 50.0f);
            // m_quatCamera->useOrthoMatrix();
            if (m_aexRenderer != nullptr)
            {
                m_aexRenderer->setQuatCamera(m_quatCamera);
            }

            m_quatCamera->lookAt(Vector(0.0f, 0.0f, 0.0f), Vector(1.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f));
        }

    }

    bool
    AexRenderTreeBase::init()
    {


        return m_aexRenderer->InitAPI(m_aexRenderer->GetWindow());

    }

    void
    AexRenderTreeBase::finish()
    {
        m_done = true;
    }

    /***************************AexRenderTree*******************/
    AexRenderTree_ptr
    AexRenderTree::makeAexrenderTree(AexRenderer* renderer, DrawNode_ptr renderRoot)
    {
        if (!renderRoot)
        {
            renderRoot = make_shared<DrawNode>();
        }

        return aex::sharedCreator<AexRenderTree>::makeSharedOfBase(renderer, renderRoot);
    }

    AexRenderTree::AexRenderTree()
    {
        m_renderRoot = aex::make_shared<aex::DrawNode>();
        m_renderRoot->setIsVisible(true);

    }

    AexRenderTree::AexRenderTree(aex::AWindow& window) :
    AexRenderTreeBase(window)
    {
    }

    AexRenderTree::AexRenderTree(DrawNode_ptr renderRoot) :
    m_renderRoot(renderRoot)
    {
    }

    AexRenderTree::AexRenderTree(AexRenderer* renderer) :
    AexRenderTreeBase(renderer)
    {
        m_renderRoot = aex::make_shared<aex::DrawNode>();
        m_renderRoot->setIsVisible(true);
    }

    AexRenderTree::AexRenderTree(AexRenderer* renderer, DrawNode_ptr renderRoot) :
    AexRenderTreeBase(renderer),
    m_renderRoot(renderRoot)
    {
    }

    AexRenderTree::~AexRenderTree()
    {
    }

    bool
    AexRenderTree::init()
    {
        m_lastToRender = aex::make_shared<aex::DrawNode>();
        return AexRenderTreeBase::init();
    }

    void AexRenderTree::setLastToRender(DrawNode_ptr LastToRender)
    {
        m_lastToRender = LastToRender;
    }

    DrawNode_ptr AexRenderTree::getLastToRender() const
    {
        return m_lastToRender;
    }
    
    void AexRenderTree::setRenderRoot(DrawNode_ptr root)
    {
        m_renderRoot = root;
    }
    
    DrawNode_ptr AexRenderTree::getRenderRoot()
    {
        return m_renderRoot;
    }


    void
    AexRenderTree::update()
    {

#if __TARGET_AEX_API__ != __UNSUPPORTED_API__

        aex::AexTimer& timer = aex::AexTimer::getInstance<aex::ChronoClock>();
        timer.setFrameTimeNow();
        aexTimeUnit stamp1=0.0;

        if (m_computeFramerate)
        {
            stamp1 = timer.getElapsedTimePauseInsensitive();
        }

        m_aexRenderer-> BeginFrame();

        if (m_quatCamera->isCameraMoved())
        {
            m_aexRenderer->setViewProjectionMatrix(m_quatCamera->transforms());
            m_aexRenderer->setViewMatrix(m_quatCamera->get_view_matrix());
        }

        m_renderRoot->draw(m_aexRenderer);

        if (m_lastToRender)
        {
            m_lastToRender->draw(m_aexRenderer);
        }

        m_aexRenderer->EndFrame();

        if (m_computeFramerate)
        {
            aexTimeUnit stamp2 = timer.getElapsedTimePauseInsensitive();
            m_frameTime += (stamp2 - stamp1);
            m_frames++;

            if (m_frames % 60 == 0)
            {

                if (m_logFrameDrawTime)
                {
                    float value = m_frameTime / 60.0f;

                    string info = "";

                    if (value < 0.01666f)
                    {
                        info = " Faster than 60fps!";
                    } else if (value < 0.03333f)
                    {
                        info = " Faster than 30fps";
                    } else
                    {
                        info = " Slower than 30fps";
                    }

                    LOG_DEBUG("frame render time :", value, info, " (", 1.0f / value, "fps)");
                }

                m_frameTime = 0.0f;
            }
        }

#endif
    }

    void
    AexRenderTree::clean()
    {
        if (m_renderRoot)
        {
            m_renderRoot->removeAll();
        }
    }

    void
    AexRenderTree::useEventHandler(EventHandler* handler)
    {
        handler->registerEventFunc(this, &AexRenderTree::handleIWantRenderRootEvent);
    }

    void
    AexRenderTree::handleIWantRenderRootEvent(const iWantRenderRoot* visual)
    {
        visual->getVisual()->setRootRenderNode(m_renderRoot);
    }

    ALoopNodePtr
    AexRenderTree::returnThisLoopNode()
    {
        return aex::static_pointer_cast<ALoopNode>(shared_from_this());
    }
}
