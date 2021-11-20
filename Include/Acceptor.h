#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "InetAddress.h"
#include "Socket.h"

namespace wk{

class Acceptor
{
public:
    Acceptor(unsigned short port, const string& ip = "127.0.0.1");

    void ready();
    int accept();

    int getFd() const;

private:
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();

private:
    Socket _listensock;
    InetAddress _addr;
};

}//end of namespace wk

#endif

