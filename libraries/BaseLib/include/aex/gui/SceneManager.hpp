#ifndef SCENE_MANAGER_GUI_HPP_
#define SCENE_MANAGER_GUI_HPP_

#include <aex/gui/GUI.hpp>
#include <aex/gui/MobApp.hpp>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <exception>

#include "SceneManager.hpp"

namespace aex
{
  namespace gui
  {
    class SceneManager;
    class EmptyLayer : public Layer
    {
    public:
      EmptyLayer() {}
      virtual ~EmptyLayer() {}

      virtual void onShow() {};
      virtual void onHide() {};
    };

    struct LayerFactory
    {
      LayerFactory();
      LayerFactory(
        MobApp::LEVEL level,
        const std::string& name,
        const std::function<std::unique_ptr<Layer>()>& factoryMethod
      );
      ~LayerFactory();

      MobApp::LEVEL m_level;
      std::string m_name;
      std::function< std::unique_ptr<Layer>() > m_factoryMethod;
    };

    class Scene
    {
    public:
      friend class SceneManager;
      Scene();
      Scene(const std::string& name);
      Scene(const std::string& name, std::vector<std::string> layersNames);
      ~Scene();
      const std::string& getName() const;
      void addLayer(const std::string& layerName);
      const std::vector<std::string>& getLayersNames() const;

    protected:
      std::string m_name;
      std::vector<std::string> m_layers;
    };

    struct SwitchParams
    {
      SwitchParams(); // default
      SwitchParams(
        float delaySeconds,
        MobApp::EFFECT effect,
        const std::function<float(float,float,float,float)>& easing
      );
      ~SwitchParams();

      float m_delaySeconds;
      MobApp::EFFECT m_effect;
      std::function<float(float,float,float,float)> m_easing;
      bool m_instantSwitch;
    };

    class SceneSwitch
    {
    public:
      SceneSwitch();
      ~SceneSwitch();
      void setParams(MobApp::LEVEL onLvl, const SwitchParams& params);
      bool hasParams(MobApp::LEVEL onLvl);
      const SwitchParams& getParams(MobApp::LEVEL onLvl);
    protected:
      std::map<MobApp::LEVEL, SwitchParams> m_params;
    };

    class SceneManager
    {
    public:
      SceneManager(aex::gui::Frame* frame);
      ~SceneManager();
      void addLayerFactory(const LayerFactory& layerFactory);
      void addScene(const Scene& scene);
      void setSwitch(const std::string& fromScene, const std::string& toScene, const SceneSwitch& sceneSwitch);
      void gotoScene(const std::string& sceneName, const std::function<void()>& withAction = [](){});
      void showScene(const std::string& sceneName, const std::function<void()>& withAction = [](){});
      void setDefaultSwitchParams(const SwitchParams& params);
      const SwitchParams& getDefaultSwitchParams() const;
      bool isSwitchingNow() const;
    protected:
      bool sceneExists(const std::string& sceneName);
      bool isAnyMobAppLayerTweening() const;
      std::vector<MobApp::LEVEL> getNotCoveredLevels(const std::string& fromSceneName, const std::string& toSceneName);
      std::string getEmptyLayerName(const MobApp::LEVEL lvl) const;

    protected:
      std::map<std::string, LayerFactory> m_layersFactories;
      std::map<std::string, Scene> m_scenes;
      std::map<std::string, std::map<std::string, SceneSwitch> > m_switches; // from scene name, to scene name, sceneSwitch config
      SwitchParams m_defaultSwitchParams;
      std::string m_tmpSceneName;
      unsigned long long m_sceneShowsGotosCounter;
      MobApp m_mobApp;
    };
  }
}

#endif
