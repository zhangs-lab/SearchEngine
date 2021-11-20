#include "../Include/CacheManger.hpp"
#include "../Include/UpdateCache.hpp"
#include <iostream>
#include <functional>

namespace reco
{

CacheManger* CacheManger::_cacheManger=new CacheManger();

CacheManger* CacheManger::getInstance()
{
    //此处要添加同步机制:
    
    //若缓存池未启动
    if(!_cacheManger->_isRunning)
    {
        _cacheManger->_isRunning=true;
        _cacheManger->initCache();
        _cacheManger->_updateManger->start();//启动更新模块
    }

    return _cacheManger;
}

CacheManger::CacheManger(int timefd,int capacity)
:_capacity(capacity)
,_caches()
,_updateManger(new UpdateCache())
,_isRunning(false)
{
    //每个线程对应两个线程
    for(int i=0;i<2*capacity;++i)
    {
        LRUCache cache;//默认容量为1000
        _caches.push_back(cache);
    }
}
void CacheManger::destory()
{
    if(_cacheManger)
    {
        delete _cacheManger;
    }
    _cacheManger=nullptr;
}
CacheManger::~CacheManger()
{
    printf("~CacheManger()\n");
    _cacheManger->getCacheForUpdate(0).writeToFile();
}

void CacheManger::initCache(const string & filename)
{
    for(auto &elm: _caches)
    {
        elm.readFromFile(filename);
    }
}
LRUCache& CacheManger::getCache(size_t idx)
{
    UpdateCache* updateManger=dynamic_cast<UpdateCache*>(_updateManger);
    if(1==updateManger->_taskque._flag){
        return _caches[idx+_capacity];//第一组cache在更新
    }else{
        return _caches[idx];
    }
}
LRUCache& CacheManger::getCacheForUpdate(size_t idx)
{
    return _caches[idx];
}

void CacheManger::addElement(int pid,const string& key,const vector<string>& value)
{
    _caches[pid].addElement(key,value);
    _caches[pid+_capacity].addElement(key,value);
}
vector<string> CacheManger::seekElement(int pid,const string& key)
{
    return getCache(pid).seekElement(key);
}

void CacheManger::periodicUpdateLowCaches()
{
    int num=_cacheManger->_capacity;
    //将0号cache作为中转，先将其一致化
    LRUCache & cache0=_cacheManger->getCacheForUpdate(0);//注意此处的&
    /* printf("\nupdate\n"); */
    for(int i=1;i<num;++i)
    {
        list<std::pair<string,vector<string>>> pendingUpdateList=
            _cacheManger->getCacheForUpdate(i).getPendingUpdateList();
        for(auto &elm: pendingUpdateList)
        {
            //将其余cache的热数据全部加入0号cache
            cache0.addElement(elm.first,elm.second);
        }
    }

    //每一个cache都复制为0号cache
    for(int i=1;i<num;++i)
    {
        LRUCache & tmp=_cacheManger->getCacheForUpdate(i);//注意此处&
        tmp=cache0;
        tmp.getPendingUpdateList().clear();
    }
    cache0.getPendingUpdateList().clear();//将待更新结点信息清空
}
void CacheManger::periodicUpdateHighCaches()
{
    int num=_cacheManger->_capacity;
    //将0号cache作为中转，先将其一致化
    LRUCache & cache0=_cacheManger->getCacheForUpdate(num);//注意此处的&
    /* printf("\nupdate\n"); */
    for(int i=num+1;i<2*num;++i)
    {
        list<std::pair<string,vector<string>>> pendingUpdateList=
            _cacheManger->getCacheForUpdate(i).getPendingUpdateList();
        for(auto &elm: pendingUpdateList)
        {
            //将其余cache的热数据全部加入0号cache
            cache0.addElement(elm.first,elm.second);
        }
    }

    //每一个cache都复制为0号cache
    for(int i=num+1;i<2*num;++i)
    {
        LRUCache & tmp=_cacheManger->getCacheForUpdate(i);//注意此处&
        tmp=cache0;
        tmp.getPendingUpdateList().clear();
    }
    cache0.getPendingUpdateList().clear();//将待更新结点信息清空
}

}//end of namespace reco;
