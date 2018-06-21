#ifndef __PODARRAY_HPP
#define __PODARRAY_HPP

#include <aex/PODTypes.hpp>
#include <vector>
#include <memory>

namespace aex
{

    template<typename T, class Alloc = std::allocator<T>> struct PODArray;
    template<typename T, size_t... _SIZES>
    struct PODArray<PODArrayType<T, _SIZES...>, std::allocator<PODArrayType<T, _SIZES...>>>
    {
        typedef T BaseType;
        typedef PODArrayType<T, _SIZES...> PODType;
        typedef typename PODArrayType<T, _SIZES...>::type value_type;
        typedef typename std::vector<value_type>::allocator_type allocator_type;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef typename std::allocator_traits<allocator_type>::pointer pointer;
        typedef typename std::allocator_traits<allocator_type>::const_pointer const_pointer;
        typedef typename std::vector<value_type>::iterator iterator;
        typedef typename std::vector<value_type>::const_iterator const_iterator;
        typedef typename std::vector<value_type>::reverse_iterator reverse_iterator;
        typedef typename std::vector<value_type>::const_reverse_iterator const_reverse_iterator;
        typedef typename std::vector<value_type>::difference_type difference_type;
        typedef typename std::vector<value_type>::size_type size_type;

        explicit PODArray(const allocator_type& alloc = allocator_type()) :
            m_data(alloc)
        {

        }
        explicit PODArray(size_type n):
            m_data(n)
        {

        }
        PODArray(size_type n, const value_type& val, const allocator_type& alloc = allocator_type()):
            m_data(n, val, alloc)
        {

        }
        template<class InputIterator>
        PODArray(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type()):
            m_data(first, last, alloc)
        {

        }
        PODArray(const PODArray& x):
            m_data(x.m_data)
        {
        }
        PODArray(const PODArray& x, const allocator_type& alloc):
            m_data(x.m_data, alloc)
        {

        }
        PODArray(const std::vector<value_type>& x):
            m_data(x)
        {

        }
        PODArray(const std::vector<value_type>& x, const allocator_type& alloc):
            m_data(x, alloc)
        {

        }
        PODArray(PODArray&& x):
            m_data(std::move(x.m_data))
        {

        }
        PODArray(PODArray&& x, const allocator_type& alloc):
            m_data(std::move(x.m_data), alloc)
        {

        }
        PODArray(std::vector<value_type>&& x):
            m_data(std::forward<std::vector<value_type>>(x))
        {
        }
        PODArray(std::vector<value_type>&& x, const allocator_type& alloc):
            m_data(std::forward<std::vector<value_type>>(x), alloc)
        {

        }
        PODArray(std::initializer_list<value_type> il, const allocator_type& alloc = allocator_type()):
            m_data(il)
        {
        }

        ~PODArray() {}

        iterator begin() NOEXCEPT
        {
            return m_data.begin();
        }
        const_iterator begin() const NOEXCEPT
        {
            return m_data.begin();
        }

        iterator end() NOEXCEPT
        {
            return m_data.end();
        }
        const_iterator end() const NOEXCEPT
        {
            return m_data.end();
        }

        reverse_iterator rbegin() NOEXCEPT
        {
            return m_data.rbegin();
        }
        const_reverse_iterator rbegin() const NOEXCEPT
        {
            return m_data.rbegin();
        }

        reverse_iterator rend() NOEXCEPT
        {
            return m_data.rend();
        }
        const_reverse_iterator rend() const NOEXCEPT
        {
            return m_data.rend();
        }

        const_iterator cbegin() const NOEXCEPT
        {
            return m_data.cbegin();
        }

        const_iterator cend() const NOEXCEPT
        {
            return m_data.cend();
        }

        const_reverse_iterator crbegin() const NOEXCEPT
        {
            return m_data.crbegin();
        }

        const_reverse_iterator crend() const NOEXCEPT
        {
            return m_data.crend();
        }

        size_type size() const NOEXCEPT
        {
            return m_data.size();
        }

        size_type max_size() const NOEXCEPT
        {
            return m_data.max_size();
        }

        void resize(size_type n)
        {
            m_data.resize(n);
        }

        void resize(size_type n, const value_type& val)
        {
            m_data.resize(n, val);
        }

        size_type capacity() const NOEXCEPT
        {
            m_data.capacity();
        }

        bool empty() const NOEXCEPT
        {
            return m_data.empty();
        }

