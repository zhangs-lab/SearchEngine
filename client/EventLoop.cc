#include "EventLoop.hpp"
#include "TcpConnection.hpp"
#include "ProtocolParser.h"
#include "TerminalProcess.h"
#include "GraphicalUserInterface.h"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

namespace wk
{

EventLoop::EventLoop(int fd)
: _efd(createEpollFd())
, _isLooping(false)
, _evtList(1024)
, _conn(new TcpConnection(fd))
, _gra(GraphicalUserInterface::getInstance())
{
    addEpollReadfd(fd);
    addEpollReadfd(STDIN_FILENO);
}

EventLoop::~EventLoop(){
    GraphicalUserInterface::destroy();
}

void EventLoop::loop()
{

    _conn->setAllCallbacks(_onMessageCb, _onCloseCb);
	_isLooping = true;
	while(_isLooping){
		waitEpollFd();
	}
}

void EventLoop::unloop()
{
	_isLooping = false;
}
	
void EventLoop::setAllCallbacks(
		TcpConnectionCallback && cb1,
		TcpConnectionClose && cb2)
{
	_onMessageCb = std::move(cb1);
	_onCloseCb = std::move(cb2);
}

//一个函数的实现最多不要超过50行
void EventLoop::waitEpollFd()
{
	int nready = -1;
	do {
		nready = ::epoll_wait(_efd, &*_evtList.begin(), _evtList.size(), -1);
	}while(nready == -1 && errno == EINTR);

	if(nready == -1) {
		perror("epoll_wait");
		return;
	} else if(nready == 0) {
		printf("epoll_wait timeout!\r\n");
	} else {
		//考虑扩容的变化	
		if(nready == _evtList.size()) {
			_evtList.resize(2 * nready);
		}

		for(int idx = 0; idx < nready; ++idx) {
			int tmp = _evtList[idx].data.fd;
			if(tmp == STDIN_FILENO && 
			   (_evtList[idx].events & EPOLLIN)) {

                string temp;
                int n = 0;

#if 0
                int nret = -1;
                while(nret != 0){
                    temp.clear();
                    n =_gra->write(temp);
                    char buff[64] = {0};
                    do{
                        nret = ::recv(STDIN_FILENO, buff, sizeof(buff), MSG_PEEK);
                    }while(nret == -1 && errno == EINTR);
                }
#else
                n =_gra->write(temp);
#endif
                if(n == -1){
                    unloop();
                }else if (n == 1){

                }else if(temp.size() > 0){
                    handleInputMessage(temp);
                }
			}else{
				if(_evtList[idx].events & EPOLLIN) {
					handleMessage();
				}
			}
		}
	}
    fflush(stdout);
}

void EventLoop::addEpollReadfd(int fd){
    struct epoll_event value;
    value.data.fd = fd;
    value.events = EPOLLIN;
    if(::epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &value) < 0){
        perror("epoll_ctl");
    }
}

int EventLoop::createEpollFd(){
    int fd = ::epoll_create1(0);
    if(fd < 0){
        ::perror("epoll_create1");
    }
    return fd;
}

void EventLoop::setProtocol(ProtocolParser* pro){
    _pro = pro;
}

void EventLoop::handleMessage()
{
    if(_conn->isClosed()){
        _conn->handleCloseCallback();
    }else{
        _conn->handleMessageCallback();
    }
}

void EventLoop::handleInputMessage(string& line){
    if(line.back() == '\n'){
        if(length_str(line) > 64){                                                            
            cout << "you enter more than 64 characters!"
                << "Only the first 64 characters are valid!" << endl;
            line = line.substr(0, bytes_str(line, 64));
        }
        int ID = 2;
        line = _pro->pack(line, ID);
        _conn->send(line);
    }else{
        int ID = 1;
        line = _pro->pack(line, ID);
        _conn->send(line);
    }

}


#if 0
size_t nBytesCode(const char ch){
    if(ch & (1 << 7)){
        int nBytes = 1;
        for(int idx = 0; idx != 6; ++idx){
            if(ch & (1 << (6 - idx))){
                ++nBytes;   
            }else{
                break;
            }
        }
        return nBytes;
    }
    return 1;
}
#endif
size_t length_str(const string &str){
    size_t ilen = 0;
    size_t idx;
    for(idx = 0; idx != str.size(); ++idx){
        int nBytes = nBytesCode(str[idx]);
        idx += (nBytes - 1);
        ++ilen;
    }
    return ilen;
}

size_t bytes_str(const string &str, size_t sLength){
    size_t temp = 0;
    for(size_t idx = 0; idx < sLength; ++idx){
        int nBytes = nBytesCode(str[temp]);
        temp += nBytes;
    }
    return temp;
}

}//end of namespace wk
