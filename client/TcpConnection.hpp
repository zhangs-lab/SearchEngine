#ifndef __WD_TCPCONNECTION_HPP__
#define __WD_TCPCONNECTION_HPP__

#include "Noncopyable.hpp"

#include "Socket.hpp"
#include "SocketIO.hpp"
#include "InetAddress.hpp"

#include <memory>
#include <functional>

namespace wk
{

class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
using TcpConnectionClose = std::function<void()>;

class TcpConnection 
: Noncopyable
, public std::enable_shared_from_this<TcpConnection>
{
public:
	TcpConnection(int fd);
	~TcpConnection();

	string receive();
	void send(const string & msg);

	string toString() const;

	void setAllCallbacks(
		const TcpConnectionCallback & cb1,
		const TcpConnectionClose & cb2);

	void handleMessageCallback();
	void handleCloseCallback();

	bool isClosed() const;

private:
	InetAddress getLocalAddr();

private:
	Socket _sock;
	SocketIO _sockIO;
	InetAddress _localAddr;
	bool _shutdownWrite;

	TcpConnectionCallback _onMessageCb;
	TcpConnectionClose _onCloseCb;
};

}//end of namespace wk
 
#endif
