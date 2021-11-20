#ifndef __RECO_TIMETASK_HPP__
#define __RECO_TIMETASK_HPP__

namespace reco
{

class TimeTask
{
public:
    virtual ~TimeTask()
    {}
    virtual void process() = 0;
};

}//end of namespace reco

#endif

