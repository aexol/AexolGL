#ifndef __RESOURCE_MANAGER_HPP
#define __RESOURCE_MANAGER_HPP

#include <typeinfo>
#include <aex/ManagedHandleMgr.hpp>
#include <unordered_map>

namespace aex
{

    inline IResourceManager::~IResourceManager() {}

    template<template T>
    struct has_resource_type
    {
        std::true_type check(T::resource_type*);
        std::false_type check(...);
        static CONSTEXPR bool = std::is_same < decltype(check(0),)
    }
    template<typename T>
    struct has_type : T


        namespace __resource_manager_detail
        {
            struct _check_has_resource_type
            {
                template<class T, class U = typename T::resource_type>
                struct get {};
            };
            template<class T> struct check_has_resource_type : aex::has_member<T, _check_has_resource_type> {};
            template<class T, bool = check_has_resource_type<T>::value> struct if_has_resource_type
            {
                typedef T type;
            };
            template<class T> struct if_has_resource_type<T, true>
            {
                typedef typename T::resource_type type;
            };
        }

    template<class __Dt>
    class ResourceManager : public IResourceManager
    {
    public:
        typedef ManagedData<typename __resource_manager_detail::if_has_resource_type<__Dt>::type> data_type;
        typedef aexFramework::Handle<data_type> handle_type;
        typedef std::unordered_map<aex::string, handle_type> NameIndex;
        typedef std::pair<typename NameIndex::iterator, bool> NameIndexInsertRet;
        typedef aexFramework::HandleMgr < __
    };

}

#endif
