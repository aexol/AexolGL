#include <aex/GPUApi.hpp>
#include <aex/Exceptions.hpp>

namespace aex
{
    void 
    IOnContextDirty::__onContextDirty()
    {
        onContextDirty();
    }

    IOnContextDirty::IOnContextDirty() 
    {
        try{
            m_onContextDirtyCon = OnContextDirty(boost::bind(&IOnContextDirty::__onContextDirty, this));
        } catch(UninitializedContextError& e)
        {
            LOG("Invalid IOnContextDirty object. Reason: ", e.what());
        }
    }

    IOnContextDirty::IOnContextDirty(const IOnContextDirty& drwObj) : IOnContextDirty()
    {
        UNUSED(drwObj);
    }
    IOnContextDirty::IOnContextDirty(IOnContextDirty&& drwObj) : IOnContextDirty()
    {
        UNUSED(drwObj);
    }

    IOnContextDirty& 
    IOnContextDirty::operator=(const IOnContextDirty&)
    {
        m_onContextDirtyCon = OnContextDirty(boost::bind(&IOnContextDirty::__onContextDirty, this));
        return *this;
    }

    IOnContextDirty& 
    IOnContextDirty::operator=(IOnContextDirty&&)
    {
        m_onContextDirtyCon = OnContextDirty(boost::bind(&IOnContextDirty::__onContextDirty, this));
        return *this;
    }
    IOnContextDirty::~IOnContextDirty(){}
}
