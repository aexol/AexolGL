#ifndef _AEX_SUBSYSTEMGUARD_IPP
#define _AEX_SUBSYSTEMGUARD_IPP

namespace aex
{
    template<class ProxyBuilderType>
    SubsystemGuardBase<ProxyBuilderType>::SubsystemGuardBase() : SubsystemGuardBase(0)
    {
    }

    template<class ProxyBuilderType>
    SubsystemGuardBase<ProxyBuilderType>::SubsystemGuardBase(const SubsystemIDType subsystems) : m_state{}, m_used(subsystems)
    {
    }

    template<class ProxyBuilderType>
    SubsystemGuardBase<ProxyBuilderType>::SubsystemGuardBase(const SubsystemGuardBase& rhs) :
        m_state{rhs.m_state},
        m_used(0)
    {
        start_system(rhs.m_used);
    }

    template<class ProxyBuilderType>
    SubsystemGuardBase<ProxyBuilderType>&
    SubsystemGuardBase<ProxyBuilderType>::operator=(const SubsystemGuardBase& rhs)
    {
        if(this != &rhs)
        {
            stop_system(m_used);
            m_state = rhs.m_state;
            start_system(rhs.m_used);
        }
        return *this;
    }

    template<class ProxyBuilderType>
    SubsystemGuardBase<ProxyBuilderType>::~SubsystemGuardBase()
    {
        stop_system(m_used);
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::start_video()
    {
        if(!start_system(VideoSubsystem))
            throw FailedToInitSubsystem("Failed to init video subsystem.");
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::stop_video()
    {
        stop_system(VideoSubsystem);
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::start_audio()
    {
        if(!start_system(AudioSubsystem))
            throw FailedToInitSubsystem("Failed to init audio subsystem.");
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::stop_audio()
    {
        stop_system(AudioSubsystem);
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::start_events()
    {
        if(!start_system(EventsSubsystem))
            throw FailedToInitSubsystem("Failed to init events subsystem.");
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::stop_events()
    {
        stop_system(EventsSubsystem);
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::start_timer()
    {
        if(!start_system(TimerSubsystem))
            throw FailedToInitSubsystem("Failed to init timer subsystem.");
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::stop_timer()
    {
        stop_system(TimerSubsystem);
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::start_joystick()
    {
        if(!start_system(JoystickSubsystem))
            throw FailedToInitSubsystem("Failed to init joystick subsystem.");
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::stop_joystick()
    {
        stop_system(JoystickSubsystem);
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::start_haptic()
    {
        if(!start_system(HapticSubsystem))
            throw FailedToInitSubsystem("Failed to init haptic subsystem.");
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::stop_haptic()
    {
        stop_system(HapticSubsystem);
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::start_controller()
    {
        if(!start_system(ControllerSubsystem))
            throw FailedToInitSubsystem("Failed to init controller subsystem.");
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::stop_controller()
    {
        stop_system(ControllerSubsystem);
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::start_fonts()
    {
        if(!start_system(FontSubsystem))
            throw FailedToInitSubsystem("Failed to init fonts subsystem.");
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::stop_fonts()
    {
        stop_system(FontSubsystem);
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::start_all()
    {
        if(!start_system(~UnsupportedSubsystem & (UnsupportedSubsystem - 1)))
            throw FailedToInitSubsystem("Failed to init subsystems.");
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::stop_all()
    {
        stop_system(m_used);
    }

    template<class ProxyBuilderType>
    bool
    SubsystemGuardBase<ProxyBuilderType>::isEnabled(SubsystemIDType subsystem)
    {
        return (m_used & subsystem) == subsystem;
    }

    template<class ProxyBuilderType>
    bool
    SubsystemGuardBase<ProxyBuilderType>::start_system(SubsystemIDType subsystem)
    {
        subsystem &= ~m_used;
        m_used |= subsystem;
        bool status;
        if(!(status = m_state && m_state->init(subsystem)))
            m_used ^= subsystem;
        return status;
    }

    template<class ProxyBuilderType>
    void
    SubsystemGuardBase<ProxyBuilderType>::stop_system(SubsystemIDType subsystem)
    {
        m_used &= ~subsystem;
        if(m_state)
            m_state->quit(subsystem);
    }

    template<class ProxyBuilderType>
    bool
    SubsystemGuardBase<ProxyBuilderType>::internalInit()
    {
        if(!m_state)
            m_state.reset(&getProxyBuilder());
        return bool(m_state);
    }

    template<class ProxyBuilderType>
    typename SubsystemGuardBase<ProxyBuilderType>::proxy_builder_type& SubsystemGuardBase<ProxyBuilderType>::getProxyBuilder()
    {
        static proxy_builder_type StateProxyBuilder;
        return StateProxyBuilder;
    }
} // namespace aex
#endif // _AEX_SUBSYSTEMGUARD_IPP
