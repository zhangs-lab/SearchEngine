#include "../Include/Dictionary.hpp"
#include "../Include/EditDistance.hpp"
#include <fstream>
#include <sstream>
#include <stdio.h>

using std::ifstream;
using std::istringstream;
 
namespace reco
{
//饿汉：类加载时就完成初始化，避免不必要的同步
//此处同步是为了避免多次创建实例
Dictionary* Dictionary::_pInstance=new Dictionary();

Dictionary* Dictionary::getInstance()
{
    return _pInstance;
}
Dictionary::Dictionary()
:_dict()
,_index()
{
    readDictfile("data/dict.dat");
    readIndexfile("data/dictIndex.dat");
}
void Dictionary::readDictfile(const string& filename)
{
    ifstream ifs(filename);
    if(!ifs.good())
    {
        ::perror("dict_file");
        ifs.close();
        return ;
    }

    string line;
    //getline第三个参数为终结符
    //       默认为'\n'
    while(getline(ifs,line))
    {
        string word;
        int freq;
        istringstream iss(line);
        iss>>word>>freq;

        _dict.push_back(std::make_pair(word,freq));
    }
}
void Dictionary::readIndexfile(const string& filename)
{
    ifstream ifs(filename);
    if(!ifs.good())
    {
        ::perror("index_file");
        ifs.close();
        return ;
    }

    string line;
    while(getline(ifs,line))
    {
        string word;
        istringstream iss(line);
        iss>>word;
        _index[word]=set<int>();

        int idx;
        while(iss>>idx)
        {
            _index[word].insert(idx);
        }
    }
}

vector<CandidateResult> Dictionary::doQuery(const string& word)
{
    vector<CandidateResult> vec;
    //查找索引
    set<int> indexs=getIndexs(word);

    for(set<int>::iterator it=indexs.begin();it!=indexs.end();++it)
    {
        std::pair<string,int> newWord=_dict[*it];
        /* printf("newWord = %s\n",newWord.first.c_str()); */
        int d=minDistance(word,newWord.first); 
        CandidateResult result(newWord.first,newWord.second,d);
        vec.push_back(result);
    }
    return vec;
}

#if 1
int Dictionary::minDistance(const string& word1,const string& word2)
{
    EditDistance wordLen;
    int n=wordLen.length(word1);
    int m=wordLen.length(word2);
    //特判，至少一个串为空串
    if(0==n*m) return n+m;

    //初始化边界状态
    //空串到非空串的距离一定是非空串的长度
    int DP[n+1][m+1];
    for(int i=0;i<n+1;++i){
        DP[i][0]=i;
    }
    for(int j=0;j<m+1;++j){
        DP[0][j]=j;
    }
    //求所有DP值
    for(int i=1;i<n+1;++i){
        for(int j=1;j<m+1;++j){
            int left=DP[i-1][j] + 1;
            int down=DP[i][j-1] + 1;
            //最后一个字符不相同时加一
            //word1[i-1]表示word1的第i个字母，下标为i-1，不要与DP下标混淆
            int left_down=0;
            if(wordLen.getSubWord(word1,i)!=wordLen.getSubWord(word2,j)){
                left_down=DP[i-1][j-1]+1;
            }else{
                left_down=DP[i-1][j-1];
            }

            DP[i][j]=std::min(left_down,std::min(left,down));
        }
    }
    return DP[n][m];
}
#endif

set<int> Dictionary::getIndexs(const string& word)
{
    EditDistance wordLen;
    string key="";

    map<string,set<int>>::iterator iter;
    int nBytes=wordLen.nBytesCode(word[0]);
    key=word.substr(0,nBytes);
    set<int> indexs=_index.find(key)->second;

    for(size_t idx=0;idx<word.size();++idx)
    {
        int nBytes=wordLen.nBytesCode(word[idx]);
        key=word.substr(idx,nBytes);
        idx+=nBytes-1;

        iter=_index.find(key);
        set<int> tmpIndexs=iter->second;
        mergeIndexs(indexs,tmpIndexs);
    }

    return indexs;
}
void Dictionary::mergeIndexs(set<int>& lhs,const set<int>& rhs)
{
    //特判lhs为空
    /* if(0==lhs.size()) */
    /* { */
    /*     lhs=rhs; */
    /*     return ; */
    /* } */

    set<int>::iterator it=lhs.begin();
    while(it!=lhs.end())
    {
        if(rhs.find(*it)==rhs.end()){
            //返回值：指向被删除元素的下一个元素的迭代器
            it=lhs.erase(it);
        }else{
            ++it;
        }
    }
}

}//end of namespace reco

