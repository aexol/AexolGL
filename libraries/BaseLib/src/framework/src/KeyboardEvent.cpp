#include "../include/KeyboardEvent.hpp"
#include <stdexcept>
#include <SDL.h>
namespace aex {
    // Name-Scancode pairs.

    static const std::unordered_map<aex::string, AEX_SCANCODE> KeyNameMap{
        {"0", AKEY_0},
        {"1", AKEY_1},
        {"2", AKEY_2},
        {"3", AKEY_3},
        {"4", AKEY_4},
        {"5", AKEY_5},
        {"6", AKEY_6},
        {"7", AKEY_7},
        {"8", AKEY_8},
        {"9", AKEY_9},
        {"A", AKEY_A},
        {"AC_BACK", AKEY_AC_BACK},
        {"AC_BOOKMARKS", AKEY_AC_BOOKMARKS},
        {"AC_FORWARD", AKEY_AC_FORWARD},
        {"AC_HOME", AKEY_AC_HOME},
        {"AC_REFRESH", AKEY_AC_REFRESH},
        {"AC_SEARCH", AKEY_AC_SEARCH},
        {"AC_STOP", AKEY_AC_STOP},
        {"AGAIN", AKEY_AGAIN},
        {"ALTERASE", AKEY_ALTERASE},
        {"APOSTROPHE", AKEY_APOSTROPHE},
        {"APPLICATION", AKEY_APPLICATION},
        {"AUDIOMUTE", AKEY_AUDIOMUTE},
        {"AUDIONEXT", AKEY_AUDIONEXT},
        {"AUDIOPLAY", AKEY_AUDIOPLAY},
        {"AUDIOPREV", AKEY_AUDIOPREV},
        {"AUDIOSTOP", AKEY_AUDIOSTOP},
        {"B", AKEY_B},
        {"BACKSLASH", AKEY_BACKSLASH},
        {"BACKSPACE", AKEY_BACKSPACE},
        {"BRIGHTNESSDOWN", AKEY_BRIGHTNESSDOWN},
        {"BRIGHTNESSUP", AKEY_BRIGHTNESSUP},
        {"C", AKEY_C},
        {"CALCULATOR", AKEY_CALCULATOR},
        {"CANCEL", AKEY_CANCEL},
        {"CAPSLOCK", AKEY_CAPSLOCK},
        {"CLEAR", AKEY_CLEAR},
        {"CLEARAGAIN", AKEY_CLEARAGAIN},
        {"COMMA", AKEY_COMMA},
        {"COMPUTER", AKEY_COMPUTER},
        {"COPY", AKEY_COPY},
        {"CRSEL", AKEY_CRSEL},
        {"CURRENCYSUBUNIT", AKEY_CURRENCYSUBUNIT},
        {"CURRENCYUNIT", AKEY_CURRENCYUNIT},
        {"CUT", AKEY_CUT},
        {"D", AKEY_D},
        {"DECIMALSEPARATOR", AKEY_DECIMALSEPARATOR},
        {"DELETE", AKEY_DELETE},
        {"DISPLAYSWITCH", AKEY_DISPLAYSWITCH},
        {"DOWN", AKEY_DOWN},
        {"E", AKEY_E},
        {"EJECT", AKEY_EJECT},
        {"END", AKEY_END},
        {"EQUALS", AKEY_EQUALS},
        {"ESCAPE", AKEY_ESCAPE},
        {"EXECUTE", AKEY_EXECUTE},
        {"EXSEL", AKEY_EXSEL},
        {"F", AKEY_F},
        {"F1", AKEY_F1},
        {"F10", AKEY_F10},
        {"F11", AKEY_F11},
        {"F12", AKEY_F12},
        {"F13", AKEY_F13},
        {"F14", AKEY_F14},
        {"F15", AKEY_F15},
        {"F16", AKEY_F16},
        {"F17", AKEY_F17},
        {"F18", AKEY_F18},
        {"F19", AKEY_F19},
        {"F2", AKEY_F2},
        {"F20", AKEY_F20},
        {"F21", AKEY_F21},
        {"F22", AKEY_F22},
        {"F23", AKEY_F23},
        {"F24", AKEY_F24},
        {"F3", AKEY_F3},
        {"F4", AKEY_F4},
        {"F5", AKEY_F5},
        {"F6", AKEY_F6},
        {"F7", AKEY_F7},
        {"F8", AKEY_F8},
        {"F9", AKEY_F9},
        {"FIND", AKEY_FIND},
        {"G", AKEY_G},
        {"GRAVE", AKEY_GRAVE},
        {"H", AKEY_H},
        {"HELP", AKEY_HELP},
        {"HOME", AKEY_HOME},
        {"I", AKEY_I},
        {"INSERT", AKEY_INSERT},
        {"J", AKEY_J},
        {"K", AKEY_K},
        {"KBDILLUMDOWN", AKEY_KBDILLUMDOWN},
        {"KBDILLUMTOGGLE", AKEY_KBDILLUMTOGGLE},
        {"KBDILLUMUP", AKEY_KBDILLUMUP},
        {"KP_0", AKEY_KP_0},
        {"KP_00", AKEY_KP_00},
        {"KP_000", AKEY_KP_000},
        {"KP_1", AKEY_KP_1},
        {"KP_2", AKEY_KP_2},
        {"KP_3", AKEY_KP_3},
        {"KP_4", AKEY_KP_4},
        {"KP_5", AKEY_KP_5},
        {"KP_6", AKEY_KP_6},
        {"KP_7", AKEY_KP_7},
        {"KP_8", AKEY_KP_8},
        {"KP_9", AKEY_KP_9},
        {"KP_A", AKEY_KP_A},
        {"KP_AMPERSAND", AKEY_KP_AMPERSAND},
        {"KP_AT", AKEY_KP_AT},
        {"KP_B", AKEY_KP_B},
        {"KP_BACKSPACE", AKEY_KP_BACKSPACE},
        {"KP_BINARY", AKEY_KP_BINARY},
        {"KP_C", AKEY_KP_C},
        {"KP_CLEAR", AKEY_KP_CLEAR},
        {"KP_CLEARENTRY", AKEY_KP_CLEARENTRY},
        {"KP_COLON", AKEY_KP_COLON},
        {"KP_COMMA", AKEY_KP_COMMA},
        {"KP_D", AKEY_KP_D},
        {"KP_DBLAMPERSAND", AKEY_KP_DBLAMPERSAND},
        {"KP_DBLVERTICALBAR", AKEY_KP_DBLVERTICALBAR},
        {"KP_DECIMAL", AKEY_KP_DECIMAL},
        {"KP_DIVIDE", AKEY_KP_DIVIDE},
        {"KP_E", AKEY_KP_E},
        {"KP_ENTER", AKEY_KP_ENTER},
        {"KP_EQUALS", AKEY_KP_EQUALS},
        {"KP_EQUALSAS400", AKEY_KP_EQUALSAS400},
        {"KP_EXCLAM", AKEY_KP_EXCLAM},
        {"KP_F", AKEY_KP_F},
        {"KP_GREATER", AKEY_KP_GREATER},
        {"KP_HASH", AKEY_KP_HASH},
        {"KP_HEXADECIMAL", AKEY_KP_HEXADECIMAL},
        {"KP_LEFTBRACE", AKEY_KP_LEFTBRACE},
        {"KP_LEFTPAREN", AKEY_KP_LEFTPAREN},
        {"KP_LESS", AKEY_KP_LESS},
        {"KP_MEMADD", AKEY_KP_MEMADD},
        {"KP_MEMCLEAR", AKEY_KP_MEMCLEAR},
        {"KP_MEMDIVIDE", AKEY_KP_MEMDIVIDE},
        {"KP_MEMMULTIPLY", AKEY_KP_MEMMULTIPLY},
        {"KP_MEMRECALL", AKEY_KP_MEMRECALL},
        {"KP_MEMSTORE", AKEY_KP_MEMSTORE},
        {"KP_MEMSUBTRACT", AKEY_KP_MEMSUBTRACT},
        {"KP_MINUS", AKEY_KP_MINUS},
        {"KP_MULTIPLY", AKEY_KP_MULTIPLY},
        {"KP_OCTAL", AKEY_KP_OCTAL},
        {"KP_PERCENT", AKEY_KP_PERCENT},
        {"KP_PERIOD", AKEY_KP_PERIOD},
        {"KP_PLUS", AKEY_KP_PLUS},
        {"KP_PLUSMINUS", AKEY_KP_PLUSMINUS},
        {"KP_POWER", AKEY_KP_POWER},
        {"KP_RIGHTBRACE", AKEY_KP_RIGHTBRACE},
        {"KP_RIGHTPAREN", AKEY_KP_RIGHTPAREN},
        {"KP_SPACE", AKEY_KP_SPACE},
        {"KP_TAB", AKEY_KP_TAB},
        {"KP_VERTICALBAR", AKEY_KP_VERTICALBAR},
        {"KP_XOR", AKEY_KP_XOR},
        {"L", AKEY_L},
        {"LALT", AKEY_LALT},
        {"LCTRL", AKEY_LCTRL},
        {"LEFT", AKEY_LEFT},
        {"LEFTBRACKET", AKEY_LEFTBRACKET},
        {"LGUI", AKEY_LGUI},
        {"LSHIFT", AKEY_LSHIFT},
        {"M", AKEY_M},
        {"MAIL", AKEY_MAIL},
        {"MEDIASELECT", AKEY_MEDIASELECT},
        {"MENU", AKEY_MENU},
        {"MINUS", AKEY_MINUS},
        {"MODE", AKEY_MODE},
        {"MUTE", AKEY_MUTE},
        {"N", AKEY_N},
        {"NUMLOCKCLEAR", AKEY_NUMLOCKCLEAR},
        {"O", AKEY_O},
        {"OPER", AKEY_OPER},
        {"OUT", AKEY_OUT},
        {"P", AKEY_P},
        {"PAGEDOWN", AKEY_PAGEDOWN},
        {"PAGEUP", AKEY_PAGEUP},
        {"PASTE", AKEY_PASTE},
        {"PAUSE", AKEY_PAUSE},
        {"PERIOD", AKEY_PERIOD},
        {"POWER", AKEY_POWER},
        {"PRINTSCREEN", AKEY_PRINTSCREEN},
        {"PRIOR", AKEY_PRIOR},
        {"Q", AKEY_Q},
        {"R", AKEY_R},
        {"RALT", AKEY_RALT},
        {"RCTRL", AKEY_RCTRL},
        {"RETURN", AKEY_RETURN},
        {"RETURN2", AKEY_RETURN2},
        {"RGUI", AKEY_RGUI},
        {"RIGHT", AKEY_RIGHT},
        {"RIGHTBRACKET", AKEY_RIGHTBRACKET},
        {"RSHIFT", AKEY_RSHIFT},
        {"S", AKEY_S},
        {"SCROLLLOCK", AKEY_SCROLLLOCK},
        {"SELECT", AKEY_SELECT},
        {"SEMICOLON", AKEY_SEMICOLON},
        {"SEPARATOR", AKEY_SEPARATOR},
        {"SLASH", AKEY_SLASH},
        {"SLEEP", AKEY_SLEEP},
        {"SPACE", AKEY_SPACE},
        {"STOP", AKEY_STOP},
        {"SYSREQ", AKEY_SYSREQ},
        {"T", AKEY_T},
        {"TAB", AKEY_TAB},
        {"THOUSANDSSEPARATOR", AKEY_THOUSANDSSEPARATOR},
        {"U", AKEY_U},
        {"UNDO", AKEY_UNDO},
        {"UNKNOWN", AKEY_UNKNOWN},
        {"UP", AKEY_UP},
        {"V", AKEY_V},
        {"VOLUMEDOWN", AKEY_VOLUMEDOWN},
        {"VOLUMEUP", AKEY_VOLUMEUP},
        {"W", AKEY_W},
        {"WWW", AKEY_WWW},
        {"X", AKEY_X},
        {"Y", AKEY_Y},
        {"Z", AKEY_Z},
        {"INTERNATIONAL1", AKEY_INTERNATIONAL1},
        {"INTERNATIONAL2", AKEY_INTERNATIONAL2},
        {"INTERNATIONAL3", AKEY_INTERNATIONAL3},
        {"INTERNATIONAL4", AKEY_INTERNATIONAL4},
        {"INTERNATIONAL5", AKEY_INTERNATIONAL5},
        {"INTERNATIONAL6", AKEY_INTERNATIONAL6},
        {"INTERNATIONAL7", AKEY_INTERNATIONAL7},
        {"INTERNATIONAL8", AKEY_INTERNATIONAL8},
        {"INTERNATIONAL9", AKEY_INTERNATIONAL9},
        {"LANG1", AKEY_LANG1},
        {"LANG2", AKEY_LANG2},
        {"LANG3", AKEY_LANG3},
        {"LANG4", AKEY_LANG4},
        {"LANG5", AKEY_LANG5},
        {"LANG6", AKEY_LANG6},
        {"LANG7", AKEY_LANG7},
        {"LANG8", AKEY_LANG8},
        {"LANG9", AKEY_LANG9}
        //{"LOCKINGCAPSLOCK", SDL_SCANCODE_LOCKINGCAPSLOCK},
        //{"LOCKINGNUMLOCK", SDL_SCANCODE_LOCKINGNUMLOCK},
        //{"LOCKINGSCROLLLOCK", SDL_SCANCODE_LOCKINGSCROLLLOCK},
        //{"NONUSBACKSLASH", SDL_SCANCODE_NONUSBACKSLASH},
        //{"BACKSLASH", SDL_SCANCODE_BACKSLASH},
        //{"NONUSHASH", SDL_SCANCODE_NONUSHASH}
    };

