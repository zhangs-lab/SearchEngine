#include "../Include/Socket.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>

namespace wk{

Socket::Socket(){
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(_fd < 0){
        ::perror("socket");
    }
}

Socket::Socket(int fd)
:_fd(fd)
{

}
Socket::~Socket(){
    ::close(_fd);
}

int Socket::getFd() const{
    return _fd;
}
void Socket::shutdownWrite(){
    ::shutdown(_fd, SHUT_WR);//断开socket通信的输入流，函数在window下有点不同
    //套接字无法发送数据，但如果输出缓冲区中还有未传输的数据，则将传递到目标主机
}
void Socket::setNonblock(){
    int flags = ::fcntl(_fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    ::fcntl(_fd, F_SETFL, flags);
}

}//end of namespace wk
