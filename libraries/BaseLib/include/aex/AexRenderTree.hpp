/*
 * File: AexRenderTree.h
 * Author: mritke
 *
 * Created on November 22, 2013, 5:16 PM
 */
#ifndef AEXRENDERTREE_H
#define AEXRENDERTREE_H
#include <aex/ALoopNode.hpp>
#include <aex/DrawNode.hpp>
#include <aex/QuaternionCamera.hpp>
#include <aex/Timer.hpp>
#include <aex/GameEvent.hpp>
#include <aex/GlobalGameEvents.hpp>
#include <aex/ObjectRenderNode.hpp>
#include "BoundingSphereObject.hpp"

namespace aex
{
    class AexRenderTree;
    typedef aex::shared_ptr<AexRenderTree> AexRenderTree_ptr;
    
    class AEX_PUBLIC_API GetRenderTree : public GameEvent
    {
    private:
        AexRenderTree* m_renderTreeRoot;
    public:
        GetRenderTree() { }
        void setRenderTreeRoot(AexRenderTree* renderTreeRoot)
        {
            m_renderTreeRoot = renderTreeRoot;
        }
        AexRenderTree* getRenderTreeRoot() const
        {
            return m_renderTreeRoot;
        }
    };
    
    class AEX_PUBLIC_API AexRenderTreeBase : public ALoopNode
    {


    public:
        bool init() ;
        void pause() {}
        void resume() {}
        void finish();
        void setQuadCamera(QuaternionCamera* camera);
        void initCamera();
        QuaternionCamera* getQuatCamera();
        void setAexRenderer(AexRenderer* aexRenderer);
        AexRenderer* getAexRenderer() const;
        void setAverageFps(aexTimeUnit averageFps);
        aexTimeUnit getAverageFps() const;
        void setComputeFramerate(bool computeFramerate);
        bool isComputeFramerate() const;
        void setLogFrameDrawTime(bool logFrameDrawTime);
        bool isLogFrameDrawTime() const;
        void setOwnsCamera(bool ownsCamera);
        bool isOwnsCamera() const;
        
        
    protected:
        AexRenderTreeBase(AexRenderer* renderer, QuaternionCamera* camera = nullptr);
        AexRenderTreeBase(aex::AWindow& window, QuaternionCamera* camera = nullptr);
        AexRenderTreeBase( QuaternionCamera* camera = nullptr);
        ~AexRenderTreeBase();
        
    protected:
        AexRenderer* m_aexRenderer;
        QuaternionCamera* m_quatCamera;
        size_t m_frames = 0;
        aexTimeUnit m_frameTime = 0.0;
        aexTimeUnit m_averageFps = 0.0;
        bool m_logFrameDrawTime = false;
        bool m_ownsCamera = true;
        bool m_computeFramerate = true;
    };
    
    class AEX_PUBLIC_API AexRenderTree : public AexRenderTreeBase, public enable_shared_from_this<AexRenderTree>
    {
    public:
        static AexRenderTree_ptr makeAexrenderTree(AexRenderer* renderer, DrawNode_ptr renderRoot = nullptr);
        bool init() ;
        void update();
        void clean();
        ALoopNodePtr returnThisLoopNode();
        void handleIWantRenderRootEvent(const iWantRenderRoot* visual);
        void useEventHandler(EventHandler* handler);
        void setRenderRoot(DrawNode_ptr root);
        DrawNode_ptr getRenderRoot();
        DrawNode_ptr getLastToRender() const;
        void setLastToRender(DrawNode_ptr LastToRender);
  
    protected:
        AexRenderTree(AexRenderer* renderer, DrawNode_ptr renderRoot);
        AexRenderTree(DrawNode_ptr renderRoot);
        AexRenderTree(AexRenderer* renderer);
        AexRenderTree(aex::AWindow& window);
        AexRenderTree();
        ~AexRenderTree();
        
    private:
        DrawNode_ptr m_renderRoot;
        DrawNode_ptr m_lastToRender;
        
    };
}
#endif /* AEXRENDERTREE_H */
