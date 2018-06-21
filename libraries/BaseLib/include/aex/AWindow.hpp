/** @file AWindow.hpp
 *  @brief Definition of AWindow.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */


#ifndef __AexolGL__AWindow__
#define __AexolGL__AWindow__
#include <boost/signals2.hpp>
#include <exception>
#include <list>
#include <string>
#include <functional>
#include <aex/Logger.hpp>
#include <aex/SubsystemGuard.hpp>

namespace aex
{
class SubsystemGuard;
class AEX_PUBLIC_API FailedToCreateWindow: public std::exception
{
    const aex::string m_sdl_error;
public:
    explicit FailedToCreateWindow(const char* err) : m_sdl_error(aex::string {"Failed to create AWindow with error: "} .append(err)) {}
    virtual const char* what() const NOEXCEPT
    {
        
        return m_sdl_error.c_str();
    }
};
struct AEX_PUBLIC_API DisplayInfo
{
    uint32_t format; /**< pixel format */
    int w; /**< width */
    int h; /**< height */
    int refresh_rate; /**< refresh rate (or zero for unspecified) */

    DisplayInfo(uint32_t cFormat, int cW, int cH, int cRefresh_rate);

};
class AEX_PUBLIC_API AWindow
{

public:
        /* Supported orientations for android and iOS */
    static CONSTEXPR std::size_t PORTRAIT = 0x01;
    static CONSTEXPR std::size_t PORTRAIT_DOWN = PORTRAIT << 1;
    static CONSTEXPR std::size_t LANDSCAPE_LEFT = PORTRAIT << 2;
    static CONSTEXPR std::size_t LANDSCAPE_RIGHT = PORTRAIT << 3;
    
    typedef boost::signals2::signal<void()> window_focus_state_t;
    typedef boost::signals2::signal<void(std::size_t, std::size_t)> window_size_change_t;
    typedef boost::signals2::connection connection;
    
    AWindow();
    AWindow(const AWindow&) = delete;
    AWindow& operator=(const AWindow&) = delete;
    AWindow(AWindow&& win);
    AWindow& operator=(AWindow&& win);
    /**
     *	@brief	Create window and GL context.
     *
     *	@param 	width 	Window width.
     *	@param 	height 	Window height.
     *
     *	@return	New window.
     */
    static AWindow createWindow(uint16_t width, uint16_t height, const aex::string& Wname = "   ");
    static void initWindow(AWindow& window, uint16_t width, uint16_t height, const aex::string& Wname = "   ");

    void printGLVersion();
    void printDisplayInfo();
    void showCursor();
    void hideCursor();
    
    /**
     *	@brief	Get window width.
     *
     *	@return	Window width.
     */
    int32_t w() const
    {
        return m_w;
    };

    /**
     *	@brief	Get window height.
     *
     *	@return	Window height.
     */
    int32_t h() const
    {
        return m_h;
    };

    /**
     *	@brief	Get pointer to SDL_Window.
     *
     *	@return	Pointer to SDL_Window.
     */
    void addWindowChangeCallback(const std::function<void(const AWindow&)>& callback);
    void addWindowChangeCallback(std::function<void(const AWindow&)>&& callback);
    static DisplayInfo getDisplayInfo();
    bool visible()
    {
        return mVisible;
    }
    void* window()
    {
        return m_window;
    }
    void supportedOrientations(std::size_t orientations);
    connection addHideWindowListener(const window_focus_state_t::slot_type& listener);
    connection addShowWindowListener(const window_focus_state_t::slot_type& listener);
    connection addWindowOrientationChange(const window_size_change_t::slot_type& listener);
    connection addWindowSizeChange(const window_size_change_t::slot_type& listener);
    void setName(aex::string name);
    aex::string getName() const;
    virtual ~AWindow();

private:
    void stateChanged();
    void changeSize(int32_t w, int32_t h);
    void hide();
    void show();
    void sizeChanged(std::size_t w, std::size_t h);
    void orientationChanged(std::size_t w, std::size_t h);
    friend class AexOverlayViewImpl;
    friend AEX_PUBLIC_API AWindow* createWindow(uint16_t width, uint16_t height, const aex::string& Wname);
    friend void HideWindow(AWindow*);
    friend void ShowWindow(AWindow*);
    friend void ResizeWindow(AWindow*, std::size_t w, std::size_t h);
protected:
    virtual void init(uint16_t width, uint16_t height, const aex::string& Wname = "   ");
    virtual void makeWindow(std::size_t w, std::size_t h);
    
private:
    bool mVisible;
    std::list<std::function<void(const AWindow&)>> m_callbacks;
    window_focus_state_t mHideSig;
    window_focus_state_t mShowSig;
    window_size_change_t mResizeSig;
    window_size_change_t mOrientationSig;
    aex::string m_name;
    SubsystemGuard mVideoForWindow;
    
protected:
    void* m_window;         /* main window */
    int32_t m_w, m_h;             /* width and height */
};
AEX_PUBLIC_API AWindow* createWindow(uint16_t width, uint16_t height, const aex::string& Wname = "   ");
}

#endif /* defined(__AexolGL__AWindow__) */
