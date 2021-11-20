#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <unordered_map>
#include <string>

using std::string;
using std::unordered_map;

namespace wk{

class Configuration
{
public:
    Configuration(Configuration &othre) = delete;
    void operator=(const Configuration&) = delete;

    static Configuration *getInstance();

    void setConfig(const string& key, const string& value);
    string findConfig(const string& key);
    
    
protected:
    Configuration(){}
    ~Configuration(){}

private:
    static Configuration* _pInstance;
    unordered_map<string, string> _confData;
};

}//end of namespace wk

#endif

