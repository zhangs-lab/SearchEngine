#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__

#include <netinet/in.h>
#include <string>
using std::string;

namespace wk{

class InetAddress
{
public:
    InetAddress(unsigned short port, const string &ip = "127.0.0.1");
    InetAddress(const struct sockaddr_in & addr);
    string getIp() const;
    unsigned short getPort() const;

    struct sockaddr_in* getInetAddressPtr();

private:
    struct sockaddr_in _addr;

};

}//end of namespace wk

#endif

