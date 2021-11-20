#include "../Include/Threadpool.h"
#include "../Include/Thread.h"
#include "../Include/Task.h"

#include <unistd.h>

namespace wk{

Threadpool::Threadpool(size_t threadNum, size_t capacity)
:_threadNum(threadNum)
,_capacity(capacity)
,_threads()
,_taskque(_capacity)
,_isExit(false)
{
    _threads.reserve(_threadNum);
}

Threadpool::~Threadpool(){}

void Threadpool::start(){
    for(size_t idx = 0; idx != _threadNum; ++idx){
        unique_ptr<Thread> up(new Thread(
                                std::bind(&Threadpool::workerDoIOTask, this,_threadNum)));
        _threads.push_back(std::move(up));
    }
    for(auto & thread : _threads){
        thread->start();
    }
}

void Threadpool::stop(){
    if(!_isExit){
        while(!_taskque.empty()){
            ::sleep(1);
        }
        _isExit = true;
        _taskque.wakeup();
        for(auto & thread : _threads){
            thread->join();
        }
    }
}

void Threadpool::addTask(Task && task){
    _taskque.push(std::move(task));
}

Task Threadpool::getTask(){
    return _taskque.pop();
}

void Threadpool::workerDoIOTask(int threadID){
    while(!_isExit){
        Task task = getTask();
        if(task){
            task(threadID);
        }
    }
}

void Threadpool::workerDoCalTask(){
    while(!_isExit){

    }
}

}//end of namespace wk

