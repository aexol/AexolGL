#ifndef __ABSTRACTFACTORY_HPP
#define __ABSTRACTFACTORY_HPP
#include <aex/Common.hpp>
#include <type_traits>
#include <tuple>
namespace aex
{
    template<class T>
    class AEX_PUBLIC_API AbstractFactory
    {
    public:
        virtual shared_ptr<T> createObject() = 0;
        virtual inline ~AbstractFactory() = 0;
    };
    template<class T>
    AbstractFactory<T>::~AbstractFactory() {}

    template<typename T> using AbstractFactoryPtr = shared_ptr<AbstractFactory<T>>;

    template<class T, class S>
    class AEX_PUBLIC_API FactorialUnit
    {
    public:
    };

    template<class T, class S, typename TAG, typename... Args>
    struct AEX_PUBLIC_API CheckFactorialUnit
    {
        typedef char no;
        struct yes
        {
            char _[2];
        };


        template<typename Type,  shared_ptr<S>(Type::*createObjectFunc)(std::tuple<Args...>, TAG*) = &Type::createObject>
        static typename std::enable_if<std::is_member_function_pointer<decltype(createObjectFunc)>::value, std::true_type>::type check(Type*, typename std::enable_if<std::is_lvalue_reference<decltype(Type::getInstance())>::value, decltype(&Type::getInstance)>::type = 0);
//
        // shared_ptr<S> (Type::*createObjectFunc)(Args...) = &(Type::createObject),
        //std::is_member_function_pointer<decltype(createObjectFunc)>::value &&

        template<typename Type>
        static std::false_type check(...);

        typedef decltype(check<FactorialUnit<T, S>>(0)) value_type;
        enum { value = (int)value_type() };
//        enum { value = (sizeof( check<FactorialUnit<T, S>>(0)) == sizeof(yes)) };
    };

    template<class T, class S, typename TAG, typename... Args>
    class AEX_PUBLIC_API ConcreteFactory : public AbstractFactory<S>
    {
    private:
        std::tuple<Args...> m_internal;
    public:
        ConcreteFactory(Args... arg);
        shared_ptr<S> createObject();

        virtual inline ~ConcreteFactory() {}
    };

    template<class T, class S, typename TAG, typename... Args>
    ConcreteFactory<T, S, TAG, Args...>::ConcreteFactory(Args... args) : m_internal(args...)
    {

    }

    template<class T, class S, typename TAG, class ...Args>
    shared_ptr<S>
    ConcreteFactory<T, S, TAG, Args...>::createObject()
    {
        static_assert(CheckFactorialUnit<T, S, TAG, Args...>::value, "Factory not implemented");
        FactorialUnit<T, S>& unit = FactorialUnit<T, S>::getInstance();
        return unit.template createObject(m_internal, static_cast<TAG*>(0));
    }
}
#endif
