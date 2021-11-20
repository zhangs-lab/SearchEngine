#include "Client.h"
#include "TcpConnection.hpp"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <sstream>

using std::cout;
using std::endl;

namespace wk{

void onMessage(const TcpConnectionPtr& conn, ProtocolParser &p){
    string rec = conn->receive();
    int ID = 0;
    size_t n = 0;
    string res;
    n = p.unpack(rec, res, ID);
    while(res.size() < n){
        string recv = conn->receive();
        res += recv;
    }
    //关键字信息
    vector<string> recommand;
    if(ID == 100){
        std::istringstream iss(res);
        string temp;
        /* cout << res ; */
        while(getline(iss, temp, '\t').good()){
            /* cout << temp << " "; */
            recommand.push_back(temp);
            temp.clear();
            if(recommand.size() == 10){
                break;
            }
        }
        /* cout << '\r' << endl; */
        (GraphicalUserInterface::getInstance())->recommand(recommand);
    }else if(ID == 200){

    }
}

void onClose(Client& client){
    system("clear");
    cout << "server closed!" << '\r' << endl;
    client.stop();
}

void sig_winch(int signo){

}

Client::Client()
:_serverFd(::socket(AF_INET, SOCK_STREAM, 0))
,_p()
,_loop(_serverFd)
{
    if(isatty(STDIN_FILENO) == 0){
        exit(1);
    }
    if(_serverFd < 0){
        perror("socket");
        exit(1);
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t length = sizeof(serverAddr);

    if(::connect(_serverFd, (struct sockaddr*)&serverAddr, length) < 0){
        close(_serverFd);
        GraphicalUserInterface::destroy();
        perror("connect");
        exit(1);
    }
}

Client::~Client(){
    close(_serverFd);
    GraphicalUserInterface::destroy();
}

void Client::start(){
    _gra = GraphicalUserInterface::getInstance();
    _gra->clear();
    _gra->setTools(SEARCHBAR);
    /* _gra->showAll(); */
    fflush(stdout);
    _loop.setAllCallbacks(std::bind(onMessage, std::placeholders::_1, std::ref(_p)), std::bind(onClose, std::ref(*this)));
    _loop.setProtocol(&_p);
    _loop.loop();
}

void Client::stop(){
    _loop.unloop();
}


}//end of namespace wk
