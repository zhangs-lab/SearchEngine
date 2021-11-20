#ifndef __RECO_CANDIDATERESULT_HPP__
#define __RECO_CANDIDATERESULT_HPP__

#include <string>
using std::string;

namespace reco
{

class CandidateResult
{
friend struct Compare;
public:
    CandidateResult(const string& word,const int& freq,const int& dist)
    :_word(word)
    ,_freq(freq)
    ,_dist(dist)
    {}
    string getWord() const{
        return _word;
    }
    int getFreq() const{
        return _freq;
    }
    int getDist() const{
        return _dist;
    }

private:
    string _word;
    int _freq;
    int _dist;
};

}//end of namespace reco

#endif

