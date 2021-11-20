#ifndef __THREAD_H__
#define __THREAD_H__

#include "Noncopyable.h"
#include "ThreadID.hpp"

#include <pthread.h>

#include <functional>

namespace wk{

class Thread : public Noncopyable
{
public:
    using ThreadCallback = std::function<void()>;
    Thread(ThreadCallback && cb);
    ~Thread();
    void start();
    void join();

private:
    static void *threadFunc(void*);

private:
    pthread_t _pthid;
    bool _isRunning;
    ThreadCallback _cb;
};

}//namespace wk end

#endif

