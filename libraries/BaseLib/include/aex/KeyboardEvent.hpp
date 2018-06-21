#ifndef __TestProject__KeyboardEvent__
#define __TestProject__KeyboardEvent__

#include <aex/Common.hpp>
#include <aex/Event.hpp>
#include <aex/Logger.hpp>
#include <unordered_map>
#include <functional>
#include <map>

namespace aex
{
        enum AKEYSTATE{
            AKEYSTATE_KEYDOWN        = 0x300, /**< Key pressed */
            AKEYSTATE_KEYUP,                  /**< Key released */
            AKEYSTATE_BADSTATE
        };

        enum AEX_SCANCODE
        {
            AKEY_UNHANDLED,
            AKEY_0,
            AKEY_1,
            AKEY_2,
            AKEY_3,
            AKEY_4,
            AKEY_5,
            AKEY_6,
            AKEY_7,
            AKEY_8,
            AKEY_9,
            AKEY_A,
            AKEY_AC_BACK,
            AKEY_AC_BOOKMARKS,
            AKEY_AC_FORWARD,
            AKEY_AC_HOME,
            AKEY_AC_REFRESH,
            AKEY_AC_SEARCH,
            AKEY_AC_STOP,
            AKEY_AGAIN,
            AKEY_ALTERASE,
            AKEY_APOSTROPHE,
            AKEY_APPLICATION,
            AKEY_AUDIOMUTE,
            AKEY_AUDIONEXT,
            AKEY_AUDIOPLAY,
            AKEY_AUDIOPREV,
            AKEY_AUDIOSTOP,
            AKEY_B,
            AKEY_BACKSLASH,
            AKEY_BACKSPACE,
            AKEY_BRIGHTNESSDOWN,
            AKEY_BRIGHTNESSUP,
            AKEY_C,
            AKEY_CALCULATOR,
            AKEY_CANCEL,
            AKEY_CAPSLOCK,
            AKEY_CLEAR,
            AKEY_CLEARAGAIN,
            AKEY_COMMA,
            AKEY_COMPUTER,
            AKEY_COPY,
            AKEY_CRSEL,
            AKEY_CURRENCYSUBUNIT,
            AKEY_CURRENCYUNIT,
            AKEY_CUT,
            AKEY_D,
            AKEY_DECIMALSEPARATOR,
            AKEY_DELETE,
            AKEY_DISPLAYSWITCH,
            AKEY_DOWN,
            AKEY_E,
            AKEY_EJECT,
            AKEY_END,
            AKEY_EQUALS,
            AKEY_ESCAPE,
            AKEY_EXECUTE,
            AKEY_EXSEL,
            AKEY_F,
            AKEY_F1,
            AKEY_F10,
            AKEY_F11,
            AKEY_F12,
            AKEY_F13,
            AKEY_F14,
            AKEY_F15,
            AKEY_F16,
            AKEY_F17,
            AKEY_F18,
            AKEY_F19,
            AKEY_F2,
            AKEY_F20,
            AKEY_F21,
            AKEY_F22,
            AKEY_F23,
            AKEY_F24,
            AKEY_F3,
            AKEY_F4,
            AKEY_F5,
            AKEY_F6,
            AKEY_F7,
            AKEY_F8,
            AKEY_F9,
            AKEY_FIND,
            AKEY_G,
            AKEY_GRAVE,
            AKEY_H,
            AKEY_HELP,
            AKEY_HOME,
            AKEY_I,
            AKEY_INSERT,
            AKEY_J,
            AKEY_K,
            AKEY_KBDILLUMDOWN,
            AKEY_KBDILLUMTOGGLE,
            AKEY_KBDILLUMUP,
            AKEY_KP_0,
            AKEY_KP_00,
            AKEY_KP_000,
            AKEY_KP_1,
            AKEY_KP_2,
            AKEY_KP_3,
            AKEY_KP_4,
            AKEY_KP_5,
            AKEY_KP_6,
            AKEY_KP_7,
            AKEY_KP_8,
            AKEY_KP_9,
            AKEY_KP_A,
            AKEY_KP_AMPERSAND,
            AKEY_KP_AT,
            AKEY_KP_B,
            AKEY_KP_BACKSPACE,
            AKEY_KP_BINARY,
            AKEY_KP_C,
            AKEY_KP_CLEAR,
            AKEY_KP_CLEARENTRY,
            AKEY_KP_COLON,
            AKEY_KP_COMMA,
            AKEY_KP_D,
            AKEY_KP_DBLAMPERSAND,
            AKEY_KP_DBLVERTICALBAR,
            AKEY_KP_DECIMAL,
            AKEY_KP_DIVIDE,
            AKEY_KP_E,
            AKEY_KP_ENTER,
            AKEY_KP_EQUALS,
            AKEY_KP_EQUALSAS400,
            AKEY_KP_EXCLAM,
            AKEY_KP_F,
            AKEY_KP_GREATER,
            AKEY_KP_HASH,
            AKEY_KP_HEXADECIMAL,
            AKEY_KP_LEFTBRACE,
            AKEY_KP_LEFTPAREN,
            AKEY_KP_LESS,
            AKEY_KP_MEMADD,
            AKEY_KP_MEMCLEAR,
            AKEY_KP_MEMDIVIDE,
            AKEY_KP_MEMMULTIPLY,
            AKEY_KP_MEMRECALL,
            AKEY_KP_MEMSTORE,
            AKEY_KP_MEMSUBTRACT,
            AKEY_KP_MINUS,
            AKEY_KP_MULTIPLY,
            AKEY_KP_OCTAL,
            AKEY_KP_PERCENT,
            AKEY_KP_PERIOD,
            AKEY_KP_PLUS,
            AKEY_KP_PLUSMINUS,
            AKEY_KP_POWER,
            AKEY_KP_RIGHTBRACE,
            AKEY_KP_RIGHTPAREN,
            AKEY_KP_SPACE,
            AKEY_KP_TAB,
            AKEY_KP_VERTICALBAR,
            AKEY_KP_XOR,
            AKEY_L,
            AKEY_LALT,
            AKEY_LCTRL,
            AKEY_LEFT,
            AKEY_LEFTBRACKET,
            AKEY_LGUI,
            AKEY_LSHIFT,
            AKEY_M,
            AKEY_MAIL,
            AKEY_MEDIASELECT,
            AKEY_MENU,
            AKEY_MINUS,
            AKEY_MODE,
            AKEY_MUTE,
            AKEY_N,
            AKEY_NUMLOCKCLEAR,
            AKEY_O,
            AKEY_OPER,
            AKEY_OUT,
            AKEY_P,
            AKEY_PAGEDOWN,
            AKEY_PAGEUP,
            AKEY_PASTE,
            AKEY_PAUSE,
            AKEY_PERIOD,
            AKEY_POWER,
            AKEY_PRINTSCREEN,
            AKEY_PRIOR,
            AKEY_Q,
            AKEY_R,
            AKEY_RALT,
            AKEY_RCTRL,
            AKEY_RETURN,
            AKEY_RETURN2,
            AKEY_RGUI,
            AKEY_RIGHT,
            AKEY_RIGHTBRACKET,
            AKEY_RSHIFT,
            AKEY_S,
            AKEY_SCROLLLOCK,
            AKEY_SELECT,
            AKEY_SEMICOLON,
            AKEY_SEPARATOR,
            AKEY_SLASH,
            AKEY_SLEEP,
            AKEY_SPACE,
            AKEY_STOP,
            AKEY_SYSREQ,
            AKEY_T,
            AKEY_TAB,
            AKEY_THOUSANDSSEPARATOR,
            AKEY_U,
            AKEY_UNDO,
            AKEY_UNKNOWN,
            AKEY_UP,
            AKEY_V,
            AKEY_VOLUMEDOWN,
            AKEY_VOLUMEUP,
            AKEY_W,
            AKEY_WWW,
            AKEY_X,
            AKEY_Y,
            AKEY_Z,
            AKEY_INTERNATIONAL1,
            AKEY_INTERNATIONAL2,
            AKEY_INTERNATIONAL3,
            AKEY_INTERNATIONAL4,
            AKEY_INTERNATIONAL5,
            AKEY_INTERNATIONAL6,
            AKEY_INTERNATIONAL7,
            AKEY_INTERNATIONAL8,
            AKEY_INTERNATIONAL9,
            AKEY_LANG1,
            AKEY_LANG2,
            AKEY_LANG3,
            AKEY_LANG4,
            AKEY_LANG5,
            AKEY_LANG6,
            AKEY_LANG7,
            AKEY_LANG8,
            AKEY_LANG9,
        };


