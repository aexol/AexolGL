#include "aex/gui/MobApp.hpp"
#include <aex/Easing.hpp>
#include <stdexcept>

namespace aex {
    namespace gui {

        Modal::Modal() :
        m_isHided(true),
        m_resultCb([](Modal* m){}),
        m_hideMeCb([](){})
        {
        }

        Modal::~Modal()
        {
        }

        void Modal::show()
        {
          m_isHided = false;
        }

        void Modal::setResultCallback(const std::function<void(Modal*)>& cb)
        {
          m_resultCb = cb;
        }

        void Modal::fireResultCallback()
        {
          m_resultCb(this);
        }

        void Modal::setHideMeCallback(const std::function<void()>& cb)
        {
          m_hideMeCb = cb;
        }

        void Modal::hide()
        {
          m_isHided = true;
          // invoke the callback if is first
          m_hideMeCb();
        }

        bool Modal::isHided()
        {
          return m_isHided;
        }

        void ModalAccessor::getValues(const Modal* obj, int typeID, std::vector<float>& retValues)
        {
          switch(typeID)
          {
            case MOVE_MODAL:
              retValues[0] = obj->getArea().p().x();
              retValues[1] = obj->getArea().p().y();
            break;
            default:
            break;
          }
        }

        void ModalAccessor::setValues(Modal* obj, int typeID, const std::vector<float>& srcValues)
        {
          switch(typeID)
          {
            case MOVE_MODAL:
              obj->setPosition(Position(srcValues[0], srcValues[1]));
            break;
            default:
            break;
          }
        }

        void LayerAccessor::getValues(const Layer* obj, int typeID, std::vector<float>& retValues) {
            switch (typeID) {
                case POSITION_XY:
                    retValues[0] = obj->getArea().p().x();
                    retValues[1] = obj->getArea().p().y();
                    break;
                default:
                    break;
            }
        }

        void LayerAccessor::setValues(Layer* obj, int typeID, const std::vector<float>& srcValues) {
            switch (typeID) {
                case POSITION_XY:
                    obj->setPosition(Position(srcValues[0], srcValues[1]));
                    break;
                default:
                    break;
            }
        }

        Layer::Layer()
        {
            setArea(Frame::getFrameArea());
        }

        Layer::~Layer() {
        }

        void Layer::refresh(int flags)
        {
            if(checkFlag(FLAG::SIZE_CHANGED, flags))
            {
                for(size_t i = 0 ; i < m_components.size() ; i++)
                {
                    m_components[i]->setArea(getArea());
                }
            }

            FixedPanel::refresh(flags);
        }

        void MobAppAccessor::getValues(const MobApp* obj, int typeID, std::vector<float>& retValues)
        {
          switch(typeID)
          {
            case CURTAIN_ALPHA:
              retValues[0] = obj->getCurtainAlpha();
            break;
            default:
            break;
          }
        }

        void MobAppAccessor::setValues(MobApp* obj, int typeID, const std::vector<float>& srcValues)
        {
          switch(typeID)
          {
            case CURTAIN_ALPHA:
              obj->setCurtainAlpha(srcValues[0]);
            break;
            default:
            break;
          }
        }

        MobApp::MobApp() :
        m_easing(aex::math::Easings::Quint::easeOut),
        m_gotoLayerDoneCallback([](LEVEL l){}),
        m_curtainAlpha(0),
        m_curtain(Color(70,20,200, m_curtainAlpha), Frame::getFrameArea())
        {
            tween::Tween<Layer>::setAccessor(std::make_shared<LayerAccessor>());
            tween::Tween<MobApp>::setAccessor(std::make_shared<MobAppAccessor>());
            tween::Tween<Modal>::setAccessor(std::make_shared<ModalAccessor>());
            m_modal = nullptr;
            for(int level = LEVEL::L0 ; level <= LEVEL::L_LAST ; level++)
            {
              m_tweeningsMap[(LEVEL)level] = false;
              m_layersToHideMap[(LEVEL)level] = std::vector<LayerPtr>();
            }
        }

        MobApp::~MobApp() {
        }

        void MobApp::refresh(int flags) {
        }

        void MobApp::addLayer(const std::string& key, std::unique_ptr<Layer> layer, LEVEL level) {
            layer->setArea(Frame::getFrameArea());
            m_layersMap[key] = std::move(layer);
            m_levelsMap[key] = level;
        }

        void MobApp::showLayer(const std::string& key) {
            LayerPtr layer = getLayerPtr(key);
            if (!layer)
                return; // maeybe add here throw an exception ???

            layer->setPosition(Position(0.0f, 0.0f));
            LEVEL level = getLevel(key);
            clearLayerInLvl(level, true); // onHide
            addLayerToLvl(layer, level, true); // onShow

            resetLayersInPanel();
        }

