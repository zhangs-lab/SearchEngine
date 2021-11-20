#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "Acceptor.h"
#include "TcpConnection.h"
#include "EventLoop.h"

namespace wk{

class TcpServer
{
        
public:
    TcpServer(unsigned short port, const string& ip = "127.0.0.1")
    :_acceptor(port, ip)
    ,_loop(_acceptor)
    {}

    void start(){
        _acceptor.ready();
        _loop.loop();
    }
    void stop(){
        _loop.uploop();
    }
    void setAllCallbacks(
                        TcpConnectionCallback && cb1,
                        TcpConnectionCallback && cb2,
                        TcpConnectionCallback && cb3
                       ){
        _loop.setAllCallbacks(std::move(cb1), std::move(cb2), std::move(cb3));
    }
    void setThreadpool(Threadpool* calpool, Threadpool* IOpool){
        _loop.setThreadpool(calpool, IOpool);
    }
    void setProtocol(ProtocolParser* pro){
        _loop.setProtocol(pro);
    }
    

private:
    Acceptor _acceptor;
    EventLoop _loop;
};

}//end of namespace wk

#endif

