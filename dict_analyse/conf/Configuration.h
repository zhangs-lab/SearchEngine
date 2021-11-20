#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

class Configuration
{
public:
    static Configuration* getInstance(){
        if(!_pInstance){
            _pInstance = new Configuration();
        }
        return _pInstance;
    }

    static void destroy(){

    }
    
    Configuration(Configuration &) = delete;
    void operator=(const Configuration&) = delete;

private:
    Configuration() {}
    ~Configuration() {}

private:
    static Configuration* _pInstance;
};

Configuration* Configuration::_pInstance = getInstance();



#endif

