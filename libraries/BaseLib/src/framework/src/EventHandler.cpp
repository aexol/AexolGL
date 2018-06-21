#include <aex/EventHandler.hpp>

namespace aex
{
    EventHandler::~EventHandler()
    {
        unregisterObject(this);
    }

    void
    EventHandler::handleEvent(const GameEvent* event)
    {
        auto it = m_handlers.find(TypeInfo(typeid(*event)));

        if(it != m_handlers.end())
        {
            for(auto iter = it->second.begin(); iter != it->second.end(); iter++)
            {
                while(iter != it->second.end() && !iter->Active)
                {
                    iter = it->second.erase(iter);
                }

                if(iter != it->second.end())
                {
                    iter->handle->exec(event);
                }
            }
        }
    }
}

