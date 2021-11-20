#ifndef __RECO_THREAD_HPP__
#define __RECO_THREAD_HPP__

#include <pthread.h>
#include <functional>

namespace reco
{

class Thread
{
public:
    Thread();
    virtual ~Thread();
    void start();
    void stop();
    bool getFlag() const;

    static void* threadFunc(void *);
    virtual void run() = 0;

private:
    pthread_t _pthid;
    bool _isRunning;
};

}//end of namespace reco

#endif

