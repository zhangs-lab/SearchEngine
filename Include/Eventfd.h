#ifndef __EVENTFD_H__
#define __EVENTFD_H__

#include <functional>

namespace wk{

class Eventfd
{
public:
    using EventCallback = std::function<void()>;
    Eventfd(EventCallback && cb);
    ~Eventfd();

    void start();
    void stop();
    void wakeup();

private:
    void handleRead();
    int createEventfd();
private:
    int _evfd;
    bool _isStarted;
    EventCallback _cb;

};

}//end of namespace wk

#endif

