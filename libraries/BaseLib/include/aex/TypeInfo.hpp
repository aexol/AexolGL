#ifndef TYPEINFO_H
#define TYPEINFO_H

#include <typeinfo>

namespace aex
{
//!
//! This class is nothing special, just a simple wrapper for std::type_info adding operator <
//! which makes it possible to use with default std::map Compare template argument
//!
    class AEX_PUBLIC_API TypeInfo
    {
    public:
        explicit TypeInfo(const std::type_info& info) : m_typeInfo(info) {}
        bool operator < (const TypeInfo& rhs) const
        {
            return m_typeInfo.before(rhs.m_typeInfo) != 0;
        }

    private:
        const std::type_info& m_typeInfo;
    };

}
#endif
