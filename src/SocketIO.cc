#include "../Include/SocketIO.h"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <iostream>

namespace wk{

SocketIO::SocketIO(int fd)
:_fd(fd)
{

}

int SocketIO::readn(char * buff, int len){
    char * pbuf = buff;
    int left = len;
    int ret = 0;
    while(left > 0){
        ret = ::recv(_fd, pbuf, left, 0);
        if(ret == 0){
            break;
        }else if(ret == -1 && errno == EINTR){
            continue;
        }else if(ret == -1){
            ::perror("recv");
        }else{
            pbuf += ret;
            left -= ret;
        }
    }
    return len - left;
}

int SocketIO::readline(char * buff, int maxlen){
    char* pbuf = buff;
    int left = maxlen - 1;
    int total = 0;
    while(left > 0){
        int ret = ::recv(_fd, pbuf, left, MSG_PEEK);//MSG_PEEK复制数据到缓冲区，但是不删除内核中的数据
        if(ret == 0){
            break;
        }else if(ret == -1 && errno == EINTR){//EINTR，表示文件在读的过程中遇到了中断
            continue;
        }else if(ret == -1){
            break;
        }else{
            if(ret + total > maxlen - 1){
                ret = maxlen - 1 - total;
            }

            for(int idx = 0; idx < ret; ++idx){
                if(pbuf[idx] == '\n'){
                    int sz = idx + 1;
                    readn(pbuf, sz);
                    pbuf[sz] = '\0';
                    return sz + total;
                }
            }
            readn(pbuf, ret);
            pbuf += ret;
            left -= ret;
            total += ret;
        }
    }
    pbuf[maxlen - 1] = '\0';
    return maxlen - 1;
}

int SocketIO::writen(const char * buff, int len){
    const char * pbuf = buff;
    int left = len;
    int ret = 0;
    while(left > 0){
        ret = ::send(_fd, pbuf, left, 0);
        if(ret == -1){
            perror("send");
            break;
        }else{
            pbuf += ret;
            left -= ret;
        }
    }
    return len -left;
}

}//end of namespace wk


