#ifndef __PROTOCOLPARSER_H__
#define __PROTOCOLPARSER_H__

#include <string>

using std::string;

namespace wk{

class ProtocolParser
{
public:
    ProtocolParser();

    string pack(const string& str, int ID); 
    size_t unpack(const string& str, string& res, int& ID);

};

}//end of namespace wk

#endif

