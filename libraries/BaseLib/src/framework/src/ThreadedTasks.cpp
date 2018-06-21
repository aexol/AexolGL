#include "aex/ThreadedTasks.hpp"

#include <aex/Logger.hpp>

Task::Task() :
bInvalid(false)
{
}

bool Task::invalid(){
    return bInvalid;
}

void Task::setInvalid(){
    bInvalid = true;
}

NormalTask::NormalTask(const std::function<void()>& task) :
task(task)
{
}

void NormalTask::tick()
{
    if(this->invalid())
        return;

    task();
    setInvalid();
}

DelayedTask::DelayedTask(const std::function<void()>& task, unsigned long long delayMills) :
NormalTask(task),
delayMills(delayMills)
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    startTimestamp = millis;
}

void DelayedTask::tick()
{
    if(this->invalid())
        return;

    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto nowMills = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

   // LOG("startTime: ", startTimestamp, " now: ", nowMills);

    if ((nowMills - startTimestamp) >= delayMills)
    {
        task();
        setInvalid();
    }
}

ThreadedTasks::ThreadedTasks() :
paused(false)
{
}

bool ThreadedTasks::init()
{
    return true;
}

void ThreadedTasks::pause()
{
    paused = true;
}

void ThreadedTasks::resume()
{
    paused = false;
}

void ThreadedTasks::update()
{
    if(m_done || paused){
        return;
    }

    std::lock_guard<std::mutex> lg(m);
    tasks.remove_if([](const std::unique_ptr<Task>& t){ return t->invalid(); });

    spliceNewTasks();
    std::for_each(tasks.begin(), tasks.end(), [](std::unique_ptr<Task>& t){ t->tick(); } );
    //LOG("tasks size", tasks.size());
}

void ThreadedTasks::finish()
{
    m_done = true;
}

void ThreadedTasks::pushTask(const std::function<void()>& normalTask)
{
    pushTask(std::unique_ptr<Task>(new NormalTask(normalTask)));
}

void ThreadedTasks::pushTask(std::unique_ptr<Task> task){
    std::lock_guard<std::mutex> lg(newTasksMutex);
    newTasks.push_back(std::move(task));
}

void ThreadedTasks::spliceNewTasks(){
    std::lock_guard<std::mutex> lg(newTasksMutex);
    tasks.splice(tasks.end(), newTasks);
}
