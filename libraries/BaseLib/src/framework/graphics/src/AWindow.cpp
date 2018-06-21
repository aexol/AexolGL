//
//  AWindow.cpp
//  AexolGL
//
//  Created by Kamil Matysiewicz on 19.08.2013.
//
//

#include "../include/ApiInternal.hpp"
#include <aex/platform_definitions.hpp>
#include <aex/GPUApi.hpp>
#include <aex/api_definitions.hpp>
#include <aex/Common.hpp>
#include <aex/AWindow.hpp>
#include <aex/utils.hpp>
#include <cds/init.h>
#include <cds/gc/hp.h>
#include <time.h>
#include <exception>
#include <stdexcept>
#include <utility>
#include <SDL.h>

namespace aex
{
    DisplayInfo::DisplayInfo(uint32_t cFormat, int cW, int cH, int cRefresh_rate) :
        format(cFormat),
        w(cW),
        h(cH),
        refresh_rate(cRefresh_rate)
    {
    }

    static int HideWindow(void* userdata, SDL_Event* e);
    static int ShowWindow(void* userdata, SDL_Event* e);
    static int ResizeWindow(void* userdata, SDL_Event* e);

    void HideWindow(AWindow* window);
    void ShowWindow(AWindow* window);
    void ResizeWindow(AWindow* window, std::size_t w, std::size_t h);
    AWindow::~AWindow()
    {
        DestroyAexContext();
        SDL_DelEventWatch(HideWindow, this);
        SDL_DelEventWatch(ShowWindow, this);
        SDL_DelEventWatch(ResizeWindow, this);

        if(m_window)
        {
            SDL_DestroyWindow(static_cast<SDL_Window*>(m_window));
        }
    }

    AWindow::AWindow() :
        mVisible(true),
        m_window(nullptr),
        m_w(0),
        m_h(0)
    {
        SDL_AddEventWatch(HideWindow, this);
        SDL_AddEventWatch(ShowWindow, this);
        SDL_AddEventWatch(ResizeWindow, this);
    }

    AWindow::AWindow(AWindow&& win) : AWindow()
    {
        std::swap(m_window, win.m_window);
        m_w = win.m_w;
        m_h = win.m_h;
    }

    AWindow&
    AWindow::operator=(AWindow&& win)
    {
        std::swap(m_window, win.m_window);
        m_w = win.m_w;
        m_h = win.m_h;
        return *this;
    }
    
    void
    AWindow::hide()
    {
        mVisible = false;
        mHideSig();
    }
    
    void
    AWindow::show()
    {
        mVisible = true;
        mShowSig();
    }

    AWindow
    AWindow::createWindow(uint16_t width, uint16_t height, const aex::string& Wname)
    {
        AWindow ret;
        AWindow::initWindow(ret, width, height , Wname);
        return ret;
    }

AEX_PUBLIC_API AWindow*
createWindow(uint16_t width, uint16_t height, const aex::string& Wname)
{
    AWindow *ret = new AWindow();
    AWindow::initWindow(*ret, width, height , Wname);
    return ret;
}

    void
    AWindow::stateChanged()
    {
        for(auto& a : m_callbacks)
        {
            a(*this);
        }
    }

    void
    AWindow::sizeChanged(std::size_t w, std::size_t h)
    {
        m_w = w;
        m_h = h;
        mResizeSig(w, h);
    }

    void AWindow::setName(aex::string name)
    {
        m_name = name;
    }

    aex::string AWindow::getName() const
    {
        return m_name;
    }

    void
    AWindow::addWindowChangeCallback(const std::function<void(const AWindow&)>& callback)
    {
        m_callbacks.push_back(callback);
    }

    void
    AWindow::addWindowChangeCallback(std::function<void(const AWindow&)>&& callback)
    {
        m_callbacks.push_back(std::move(callback));
    }

    AWindow::connection
    AWindow::addHideWindowListener(const window_focus_state_t::slot_type& listener)
    {
        return mHideSig.connect(listener);
    }

    AWindow::connection
    AWindow::addShowWindowListener(const window_focus_state_t::slot_type& listener)
    {
        return mShowSig.connect(listener);
    }

    AWindow::connection
    AWindow::addWindowSizeChange(const window_size_change_t::slot_type& listener)
    {
        return mResizeSig.connect(listener);
    }
    void
    AWindow::changeSize(int32_t w, int32_t h)
    {
        m_w = w;
        m_h = h;
    }

    void
    AWindow::initWindow(aex::AWindow& window, uint16_t width, uint16_t height, const aex::string& Wname)
    {
        window.init(width, height, Wname);
    }

    void
    AWindow::init(uint16_t width, uint16_t height, const aex::string& Wname)
    {
        
        setName(Wname);
        if(!mVideoForWindow.isEnabled(VideoSubsystem) && mVideoForWindow.init())
        {
            mVideoForWindow.start_video();
        }

        /* seed the random number generator */
        srand(time(NULL));
        /*
         request some OpenGL parameters
         that may speed drawing
         */

        makeWindow(width, height);
    }


