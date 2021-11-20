#ifndef __RECO_UPDATECACHE_HPP__
#define __RECO_UPDATECACHE_HPP__

#include "Thread.hpp"
#include "TimeManager.hpp"

namespace reco
{

class UpdateCache
:public Thread 
{
public:
    UpdateCache()
    :_taskque(TimeManager())
    {}
private:
    void run() override
    {  
        while(getFlag())
        {   
            int n=2*(_taskque._timefd+1);
            for(int i=0;i<n;++i)
            {
                if(i==_taskque._timefd){
                    _taskque._flag=1;//第一组cache在更新
                }else if(i==n-1){
                    _taskque._flag=2;
                }
                _taskque._wheelList[i]->process();
            }
        }   
    }
public:
    TimeManager _taskque;
};

}//end of namespace reco

#endif

