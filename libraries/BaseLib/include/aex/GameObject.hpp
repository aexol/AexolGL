#ifndef AEXOLGL_GAMEOBJECT_H
#define AEXOLGL_GAMEOBJECT_H

#include <aex/EventHandler.hpp>
#include <aex/Common.hpp>

namespace aex
{
    class GameObject;

    typedef GameObject* GameObjectPtr;
//!
//! Convieniece empty class to make separating
//! local and global events more plain.
//!
    class AEX_PUBLIC_API GameObject : public EventHandler
    {
    private:
        aex::string m_name;
    public:
        GameObject(const aex::string& name) : m_name(name) {}
        static unique_ptr<GameObject> makeGameObject(const aex::string& name);
        const aex::string& getName() const
        {
            return m_name;
        }
        virtual ~GameObject();
    };

}

#endif
