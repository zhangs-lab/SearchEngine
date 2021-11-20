#include "../Include/EventLoop.h"
#include "../Include/Acceptor.h"
#include "../Include/Mytask.h"

#include <errno.h>
#include <stdio.h>

#include <iostream>

using std::cout;
using std::endl;

namespace wk{

EventLoop::EventLoop(Acceptor& acceptor)
:_efd(createEpollFd())
,_acceptor(acceptor)
,_isLooping(false)
,_evtList(1024)
{
    addEpollReadFd(_acceptor.getFd());
}

void EventLoop::setAllCallbacks(TcpConnectionCallback && cb1,
                             TcpConnectionCallback && cb2,
                             TcpConnectionCallback && cb3){
    _onConnection = std::move(cb1);
    _onMessage = std::move(cb2);
    _onClose = std::move(cb3);
}


void EventLoop::loop(){
    _isLooping = true;
    while(_isLooping){
        waitEpollFd();
    }
}

void EventLoop::uploop(){
    _isLooping = false;
}

int EventLoop::createEpollFd(){
    int fd = ::epoll_create1(0);
    if(fd < 0){
        ::perror("epoll_create1");
    }
    return fd;
}

void EventLoop::addEpollReadFd(int fd){
    struct epoll_event value;
    value.data.fd = fd;
    value.events = EPOLLIN;
    if(::epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &value) < 0){
        perror("epoll_ctl");
    }
}

void EventLoop::delEpollReadFd(int fd){
    struct epoll_event value;
    value.data.fd = fd;
    if(::epoll_ctl(_efd, EPOLL_CTL_DEL, fd, &value) < 0){
        perror("epoll_ctl");
    }
}

void EventLoop::waitEpollFd(){
    int nready = -1;
    do{
        nready = ::epoll_wait(_efd, &*_evtList.begin(), _evtList.size(), 5000);
    }while(nready == -1 && errno == EINTR);

    if(nready == -1){
        perror("epoll_wait");
        return;
    }else if(nready == 0){
        cout << "epoll_wait timeout!" << endl;
    }else{
        if(nready == _evtList.size()){
            _evtList.resize(2 * nready);
        }

        for(int idx = 0; idx < nready; ++idx){
            int tmp = _evtList[idx].data.fd;
            if(tmp == _acceptor.getFd() && (_evtList[idx].events & EPOLLIN)){
                handleNewConnection();
            }else{
                if(_evtList[idx].events & EPOLLIN){
                    handleMessage(tmp);
                }
            }
        }
    }
}

void EventLoop::handleNewConnection(){
    int peerfd = _acceptor.accept();
    addEpollReadFd(peerfd);

    TcpConnectionPtr conn(new TcpConnection(peerfd));

    conn->setAllCallbacks(_onConnection, _onMessage, _onClose);
    _conns.insert(std::make_pair(peerfd, conn));
    conn->handleConnectionCallback();
}

void EventLoop::setThreadpool(Threadpool* calthreadpool, Threadpool* IOthreadpool){
    _calThreadpool = calthreadpool;
    _IOthreadpool = IOthreadpool;
}

void EventLoop::setProtocol(ProtocolParser* p){
    _pro = p;
}

void EventLoop::handleMessage(int fd){
    auto iter = _conns.find(fd);
    if(iter != _conns.end()){
        if(iter->second->isClosed()){
            iter->second->handleCloseCallback();
            delEpollReadFd(fd);
            _conns.erase(iter);
        }else{
            /* iter->second->doTask(); */
            /* iter->second->handleMessageCallback(); */
            string temp = iter->second->receive();
            _calThreadpool->addTask(std::bind(&Mytask::recommandProcess, 
                                           Mytask(), iter->second, 
                                           _IOthreadpool, temp, _pro
                                            ,std::placeholders::_1));
        }
    }
}

}//end of namespace wk

