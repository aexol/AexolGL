#ifndef THREADED_TASKS_HPP_FILE
#define THREADED_TASKS_HPP_FILE


#include <exception>
#include <map>
#include <array>
#include <vector>
#include <functional>
#include <atomic>
#include <queue>
#include <memory>
#include <mutex>
#include <string>
#include <iostream>
#include <chrono>
#include <list>

#include <aex/ALoopNode.hpp>

class ThreadedTasks;

class AEX_PUBLIC_API Task {
public:
    Task();
    friend class ThreadedTasks;
protected:
    virtual void tick() = 0;
    bool invalid();
    void setInvalid();
protected:
    bool bInvalid;
};

class AEX_PUBLIC_API NormalTask : public Task {
public:
    NormalTask(const std::function<void()>& task);

    friend class ThreadedTasks;
protected:
    virtual void tick();
protected:
    std::function<void() > task;
};

class AEX_PUBLIC_API DelayedTask : public NormalTask {
public:
    DelayedTask(const std::function<void()>& task, unsigned long long delayMills);

    friend class ThreadedTasks;
protected:
    virtual void tick();
protected:
    unsigned long long delayMills;
    unsigned long long startTimestamp;
};

class AEX_PUBLIC_API ThreadedTasks : public aex::ALoopNode {
public:
    ThreadedTasks();
    virtual bool init();
    virtual void pause();
    virtual void resume();
    virtual void update();
    virtual void finish();

    void pushTask(const std::function<void()>& normalTask);
    void pushTask(std::unique_ptr<Task> task);
protected:
    void spliceNewTasks();

private:
    std::list<std::unique_ptr<Task> > newTasks;
    std::list<std::unique_ptr<Task> > tasks;
    std::mutex m;
    std::mutex newTasksMutex;

    bool paused;
};



#endif // TASKS_QUEUE_HPP
