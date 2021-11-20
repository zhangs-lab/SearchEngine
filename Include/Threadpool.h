#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "TaskQueue.h"
#include "Task.h"

#include <queue>
#include <vector>
#include <memory>
#include <iostream>

using std::vector;
using std::unique_ptr;

namespace wk{

class Thread;

class Threadpool
{
    
public:
    Threadpool(size_t, size_t);//线程数量，容量
    ~Threadpool();

    void start();
    void stop();

    void addTask(Task && task);

private:
    Task getTask();
    void workerDoIOTask(int threadID);
    void workerDoCalTask();

private:
    size_t _threadNum;
    size_t _capacity;
    vector<unique_ptr<Thread>> _threads;
    TaskQueue _taskque;
    bool _isExit;
};

}//end of namespace wk

#endif