        void MobApp::deleteLayer(const std::string& key) {
            LayerPtr layer = getLayerPtr(key);
            if (!layer)
                return;
            LEVEL level = getLevel(key);

            if (isLayerInLevel(layer, level)) {
                clearLayerInLvl(level, true);
            }

            m_layersMap.erase(key);
            m_levelsMap.erase(key);

            resetLayersInPanel();
        }

        void MobApp::gotoLayer(const std::string& key, EFFECT effect, float timeSeconds) {
            gotoLayer(key,effect,timeSeconds,m_easing);
        }

        void MobApp::gotoLayer  (const std::string& key, EFFECT effect, float timeSeconds,
                const std::function<float(float,float,float,float)>& easing)
        {
            LayerPtr nextLayer = getLayerPtr(key);
            LEVEL level = getLevel(key);
            if (!nextLayer)
                throw std::runtime_error("MobApp::gotoLayer(...) layer at provided key (name) not exists: "+key);

            LayerPtr prevLayer = getLayerFromLvl(level); // can be null
            Area appArea = Frame::getFrameArea();

            Position prevStartPos = appArea.p();
            Position prevEndPos = appArea.p();
            Position nextStartPos = appArea.p();
            Position nextEndPos = appArea.p();

            switch (effect) {
                case EFFECT::FROM_LEFT:
                    nextStartPos.move(-appArea.s().w(), 0.0f);
                    break;
                case EFFECT::FROM_RIGHT:
                    nextStartPos.move(appArea.s().w(), 0.0f);
                    break;
                case EFFECT::FROM_TOP:
                    nextStartPos.move(0.0f, -appArea.s().h());
                    break;
                case EFFECT::FROM_BOTTOM:
                    nextStartPos.move(0.0f, appArea.s().h());
                    break;

                case EFFECT::SLIDE_RIGHT:
                    nextStartPos.move(-appArea.s().w(), 0.0f);
                    prevEndPos.move(appArea.s().w(), 0.0f);
                    break;
                case EFFECT::SLIDE_LEFT:
                    nextStartPos.move(appArea.s().w(), 0.0f);
                    prevEndPos.move(-appArea.s().w(), 0.0f);
                    break;
                case EFFECT::SLIDE_UP:
                    nextStartPos.move(0.0f, appArea.s().h());
                    prevEndPos.move(0.0f, -appArea.s().h());
                    break;
                case EFFECT::SLIDE_DOWN:
                    nextStartPos.move(0.0f, -appArea.s().h());
                    prevEndPos.move(0.0f, appArea.s().h());
                    break;
                default:
                    break;
            }
            // set up tweens:

            m_layersToHideMap[level] = m_layers[level];
            addLayerToLvl(nextLayer, level, true); // new onShow
            resetLayersInPanel();

            // animation
            if(prevLayer)
            tween::Timeline::sequence()
                ->push(tween::Tween<Layer>::set(prevLayer, LayerAccessor::POSITION_XY)->target({prevStartPos.x(), prevStartPos.y() }) )
                ->push(tween::Tween<Layer>::to(prevLayer, LayerAccessor::POSITION_XY, timeSeconds)->target({prevEndPos.x(), prevEndPos.y()})
                    ->interpolation(easing)
                )->start(*Frame::instance()->getTweenPlayer());

            m_tweeningsMap[level] = true;
            tween::Timeline::sequence()
                ->push(tween::Tween<Layer>::set(nextLayer, LayerAccessor::POSITION_XY)->target({nextStartPos.x(), nextStartPos.y()} ))
                ->push(tween::Tween<Layer>::to(nextLayer, LayerAccessor::POSITION_XY, timeSeconds)->target( {nextEndPos.x(), nextEndPos.y()})
                    ->callback([this, level, nextLayer](int flag){
                        if( (flag & tween::TWEENING::END) != 0){
                          clearLayerInLvl(level, false); // next still onShow
                          addLayerToLvl(nextLayer, level, false); // next stillOnShow
                          resetLayersInPanel();
                          m_gotoLayerDoneCallback(level); // callback invocation
                          m_tweeningsMap[level] = false;
                          for(LayerPtr ptr : m_layersToHideMap[level])
                            ptr->onHide();
                          m_layersToHideMap[level].clear();
                      }
                    })->interpolation(easing)
                )->start(*Frame::instance()->getTweenPlayer());
        }

        void MobApp::setGotoEasing(const std::function<float(float,float,float,float)>& easing)
        {
            m_easing = easing;
        }

        void MobApp::setOnGotoLayerDoneCallback(const std::function<void(LEVEL)>& callback)
        {
          m_gotoLayerDoneCallback = callback;
        }

