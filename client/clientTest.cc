#include "ProtocolParser.h"
#include "Client.h"
 
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#ifndef TIOCGWINSZ
#include <sys/ioctl.h>
#endif

#include <iostream>
#include <string>
using std::cin;
using std::cout;
using std::endl;
using std::string;
#if 0
size_t nBytesCode(const char ch)
{
	if(ch & (1 << 7))
	{
		int nBytes = 1;
		for(int idx = 0; idx != 6; ++idx)
		{
			if(ch & (1 << (6 - idx)))
			{
				++nBytes;	
			}
			else
				break;
		}
		return nBytes;
	}
	return 1;
}  
  
size_t length_str(const string &str)
{
	size_t ilen = 0;
    size_t idx;
	for(idx = 0; idx != str.size(); ++idx)
	{
		int nBytes = nBytesCode(str[idx]);
		idx += (nBytes - 1);
		++ilen;
	}
	return ilen;
}

size_t bytes_str(const string &str, size_t sLength){
    size_t temp = 0;
    for(size_t idx = 0; idx < sLength; ++idx){
        int nBytes = nBytesCode(str[temp]);
        temp += nBytes;
    }
    return temp;
}
 
void test0() 
{
    using namespace wk;
	int clientfd = ::socket(AF_INET, SOCK_STREAM, 0);
	if(clientfd < 0) {
		perror("socket");
		return ;
	}
    int ret = 0;

	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t length = sizeof(serverAddr);

	if(::connect(clientfd,(struct sockaddr*)&serverAddr, length) < 0) {
		perror("connect");
		close(clientfd);
		return;
	}
	printf("conn has connected!...\n");

	while(1) {
		string line;
		cout << ">> pls input some message:";
		//1. 客户端先发数据
		getline(cin, line);
        if(length_str(line) > 64){
            cout << "you enter more than 64 characters!"
                 << "Only the first 64 characters are valid!" << endl;
            line = line.substr(0, bytes_str(line, 64));
        }
        int ID = 1;
        ProtocolParser p;
        line = p.pack(line, ID);
        
		ret = send(clientfd, line.data(), line.size(), 0);

		char buff[1024] = {0};
		ret = recv(clientfd, buff, sizeof(buff), 0);

		printf("recv msg from server: %s\n", buff);
	}

	close(clientfd);
} 
#endif
void pr_winsize(int fd){
    struct winsize size;
    ioctl(fd, TIOCGWINSZ, (&size));
    printf("%d rows, %d columns\r\n", size.ws_row, size.ws_col);
}

void sig_winch(int signo){
    printf("SIGWINCH received\r\n");
    pr_winsize(STDIN_FILENO);
}

using namespace wk;
void test1(){
    Client client;
    client.start();
}
 
int main(void)
{
    test1();
	return 0;
}
