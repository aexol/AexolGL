#include "aex/gui/Flags.hpp"

namespace aex
{
    namespace gui
    {
        namespace flags
        {
            void clearFlags(int* flags)
            {
                *flags = 0;
            }
            
            void setFlag(int flag, int* inFlags)
            {
                (*inFlags) = (*inFlags) | flag;
            }
            
            bool checkFlag(int flag, int inFlags)
            {
                return ((inFlags & flag) != 0);
            }
            
            bool checkFlag(FLAG flag, int inFlags)
            {
                return checkFlag(static_cast<int>(flag), inFlags);
            }
        }
    }
}