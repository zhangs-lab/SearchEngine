#include "../Include/SearchEngineServer.h"

namespace wk{

//处理连接成功的函数
void onConnection(const TcpConnectionPtr &conn){
    cout << conn->toString() << " has connected!" << endl;
}

//处理消息的函数
void onMessage(const TcpConnectionPtr &conn){
    cout << "send message!" << endl;
}

//处理连接断开的函数
void onClose(const TcpConnectionPtr &conn){
    cout << conn->toString() << " has closed!" << endl;
}

SearchEngineServer::SearchEngineServer()
:_conf(Configuration::getInstance())
,_IOthreadpool(atoi(_conf->findConfig("IOthreadNum").c_str()), atoi(_conf->findConfig("IOcapacity").c_str()))
,_calThreadpool(atoi(_conf->findConfig("calThreadNum").c_str()), atoi(_conf->findConfig("calCapacity").c_str()))
,_tcpserver(atoi(_conf->findConfig("port").c_str()), _conf->findConfig("ip"))
,_cacheManger(reco::CacheManger::getInstance())
,_webPageSearcher(WebPageSearcher())
,_protocolParser(ProtocolParser())
{}

SearchEngineServer::~SearchEngineServer()
{
    cout<<"~SEA"<<endl;
    reco::CacheManger::destory();
}

void SearchEngineServer::start(){
#if 1
    _IOthreadpool.start();
    _calThreadpool.start();
    _tcpserver.setAllCallbacks(onConnection, onMessage, onClose);
    _tcpserver.setThreadpool(&_calThreadpool, &_IOthreadpool);
    _tcpserver.setProtocol(&_protocolParser);
    _tcpserver.start();
#else
    _IOthreadpool->start();
    _calThreadpool->start();
    _tcpserver->setAllCallbacks(onConnection, onMessage, onClose);
    _tcpserver->setThreadpool(_IOthreadpool);
    _tcpserver->start();

#endif
}

void SearchEngineServer::stop(){
#if 1
    _tcpserver.stop();
    _IOthreadpool.stop();
    _calThreadpool.stop();
#else
    _tcpserver->stop();
    _IOthreadpool->stop();
    _calThreadpool->stop();

#endif
}


void SearchEngineServer::doTaskThread(const TcpConnectionPtr&, string& msg){

}

}//end of namespace wk
