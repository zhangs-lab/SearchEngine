#ifndef __RECO_CACHEMANAGER_HPP__
#define __RECO_CACHEMANAGER_HPP__

#include "LRUCache.hpp"
#include <vector>

using std::vector;

namespace reco
{

class Thread;
class CacheManger
{
public:
    static CacheManger * getInstance();
    static void periodicUpdateLowCaches();//更新第一组cache
    static void periodicUpdateHighCaches();//更新第二组cache
    static void destory();

    void addElement(int pid,const string& key,const vector<string>& value);
    vector<string> seekElement(int pid,const string& key);
private:
    ~CacheManger();
    CacheManger(int timefd = 2,int capacity = 5);
    LRUCache & getCache(size_t idx);//获取某个缓存
    LRUCache & getCacheForUpdate(size_t idx);//获取某个缓存用于更新
    void initCache(const string & filename = "cache.dat");//从磁盘文件中读取缓存信息

private:
    int _capacity;
    vector<LRUCache> _caches;//一个线程对应一个缓存
    Thread* _updateManger;//负责更新cache
    bool _isRunning;

    static CacheManger* _cacheManger;
};

}//end of namespace reco

#endif