    AEX_SCANCODE sdlKeycodeToAexKeycode(SDL_Scancode key) {
        switch (key) {
            case SDL_SCANCODE_0: return AKEY_0;
            case SDL_SCANCODE_1: return AKEY_1;
            case SDL_SCANCODE_2: return AKEY_2;
            case SDL_SCANCODE_3: return AKEY_3;
            case SDL_SCANCODE_4: return AKEY_4;
            case SDL_SCANCODE_5: return AKEY_5;
            case SDL_SCANCODE_6: return AKEY_6;
            case SDL_SCANCODE_7: return AKEY_7;
            case SDL_SCANCODE_8: return AKEY_8;
            case SDL_SCANCODE_9: return AKEY_9;
            case SDL_SCANCODE_A: return AKEY_A;
            case SDL_SCANCODE_AC_BACK: return AKEY_AC_BACK;
            case SDL_SCANCODE_AC_BOOKMARKS: return AKEY_AC_BOOKMARKS;
            case SDL_SCANCODE_AC_FORWARD: return AKEY_AC_FORWARD;
            case SDL_SCANCODE_AC_HOME: return AKEY_AC_HOME;
            case SDL_SCANCODE_AC_REFRESH: return AKEY_AC_REFRESH;
            case SDL_SCANCODE_AC_SEARCH: return AKEY_AC_SEARCH;
            case SDL_SCANCODE_AC_STOP: return AKEY_AC_STOP;
            case SDL_SCANCODE_AGAIN: return AKEY_AGAIN;
            case SDL_SCANCODE_ALTERASE: return AKEY_ALTERASE;
            case SDL_SCANCODE_APOSTROPHE: return AKEY_APOSTROPHE;
            case SDL_SCANCODE_APPLICATION: return AKEY_APPLICATION;
            case SDL_SCANCODE_AUDIOMUTE: return AKEY_AUDIOMUTE;
            case SDL_SCANCODE_AUDIONEXT: return AKEY_AUDIONEXT;
            case SDL_SCANCODE_AUDIOPLAY: return AKEY_AUDIOPLAY;
            case SDL_SCANCODE_AUDIOPREV: return AKEY_AUDIOPREV;
            case SDL_SCANCODE_AUDIOSTOP: return AKEY_AUDIOSTOP;
            case SDL_SCANCODE_B: return AKEY_B;
            case SDL_SCANCODE_BACKSLASH: return AKEY_BACKSLASH;
            case SDL_SCANCODE_BACKSPACE: return AKEY_BACKSPACE;
            case SDL_SCANCODE_BRIGHTNESSDOWN: return AKEY_BRIGHTNESSDOWN;
            case SDL_SCANCODE_BRIGHTNESSUP: return AKEY_BRIGHTNESSUP;
            case SDL_SCANCODE_C: return AKEY_C;
            case SDL_SCANCODE_CALCULATOR: return AKEY_CALCULATOR;
            case SDL_SCANCODE_CANCEL: return AKEY_CANCEL;
            case SDL_SCANCODE_CAPSLOCK: return AKEY_CAPSLOCK;
            case SDL_SCANCODE_CLEAR: return AKEY_CLEAR;
            case SDL_SCANCODE_CLEARAGAIN: return AKEY_CLEARAGAIN;
            case SDL_SCANCODE_COMMA: return AKEY_COMMA;
            case SDL_SCANCODE_COMPUTER: return AKEY_COMPUTER;
            case SDL_SCANCODE_COPY: return AKEY_COPY;
            case SDL_SCANCODE_CRSEL: return AKEY_CRSEL;
            case SDL_SCANCODE_CURRENCYSUBUNIT: return AKEY_CURRENCYSUBUNIT;
            case SDL_SCANCODE_CURRENCYUNIT: return AKEY_CURRENCYUNIT;
            case SDL_SCANCODE_CUT: return AKEY_CUT;
            case SDL_SCANCODE_D: return AKEY_D;
            case SDL_SCANCODE_DECIMALSEPARATOR: return AKEY_DECIMALSEPARATOR;
            case SDL_SCANCODE_DELETE: return AKEY_DELETE;
            case SDL_SCANCODE_DISPLAYSWITCH: return AKEY_DISPLAYSWITCH;
            case SDL_SCANCODE_DOWN: return AKEY_DOWN;
            case SDL_SCANCODE_E: return AKEY_E;
            case SDL_SCANCODE_EJECT: return AKEY_EJECT;
            case SDL_SCANCODE_END: return AKEY_END;
            case SDL_SCANCODE_EQUALS: return AKEY_EQUALS;
            case SDL_SCANCODE_ESCAPE: return AKEY_ESCAPE;
            case SDL_SCANCODE_EXECUTE: return AKEY_EXECUTE;
            case SDL_SCANCODE_EXSEL: return AKEY_EXSEL;
            case SDL_SCANCODE_F: return AKEY_F;
            case SDL_SCANCODE_F1: return AKEY_F1;
            case SDL_SCANCODE_F10: return AKEY_F10;
            case SDL_SCANCODE_F11: return AKEY_F11;
            case SDL_SCANCODE_F12: return AKEY_F12;
            case SDL_SCANCODE_F13: return AKEY_F13;
            case SDL_SCANCODE_F14: return AKEY_F14;
            case SDL_SCANCODE_F15: return AKEY_F15;
            case SDL_SCANCODE_F16: return AKEY_F16;
            case SDL_SCANCODE_F17: return AKEY_F17;
            case SDL_SCANCODE_F18: return AKEY_F18;
            case SDL_SCANCODE_F19: return AKEY_F19;
            case SDL_SCANCODE_F2: return AKEY_F2;
            case SDL_SCANCODE_F20: return AKEY_F20;
            case SDL_SCANCODE_F21: return AKEY_F21;
            case SDL_SCANCODE_F22: return AKEY_F22;
            case SDL_SCANCODE_F23: return AKEY_F23;
            case SDL_SCANCODE_F24: return AKEY_F24;
            case SDL_SCANCODE_F3: return AKEY_F3;
            case SDL_SCANCODE_F4: return AKEY_F4;
            case SDL_SCANCODE_F5: return AKEY_F5;
            case SDL_SCANCODE_F6: return AKEY_F6;
            case SDL_SCANCODE_F7: return AKEY_F7;
            case SDL_SCANCODE_F8: return AKEY_F8;
            case SDL_SCANCODE_F9: return AKEY_F9;
            case SDL_SCANCODE_FIND: return AKEY_FIND;
            case SDL_SCANCODE_G: return AKEY_G;
            case SDL_SCANCODE_GRAVE: return AKEY_GRAVE;
            case SDL_SCANCODE_H: return AKEY_H;
            case SDL_SCANCODE_HELP: return AKEY_HELP;
            case SDL_SCANCODE_HOME: return AKEY_HOME;
            case SDL_SCANCODE_I: return AKEY_I;
            case SDL_SCANCODE_INSERT: return AKEY_INSERT;
            case SDL_SCANCODE_J: return AKEY_J;
            case SDL_SCANCODE_K: return AKEY_K;
            case SDL_SCANCODE_KBDILLUMDOWN: return AKEY_KBDILLUMDOWN;
            case SDL_SCANCODE_KBDILLUMTOGGLE: return AKEY_KBDILLUMTOGGLE;
            case SDL_SCANCODE_KBDILLUMUP: return AKEY_KBDILLUMUP;
            case SDL_SCANCODE_KP_0: return AKEY_KP_0;
            case SDL_SCANCODE_KP_00: return AKEY_KP_00;
            case SDL_SCANCODE_KP_000: return AKEY_KP_000;
            case SDL_SCANCODE_KP_1: return AKEY_KP_1;
            case SDL_SCANCODE_KP_2: return AKEY_KP_2;
            case SDL_SCANCODE_KP_3: return AKEY_KP_3;
            case SDL_SCANCODE_KP_4: return AKEY_KP_4;
            case SDL_SCANCODE_KP_5: return AKEY_KP_5;
            case SDL_SCANCODE_KP_6: return AKEY_KP_6;
            case SDL_SCANCODE_KP_7: return AKEY_KP_7;
            case SDL_SCANCODE_KP_8: return AKEY_KP_8;
            case SDL_SCANCODE_KP_9: return AKEY_KP_9;
            case SDL_SCANCODE_KP_A: return AKEY_KP_A;
            case SDL_SCANCODE_KP_AMPERSAND: return AKEY_KP_AMPERSAND;
            case SDL_SCANCODE_KP_AT: return AKEY_KP_AT;
            case SDL_SCANCODE_KP_B: return AKEY_KP_B;
            case SDL_SCANCODE_KP_BACKSPACE: return AKEY_KP_BACKSPACE;
            case SDL_SCANCODE_KP_BINARY: return AKEY_KP_BINARY;
            case SDL_SCANCODE_KP_C: return AKEY_KP_C;
            case SDL_SCANCODE_KP_CLEAR: return AKEY_KP_CLEAR;
            case SDL_SCANCODE_KP_CLEARENTRY: return AKEY_KP_CLEARENTRY;
            case SDL_SCANCODE_KP_COLON: return AKEY_KP_COLON;
            case SDL_SCANCODE_KP_COMMA: return AKEY_KP_COMMA;
            case SDL_SCANCODE_KP_D: return AKEY_KP_D;
            case SDL_SCANCODE_KP_DBLAMPERSAND: return AKEY_KP_DBLAMPERSAND;
            case SDL_SCANCODE_KP_DBLVERTICALBAR: return AKEY_KP_DBLVERTICALBAR;
            case SDL_SCANCODE_KP_DECIMAL: return AKEY_KP_DECIMAL;
            case SDL_SCANCODE_KP_DIVIDE: return AKEY_KP_DIVIDE;
            case SDL_SCANCODE_KP_E: return AKEY_KP_E;
            case SDL_SCANCODE_KP_ENTER: return AKEY_KP_ENTER;
            case SDL_SCANCODE_KP_EQUALS: return AKEY_KP_EQUALS;
            case SDL_SCANCODE_KP_EQUALSAS400: return AKEY_KP_EQUALSAS400;
            case SDL_SCANCODE_KP_EXCLAM: return AKEY_KP_EXCLAM;
            case SDL_SCANCODE_KP_F: return AKEY_KP_F;
            case SDL_SCANCODE_KP_GREATER: return AKEY_KP_GREATER;
            case SDL_SCANCODE_KP_HASH: return AKEY_KP_HASH;
            case SDL_SCANCODE_KP_HEXADECIMAL: return AKEY_KP_HEXADECIMAL;
            case SDL_SCANCODE_KP_LEFTBRACE: return AKEY_KP_LEFTBRACE;
            case SDL_SCANCODE_KP_LEFTPAREN: return AKEY_KP_LEFTPAREN;
            case SDL_SCANCODE_KP_LESS: return AKEY_KP_LESS;
            case SDL_SCANCODE_KP_MEMADD: return AKEY_KP_MEMADD;
            case SDL_SCANCODE_KP_MEMCLEAR: return AKEY_KP_MEMCLEAR;
            case SDL_SCANCODE_KP_MEMDIVIDE: return AKEY_KP_MEMDIVIDE;
            case SDL_SCANCODE_KP_MEMMULTIPLY: return AKEY_KP_MEMMULTIPLY;
            case SDL_SCANCODE_KP_MEMRECALL: return AKEY_KP_MEMRECALL;
            case SDL_SCANCODE_KP_MEMSTORE: return AKEY_KP_MEMSTORE;
            case SDL_SCANCODE_KP_MEMSUBTRACT: return AKEY_KP_MEMSUBTRACT;
            case SDL_SCANCODE_KP_MINUS: return AKEY_KP_MINUS;
            case SDL_SCANCODE_KP_MULTIPLY: return AKEY_KP_MULTIPLY;
            case SDL_SCANCODE_KP_OCTAL: return AKEY_KP_OCTAL;
            case SDL_SCANCODE_KP_PERCENT: return AKEY_KP_PERCENT;
            case SDL_SCANCODE_KP_PERIOD: return AKEY_KP_PERIOD;
            case SDL_SCANCODE_KP_PLUS: return AKEY_KP_PLUS;
            case SDL_SCANCODE_KP_PLUSMINUS: return AKEY_KP_PLUSMINUS;
            case SDL_SCANCODE_KP_POWER: return AKEY_KP_POWER;
            case SDL_SCANCODE_KP_RIGHTBRACE: return AKEY_KP_RIGHTBRACE;
            case SDL_SCANCODE_KP_RIGHTPAREN: return AKEY_KP_RIGHTPAREN;
            case SDL_SCANCODE_KP_SPACE: return AKEY_KP_SPACE;
            case SDL_SCANCODE_KP_TAB: return AKEY_KP_TAB;
            case SDL_SCANCODE_KP_VERTICALBAR: return AKEY_KP_VERTICALBAR;
            case SDL_SCANCODE_KP_XOR: return AKEY_KP_XOR;
            case SDL_SCANCODE_L: return AKEY_L;
            case SDL_SCANCODE_LALT: return AKEY_LALT;
            case SDL_SCANCODE_LCTRL: return AKEY_LCTRL;
            case SDL_SCANCODE_LEFT: return AKEY_LEFT;
            case SDL_SCANCODE_LEFTBRACKET: return AKEY_LEFTBRACKET;
            case SDL_SCANCODE_LGUI: return AKEY_LGUI;
            case SDL_SCANCODE_LSHIFT: return AKEY_LSHIFT;
            case SDL_SCANCODE_M: return AKEY_M;
            case SDL_SCANCODE_MAIL: return AKEY_MAIL;
            case SDL_SCANCODE_MEDIASELECT: return AKEY_MEDIASELECT;
            case SDL_SCANCODE_MENU: return AKEY_MENU;
            case SDL_SCANCODE_MINUS: return AKEY_MINUS;
            case SDL_SCANCODE_MODE: return AKEY_MODE;
            case SDL_SCANCODE_MUTE: return AKEY_MUTE;
            case SDL_SCANCODE_N: return AKEY_N;
            case SDL_SCANCODE_NUMLOCKCLEAR: return AKEY_NUMLOCKCLEAR;
            case SDL_SCANCODE_O: return AKEY_O;
            case SDL_SCANCODE_OPER: return AKEY_OPER;
            case SDL_SCANCODE_OUT: return AKEY_OUT;
            case SDL_SCANCODE_P: return AKEY_P;
            case SDL_SCANCODE_PAGEDOWN: return AKEY_PAGEDOWN;
            case SDL_SCANCODE_PAGEUP: return AKEY_PAGEUP;
            case SDL_SCANCODE_PASTE: return AKEY_PASTE;
            case SDL_SCANCODE_PAUSE: return AKEY_PAUSE;
            case SDL_SCANCODE_PERIOD: return AKEY_PERIOD;
            case SDL_SCANCODE_POWER: return AKEY_POWER;
            case SDL_SCANCODE_PRINTSCREEN: return AKEY_PRINTSCREEN;
            case SDL_SCANCODE_PRIOR: return AKEY_PRIOR;
            case SDL_SCANCODE_Q: return AKEY_Q;
            case SDL_SCANCODE_R: return AKEY_R;
            case SDL_SCANCODE_RALT: return AKEY_RALT;
            case SDL_SCANCODE_RCTRL: return AKEY_RCTRL;
            case SDL_SCANCODE_RETURN: return AKEY_RETURN;
            case SDL_SCANCODE_RETURN2: return AKEY_RETURN2;
            case SDL_SCANCODE_RGUI: return AKEY_RGUI;
            case SDL_SCANCODE_RIGHT: return AKEY_RIGHT;
            case SDL_SCANCODE_RIGHTBRACKET: return AKEY_RIGHTBRACKET;
            case SDL_SCANCODE_RSHIFT: return AKEY_RSHIFT;
            case SDL_SCANCODE_S: return AKEY_S;
            case SDL_SCANCODE_SCROLLLOCK: return AKEY_SCROLLLOCK;
            case SDL_SCANCODE_SELECT: return AKEY_SELECT;
            case SDL_SCANCODE_SEMICOLON: return AKEY_SEMICOLON;
            case SDL_SCANCODE_SEPARATOR: return AKEY_SEPARATOR;
            case SDL_SCANCODE_SLASH: return AKEY_SLASH;
            case SDL_SCANCODE_SLEEP: return AKEY_SLEEP;
            case SDL_SCANCODE_SPACE: return AKEY_SPACE;
            case SDL_SCANCODE_STOP: return AKEY_STOP;
            case SDL_SCANCODE_SYSREQ: return AKEY_SYSREQ;
            case SDL_SCANCODE_T: return AKEY_T;
            case SDL_SCANCODE_TAB: return AKEY_TAB;
            case SDL_SCANCODE_THOUSANDSSEPARATOR: return AKEY_THOUSANDSSEPARATOR;
            case SDL_SCANCODE_U: return AKEY_U;
            case SDL_SCANCODE_UNDO: return AKEY_UNDO;
            case SDL_SCANCODE_UNKNOWN: return AKEY_UNKNOWN;
            case SDL_SCANCODE_UP: return AKEY_UP;
            case SDL_SCANCODE_V: return AKEY_V;
            case SDL_SCANCODE_VOLUMEDOWN: return AKEY_VOLUMEDOWN;
            case SDL_SCANCODE_VOLUMEUP: return AKEY_VOLUMEUP;
            case SDL_SCANCODE_W: return AKEY_W;
            case SDL_SCANCODE_WWW: return AKEY_WWW;
            case SDL_SCANCODE_X: return AKEY_X;
            case SDL_SCANCODE_Y: return AKEY_Y;
            case SDL_SCANCODE_Z: return AKEY_Z;
            case SDL_SCANCODE_INTERNATIONAL1: return AKEY_INTERNATIONAL1;
            case SDL_SCANCODE_INTERNATIONAL2: return AKEY_INTERNATIONAL2;
            case SDL_SCANCODE_INTERNATIONAL3: return AKEY_INTERNATIONAL3;
            case SDL_SCANCODE_INTERNATIONAL4: return AKEY_INTERNATIONAL4;
            case SDL_SCANCODE_INTERNATIONAL5: return AKEY_INTERNATIONAL5;
            case SDL_SCANCODE_INTERNATIONAL6: return AKEY_INTERNATIONAL6;
            case SDL_SCANCODE_INTERNATIONAL7: return AKEY_INTERNATIONAL7;
            case SDL_SCANCODE_INTERNATIONAL8: return AKEY_INTERNATIONAL8;
            case SDL_SCANCODE_INTERNATIONAL9: return AKEY_INTERNATIONAL9;
            case SDL_SCANCODE_LANG1: return AKEY_LANG1;
            case SDL_SCANCODE_LANG2: return AKEY_LANG2;
            case SDL_SCANCODE_LANG3: return AKEY_LANG3;
            case SDL_SCANCODE_LANG4: return AKEY_LANG4;
            case SDL_SCANCODE_LANG5: return AKEY_LANG5;
            case SDL_SCANCODE_LANG6: return AKEY_LANG6;
            case SDL_SCANCODE_LANG7: return AKEY_LANG7;
            case SDL_SCANCODE_LANG8: return AKEY_LANG8;
            case SDL_SCANCODE_LANG9: return AKEY_LANG9;
            default:
            {
                LOG_ERR("This key is not handled!");
                return AKEY_UNHANDLED;
            }
        }
    }

