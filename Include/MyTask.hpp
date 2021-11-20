#ifndef __RECO_MYTASK_HPP__
#define __RECO_MYTASK_HPP__
#include "TimeTask.hpp"
#include "CacheManger.hpp"
#include <unistd.h>

namespace reco
{
class UpdateLowCache
:public TimeTask
{
public:
    UpdateLowCache()
    {}
    void process() override
    {
        printf("--------low--------\n");
        CacheManger::periodicUpdateLowCaches();
    }
};

class UpdateHighCache
:public TimeTask
{
public:
    UpdateHighCache()
    {}
    void process() override
    {
        printf("--------high-------\n");
        CacheManger::periodicUpdateHighCaches();
    }
};

class WaitTask
:public TimeTask
{
public:
    WaitTask()
    {}
    void process() override
    {
        ::sleep(1);
    }
};

}//end of namespace reco

#endif

