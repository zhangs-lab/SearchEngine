#ifndef __WD_EVENTLOOP_HPP__
#define __WD_EVENTLOOP_HPP__

#include <sys/epoll.h>

#include <map>
#include <vector>
#include <memory>
#include <functional>

namespace wk
{

class Acceptor;

class TcpConnection;
class ProtocolParser;
class GraphicalUserInterface;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
using TcpConnectionClose = std::function<void()>;

size_t nBytesCode(const char ch);
size_t length_str(const std::string &str);
size_t bytes_str(const std::string &str, size_t sLength);

class EventLoop
{
public:
	EventLoop(int fd);
    ~EventLoop();
	void loop();
	void unloop();

	void setAllCallbacks(
		TcpConnectionCallback && cb1,
		TcpConnectionClose && cb2);

    void setProtocol(ProtocolParser* pro);
private:
	void waitEpollFd();
	void handleMessage();
    void handleInputMessage(std::string& line);
    void handleSearchMessage(std::string& line);
    void addEpollReadfd(int fd);
    int createEpollFd();

private:
	int _efd;
	bool _isLooping;
	std::vector<struct epoll_event> _evtList;
    ProtocolParser* _pro;
    TcpConnectionPtr _conn;
	TcpConnectionCallback _onMessageCb;
	TcpConnectionClose _onCloseCb;
    GraphicalUserInterface* _gra;
    
};

}//end of namespace wk
 
#endif
