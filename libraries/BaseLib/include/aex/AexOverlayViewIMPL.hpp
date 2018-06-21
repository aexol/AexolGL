#ifndef __AEX_OVERLAY_VIEW_IMPL_HPP
#define __AEX_OVERLAY_VIEW_IMPL_HPP
#include <aex/AexOverlaySubView.hpp>
#include <boost/uuid/uuid.hpp>
#include <map>
#include <memory>
namespace aex
{
    class AWindow;
    class AEX_PUBLIC_API AexOverlayViewImpl
    {
    public:
        AexOverlayViewImpl() : self(nullptr) {};
        void init(const AWindow& window);
        void addView(AexOverlaySubViewIMPL* subview);
        ~AexOverlayViewImpl();
    private:
        AexOverlayViewImpl(const AexOverlayViewImpl&) = delete;
        AexOverlayViewImpl(AexOverlayViewImpl&&) = delete;
        void* self;
    };
}

#endif
