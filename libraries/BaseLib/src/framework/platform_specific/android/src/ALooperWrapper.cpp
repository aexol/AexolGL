#include "../include/ALooperWrapper.hpp"
#include <android/looper.h>
#include <algorithm>

namespace aex
{
    ALooperWrapper::ALooperWrapper()
    {
        ALooper* looper = ALooper_forThread();

        if(!looper)
        {
            looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
        }
    }
    void
    ALooperWrapper::exec()
    {
        int ident;

        while((ident = ALooper_pollAll(0, NULL, NULL, NULL)) >= 0)
        {
            for(AQueue* q : m_queues)
            {
                q->poll();
            }
        }
    }
    void
    ALooperWrapper::add(AQueue* q)
    {
        if(!q)
        {
            return;
        }

        auto findIter = std::find(m_queues.begin(), m_queues.end(), q);

        if(findIter == m_queues.end())
        {
            return;
        }

        m_queues.push_back(q);
    }
    void
    ALooperWrapper::remove(AQueue* q)
    {
        if(!q)
        {
            return;
        }

        auto findIter = std::find(m_queues.begin(), m_queues.end(), q);

        if(findIter != m_queues.end())
        {
            m_queues.erase(findIter);
        }
    }
}