    AKEYSTATE sdlStateToAexState(uint32_t key) {
        switch (key) {
            case SDL_KEYDOWN: return AKEYSTATE_KEYDOWN;
            case SDL_KEYUP: return AKEYSTATE_KEYUP;
            default: return AKEYSTATE_BADSTATE;
        }
    }
    
    
    TextInputEventData::TextInputEventData( uint32_t stype,uint32_t stimestamp,  uint32_t swindowID, char (&stext)[32]):
    type(stype),
    timestamp(stimestamp),
    windowID(swindowID)
    {
        memcpy ( text, stext, strlen(stext)+1 );
    }
    
    TextEditEventData::TextEditEventData( uint32_t stype,uint32_t stimestamp,  uint32_t swindowID, char (&stext)[32], int32_t sstart, int32_t slength ):
    type(stype),
    timestamp(stimestamp),
    windowID(swindowID),
    start(sstart),
    length(slength)
    {
        memcpy ( text, stext, strlen(stext)+1 );
    }

    KeyboardEventData::KeyboardEventData(uint32_t stype, uint32_t stimestamp, uint32_t swindowID, AKEYSTATE sstate, uint8_t srepeat, AEX_SCANCODE sscancode, uint16_t smod) :
    type(stype),
    timestamp(stimestamp),
    windowID(swindowID),
    state(sstate),
    repeat(srepeat),
    scancode(sscancode),
    mod(smod) {

    }