        bool MobApp::isLevelTweening(LEVEL level) const
        {
          return m_tweeningsMap.at(level);
        }

        void MobApp::showModal(Modal* modal, const std::function<void(int)>& onShowCb)
        {
            if(m_modal)
              return;
            m_modal = modal;

            float xPos = ((Frame::getFrameArea().s().w() - m_modal->getArea().s().w()) / 2.0f);
            float yPos = Frame::getFrameArea().s().h();

            m_modal->setHideMeCallback([this, xPos](){
              // TODO change this to transistions etc

              tween::Tween<Modal>::to(m_modal, ModalAccessor::MOVE_MODAL, 0.7)
              ->target({xPos, Frame::getFrameArea().s().h()})
              ->interpolation(aex::math::Easings::Back::easeIn)
              ->callback([this](int type){
                  resetLayersInPanel();
                  Modal* m = m_modal;
                  m_modal = nullptr;
                  m->fireResultCallback();
              })
              ->start(*Frame::instance()->getTweenPlayer());

              tween::Tween<MobApp>::to(this, MobAppAccessor::CURTAIN_ALPHA, 0.5f)
              ->target({0})
              ->start(*Frame::instance()->getTweenPlayer());
            });

            m_modal->show();

            m_modal->setPosition(Position(xPos, yPos));
            setCurtainAlpha(0);
            tween::Tween<Modal>::to(m_modal, ModalAccessor::MOVE_MODAL, 0.7f)
            ->target({xPos, 0.2f})
            ->interpolation(aex::math::Easings::Back::easeOut)
            ->callback(onShowCb)
            ->start(*Frame::instance()->getTweenPlayer());

            tween::Tween<MobApp>::to(this, MobAppAccessor::CURTAIN_ALPHA, 0.5f)
            ->target({125})
            ->start(*Frame::instance()->getTweenPlayer());

            addComponent(&m_curtain);
            addComponent(m_modal);

            // set start position:
        }

        bool MobApp::layerExists(const std::string& key)
        {
          if(getLayerPtr(key))
            return true;
          return false;
        }

        bool MobApp::layerVisible(const std::string& key)
        {
          LayerPtr layer = getLayerPtr(key);
          if (!layer)
              return false;
          LEVEL level = getLevel(key);

          if (isLayerInLevel(layer, level)) {
              return true;
          }
          return false;
        }

        void MobApp::clearAllLayerLevels()
        {
          for (int i = 0; i < (LEVEL::L_LAST + 1); i++)
          {
            clearLayerInLvl((LEVEL)i, true);
          }
          resetLayersInPanel();
        }

        LayerPtr MobApp::getLayerPtr(const std::string& key) {
            auto layerIter = m_layersMap.find(key);
            if (layerIter == m_layersMap.end())
                return nullptr;
            return (layerIter->second).get();
        }

        MobApp::LEVEL MobApp::getLevel(const std::string& key) {
            auto levelIter = m_levelsMap.find(key);
            if (levelIter == m_levelsMap.end())
                return LEVEL::L0;

            return levelIter->second;
        }

        LayerPtr MobApp::getLayerFromLvl(MobApp::LEVEL level)
        {
            if( !m_layers[level].empty() )
                return m_layers[level][0];
            else
              return nullptr;
        }

        void MobApp::addLayerToLvl(LayerPtr layer, LEVEL level, bool invokeCallbacks)
        {
            if(invokeCallbacks)
            {
              layer->onShow();
            }
            m_layers[level].push_back(layer);
        }

        void MobApp::clearLayerInLvl(LEVEL level, bool invokeCallbacks)
        {
            if(invokeCallbacks)
            {
              for(size_t i = 0 ; i < m_layers[level].size() ; i++)
              {
                m_layers[level][i]->onHide();
              }
            }
            m_layers[level].clear();
        }

        void MobApp::resetLayersInPanel() {
            clear();
            for (int i = 0; i < (LEVEL::L_LAST + 1); i++) {
                for(size_t j = 0 ; j < m_layers[i].size() ; j++)
                {
                    addComponent(m_layers[i][j]);
                }
            }
        }

        bool MobApp::isLayerInLevel(LayerPtr layer, LEVEL level) const
        {
            for(size_t i = 0 ; i < m_layers[level].size() ; i++)
            {
                if(m_layers[level][i] == layer)
                    return true;
            }
            return false;
        }

        int MobApp::getCurtainAlpha() const
        {
          return m_curtainAlpha;
        }

        void MobApp::setCurtainAlpha(int alpha)
        {
          m_curtainAlpha = alpha;
          m_curtain.setColor(Color(0,0,0,alpha));
        }
    }
}
