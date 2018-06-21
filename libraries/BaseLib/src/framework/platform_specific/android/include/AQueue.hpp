#ifndef __AEX_AQUEUE_HPP
#define __AEX_AQUEUE_HPP

namespace aex
{
    class AQueue
    {
    public:
        virtual void poll() = 0;
        virtual ~AQueue() = 0;
    };
    inline AQueue::~AQueue() {}
}

#endif
