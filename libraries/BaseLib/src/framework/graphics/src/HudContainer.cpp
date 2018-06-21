#include <aex/HudContainer.hpp>
#include <json/json.h>

#include <aex/Common.hpp>
#include <aex/FileManager.hpp>

namespace aex
{
    namespace gui
    {
        using HudContainerPtr = HudContainer::HudContainerPtr;
        namespace {
            static void parseLevel(HudContainerPtr& hcParent, Json::Value& parentElem, Hud_ptr hud)
            {
                Json::Value::Members names = parentElem.getMemberNames();
                for(auto& name : names){
                    HudElementPtr he = hud->getElementByName(name);
                    if (he)
                    {
                        //LOG_DEBUG("adding ", name, " to parent: ", hcParent->getHudElemName());
                        HudContainerPtr hcChild = aex::make_shared<HudContainer>(he);
                        hcParent->add(hcChild);
                        
                        // check that this elem have a childs
                        Json::Value childElem = parentElem[name];
                        if(childElem.getMemberNames().size() > 0){
                            parseLevel(hcChild, childElem, hud);
                        }
                        
                    }else{
                        LOG_ERR("Provided hud doesnt have elem with name: ", name," can't reed childs of that elem");
                    }
                }
            }
            
            static void moveAndScaleIfNeeded(HudContainerPtr& container, Json::Value& root){
                Json::Value scaleVal = root["scale"];
                if( ! scaleVal.empty()){
                    float scaleX = ( ! scaleVal["x"].empty()) ? scaleVal["x"].asFloat() : 1.0;
                    float scaleY = ( ! scaleVal["y"].empty()) ? scaleVal["y"].asFloat() : 1.0;
                    bool centerX = ( ! scaleVal["centerX"].empty()) ? scaleVal["centerX"].asBool() : false;
                    bool centerY = ( ! scaleVal["centerY"].empty()) ? scaleVal["centerY"].asBool() : false;
                    
                    container->scale(scaleX, scaleY, centerX, centerY);
                }
                
                Json::Value moveVal = root["move"];
                if( ! scaleVal.empty()){
                    float moveX = ( ! moveVal["x"].empty()) ? moveVal["x"].asFloat() : 0.0;
                    float moveY = ( ! moveVal["y"].empty()) ? moveVal["y"].asFloat() : 0.0;
                    
                    container->move(moveX, moveY);
                }
            }
        }

        HudContainer::HudContainer() :
        hudElem(nullptr)
        {
        }

        HudContainer::HudContainer(HudElementPtr hudElem) :
        hudElem
        {
            hudElem
        }
        {
        }

        HudContainer::HudContainerPtr HudContainer::getContainerBy(const aex::string& name)
        {
            for(auto& child : childs){
                if(child->getHudElemName() == name){
                    return child;
                }
                else{
                    return child->getContainerBy(name);
                }
            }

            return nullptr;
        }

        HudContainer::HudContainerPtr HudContainer::getContainerBy(const HudElementPtr& hudElem)
        {
            return this->getContainerBy(hudElem->getName());
        }

        bool HudContainer::empty() const
        {
            if (!hudElem)
            {
                if (childs.empty())
                {
                    return true;
                }
            }
            return false;
        }

        void HudContainer::add(const HudContainerPtr& child)
        {
            childs.push_back(child);
        }

        void HudContainer::scale(float scaleX, float scaleY, bool centerX, bool centerY)
        {
            if((scaleX == 0) && (scaleY ==0))
                return;

            if (hudElem)
            {
                NormalArea oldArea = getHudElementArea(hudElem);
                NormalArea newArea = oldArea;
                newArea.scale(scaleX, scaleY);
                setHudElementUsingArea(hudElem, newArea);

                for (auto& hc : childs)
                {
                    hc->scaleByParent(oldArea, newArea, scaleX, scaleY);
                }

                //LOG_DEBUG("before CENTERING .....................", centerX, centerY);

                if(centerX){
                   float moveX = ((scaleX-1)/2) * oldArea.size.x();
                   this->move(-moveX, 0.0);
                }

                if(centerY){
                    float moveY = ((scaleY-1)/2) * oldArea.size.y();
                    this->move(0.0, -moveY);
                }

            } else
            {
                // if root, scale all childs like they are parents
                for (auto& hc : childs)
                {
                    hc->scale(scaleX, scaleY, centerX, centerY);
                }
            }
        }

        void HudContainer::move(float moveX, float moveY)
        {
            if((moveX == 0) && (moveY == 0))
                return;

            if (hudElem != nullptr)
            {
                NormalArea oldArea = getHudElementArea(hudElem);
                NormalArea newArea = oldArea;
                newArea.move(moveX, moveY);
                setHudElementUsingArea(hudElem, newArea);

                for (auto& hc : childs)
                {
                    hc->moveByParent(moveX, moveY);
                }
            } else
            {
                // if root, move all childs like they are parents
                for (auto& hc : childs)
                {
                    hc->move(moveX, moveY);
                }
            }
        }

        // private:

