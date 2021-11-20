#include "../Include/KeyRecommander.hpp"
#include "../Include/Dictionary.hpp"
#include "../Include/EditDistance.hpp"
#include "../Include/MyCppjieba.hpp"
#include <iostream>

namespace reco
{
//不使用初始化列表时，会调用默认构造函数
//初始化列表采用拷贝构造函数，
//省去了一次默认构造函数的调用
KeyRecommander::KeyRecommander(const string& str)
:_str(str)
,_prique()
{
    doQuery();
}
#if 0
//模糊版本
vector<string> KeyRecommander::doQuery()
{
    EditDistance wordLen;
    int n=wordLen.length(_str);
    string rhs=wordLen.getSubWord(_str,n);
    _words=vector<string>({rhs});

    vector<string> tmp=queryWord(*_words.begin());

    string lhs=_str;
    char ch=rhs.c_str()[0];
    for(size_t i=0;i<wordLen.nBytesCode(ch);++i)
    {
        lhs.pop_back();
    }

    for(auto &elm: tmp)
    {
        elm=lhs+elm;
        printf("%s\n",elm.c_str());
    }
    return tmp;
}
#endif
#if 0
//精准版本，使用Cppjie分词，速度太慢
vector<string> KeyRecommander::doQuery()
{
    MyCppjieba jieba;
    _words=jieba(_str);

    string ans="";
    vector<string>::iterator it=_words.end()-1;
    vector<string> tmp=queryWord(*it);//返回值 

    if(tmp.empty())//词典库中未查到关键词，返回原字符串
    {
        tmp.push_back(_str);
        return tmp;
    }

    for(it=_words.begin();it!=_words.end();++it)
    {
        if(it!=_words.end()-1) 
            ans=ans+*it;
    }
    for(auto &elm: tmp)
    {
        elm=ans+elm;
        /* printf("%s\n",elm.c_str()); */
    }
    return tmp;
}
#endif

#if 1
//手动拆分字符串，匹配最后一个word
vector<string> KeyRecommander::doQuery()
{
    string lastWord=getLastWord();

    vector<string> words=queryWord(lastWord);
    if(words.empty()){//词典库中未查到关键词，返回原字符串
        words.push_back(_str);
        return words;
    }
    for(auto &elm: words)
    {
        elm=_subStr+elm;
    }
    return words;
}
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
string KeyRecommander::getLastWord()
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
    //第一个参数指起始位置下标，第二个指子串长度
    string lastWord=tmp.substr(n,tmp.size()-n);
    str+=tmp.substr(0,n);
    _subStr=str;
    return lastWord;

}
#endif

vector<string> KeyRecommander::queryWord(const string& word)
{
    Dictionary* pInstance=Dictionary::getInstance();
    vector<CandidateResult> vec=pInstance->doQuery(word);
    vector<string> tmp;
    for(auto &elm: vec){
        _prique.push(elm);
    }

    int n=_prique.size();
    for(int i=0;i<n;++i)
    {
        if(i<10)
        {
            tmp.push_back(_prique.top().getWord());
        }
        /* printf("第%d个 = %s\n",i,_prique.top().getWord().c_str()); */
        _prique.pop();
    }
    /* _prique.clear(); */
    
    return tmp;
}


}//end of namespace reco:
