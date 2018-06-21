#ifndef _AEX_SUBSYSTEMGUARD_HPP
#define _AEX_SUBSYSTEMGUARD_HPP
#include <aex/Proxy.hpp>
#include <exception>
#include <mutex>
#include <string>
#include <array>
#include <type_traits>
namespace aex
{
    typedef uint16_t SubsystemIDType;
    static CONSTEXPR SubsystemIDType VideoSubsystem       = 0x0001;
    static CONSTEXPR SubsystemIDType AudioSubsystem       = VideoSubsystem << 1;
    static CONSTEXPR SubsystemIDType EventsSubsystem      = VideoSubsystem << 2;
    static CONSTEXPR SubsystemIDType TimerSubsystem       = VideoSubsystem << 3;
    static CONSTEXPR SubsystemIDType JoystickSubsystem    = VideoSubsystem << 4;
    static CONSTEXPR SubsystemIDType HapticSubsystem      = VideoSubsystem << 5;
    static CONSTEXPR SubsystemIDType ControllerSubsystem  = VideoSubsystem << 6;
    static CONSTEXPR SubsystemIDType FontSubsystem        = VideoSubsystem << 7;
    static CONSTEXPR SubsystemIDType UnsupportedSubsystem = VideoSubsystem << 8;

    namespace _subsystem_detail
    {
        CONSTEXPR SubsystemIDType unsupported_index(SubsystemIDType index=0, SubsystemIDType flag = UnsupportedSubsystem)
        {
            return flag == 0x0 ? index : unsupported_index(index+1, flag >> 1);
        }
        CONSTEXPR SubsystemIDType flag_to_index(SubsystemIDType flag, SubsystemIDType index = 0)
        {
            return index >= unsupported_index() ? unsupported_index() : (flag & 0x01) ?
                index : flag_to_index(flag>>1, index+1);
        }
    }

    class AEX_PUBLIC_API FailedToInitSubsystem : public std::exception
    {
        const aex::string m_error;
    public:
        explicit FailedToInitSubsystem(const aex::string& error);
        virtual const char* what() const NOEXCEPT;
    };

    class AEX_PUBLIC_API ISubsystemInitManager
    {
        public:
            virtual bool init(SubsystemIDType subsystems) = 0;
            virtual void quit(SubsystemIDType subsystems) = 0;
            virtual const aex::string& getError() const = 0;
            virtual void clearError() = 0;
            virtual bool noError() const = 0;
            virtual ~ISubsystemInitManager() = 0;
    };
    inline ISubsystemInitManager::~ISubsystemInitManager(){}

    namespace _subsystem_detail
    {
        using implementation_interface = ISubsystemInitManager;
        using implementation_type = std::unique_ptr<implementation_interface >;

        template<class T, bool = std::is_convertible<decltype(std::declval<T>()()),implementation_type>::value>
        struct _call_operator_returns_implementation_type_convertible
        {
            typedef T type;
        };

        template<class T>
        struct _call_operator_returns_implementation_type_convertible<T, false>
        {
            typedef int type;
        };
        template<class T>
        struct is_isubsysteminitmanager_factory
        {
            template<class C> static auto Test(void*) -> decltype(
                    std::declval<typename _call_operator_returns_implementation_type_convertible<C>::type>(),
                    std::true_type{}
            );
            template<class C> static std::false_type Test(...);
            static CONSTEXPR bool value = decltype(Test<T>(0))::value;
        };

        template<class T>
        struct is_isubsysteminitmanager_impl : public std::is_base_of<implementation_interface, typename std::decay<T>::type>
        {
        };

        template<class T>
        struct is_isubsysteminitmanager_uptr_convertible : public std::is_convertible<T, implementation_type>
        {};
    }

    class AEX_PUBLIC_API SubsystemState
    {
        using implementation_type =  _subsystem_detail::implementation_type;
        public:
            static CONSTEXPR std::size_t SUBSYSTEMS_COUNT = _subsystem_detail::flag_to_index(UnsupportedSubsystem);
            SubsystemState();

            /*
             * @brief Was subsytem initialized by this state.
             */
            bool wasInit(SubsystemIDType subsystem) const;

            /*
             * @brief init subsystem and increase ref count.
             */
            bool init(SubsystemIDType subsystem);

            /*
             * @brief Decrese ref count on subsytem and release if orphaned.
             */
            void quit(SubsystemIDType subsystem);

