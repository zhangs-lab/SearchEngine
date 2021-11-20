#ifndef __MYTASK_H__
#define __MYTASK_H__
#include "TcpConnection.h"
#include "Threadpool.h"
#include "ProtocolParser.h"
#include "KeyRecommander.hpp"
#include "CacheManger.hpp"
/* #include "ThreadID.hpp" */

using namespace reco;

namespace wk{

class Mytask
{
size_t nBytesCode(const char ch)
{
    if(ch&(1<<7))
    {
        int nBytes=1;
        for(int idx=0;idx!=6;++idx)
        {
            if(ch & (1 << (6-idx))){
                ++nBytes;
            }else{
                break;
            }
        }
        return nBytes;
    }
    return 1;
}

string getLastWord(string & _str)
{
    auto iter=_str.end();
    for(auto it=_str.begin();it!=_str.end();++it)                                                     
    {
        if(*it==' ') iter=it;
    }
    string str, tmp;
    if(iter == _str.end()){
        str = "";
        tmp = _str;
    }else{
        string s1(_str.begin(), iter + 1);
        string s2(iter + 1, _str.end());
        str = s1;
        tmp = s2;
    }
    int flag=0;
    int n=0;
    for(size_t idx=0;idx!=tmp.size();)
    {
        int nBytes=nBytesCode(tmp[idx]);
        if(flag!=nBytes){
            flag=nBytes;
            n=idx;
        }
        idx += nBytes;
    }

    string lastWord=tmp.substr(n,tmp.size()-n);
    str+=tmp.substr(0,n);
    _str=str;
    return lastWord;
}


public:
    
    void recommandProcess(const TcpConnectionPtr& conn, 
                          Threadpool* IOthreadpool, 
                          string& temp, 
                          ProtocolParser* p,
                          int threadID){
        int ID = 0;
        string res;
        int n = 0;
        n = p->unpack(temp, res, ID);
        string sendStr;
        if(ID == 1){
            string subStr=res;
            string lastWord=getLastWord(subStr);

            CacheManger* cacheMangerPtr=CacheManger::getInstance();
            vector<string> words = cacheMangerPtr->seekElement(threadID,lastWord);
            if(words.empty()){
                KeyRecommander keyCommander(lastWord);
                words = keyCommander.doQuery();
                cacheMangerPtr->addElement(threadID,lastWord,words);
            }

            for(auto &it: words){
                it=subStr+it;
            }
            

            for(auto &str : words){
                sendStr += str + '\t';
            }
            ID = 100;
        }else if(ID == 2){

            cout << 2 << endl;
        }
        temp = p->pack(sendStr, ID);
        IOthreadpool->addTask(std::bind(&Mytask::sendProcess, Mytask(), conn, temp));
    }

    void sendProcess(const TcpConnectionPtr& conn, string& str){
        conn->send(str);
        conn->handleMessageCallback();
    }
};

}//end of namespace wk

#endif