    typedef std::map<AEX_SCANCODE, std::function<void()>> KeyCallbackMap;

    struct AEX_PUBLIC_API TextInputEventData
    {
        uint32_t type;
        uint32_t timestamp;
        uint32_t windowID;
        char text[32];

        TextInputEventData( uint32_t stype,uint32_t stimestamp,  uint32_t swindowID, char (&stext)[32]);
    };

    struct AEX_PUBLIC_API TextEditEventData
    {
        uint32_t type;
        uint32_t timestamp;
        uint32_t windowID;
        char text[32];
        int32_t start;                               /**< The start cursor of selected editing text */
        int32_t length;                              /**< The length of selected editing text */

        TextEditEventData( uint32_t stype,uint32_t stimestamp,  uint32_t swindowID, char (&stext)[32], int32_t sstart, int32_t slength );
    };

    struct AEX_PUBLIC_API KeyboardEventData
    {
        uint32_t type;        /**< ::SDL_KEYDOWN or ::SDL_KEYUP */
        uint32_t timestamp;
        uint32_t windowID;    /**< The window with keyboard focus, if any */
        AKEYSTATE state;        /**< ::SDL_PRESSED or ::SDL_RELEASED */
        uint8_t repeat;       /**< Non-zero if this is a key repeat */
        AEX_SCANCODE scancode;
        uint16_t mod;


