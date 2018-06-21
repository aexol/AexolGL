#include "aex/gui/SceneManager.hpp"
#include <sstream>
#include <algorithm>
//#include <stdexcept>

namespace aex
{
  namespace gui
  {

    LayerFactory::LayerFactory()
    {
    }

    LayerFactory::LayerFactory(
      MobApp::LEVEL level,
      const std::string& name,
      const std::function<std::unique_ptr<Layer>()>& factoryMethod
    ):
    m_level(level),
    m_name(name),
    m_factoryMethod(factoryMethod)
    {
    }

    LayerFactory::~LayerFactory()
    {
    }

    Scene::Scene()
    {
    }

    Scene::Scene(const std::string& name) :
    m_name(name)
    {
    }

    Scene::Scene(const std::string& name, std::vector<std::string> layersNames) :
    m_name(name)
    {
      for(const auto& ln : layersNames)
      {
        addLayer(ln);
      }
    }

    Scene::~Scene()
    {
    }

    const std::string& Scene::getName() const
    {
      return m_name;
    }

    void Scene::addLayer(const std::string& layerName)
    {
      // check if exists
      if( std::find(m_layers.begin(), m_layers.end(), layerName) != m_layers.end())
        throw std::runtime_error("addLayer(...), provided layerName exists in that scene instance");

      m_layers.push_back(layerName);
    }

    const std::vector<std::string>& Scene::getLayersNames() const
    {
      return m_layers;
    }

    SwitchParams::SwitchParams() :
    m_delaySeconds(0.5f),
    m_effect(MobApp::EFFECT::SLIDE_LEFT),
    m_easing(aex::math::Easings::Cubic::easeInOut),
    m_instantSwitch(false)
    {
    }

    SwitchParams::SwitchParams(float delaySeconds,
      MobApp::EFFECT effect,
      const std::function<float(float,float,float,float)>& easing):
     m_delaySeconds(delaySeconds),
     m_effect(effect),
     m_easing(easing),
     m_instantSwitch(false)
    {
    }

    SwitchParams::~SwitchParams()
    {
    }

    SceneSwitch::SceneSwitch()
    {
    }

    SceneSwitch::~SceneSwitch()
    {
    }

    void SceneSwitch::setParams(MobApp::LEVEL onLvl, const SwitchParams& params)
    {
      m_params[onLvl] = params;
    }

    bool SceneSwitch::hasParams(MobApp::LEVEL onLvl)
    {
      return m_params.find(onLvl) != m_params.end();
    }

    const SwitchParams& SceneSwitch::getParams(MobApp::LEVEL onLvl)
    {
      return m_params.at(onLvl);
    }

    SceneManager::SceneManager(aex::gui::Frame* frame) :
    m_tmpSceneName(""),
    m_sceneShowsGotosCounter(0)
    {
      frame->setMainPanel(&m_mobApp);
      // gen empty layers factories for slides between scenes with different layers lvls (first scene have, second have not layer on specified lvl, neet any layer to gotoScene working);
      for(int lvl = 0 ; lvl < MobApp::LEVEL::L_LAST+1 ; lvl++)
      {
        addLayerFactory(LayerFactory((MobApp::LEVEL)lvl, getEmptyLayerName((MobApp::LEVEL)lvl), []()->std::unique_ptr<Layer>{
          return std::unique_ptr<Layer>( new EmptyLayer()); }
        ));

      }
    }

    SceneManager::~SceneManager()
    {
    }

    void SceneManager::addLayerFactory(const LayerFactory& layerFactory)
    {
      // check if names of layers are not repeated
      if(m_layersFactories.find(layerFactory.m_name) != m_layersFactories.end())
        throw std::runtime_error("addLayerFactory(...), layerFactory with provided layer name exists in SceneManager instance");

      // now is just instant, creating, need to add possibility to clear layers memory if are not needed
      m_layersFactories[layerFactory.m_name] = layerFactory;
      m_mobApp.addLayer(layerFactory.m_name, layerFactory.m_factoryMethod(), layerFactory.m_level); // layer factory method invocation
    }

    void SceneManager::addScene(const Scene& scene)
    {
      // check if scene with that name exists:
      if ( m_scenes.find(scene.getName()) != m_scenes.end() ) {
        std::stringstream ss;
        ss << "Scene with the name \"" << scene.getName() << "\" exists in SceneManager instance";
        throw std::runtime_error(ss.str());
      }

      // check if layers in scene are provided in LayerFactory
      const std::vector<std::string>& layersNames = scene.getLayersNames();
      for(size_t i = 0 ; i < layersNames.size() ; i++)
      {
          if(m_layersFactories.find(layersNames[i]) == m_layersFactories.end() )
          {
            throw std::runtime_error("addScene(...) provided scene contains layer name, that not exists in SceneManager. Use addLayerFactory(...) method in SceneManager with needed layers before addScene");
            return;
          }
      }

      m_scenes[scene.getName()] = scene;
    }

    void SceneManager::setSwitch(const std::string& fromScene, const std::string& toScene, const SceneSwitch& sceneSwitch)
    {
      if(!sceneExists(fromScene) || !sceneExists(toScene))
      {
        throw std::runtime_error("Try to set switch params with not existing scene. Add scenes before using setSwitch(...) method");
      }
      if(m_switches.find(fromScene) == m_switches.end() )
        m_switches[fromScene] = std::map<std::string, SceneSwitch>();

      m_switches[fromScene][toScene] = sceneSwitch;
    }