    int HideWindow(void* userdata, SDL_Event* e)
    {
        if(e->type == SDL_WINDOWEVENT)
        {
            if(e->window.event == SDL_WINDOWEVENT_MINIMIZED)
            {
                AWindow* window = static_cast<AWindow*>(userdata);
                HideWindow(window);
            }
        }

        return 1;
    }

    void HideWindow(AWindow* window)
    {
        window->hide();
    }

    int ShowWindow(void* userdata, SDL_Event* e)
    {
        if(e->type == SDL_WINDOWEVENT)
        {
            if(e->window.event == SDL_WINDOWEVENT_RESTORED)
            {
                AWindow* window = static_cast<AWindow*>(userdata);
                ShowWindow(window);
            }
        }

        return 1;
    }
    void ShowWindow(AWindow* window)
    {
        window->show();
    }

    int ResizeWindow(void* userdata, SDL_Event* e)
    {
        if(e->type == SDL_WINDOWEVENT)
        {
            if(e->window.event == SDL_WINDOWEVENT_RESIZED)
            {
                AWindow* window = static_cast<AWindow*>(userdata);
                ResizeWindow(window, e->window.data1, e->window.data2);
            }
        }

        return 1;
    }
    void ResizeWindow(AWindow* window, std::size_t w, std::size_t h)
    {
        window->sizeChanged(w, h);
    }

    void AWindow::printGLVersion()
    {
        LOG("GLSL Version = ", glGetString(GL_SHADING_LANGUAGE_VERSION));
        LOG("GL Version = ", glGetString(GL_VERSION));
    }
    
     void AWindow::printDisplayInfo()
    {
        DisplayInfo display = getDisplayInfo();
        
        LOG("Display info: ");
        LOG("    pixel format: ", display.format);
        LOG("    width: ", display.w);
        LOG("    height: ", display.h);
        LOG("    refresh rate (or zero for unspecified): ", display.refresh_rate);
    }
    
    void AWindow::showCursor()
    {
        SDL_ShowCursor(1);
    }
    
    void AWindow::hideCursor()
    {
        SDL_ShowCursor(0);
    }
    
    DisplayInfo AWindow::getDisplayInfo()
    {
        SDL_DisplayMode current;
        int32_t displayCount = SDL_GetNumVideoDisplays();

        for(int32_t ii = 0; ii < displayCount; ++ii)
        {
            int should_be_zero = SDL_GetCurrentDisplayMode(ii, &current);

            if(should_be_zero != 0)
            {
                fatalError("Could not get display mode for video display: ");
            }
            LOG_DEBUG("DIsplay[",ii,"]: ", current.w, " - ", current.h);
        }
        
        DisplayInfo ret(current.format, current.w, current.h, current.refresh_rate);
        return ret;
    }

    void
    AWindow::makeWindow(std::size_t w, std::size_t h)
    {
        if(!mVideoForWindow.isEnabled(VideoSubsystem) && mVideoForWindow.init())
        {
            mVideoForWindow.start_video();
        }

        /* seed the random number generator */
        srand(time(NULL));

        /*
         request some OpenGL parameters
         that may speed drawing
         */
        /*
         * @FIXME: SDL_* Functions should be moved out of this module. m_window
         * should become unique_ptr with default deleter.
         */
#if __TARGET_PLATFORM__== __ANDROID_OS__ || __TARGET_PLATFORM__ == __APPLE_IOS__
        DisplayInfo di = getDisplayInfo();
 //       SDL_SetHint(SDL_HINT_ORIENTATIONS, "Portrait");
        m_w = di.w;
        m_h = di.h;
//#   if __TARGET_PLATFORM__ == __APPLE_IOS__
//#       ifdef HAVE_OPENGLES_ES2_GL_H
//        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES); 
//        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
//#       else
//        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES); 
//        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//#       endif
//#   endif
        m_window = SDL_CreateWindow(NULL, 0, 0, m_w, m_h,
                                           SDL_WINDOW_OPENGL |
                                           SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN);
#else
    /*FIXME: For Windows+ANGLE OpenGL ES Mask
     * must be set before windows is created.
     * There should be a better place to set it probably.
     */
                    int _sx = 0;
                    int _sy = 0;
    #if __TARGET_PLATFORM__ == __AEX_WIN32__
    #	if __TARGET_AEX_API__ == __GLES__
                    SDL_GL_SetAttribute(SDL_GL_CONTEXT_EGL, 1); 
                    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

    #   elif __TARGET_AEX_API__ == __GL__
                    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    #	endif //  __TARGET_AEX_API__ == __GLES__
                    _sx = 0;
                    _sy = 60;
    #endif //  __TARGET_PLATFORM__ == __AEX_WIN32__
            m_w = w;
            m_h = h;
            const char* title = m_name.c_str();
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

            m_window = SDL_CreateWindow(title, _sx, _sy, m_w, m_h,
                                               SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN    );
#endif
        if(!m_window) throw FailedToCreateWindow
        {
            SDL_GetError()
        };

        InitAexContext(*this, m_window);
        setViewPort(m_w, m_h);
        LOG_DEBUG("VIEWPORT: ", m_w, " - ", m_h);
    }
}
