#include "../Include/Thread.hpp"
#include "../Include/CacheManger.hpp"
#include <pthread.h>
#include <iostream>

using std::cout;
using std::endl;

namespace reco
{

Thread::Thread()
:_pthid(0)
,_isRunning(false)
{}
Thread::~Thread()
{
    if(_isRunning){
        _isRunning=false;
    }
    pthread_detach(_pthid);
}

void Thread::start()
{
    int ret=pthread_create(&_pthid,nullptr,threadFunc,this);
    if(0!=ret){
        perror("thread_start");
        return ;
    }
    _isRunning=true;
}
void Thread::stop()
{
    if(_isRunning) _isRunning=false;
    pthread_join(_pthid,nullptr);
}
bool Thread::getFlag() const
{
    return _isRunning;
}

void* Thread::threadFunc(void* arg)
{
    Thread* pthread=static_cast<Thread*>(arg);
    if(pthread)
    {
        pthread->run();
    }
    return nullptr;
}

}//end  of namespace reco
