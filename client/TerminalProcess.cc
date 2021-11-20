#include "TerminalProcess.h"

#include <sys/socket.h>

#include <iostream>

using std::cout;
using std::endl;

namespace wk{


TerminalProcess* TerminalProcess::_pInstance = getInstance();

TerminalProcess* TerminalProcess::getInstance(){
    if(!_pInstance){
        _pInstance = new TerminalProcess();
    }
    return _pInstance;
}

void TerminalProcess::destroy(){
    if(_pInstance){
        delete _pInstance;
    }
    _pInstance = nullptr;

}

TerminalProcess::TerminalProcess(){
    if(tcgetattr(0, &_tmOld) < 0){
        perror("tcgetattr");
        return;
    }
    cfmakeraw(&_tmNew);
}

TerminalProcess::~TerminalProcess(){
    tcsetattr(0, TCSANOW, &_tmOld);
}


int TerminalProcess::getCharacter(string& temp){
    char ch = 0;
    ch = getchar();
    int n = nBytesCode(ch);
    if(n == 3 || (ch >= 32 && ch < 127)){
        temp += ch;
        for(int i = 0; i < n - 1; ++i){
            ch = getchar();
            temp += ch;
        }
    }
    return ch;
}

void TerminalProcess::getAll(){
    char buf[1024] = {0};
    recv(0, buf, 1023, MSG_DONTWAIT);
    _str = buf;
    cout << _str << '\r' << endl;
}

bool TerminalProcess::isEmpty(){
    return !_str.size();
}

int TerminalProcess::getCharacter1(string& temp){
    char ch = 0;
    size_t index = 0;
    ch = _str[index++];
    int n = nBytesCode(ch);
    if(n == 3 || (ch >= 32 && ch < 127)){
        temp += ch;
        for(int i = 0; i < n - 1; ++i){
            ch = _str[index++];
            temp += ch;
        }
    }
    if(index < _str.size()){
        _str.substr(index);
        _str.clear();
    }else{
        _str.clear();
    }
    return ch;
}

void TerminalProcess::show(const string& temp){
    if(temp.size()){
        cout << temp;
    }
}

void TerminalProcess::show(const char c){
    cout << c;
}

void TerminalProcess::showEnter(const string& temp){
    cout << temp << '\r' << endl;
}

void TerminalProcess::showEnter(){
    cout << '\r' << endl;
}

void TerminalProcess::setOriginalMode(){
    tcsetattr(0, TCSANOW, &_tmNew);
}

void TerminalProcess::reduction(){
    tcsetattr(0, TCSANOW, &_tmOld);
}

size_t nBytesCode(const char ch){
    if(ch & (1 << 7)){
        int nBytes = 1;
        for(int idx = 0; idx != 6; ++idx){
            if(ch & (1 << (6 - idx))){
                ++nBytes;   
            }else{
                break;
            }
        }
        return nBytes;
    }
    return 1;
}

}//end of namespace wk

#if 0
#include <iostream>
using std::cout;
using std::endl;
void test0(){
    using namespace wk;
    TerminalProcess p;
    p.setOriginalMode();
    string temp;
    string res;
    char c;

    do{
        char tail_c = 0;
        if(res.size() != 0){
            tail_c = res.back();
        }
        c = p.getCharacter(temp, tail_c);
        if(c == 13){
            p.showEnter();
        }else if(c == 127 || c == 8){
            if(tail_c >> 7){
                res.pop_back();
                res.pop_back();
            }
            res.pop_back();
        }else{
            p.show(temp);
            res += temp;
            temp.clear();
        }
    }while(c != 'q');
    p.show(res);

#if 0
    while((c = p.getCharacter(temp, res.back())) != 'q'){
        if(c == 13){
            p.showEnter();
        }else if(c == 127 || c == 8){
            if(res.back() >> 7){
                res.pop_back();
                res.pop_back();
            }
            res.pop_back();
        }else{
            p.show(temp);
            res += temp;
            temp.clear();
        }
    }
#endif

}

int main(){
    test0();
    return 0;
}
#endif
