#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

namespace wk{

class Noncopyable
{
protected:
    Noncopyable() {}
    ~Noncopyable() {}

    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};

}//end of namespace wk

#endif

