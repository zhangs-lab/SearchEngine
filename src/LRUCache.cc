#include "../Include/LRUCache.hpp"
#include <fstream>
#include <sstream>
using std::ifstream;
using std::ofstream;
using std::stringstream;

namespace reco
{

LRUCache::LRUCache(const size_t & capacity)
:_capacity(capacity)
{}
LRUCache::LRUCache(const LRUCache & cache)
:_hashMap()
,_resultsList(cache._resultsList)
,_pendingUpdateList(cache._pendingUpdateList)
,_capacity(cache._capacity)
{
    //【bug】此处的迭代器不能直接复制
    _hashMap.clear();
    auto iter=_resultsList.begin();
    for(;iter!=_resultsList.end();++iter)
    {
        _hashMap[iter->first]=iter;
    }
}
LRUCache& LRUCache::operator=(const LRUCache& rhs)
{
    /* this->_hashMap=rhs._hashMap;//error迭代器不能直接复制 */
    this->_resultsList=rhs._resultsList;

    _hashMap.clear();
    auto iter=_resultsList.begin();
    for(;iter!=_resultsList.end();++iter)
    {
        _hashMap[iter->first]=iter;
    }
    this->_pendingUpdateList=rhs._pendingUpdateList;
    this->_capacity=rhs._capacity;
    return *this;
}

void LRUCache::readFromFile(const string& filename)
{
    ifstream ifs(filename);
    string line;
    while(getline(ifs,line))
    {
        string key,word;
        vector<string> words;

        stringstream iss(line);
        iss>>key;
        while(iss>>word)
        {
            words.push_back(word);
        }
        _resultsList.push_front(std::make_pair(key,words));
        _hashMap[key]=_resultsList.begin();
    }
    ifs.close();
}
void LRUCache::writeToFile(const string& filename)
{
    ofstream ofs(filename);

    auto it = _resultsList.end();
    do{
        --it;
        ofs<<it->first;
        for(auto &elm: it->second)
        {
            ofs<<" "<<elm;
        }
        ofs<<std::endl;
    }while(it!=_resultsList.begin());

    ofs.close();
}

vector<string> LRUCache::seekElement(const string& key)
{
    vector<string> words;
    auto iter=_hashMap.find(key);
    if(iter!=_hashMap.end()){
        //成功找到key，移动其位置，变为最热数据
        words=iter->second->second;
        
        _resultsList.splice(_resultsList.begin(),
                _resultsList, iter->second);
        /* _resultsList.erase(iter->second); */
        /* _resultsList.push_front(std::make_pair(key,word)); */

        _hashMap[key]=_resultsList.begin();
        
        printf("word[0] = %s, list size:%ld\n",words[0].c_str(), _resultsList.size());
    }
    return words;
}
void LRUCache::addElement(const string& key,vector<string> value)
{
    //若容量已达上限，要将尾部冷数据移除
    if(_resultsList.size()==_capacity){
        string key=(--_resultsList.end())->first;
        printf("key = %s\n",key.c_str());
        _resultsList.pop_back();

        auto iter=_hashMap.find(key);
        _hashMap.erase(iter);
    }

    printf("add %s\n",key.c_str());

    _resultsList.push_front(std::make_pair(key,value));
    _hashMap[key]=_resultsList.begin();
    _pendingUpdateList.push_front(std::make_pair(key,value));
}
void LRUCache::update(const LRUCache& rhs)
{
    *this=rhs;
}
    
list<std::pair<string,vector<string>>> & LRUCache::getPendingUpdateList()
{
    return _pendingUpdateList;
}

}//end of namespace reco
 
