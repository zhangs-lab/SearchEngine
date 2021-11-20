#include "../Include/SearchEngineServer.h"
#include "../Include/Configuration.h"

using namespace wk;

void setConf(Configuration* conf){
    conf->setConfig("IOthreadNum", "3");
    conf->setConfig("IOcapacity", "4");
    conf->setConfig("calThreadNum", "3");
    conf->setConfig("calCapacity", "4");
    conf->setConfig("port", "8080");
    conf->setConfig("ip", "127.0.0.1");
}

int main()
{
    Configuration* conf = Configuration::getInstance();
    setConf(conf);

    SearchEngineServer s;
    s.start();
    return 0;
}
