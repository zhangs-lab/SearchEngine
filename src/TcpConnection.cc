#include "../Include/TcpConnection.h"

#include <string.h>

#include <sstream>

namespace wk{

TcpConnection::TcpConnection(int fd)
:_sock(fd)
,_sockIO(fd)
,_localAddr(getLocalAddr())
,_peerAddr(getPeerAddr())
,_shutdownWrite(false)
{
    
}

TcpConnection::~TcpConnection(){

}

string TcpConnection::receive(){
    char buff[65536] = {0};
    _sockIO.readline(buff, sizeof(buff));
    return string(buff);
}
void TcpConnection::send(const string& msg){
    _sockIO.writen(msg.c_str(), msg.size());
}

string TcpConnection::toString() const{
    std::ostringstream oss;
    oss << "tcp "
        << _localAddr.getIp() << ":" << _localAddr.getPort() << " --> "
        << _peerAddr.getIp() << ":" << _peerAddr.getPort();
    return oss.str();
}

InetAddress TcpConnection::getLocalAddr(){
    struct sockaddr_in localaddr;
    socklen_t len = sizeof(localaddr);
    ::memset(&localaddr, 0, sizeof(localaddr));
    //getsockname 可以获得一个与socket相关的地址
    if(getsockname(_sock.getFd(), (struct sockaddr*)&localaddr, &len) < 0){
        perror("getsockname");
    }
    return InetAddress(localaddr);
}

void TcpConnection::setAllCallbacks(
                     const TcpConnectionCallback & cb1,
                     const TcpConnectionCallback & cb2,
                     const TcpConnectionCallback & cb3
                    ){
    _onConnectionCb = std::move(cb1);
    _onMessageCb = std::move(cb2);
    _onCloseCb = std::move(cb3);
}

void TcpConnection::handleConnectionCallback(){
    if(_onConnectionCb){
        _onConnectionCb(shared_from_this());
    }
}

void TcpConnection::handleMessageCallback(){
    if(_onMessageCb){
        _onMessageCb(shared_from_this());
    }
}

void TcpConnection::handleCloseCallback(){
    if(_onCloseCb){
        _onCloseCb(shared_from_this());
    }
}

bool TcpConnection::isClosed() const{
    int nret = -1;
    char buff[64] = {0};
    do{
        nret = ::recv(_sock.getFd(), buff, sizeof(buff), MSG_PEEK);
    }while(nret == -1 && errno == EINTR);

    return nret == 0;
}

InetAddress TcpConnection::getPeerAddr(){
    struct sockaddr_in localaddr;
    socklen_t len = sizeof(localaddr);
    ::memset(&localaddr, 0, sizeof(localaddr));
    //获得socket中对方的地址
    if(getpeername(_sock.getFd(), (struct sockaddr*)& localaddr, &len) < 0){
        perror("getsockname");
    }
    return InetAddress(localaddr);
}

}//end of namespace wk