        void reserve(size_type n)
        {
            m_data.reserve(n);
        }

        void shrink_to_fit()
        {
            m_data.shrink_to_fit();
        }

        reference operator[](size_type n)
        {
            return m_data[n];
        }
        const_reference operator[](size_type n) const
        {
            return m_data[n];
        }

        reference at(size_type n)
        {
            return m_data.at(n);
        }
        const_reference at(size_type n) const
        {
            return m_data.at(n);
        }

        reference front()
        {
            return m_data.front();
        }
        const_reference front() const
        {
            return m_data.front();
        }

        reference back()
        {
            return m_data.back();
        }
        const_reference back() const
        {
            return m_data.back();
        }

        value_type* data() NOEXCEPT
        {
            return m_data.data();
        }
        const value_type* data() const NOEXCEPT
        {
            return m_data.data();
        }

        template<class InputIterator>
        void assign(InputIterator first, InputIterator last)
        {
            m_data.assign(first, last);
        }
        void assign(size_type n, const value_type& val)
        {
            m_data.assign(n, val);
        }
        void assign(std::initializer_list<value_type> il)
        {
            m_data.assign(il);
        }

        void push_back(const value_type& val)
        {
            m_data.push_back(val);
        }

        void push_back(value_type&& val)
        {
            m_data.push_back(std::move(val));
        }

        void pop_back()
        {
            m_data.pop_back();
        }

        iterator insert(const_iterator position, const value_type& val)
        {
            return m_data.insert(position, val);
        }

        iterator insert(const_iterator position, size_type n, const value_type& val)
        {
            return m_data.insert(position, n, val);
        }

        template<class InputIterator>
        iterator insert(const_iterator position, InputIterator first, InputIterator last)
        {
            return m_data.insert(position, first, last);
        }
        iterator insert(const_iterator position, value_type&& val)
        {
            return m_data.insert(position, val);
        }
        iterator insert(const_iterator position, std::initializer_list<value_type> il)
        {
            return m_data.insert(position, il);
        }

        iterator erase(const_iterator position)
        {
            return m_data.erase(position);
        }
        iterator erase(const_iterator first, const_iterator last)
        {
            return m_data.erase(first, last);
        }

        void swap(PODArray& x)
        {
            m_data.swap(x.m_data);
        }

        void swap(std::vector<value_type>& x)
        {
            m_data.swap(x);
        }

        void clear() NOEXCEPT
        {
            m_data.clear();
        }

        template<class... Args>
        iterator emplace(const_iterator position, Args&& ... args)
        {
            m_data.emplace(std::forward<Args>(args)...);
        }

        template<class... Args>
        iterator emplace_back(const_iterator position, Args&& ... args)
        {
            m_data.emplace_back(std::forward<Args>(args)...);
        }

        allocator_type get_allocator() const NOEXCEPT
        {
            return m_data.get_allocator();
        }

    private:
        std::vector<value_type> m_data;
    };

    template<class T, class Alloc>
    bool operator==(const PODArray<T, Alloc>& lhs, const PODArray<T, Alloc>& rhs)
    {
        return lhs.m_data == rhs.m_data;
    }
    template<class T, class Alloc>
    bool operator!=(const PODArray<T, Alloc>& lhs, const PODArray<T, Alloc>& rhs)
    {
        return lhs.m_data != rhs.m_data;
    }
    template<class T, class Alloc>
    bool operator<(const PODArray<T, Alloc>& lhs, const PODArray<T, Alloc>& rhs)
    {
        return lhs.m_data < rhs.m_data;
    }
    template<class T, class Alloc>
    bool operator<=(const PODArray<T, Alloc>& lhs, const PODArray<T, Alloc>& rhs)
    {
        return lhs.m_data <= rhs.m_data;
    }
    template<class T, class Alloc>
    bool operator>(const PODArray<T, Alloc>& lhs, const PODArray<T, Alloc>& rhs)
    {
        return lhs.m_data > rhs.m_data;
    }
    template<class T, class Alloc>
    bool operator>=(const PODArray<T, Alloc>& lhs, const PODArray<T, Alloc>& rhs)
    {
        return lhs.m_data >= rhs.m_data;
    }

    template<class T, class Alloc>
    void swap(PODArray<T, Alloc>& x, PODArray<T, Alloc>& y)
    {
        std::swap(x.m_data, y.m_data);
    }

}


#endif
