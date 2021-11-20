#include "TcpConnection.hpp"

#include <string.h>

#include <sstream>
#include <iostream>

namespace wk
{

TcpConnection::TcpConnection(int fd)
: _sock(fd)
, _sockIO(fd)
, _localAddr(getLocalAddr())
, _shutdownWrite(false)
{}

TcpConnection::~TcpConnection()
{
}

string TcpConnection::receive()
{
	char buff[65536] = {0};
	_sockIO.readline(buff, sizeof(buff));
	return string(buff);
}

void TcpConnection::send(const string & msg)
{
	_sockIO.writen(msg.c_str(), msg.size());
}

InetAddress TcpConnection::getLocalAddr()
{
	struct sockaddr_in localaddr;
	socklen_t len = sizeof(localaddr);
	::memset(&localaddr, 0, sizeof(localaddr));
	if(getsockname(_sock.fd(), (struct sockaddr*)&localaddr, &len) < 0) {
		perror("getlocalsockname");
	}
	return InetAddress(localaddr);
}

void TcpConnection::setAllCallbacks(
		const TcpConnectionCallback & cb1,
		const TcpConnectionClose & cb2)
{
	_onMessageCb = std::move(cb1);
	_onCloseCb = std::move(cb2);
}

bool TcpConnection::isClosed() const
{
	int nret = -1;
	char buff[64] = {0};
	do{
		nret = ::recv(_sock.fd(), buff, sizeof(buff), MSG_PEEK);
	}while(nret == -1 && errno == EINTR);

	return nret == 0;
}


void TcpConnection::handleMessageCallback()
{
	if(_onMessageCb) {
		_onMessageCb(shared_from_this());
	}
}

void TcpConnection::handleCloseCallback()
{
	if(_onCloseCb) {
		_onCloseCb();
	}
}



string TcpConnection::toString() const
{
	std::ostringstream oss;
	oss << "tcp " 
		<< _localAddr.ip() << ":" << _localAddr.port() << " --> ";
	return oss.str();
}

}//end of namesapce wk
