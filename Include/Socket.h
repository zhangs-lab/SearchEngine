#ifndef __SOCKET_H__
#define __SOCKET_H__
#include "Noncopyable.h"

namespace wk{

class Socket : Noncopyable
{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();

    int getFd() const;
    void shutdownWrite();
    void setNonblock();

private:
    int _fd;
};

}//end of namespace wk

#endif

