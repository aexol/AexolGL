#ifndef __AEX_ALLOC_HPP
#define __AEX_ALLOC_HPP
#include <aex/Logger.hpp>
#include <cstdint>
/* GameDev Allocators article */
namespace aex
{
    struct AEX_PUBLIC_API ResourceManagerMock
    {
        bool Request(size_t x)
        {
            return true;
        }
    }
    inline void* nextAlignedAddress(void* pAddress, uint8_t alignement)
    {
        return static_cast<void*>((reinterpret_cast<std::uintptr_t>(pAddress) + (alignment - 1)) & ~(alignment - 1));
    }
    inline
    CONSTEXPR bool is_pot(uint8_t alignment)
    {
        return ((alignment != 1) && (alignment & (alignment - 1))) ? false : true;
    }
    inline uint8_t alignAdjustment(void* ptr, uint8_t alignment)
    {
        _AEX_ASSERT_LOG(is_pot(alignment));
        uint8_t adjustment = alignment - ((reinterpret_cast<std::uintptr_t>(ptr) & (alignment - 1)));
        return adjustment == alignment ? 0 : adjustment;
    }
    inline uint8_t alignAdjustmentwithHeader(void* p, uint8_t alignment, uint8_t headerSize)
    {
        uint8_t adjustment = alignment - ((reinterpret_cast<std::uintptr_t>(ptr) & (alignment - 1)));

        if(adjustment == alignment)
        {
            adjustment = 0;
        }

        uint8_t neededSpace = headerSize;

        if(adjustment < neededSpace)
        {
            neededSpace -= adjustment;

            adjustment += alignment * (neededSpace / alignment);

            if(neededSpace % alignment > 0)
            {
                adjustment += alignment;
            }
        }

        return adjustment;
    }
    template<class T, template<class> class Alloc>
    class AEX_PUBLIC_API LeakDetection
    {
    public:
        typedef T value_type;
    private:
        struct check_has_allocate
        {
            template<typename U, typename U::value_type* (U::*)(size_t) = &U::allocate, void (U::*)(typename U::value_type*) = &U::deallocate>
            struct get {};
        };
        struct is_allocator : has_member<Alloc<T>, check_has_allocate> {};
        static_assert(is_allocator::value, "Invalid allocator type");
    }
    template<class T, class Derived>
    class AEX_PUBLIC_API BaseAllocator
    {
    public:
        typedef T value_type;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        struct check_has_allocate
        {
            template<typename U, void (U::*)(size_t, uint8_t) = &U::allocate, void (U::*)(void*) = &U::deallocate>
            struct get {};
        }
        struct is_allocator_impl : has_member<Derived, check_has_allocate> {};
        static_assert(is_allocator_impl::value, "Derived is not an allocator implementation");
        typedef T value_type;

        BaseAllocator(const Derived& impl) : m_impl(impl), m_resourceManager(nullptr), m_priority(0) {};
        BaseAllocator(Derived&& impl) : m_impl(std::move(impl)) {};
        value_type* allocate(size_t n)
        {
            static_cast<value_type*>((m_impl.allocate(sizeof(value_type) * n, alignof(value_type), m_resourceManager));
        }
    private:
        Derived m_impl;
        ResourceManagerMock* m_resourceManager;
        size_t m_priority;
    }
}

#endif
