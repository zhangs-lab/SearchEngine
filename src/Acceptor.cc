#include "../Include/Acceptor.h"

namespace wk{

Acceptor::Acceptor(unsigned short port, const string& ip)
:_listensock()
,_addr(port, ip)
{

}

void Acceptor::ready(){
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}
int Acceptor::accept(){
    int peerfd = ::accept(getFd(), nullptr, nullptr);
    if(peerfd < 0){
        perror("accept");
    }
    return peerfd;
}

int Acceptor::getFd() const{
    return _listensock.getFd();
}

void Acceptor::setReuseAddr(){
    int on = 1;
    //设置socket状态，SO_REUSEADDR表示bind（）过程中本地地址可重复使用，on表示欲设置的值
    if(setsockopt(getFd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0){
        perror("setsockopt");
    }
}
void Acceptor::setReusePort(){
    int on = 1;
    if(setsockopt(getFd(), SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on)) < 0){
        perror("setsockopt");
    }
}
void Acceptor::bind(){
    if(::bind(getFd(), (struct sockaddr*)_addr.getInetAddressPtr(), sizeof(_addr)) < 0){
        perror("bind");
    }
}
void Acceptor::listen(){
    if(::listen(getFd(), 128) < 0){
        perror("listen");
    }
}

}//end of namespace wk
