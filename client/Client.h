#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "ProtocolParser.h"
#include "EventLoop.hpp"
#include "Noncopyable.hpp"
#include "GraphicalUserInterface.h"

/* #include <termios.h> */
/* #include <signal.h> */
/* #ifndef TIOCGWINSZ */
/* #include <sys/ioctl.h> */
/* #endif */

namespace wk{

void onMessage(const TcpConnectionPtr& conn, ProtocolParser &p);

class Client : Noncopyable
{
public:
    Client();
    ~Client();
    void start();
    void stop();

private:
    int _serverFd;
    ProtocolParser _p;
    EventLoop _loop;
    GraphicalUserInterface* _gra;
    
};

void onClose(Client& client);

}//end of namespace wk


#endif

