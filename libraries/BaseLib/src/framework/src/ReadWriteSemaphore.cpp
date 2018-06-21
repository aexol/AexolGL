#include <aex/ReadWriteSemaphore.hpp>

namespace aex
{
    void ReadWriteSemaphore::write_lock()
    {
        while(!constructed);

        m_writeLock.lock();
        unsigned acquired = 0;

        while(acquired < ReadersLimit)
        {
            acquired += try_acquire();
        }
    }
    void ReadWriteSemaphore::write_unlock()
    {
        m_count -= ReadersLimit;
        m_writeLock.unlock();
    }
    bool ReadWriteSemaphore::try_acquire()
    {
        auto currCount = m_count.load(std::memory_order_acquire);
        auto desired = currCount + 1;

        if(desired <= ReadersLimit)
        {
            return m_count.compare_exchange_strong(currCount, desired);
        }

        return false;
    }
    void ReadWriteSemaphore::release()
    {
        m_count--;
    }
    SemaphoreWriteGuard::SemaphoreWriteGuard(ReadWriteSemaphore& sem) : shared(sem)
    {
        shared.write_lock();
    }
    SemaphoreWriteGuard::~SemaphoreWriteGuard()
    {
        shared.write_unlock();
    }
    SemaphoreReadGuard::SemaphoreReadGuard(ReadWriteSemaphore& sem) : shared(sem)
    {
        while(!shared.try_acquire());
    }
    SemaphoreReadGuard::~SemaphoreReadGuard()
    {
        shared.release();
    }
}

