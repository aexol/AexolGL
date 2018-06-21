#ifndef __AEX_READWRITE_SEMAPHORE
#define __AEX_READWRITE_SEMAPHORE

#include <aex/Common.hpp>
#include <atomic>
#include <mutex>
#include <thread>

namespace aex
{

    /*
     *                class TFSpinLock {
     *        typedef enum {TFSpinLockLocked, TFSpinLockUnlocked} TFSpinLockState;
     *        std::atomic<TFSpinLockState> m_state;
     *
     *        public:
     *        TFSpinLock() : m_state(TFSpinLockUnlocked) {}
     *        void lock();
     *        void unlock();
     *        };
     *                void TFSpinLock::lock() {
     *    while (m_state.exchange(TFSpinLockLocked, std::memory_order_acquire) == TFSpinLockLocked) {
     *    }
     *
     *}
     */

    /*
     *void TFSpinLock::unlock() {
     *    m_state.store(TFSpinLockUnlocked, std::memory_order_release);
     *}
     */
    class AEX_PUBLIC_API ReadWriteSemaphore
    {
    public:
        ReadWriteSemaphore(unsigned limit = std::thread::hardware_concurrency()) : m_writeLock(), m_count(0), ReadersLimit(limit), constructed(1) { }
        ReadWriteSemaphore(const ReadWriteSemaphore&) = delete;
        void write_lock();
        void write_unlock() ;
        bool try_acquire();
        void release();

    private:
        mutable std::mutex m_writeLock;
        std::atomic<unsigned int> m_count;
        const unsigned int ReadersLimit;
        unsigned char constructed;
    };
    class AEX_PUBLIC_API SemaphoreWriteGuard
    {
    public:
        explicit SemaphoreWriteGuard(ReadWriteSemaphore&);
        ~SemaphoreWriteGuard();
    private:
        ReadWriteSemaphore& shared;
        std::unique_lock<std::mutex> lock;
    };
    class AEX_PUBLIC_API SemaphoreReadGuard
    {
    public:
        explicit SemaphoreReadGuard(ReadWriteSemaphore&);
        ~SemaphoreReadGuard();
    private:
        ReadWriteSemaphore& shared;
    };
}

#endif
