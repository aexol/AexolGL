/** @file Indexer.hpp
 *  @brief Definition of Indexer.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__Indexer__
#define __aexolGL__Indexer__

#include <aex/Math.hpp>
#include <aex/HashTable.hpp>
#include <array>


namespace aex
{

    struct AEX_PUBLIC_API IndexingPoint
    {
        std::array<float, 3>  vertex;
        size_t hash() const
        {
            size_t x = *(uint32_t*)&vertex[0];
            size_t y = *(uint32_t*)&vertex[1];
            size_t z = *(uint32_t*)&vertex[2];
            size_t ret = (x * 73856093) ^ (y * 19349663) ^ (z * 83492791);
            return ret;
        }
        bool operator==(const IndexingPoint& p) const
        {
            if(vertex[0] == p.vertex[0] && vertex[1] == p.vertex[1] && vertex[2] == p.vertex[2])
            {
                return true;
            }

            return false;
        }
    };

    struct AEX_PUBLIC_API Line
    {
        std::array<int16_t, 2>  line;
        size_t hash() const
        {
            size_t x = *(size_t*)&line[0];
            size_t y = *(size_t*)&line[1];
            size_t ret = (x * 73856093) ^ (y * 19349663);
            return ret;
        }
        bool operator==(const Line& l) const
        {
            if(line[0] == l.line[0] && line[1] == l.line[1])
            {
                return true;
            }

            return false;
        }
    };

    /**
     *	@brief	Helper class for creating indices.
     */
    template<typename V> class Indexer
    {
    public:
        aex::HashTable<V>   m_table;

        Indexer(size_t n) : m_table(n) { }
        inline void   add(const V& v);
        inline size_t size()
        {
            return m_table.size();
        }
    };
    template <typename V>
    void Indexer<V>::add(const V& v)
    {
        if(!m_table.contains(v))
        {
            m_table.put(v);
        }
    }
}

#endif /* defined(__aexolGL__Indexer__) */
