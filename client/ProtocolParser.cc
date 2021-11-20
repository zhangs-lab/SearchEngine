#include "ProtocolParser.h"

#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::ostringstream;
using std::istringstream;
namespace wk{

ProtocolParser::ProtocolParser(){}

string ProtocolParser::pack(const string& str, int ID){
    ostringstream os;
    size_t n = str.size();
    os << n << " " << ID << " " << str << '\n';
    return os.str();
}

size_t ProtocolParser::unpack(const string& str, string& res, int& ID){
    istringstream is(str);
    size_t n = 0;
    is >> n;
    is >> ID;
    string temp;
    getline(is, temp);
    temp = temp.substr(1);
    res += temp;
    return n;
}

}//end of namespace wk

