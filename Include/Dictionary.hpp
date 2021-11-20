#ifndef __RECO_DICTIONARY_HPP__
#define __RECO_DICTIONARY_HPP__

#include "CandidateResult.hpp"

#include <vector>
#include <map>
#include <set>
#include <string>
using std::string;
using std::set;
using std::map;
using std::vector;

namespace reco
{

class Dictionary
{
public:
    vector<CandidateResult> doQuery(const string&);
    static Dictionary* getInstance();

private:
    Dictionary();
    void readDictfile(const string&);
    void readIndexfile(const string&);

    int minDistance(const string&,const string&);
    set<int> getIndexs(const string&);
    void mergeIndexs(set<int>& lhs,const set<int>& rhs);

private:
    static Dictionary* _pInstance;
    vector<std::pair<string,int>> _dict;
    map<string,set<int>> _index;
};

}//end of namespace reco

#endif

