#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_
#include <future>
#include <mutex>
#include <deque>
template <class T>
class MessageQueue
{
public:
    void send(T&& msg);
    T receive(void);
private:
    std::deque<T> _queue;
    std::mutex _commlock;
    std::condition_variable _condition;
};
class mp3fileunit
{
private:
    /* data */
public:
    mp3fileunit(/* args */);
    ~mp3fileunit();
};

mp3fileunit::mp3fileunit(/* args */)
{
}

mp3fileunit::~mp3fileunit()
{
}

#endif