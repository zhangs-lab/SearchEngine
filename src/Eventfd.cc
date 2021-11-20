#include "../Include/Eventfd.h"

#include <unistd.h>
#include <poll.h>
#include <sys/eventfd.h>

#include <iostream>

using std::cout;
using std::endl;

namespace wk{

Eventfd::Eventfd(EventCallback && cb)
:_evfd(createEventfd())
,_isStarted(false)
,_cb(std::move(cb))
{}

Eventfd::~Eventfd(){
    close(_evfd);
}
void Eventfd::start(){
    _isStarted = true;

    struct pollfd pfd;
    pfd.fd = _evfd;
    pfd.events = POLLIN;

    while(_isStarted){
        int nready = ::poll(&pfd, 1, 5000);
        if(nready == -1 && errno == EINTR){
            continue;
        }else if(nready == -1){
            perror("poll");
            return;
        }else if(nready == 0){
            cout << "poll timeout!" << endl;
        }else{
            if(pfd.revents & POLLIN){
                handleRead();
                if(_cb){
                    _cb();
                }
            }
        }
    }
}

void Eventfd::stop(){
    _isStarted = false;
}

void Eventfd::wakeup(){
    uint64_t one = 1;
    int ret = ::write(_evfd, &one, sizeof(one));
    if(ret != sizeof(one)){
        perror("write");
    }
}

void Eventfd::handleRead(){
    uint64_t howmany = 1 << 31;
    int ret = ::read(_evfd, &howmany, sizeof(howmany));
    if(ret != sizeof(howmany)){
        perror("read");
    }
}

int Eventfd::createEventfd(){
    int fd = ::eventfd(0, 0);
    if(fd < 0){
        perror("eventfd");
    }
    return fd;
}

}// end of namespace wk

