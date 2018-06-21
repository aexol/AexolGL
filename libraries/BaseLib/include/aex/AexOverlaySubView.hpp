#ifndef __AEX_OVERLAY_SUBVIEW_HPP
#define __AEX_OVERLAY_SUBVIEW_HPP

#include <aex/Common.hpp>

#include <type_traits>
#include <memory>
#include <functional>

namespace aex
{
class AexOverlayViewImpl;
class AEX_PUBLIC_API AexOverlaySubViewIMPL
{
public:
    void* getViewObject()
    {
        return self;
    }
    void onViewAdded(void* parentImpl);
    virtual ~AexOverlaySubViewIMPL() = 0;
protected:
    AexOverlaySubViewIMPL();
    void* self;
    std::function<void(void* const)> m_callback;
};
inline AexOverlaySubViewIMPL::~AexOverlaySubViewIMPL() {}

template<class IMPL>
class AEX_PUBLIC_API AexOverlaySubView
{
    static_assert(std::is_base_of<AexOverlaySubViewIMPL, IMPL>::value, "Cannot convert type to AexOverlaySubViewIMPL.");
public:
    AexOverlaySubView() : m_impl {new IMPL{}} {}
    IMPL& operator*()
    {
        return static_cast<IMPL&>(*m_impl.get());
    }
    IMPL* operator->()
    {
        return static_cast<IMPL*>(m_impl.get());
    }
    IMPL* get()
    {
        return static_cast<IMPL*>(m_impl.get());
    }
    void* getViewObject()
    {
        if(m_impl)
        {
            return m_impl->getViewObject();
        }

        return nullptr;
    }
private:
    std::unique_ptr<AexOverlaySubViewIMPL> m_impl;
};

}

#endif
