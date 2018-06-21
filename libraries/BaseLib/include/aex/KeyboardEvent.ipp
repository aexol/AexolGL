#ifndef __AEX_KEYBOARDEVENT_IPP
#define __AEX_KEYBOARDEVENT_IPP
#include "KeyboardEvent.hpp"
namespace aex
{
    template<class T>
    T&
    NamedKeyDispatcher<T>::getHandler(const std::string& key)
    {
        uint32_t scancode = nameToScancode(key);
        //Name supported.
        //Check if handler already in map.
        T* handler = getHandler(scancode);

        //Return handler if already in map, add new and return it if not.
        return handler != nullptr ? *handler : (m_dispatchLookup[scancode] = T());
    }

    template<class T>
    const T*
    NamedKeyDispatcher<T>::getHandler(uint32_t scancode) const
    {
        auto key_it = m_dispatchLookup.find(scancode);
        if(key_it == m_dispatchLookup.end())
            return nullptr;
        return &(key_it->second);
    }

    template<class T>
    T*
    NamedKeyDispatcher<T>::getHandler(uint32_t scancode)
    {
        const NamedKeyDispatcher& self = *this;
        return const_cast<T*>(self.getHandler(scancode)); // This const_cast is fine.
    }
}
#endif // __AEX_KEYBOARDEVENT_IPP
