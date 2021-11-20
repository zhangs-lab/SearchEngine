#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "Noncopyable.h"

#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h"
#include "Task.h"

#include <memory>
#include <functional>
#include <iostream>

using std::cout;
using std::endl;

namespace wk{

class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallback = std::function<void(const TcpConnectionPtr&)>;



class TcpConnection : Noncopyable, public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(int fd);
    ~TcpConnection();

    string receive();
    void send(const string& msg);

    string toString() const;

    void setAllCallbacks(
                         const TcpConnectionCallback & cb1,
                         const TcpConnectionCallback & cb2,
                         const TcpConnectionCallback & cb3
                        );
    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

    bool isClosed() const;
private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();

private:
    Socket _sock;
    SocketIO _sockIO;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    bool _shutdownWrite;

    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;


};

}//end of namespace wk

#endif

