#ifndef __SEARCHENGINESERVER_H__
#define __SEARCHENGINESERVER_H__
#include "Threadpool.h"
#include "TcpServer.h"
#include "WebPageSearcher.h"
#include "ProtocolParser.h"
#include "Task.h"
#include "Configuration.h"
#include "../Include/RAII.hpp"
#include "../Include/CacheManger.hpp"

/* #include <pthread.h> */
#include <functional>
#include <iostream>

using std::cout;
using std::endl;


namespace wk{

void onConnection(const TcpConnectionPtr &conn);
void onMessage(const TcpConnectionPtr &conn);
void onClose(const TcpConnectionPtr &conn);

using ThreadFunctionCallback = ::std::function<void(const TcpConnectionPtr&, const Threadpool&)>;
class SearchEngineServer
{
public:
    /* SearchEngineServer(size_t threadNum, */ 
    /*                    size_t capacity, */ 
    /*                    unsigned short port, */ 
    /*                    const string& ip = "127.0.0.1"); */
    SearchEngineServer();

    ~SearchEngineServer();
    void start();
    void stop();

    
    void doTaskThread(const TcpConnectionPtr&, string& msg);
    

private:
    Configuration* _conf;
    Threadpool _IOthreadpool;
    Threadpool _calThreadpool;
    TcpServer _tcpserver;
    /* KeyRecommander _keyRecommander; */
    reco::CacheManger* _cacheManger;
    WebPageSearcher _webPageSearcher;
    ProtocolParser _protocolParser;
};

}//end of namespace wk

#endif

