#include "../Include/TaskQueue.h"

namespace wk{

TaskQueue::TaskQueue(size_t capacity)
:_capacity(capacity)
,_que()
,_mutex()
,_notFull(_mutex)
,_notEmpty(_mutex)
,_flag(true)
{}

bool TaskQueue::full() const{
    return _que.size() == _capacity;
}

bool TaskQueue::empty() const{
    return _que.size() == 0;
}

void TaskQueue::push(Elemtype&& elem){
    {
        MutexLockGuard autolock(_mutex);
        while(full()){
            _notFull.wait();
        }

        _que.push(std::move(elem));
    }
    _notEmpty.notify();
}

Elemtype TaskQueue::pop(){
    MutexLockGuard autolock(_mutex);
    while(_flag && empty()){
        _notEmpty.wait();
    }

    if(_flag){
        Elemtype tmp = _que.front();
        _que.pop();

        _notFull.notify();
        return tmp;
    }else{
        return nullptr;
    }
}

void TaskQueue::wakeup(){
    _flag = false;
    _notEmpty.notifyAll();
}

}//end of namespace wk



