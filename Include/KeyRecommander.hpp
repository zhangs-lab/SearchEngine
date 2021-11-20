#ifndef __RECO_KEYRECOMMANDER_HPP__
#define __RECO_KEYRECOMMANDER_HPP__ 

/* #include "TcpConnection.hpp" */
#include "CandidateResult.hpp"

#include <string>
#include <queue>
#include <vector>
#include <memory>
using std::string;
using std::vector;
using std::priority_queue;
using std::shared_ptr;

namespace reco
{
struct Compare                                                                    
{
    bool operator()(const CandidateResult& lhs,const CandidateResult& rhs)
    {   
        if(lhs.getDist()!=rhs.getDist()){
            //按最小编辑距离升序排列
            return lhs.getDist()>rhs.getDist();
        }else if(lhs.getFreq()!=rhs.getFreq()){
            //按出现次数降序排列
            return lhs.getFreq()<rhs.getFreq();
        }else{
            //按字典序排列
            return lhs.getWord()>rhs.getWord();
        }   
    }   
};

class KeyRecommander
{
public:
    KeyRecommander(const string&);
    vector<string> doQuery();

private:
    string getLastWord();

    
    vector<string> queryWord(const string& word);

private:
    string _str;
    vector<string> _words;
    priority_queue<CandidateResult,vector<CandidateResult>,Compare> _prique;
    string _subStr;
};

}//end of namespace reco

#endif

