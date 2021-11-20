#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include "MutexLock.h"
#include "Condition.h"
#include "Task.h"

#include <queue>

using std::queue;

namespace wk{

using Elemtype = Task;

class TaskQueue
{
public:
    TaskQueue(size_t capacity);

    bool full() const;
    bool empty() const;
    void push(Elemtype && elem );
    Elemtype pop();
    void wakeup();

private:
    size_t _capacity;
    queue<Elemtype> _que;
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;
    bool _flag;
};

}//end of namespace wk

#endif

