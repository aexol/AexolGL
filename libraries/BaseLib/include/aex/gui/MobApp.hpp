#ifndef MOB_APP_AEX_GUI_HEADER
#define MOB_APP_AEX_GUI_HEADER

#include "FixedPanel.hpp"
#include "Frame.hpp"
#include "ColorRectComponent.hpp"
#include <chrono>
#include <memory>
#include <array>
#include <map>
#include <functional>

#include <aex/tween/Delay.hpp>
#include <aex/tween/Timeline.hpp>
#include <aex/tween/Tween.hpp>

namespace aex
{
    namespace gui
    {
        class MobApp;
        class Modal : public FixedPanel
        {
        public:
          friend class MobApp;
        public:
          Modal();
          virtual ~Modal();
          void hide();
          bool isHided();
          void onShow();
          void show();
          void setResultCallback(const std::function<void(Modal*)>& cb);
          void fireResultCallback();

        protected:
          void setHideMeCallback(const std::function<void()>& cb);
        protected:
          bool m_isHided;
          std::function<void(Modal*)> m_resultCb;
          std::function<void()> m_hideMeCb;
          bool m_hidePls;
        };

        class ModalAccessor : public aex::tween::TweenAccessor<Modal>
        {
        public:
          static const int MOVE_MODAL = 0;
          virtual void getValues(const Modal* obj, int typeID, std::vector<float>& retValues);
          virtual void setValues(Modal* obj, int typeID, const std::vector<float>& srcValues);
        protected:
        };

        class Layer : public FixedPanel
        {
        public:
            Layer();
            virtual ~Layer();
            virtual void refresh(int flags);

            virtual void onShow() = 0;
            virtual void onHide() = 0;

        protected:
        };
        typedef Layer* LayerPtr;

        class LayerAccessor : public aex::tween::TweenAccessor<Layer>
        {
        public:
            static const int POSITION_XY = 0;
            virtual void getValues(const Layer* obj, int typeID, std::vector<float>& retValues);
            virtual void setValues(Layer* obj, int typeID, const std::vector<float>& srcValues);
        };

        class MobAppAccessor : public aex::tween::TweenAccessor<MobApp>
        {
        public:
          static const int CURTAIN_ALPHA = 0;
          virtual void getValues(const MobApp* obj, int typeID, std::vector<float>& retValues);
          virtual void setValues(MobApp* obj, int typeID, const std::vector<float>& srcValues);
        };

        class MobApp : public Panel
        {
        public:
          friend class MobAppAccessor;
            enum LEVEL{
                L0 = 0,
                L1, // 1
                L2, // 2
                L3, // 3
                L4, // 4
                L_LAST // 5
            };

            enum EFFECT {
                FROM_RIGHT,
                FROM_LEFT,
                FROM_TOP,
                FROM_BOTTOM,
                SLIDE_LEFT,
                SLIDE_RIGHT,
                SLIDE_DOWN,
                SLIDE_UP
            };

        public:
            MobApp();
            virtual ~MobApp();
            virtual void refresh(int flags);

            void addLayer   (const std::string& key, std::unique_ptr<Layer> layer, LEVEL level);
            void showLayer  (const std::string& key);
            void deleteLayer (const std::string& key);
            void gotoLayer  (const std::string& key, EFFECT effect, float timeSeconds);
            void gotoLayer  (const std::string& key, EFFECT effect, float timeSeconds, const std::function<float(float,float,float,float)>& easing);
            void setGotoEasing(const std::function<float(float,float,float,float)>& easing);
            void setOnGotoLayerDoneCallback(const std::function<void(LEVEL)>& callback);
            bool isLevelTweening(LEVEL level) const;
            void showModal(Modal* modal, const std::function<void(int)>& onShowCb = [](int){});
            bool layerExists(const std::string& key);
            bool layerVisible(const std::string& key);
            void clearAllLayerLevels();
        protected:
            LayerPtr getLayerPtr(const std::string& key);
            LEVEL getLevel(const std::string& key);

            LayerPtr getLayerFromLvl(LEVEL level);
            void addLayerToLvl(LayerPtr layer, LEVEL level, bool invokeCallbacks);
            void clearLayerInLvl(LEVEL level, bool invokeCallbacks);

            void resetLayersInPanel();
            bool isLayerInLevel(LayerPtr layer, LEVEL level) const;

        protected:
            int getCurtainAlpha() const;
            void setCurtainAlpha(int alpha);
        protected:
            std::array< std::vector<LayerPtr> , LEVEL::L_LAST+1> m_layers; // vector only for animations time (switches between layers)

            std::map<std::string, std::unique_ptr<Layer> > m_layersMap;
            std::map<std::string, LEVEL> m_levelsMap;
            tween::TweenPlayer m_tweenPlayer;
            std::function<float(float,float,float,float)> m_easing;
            std::function<void(LEVEL)> m_gotoLayerDoneCallback;
            std::map<LEVEL, bool> m_tweeningsMap;
            std::map<LEVEL, std::vector<LayerPtr>> m_layersToHideMap; // after
            // For modals
            int m_curtainAlpha;
            ColorRectComponent m_curtain;
            Modal* m_modal;
            // STATIC:
        protected:
        };
    }
}

#endif
