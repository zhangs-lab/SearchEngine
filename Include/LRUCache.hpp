#ifndef __RECO_LRUCACHE_HPP__
#define __RECO_LRUCACHE_HPP__

#include <unordered_map>
#include <list>
#include <string>
#include <vector>
#include <iterator>
using std::unordered_map;
using std::list;
using std::string;
using std::vector;
using std::iterator;

namespace reco
{

using Iterator = list<std::pair<string,vector<string>>>::iterator;
class LRUCache
{
public:
    LRUCache(const size_t & capacity = 10);
    LRUCache(const LRUCache & cache);
    LRUCache& operator=(const LRUCache& rhs);

    void readFromFile(const string& filename = "cache.dat");//从文件读取缓存数据
    void writeToFile(const string& filename = "cache.dat");//将缓存信息写入文件

    vector<string> seekElement(const string& key);//查找cache中的key
    void addElement(const string& key,vector<string> value);//向缓存添加数据
    void update(const LRUCache& rhs);//更新缓存信息
    list<std::pair<string,vector<string>>> & getPendingUpdateList();//获得更新的节点信息

private:
    unordered_map<string,Iterator> _hashMap;//key-->iterator
    list<std::pair<string,vector<string>>> _resultsList;//保存键值对
    list<std::pair<string,vector<string>>> _pendingUpdateList;//待更新节点信息
    size_t _capacity;//缓存节点容量
};

}//end of namespace reco

#endif

