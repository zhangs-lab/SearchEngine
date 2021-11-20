#include "../Include/InetAddress.h"

#include <string.h>
#include <arpa/inet.h>

namespace wk{

InetAddress::InetAddress(unsigned short port, const string &ip){
    ::memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());

}
InetAddress::InetAddress(const struct sockaddr_in & addr)
:_addr(addr)
{}

string InetAddress::getIp() const{
    return string(inet_ntoa(_addr.sin_addr));
}
unsigned short InetAddress::getPort() const{
    return ntohs(_addr.sin_port);
}

struct sockaddr_in* InetAddress::getInetAddressPtr(){
    return &_addr;
}


}//end of namespace wk