    void SceneManager::gotoScene(const std::string& sceneName, const std::function<void()>& withAction)
    {
      // check tmp setted scene, if in new scene are empty layers, that are not exists in new, use tmp empty layers
      if(!sceneExists(sceneName))
        throw std::runtime_error("gotoScene(...), scene with provided name not exists");

      if(isAnyMobAppLayerTweening())
        return;

      if(m_sceneShowsGotosCounter == 0){
        showScene(sceneName, withAction);
        return;
      }

      withAction(); // cb invocation

      // check if user set switch params between tmp and next scene
      SceneSwitch* sceneSwitch = nullptr;
      if(m_switches.find(m_tmpSceneName) != m_switches.end())
      {
        std::map<std::string, SceneSwitch>& swMap = m_switches.at(m_tmpSceneName);
        if(swMap.find(sceneName) != swMap.end())
        {
          sceneSwitch = &swMap[sceneName];
        }
      }


      const Scene& nextScene = m_scenes.at(sceneName);
      const std::vector<std::string>& names = nextScene.getLayersNames(); // all layers (should be every on another MobApp::LEVEL)
      for(size_t i = 0 ; i < names.size() ; i++)
      {
        // check if it is not the same layer visible
        if(m_mobApp.layerVisible(names[i]))
          continue;

        // go to new layers in next scene
        MobApp::LEVEL level = m_layersFactories.at(names[i]).m_level;
        if(sceneSwitch && sceneSwitch->hasParams(level))
        {
          const SwitchParams& sp = sceneSwitch->getParams(level);
          m_mobApp.gotoLayer(names[i], sp.m_effect, sp.m_delaySeconds, sp.m_easing);
        }else{
          const SwitchParams& sp = m_defaultSwitchParams;
          m_mobApp.gotoLayer(names[i], sp.m_effect, sp.m_delaySeconds, sp.m_easing);
        }
      }

      // if not all layers all covered, push previous layers using empty layers
      std::vector<MobApp::LEVEL> m_notCoveredLvls = getNotCoveredLevels(m_tmpSceneName, sceneName);
      for(auto lvl : m_notCoveredLvls)
      {
        LOG_DEBUG("NOT COVERED LVL: ", lvl);
        if(sceneSwitch && sceneSwitch->hasParams(lvl))
        {
          const SwitchParams& sp = sceneSwitch->getParams(lvl);
          m_mobApp.gotoLayer(getEmptyLayerName(lvl), sp.m_effect, sp.m_delaySeconds, sp.m_easing);
        }else{
          const SwitchParams& sp = m_defaultSwitchParams;
          m_mobApp.gotoLayer(getEmptyLayerName(lvl), sp.m_effect, sp.m_delaySeconds, sp.m_easing);
        }
      }

      m_sceneShowsGotosCounter++;
      m_tmpSceneName = sceneName;
    }

    void SceneManager::showScene(const std::string& sceneName, const std::function<void()>& withAction)
    {
      if(!sceneExists(sceneName))
        throw std::runtime_error("showScene(...), scene with provided name not exists");

      if(isAnyMobAppLayerTweening())
        return;

      m_mobApp.clearAllLayerLevels(); // removes from visible panel
      withAction();
      const Scene& nextScene = m_scenes.at(sceneName);
      const std::vector<std::string>& names =  nextScene.getLayersNames();
      for(size_t i = 0 ; i < names.size() ; i++)
      {
        m_mobApp.showLayer(names[i]);
      }

      m_sceneShowsGotosCounter++;
      m_tmpSceneName = sceneName;
    }

    void SceneManager::setDefaultSwitchParams(const SwitchParams& params)
    {
      m_defaultSwitchParams = params;
    }

    const SwitchParams& SceneManager::getDefaultSwitchParams() const
    {
      return m_defaultSwitchParams;
    }

    bool SceneManager::isSwitchingNow() const
    {
      return isAnyMobAppLayerTweening();
    }

    bool SceneManager::sceneExists(const std::string& sceneName)
    {
      return m_scenes.find(sceneName) != m_scenes.end();
    }

    bool SceneManager::isAnyMobAppLayerTweening() const
    {
      for(int level = MobApp::LEVEL::L0 ; level < MobApp::LEVEL::L_LAST+1 ; level++)
      {
        if(m_mobApp.isLevelTweening( (MobApp::LEVEL)level ))
          return true;
      }
      return false;
    }

    std::vector<MobApp::LEVEL> SceneManager::getNotCoveredLevels(const std::string& fromSceneName, const std::string& toSceneName)
    {
      const Scene& fromScene = m_scenes.at(fromSceneName);
      const Scene& toScene = m_scenes.at(toSceneName);

      const std::vector<std::string>& fromLayersNames = fromScene.getLayersNames();
      const std::vector<std::string>& toLayersNames = toScene.getLayersNames();

      std::map<MobApp::LEVEL, bool> fromLevelsMap;
      std::map<MobApp::LEVEL, bool> toLevelsMap;

      for(size_t i = 0 ; i < fromLayersNames.size() ; i++)
      {
        MobApp::LEVEL lvl = m_layersFactories.at(fromLayersNames[i]).m_level;
        fromLevelsMap[lvl] = true;
      }

      for(size_t i = 0 ; i < toLayersNames.size() ; i++)
      {
        MobApp::LEVEL lvl = m_layersFactories.at(toLayersNames[i]).m_level;
        toLevelsMap[lvl] = true;
      }

      std::vector<MobApp::LEVEL> retLevels;
      for (auto& pair : fromLevelsMap)
      {
        if( toLevelsMap.find( pair.first ) == toLevelsMap.end() )
        {
          retLevels.push_back(pair.first);
        }
      }

      return retLevels;
    }

    std::string SceneManager::getEmptyLayerName(const MobApp::LEVEL lvl) const
    {
      std::stringstream sstr;
      sstr << "empty_lAyEr" << (int)lvl;
      return sstr.str();
    }
  }
}
