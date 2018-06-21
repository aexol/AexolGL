#include "../graphics/include/ApiInternal.hpp"
#include <aex/SubsystemGuard.hpp>
#include <aex/Logger.hpp>
#include <SDL.h>
#include <SDL_ttf.h>
#include <sstream>
#include <type_traits>
#include <cassert>

namespace aex
{
static CONSTEXPR SubsystemIDType FirstNonSDLSubsystem = (ControllerSubsystem << 1);
SubsystemGuard::proxy_builder_type StateProxyBuilder{};

CONSTEXPR uint32_t sdl_video_flag(SubsystemIDType flag)
{
    return (flag & VideoSubsystem) != 0 ? SDL_INIT_VIDEO : 0;
}

CONSTEXPR uint32_t sdl_audio_flag(SubsystemIDType flag)
{
    return (flag & AudioSubsystem) != 0 ? SDL_INIT_AUDIO : 0;
}

CONSTEXPR uint32_t sdl_events_flag(SubsystemIDType flag)
{
    return (flag & EventsSubsystem) != 0 ? SDL_INIT_EVENTS : 0;
}

CONSTEXPR uint32_t sdl_timer_flag(SubsystemIDType flag)
{
    return (flag & TimerSubsystem) != 0 ? SDL_INIT_TIMER : 0;
}

CONSTEXPR uint32_t sdl_joystick_flag(SubsystemIDType flag)
{
    return (flag & JoystickSubsystem) != 0 ? SDL_INIT_JOYSTICK : 0;
}

CONSTEXPR uint32_t sdl_haptic_flag(SubsystemIDType flag)
{
    return (flag & HapticSubsystem) != 0 ? SDL_INIT_HAPTIC : 0;
}

CONSTEXPR uint32_t sdl_controller_flag(SubsystemIDType flag)
{
    return (flag & ControllerSubsystem) != 0 ? SDL_INIT_GAMECONTROLLER : 0;
}

CONSTEXPR uint32_t sdl_flags(SubsystemIDType flag)
{
    return sdl_video_flag(flag) | sdl_audio_flag(flag) | sdl_events_flag(flag) | sdl_timer_flag(flag) | sdl_joystick_flag(flag) | sdl_haptic_flag(flag) | sdl_controller_flag(flag);
}

bool SDLInitDispatch(const SubsystemIDType flag)
{
    return flag ? SDL_InitSubSystem(sdl_flags(flag)) == 0 : true;
}

bool SDLTTFInitDispatch(const SubsystemIDType flag)
{
    // TTF_WasInit() Returns: 1 if initialized, 0 if not; 
    // TTF_Init() Returns 0 on succes, -1 on error;
    // TTF_GetError() get %s errors info string;
    assert(flag & FontSubsystem);
    return (TTF_WasInit() == 1) ? true : (TTF_Init() == 0);
}

typedef bool(*SubsystemInitFuncType)(SubsystemIDType flag);
SubsystemInitFuncType dispatchInitFunctions[1]
{
    SDLTTFInitDispatch
};


bool InitSubsystem(const SubsystemIDType subsystemFlag)
{
    const SubsystemIDType sdl_flags_mask = FirstNonSDLSubsystem-1;
    const SubsystemIDType sdl_flags = subsystemFlag & sdl_flags_mask;
    bool success = SDLInitDispatch(sdl_flags);
    for(SubsystemIDType i = FirstNonSDLSubsystem, j = 0; i < UnsupportedSubsystem; i<<=1, j++)
        if(subsystemFlag & i)
            success &= dispatchInitFunctions[j](i);
    return success;
}


bool SDLWasInitDispatch(const SubsystemIDType flag)
{
    return flag ? SDL_WasInit(sdl_flags(flag)) : true;
}

bool SDLWasTTFInitDispatch(const SubsystemIDType flag)
{
    assert(flag & FontSubsystem);
    return TTF_WasInit();
}

typedef bool(*SubsystemWasInitFuncType)(SubsystemIDType flag);
SubsystemWasInitFuncType dispatchWasInitFunctions[1]
{
    SDLWasTTFInitDispatch
};

static bool WasInitSubsystem(const SubsystemIDType subsystemFlag)
{
    const SubsystemIDType sdl_flags_mask = FirstNonSDLSubsystem-1;
    const SubsystemIDType sdl_flags = subsystemFlag & sdl_flags_mask;
    bool success = SDLWasInitDispatch(sdl_flags);
    for(SubsystemIDType i = FirstNonSDLSubsystem, j = 0; i < UnsupportedSubsystem; i<<=1, j++)
        if(subsystemFlag & i)
            success &= dispatchWasInitFunctions[j](i);
    return success;
}


void SDLQuitDispatch(const SubsystemIDType flag)
{
    SDL_QuitSubSystem(sdl_flags(flag));
}

void SDLTTFQuitDispatch(const SubsystemIDType flag)
{
    UNUSED(flag);
    TTF_Quit();
}

typedef void(*SubsystemQuitFuncType)(SubsystemIDType flag);
SubsystemQuitFuncType dispatchQuitFunctions[1]
{
    SDLTTFQuitDispatch
};

static void QuitSubsystem(const SubsystemIDType subsystemFlag)
{
    const SubsystemIDType sdl_flags_mask = FirstNonSDLSubsystem - 1;
    const SubsystemIDType sdl_flags = subsystemFlag & sdl_flags_mask;
    SDLQuitDispatch(sdl_flags);
    for(SubsystemIDType i = FirstNonSDLSubsystem, j = 0; i < UnsupportedSubsystem; i<<=1, j++)
        if(subsystemFlag & i)
           dispatchQuitFunctions[j](i);
}

SubsystemState::SubsystemState() : m_mutex{}, m_state{}, m_subsystemsRefCount{}, m_impl{}{}

bool 
SubsystemState::wasInit(SubsystemIDType subsystem) const
{
    std::lock_guard<std::mutex> lock{m_mutex};
    return ((m_state & subsystem) != 0);
}

bool 
SubsystemState::init(SubsystemIDType subsystem)
{
    bool wasInit = this->wasInit(subsystem);
    if(!wasInit)
    {
        wasInit = m_impl && m_impl->init(subsystem);
    }

    if(wasInit)
    {
        std::lock_guard<std::mutex> lock{m_mutex};
        m_state |= subsystem;
        for(SubsystemIDType i = 1; i < UnsupportedSubsystem; i<<=1)
            m_subsystemsRefCount[_subsystem_detail::flag_to_index(i)] += static_cast<bool>(subsystem & i);
    }
    return wasInit;
}

void 
SubsystemState::quit(SubsystemIDType subsystem)
{
    if(!m_impl) return;
    SubsystemIDType cleanFlag = 0;
    for(SubsystemIDType i = 1; i < UnsupportedSubsystem; i<<=1)
    {
        if((subsystem & i) && wasInit(i))
        {
                cleanFlag |= i;
        }
    }
    std::lock_guard<std::mutex> lock{m_mutex};
    for(SubsystemIDType i = 1; i < UnsupportedSubsystem; i<<=1)
        if(cleanFlag & i && --m_subsystemsRefCount[_subsystem_detail::flag_to_index(i)])
            cleanFlag ^= i;
    m_state ^= cleanFlag;
    if(cleanFlag) m_impl->quit(cleanFlag);
}

const aex::string& 
SubsystemState::getError() const
{
    static const aex::string NoImplError = "No implementation set.";
    if(!m_impl) return NoImplError;
    return m_impl->getError();
}

void 
SubsystemState::clearError()
{
    if(m_impl)
        m_impl->clearError();
}

bool
SubsystemState::noError() const
{
    return m_impl && m_impl->noError();
}

SubsystemState::operator bool()
{
    return m_impl != nullptr;
}

SubsystemState::~SubsystemState()
{
    reset();
}

void
SubsystemState::reset()
{
    if(m_impl && m_state)
        m_impl->quit(m_state);
}

class SDLSubsystemInitManager : public ISubsystemInitManager
{
    public:
        bool init(SubsystemIDType subsystem)
        {
            bool wasInit = WasInitSubsystem(subsystem);
            if(!wasInit)
            {
                if(!(wasInit = InitSubsystem(subsystem)))
                {
                    std::stringstream sstr("Failed to initialize SDL library with error: ");
                    sstr << SDL_GetError();
                    m_lastError = sstr.str();
                }
            }
            return wasInit;
        }

        void quit(SubsystemIDType subsystem)
        {
            for(SubsystemIDType i = 1; i < UnsupportedSubsystem; i<<=1)
            {
                if(subsystem & i)
                {
                    assert(WasInitSubsystem(i)); // This shouldn't happen.
                    QuitSubsystem(i);
                }
            }
        }

        const aex::string& getError() const
        {
            return m_lastError;
        }

        void clearError()
        {
            m_lastError = "";
        }

        bool noError() const
        {
            return m_lastError.empty();
        }

        ~SDLSubsystemInitManager()
        {
        }
    private:
        aex::string m_lastError;
};



FailedToInitSubsystem::FailedToInitSubsystem(const aex::string& error) : m_error(error) { }

const char* FailedToInitSubsystem::what() const NOEXCEPT
{
    return m_error.c_str();
}
SubsystemGuard::SubsystemGuard(const SubsystemIDType subsystems) : base_type(subsystems){}

SubsystemGuard::~SubsystemGuard(){}

bool
SubsystemGuard::init()
{
    return setSubsystemInitManager([](){return std::unique_ptr<ISubsystemInitManager>(new SDLSubsystemInitManager{});});
}
}
