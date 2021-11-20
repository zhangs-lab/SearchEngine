#ifndef __WD_INETADDRESS_HPP__
#define __WD_INETADDRESS_HPP__

#include <netinet/in.h>
#include <string>
using std::string;

namespace wk
{

class InetAddress
{
public:
	InetAddress(unsigned short port, const string & ip = "127.0.0.1");
	InetAddress(const struct sockaddr_in & addr);
	string ip() const;
	unsigned short port() const;

	struct sockaddr_in * getInetAddressPtr() {	return &_addr;	}

private:
	struct sockaddr_in _addr;
};

}//end of namespace wk
 
 
 
#endif
