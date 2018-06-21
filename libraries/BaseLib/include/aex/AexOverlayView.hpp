#ifndef __AEX_OVERLAY_VIEW_HPP
#define __AEX_OVERLAY_VIEW_HPP

#include <aex/AexOverlayViewIMPL.hpp>
#include <aex/AexOverlaySubView.hpp>
#include <exception>
#include <memory>
#include <string>

namespace aex
{

    class AEX_PUBLIC_API AexOverlayException : public std::exception
    {
        const aex::string m_msg;
    public:
        AexOverlayException() = delete;
        AexOverlayException(const aex::string& s): m_msg(s) {};
        AexOverlayException(aex::string&& s): m_msg(std::move(s)) {};
        const char* what() const NOEXCEPT;
    };

    class AEX_PUBLIC_API AexOverlayView
    {
        typedef boost::uuids::uuid uuid;
    public:
        AexOverlayView(const AWindow& window);
        template<class IMPL>
        void addView(const std::shared_ptr<AexOverlaySubView<IMPL>>& subview)
        {
            {
                m_impl.addView(subview->get());
            }
        }
        void findView(uuid uid);
    private:
        AexOverlayViewImpl m_impl;
    };
}

#endif