        KeyboardEventData( uint32_t stype,uint32_t stimestamp,  uint32_t swindowID, AKEYSTATE sstate, uint8_t srepeat,
        AEX_SCANCODE sscancode, uint16_t mod);
    };


    class AEX_PUBLIC_API KeyboardEvent : public Event
    {
        private:
            void handle(void* e);
        protected:
            virtual void key_down_event(KeyboardEventData keyEvent) {UNUSED(keyEvent);}
            virtual void key_up_event(KeyboardEventData keyEvent) {UNUSED(keyEvent);}
            virtual void key_textedit_event(TextEditEventData textEditEvent) {UNUSED(textEditEvent);}
            virtual void key_textinput_event(TextInputEventData textInputEvent) {UNUSED(textInputEvent);}

        public:
            virtual ~KeyboardEvent(){}
    };
    struct AEX_PUBLIC_API NKeyData
    {
        uint32_t timestamp;
        uint32_t windowId;
    };

    AEX_PUBLIC_API uint32_t nameToScancode(const aex::string& name);
    template<class DispatchType>
    class NamedKeyDispatcher : public KeyboardEvent
    {
        protected:
            DispatchType& getHandler(const aex::string& key);
            const DispatchType* getHandler(uint32_t key) const;
            DispatchType* getHandler(uint32_t key);
        private:
            std::unordered_map<uint32_t, DispatchType> m_dispatchLookup;
    };

    struct AEX_PUBLIC_API key_handler
    {
        typedef void(*key_down_dispatch_t)(const NKeyData& state);
        typedef void(*key_up_dispatch_t)(const NKeyData& state);
        key_down_dispatch_t key_pressed;
        key_up_dispatch_t key_released;
    };

    class AEX_PUBLIC_API SimpleKeyDispatch : public NamedKeyDispatcher<key_handler>
    {
        public:
            void addKeyPressListener(const aex::string& key, key_handler::key_down_dispatch_t dispatcher);
            void addKeyReleaseListener(const aex::string& key, key_handler::key_up_dispatch_t dispatcher);
        protected:
            void key_down_event(KeyboardEventData);
            void key_up_event(KeyboardEventData);
    };
}
#include "KeyboardEvent.ipp"
#endif // __TestProject__KeyboardEvent__
