#ifndef __SOCKETIO_H__
#define __SOCKETIO_H__

namespace wk{

class SocketIO
{
public:
    SocketIO(int fd);
    int readn(char * buff, int len);
    int readline(char * buff, int maxlen);
    int writen(const char * buff, int len);

private:
    int _fd;
};

}//end of namespace wk

#endif

