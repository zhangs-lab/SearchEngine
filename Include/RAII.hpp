#ifndef __RECO_RAII_HPP__
#define __RECO_RAII_HPP__ 

namespace reco
{

template <typename T>
class RAII
{
public:
    RAII(T* data)
    :_data(data)
    {}
    ~RAII()
    {
        delete _data;
    }
    //提供若干访问资源的方法
    T* operator->()
    {
        return _data;
    }
    T& operator*()
    {
        return *_data;
    }
private:
    //复制、赋值设为delete
    RAII& operator=(const RAII& rhs) = delete;
    RAII(const RAII& rhs) = delete;

private:
    T* _data;
};

}//end og namespace reco

#endif

