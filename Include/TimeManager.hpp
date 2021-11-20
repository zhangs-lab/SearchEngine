#ifndef __RECO_TIMEMANAGER_HPP__
#define __RECO_TIMEMANAGER_HPP__

#include "MyTask.hpp"
#include <vector>

using std::vector;

namespace reco
{

struct TimeManager
{
public:
    TimeManager(int timefd = 2)
    :_timefd(timefd)
    ,_flag(1)
    ,_wheelList(2*(timefd+1))
    {
        int n=2*(timefd+1);
        for(int i=0;i<n;++i)
        {
            if(i==timefd) _wheelList[i]=new UpdateLowCache();
            else if(i==n-1) _wheelList[i]=new UpdateHighCache();
            else _wheelList[i]=new WaitTask();
        }
    }
    ~TimeManager()
    {}

    int _timefd;
    int _flag;//标志现在正在进行更新的cache组
    vector<TimeTask*> _wheelList;
};

}//end of namespace reco

#endif

