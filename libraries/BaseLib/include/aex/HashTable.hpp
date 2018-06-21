/** @file HashTable.hpp
 *  @brief Definition of HashTable.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__HashTable__
#define __aexolGL__HashTable__

#include <aex/HashEntry.hpp>
#include <vector>
#include <list>
#include <memory>
#include <cassert>
#include <stdexcept>

namespace aex
{



    /**
     *	@brief HashTable, I'm sure I needed it somewhere, but wehere eludes me.
     */
    template<typename V> class HashTable
    {

    public:
        typedef std::unique_ptr<std::list<HashEntry<V>>>    BUCKET;
        typedef std::list<HashEntry<V>>*                    BUCKEThandle;
        /**
         *	@brief	HashTable iter.
         */
        struct iterator
        {

        public:
            iterator(HashTable<V>& p) : m_index(0), m_parent(p)
            {
                while(!m_parent.m_table[m_index])
                {
                    m_index++;

                    if(m_index >= m_parent.m_table.size())
                    {
                        break;
                    }
                }

                if(m_index < m_parent.m_table.size())
                {
                    list_iter = m_parent.m_table[m_index]->begin();
                }
            }
            iterator(HashTable<V>& p, size_t index) : m_index(index), m_parent(p)
            {
                if(m_index < m_parent.m_table.size())
                {
                    while(!m_parent.m_table[m_index])
                    {
                        m_index++;

                        if(m_index >= m_parent.m_table.size())
                        {
                            break;
                        }
                    }

                    if(m_index < m_parent.m_table.size())
                    {
                        list_iter = m_parent.m_table[m_index]->begin();
                    }
                }
            }
            HashEntry<V>& operator*()
            {
                return *list_iter;
            }
            iterator& operator++()
            {
                list_iter++;

                if(list_iter != m_parent.m_table[m_index]->end())
                {
                    return (*this);
                }
                else
                {
                    m_index++;

                    while(!m_parent.m_table[m_index])
                    {
                        m_index++;

                        if(m_index >= m_parent.m_table.size())
                        {
                            break;
                        }
                    }

                    if(m_index < m_parent.m_table.size())
                    {
                        list_iter = m_parent.m_table[m_index]->begin();
                        return (*this);
                    }
                }

                return (*this);
            };
            bool operator==(iterator& it)
            {
                if(&m_parent == &it.m_parent)
                {
                    if(m_index == it.m_index)
                    {
                        if(m_index >= m_parent.m_table.size())
                        {
                            return true;
                        }

                        if(list_iter == it.list_iter)
                        {
                            return true;
                        }
                    }
                    else
                    {
                        if(m_index >= m_parent.m_table->size() && it.m_index >= m_parent.m_table->size())
                        {
                            return true;
                        }
                    }
                }

                return false;
            }
            bool operator!=(iterator& it)
            {
                if(&m_parent == &it.m_parent)
                {
                    if(m_index == it.m_index)
                    {
                        if(m_index >= m_parent.m_table.size())
                        {
                            return false;
                        }

                        if(list_iter == it.list_iter)
                        {
                            return false;
                        }
                    }
                    else
                    {
                        if(m_index >= m_parent.m_table.size() && it.m_index >= m_parent.m_table.size())
                        {
                            return false;
                        }
                    }
                }

                return true;
            }
        private:
            HashTable&                                  m_parent;
            size_t                                      m_index;
            typename std::list<HashEntry<V>>::iterator  list_iter;
        };

        HashTable();
        HashTable(size_t n);
        /**
         *	@brief	Add element to table.
         *
         *	@param 	V& 	reference to element.
         */
        inline void put(const V&);

        /**
         *	@brief	Element in table.
         *
         *	@param 	V& 	Element
         *
         *	@return	True if in.
         */
        inline bool contains(const V&) const;

        /**
         *	@brief	Current table size.
         */
        inline size_t size()
        {
            return m_size;
        }

        /**
         *	@brief	Reference to value.
         */
        inline V&     getValue(const V&);


        inline size_t getIndex(size_t hash) const
        {
            return hash & m_significant_bits;
        }
        /**
         *	@brief	Begin iterator.
         */
        iterator begin();

        /**
         *	@brief	end iterator.
         */
        iterator end();


    private:
        std::vector<BUCKET> m_table;
        size_t              m_significant_bits;
        size_t              m_size;
    };
    template<typename V>
    HashTable<V>::HashTable() : m_table(16), m_significant_bits(4), m_size(0)
    {

    }
    template<typename V>
    HashTable<V>::HashTable(size_t n) : m_significant_bits(n), m_size(0)
    {
        size_t size_t_bits = sizeof(size_t) * 8;

        if(!((n != 0) && ((n & (~n + 1)) == n)))
        {
            n--;

            for(size_t i = 1; i < size_t_bits; i *= 2)
            {
                n |= n >> i;
            }

            m_significant_bits = n;
            n++;
        }

        m_table.resize(n);

    }
    template<typename V>
    void HashTable<V>::put(const V& v)
    {
        HashEntry<V> entry(v);
        size_t index = getIndex(entry.getHash());

        if(!m_table[index])
        {
            m_table[index] = BUCKET(new std::list<HashEntry<V>>);
        }

        m_table[index]->push_back(entry);
        m_size++;
    }
    template<typename V>
    V& HashTable<V>::getValue(const V& v)
    {
        HashEntry<V> entry(v);
        size_t index = getIndex(entry.getHash());

        if(index > m_table.size())
        {
            throw std::out_of_range("Somethings's wrong, have no idea what");
        }

        if(!m_table[index])
        {
            throw std::out_of_range("Try again!:D");
        }

        BUCKET bucket = m_table[index];

        for(auto& e : bucket)
        {
            if(entry == e)
            {
                return e.getValue();
            }
        }

        throw std::out_of_range("Bardzo madry wyjatek");
    }
    template<typename V>
    bool HashTable<V>::contains(const V& v) const
    {
        HashEntry<V> entry(v);
        size_t index = getIndex(entry.getHash());

        if(index >= m_table.size())
        {
            return false;
        }

        if(!m_table[index])
        {
            return false;
        }

        BUCKEThandle bucket = m_table[index].get();

        for(auto& e : (*bucket))
        {
            if(v == e.getValue())
            {
                return true;
            }
        }

        return false;
    }
    template<typename V>
    typename HashTable<V>::iterator HashTable<V>::begin()
    {
        return iterator(*this);
    }
    template<typename V>
    typename HashTable<V>::iterator HashTable<V>::end()
    {
        return iterator((*this), m_table.size());
    }
}

#endif /* defined(__aexolGL__HashTable__) */
