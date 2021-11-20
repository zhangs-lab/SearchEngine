#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__
#include "Task.h"
#include "Threadpool.h"

#include <sys/epoll.h>

#include <functional>
#include <memory>
#include <map>
#include <vector>



namespace wk{
class ProtocolParser;
class Acceptor;
class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallback = std::function<void(const TcpConnectionPtr&)>;


class EventLoop
{
public:
    EventLoop(Acceptor&);
    void setAllCallbacks(TcpConnectionCallback && cb1,
                      TcpConnectionCallback && cb2,
                      TcpConnectionCallback && cb3);
    void setThreadpool(Threadpool* calThreadpool, Threadpool* IOthreadpool);
    void setProtocol(ProtocolParser* p);
    void loop();
    void uploop();
private:
    int createEpollFd();
    void addEpollReadFd(int fd);
    void delEpollReadFd(int fd);

    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int);

private:
    int _efd;
    Acceptor& _acceptor;
    bool _isLooping;
    std::map<int, TcpConnectionPtr> _conns;
    std::vector<struct epoll_event> _evtList;
    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;
    Threadpool* _calThreadpool;
    Threadpool* _IOthreadpool;
    ProtocolParser* _pro;
};

}//end of namespace wk

#endif