            /*
             * @brief Return last error message.
             */
            const aex::string& getError() const;

            /*
             * @brief Clear last error.
             */
            void clearError();

            /*
             * @brief Return true if error is set.
             */
            bool noError() const;

            /*
             * @brief If SubsystemState implementation is not set, set it using f functor.
             */
            template<class F>
            bool trySet(F&& f)
            {
                if(!bool(*this)) {
                    reset();
                    internalTrySet(aex::forward<F>(f));
                    m_state = 0;
                    m_subsystemsRefCount = std::array<unsigned char, SUBSYSTEMS_COUNT>{};
                }
                return bool(*this);
            }

            /*
             * @brief Is SubsystemState implementation set.
             */
            operator bool();

            /*
             * @brief Destroy state and if implementation exists, release subsystems.
             */
            virtual ~SubsystemState();
        private:
            template<class Factory, typename std::enable_if<
                _subsystem_detail::is_isubsysteminitmanager_factory<Factory>::value>::type* = nullptr
            >
            void internalTrySet(Factory&& f)
            {
                m_impl = f();
            }

            template<class Factory, typename std::enable_if<
                !_subsystem_detail::is_isubsysteminitmanager_factory<Factory>::value &&
                _subsystem_detail::is_isubsysteminitmanager_impl<Factory>::value>::type* = nullptr
            >
            void internalTrySet(Factory&& f)
            {
                m_impl = implementation_type(new typename std::decay<Factory>::type(aex::forward<Factory>(f)));
            }

            template<class Factory, typename std::enable_if<
                !_subsystem_detail::is_isubsysteminitmanager_factory<Factory>::value &&
                !_subsystem_detail::is_isubsysteminitmanager_impl<Factory>::value &&
                _subsystem_detail::is_isubsysteminitmanager_uptr_convertible<Factory>::value>::type* = nullptr
            >
            void internalTrySet(Factory&& f)
            {
                m_impl = aex::forward<Factory>(f);
            }

            void reset();

            mutable std::mutex m_mutex;
            SubsystemIDType m_state;
            std::array<unsigned char, SUBSYSTEMS_COUNT> m_subsystemsRefCount;
            implementation_type m_impl;
    };

    template<class ProxyBuilderType>
    class SubsystemGuardBase
    {
    public:
        typedef ProxyBuilderType proxy_builder_type;
        typedef typename proxy_builder_type::proxy_type proxy_type;
        SubsystemGuardBase();
        explicit SubsystemGuardBase(const SubsystemIDType subsystems);
        explicit SubsystemGuardBase(const SubsystemGuardBase&);
        SubsystemGuardBase& operator=(const SubsystemGuardBase&);
        explicit SubsystemGuardBase(SubsystemGuardBase&& sg) = default;
        SubsystemGuardBase& operator=(SubsystemGuardBase&& sg) = default;
        virtual ~SubsystemGuardBase();
        void start_video();
        void stop_video();
        void start_audio();
        void stop_audio();
        void start_events();
        void stop_events();
        void start_timer();
        void stop_timer();
        void start_joystick();
        void stop_joystick();
        void start_haptic();
        void stop_haptic();
        void start_controller();
        void stop_controller();
        void start_fonts();
        void stop_fonts();
        void start_all();
        void stop_all();
        bool isEnabled(SubsystemIDType subsystems);
        bool start_system(SubsystemIDType system);
        void stop_system(SubsystemIDType system);
        virtual bool init() = 0;
    protected:
        template<class Factory>
        bool setSubsystemInitManager(Factory&& f)
        {
            return internalInit()  &&  m_state->trySet(aex::forward<Factory>(f)) && m_state->init(m_used);
        }
    private:
        static proxy_builder_type& getProxyBuilder();
        bool internalInit();
        proxy_type m_state;
        SubsystemIDType m_used;
    };

    class AEX_PUBLIC_API SubsystemGuard : public SubsystemGuardBase<ProxyBuilder<SubsystemState>>
    {
        public:
            typedef SubsystemGuardBase<ProxyBuilder<SubsystemState>> base_type;
            SubsystemGuard() = default;
            explicit SubsystemGuard(const SubsystemIDType subsystems);
            ~SubsystemGuard();
            bool init();
    };
}
#include "details/SubsystemGuard.ipp"
#endif
