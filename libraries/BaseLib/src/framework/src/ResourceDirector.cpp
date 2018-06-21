#include <aex/ResourceDirector.hpp>
namespace aex
{
    std::unique_ptr<ResourceDirector> ResourceDirector::m_instance;
    std::once_flag ResourceDirector::m_flag;
    ResourceDirector::ResourceDirector() : m_handlesBuffer(0){}
    ResourceDirector::~ResourceDirector()
    {
        // Remove all cached handles.
        handle_type* h;
        while(m_handlesBuffer.pop(h))
            delete h;
    }
    ResourceDirector&
    ResourceDirector::GetInstance()
    {
        std::call_once(m_flag, []
        {
            m_instance.reset(new ResourceDirector);
        });
        return *m_instance.get();
    }
}
