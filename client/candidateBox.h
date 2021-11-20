#ifndef __CANDIDATEBOX_H__
#define __CANDIDATEBOX_H__
#include "winComponent.h"

#include <vector>
#include <sstream>

using std::vector;

namespace wk{

class candidateBox : public winComponent
{
public:
    candidateBox(size_t left, size_t top) 
    :winComponent(left, top, 10, 20)
    {}
    ~candidateBox() {}

    void show() override{

    }


    string getContent() override{
        string temp;
        for(auto &ele : _dict){
            temp += ele;
            temp += ' ';
        }
        return temp;
    }
    void setContent(const string& str)override{
        std::istringstream is(str);
        _dict.clear();
        while(is.good()){
            string temp;
            is >> temp;
            _dict.push_back(temp);
        }
    }
    

private:
    vector<string> _dict;

};

}//end of namespace wk

#endif

