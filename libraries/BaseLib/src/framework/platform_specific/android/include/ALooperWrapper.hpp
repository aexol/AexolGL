#ifndef __AEX_ALOOPERWRAPPER_HPP
#define __AEX_ALOOPERWRAPPER_HPP
#include "AQueue.hpp"
#include <aex/ThreadLocalSingleton.hpp>
#include <list>

namespace aex
{
    class ALooperWrapper : public ThreadLocalSingleton<ALooperWrapper>
    {
    protected:

    public:
        ALooperWrapper();
        void exec();
        void add(AQueue* q);
        void remove(AQueue* q);
    private:
        std::list<AQueue*> m_queues;
        friend class ThreadLocalSingleton<ALooperWrapper>;
    };


}

#endif
