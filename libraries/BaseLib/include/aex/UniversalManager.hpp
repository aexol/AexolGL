#ifndef __AEX_MANAGER_HPP
#define __AEX_MANAGER_HPP
#include <aex/details/cds_helpers.hpp>
#include <aex/SharedResourceMap.hpp>
#include <mutex>
#include <exception>
#include <atomic>
#include <string>

namespace aex
{

    class AEX_PUBLIC_API out_of_memory : std::exception
    {
        public:
            virtual const char* what() const NOEXCEPT
            {
                return "Out of memory";
            }
    };

    namespace _manager_detail {
        namespace cc = ::cds::container;
        template<class KeyType, class DataType, class Alloc = std::allocator<DataType>>
        struct _eval_default_cc_splitlistmap{
            struct map_traits : public cc::split_list::traits {
                using ordered_list = cc::michael_list_tag;
                using hash = std::hash<KeyType>;
                using bit_reversal = aex::_cds_details::lookup;
                struct ordered_list_traits : public cc::michael_list::traits {
                    using less = std::less<KeyType>;
                };
            };
            using type = map::_add_allocator<cc::SplitListMap<cds::gc::HP, KeyType, DataType, map_traits>, Alloc>;
        };

        template<class KeyType, class DataType, class Alloc = std::allocator<DataType>>
        using _default_cc_splitlistmap = typename _eval_default_cc_splitlistmap<
            KeyType,
            DataType,
            Alloc>::type;
    }

    class IManager{};

    template<class KeyType, class DataType, class TMap>
    class TResourceManager final : public IManager {
            using map_type = SharedResourceMapAdaptor<TMap>;
            map_type m_map;
        public:
            using uh_type = typename map_type::accessor_type;
            using key_type = typename map_type::key_type;
            using mapped_type = typename map_type::mapped_type;
            using value_type = typename map_type::value_type;
            using allocator_type = typename map_type::allocator_type;
            template<class... Args>
            uh_type GetResourceHandle(const key_type& key, Args&&... args) {
                uh_type h;
                m_map.insert(key, h, aex::forward<Args>(args)...);
                return h;
            }
    };

    template<class DataType, class Alloc = std::allocator<DataType>>
    using Manager = TResourceManager<std::string, DataType, _manager_detail::_default_cc_splitlistmap<std::string, DataType, Alloc>>;

}

#endif
