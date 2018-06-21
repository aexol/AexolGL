#include <aex/GameObject.hpp>
#include <aex/Logger.hpp>

namespace aex
{
    unique_ptr<GameObject>
    GameObject::makeGameObject(const aex::string& name)
    {
        return unique_ptr<GameObject>(new GameObject(name));
    }

    GameObject::~GameObject()
    {
        //LOG_DEBUG(m_name , "Dies(GameObject)");
    }
}