        void HudContainer::scaleByParent(const NormalArea& oldParentArea, const NormalArea& newParentArea, float scaleX, float scaleY)
        {
            if (hudElem)
            {
                NormalArea oldArea = getHudElementArea(hudElem);
                NormalArea newArea = oldArea;
                newArea.scale(scaleX, scaleY);
                float oldDistX = oldArea.pos.x() - oldParentArea.pos.x();
                float newDistX = oldDistX * scaleX;
                float oldDistY = oldArea.pos.y() - oldParentArea.pos.y();
                float newDistY = oldDistY * scaleY;
                newArea.pos.x() = newParentArea.pos.x() + newDistX; // TODO check that need oldParentArea or new ???
                newArea.pos.y() = newParentArea.pos.y() + newDistY;
                setHudElementUsingArea(hudElem, newArea);

                for (auto& hc : childs)
                {
                    hc->scaleByParent(oldArea, newArea, scaleX, scaleY);
                }

            } else
            {
                LOG_ERR("child container doesnt have hudElem setted");
//                // if noname container, scale all childs like they are roots
//                for (auto& hc : childs)
//                {
//                    hc->scale(scaleX, scaleY, );
//                }
            }
        }

        void HudContainer::moveByParent(float moveX, float moveY)
        {

            if (hudElem)
            {
                NormalArea oldArea = getHudElementArea(hudElem);
                NormalArea newArea = oldArea;
                newArea.move(moveX, moveY);
                setHudElementUsingArea(hudElem, newArea);

                for (auto& hc : childs)
                {
                    hc->moveByParent(moveX, moveY);
                }
            }
        }

        NormalArea HudContainer::getHudElementArea(HudElementPtr hudElem)
        {
            return NormalArea(FPoint2D(hudElem->getPosX(), hudElem->getPosY()), FPoint2D(hudElem->getSizeX(), hudElem->getSizeY()));
        }

        void HudContainer::setHudElementUsingArea(HudElementPtr hudEl, const NormalArea& area)
        {
            hudEl->setPos(area.pos.x(), area.pos.y());
            hudEl->setSize(area.size.x(), area.size.y());
        }

        aex::string HudContainer::getHudElemName() const{
            if(hudElem){
                return hudElem->getName();
            }

            return "__NO_NAME__";
        }

         HudContainer::HudContainerPtr HudContainer::readFromJSON(const aex::string& jsonPath, Hud_ptr hud)
        {
            FileManager fManager;
            auto p = fManager.work_dir();
            typedef boost::filesystem::path path;
            auto fileHandle = fManager.open_read_file(p /= path{jsonPath});
            if (fileHandle.is_open())
            {
                Json::Value root;
                Json::Reader reader;
                bool parsingSuccessful = reader.parse(fileHandle, root);

                // fileHandle.close();
                if (!parsingSuccessful)
                {
                    LOG_DEBUG(__LINE__);
                    return nullptr;
                }

                Json::Value elements = root["container"];
                //  LOG("elements in container: ", elements.getMemberNames().size());

                HudContainerPtr rootContainer = aex::make_shared<HudContainer>();
                parseLevel(rootContainer, elements, hud);
                moveAndScaleIfNeeded(rootContainer, root);

                return rootContainer;
            }
            fileHandle.close();

            return nullptr;
        }

        MoveHudContainerAnimation::MoveHudContainerAnimation(HudContainer& hudContainer) :
        m_hudContainer(hudContainer),
        m_bIsAnimating(false),
        m_deltaX(0.0f),
        m_deltaY(0.0f),
        m_animTimeMills(1000)
        {
        }

        MoveHudContainerAnimation::~MoveHudContainerAnimation()
        {
        }

        void MoveHudContainerAnimation::startAnimation(const std::function<void()>& onEndCb)
        {
            if (isAnimating())
                return;

            auto duration = std::chrono::system_clock::now().time_since_epoch();
            m_startTimestamp = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            m_lastTimestamp = m_startTimestamp;
            m_onAnimationEndCb = onEndCb;
            m_bIsAnimating = true;
        }

        bool MoveHudContainerAnimation::isAnimating() const
        {
            return m_bIsAnimating;
        }

        void MoveHudContainerAnimation::setVectorShiftAndTime(float dX, float dY, float seconds)
        {
            m_deltaX = dX;
            m_deltaY = dY;
            m_animTimeMills = seconds * 1000.0f;
        }

        void MoveHudContainerAnimation::step()
        {
            if (!isAnimating())
                return;

            auto duration = std::chrono::system_clock::now().time_since_epoch();
            long long unsigned tmpTimestamp = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            long long unsigned durationMills;
            double animPercent = 0.0f;
            if ((tmpTimestamp - m_startTimestamp) <= m_animTimeMills)
            {
                durationMills = tmpTimestamp - m_lastTimestamp;
                animPercent = (double) durationMills / m_animTimeMills;
            } else
            {
                durationMills = m_animTimeMills - (m_lastTimestamp - m_startTimestamp);
                if (durationMills > 0)
                {
                    animPercent = (double) durationMills / m_animTimeMills;
                }
                m_bIsAnimating = false;
            }
            
            m_lastTimestamp = tmpTimestamp;
            float dX = animPercent * m_deltaX;
            float dY = animPercent * m_deltaY;
            //LOG_DEBUG("dX:", dX, ", dY:", dY);
            if(animPercent != 0.0f)
                m_hudContainer.move(dX, dY);
            
            if(m_bIsAnimating == false){
                m_onAnimationEndCb();
            }
        }

        bool MoveHudContainerAnimation::init()
        {
            return true; // positive init
        }

        void MoveHudContainerAnimation::pause()
        {
            // nothing
        }

        void MoveHudContainerAnimation::resume()
        {
            // nothing
        }

        void MoveHudContainerAnimation::update()
        {
            step();
        }

        void MoveHudContainerAnimation::finish()
        {

        }
        
    }
}
