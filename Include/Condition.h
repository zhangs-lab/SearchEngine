#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "Noncopyable.h"

#include <pthread.h>

namespace wk{

class MutexLock;

class Condition : Noncopyable
{
public:
    Condition(MutexLock &mutex);
    ~Condition();

    void wait();
    void notify();
    void notifyAll();

private:
    MutexLock& _mutex;
    pthread_cond_t _cond;
};

}//end of namespace wk

#endif

