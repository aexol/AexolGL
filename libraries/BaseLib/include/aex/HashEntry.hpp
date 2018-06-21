/** @file HashEntry.hpp
 *  @brief Definition of HashEntry.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__MapEntry__
#define __aexolGL__MapEntry__

#include <type_traits>
#include <cstring>

namespace aex
{
    namespace detail
    {
        template<class T>
        static auto test_stream(int) -> decltype(std::declval<T>().hash(), void());

        template<class>
        static char test_stream(long);
    }
    template<class T>
    struct has_hash : std::is_void<decltype(detail::test_stream<T>(0))> {};

    /**
     *	@brief	Entry in hash_table.
     */
    template<typename V> class HashEntry
    {

    public:
        /**
         *	@brief	Ctor
         */
        HashEntry(const V&);

        long getHash()
        {
            return m_hash;
        }
        
        V getValue()
        {
            return m_v;
        }
        /**
         *	@brief	Equality.
         *
         *	@param 	e 	compare to.
         *
         *	@return	True if equal.
         */
        bool operator==(HashEntry& e)
        {
            return (m_v == e.m_v) && (m_hash == e.m_hash);
        }
    private:
        V m_v;
        size_t m_hash;
    };
    template <typename V>
    HashEntry<V>::HashEntry(const V& v)
    {
        static_assert(has_hash<V>() == true, "Template argument does not implement hash() method");
        m_hash = v.hash();
        m_v = v;
    }
}

#endif /* defined(__aexolGL__MapEntry__) */