    uint32_t nameToScancode(const std::string& name) {
        try {
            // Check if key name is supported.
            return KeyNameMap.at(name);
        } catch (std::out_of_range& e) {
            // Throw exception if key is not supported.
            throw std::runtime_error("Unknown key.");
        }
        return 0;
    }

    KeyboardEventData sdlEventToAexKeyboard(SDL_KeyboardEvent* event) {
        return KeyboardEventData(event->type, event->timestamp, event->windowID, sdlStateToAexState(event->state), event->repeat, sdlKeycodeToAexKeycode(event->keysym.scancode), event->keysym.mod);
    }
    
    TextInputEventData sdlEventToAexTextInput(SDL_TextInputEvent* event) {
        return TextInputEventData(event->type, event->timestamp, event->windowID, event->text);
    }
    
    TextEditEventData  sdlEventToAexTextEdit(SDL_TextEditingEvent* event) {
        return TextEditEventData(event->type, event->timestamp, event->windowID, event->text, event->start, event->length);
    }

    void
    KeyboardEvent::handle(void* e) {
        SDL_Event* ev = static_cast<SDL_Event*> (e);



        switch (ev->type) {
            case SDL_KEYDOWN:

                key_down_event(sdlEventToAexKeyboard(&ev->key));
                break;

            case SDL_KEYUP:
                key_up_event(sdlEventToAexKeyboard(&ev->key));
                break;

            case SDL_TEXTEDITING:
                key_textedit_event(sdlEventToAexTextEdit(&ev->edit));
                break;
        
            case SDL_TEXTINPUT:
                key_textinput_event(sdlEventToAexTextInput(&ev->text));
                break;
                
            default:
                break;
        }
    }

    void
    SimpleKeyDispatch::addKeyPressListener(const aex::string& key, key_handler::key_down_dispatch_t dispatcher)
    {
        getHandler(key).key_pressed = dispatcher;
    }

    void
    SimpleKeyDispatch::addKeyReleaseListener(const aex::string& key, key_handler::key_up_dispatch_t dispatcher)
    {
        getHandler(key).key_released = dispatcher;
    }

    AEX_PUBLIC_API uint32_t extractScancode(KeyboardEventData& e) {
        return e.scancode;
    }

    AEX_PUBLIC_API NKeyData extractFromEvent(KeyboardEventData& e) {
        return
        {
            e.timestamp, e.windowID
        };
    }

    void
    SimpleKeyDispatch::key_down_event(KeyboardEventData e) {
        getHandler(extractScancode(e))->key_pressed(extractFromEvent(e));
    }

    void
    SimpleKeyDispatch::key_up_event(KeyboardEventData e) {
        getHandler(extractScancode(e))->key_released(extractFromEvent(e));
    }





}
