#ifndef __AEX_INSTANCE_IPP
#define __AEX_INSTANCE_IPP
#include <aex/ASceneNode.hpp>
#include <aex/Aex.hpp>
#include <aex/Instance.hpp>
namespace aex
{
    template<class T>
    void
    TInstance<T>::reset(TAex<T>* parent)
    {
        std::swap(parent, m_parentAex);
        if(parent)
            parent->removeInstance(this);
        if(m_parentAex)
            m_parentAex->addInstance(this);
    }

    template<class T>
    TInstance<T>::TInstance() :
        T(),
        m_parentAex(nullptr)
    {}

    template<class T>
    TInstance<T>::TInstance(TAex<T>& aex) : T(aex)
    {
        reset(&aex);
    }

    template<class T>
    void
    TInstance<T>::setParent(TAex<T>* aex)
    {
        T::operator=(*aex);
        reset(aex);
    }

    template<class T>
    TInstance<T>::~TInstance()
    {
        reset();
    }

    typedef TInstance<TRSNode> Instance;
}
#endif  // __AEX_INSTANCE_IPP
