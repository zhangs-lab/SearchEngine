#include "../Include/Thread.h"
#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

namespace wk{

Thread::Thread(ThreadCallback && cb)
:_pthid(0)
,_isRunning(false)
,_cb(std::move(cb))
{}

Thread::~Thread(){
    if(_isRunning){
        pthread_detach(_pthid);
        _isRunning = false;
    }
}

void Thread::start(){
    int ret = 0;
    if((ret = pthread_create(&_pthid, nullptr, threadFunc, this)) != 0){
        printf("%s : %s\n", "pthread_create", strerror(ret));
        return;
    }
    _isRunning = true;
}

void *Thread::threadFunc(void* p){
    Thread* pthread = static_cast<Thread*>(p);
    if(pthread){
        pthread->_cb();
    }
    return nullptr;
}

void Thread::join(){
    if(_isRunning){
        pthread_join(_pthid, nullptr);
        _isRunning = false;
    }
}

}//end of namespace wk
