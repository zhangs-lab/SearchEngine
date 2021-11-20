#include "../Include/Configuration.h"

namespace wk{

Configuration* Configuration::_pInstance = getInstance();

Configuration* Configuration::getInstance(){
    if(!_pInstance){
        _pInstance = new Configuration();
    }
    return _pInstance;
}
void Configuration::setConfig(const string& key, const string& value){
    _confData.insert({key, value});
}

string Configuration::findConfig(const string& key){
    auto iter = _confData.find(key);
    string temp;
    if(iter != _confData.end()){
        temp = iter->second;
    }
    return temp;
}
 

}//end of namespace wk
